/* hello.c */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"

#ifdef HAVE_CURL_EASY_H
# include <curl/curl.h>
#endif

zend_class_entry *php_hello_curl_ce;

PHP_FUNCTION(hello_world) {
    php_printf("Hello World!\n");
}

PHP_FUNCTION(hello_return) {
    RETURN_STRING("Hello World!\n");
}

PHP_FUNCTION(hello_number) {
    RETURN_LONG(42);
}

PHP_FUNCTION(hello_you) {
    char *name;
    size_t name_len; /* Note: In PHP5, this is of type int */

    if (zend_parse_parameters(ZEND_NUM_ARGS(),
                              "s", &name, &name_len) == FAILURE) {
        return;
    }

    /* Easy way: php_printf("Hello %s!\n", name); */

    /* Binary safe way: */
    php_printf("Hello ");
    PHPWRITE(name, name_len);
    php_printf("!\n");
}

PHP_FUNCTION(hello_everyone) {
    zval *zarr, *name;
    zend_bool greet = 1;
    const char *salutation = "Hello";

    if (zend_parse_parameters(ZEND_NUM_ARGS(),
                              "a|b", &zarr, &greet) == FAILURE) {
        return;
    }

    if (!greet) salutation = "Goodbye";
    ZEND_HASH_FOREACH_VAL(Z_ARR_P(zarr), name)
        if (Z_TYPE_P(name) == IS_STRING) {
            php_printf("%s %s!\n", salutation, Z_STRVAL_P(name));
        }
    ZEND_HASH_FOREACH_END();
}

PHP_FUNCTION(hello_greetings) {
    array_init(return_value);
    add_next_index_string(return_value, "Hello");
    add_index_string(return_value, 1, "Goodbye");
}

zend_function_entry hello_functions[] = {
    PHP_FE(hello_world, NULL)
    PHP_FE(hello_return, NULL)
    PHP_FE(hello_number, NULL)
    PHP_FE(hello_you, NULL)
    PHP_FE(hello_everyone, NULL)
    PHP_FE(hello_greetings, NULL)
    PHP_FE_END
};

zend_class_entry *php_hello_curl_ce;

PHP_METHOD(HelloCurl, __construct) {
    zval *this_ = getThis();


}

zend_function_entry php_hello_curl_methods[] = {
    PHP_ME(HelloCurl, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(hello) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Hello\\cURL", php_hello_curl_methods);
    php_hello_curl_ce = zend_register_internal_class(&ce);

#ifdef HAVE_CURL_EASY_H
    REGISTER_BOOL_CONSTANT("HELLO_CURL", 1, CONST_CS | CONST_PERSISTENT);
    REGISTER_STRING_CONSTANT("HELLO_CURL_VERSION", curl_version(), CONST_CS | CONST_PERSISTENT);
#else
    REGISTER_BOOL_CONSTANT("HELLO_CURL", 0, CONST_CS | CONST_PERSISTENT);
#endif

    return SUCCESS;
}

zend_module_entry hello_module_entry = {
    STANDARD_MODULE_HEADER,
    "hello",
    hello_functions,
    PHP_MINIT(hello),
    NULL, NULL, NULL, NULL,
    NO_VERSION_YET,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HELLO
ZEND_GET_MODULE(hello)
#endif
