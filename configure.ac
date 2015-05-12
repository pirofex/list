AC_INIT([eportfolio], [0.1], [], [])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CXX
AC_LANG(C++)

AC_CONFIG_HEADERS([src/config.h])
AC_CHECK_HEADER([cxxtools/mutex.h], , AC_MSG_ERROR([cxxtools headers not found]))
AC_CHECK_HEADER([tnt/tntnet.h], , AC_MSG_ERROR([tntnet headers not found]))
AC_CHECK_HEADER([tntdb/connection.h], , AC_MSG_ERROR([tntdb headers not found]))

AC_PROG_SED
AC_CHECK_PROGS(ECPPC, [ecppc], AC_MSG_ERROR([ecppc compiler not found]))

AX_CHECK_COMPILE_FLAG([-Wall], [CPPFLAGS="$CPPFLAGS -Wall"])
AX_CHECK_COMPILE_FLAG([-pedantic], [CPPFLAGS="$CPPFLAGS -pedantic"])
AX_CHECK_COMPILE_FLAG([-std=c++1y], [CPPFLAGS="$CPPFLAGS -std=c++1y"])

AC_OUTPUT([
	Makefile
	src/Makefile
])
