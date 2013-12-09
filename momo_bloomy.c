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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_momo_bloomy.h"

#include "bloom.h"
#include "apc_mmap.h"
/* If you declare any globals in php_momo_bloomy.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(momo_bloomy)
*/

/* True global resources - no need for thread safety here */
static int le_momo_bloomy;
static int bloomy_initialized = 0; /* true if the bloomy has been initialized */
static char *mmap_file_mask = "/tmp/momo_bloomy_shm.xxx";
static apc_segment_t segment_t;
static bloom_t *bloom;
static const double DEFAULT_ERROR_RATE = 0.01;
static const double DEFAULT_ELEMENTS_NUM = 100000000;

/* {{{ momo_bloomy_functions[]
 *
 * Every user visible function must have an entry in momo_bloomy_functions[].
 */
const zend_function_entry momo_bloomy_functions[] = {
	PHP_FE(confirm_momo_bloomy_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(momo_bloomy_add,	NULL)		/* For testing, remove later. */
	PHP_FE(momo_bloomy_has,	NULL)		/* For testing, remove later. */
	PHP_FE(momo_bloomy_info,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in momo_bloomy_functions[] */
};
/* }}} */

/* {{{ momo_bloomy_module_entry
 */
zend_module_entry momo_bloomy_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"momo_bloomy",
	momo_bloomy_functions,
	PHP_MINIT(momo_bloomy),
	PHP_MSHUTDOWN(momo_bloomy),
	PHP_RINIT(momo_bloomy),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(momo_bloomy),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(momo_bloomy),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MOMO_BLOOMY
ZEND_GET_MODULE(momo_bloomy)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("momo_bloomy.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_momo_bloomy_globals, momo_bloomy_globals)
    STD_PHP_INI_ENTRY("momo_bloomy.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_momo_bloomy_globals, momo_bloomy_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_momo_bloomy_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_momo_bloomy_init_globals(zend_momo_bloomy_globals *momo_bloomy_globals)
{
	momo_bloomy_globals->global_value = 0;
	momo_bloomy_globals->global_string = NULL;
}
*/
/* }}} */

static void momo_bloomy_init()
{
    double error_rate = DEFAULT_ERROR_RATE;
    long elements_num = DEFAULT_ELEMENTS_NUM;
    bloom_return status;

    if (bloomy_initialized) {
        return;
    }

    //segment_t = (apc_segment_t*) malloc(sizeof(apc_segment_t) TSRMLS_CC);
    segment_t = apc_mmap(mmap_file_mask, bloom->spec.size_bytes TSRMLS_CC); 
    if ((long)segment_t.shmaddr == -1) {
    
    }

    bloom = segment_t.shmaddr;
    if (bloom->is_init != 1) {
        //bloom = (bloom_t *) emalloc(sizeof(bloom_t));
        status = bloom_init(bloom, elements_num, error_rate);
    
        if (status != BLOOM_SUCCESS) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "could not create filter");
            return;
        }
	    
        bloom->filter = (uint8_t *)safe_emalloc(bloom->spec.filter_size / 8, sizeof(uint8_t), 0);
	    memset(bloom->filter, 0, bloom->spec.filter_size / 8 * sizeof(uint8_t));
        bloom->is_init = 1;
    }

}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(momo_bloomy)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
    momo_bloomy_init();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(momo_bloomy)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(momo_bloomy)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(momo_bloomy)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(momo_bloomy)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "momo_bloomy support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_momo_bloomy_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_momo_bloomy_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "momo_bloomy", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

PHP_FUNCTION(momo_bloomy_add)
{
    char *data=NULL;
    int data_len;
    bloom_return status;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &data_len) == FAILURE) {
        return;
    }

    status = bloom_add(bloom, data, data_len);
    if (status != BLOOM_SUCCESS) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "could not add data to filter");
        RETURN_FALSE;
    }
    RETURN_TRUE;
}

PHP_FUNCTION(momo_bloomy_has)
{
    char *data = NULL;
    int   data_len;
    bloom_return status;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &data_len) == FAILURE) {
        return;
    }   

    status = bloom_contains(bloom, data, data_len);

    if (status == BLOOM_NOTFOUND) {
        RETURN_FALSE;
    } else {
        RETURN_TRUE;
    }
}

PHP_FUNCTION(momo_bloomy_info) 
{
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
        return;
    }

    array_init(return_value);
    add_assoc_double_ex(return_value, ZEND_STRS("error_rate"), bloom->max_error_rate);
    add_assoc_long_ex(return_value, ZEND_STRS("num_hashes"), bloom->spec.num_hashes);
    add_assoc_long_ex(return_value, ZEND_STRS("filter_size"), bloom->spec.filter_size);
    add_assoc_long_ex(return_value, ZEND_STRS("filter_size_in_bytes"), bloom->spec.size_bytes);
    add_assoc_long_ex(return_value, ZEND_STRS("num_items"), bloom->num_elements);
    //add_assoc_stringl_ex(return_value, ZEND_STRS("filter"), (char *)bloom->filter, bloom->spec.filter_size / 8 , 0);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
