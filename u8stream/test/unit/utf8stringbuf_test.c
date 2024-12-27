/* File: utf8stringbuf_test.c; Copyright and License: see below */

#include "utf8stringbuf_test.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>
#include <pthread.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t testInit( test_fixture_t *fix );
static test_case_result_t testClear( test_fixture_t *fix );
static test_case_result_t testLength( test_fixture_t *fix );
static test_case_result_t testGetView( test_fixture_t *fix );
static test_case_result_t testEquals( test_fixture_t *fix );
static test_case_result_t testStartsWith( test_fixture_t *fix );
static test_case_result_t testEndsWith( test_fixture_t *fix );
static test_case_result_t testCopyBuf( test_fixture_t *fix );
static test_case_result_t testCopyStr( test_fixture_t *fix );
static test_case_result_t testCopyWithCutUtf8( test_fixture_t *fix );
static test_case_result_t testCopyView( test_fixture_t *fix );
static test_case_result_t testAppendStr( test_fixture_t *fix );
static test_case_result_t testAppendBuf( test_fixture_t *fix );
static test_case_result_t testAppendInt( test_fixture_t *fix );
static test_case_result_t testAppendHex( test_fixture_t *fix );
static test_case_result_t testAppendChar( test_fixture_t *fix );
static test_case_result_t testAppendWStr( test_fixture_t *fix );
static test_case_result_t testAppendView( test_fixture_t *fix );
static test_case_result_t testGetEnd( test_fixture_t *fix );

test_suite_t utf8stringbuf_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "utf8StringBufTest",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "testInit", &testInit );
    test_suite_add_test_case( &result, "testClear", &testClear );
    test_suite_add_test_case( &result, "testLength", &testLength );
    test_suite_add_test_case( &result, "testGetView", &testGetView );
    test_suite_add_test_case( &result, "testEquals", &testEquals );
    test_suite_add_test_case( &result, "testStartsWith", &testStartsWith );
    test_suite_add_test_case( &result, "testEndsWith", &testEndsWith );
    test_suite_add_test_case( &result, "testCopyBuf", &testCopyBuf );
    test_suite_add_test_case( &result, "testCopyStr", &testCopyStr );
    test_suite_add_test_case( &result, "testCopyWithCutUtf8", &testCopyWithCutUtf8 );
    test_suite_add_test_case( &result, "testCopyView", &testCopyView );
    test_suite_add_test_case( &result, "testAppendStr", &testAppendStr );
    test_suite_add_test_case( &result, "testAppendBuf", &testAppendBuf );
    test_suite_add_test_case( &result, "testAppendInt", &testAppendInt );
    test_suite_add_test_case( &result, "testAppendHex", &testAppendHex );
    test_suite_add_test_case( &result, "testAppendChar", &testAppendChar );
    test_suite_add_test_case( &result, "testAppendWStr", &testAppendWStr );
    test_suite_add_test_case( &result, "testAppendView", &testAppendView );
    test_suite_add_test_case( &result, "testGetEnd", &testGetEnd );
    return result;
}

struct InitTestStruct {
    char urlArr[8192];
    utf8stringbuf_t url;
    pthread_mutex_t lock;
};
typedef struct InitTestStruct InitTest_t;
static InitTest_t structTest = { "http://", UTF8STRINGBUF_INIT( structTest.urlArr), PTHREAD_MUTEX_INITIALIZER, };

#define INITTEST(testStr,this_) { testStr, UTF8STRINGBUF_INIT( this_.urlArr), PTHREAD_MUTEX_INITIALIZER, }
static InitTest_t structArrTest[] = {
        INITTEST( "svn://first", structArrTest[0] ),
        INITTEST( "http://second", structArrTest[1] ),
        INITTEST( "ftp://third", structArrTest[2] ),
        INITTEST( "file://last", structArrTest[3] ),
};

static char ThousandPathNames[1000][256];
#define PATH_INIT(x) UTF8STRINGBUF_INIT( ThousandPathNames[x] )
#define FIVE_PATHS_INIT(x) PATH_INIT(x+0), PATH_INIT(x+1), PATH_INIT(x+2), PATH_INIT(x+3), PATH_INIT(x+4)
#define TWENTY_PATHS_INIT(x) FIVE_PATHS_INIT(x+0), FIVE_PATHS_INIT(x+5), FIVE_PATHS_INIT(x+10), FIVE_PATHS_INIT(x+15)
#define HUNDRED_PATHS_INIT(x) TWENTY_PATHS_INIT(x+0), TWENTY_PATHS_INIT(x+20), TWENTY_PATHS_INIT(x+40), TWENTY_PATHS_INIT(x+60), TWENTY_PATHS_INIT(x+80)
static utf8stringbuf_t ThousandPaths[1000] = {
        HUNDRED_PATHS_INIT(0), HUNDRED_PATHS_INIT(100), HUNDRED_PATHS_INIT(200), HUNDRED_PATHS_INIT(300),
        HUNDRED_PATHS_INIT(400), HUNDRED_PATHS_INIT(500), HUNDRED_PATHS_INIT(600), HUNDRED_PATHS_INIT(700),
        HUNDRED_PATHS_INIT(800), HUNDRED_PATHS_INIT(900),
};

enum { UTF8STRINGBUFTEST_MEGASIZE = (1024*1024), };

struct test_fixture_struct {
    char one_byte_arr[1];
    utf8stringbuf_t one_byte_buf;
    char four_byte_arr[4];
    utf8stringbuf_t four_byte_buf;
    char mega_byte_arr[UTF8STRINGBUFTEST_MEGASIZE];
    utf8stringbuf_t mega_byte_buf;
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    (*fix).one_byte_arr[0] = '\0';
    (*fix).one_byte_buf = UTF8STRINGBUF((*fix).one_byte_arr);
    memcpy( (*fix).four_byte_arr, "12\0", sizeof( (*fix).four_byte_arr ) );
    (*fix).four_byte_buf = UTF8STRINGBUF((*fix).four_byte_arr);
    memset( (*fix).mega_byte_arr, 'a', UTF8STRINGBUFTEST_MEGASIZE-1 );
    (*fix).mega_byte_arr[UTF8STRINGBUFTEST_MEGASIZE-1] = '\0';
    (*fix).mega_byte_arr[1000] = 'Z';
    (*fix).mega_byte_arr[1004] = 'Z';
    (*fix).mega_byte_arr[900000] = 'Z';
    (*fix).mega_byte_buf = UTF8STRINGBUF((*fix).mega_byte_arr);
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
}

static char staticByteArr[] = "12345";
static utf8stringbuf_t staticByteBuf = UTF8STRINGBUF_INIT(staticByteArr);

static test_case_result_t testInit( test_fixture_t *fix )
{
    /* check test_fixture_t */
    TEST_EXPECT( utf8stringbuf_get_string( &(*fix).one_byte_buf ) == (*fix).one_byte_arr );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringbuf_get_size( &(*fix).one_byte_buf ) );
    TEST_EXPECT( utf8stringbuf_get_string( &(*fix).four_byte_buf ) == (*fix).four_byte_arr );
    TEST_EXPECT_EQUAL_INT( 4, utf8stringbuf_get_size( &(*fix).four_byte_buf ) );
    TEST_EXPECT( utf8stringbuf_get_string( &(*fix).mega_byte_buf ) == (*fix).mega_byte_arr );
    TEST_EXPECT_EQUAL_INT( UTF8STRINGBUFTEST_MEGASIZE, utf8stringbuf_get_size( &(*fix).mega_byte_buf ) );

    /* check static initialization */
    TEST_EXPECT( utf8stringbuf_get_string( &staticByteBuf ) == staticByteArr );
    TEST_EXPECT_EQUAL_INT( 6, utf8stringbuf_get_size( &staticByteBuf ) );

    /* check dynamic initialization */
    char dynTestArr[] = "Hello";
    utf8stringbuf_t dynTestBuf = utf8stringbuf(dynTestArr);
    TEST_EXPECT( utf8stringbuf_get_string( &dynTestBuf ) == dynTestArr );
    TEST_EXPECT_EQUAL_INT( 6, utf8stringbuf_get_size( &dynTestBuf ) );

    /* compile time initialization on stack */
    char dynTestArr2[17] = "";
    utf8stringbuf_t dynTestBuf2 = UTF8STRINGBUF( dynTestArr2 );
    TEST_EXPECT( utf8stringbuf_get_string( &dynTestBuf2 ) == dynTestArr2 );
    TEST_EXPECT_EQUAL_INT( 17, utf8stringbuf_get_size( &dynTestBuf2 ) );

    /* check static struct initialization */
    TEST_EXPECT( utf8stringbuf_get_string( &structTest.url ) == structTest.urlArr );
    TEST_EXPECT_EQUAL_INT( 8192, utf8stringbuf_get_size( &structTest.url ) );

    /* check static struct-array initialization */
    TEST_EXPECT( utf8stringbuf_get_string( &structArrTest[0].url ) == structArrTest[0].urlArr );
    TEST_EXPECT_EQUAL_INT( 8192, utf8stringbuf_get_size( &structArrTest[0].url ) );
    TEST_EXPECT( utf8stringbuf_equals_str( &structArrTest[0].url, "svn://first" ));
    TEST_EXPECT( utf8stringbuf_get_string( &structArrTest[1].url ) == structArrTest[1].urlArr );
    TEST_EXPECT_EQUAL_INT( 8192, utf8stringbuf_get_size( &structArrTest[0].url ) );
    TEST_EXPECT( utf8stringbuf_equals_str( &structArrTest[1].url, "http://second" ));
    TEST_EXPECT( utf8stringbuf_get_string( &structArrTest[2].url ) == structArrTest[2].urlArr );
    TEST_EXPECT_EQUAL_INT( 8192, utf8stringbuf_get_size( &structArrTest[0].url ) );
    TEST_EXPECT( utf8stringbuf_equals_str( &structArrTest[2].url, "ftp://third" ));
    TEST_EXPECT( utf8stringbuf_get_string( &structArrTest[3].url ) == structArrTest[3].urlArr );
    TEST_EXPECT_EQUAL_INT( 8192, utf8stringbuf_get_size( &structArrTest[0].url ) );
    TEST_EXPECT( utf8stringbuf_equals_str( &structArrTest[3].url, "file://last" ));

    /* check static array initialization */
    strcpy( ThousandPathNames[99], "one-hundred" );
    TEST_EXPECT( utf8stringbuf_get_string( &ThousandPaths[99] ) == ThousandPathNames[99] );
    TEST_EXPECT_EQUAL_INT( 256, utf8stringbuf_get_size( &ThousandPaths[99] ) );
    TEST_EXPECT( utf8stringbuf_equals_str( &ThousandPaths[99], "one-hundred" ));
    TEST_EXPECT( utf8stringbuf_get_string( &ThousandPaths[100] ) == ThousandPathNames[100] );
    TEST_EXPECT_EQUAL_INT( 256, utf8stringbuf_get_size( &ThousandPaths[100] ) );
    TEST_EXPECT( utf8stringbuf_equals_str( &ThousandPaths[100], "" ));

    /* check runtime init function */
    utf8stringbuf_t dynTestBuf3 = utf8stringbuf_new( 10, dynTestArr2 );
    TEST_EXPECT( utf8stringbuf_get_string( &dynTestBuf3 ) == dynTestArr2 );
    TEST_EXPECT_EQUAL_INT( 10, utf8stringbuf_get_size( &dynTestBuf3 ) );

    /* check NULL initialization */
    utf8stringbuf_t nullTestBuf = utf8stringbuf_new( 4, NULL );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringbuf_get_size( &nullTestBuf ) );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testClear( test_fixture_t *fix )
{
    /* clear regular string */
    char dynTestArr[] = "Hello";
    utf8stringbuf_t dynTestBuf = UTF8STRINGBUF( dynTestArr );
    utf8stringbuf_clear( &dynTestBuf );
    TEST_EXPECT( 0 == memcmp( utf8stringbuf_get_string( &dynTestBuf ), "\0\0\0\0\0", 6 ) );
    TEST_EXPECT_EQUAL_INT( 6, utf8stringbuf_get_size( &dynTestBuf ) );

    /* clear NULL */
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf( NULL );
    utf8stringbuf_clear( &dynTestBuf2 );
    TEST_EXPECT( 0 == memcmp( utf8stringbuf_get_string( &dynTestBuf2 ), "", 1 ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringbuf_get_size( &dynTestBuf2 ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testLength( test_fixture_t *fix )
{
    int len;

    /* check utf8stringbuf_get_length */
    len = utf8stringbuf_get_length( &(*fix).one_byte_buf );
    TEST_EXPECT_EQUAL_INT( 0, len );
    len = utf8stringbuf_get_length( &(*fix).four_byte_buf );
    TEST_EXPECT_EQUAL_INT( 2, len );
    len = utf8stringbuf_get_length( &(*fix).mega_byte_buf );
    TEST_EXPECT_EQUAL_INT( UTF8STRINGBUFTEST_MEGASIZE-1, len );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testGetView( test_fixture_t *fix )
{
    char dynTestArr[] = "Hello";
    utf8stringbuf_t dynTestBuf = UTF8STRINGBUF( dynTestArr );
    const utf8stringview_t testme = utf8stringbuf_get_view( &dynTestBuf );
    TEST_EXPECT_EQUAL_PTR( &dynTestArr, utf8stringview_get_start( &testme ) );
    TEST_EXPECT_EQUAL_INT( 5, utf8stringview_get_length( &testme ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testEquals( test_fixture_t *fix )
{
    //  prepare
    int equal;
    char dynTestArr1[] = "Hello";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello World";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);

    //  test utf8stringbuf_equals_buf
    equal = utf8stringbuf_equals_buf( &dynTestBuf1, &dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_buf( &dynTestBuf1, &dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_buf( &dynTestBuf2, &dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_buf( &dynTestBuf2, &((*fix).one_byte_buf) );
    TEST_EXPECT_EQUAL_INT( 0, equal );

    //  test utf8stringbuf_equals_str
    equal = utf8stringbuf_equals_str( &dynTestBuf1, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_str( &dynTestBuf2, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_str( &(*fix).one_byte_buf, "Hi" );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_str( &(*fix).one_byte_buf, NULL );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, NULL );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testStartsWith( test_fixture_t *fix )
{
    //  prepare
    int matches;
    char dynTestArr1[] = "Hello";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello World";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);
    char dynTestArr3[] = "";
    utf8stringbuf_t dynTestBuf3 = utf8stringbuf(dynTestArr3);

    //  test utf8stringbuf_starts_with_str
    matches = utf8stringbuf_starts_with_str( &dynTestBuf1, dynTestArr2 );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_starts_with_str( &dynTestBuf2, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_str( &dynTestBuf1, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_str( &dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_str( &dynTestBuf1, NULL );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_starts_with_str( &dynTestBuf3, "" );
    TEST_EXPECT_EQUAL_INT( 1, matches );

    //  test utf8stringbuf_starts_with_buf
    matches = utf8stringbuf_starts_with_buf( &dynTestBuf1, &dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_starts_with_buf( &dynTestBuf2, &dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_buf( &dynTestBuf1, &dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_buf( &dynTestBuf1, &dynTestBuf3 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_buf( &dynTestBuf3, &dynTestBuf3 );
    TEST_EXPECT_EQUAL_INT( 1, matches );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testEndsWith( test_fixture_t *fix )
{
    //  prepare
    int matches;
    char dynTestArr1[] = "World";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello World";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);
    char dynTestArr3[] = "";
    utf8stringbuf_t dynTestBuf3 = utf8stringbuf(dynTestArr3);

    //  test utf8stringbuf_ends_with_str
    matches = utf8stringbuf_ends_with_str( &dynTestBuf1, dynTestArr2 );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_ends_with_str( &dynTestBuf2, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_str( &dynTestBuf1, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_str( &dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_str( &dynTestBuf1, NULL );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_ends_with_str( &dynTestBuf3, "" );
    TEST_EXPECT_EQUAL_INT( 1, matches );

    //  test utf8stringbuf_ends_with_buf
    matches = utf8stringbuf_ends_with_buf( &dynTestBuf1, &dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_ends_with_buf( &dynTestBuf2, &dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_buf( &dynTestBuf1, &dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_buf( &dynTestBuf1, &dynTestBuf3 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_buf( &dynTestBuf3, &dynTestBuf3 );
    TEST_EXPECT_EQUAL_INT( 1, matches );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testCopyBuf( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[] = "World";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello World";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);

    /* check utf8stringbuf_copy_buf */
    error = utf8stringbuf_copy_buf( &dynTestBuf1, &dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_buf( &((*fix).one_byte_buf), &dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &(*fix).one_byte_buf, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_buf( &dynTestBuf2, &dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf2, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_buf( &dynTestBuf2, &((*fix).one_byte_buf) );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf2, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testCopyStr( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[] = "World";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);

    /* check utf8stringbuf_copy_str */
    error = utf8stringbuf_copy_str( &dynTestBuf1, "Hello World" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( &(*fix).one_byte_buf, "Hi" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &(*fix).one_byte_buf, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( &dynTestBuf1, NULL );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( &dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( &dynTestBuf1, "Hi" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hi" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( &dynTestBuf1, "World" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "World" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testCopyWithCutUtf8( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[3] = "Wo";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[8] = "Hello W";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);

    /* check utf8stringbuf_copy_str, \xE2\x82\xAC is the euro symbol in utf8 */
    error = utf8stringbuf_copy_str( &dynTestBuf1, "\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( &dynTestBuf2, "\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf2, "\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( &dynTestBuf2, "\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf2, "\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( &dynTestBuf2, "\xE2\x82\xAC\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf2, "\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( &dynTestBuf2, "__\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf2, "__\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( &dynTestBuf2, "_\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf2, "_\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( &dynTestBuf2, "_\xE2\x82\xAC\xE2\x82\xAC_" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf2, "_\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_copy_str, \xC2\xA2 and \xF0\xA4\xAD\xA2 are other characters in utf8 */
    error = utf8stringbuf_copy_str( &dynTestBuf1, "\xC2\xA2_" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "\xC2\xA2" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( &dynTestBuf1, "\xF0\xA4\xAD\xA2" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_copy_str, \xC2\xA2 but not \xFC\xA4\xAD\xA2\xAD\xA2 are other characters in utf8 */
    error = utf8stringbuf_copy_str( &dynTestBuf2, "\xC2\xA2\xFC\xA4\xAD\xA2\xAD\xA2" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf2, "\xC2\xA2\xFC\xA4\xAD\xA2\xAD" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( &dynTestBuf2, "\xC2\xA2_\xFC\xA4\xAD\xA2\xAD\xA2" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf2, "\xC2\xA2_\xFC\xA4\xAD\xA2" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( &dynTestBuf2, "\xC2\xA2_\xC2\xA2_\xC2\xA2" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf2, "\xC2\xA2_\xC2\xA2_" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testCopyView( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;

    /* check success */
    error = utf8stringbuf_copy_view( &(*fix).four_byte_buf, &UTF8STRINGVIEW_STR("Hi") );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &(*fix).four_byte_buf, "Hi" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check truncated */
    error = utf8stringbuf_copy_view( &(*fix).four_byte_buf, &UTF8STRINGVIEW_STR("H\xE2\x80\x8B") );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &(*fix).four_byte_buf, "H" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testAppendStr( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[6] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_append_str */
    utf8stringbuf_copy_str( &dynTestBuf1, "Hell" );
    error = utf8stringbuf_append_str( &dynTestBuf1, "o" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "" );
    error = utf8stringbuf_append_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_str( &dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_str( &dynTestBuf1, NULL );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "Hell" );
    error = utf8stringbuf_append_str( &dynTestBuf1, "\xC2\xA2" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hell" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "Hell" );
    error = utf8stringbuf_append_str( &dynTestBuf1, "o!" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "Hel" );
    error = utf8stringbuf_append_str( &dynTestBuf1, "\xC2\xA2!" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hel\xC2\xA2" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testAppendBuf( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[6] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
    char dynTestArr2[] = "l\xC2\xA2";
    utf8stringbuf_t dynTestBuf2 = UTF8STRINGBUF(dynTestArr2);

    /* check utf8stringbuf_append_buf */
    utf8stringbuf_copy_str( &dynTestBuf1, "He" );
    error = utf8stringbuf_append_buf( &dynTestBuf1, &dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hel\xC2\xA2" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "Hel" );
    error = utf8stringbuf_append_buf( &dynTestBuf1, &dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hell" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_buf( &dynTestBuf1, &dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testAppendInt( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[6] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_append_int */
    utf8stringbuf_copy_str( &dynTestBuf1, "12" );
    error = utf8stringbuf_append_int( &dynTestBuf1, 345 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "12345" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "" );
    error = utf8stringbuf_append_int( &dynTestBuf1, -1234 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "-1234" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "1234" );
    error = utf8stringbuf_append_int( &dynTestBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "12340" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "12345" );
    error = utf8stringbuf_append_int( &dynTestBuf1, 6 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "12345" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "1" );
    error = utf8stringbuf_append_int( &dynTestBuf1, 234567 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "12345" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "1234" );
    error = utf8stringbuf_append_int( &dynTestBuf1, -1 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "1234-" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testAppendHex( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[6] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_append_hex */
    utf8stringbuf_copy_str( &dynTestBuf1, "0x" );
    error = utf8stringbuf_append_hex( &dynTestBuf1, -1 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "0xfff" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "0x" );
    error = utf8stringbuf_append_hex( &dynTestBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "0x0" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "0x" );
    error = utf8stringbuf_append_hex( &dynTestBuf1, 0x1000555 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "0x100" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testAppendChar( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[6] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_append_char */
    utf8stringbuf_copy_str( &dynTestBuf1, "Hell" );
    error = utf8stringbuf_append_char( &dynTestBuf1, 'o' );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append one byte too much */
    utf8stringbuf_copy_str( &dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_char( &dynTestBuf1, 'o' );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_char */
    utf8stringbuf_copy_str( &dynTestBuf1, "He" );
    error = utf8stringbuf_append_char( &dynTestBuf1, 0x20ac );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "He\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append one byte too much */
    utf8stringbuf_copy_str( &dynTestBuf1, "Hel" );
    error = utf8stringbuf_append_char( &dynTestBuf1, 0x20ac );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hel" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append zero */
    utf8stringbuf_clear( &dynTestBuf1 );
    error = utf8stringbuf_append_char( &dynTestBuf1, 0x0 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0x1 1-byte code point */
    utf8stringbuf_clear( &dynTestBuf1 );
    error = utf8stringbuf_append_char( &dynTestBuf1, 0x01 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "\x01" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0x7f 1-byte code point */
    utf8stringbuf_clear( &dynTestBuf1 );
    error = utf8stringbuf_append_char( &dynTestBuf1, 0x7f );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "\x7f" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0x80 2-byte code point */
    utf8stringbuf_clear( &dynTestBuf1 );
    error = utf8stringbuf_append_char( &dynTestBuf1, 0x80 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "\xC2\x80" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0x7ff 2-byte code point */
    utf8stringbuf_clear( &dynTestBuf1 );
    error = utf8stringbuf_append_char( &dynTestBuf1, 0x07ff );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "\xDF\xBF" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0x800 3-byte code point */
    utf8stringbuf_clear( &dynTestBuf1 );
    error = utf8stringbuf_append_char( &dynTestBuf1, 0x800 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "\xE0\xA0\x80" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0xffff 3-byte code point */
    utf8stringbuf_clear( &dynTestBuf1 );
    error = utf8stringbuf_append_char( &dynTestBuf1, 0xffff );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "\xEF\xBF\xBF" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0x10000 4-byte code point */
    utf8stringbuf_clear( &dynTestBuf1 );
    error = utf8stringbuf_append_char( &dynTestBuf1, 0x10000 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "\xF0\x90\x80\x80" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0x10ffff 4-byte code point */
    utf8stringbuf_clear( &dynTestBuf1 );
    error = utf8stringbuf_append_char( &dynTestBuf1, 0x10ffff );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "\xF4\x8F\xBF\xBF" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append illegal code point */
    utf8stringbuf_clear( &dynTestBuf1 );
    error = utf8stringbuf_append_char( &dynTestBuf1, 0x80000000 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_A_CODEPOINT, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testAppendWStr( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[6] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_append_wstr */
    wchar_t *wideString = L"lo";
    utf8stringbuf_copy_str( &dynTestBuf1, "Hel" );
    error = utf8stringbuf_append_wstr( &dynTestBuf1, wideString );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_wstr with multibyte */
    const wchar_t *wideString2 = L"e\x20ac";
    utf8stringbuf_copy_str( &dynTestBuf1, "H" );
    error = utf8stringbuf_append_wstr( &dynTestBuf1, wideString2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "He\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_wstr with truncation */
    utf8stringbuf_copy_str( &dynTestBuf1, "He" );
    error = utf8stringbuf_append_wstr( &dynTestBuf1, wideString2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hee" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_wstr( &dynTestBuf1, wideString2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_wstr with NULL */
    utf8stringbuf_copy_str( &dynTestBuf1, "He" );
    error = utf8stringbuf_append_wstr( &dynTestBuf1, NULL );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "He" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testAppendView( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[6] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_append_wstr */
    const utf8stringview_t view0 = UTF8STRINGVIEW_STR( "Hel" );
    const utf8stringview_t view1 = UTF8STRINGVIEW_STR( "lo" );
    utf8stringbuf_copy_view( &dynTestBuf1, &view0 );
    error = utf8stringbuf_append_view( &dynTestBuf1, &view1 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_view with multibyte */
    const utf8stringview_t view2 = UTF8STRINGVIEW_STR( "e\xE2\x82\xAC" );
    utf8stringbuf_copy_str( &dynTestBuf1, "H" );
    error = utf8stringbuf_append_view( &dynTestBuf1, &view2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "He\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_view with truncation */
    utf8stringbuf_copy_str( &dynTestBuf1, "He" );
    error = utf8stringbuf_append_view( &dynTestBuf1, &view2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hee" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( &dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_view( &dynTestBuf1, &view2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_view with NULL */
    const utf8stringview_t no_view = UTF8STRINGVIEW_EMPTY;
    utf8stringbuf_copy_str( &dynTestBuf1, "He" );
    error = utf8stringbuf_append_view( &dynTestBuf1, &no_view );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "He" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testGetEnd( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[4] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
    utf8stringbuf_t dynTestEnd;

    //  test utf8stringbuf_get_end
    dynTestEnd = utf8stringbuf_get_end( &dynTestBuf1 );
    error = utf8stringbuf_append_str( &dynTestEnd, "Aa" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "Aa" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    dynTestEnd = utf8stringbuf_get_end( &dynTestBuf1 );
    error = utf8stringbuf_append_str( &dynTestEnd, "Bb" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( &dynTestBuf1, "AaB" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2012-2024 Andreas Warnke
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
