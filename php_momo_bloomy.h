/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_MOMO_BLOOMY_H
#define PHP_MOMO_BLOOMY_H

extern zend_module_entry momo_bloomy_module_entry;
#define phpext_momo_bloomy_ptr &momo_bloomy_module_entry

#ifdef PHP_WIN32
#	define PHP_MOMO_BLOOMY_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_MOMO_BLOOMY_API __attribute__ ((visibility("default")))
#else
#	define PHP_MOMO_BLOOMY_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(momo_bloomy);
PHP_MSHUTDOWN_FUNCTION(momo_bloomy);
PHP_RINIT_FUNCTION(momo_bloomy);
PHP_RSHUTDOWN_FUNCTION(momo_bloomy);
PHP_MINFO_FUNCTION(momo_bloomy);

PHP_FUNCTION(confirm_momo_bloomy_compiled);	/* For testing, remove later. */
PHP_FUNCTION(momo_bloomy_add);	/* For testing, remove later. */
PHP_FUNCTION(momo_bloomy_has);	
PHP_FUNCTION(momo_bloomy_info);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(momo_bloomy)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(momo_bloomy)
*/

/* In every utility function you add that needs to use variables 
   in php_momo_bloomy_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as MOMO_BLOOMY_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define MOMO_BLOOMY_G(v) TSRMG(momo_bloomy_globals_id, zend_momo_bloomy_globals *, v)
#else
#define MOMO_BLOOMY_G(v) (momo_bloomy_globals.v)
#endif

#endif	/* PHP_MOMO_BLOOMY_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
