/* File: utf8stringviewtokenizer_test.c; Copyright and License: see below */

#include "utf8string_test.h"
#include "utf8stringbuf/utf8stringviewtokenizer.h"
#include "utf8stringbuf/utf8stringview.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t testIntEmpty( test_fixture_t *fix );
static test_case_result_t testIntSpaceSeparators( test_fixture_t *fix );
static test_case_result_t testIntMixedCodepoints( test_fixture_t *fix );
static test_case_result_t testIntIntegers( test_fixture_t *fix );
static test_case_result_t testFloatEmpty( test_fixture_t *fix );
static test_case_result_t testFloatValidFloats( test_fixture_t *fix );
static test_case_result_t testFloatInvalidFloats( test_fixture_t *fix );
static test_case_result_t testFloatOnly( test_fixture_t *fix );
static test_case_result_t testTextMixedNumbers( test_fixture_t *fix );
static test_case_result_t testSwitchModes( test_fixture_t *fix );

test_suite_t utf8stringviewtokenizer_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "utf8stringviewtokenizer_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "testIntEmpty", &testIntEmpty );
    test_suite_add_test_case( &result, "testIntSpaceSeparators", &testIntSpaceSeparators );
    test_suite_add_test_case( &result, "testIntMixedCodepoints", &testIntMixedCodepoints );
    test_suite_add_test_case( &result, "testIntIntegers", &testIntIntegers );
    test_suite_add_test_case( &result, "testFloatEmpty", &testFloatEmpty );
    test_suite_add_test_case( &result, "testFloatValidFloats", &testFloatValidFloats );
    test_suite_add_test_case( &result, "testFloatInvalidFloats", &testFloatInvalidFloats );
    test_suite_add_test_case( &result, "testFloatOnly", &testFloatOnly );
    test_suite_add_test_case( &result, "testTextMixedNumbers", &testTextMixedNumbers );
    test_suite_add_test_case( &result, "testSwitchModes", &testSwitchModes );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t testIntEmpty( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, &UTF8STRINGVIEW_NULL, UTF8STRINGVIEWTOKENMODE_INT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );

    /* init */
    static const char *const my_list_2 = "";
    utf8stringviewtokenizer_init( &tok, &UTF8STRINGVIEW_STR( my_list_2 ), UTF8STRINGVIEWTOKENMODE_INT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );

    /* init */
    static const char *const my_list_3 = " \r\n\t";
    utf8stringviewtokenizer_init( &tok, &UTF8STRINGVIEW_STR( my_list_3 ), UTF8STRINGVIEWTOKENMODE_INT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testIntSpaceSeparators( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    uint32_t line;
    static const char *const my_list = "\nab 12\tab12 \r^$\x7f \n ";

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, &UTF8STRINGVIEW_STR( my_list ), UTF8STRINGVIEWTOKENMODE_INT );

    line = utf8stringviewtokenizer_get_line( &tok );
    TEST_EXPECT_EQUAL_INT( 0, line );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    line = utf8stringviewtokenizer_get_line( &tok );
    TEST_EXPECT_EQUAL_INT( 0, line );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_PTR( (my_list+1), utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &next ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "ab" ) );

    line = utf8stringviewtokenizer_get_line( &tok );
    TEST_EXPECT_EQUAL_INT( 2, line );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_PTR( (my_list+4), utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "12" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "ab12" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "^" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "$" ) );

    line = utf8stringviewtokenizer_get_line( &tok );
    TEST_EXPECT_EQUAL_INT( 2, line );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    line = utf8stringviewtokenizer_get_line( &tok );
    TEST_EXPECT_EQUAL_INT( 2, line );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    line = utf8stringviewtokenizer_get_line( &tok );
    TEST_EXPECT_EQUAL_INT( 3, line );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testIntMixedCodepoints( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = " !\"+-./09:@AZ[`_az{~\x7f\xc2\xa0\xc2\xa1";

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, &UTF8STRINGVIEW_STR( my_list ), UTF8STRINGVIEWTOKENMODE_INT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_PTR( (my_list+1), utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_get_length( &next ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "!" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "\"" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "+" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "-" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "/" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "09" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, ":" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "@" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "AZ" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "[" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "`" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "_az" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "{" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "~" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "\xc2\xa0\xc2\xa1" ) );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testIntIntegers( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = " int+32.23,-32e23-Infinity";

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, &UTF8STRINGVIEW_STR( my_list ), UTF8STRINGVIEWTOKENMODE_INT );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "int" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( &next ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "+32" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "23" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "," ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "-32" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "e23" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "-" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "Infinity" ) );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testFloatEmpty( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, &UTF8STRINGVIEW_NULL, UTF8STRINGVIEWTOKENMODE_FLOAT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );

    /* init */
    static const char *const my_list_2 = "";
    utf8stringviewtokenizer_init( &tok, &UTF8STRINGVIEW_STR( my_list_2 ), UTF8STRINGVIEWTOKENMODE_FLOAT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );

    /* init */
    static const char *const my_list_3 = " \r\n\t";
    utf8stringviewtokenizer_init( &tok, &UTF8STRINGVIEW_STR( my_list_3 ), UTF8STRINGVIEWTOKENMODE_FLOAT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testFloatValidFloats( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = " int+3EUR+32.23e+1,-InfinityNaN5.e05INFINITY5e-20 5.sec";

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, &UTF8STRINGVIEW_STR( my_list ), UTF8STRINGVIEWTOKENMODE_FLOAT );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "int" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "+3" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "EUR" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "+32.23e+1" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "," ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 9, utf8stringview_get_length( &next ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "-Infinity" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "NaN" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "5.e05" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "INFINITY" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "5e-20" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "5." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "sec" ) );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testFloatInvalidFloats( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "+nan +-.3e++4 +3.e 3.3.3e3.3E3i";

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, &UTF8STRINGVIEW_STR( my_list ), UTF8STRINGVIEWTOKENMODE_FLOAT );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "+" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "nan" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "+" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "-" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_get_length( &next ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "3" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "e" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "+" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "+4" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "+3." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "e" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "3.3" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "3e3" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "3E3" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "i" ) );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testFloatOnly( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "a1.2b.3e4.3ff-Infinityy";

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, &UTF8STRINGVIEW_STR( my_list ), UTF8STRINGVIEWTOKENMODE_FLOAT_ONLY );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "a" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "1.2" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "b" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "3e4" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "3" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "f" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "f" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "-Infinity" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "y" ) );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testTextMixedNumbers( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = " _int+32.23ij-Infinity12_3";

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, &UTF8STRINGVIEW_STR( my_list ), UTF8STRINGVIEWTOKENMODE_TEXT );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "_int" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "+" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "32" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "23ij" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "-" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "Infinity12_3" ) );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testSwitchModes( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = " +12.34e-56 +12.34e-56 +12.34e-56";

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, &UTF8STRINGVIEW_STR( my_list ), UTF8STRINGVIEWTOKENMODE_TEXT );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "+" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "12" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "34e" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "-" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "56" ) );

    utf8stringviewtokenizer_set_mode( &tok, UTF8STRINGVIEWTOKENMODE_INT );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "+12" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "34" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "e" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "-56" ) );

    utf8stringviewtokenizer_set_mode( &tok, UTF8STRINGVIEWTOKENMODE_FLOAT );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( &next, "+12.34e-56" ) );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2023-2024 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
