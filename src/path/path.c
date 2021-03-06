/* -*- c-set-style: "K&R"; c-basic-offset: 8 -*-
 *
 * This file is part of PRoot.
 *
 * Copyright (C) 2010, 2011, 2012 STMicroelectronics
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#include <string.h>    /* string(3), */
#include <stdarg.h>    /* va_*(3), */
#include <assert.h>    /* assert(3), */
#include <stdlib.h>    /* realpath(3), free(3), */
#include <unistd.h>    /* readlink*(2), *stat(2), getpid(2), */
#include <sys/types.h> /* pid_t, */
#include <sys/stat.h>  /* S_ISDIR, */
#include <dirent.h>    /* opendir(3), readdir(3), */
#include <stdio.h>     /* sprintf(3), */
#include <errno.h>     /* E*, */
#include <stddef.h>    /* ptrdiff_t, */

#include "path/path.h"
#include "path/binding.h"
#include "path/canon.h"
#include "notice.h"
#include "config.h"
#include "build.h"

#include "compat.h"

static int initialized = 0;
char root[PATH_MAX];
size_t root_length;

/**
 * Copy in @component the first path component pointed to by @cursor,
 * this later is updated to point to the next component for a further
 * call. This function returns:
 *
 *     - -errno if an error occured.
 *
 *     - FINAL_FORCE_DIR if it the last component of the path but we
 *       really expect a directory.
 *
 *     - FINAL_NORMAL if it the last component of the path.
 *
 *     - 0 otherwise.
 */
int next_component(char component[NAME_MAX], const char **cursor)
{
	const char *start;
	ptrdiff_t length;
	int want_dir;

	/* Sanity checks. */
	assert(component != NULL);
	assert(cursor    != NULL);

	/* Skip leading path separators. */
	while (**cursor != '\0' && **cursor == '/')
		(*cursor)++;

	/* Find the next component. */
	start = *cursor;
	while (**cursor != '\0' && **cursor != '/')
		(*cursor)++;
	length = *cursor - start;

	if (length >= NAME_MAX)
		return -ENAMETOOLONG;

	/* Extract the component. */
	strncpy(component, start, length);
	component[length] = '\0';

	/* Check if a [link to a] directory is expected. */
	want_dir = (**cursor == '/');

	/* Skip trailing path separators. */
	while (**cursor != '\0' && **cursor == '/')
		(*cursor)++;

	if (**cursor == '\0')
		return (want_dir
			? FINAL_FORCE_DIR
			: FINAL_NORMAL);

	return 0;
}

/**
 * Put an end-of-string ('\0') right before the last component of @path.
 */
void pop_component(char *path)
{
	int offset;

	/* Sanity checks. */
	assert(path != NULL);

	offset = strlen(path) - 1;
	assert(offset >= 0);

	/* Don't pop over "/", it doesn't mean anything. */
	if (offset == 0) {
		assert(path[0] == '/' && path[1] == '\0');
		return;
	}

	/* Skip trailing path separators. */
	while (offset > 1 && path[offset] == '/')
		offset--;

	/* Search for the previous path separator. */
	while (offset > 1 && path[offset] != '/')
		offset--;

	/* Cut the end of the string before the last component. */
	path[offset] = '\0';
	assert(path[0] == '/');
}

/**
 * Copy in @result the concatenation of several paths (@number_paths)
 * and adds a path separator ('/') in between when needed. This
 * function returns -errno if an error occured, otherwise it returns 0.
 */
int join_paths(int number_paths, char result[PATH_MAX], ...)
{
	va_list paths;
	size_t length;
	int i;

	result[0] = '\0';

	/* Parse the list of variadic arguments. */
	va_start(paths, result);
	length = 0;
	for (i = 0; i < number_paths; i++) {
		const char *path;
		int need_separator;
		size_t old_length;

		path = va_arg(paths, const char *);
		if (path == NULL)
			continue;

		/* Check if a path separator is needed. */
		if (length > 0 && result[length - 1] != '/' && path[0] != '/')
			need_separator = 1;
		else if (length > 0 && result[length - 1] == '/' && path[0] == '/')
			need_separator = -1;
		else
			need_separator = 0;

		old_length = length;
		length += strlen(path) + need_separator;
		if (length >= PATH_MAX) {
			va_end(paths);
			return -ENAMETOOLONG;
		}

		if (need_separator == -1) {
			path++;
		}
		else if (need_separator == 1) {
			strcat(result + old_length, "/");
			old_length++;
		}
		strcat(result + old_length, path);
	}
	va_end(paths);

	return 0;
}

/**
 * Initialize internal data of the path translator.
 */
void init_module_path()
{
	assert(strlen(config.guest_rootfs) < PATH_MAX);
	strcpy(root, config.guest_rootfs);

	if (strcmp(root, "/") == 0)
		root_length = 0;
	else
		root_length = strlen(root);
	initialized = 1;

	init_bindings();
}

/**
 * Check if the translation should be delayed.
 *
 * It is useful when using a runner that needs shared libraries or
 * reads some configuration files, for instance.
 */
static int is_delayed(struct tracee_info *tracee, const char *path)
{
	if (tracee->trigger == NULL)
		return 0;

	if (strcmp(tracee->trigger, path) != 0)
		return 1;

	free(tracee->trigger);
	tracee->trigger = NULL;

	return 0;
}

/**
 * Copy in @result the equivalent of @root + canonicalize(@dir_fd +
 * @fake_path).  If @fake_path is not absolute then it is relative to
 * the directory referred by the descriptor @dir_fd (AT_FDCWD is for
 * the current working directory).  See the documentation of
 * canonicalize() for the meaning of @deref_final.
 */
int translate_path(struct tracee_info *tracee, char result[PATH_MAX], int dir_fd, const char *fake_path, int deref_final)
{
	char link[32]; /* 32 > sizeof("/proc//cwd") + sizeof(#ULONG_MAX) */
	char tmp[PATH_MAX];
	int status;
	pid_t pid;

	assert(initialized != 0);

	pid = (tracee != NULL ? tracee->pid : getpid());

	/* Use "/" as the base if it is an absolute [fake] path. */
	if (fake_path[0] == '/') {
		strcpy(result, "/");
	}
	/* It is relative to the current working directory or to a
	 * directory referred by a descriptors, see openat(2) for
	 * details. */
	else {
		struct stat statl;

		/* Format the path to the "virtual" link. */
		if (dir_fd == AT_FDCWD)
			status = sprintf(link, "/proc/%d/cwd", pid);
		else
			status = sprintf(link, "/proc/%d/fd/%d", pid, dir_fd);
		if (status < 0)
			return -EPERM;
		if (status >= sizeof(link))
			return -EPERM;

		/* Read the value of this "virtual" link. */
		status = readlink(link, result, PATH_MAX);
		if (status < 0)
			return -EPERM;
		if (status >= PATH_MAX)
			return -ENAMETOOLONG;
		result[status] = '\0';

		if (dir_fd != AT_FDCWD) {
			/* Ensure it points to a directory. */
			status = stat(result, &statl);
			if (!S_ISDIR(statl.st_mode))
				return -ENOTDIR;
		}

		/* Remove the leading "root" part of the base
		 * (required!). */
		status = detranslate_path(result, true, true);
		if (status < 0)
			return status;
	}

	VERBOSE(4, "pid %d: translate(\"%s\" + \"%s\")", pid, result, fake_path);

	/* Canonicalize regarding the new root. */
	status = canonicalize(pid, fake_path, deref_final, result, 0);
	if (status < 0)
		return status;

	/* Don't use the result of the canonicalization if the
	 * translation is delayed, use the origin input path instead. */
	if (config.qemu && tracee != NULL && is_delayed(tracee, fake_path)) {
		if (strlen(fake_path) >= PATH_MAX)
			return -ENAMETOOLONG;
		strcpy(result, fake_path);
		goto end;
	}

	/* Don't prepend the new root to the result of the
	 * canonicalization if it is a binding, instead substitute the
	 * binding location (leading part) with the real path.*/
	if (substitute_binding(BINDING_LOCATION, result) >= 0)
		goto end;

	strcpy(tmp, result);
	status = join_paths(2, result, root, tmp);
	if (status < 0)
		return status;

	/* Small sanity check. */
	if (deref_final != 0
	    && realpath(result, tmp) != NULL
	    && !belongs_to_guestfs(tmp)) {
		notice(WARNING, INTERNAL, "tracee %d is out of my control (2)", pid);
		return -EPERM;
	}

end:
	VERBOSE(4, "pid %d:          -> \"%s\"", pid, result);
	return 0;
}

/**
 * Remove/substitute the leading part of a "translated" @path.  It
 * returns 0 if no transformation is required (ie. symmetric binding),
 * otherwise it returns the size in bytes of the updated @path,
 * including the end-of-string terminator.  On error it returns
 * -errno.
 */
int detranslate_path(char path[PATH_MAX], bool sanity_check, bool follow_binding)
{
	size_t new_length;

	assert(initialized != 0);

#if BENCHMARK_TRACEE_HANDLING
	return 0;
#endif

	/* Don't try to detranslate relative paths (typically the
	 * target of a relative symbolic link). */
	if (path[0] != '/')
		return 0;

	if (follow_binding) {
		switch (substitute_binding(BINDING_REAL, path)) {
		case 0:
			return 0;
		case 1:
			return strlen(path) + 1;
		default:
			break;
		}
	}

	/* Ensure the path is within the new root. */
	if (!belongs_to_guestfs(path)) {
		if (sanity_check)
			return -EPERM;
		else
			return 0;
	}

	/* Remove the leading part, that is, the "root". */
	assert(strlen(path) >= root_length);
	new_length = strlen(path) - root_length;
	if (new_length != 0) {
		memmove(path, path + root_length, new_length);
		path[new_length] = '\0';
	}
	else {
		/* Special case when path == root. */
		new_length = 1;
		strcpy(path, "/");
	}

	return new_length + 1;
}

/**
 * Check if the translated @t_path belongs to the guest rootfs, that
 * is, isn't from a binding.
 */
bool belongs_to_guestfs(char *t_path)
{
	/* Works only with translated paths!  */
	return (strncmp(root, t_path, root_length) == 0
		&& (t_path[root_length] == '\0' || t_path[root_length] == '/'));
}

typedef int (*foreach_fd_t)(pid_t pid, int fd, char path[PATH_MAX]);

/**
 * Call @callback on each open file descriptors of @pid. It returns
 * the status of the first failure, that is, if @callback returned
 * seomthing lesser than 0, otherwise 0.
 */
static int foreach_fd(pid_t pid, foreach_fd_t callback)
{
	struct dirent *dirent;
	char path[PATH_MAX];
	char proc_fd[32]; /* 32 > sizeof("/proc//fd") + sizeof(#ULONG_MAX) */
	int status;
	DIR *dirp;

	/* Format the path to the "virtual" directory. */
	status = sprintf(proc_fd, "/proc/%d/fd", pid);
	if (status < 0 || status >= sizeof(proc_fd))
		return 0;

	/* Open the virtual directory "/proc/$pid/fd". */
	dirp = opendir(proc_fd);
	if (dirp == NULL)
		return 0;

	while ((dirent = readdir(dirp)) != NULL) {
		/* Read the value of this "virtual" link. */
#ifdef HAVE_READLINKAT
		status = readlinkat(dirfd(dirp), dirent->d_name, path, PATH_MAX);
#else
		char tmp[PATH_MAX];
		if (strlen(proc_fd) + strlen(dirent->d_name) + 1 >= PATH_MAX)
			continue;
		strcpy(tmp, proc_fd);
		strcat(tmp, "/");
		strcat(tmp, dirent->d_name);
		status = readlink(tmp, path, PATH_MAX);
#endif
		if (status < 0 || status >= PATH_MAX)
			continue;
		path[status] = '\0';

		/* Ensure it points to a path (not a socket or somethink like that). */
		if (path[0] != '/')
			continue;

		status = callback(pid, atoi(dirent->d_name), path);
		if (status < 0)
			goto end;
	}
	status = 0;

end:
	closedir(dirp);
	return status;
}

/**
 * Helper for check_fd().
 */
static int check_fd_callback(pid_t pid, int fd, char path[PATH_MAX])
{
	/* XXX TODO: don't warn for files that were open before the attach. */
	if (!belongs_to_guestfs(path)) {
		notice(WARNING, INTERNAL, "tracee %d is out of my control (3)", pid);
		notice(WARNING, INTERNAL, "\"%s\" is not inside the new root (\"%s\")", path, root);
		return -pid;
	}
	return 0;
}

/**
 * Check if the file descriptors open by the process @pid point into
 * the new root directory; it returns -@pid if it is not the case,
 * otherwise 0 (or if an ignored error occured).
 */
int check_fd(pid_t pid)
{
	return foreach_fd(pid, check_fd_callback);
}

/**
 * Helper for list_open_fd().
 */
static int list_open_fd_callback(pid_t pid, int fd, char path[PATH_MAX])
{
	VERBOSE(1, "pid %d: access to \"%s\" (fd %d) won't be translated until closed", pid, path, fd);
	return 0;
}

/**
 * Warn for files that are open. It is useful right after PRoot has
 * attached a process.
 */
int list_open_fd(pid_t pid)
{
	return foreach_fd(pid, list_open_fd_callback);
}
