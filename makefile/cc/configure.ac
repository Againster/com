#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([SCloud], [1.0], [lissaagainster@yahoo.com])
AC_CONFIG_SRCDIR([so], [daemon])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign silent-rules])
AC_CONFIG_MACRO_DIR([m4])
AC_DISABLE_STATIC
AC_ENABLE_SHARED
LT_INIT([dlopen])
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# With feature
distprefix=$prefix
AC_ARG_WITH([distprefix], AS_HELP_STRING([--with-distprefix], ['This option will change SYSCONFDIR to be the value of it. SYSCONFDIR is the etc prefix where all SCLOUD programs will find configuration file.']))
AS_IF([test "x$with_distprefix"] == "x"], [], [distprefix=$with_distprefix])

# Var
SCLOUD_CONF_MACRO="-DSCLOUD -DSYSCONFDIR='\"$distprefix/etc\"' -DRUNSTATEDIR='\"$distprefix/var/run\"'"
AC_SUBST([AM_CXXFLAGS], [$SCLOUD_CONF_MACRO])
AC_SUBST([AM_CPPFLAGS], [$SCLOUD_CONF_MACRO])
AC_SUBST([AM_CFLAGS], [$SCLOUD_CONF_MACRO])

# Checks for protobuf.
#PKG_CHECK_MODULES(PROTOBUF, protobuf = 3.4.0)
#AC_SUBST(PROTOBUF_LIBS)
#AC_SUBST(PROTOBUF_CFLAGS)
#AC_SUBST(PROTOBUF_VERSION)
#
## Checks for protoc.
#AC_CHECK_PROG([PROTOC], [protoc], [protoc])
#AS_IF([test "x${PROTOC}" == "x"],
#      [AC_MSG_ERROR([ProtoBuf compiler "protoc" not found.])])

# Checks for libraries.
AC_CHECK_LIB([gmp], [main])
AC_CHECK_LIB([lzo], [main])
AC_CHECK_LIB([protobuf], [main])
AC_CHECK_LIB([zeromq], [main])
AC_CHECK_LIB([crypto], [main])
AC_CHECK_LIB([dl], [main])
AC_CHECK_LIB([gcov], [main])
AC_CHECK_LIB([m], [main])
AC_CHECK_LIB([pthread], [main])
AC_CHECK_LIB([rt], [main])
AC_CHECK_LIB([ssl], [main])

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h limits.h locale.h mach/mach.h malloc.h memory.h netdb.h netinet/in.h paths.h sgtty.h stddef.h stdint.h stdlib.h string.h strings.h sys/file.h sys/ioctl.h sys/param.h sys/socket.h sys/time.h sys/timeb.h syslog.h termio.h termios.h unistd.h utime.h wchar.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_CHECK_MEMBERS([struct stat.st_rdev])
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_STRERROR_R
AC_CHECK_FUNCS([alarm atexit bzero clock_gettime dup2 ftime ftruncate getcwd gethostbyaddr gethostbyname gethostname gethrtime getpagesize gettimeofday inet_ntoa localtime_r memchr memmove memset mkdir munmap pathconf realpath rmdir select setlocale socket strcasecmp strchr strcspn strdup strerror strncasecmp strndup strrchr strspn strstr strtol strtoul strtoull strtoumax sysinfo uname utime])

AC_CONFIG_FILES([Makefile
        daemon/Makefile
        daemon/zproxy/Makefile
        daemon/auth_dev/Makefile
        daemon/bbc_cgi_agent/Makefile
        daemon/bbc_dev_status/Makefile
        daemon/bbc_logsvr/Makefile
        daemon/bbc_ssopxy/Makefile
        so/Makefile
        so/base/Makefile
        so/bbcmonitor/Makefile
        so/bbcutil/Makefile
        so/cgi_agent/Makefile
        so/cl_ev/Makefile
        so/clog/Makefile
        so/debug/Makefile
        so/dirutil/Makefile
        so/db_client/Makefile
        so/encrypt/Makefile
        so/json/Makefile
        so/linux/Makefile
        so/proto/Makefile
        so/rwini/Makefile
        so/strutil/Makefile
        so/sync/Makefile
        so/xtest/Makefile
        so/uv2.6/Makefile
        so/zproxy/Makefile
        apps/Makefile
        apps/common/Makefile
        apps/usersso/Makefile
        apps/admin/Makefile
        apps/branch/Makefile
        apps/dbservice/Makefile
        apps/devauth/Makefile
        apps/email/Makefile
        apps/sms/Makefile
        apps/usereg/Makefile
        apps/auth/Makefile
        apps/devaddr/Makefile
        apps/icenter/Makefile
        apps/moa/Makefile
        apps/slogreceiver/Makefile
        apps/ucenter/Makefile
        apps/usermgr/Makefile])
#AC_CONFIG_SUBDIRS([3party/src/gmp-6.1.2])
AC_OUTPUT

AC_MSG_NOTICE([summary of build options:
    Install prefix:             ${prefix}
    Distribution prefix:        ${distprefix}
])
