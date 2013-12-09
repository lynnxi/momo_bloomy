dnl $Id$
dnl config.m4 for extension momo_bloomy

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(momo_bloomy, for momo_bloomy support,
Make sure that the comment is aligned:
[  --with-momo_bloomy             Include momo_bloomy support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(momo_bloomy, whether to enable momo_bloomy support,
 Make sure that the comment is aligned:
 [  --enable-momo_bloomy           Enable momo_bloomy support])

if test "$PHP_MOMO_BLOOMY" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-momo_bloomy -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/momo_bloomy.h"  # you most likely want to change this
  dnl if test -r $PHP_MOMO_BLOOMY/$SEARCH_FOR; then # path given as parameter
  dnl   MOMO_BLOOMY_DIR=$PHP_MOMO_BLOOMY
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for momo_bloomy files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       MOMO_BLOOMY_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$MOMO_BLOOMY_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the momo_bloomy distribution])
  dnl fi

  dnl # --with-momo_bloomy -> add include path
  dnl PHP_ADD_INCLUDE($MOMO_BLOOMY_DIR/include)

  dnl # --with-momo_bloomy -> check for lib and symbol presence
  dnl LIBNAME=momo_bloomy # you may want to change this
  dnl LIBSYMBOL=momo_bloomy # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $MOMO_BLOOMY_DIR/lib, MOMO_BLOOMY_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_MOMO_BLOOMYLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong momo_bloomy lib version or lib not found])
  dnl ],[
  dnl   -L$MOMO_BLOOMY_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(MOMO_BLOOMY_SHARED_LIBADD)

  PHP_NEW_EXTENSION(momo_bloomy, momo_bloomy.c, $ext_shared)
fi
