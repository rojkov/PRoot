#undef PR_ARM_breakpoint
#undef PR_ARM_cacheflush
#undef PR_ARM_set_tls
#undef PR_ARM_usr26
#undef PR_ARM_usr32
#undef PR__llseek
#undef PR__newselect
#undef PR__sysctl
#undef PR_accept4
#undef PR_accept
#undef PR_access
#undef PR_acct
#undef PR_add_key
#undef PR_adjtimex
#undef PR_afs_syscall
#undef PR_alarm
#undef PR_arch_prctl
#undef PR_arm_fadvise64_64
#undef PR_arm_sync_file_range
#undef PR_bdflush
#undef PR_bind
#undef PR_break
#undef PR_brk
#undef PR_cacheflush
#undef PR_capget
#undef PR_capset
#undef PR_chdir
#undef PR_chmod
#undef PR_chown32
#undef PR_chown
#undef PR_chroot
#undef PR_clock_getres
#undef PR_clock_gettime
#undef PR_clock_nanosleep
#undef PR_clock_settime
#undef PR_clone
#undef PR_close
#undef PR_connect
#undef PR_creat
#undef PR_create_module
#undef PR_delete_module
#undef PR_dup2
#undef PR_dup3
#undef PR_dup
#undef PR_epoll_create1
#undef PR_epoll_create
#undef PR_epoll_ctl
#undef PR_epoll_ctl_old
#undef PR_epoll_pwait
#undef PR_epoll_wait
#undef PR_epoll_wait_old
#undef PR_eventfd2
#undef PR_eventfd
#undef PR_execve
#undef PR_exit
#undef PR_exit_group
#undef PR_faccessat
#undef PR_fadvise64
#undef PR_fadvise64_64
#undef PR_fallocate
#undef PR_fanotify_init
#undef PR_fanotify_mark
#undef PR_fchdir
#undef PR_fchmod
#undef PR_fchmodat
#undef PR_fchown32
#undef PR_fchown
#undef PR_fchownat
#undef PR_fcntl64
#undef PR_fcntl
#undef PR_fdatasync
#undef PR_fgetxattr
#undef PR_flistxattr
#undef PR_flock
#undef PR_fork
#undef PR_fremovexattr
#undef PR_fsetxattr
#undef PR_fstat64
#undef PR_fstat
#undef PR_fstatat64
#undef PR_fstatfs64
#undef PR_fstatfs
#undef PR_fsync
#undef PR_ftime
#undef PR_ftruncate64
#undef PR_ftruncate
#undef PR_futex
#undef PR_futimesat
#undef PR_get_kernel_syms
#undef PR_get_mempolicy
#undef PR_get_robust_list
#undef PR_get_thread_area
#undef PR_getcpu
#undef PR_getcwd
#undef PR_getdents64
#undef PR_getdents
#undef PR_getegid32
#undef PR_getegid
#undef PR_geteuid32
#undef PR_geteuid
#undef PR_getgid32
#undef PR_getgid
#undef PR_getgroups32
#undef PR_getgroups
#undef PR_getitimer
#undef PR_getpeername
#undef PR_getpgid
#undef PR_getpgrp
#undef PR_getpid
#undef PR_getpmsg
#undef PR_getppid
#undef PR_getpriority
#undef PR_getresgid32
#undef PR_getresgid
#undef PR_getresuid32
#undef PR_getresuid
#undef PR_getrlimit
#undef PR_getrusage
#undef PR_getsid
#undef PR_getsockname
#undef PR_getsockopt
#undef PR_gettid
#undef PR_gettimeofday
#undef PR_getuid32
#undef PR_getuid
#undef PR_getxattr
#undef PR_gtty
#undef PR_idle
#undef PR_init_module
#undef PR_inotify_add_watch
#undef PR_inotify_init1
#undef PR_inotify_init
#undef PR_inotify_rm_watch
#undef PR_io_cancel
#undef PR_io_destroy
#undef PR_io_getevents
#undef PR_io_setup
#undef PR_io_submit
#undef PR_ioctl
#undef PR_ioperm
#undef PR_iopl
#undef PR_ioprio_get
#undef PR_ioprio_set
#undef PR_ipc
#undef PR_kexec_load
#undef PR_keyctl
#undef PR_kill
#undef PR_lchown32
#undef PR_lchown
#undef PR_lgetxattr
#undef PR_link
#undef PR_linkat
#undef PR_listen
#undef PR_listxattr
#undef PR_llistxattr
#undef PR_lock
#undef PR_lookup_dcookie
#undef PR_lremovexattr
#undef PR_lseek
#undef PR_lsetxattr
#undef PR_lstat64
#undef PR_lstat
#undef PR_madvise1 /* On i386 both values are equal. */
#undef PR_madvise
#undef PR_mbind
#undef PR_migrate_pages
#undef PR_mincore
#undef PR_mkdir
#undef PR_mkdirat
#undef PR_mknod
#undef PR_mknodat
#undef PR_mlock
#undef PR_mlockall
#undef PR_mmap2
#undef PR_mmap
#undef PR_modify_ldt
#undef PR_mount
#undef PR_move_pages
#undef PR_mprotect
#undef PR_mpx
#undef PR_mq_getsetattr
#undef PR_mq_notify
#undef PR_mq_open
#undef PR_mq_timedreceive
#undef PR_mq_timedsend
#undef PR_mq_unlink
#undef PR_mremap
#undef PR_msgctl
#undef PR_msgget
#undef PR_msgrcv
#undef PR_msgsnd
#undef PR_msync
#undef PR_munlock
#undef PR_munlockall
#undef PR_munmap
#undef PR_nanosleep
#undef PR_newfstatat
#undef PR_nfsservctl
#undef PR_nice
#undef PR_oldfstat
#undef PR_oldlstat
#undef PR_oldolduname
#undef PR_oldstat
#undef PR_olduname
#undef PR_open
#undef PR_openat
#undef PR_pause
#undef PR_pciconfig_iobase
#undef PR_pciconfig_read
#undef PR_pciconfig_write
#undef PR_perf_event_open
#undef PR_personality
#undef PR_pipe2
#undef PR_pipe
#undef PR_pivot_root
#undef PR_poll
#undef PR_ppoll
#undef PR_prctl
#undef PR_pread64
#undef PR_preadv
#undef PR_prlimit64
#undef PR_prof
#undef PR_profil
#undef PR_pselect6
#undef PR_ptrace
#undef PR_putpmsg
#undef PR_pwrite64
#undef PR_pwritev
#undef PR_query_module
#undef PR_quotactl
#undef PR_read
#undef PR_readahead
#undef PR_readdir
#undef PR_readlink
#undef PR_readlinkat
#undef PR_readv
#undef PR_reboot
#undef PR_recv
#undef PR_recvfrom
#undef PR_recvmmsg
#undef PR_recvmsg
#undef PR_remap_file_pages
#undef PR_removexattr
#undef PR_rename
#undef PR_renameat
#undef PR_request_key
#undef PR_restart_syscall
#undef PR_rmdir
#undef PR_rt_sigaction
#undef PR_rt_sigpending
#undef PR_rt_sigprocmask
#undef PR_rt_sigqueueinfo
#undef PR_rt_sigreturn
#undef PR_rt_sigsuspend
#undef PR_rt_sigtimedwait
#undef PR_rt_tgsigqueueinfo
#undef PR_sched_get_priority_max
#undef PR_sched_get_priority_min
#undef PR_sched_getaffinity
#undef PR_sched_getparam
#undef PR_sched_getscheduler
#undef PR_sched_rr_get_interval
#undef PR_sched_setaffinity
#undef PR_sched_setparam
#undef PR_sched_setscheduler
#undef PR_sched_yield
#undef PR_security
#undef PR_select
#undef PR_semctl
#undef PR_semget
#undef PR_semop
#undef PR_semtimedop
#undef PR_send
#undef PR_sendfile64
#undef PR_sendfile
#undef PR_sendmsg
#undef PR_sendto
#undef PR_set_mempolicy
#undef PR_set_robust_list
#undef PR_set_thread_area
#undef PR_set_tid_address
#undef PR_setdomainname
#undef PR_setfsgid32
#undef PR_setfsgid
#undef PR_setfsuid32
#undef PR_setfsuid
#undef PR_setgid32
#undef PR_setgid
#undef PR_setgroups32
#undef PR_setgroups
#undef PR_sethostname
#undef PR_setitimer
#undef PR_setpgid
#undef PR_setpriority
#undef PR_setregid32
#undef PR_setregid
#undef PR_setresgid32
#undef PR_setresgid
#undef PR_setresuid32
#undef PR_setresuid
#undef PR_setreuid32
#undef PR_setreuid
#undef PR_setrlimit
#undef PR_setsid
#undef PR_setsockopt
#undef PR_settimeofday
#undef PR_setuid32
#undef PR_setuid
#undef PR_setxattr
#undef PR_sgetmask
#undef PR_shmat
#undef PR_shmctl
#undef PR_shmdt
#undef PR_shmget
#undef PR_shutdown
#undef PR_sigaction
#undef PR_sigaltstack
#undef PR_signal
#undef PR_signalfd4
#undef PR_signalfd
#undef PR_sigpending
#undef PR_sigprocmask
#undef PR_sigreturn
#undef PR_sigsuspend
#undef PR_socket
#undef PR_socketcall
#undef PR_socketpair
#undef PR_splice
#undef PR_ssetmask
#undef PR_stat64
#undef PR_stat
#undef PR_statfs64
#undef PR_statfs
#undef PR_stime
#undef PR_streams1
#undef PR_streams2
#undef PR_stty
#undef PR_swapoff
#undef PR_swapon
#undef PR_symlink
#undef PR_symlinkat
#undef PR_sync
#undef PR_sync_file_range
#undef PR_sysfs
#undef PR_sysinfo
#undef PR_syslog
#undef PR_tee
#undef PR_tgkill
#undef PR_time
#undef PR_timer_create
#undef PR_timer_delete
#undef PR_timer_getoverrun
#undef PR_timer_gettime
#undef PR_timer_settime
#undef PR_timerfd_create
#undef PR_timerfd_gettime
#undef PR_timerfd_settime
#undef PR_times
#undef PR_tkill
#undef PR_truncate64
#undef PR_truncate
#undef PR_tuxcall
#undef PR_ugetrlimit
#undef PR_ulimit
#undef PR_umask
#undef PR_umount2
#undef PR_umount
#undef PR_uname
#undef PR_unlink
#undef PR_unlinkat
#undef PR_unshare
#undef PR_uselib
#undef PR_ustat
#undef PR_utime
#undef PR_utimensat
#undef PR_utimes
#undef PR_vfork
#undef PR_vhangup
#undef PR_vm86
#undef PR_vm86old
#undef PR_vmsplice
#undef PR_vserver
#undef PR_wait4
#undef PR_waitid
#undef PR_waitpid
#undef PR_write
#undef PR_writev
#undef PR_sync_file_range2

