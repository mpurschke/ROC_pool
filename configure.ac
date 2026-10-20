dnl Process this file with autoconf to produce a configure script.
AC_INIT(ROC_pool,[1.00])
AC_CONFIG_SRCDIR([configure.ac])

dnl Checks for C++ compiler and sets default flags
AC_PROG_CXX(cxx CC g++)

AC_CANONICAL_HOST
AC_CONFIG_AUX_DIR([.])

AM_INIT_AUTOMAKE

LT_INIT([disable-static])

AC_PROG_INSTALL

if test $ac_cv_prog_gxx = yes; then
  CXXFLAGS="$CXXFLAGS -Wall -Werror"
fi

dnl gcc 14 and clang 20 still default to C++17
CXXFLAGS="$CXXFLAGS -std=c++20"

dnl -pthread taken out as in pmonitor (clang reports it as unused in the
dnl -nostdlib link of the library, which -Werror turns into an error)
ROOTCFLAGS=`root-config --cflags | sed 's/ *-pthread//'`
ROOTLIBS=`root-config --libs | sed 's/ *-pthread//'`
ROOTINC=`root-config --incdir`
AC_SUBST(ROOTLIBS)
AC_SUBST(ROOTINC)
AC_SUBST(ROOTCFLAGS)

CXXFLAGS="$CXXFLAGS $ROOTCFLAGS"

CINTDEFS=" -noIncludePaths  -inlineInputHeader "
AC_SUBST(CINTDEFS)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
