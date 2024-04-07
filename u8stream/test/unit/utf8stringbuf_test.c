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
#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
static test_case_result_t testEqualsRegion( test_fixture_t *fix );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */
static test_case_result_t testStartsWith( test_fixture_t *fix );
static test_case_result_t testEndsWith( test_fixture_t *fix );
#ifdef UTF8STRINGBUF_DEPRECATED_INDEX
static test_case_result_t testFindFirst( test_fixture_t *fix );
static test_case_result_t testFindNext( test_fixture_t *fix );
static test_case_result_t testFindLast( test_fixture_t *fix );
#endif  /* UTF8STRINGBUF_DEPRECATED_INDEX */
static test_case_result_t testCopyBuf( test_fixture_t *fix );
static test_case_result_t testCopyStr( test_fixture_t *fix );
static test_case_result_t testCopyWithCutUtf8( test_fixture_t *fix );
static test_case_result_t testCopyView( test_fixture_t *fix );
#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
static test_case_result_t testCopyRegion( test_fixture_t *fix );
static test_case_result_t testReplaceRegion( test_fixture_t *fix );
static test_case_result_t testReplaceRegionExceededRanges( test_fixture_t *fix );
static test_case_result_t testReplaceRegionWithCutUtf8( test_fixture_t *fix );
static test_case_result_t testReplaceRegionBuf( test_fixture_t *fix );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */
static test_case_result_t testReplaceAll( test_fixture_t *fix );
static test_case_result_t testReplaceAllBadCases( test_fixture_t *fix );
static test_case_result_t testReplaceAllStr( test_fixture_t *fix );
#ifdef UTF8STRINGBUF_DEPRECATED_INPLACE
static test_case_result_t testReplaceAllBuf( test_fixture_t *fix );
#endif  /* UTF8STRINGBUF_DEPRECATED_INPLACE */
#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
static test_case_result_t testInsertBuf( test_fixture_t *fix );
static test_case_result_t testInsertStr( test_fixture_t *fix );
static test_case_result_t testDelete( test_fixture_t *fix );
static test_case_result_t testDeleteToEnd( test_fixture_t *fix );
static test_case_result_t testDeleteFromEnd( test_fixture_t *fix );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */
static test_case_result_t testAppendStr( test_fixture_t *fix );
static test_case_result_t testAppendBuf( test_fixture_t *fix );
static test_case_result_t testAppendInt( test_fixture_t *fix );
static test_case_result_t testAppendHex( test_fixture_t *fix );
static test_case_result_t testAppendChar( test_fixture_t *fix );
static test_case_result_t testAppendWStr( test_fixture_t *fix );
static test_case_result_t testAppendView( test_fixture_t *fix );
#ifdef UTF8STRINGBUF_DEPRECATED_INDEX
static test_case_result_t testCharAt( test_fixture_t *fix );
static test_case_result_t testCharAtLoops( test_fixture_t *fix );
#endif  /* UTF8STRINGBUF_DEPRECATED_INDEX */
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
#ifdef UTF8STRINGBUF_DEPRECATED_INDEX
    test_suite_add_test_case( &result, "testEqualsRegion", &testEqualsRegion );
#endif  /* UTF8STRINGBUF_DEPRECATED_INDEX */
    test_suite_add_test_case( &result, "testStartsWith", &testStartsWith );
    test_suite_add_test_case( &result, "testEndsWith", &testEndsWith );
#ifdef UTF8STRINGBUF_DEPRECATED_INDEX
    test_suite_add_test_case( &result, "testFindFirst", &testFindFirst );
    test_suite_add_test_case( &result, "testFindNext", &testFindNext );
    test_suite_add_test_case( &result, "testFindLast", &testFindLast );
#endif  /* UTF8STRINGBUF_DEPRECATED_INDEX */
    test_suite_add_test_case( &result, "testCopyBuf", &testCopyBuf );
    test_suite_add_test_case( &result, "testCopyStr", &testCopyStr );
    test_suite_add_test_case( &result, "testCopyWithCutUtf8", &testCopyWithCutUtf8 );
    test_suite_add_test_case( &result, "testCopyView", &testCopyView );
#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
    test_suite_add_test_case( &result, "testCopyRegion", &testCopyRegion );
    test_suite_add_test_case( &result, "testReplaceRegion", &testReplaceRegion );
    test_suite_add_test_case( &result, "testReplaceRegionExceededRanges", &testReplaceRegionExceededRanges );
    test_suite_add_test_case( &result, "testReplaceRegionWithCutUtf8", &testReplaceRegionWithCutUtf8 );
    test_suite_add_test_case( &result, "testReplaceRegionBuf", &testReplaceRegionBuf );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */
    test_suite_add_test_case( &result, "testReplaceAll", &testReplaceAll );
    test_suite_add_test_case( &result, "testReplaceAllBadCases", &testReplaceAllBadCases );
    test_suite_add_test_case( &result, "testReplaceAllStr", &testReplaceAllStr );
#ifdef UTF8STRINGBUF_DEPRECATED_INPLACE
    test_suite_add_test_case( &result, "testReplaceAllBuf", &testReplaceAllBuf );
#endif  /* UTF8STRINGBUF_DEPRECATED_INPLACE */
#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
    test_suite_add_test_case( &result, "testInsertBuf", &testInsertBuf );
    test_suite_add_test_case( &result, "testInsertStr", &testInsertStr );
    test_suite_add_test_case( &result, "testDelete", &testDelete );
    test_suite_add_test_case( &result, "testDeleteToEnd", &testDeleteToEnd );
    test_suite_add_test_case( &result, "testDeleteFromEnd", &testDeleteFromEnd );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */
    test_suite_add_test_case( &result, "testAppendStr", &testAppendStr );
    test_suite_add_test_case( &result, "testAppendBuf", &testAppendBuf );
    test_suite_add_test_case( &result, "testAppendInt", &testAppendInt );
    test_suite_add_test_case( &result, "testAppendHex", &testAppendHex );
    test_suite_add_test_case( &result, "testAppendChar", &testAppendChar );
    test_suite_add_test_case( &result, "testAppendWStr", &testAppendWStr );
    test_suite_add_test_case( &result, "testAppendView", &testAppendView );
#ifdef UTF8STRINGBUF_DEPRECATED_INDEX
    test_suite_add_test_case( &result, "testCharAt", &testCharAt );
    test_suite_add_test_case( &result, "testCharAtLoops", &testCharAtLoops );
#endif  /* UTF8STRINGBUF_DEPRECATED_INDEX */
    test_suite_add_test_case( &result, "testGetEnd", &testGetEnd );
    return result;
}

static const char * const SQL_ENCODE[][2] = {
         { "\0", "\\0" },  //  within strings, null cannot be represented.
         { "\x09", "\\t" },
         { "\x0a", "\\n" },
         { "\x0d", "\\r" },
         { "\x0e", "\\b" },
         { "\x1a", "\\z" },
         { "\"", "\\\"" },
         { "'", "\\'" },
         { "\\", "\\\\" },
         { "%", "\\%" },  //  % replacement only needed in searches by LIKE operator
         { "_", "\\_" },  //  _ replacement only needed in searches by LIKE operator
         { NULL, NULL } };
static const char *const XML_ENCODE[][2] = {
         { "<", "&lt;" },
         { ">", "&gt;" },
         { "&", "&amp;" },
         { "\"", "&quot;" },  //  " replacement only needed in attribute values
         { "'", "&apos;" },  //  " replacement only needed in attribute values
         { NULL, NULL } };

static const char *const TO_UPPER_CASE[][2] = {
         { "a", "A" }, { "b", "B" }, { "c", "C" }, { "d", "D" }, { "e", "E" }, { "f", "F" }, { "g", "G" }, { "h", "H" },
         { "i", "I" }, { "j", "J" }, { "k", "K" }, { "l", "L" }, { "m", "M" }, { "n", "N" }, { "o", "O" }, { "p", "P" },
         { "q", "Q" }, { "r", "R" }, { "s", "S" }, { "t", "T" }, { "u", "U" }, { "v", "V" }, { "w", "W" }, { "x", "X" },
         { "y", "Y" }, { "z", "Z" }, { "\xc3\xa4", "\xc3\x84" }, { "\xc3\xb6", "\xc3\x96" },
         { "\xc3\xbc", "\xc3\x9c" },
         { NULL, NULL } };

static const char *const TO_LOWER_CASE[][2] = {
         { "A", "a" }, { "B", "b" }, { "C", "c" }, { "D", "d" }, { "E", "e" }, { "F", "f" }, { "G", "g" }, { "H", "h" },
         { "I", "i" }, { "J", "j" }, { "K", "k" }, { "L", "l" }, { "M", "m" }, { "N", "n" }, { "O", "o" }, { "P", "p" },
         { "Q", "q" }, { "R", "r" }, { "S", "s" }, { "T", "t" }, { "U", "u" }, { "V", "v" }, { "W", "w" }, { "X", "x" },
         { "Y", "y" }, { "Z", "z" }, { "\xc3\x84", "\xc3\xa4" }, { "\xc3\x96", "\xc3\xb6" },
         { "\xc3\x9c", "\xc3\xbc" },
         { NULL, NULL } };

static const char *const SHRINK_DUPLICATES_EXCEPT_Z[][2] = {
         { "aa", "A" }, { "bb", "B" }, { "cc", "C" }, { "dd", "D" }, { "ee", "E" }, { "ff", "F" }, { "gg", "G" }, { "hh", "H" },
         { "ii", "I" }, { "jj", "J" }, { "kk", "K" }, { "ll", "L" }, { "mm", "M" }, { "nn", "N" }, { "oo", "O" }, { "pp", "P" },
         { "qq", "Q" }, { "rr", "R" }, { "ss", "S" }, { "tt", "T" }, { "uu", "U" }, { "vv", "V" }, { "ww", "W" }, { "xx", "X" },
         { "yy", "Y" },
         { NULL, NULL } };

static const char *const EXPAND_SINGLES_EXCEPT_Z[][2] = {
         { "A", "aa" }, { "B", "bb" }, { "C", "cc" }, { "D", "dd" }, { "E", "ee" }, { "F", "ff" }, { "G", "gg" }, { "H", "hh" },
         { "I", "ii" }, { "J", "jj" }, { "K", "kk" }, { "L", "ll" }, { "M", "mm" }, { "N", "nn" }, { "O", "oo" }, { "P", "pp" },
         { "Q", "qq" }, { "R", "rr" }, { "S", "ss" }, { "T", "tt" }, { "U", "uu" }, { "V", "vv" }, { "W", "ww" }, { "X", "xx" },
         { "Y", "yy" },
         { NULL, NULL } };

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
    TEST_EXPECT( utf8stringbuf_get_string( (*fix).one_byte_buf ) == (*fix).one_byte_arr );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringbuf_get_size( (*fix).one_byte_buf ) );
    TEST_EXPECT( utf8stringbuf_get_string( (*fix).four_byte_buf ) == (*fix).four_byte_arr );
    TEST_EXPECT_EQUAL_INT( 4, utf8stringbuf_get_size( (*fix).four_byte_buf ) );
    TEST_EXPECT( utf8stringbuf_get_string( (*fix).mega_byte_buf ) == (*fix).mega_byte_arr );
    TEST_EXPECT_EQUAL_INT( UTF8STRINGBUFTEST_MEGASIZE, utf8stringbuf_get_size( (*fix).mega_byte_buf ) );

    /* check static initialization */
    TEST_EXPECT( utf8stringbuf_get_string( staticByteBuf ) == staticByteArr );
    TEST_EXPECT_EQUAL_INT( 6, utf8stringbuf_get_size( staticByteBuf ) );

    /* check dynamic initialization */
    char dynTestArr[] = "Hello";
    utf8stringbuf_t dynTestBuf = utf8stringbuf(dynTestArr);
    TEST_EXPECT( utf8stringbuf_get_string( dynTestBuf ) == dynTestArr );
    TEST_EXPECT_EQUAL_INT( 6, utf8stringbuf_get_size( dynTestBuf ) );

    /* compile time initialization on stack */
    char dynTestArr2[17] = "";
    utf8stringbuf_t dynTestBuf2 = UTF8STRINGBUF( dynTestArr2 );
    TEST_EXPECT( utf8stringbuf_get_string( dynTestBuf2 ) == dynTestArr2 );
    TEST_EXPECT_EQUAL_INT( 17, utf8stringbuf_get_size( dynTestBuf2 ) );

    /* check static struct initialization */
    TEST_EXPECT( utf8stringbuf_get_string( structTest.url ) == structTest.urlArr );
    TEST_EXPECT_EQUAL_INT( 8192, utf8stringbuf_get_size( structTest.url ) );

    /* check static struct-array initialization */
    TEST_EXPECT( utf8stringbuf_get_string( structArrTest[0].url ) == structArrTest[0].urlArr );
    TEST_EXPECT_EQUAL_INT( 8192, utf8stringbuf_get_size( structArrTest[0].url ) );
    TEST_EXPECT( utf8stringbuf_equals_str( structArrTest[0].url, "svn://first" ));
    TEST_EXPECT( utf8stringbuf_get_string( structArrTest[1].url ) == structArrTest[1].urlArr );
    TEST_EXPECT_EQUAL_INT( 8192, utf8stringbuf_get_size( structArrTest[0].url ) );
    TEST_EXPECT( utf8stringbuf_equals_str( structArrTest[1].url, "http://second" ));
    TEST_EXPECT( utf8stringbuf_get_string( structArrTest[2].url ) == structArrTest[2].urlArr );
    TEST_EXPECT_EQUAL_INT( 8192, utf8stringbuf_get_size( structArrTest[0].url ) );
    TEST_EXPECT( utf8stringbuf_equals_str( structArrTest[2].url, "ftp://third" ));
    TEST_EXPECT( utf8stringbuf_get_string( structArrTest[3].url ) == structArrTest[3].urlArr );
    TEST_EXPECT_EQUAL_INT( 8192, utf8stringbuf_get_size( structArrTest[0].url ) );
    TEST_EXPECT( utf8stringbuf_equals_str( structArrTest[3].url, "file://last" ));

    /* check static array initialization */
    strcpy( ThousandPathNames[99], "one-hundred" );
    TEST_EXPECT( utf8stringbuf_get_string( ThousandPaths[99] ) == ThousandPathNames[99] );
    TEST_EXPECT_EQUAL_INT( 256, utf8stringbuf_get_size( ThousandPaths[99] ) );
    TEST_EXPECT( utf8stringbuf_equals_str( ThousandPaths[99], "one-hundred" ));
    TEST_EXPECT( utf8stringbuf_get_string( ThousandPaths[100] ) == ThousandPathNames[100] );
    TEST_EXPECT_EQUAL_INT( 256, utf8stringbuf_get_size( ThousandPaths[100] ) );
    TEST_EXPECT( utf8stringbuf_equals_str( ThousandPaths[100], "" ));

    /* check runtime init function */
    utf8stringbuf_t dynTestBuf3 = utf8stringbuf_init( 10, dynTestArr2 );
    TEST_EXPECT( utf8stringbuf_get_string( dynTestBuf3 ) == dynTestArr2 );
    TEST_EXPECT_EQUAL_INT( 10, utf8stringbuf_get_size( dynTestBuf3 ) );

    /* check NULL initialization */
    utf8stringbuf_t nullTestBuf = utf8stringbuf_init( 4, NULL );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringbuf_get_size( nullTestBuf ) );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testClear( test_fixture_t *fix )
{
    /* clear regular string */
    char dynTestArr[] = "Hello";
    utf8stringbuf_t dynTestBuf = UTF8STRINGBUF( dynTestArr );
    utf8stringbuf_clear ( dynTestBuf );
    TEST_EXPECT( 0 == memcmp( utf8stringbuf_get_string( dynTestBuf ), "\0\0\0\0\0", 6 ) );
    TEST_EXPECT_EQUAL_INT( 6, utf8stringbuf_get_size( dynTestBuf ) );

    /* clear NULL */
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf( NULL );
    utf8stringbuf_clear ( dynTestBuf2 );
    TEST_EXPECT( 0 == memcmp( utf8stringbuf_get_string( dynTestBuf2 ), "", 1 ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringbuf_get_size( dynTestBuf2 ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testLength( test_fixture_t *fix )
{
    int len;

    /* check utf8stringbuf_get_length */
    len = utf8stringbuf_get_length( (*fix).one_byte_buf );
    TEST_EXPECT_EQUAL_INT( 0, len );
    len = utf8stringbuf_get_length( (*fix).four_byte_buf );
    TEST_EXPECT_EQUAL_INT( 2, len );
    len = utf8stringbuf_get_length( (*fix).mega_byte_buf );
    TEST_EXPECT_EQUAL_INT( UTF8STRINGBUFTEST_MEGASIZE-1, len );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testGetView( test_fixture_t *fix )
{
    char dynTestArr[] = "Hello";
    utf8stringbuf_t dynTestBuf = UTF8STRINGBUF( dynTestArr );
    const utf8stringview_t testme = utf8stringbuf_get_view( dynTestBuf );
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
    equal = utf8stringbuf_equals_buf( dynTestBuf1, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_buf( dynTestBuf1, dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_buf( dynTestBuf2, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_buf( dynTestBuf2, (*fix).one_byte_buf );
    TEST_EXPECT_EQUAL_INT( 0, equal );

    //  test utf8stringbuf_equals_str
    equal = utf8stringbuf_equals_str( dynTestBuf1, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_str( dynTestBuf2, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_str( (*fix).one_byte_buf, "Hi" );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_str( (*fix).one_byte_buf, NULL );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_str( dynTestBuf1, NULL );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    return TEST_CASE_RESULT_OK;
}

#ifdef UTF8STRINGBUF_DEPRECATED_INDEX
static test_case_result_t testEqualsRegion( test_fixture_t *fix )
{
    //  prepare
    int equal;
    char dynTestArr1[] = "Hello";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello Hell Hello Hello";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);

    //  test utf8stringbuf_equals_region_buf
    equal = utf8stringbuf_equals_region_buf( dynTestBuf1, 0, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf1, -1, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf1, 1, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf1, 6, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf1, 0, dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf2, 0, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf2, 6, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf2, 11, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf2, 17, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf2, 3, (*fix).one_byte_buf );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    //  test utf8stringbuf_equals_region_str
    equal = utf8stringbuf_equals_region_str( dynTestBuf1, 0, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_str( dynTestBuf1, 0, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_str( dynTestBuf1, 5, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_str( dynTestBuf1, 0, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_str( dynTestBuf2, 6, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_str( dynTestBuf2, 17, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_str( (*fix).one_byte_buf, 0, "Hi" );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_str( (*fix).one_byte_buf, 0, NULL );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_str( dynTestBuf1, -1, NULL );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_DEPRECATED_INDEX */

static test_case_result_t testStartsWith( test_fixture_t *fix )
{
    //  prepare
    int matches;
    char dynTestArr1[] = "Hello";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello World";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);

    //  test utf8stringbuf_starts_with_str
    matches = utf8stringbuf_starts_with_str( dynTestBuf1, dynTestArr2 );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_starts_with_str( dynTestBuf2, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_str( dynTestBuf1, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_str( dynTestBuf1, NULL );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_starts_with_str( utf8stringbuf(""), "" );
    TEST_EXPECT_EQUAL_INT( 1, matches );

    //  test utf8stringbuf_starts_with_buf
    matches = utf8stringbuf_starts_with_buf( dynTestBuf1, dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_starts_with_buf( dynTestBuf2, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_buf( dynTestBuf1, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_buf( dynTestBuf1, utf8stringbuf("") );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_buf( utf8stringbuf(""), utf8stringbuf("") );
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

    //  test utf8stringbuf_ends_with_str
    matches = utf8stringbuf_ends_with_str( dynTestBuf1, dynTestArr2 );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_ends_with_str( dynTestBuf2, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_str( dynTestBuf1, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_str( dynTestBuf1, NULL );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_ends_with_str( utf8stringbuf(""), "" );
    TEST_EXPECT_EQUAL_INT( 1, matches );

    //  test utf8stringbuf_ends_with_buf
    matches = utf8stringbuf_ends_with_buf( dynTestBuf1, dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_ends_with_buf( dynTestBuf2, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_buf( dynTestBuf1, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_buf( dynTestBuf1, utf8stringbuf("") );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_buf( utf8stringbuf(""), utf8stringbuf("") );
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
    error = utf8stringbuf_copy_buf( dynTestBuf1, dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_buf( (*fix).one_byte_buf, dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( (*fix).one_byte_buf, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_buf( dynTestBuf2, dynTestBuf1 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_buf( dynTestBuf2, (*fix).one_byte_buf );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "" );
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
    error = utf8stringbuf_copy_str( dynTestBuf1, "Hello World" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( (*fix).one_byte_buf, "Hi" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( (*fix).one_byte_buf, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf1, NULL );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf1, "Hi" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hi" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf1, "World" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "World" );
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
    error = utf8stringbuf_copy_str( dynTestBuf1, "\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "\xE2\x82\xAC\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "__\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "__\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "_\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "_\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "_\xE2\x82\xAC\xE2\x82\xAC_" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "_\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_copy_str, \xC2\xA2 and \xF0\xA4\xAD\xA2 are other characters in utf8 */
    error = utf8stringbuf_copy_str( dynTestBuf1, "\xC2\xA2_" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "\xC2\xA2" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf1, "\xF0\xA4\xAD\xA2" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_copy_str, \xC2\xA2 but not \xFC\xA4\xAD\xA2\xAD\xA2 are other characters in utf8 */
    error = utf8stringbuf_copy_str( dynTestBuf2, "\xC2\xA2\xFC\xA4\xAD\xA2\xAD\xA2" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "\xC2\xA2\xFC\xA4\xAD\xA2\xAD" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "\xC2\xA2_\xFC\xA4\xAD\xA2\xAD\xA2" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "\xC2\xA2_\xFC\xA4\xAD\xA2" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "\xC2\xA2_\xC2\xA2_\xC2\xA2" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "\xC2\xA2_\xC2\xA2_" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testCopyView( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;

    /* check success */
    error = utf8stringbuf_copy_view( (*fix).four_byte_buf, &UTF8STRINGVIEW_STR("Hi") );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( (*fix).four_byte_buf, "Hi" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check truncated */
    error = utf8stringbuf_copy_view( (*fix).four_byte_buf, &UTF8STRINGVIEW_STR("H\xE2\x80\x8B") );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( (*fix).four_byte_buf, "H" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    return TEST_CASE_RESULT_OK;
}

#ifdef UTF8STRINGBUF_DEPRECATED_INDEX
static test_case_result_t testFindFirst( test_fixture_t *fix )
{
    int pos;
    char srchArr1[] = "aaaaaaaaaaaaaaaaaaaaaaa";
    utf8stringbuf_t srchBuf1 = utf8stringbuf(srchArr1);
    char srchArr2[100] = "aaaaaaaaaaaaaaaaaaaaZaa";
    utf8stringbuf_t srchBuf2 = utf8stringbuf(srchArr2);
    char srchArr3[] = "N/A";
    utf8stringbuf_t srchBuf3 = UTF8STRINGBUF(srchArr3);

    pos = utf8stringbuf_find_first_str( (*fix).mega_byte_buf, srchArr1);
    TEST_EXPECT_EQUAL_INT( 0, pos );

    pos = utf8stringbuf_find_first_str( (*fix).mega_byte_buf, srchArr2);
    TEST_EXPECT_EQUAL_INT( 980, pos );

    pos = utf8stringbuf_find_first_str( (*fix).mega_byte_buf, "");
    TEST_EXPECT_EQUAL_INT( 0, pos );

    pos = utf8stringbuf_find_first_str( (*fix).mega_byte_buf, NULL);
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_first_str( (*fix).mega_byte_buf, srchArr3);
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_first_str( (*fix).one_byte_buf, srchArr3);
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_first_str( srchBuf3, srchArr3);
    TEST_EXPECT_EQUAL_INT( 0, pos );


    pos = utf8stringbuf_find_first_buf( (*fix).mega_byte_buf, srchBuf1);
    TEST_EXPECT_EQUAL_INT( 0, pos );

    pos = utf8stringbuf_find_first_buf( (*fix).mega_byte_buf, srchBuf2);
    TEST_EXPECT_EQUAL_INT( 980, pos );

    pos = utf8stringbuf_find_first_buf( (*fix).mega_byte_buf, (*fix).one_byte_buf);
    TEST_EXPECT_EQUAL_INT( 0, pos );

    pos = utf8stringbuf_find_first_buf( (*fix).mega_byte_buf, srchBuf3);
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_first_buf( (*fix).one_byte_buf, srchBuf3);
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_first_buf( srchBuf3, srchBuf3);
    TEST_EXPECT_EQUAL_INT( 0, pos );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_DEPRECATED_INDEX */

#ifdef UTF8STRINGBUF_DEPRECATED_INDEX
static test_case_result_t testFindNext( test_fixture_t *fix )
{
    int pos;
    char srchArr1[10] = "aaaa";
    utf8stringbuf_t srchBuf1 = UTF8STRINGBUF(srchArr1);
    char srchArr2[] = "aaaZaaa";
    utf8stringbuf_t srchBuf2 = utf8stringbuf(srchArr2);
    char srchArr3[] = "N/A";
    utf8stringbuf_t srchBuf3 = UTF8STRINGBUF(srchArr3);

    pos = utf8stringbuf_find_next_buf( (*fix).one_byte_buf, srchBuf3, -17 );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_next_buf( (*fix).one_byte_buf, srchBuf3, 17 );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_next_buf( (*fix).one_byte_buf, srchBuf3, 0 );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_next_buf( srchBuf1, (*fix).mega_byte_buf, 1 );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_next_buf( (*fix).mega_byte_buf, srchBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( 0, pos );

    pos = utf8stringbuf_find_next_buf( (*fix).mega_byte_buf, srchBuf1, 1 );
    TEST_EXPECT_EQUAL_INT( 1, pos );

    pos = utf8stringbuf_find_next_buf( (*fix).mega_byte_buf, srchBuf2, 1 );
    TEST_EXPECT_EQUAL_INT( 997, pos );

    pos = utf8stringbuf_find_next_buf( (*fix).mega_byte_buf, srchBuf2, 1001 );
    TEST_EXPECT_EQUAL_INT( 1001, pos );

    pos = utf8stringbuf_find_next_buf( (*fix).mega_byte_buf, srchBuf2, 1002 );
    TEST_EXPECT_EQUAL_INT( 899997, pos );

    pos = utf8stringbuf_find_next_buf( (*fix).mega_byte_buf, srchBuf2, 899998 );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_next_buf( (*fix).mega_byte_buf, srchBuf3, 1000 );
    TEST_EXPECT_EQUAL_INT( -1, pos );


    pos = utf8stringbuf_find_next_str( (*fix).mega_byte_buf, NULL, 1000 );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_next_str( (*fix).mega_byte_buf, srchArr1, 1000 );
    TEST_EXPECT_EQUAL_INT( 1005, pos );

    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_DEPRECATED_INDEX */

#ifdef UTF8STRINGBUF_DEPRECATED_INDEX
static test_case_result_t testFindLast( test_fixture_t *fix )
{
    int pos;
    char srchArr1[10] = "aaaa";
    utf8stringbuf_t srchBuf1 = UTF8STRINGBUF(srchArr1);
    char srchArr2[] = "aaaZaaa";
    utf8stringbuf_t srchBuf2 = utf8stringbuf(srchArr2);
    char srchArr3[] = "N/A";
    utf8stringbuf_t srchBuf3 = UTF8STRINGBUF(srchArr3);

    pos = utf8stringbuf_find_last_buf( (*fix).one_byte_buf, srchBuf3 );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_last_buf( srchBuf1, (*fix).mega_byte_buf );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_last_buf( (*fix).mega_byte_buf, srchBuf1 );
    TEST_EXPECT_EQUAL_INT( UTF8STRINGBUFTEST_MEGASIZE - 1 - 4, pos );

    pos = utf8stringbuf_find_last_buf( (*fix).mega_byte_buf, srchBuf2 );
    TEST_EXPECT_EQUAL_INT( 899997, pos );

    pos = utf8stringbuf_find_last_buf( (*fix).mega_byte_buf, srchBuf3 );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_last_buf( srchBuf2, srchBuf2 );
    TEST_EXPECT_EQUAL_INT( 0, pos );

    pos = utf8stringbuf_find_last_str( (*fix).mega_byte_buf, NULL );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_last_str( (*fix).mega_byte_buf, "" );
    TEST_EXPECT_EQUAL_INT( UTF8STRINGBUFTEST_MEGASIZE - 1, pos );

    pos = utf8stringbuf_find_last_str( srchBuf2, "aaa" );
    TEST_EXPECT_EQUAL_INT( 4, pos );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_DEPRECATED_INDEX */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
static test_case_result_t testCopyRegion( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[3] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
    char dynTestArr2[8] = "Hello W";
    utf8stringbuf_t dynTestBuf2 = UTF8STRINGBUF(dynTestArr2);

    /* check utf8stringbuf_copy_region_from_buf: standard cases */
    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, 0, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, 5, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, " W" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_copy_region_from_buf: illegal ranges */
    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, -1, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, 0, -2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, 0, 0 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_copy_region_from_buf: out of bounds */
    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, 6, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "W" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, 8, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, 1, 4 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "el" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_copy_region_from_str */
    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 0, 4 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 0, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 3, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "lo" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 4, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "o" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 5, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 7, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", -1, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 3, 0 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 3, -1 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "\xC2\xA2_\xC2\xA2", 2, 3 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "_" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "\xC2\xA2_\xC2\xA2", 0, 3 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "\xC2\xA2" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "\xC2\xA2_\xC2\xA2", 3, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "\xC2\xA2" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, NULL, 0, 0 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
static test_case_result_t testReplaceRegion( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[15] = "";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(13,&(dynTestArr1[1]));

    /* check that markers at start and end are not overwritten */
    dynTestArr1[0]=127;
    dynTestArr1[14]=127;

    /* check utf8stringbuf_replace_region_by_str: standard cases */
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 0, 0, "all inserted" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "all inserted" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 0, 12, "all replaced" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "all replaced" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 1, 5, " 5 " );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "a 5 placed" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 1, "333" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    /*printf("%s",dynTestArr1);*/
    equal = utf8stringbuf_equals_str( dynTestBuf1, "a 333 placed" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 1, 10, "" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "ad" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 0, "." );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "ad." );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 0, 3, NULL );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check that markers at start and end are not overwritten */
    TEST_EXPECT_EQUAL_INT( 127, dynTestArr1[0] );
    TEST_EXPECT_EQUAL_INT( 127, dynTestArr1[14] );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
static test_case_result_t testReplaceRegionExceededRanges( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[15] = "";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(13,&(dynTestArr1[1]));

    /* check that markers at start and end are not overwritten */
    dynTestArr1[0]=127;
    dynTestArr1[14]=127;

    /* check utf8stringbuf_replace_region_by_str: range exceeded cases */
    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, -1, 2, "replaced" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 0, 12, "replaced" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "replaced" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 12, "." );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He." );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 0, 14, "replaced" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "replaced" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    /*TEST_EXPECT_EQUAL_INT( 127, dynTestArr1[14] );*/

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 6, 0, "replaced" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    /*TEST_EXPECT_EQUAL_INT( 127, dynTestArr1[14] );*/

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 5, 0, "12345678" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello1234567" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 0, "12345678" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He12345678ll" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 0, "1234567890" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He1234567890" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 0, "1234567890A" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He1234567890" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check that markers at start and end are not overwritten */
    TEST_EXPECT_EQUAL_INT( 127, dynTestArr1[0] );
    TEST_EXPECT_EQUAL_INT( 127, dynTestArr1[14] );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
static test_case_result_t testReplaceRegionWithCutUtf8( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[15] = "";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(13,&(dynTestArr1[1]));

    /* check that markers at start and end are not overwritten */
    dynTestArr1[0]=127;
    dynTestArr1[14]=127;

    /* check utf8stringbuf_copy_str, \xE2\x82\xAC is the euro symbol in utf8 */
    utf8stringbuf_copy_str( dynTestBuf1, "Hello \xE2\x82\xAC" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 0, "1234" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He1234llo " );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 0, "12345678\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He12345678" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello " );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 6, 0, "1234\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello 1234" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check that markers at start and end are not overwritten */
    TEST_EXPECT_EQUAL_INT( 127, dynTestArr1[0] );
    TEST_EXPECT_EQUAL_INT( 127, dynTestArr1[14] );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
static test_case_result_t testReplaceRegionBuf( test_fixture_t *fix )
{
    /* utf8stringbuf_replace_region_by_buf is just a wrapper around utf8stringbuf_replace_region_by_str */
    /* therefore, we do only one test */
    utf8error_t error;
    int equal;
    char dynTestArr1[7] = "Helllo";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
    char dynTestArr2[] = "ll";
    utf8stringbuf_t dynTestBuf2 = UTF8STRINGBUF(dynTestArr2);

    /* check utf8stringbuf_replace_region_by_buf */
    error = utf8stringbuf_replace_region_by_buf( dynTestBuf1, 2, 3, dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

static test_case_result_t testReplaceAll ( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[20] = "He&l<'";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_replace_all */
    error = utf8stringbuf_replace_all( dynTestBuf1, &XML_ENCODE );
    /*printf( "%s", utf8stringbuf_get_string(dynTestBuf1) );*/
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He&amp;l&lt;&apos;" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_all( dynTestBuf1, &SQL_ENCODE );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He&amp;l&lt;&apos;" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_all( dynTestBuf1, &XML_ENCODE );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He&amp;amp;l&amp;lt" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* change nearly everything */
    error = utf8stringbuf_replace_all( (*fix).mega_byte_buf, &TO_UPPER_CASE );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_starts_with_str( (*fix).mega_byte_buf, "AAAAAAAAAA" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* change everything */
    error = utf8stringbuf_replace_all( (*fix).mega_byte_buf, &TO_LOWER_CASE );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_starts_with_str( (*fix).mega_byte_buf, "aaaaaaaaaaaaaaaaaaaaaaaaaaa" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* change nothing */
    error = utf8stringbuf_replace_all( (*fix).mega_byte_buf, &TO_LOWER_CASE );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_starts_with_str( (*fix).mega_byte_buf, "aaaaaaaaaaaaaaaaaaaaaaaaaaa" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* shrink because performance hungry function */
    (*fix).mega_byte_arr[20000] = '\0';
    /* utf8stringbuf_delete_to_end( (*fix).mega_byte_buf, 20000 ); */

    /* shrink at each replacement */
    error = utf8stringbuf_replace_all( (*fix).mega_byte_buf, &SHRINK_DUPLICATES_EXCEPT_Z );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_starts_with_str( (*fix).mega_byte_buf, "AAAAAAAAAA" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* grow at each replacement */
    error = utf8stringbuf_replace_all( (*fix).mega_byte_buf, &EXPAND_SINGLES_EXCEPT_Z );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_starts_with_str( (*fix).mega_byte_buf, "aaaaaaaaaaaaaaaaaaaaaaaaaaa" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    return TEST_CASE_RESULT_OK;
}

static const char *const TEST_PATTERNS[][2] = {
         { "", "" },
         { "", NULL },
         { "Z", NULL },
         { "Y", "yyy" },
         { "Hello", "Hello World" },
         { "World", "Hello World" },
         { "Hello World", "" },
         { "\xE2\x82\xAC", "EUR" },
         { "EUR", "\xE2\x82\xAC" },
         { "5E", "5 \xE2\x82\xAC" },
         { NULL, "anything"  } };

static test_case_result_t testReplaceAllBadCases( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[32] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
    char dynTestArr2[8] = "";
    utf8stringbuf_t dynTestBuf2 = UTF8STRINGBUF(dynTestArr2);

    /* check utf8stringbuf_replace_all */
    utf8stringbuf_copy_str( dynTestBuf1, "Hello World" );
    error = utf8stringbuf_replace_all( dynTestBuf1, &TEST_PATTERNS );
    /*printf( "%s", utf8stringbuf_get_string(dynTestBuf1) );*/
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello World Hello World" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* replace everything, including shrinking */
    utf8stringbuf_copy_str( dynTestBuf2, "\xE2\x82\xAC" "Z" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "EUREUR" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* test null pattern*/
    utf8stringbuf_copy_str( dynTestBuf2, "\xE2\x82\xAC" "Z" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, NULL );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "\xE2\x82\xAC" "Z" "\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* try to cut trailing string code point */
    utf8stringbuf_copy_str( dynTestBuf2, "\xE2\x82\xAC" "Y" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "EURyyy" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* first shrink, then grow, fit exactly*/
    utf8stringbuf_copy_str( dynTestBuf2, "ZZaY" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "ayyyEUR" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* first shrink, then grow, cut after second byte of multibyte code point*/
    utf8stringbuf_copy_str( dynTestBuf2, "ZaaY" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "aayyy" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* first grow, then shrink, cut after first byte of multibyte code point*/
    utf8stringbuf_copy_str( dynTestBuf2, "YZZa" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "yyya" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* first grow, then shrink, cut after second byte of multibyte code point*/
    utf8stringbuf_copy_str( dynTestBuf2, "YZa" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "yyya" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* try to cut replacement string*/
    utf8stringbuf_copy_str( dynTestBuf2, "aZWorld" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "aHello " );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* try to cut replacement code point */
    utf8stringbuf_copy_str( dynTestBuf2, "Y5EHell" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "yyy5 " );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* try to cut trailing string code point */
    utf8stringbuf_copy_str( dynTestBuf2, "a5E" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "a5 " "\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testReplaceAllStr( test_fixture_t *fix )
{
    /* utf8stringbuf_replace_all_str_by_str is just a wrapper around utf8stringbuf_replace_all */
    /* therefore, we do only two tests */
    utf8error_t error;
    int equal;
    char dynTestArr1[7] = "Helllo";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_replace_all_str_by_str */
    error = utf8stringbuf_replace_all_str_by_str( dynTestBuf1, NULL, "" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Helllo" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_all_str_by_str( dynTestBuf1, "lo", "o!" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello!" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}

#ifdef UTF8STRINGBUF_DEPRECATED_INPLACE
static test_case_result_t testReplaceAllBuf( test_fixture_t *fix )
{
    /* utf8stringbuf_replace_all_buf_by_buf is just a wrapper around utf8stringbuf_replace_all */
    /* therefore, we do only one test */
    utf8error_t error;
    int equal;
    char dynTestArr1[7] = "Helllo";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_replace_all_buf_by_buf */
    error = utf8stringbuf_replace_all_buf_by_buf( dynTestBuf1, utf8stringbuf("ll"), utf8stringbuf("l") );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_DEPRECATED_INPLACE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
static test_case_result_t testInsertBuf( test_fixture_t *fix )
{
    /* utf8stringbuf_insert_buf is just a wrapper around utf8stringbuf_replace_region_by_str */
    /* therefore, we do only one test */
    utf8error_t error;
    int equal;
    char dynTestArr1[6] = "Heo";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
    char dynTestArr2[] = "ll";
    utf8stringbuf_t dynTestBuf2 = UTF8STRINGBUF(dynTestArr2);

    /* check utf8stringbuf_insert_buf */
    error = utf8stringbuf_insert_buf( dynTestBuf1, 2, dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
static test_case_result_t testInsertStr( test_fixture_t *fix )
{
    /* utf8stringbuf_insert_str is just a wrapper around utf8stringbuf_replace_region_by_str */
    /* therefore, we do only one test */
    utf8error_t error;
    int equal;
    char dynTestArr1[6] = "He";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_insert_str */
    error = utf8stringbuf_insert_str( dynTestBuf1, 2, "llo" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
static test_case_result_t testDelete( test_fixture_t *fix )
{
    /* utf8stringbuf_delete is just a wrapper around utf8stringbuf_replace_region_by_str */
    /* therefore, we do only one test */
    utf8error_t error;
    int equal;
    char dynTestArr1[12] = "Hellllllllo";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_delete */
    error = utf8stringbuf_delete( dynTestBuf1, 4, 6 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
static test_case_result_t testDeleteFromEnd( test_fixture_t *fix )
{
    /* utf8stringbuf_delete is just a wrapper around utf8stringbuf_replace_region_by_str */
    /* therefore, we do only one test */
    utf8error_t error;
    int equal;
    char dynTestArr1[12] = "Hellllllllo";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_delete_from_end */
    error = utf8stringbuf_delete_from_end( dynTestBuf1, 7 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hell" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
static test_case_result_t testDeleteToEnd( test_fixture_t *fix )
{
    /* utf8stringbuf_delete is just a wrapper around utf8stringbuf_replace_region_by_str */
    /* therefore, we do only one test */
    utf8error_t error;
    int equal;
    char dynTestArr1[12] = "Hellllllllo";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_delete_to_end */
    error = utf8stringbuf_delete_to_end( dynTestBuf1, 4 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hell" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

static test_case_result_t testAppendStr( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[6] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_append_str */
    utf8stringbuf_copy_str( dynTestBuf1, "Hell" );
    error = utf8stringbuf_append_str( dynTestBuf1, "o" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "" );
    error = utf8stringbuf_append_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_str( dynTestBuf1, NULL );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hell" );
    error = utf8stringbuf_append_str( dynTestBuf1, "\xC2\xA2" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hell" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hell" );
    error = utf8stringbuf_append_str( dynTestBuf1, "o!" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hel" );
    error = utf8stringbuf_append_str( dynTestBuf1, "\xC2\xA2!" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hel\xC2\xA2" );
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
    utf8stringbuf_copy_str( dynTestBuf1, "He" );
    error = utf8stringbuf_append_buf( dynTestBuf1, dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hel\xC2\xA2" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hel" );
    error = utf8stringbuf_append_buf( dynTestBuf1, dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hell" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_buf( dynTestBuf1, dynTestBuf2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
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
    utf8stringbuf_copy_str( dynTestBuf1, "12" );
    error = utf8stringbuf_append_int( dynTestBuf1, 345 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "12345" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "" );
    error = utf8stringbuf_append_int( dynTestBuf1, -1234 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "-1234" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "1234" );
    error = utf8stringbuf_append_int( dynTestBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "12340" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "12345" );
    error = utf8stringbuf_append_int( dynTestBuf1, 6 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "12345" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "1" );
    error = utf8stringbuf_append_int( dynTestBuf1, 234567 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "12345" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "1234" );
    error = utf8stringbuf_append_int( dynTestBuf1, -1 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "1234-" );
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
    utf8stringbuf_copy_str( dynTestBuf1, "0x" );
    error = utf8stringbuf_append_hex( dynTestBuf1, -1 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "0xfff" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "0x" );
    error = utf8stringbuf_append_hex( dynTestBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "0x0" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "0x" );
    error = utf8stringbuf_append_hex( dynTestBuf1, 0x1000555 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "0x100" );
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
    utf8stringbuf_copy_str( dynTestBuf1, "Hell" );
    error = utf8stringbuf_append_char( dynTestBuf1, 'o' );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append one byte too much */
    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_char( dynTestBuf1, 'o' );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_char */
    utf8stringbuf_copy_str( dynTestBuf1, "He" );
    error = utf8stringbuf_append_char( dynTestBuf1, 0x20ac );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append one byte too much */
    utf8stringbuf_copy_str( dynTestBuf1, "Hel" );
    error = utf8stringbuf_append_char( dynTestBuf1, 0x20ac );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hel" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append zero */
    utf8stringbuf_clear( dynTestBuf1 );
    error = utf8stringbuf_append_char( dynTestBuf1, 0x0 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0x1 1-byte code point */
    utf8stringbuf_clear( dynTestBuf1 );
    error = utf8stringbuf_append_char( dynTestBuf1, 0x01 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "\x01" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0x7f 1-byte code point */
    utf8stringbuf_clear( dynTestBuf1 );
    error = utf8stringbuf_append_char( dynTestBuf1, 0x7f );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "\x7f" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0x80 2-byte code point */
    utf8stringbuf_clear( dynTestBuf1 );
    error = utf8stringbuf_append_char( dynTestBuf1, 0x80 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "\xC2\x80" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0x7ff 2-byte code point */
    utf8stringbuf_clear( dynTestBuf1 );
    error = utf8stringbuf_append_char( dynTestBuf1, 0x07ff );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "\xDF\xBF" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0x800 3-byte code point */
    utf8stringbuf_clear( dynTestBuf1 );
    error = utf8stringbuf_append_char( dynTestBuf1, 0x800 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "\xE0\xA0\x80" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0xffff 3-byte code point */
    utf8stringbuf_clear( dynTestBuf1 );
    error = utf8stringbuf_append_char( dynTestBuf1, 0xffff );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "\xEF\xBF\xBF" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0x10000 4-byte code point */
    utf8stringbuf_clear( dynTestBuf1 );
    error = utf8stringbuf_append_char( dynTestBuf1, 0x10000 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "\xF0\x90\x80\x80" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append 0x10ffff 4-byte code point */
    utf8stringbuf_clear( dynTestBuf1 );
    error = utf8stringbuf_append_char( dynTestBuf1, 0x10ffff );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "\xF4\x8F\xBF\xBF" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* append illegal code point */
    utf8stringbuf_clear( dynTestBuf1 );
    error = utf8stringbuf_append_char( dynTestBuf1, 0x80000000 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_A_CODEPOINT, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
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
    utf8stringbuf_copy_str( dynTestBuf1, "Hel" );
    error = utf8stringbuf_append_wstr( dynTestBuf1, wideString );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_wstr with multibyte */
    const wchar_t *wideString2 = L"e\x20ac";
    utf8stringbuf_copy_str( dynTestBuf1, "H" );
    error = utf8stringbuf_append_wstr( dynTestBuf1, wideString2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_wstr with truncation */
    utf8stringbuf_copy_str( dynTestBuf1, "He" );
    error = utf8stringbuf_append_wstr( dynTestBuf1, wideString2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hee" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_wstr( dynTestBuf1, wideString2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_wstr with NULL */
    utf8stringbuf_copy_str( dynTestBuf1, "He" );
    error = utf8stringbuf_append_wstr( dynTestBuf1, NULL );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He" );
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
    utf8stringbuf_copy_view( dynTestBuf1, &view0 );
    error = utf8stringbuf_append_view( dynTestBuf1, &view1 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_view with multibyte */
    const utf8stringview_t view2 = UTF8STRINGVIEW_STR( "e\xE2\x82\xAC" );
    utf8stringbuf_copy_str( dynTestBuf1, "H" );
    error = utf8stringbuf_append_view( dynTestBuf1, &view2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He\xE2\x82\xAC" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_view with truncation */
    utf8stringbuf_copy_str( dynTestBuf1, "He" );
    error = utf8stringbuf_append_view( dynTestBuf1, &view2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hee" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_view( dynTestBuf1, &view2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_view with NULL */
    const utf8stringview_t no_view = UTF8STRINGVIEW_EMPTY;
    utf8stringbuf_copy_str( dynTestBuf1, "He" );
    error = utf8stringbuf_append_view( dynTestBuf1, &no_view );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    return TEST_CASE_RESULT_OK;
}

#ifdef UTF8STRINGBUF_DEPRECATED_INDEX
static test_case_result_t testCharAt( test_fixture_t *fix )
{
    utf8codepoint_t result;
    char dynTestArr1[6] = "He\xE2\x82\xAC";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_get_char_at */
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 'H', utf8codepoint_get_char(result) );

    result = utf8stringbuf_get_char_at( dynTestBuf1, 2 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( 3, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 0x20ac, utf8codepoint_get_char(result) );

    result = utf8stringbuf_get_char_at( dynTestBuf1, 3 );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 0x0, utf8codepoint_get_char(result) );

    result = utf8stringbuf_get_char_at( dynTestBuf1, 5 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 0x0, utf8codepoint_get_char(result) );

    /* check example from documentation */
    char testArr[] = "\xC2\xAE 2005 A.A. \xC2\xAE 2006-2012 B.B.";
    utf8stringbuf_t testBuf = UTF8STRINGBUF(testArr);
    result = utf8stringbuf_get_char_at( testBuf, 13 );
    TEST_EXPECT_EQUAL_INT( 0xae, utf8codepoint_get_char(result) );

    /* check undefined behaviour */
    result = utf8stringbuf_get_char_at( dynTestBuf1, 6 );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 0x0, utf8codepoint_get_char(result) );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_DEPRECATED_INDEX */

#ifdef UTF8STRINGBUF_DEPRECATED_INDEX
static test_case_result_t testCharAtLoops( test_fixture_t *fix )
{
    utf8codepoint_t result;
    char dynTestArr1[6] = "He\xE2\x82\xAC";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* loop over all bytes, independant of code-points */
    unsigned int countCodePoints = 0;
    unsigned int byte_length = utf8stringbuf_get_length( dynTestBuf1 );
    for ( int idx = 0; idx < byte_length; idx ++ ) {
        result = utf8stringbuf_get_char_at( dynTestBuf1, idx );
        if ( utf8codepoint_is_valid(result) ) {
            countCodePoints ++;
        }
    }
    TEST_EXPECT_EQUAL_INT( 3, countCodePoints );

    /* loop over all code points */
    countCodePoints = 0;
    byte_length = utf8stringbuf_get_length( dynTestBuf1 );
    for ( int idx = 0; idx < byte_length; ) {
        result = utf8stringbuf_get_char_at( dynTestBuf1, idx );
        if ( utf8codepoint_is_valid(result) ) {
            countCodePoints ++;
            idx += utf8codepoint_get_length(result);
        }
        else {
            break;
        }
    }
    TEST_EXPECT_EQUAL_INT( 3, countCodePoints );

    /* loop over buffer size */
    /* this is possibly the fastest way to loop over the buffer contents */
    countCodePoints = 0;
    unsigned int byteSize = utf8stringbuf_get_size( dynTestBuf1 );
    for ( int idx = 0; idx < byteSize; ) {
        result = utf8stringbuf_get_char_at( dynTestBuf1, idx );
        if ( utf8codepoint_is_valid(result) && ( utf8codepoint_get_char(result) != '\0' )) {
            countCodePoints ++;
            idx += utf8codepoint_get_length(result);
        }
        else {
            break;
        }
    }
    TEST_EXPECT_EQUAL_INT( 3, countCodePoints );

    /* safely loop over buffer size, using a position-index and a loop-limit-counter */
    countCodePoints = 0;
    byteSize = utf8stringbuf_get_size( dynTestBuf1 );
    unsigned int currentIndex = 0;
    for ( int loopCount = 0; loopCount < byteSize; loopCount ++ ) {
        result = utf8stringbuf_get_char_at( dynTestBuf1, currentIndex );
        if ( utf8codepoint_is_valid(result) && ( utf8codepoint_get_char(result) != '\0' )) {
            countCodePoints ++;
            currentIndex += utf8codepoint_get_length(result);
        }
        else {
            break;
        }
    }
    TEST_EXPECT_EQUAL_INT( 3, countCodePoints );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_DEPRECATED_INDEX */

static test_case_result_t testGetEnd( test_fixture_t *fix )
{
    utf8error_t error;
    int equal;
    char dynTestArr1[4] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
    utf8stringbuf_t dynTestEnd;

    //  test utf8stringbuf_get_end
    dynTestEnd = utf8stringbuf_get_end( dynTestBuf1 );
    error = utf8stringbuf_append_str( dynTestEnd, "Aa" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Aa" );
    TEST_EXPECT_EQUAL_INT( 1, equal );

    dynTestEnd = utf8stringbuf_get_end( dynTestBuf1 );
    error = utf8stringbuf_append_str( dynTestEnd, "Bb" );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "AaB" );
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
