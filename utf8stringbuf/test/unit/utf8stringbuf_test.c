/* File: utf8stringbuf_test.c; Copyright and License: see below */

#include "utf8stringbuf_test.h"
#include "util/string/utf8stringbuf.h"
#include "utf8stringbuf_cross_language.h"
#include "test_assert.h"
#include <string.h>
#include <pthread.h>
#include <assert.h>

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

const char END[] = "";

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

static char oneByteArr[] = "";
static utf8stringbuf_t oneByteBuf = UTF8STRINGBUF(oneByteArr);

static char fourByteArr[4] = "12";
static utf8stringbuf_t fourByteBuf = UTF8STRINGBUF(fourByteArr);

struct InitTestStruct {
    char urlArr[8192];
    utf8stringbuf_t url;
    pthread_mutex_t lock;
};
typedef struct InitTestStruct InitTest_t;
static InitTest_t structTest = { "http://", UTF8STRINGBUF( structTest.urlArr), PTHREAD_MUTEX_INITIALIZER, };

#define INITTEST(testStr,this_) { testStr, UTF8STRINGBUF( this_.urlArr), PTHREAD_MUTEX_INITIALIZER, }
static InitTest_t structArrTest[] = {
        INITTEST( "svn://first", structArrTest[0] ),
        INITTEST( "http://second", structArrTest[1] ),
        INITTEST( "ftp://third", structArrTest[2] ),
        INITTEST( "file://last", structArrTest[3] ),
};

static char ThousandPathNames[1000][256];
#define PATH_INIT(x) UTF8STRINGBUF( ThousandPathNames[x] )
#define FIVE_PATHS_INIT(x) PATH_INIT(x+0), PATH_INIT(x+1), PATH_INIT(x+2), PATH_INIT(x+3), PATH_INIT(x+4)
#define TWENTY_PATHS_INIT(x) FIVE_PATHS_INIT(x+0), FIVE_PATHS_INIT(x+5), FIVE_PATHS_INIT(x+10), FIVE_PATHS_INIT(x+15)
#define HUNDRED_PATHS_INIT(x) TWENTY_PATHS_INIT(x+0), TWENTY_PATHS_INIT(x+20), TWENTY_PATHS_INIT(x+40), TWENTY_PATHS_INIT(x+60), TWENTY_PATHS_INIT(x+80)
static utf8stringbuf_t ThousandPaths[1000] = {
        HUNDRED_PATHS_INIT(0), HUNDRED_PATHS_INIT(100), HUNDRED_PATHS_INIT(200), HUNDRED_PATHS_INIT(300),
        HUNDRED_PATHS_INIT(400), HUNDRED_PATHS_INIT(500), HUNDRED_PATHS_INIT(600), HUNDRED_PATHS_INIT(700),
        HUNDRED_PATHS_INIT(800), HUNDRED_PATHS_INIT(900),
};

enum { UTF8STRINGBUFTEST_MEGASIZE = (1024*1024), };
static char megaByteArr[UTF8STRINGBUFTEST_MEGASIZE] = "";
static utf8stringbuf_t megaByteBuf = UTF8STRINGBUF(megaByteArr);

static void setUp(void);
static void tearDown(void);
static void testInit(void);
static void testClear(void);
static void testLength(void);
static void testEquals(void);
static void testEqualsRegion(void);
static void testStartsWith(void);
static void testEndsWith(void);
static void testFindFirst(void);
static void testFindNext(void);
static void testFindLast(void);
static void testCopyBuf(void);
static void testCopyStr(void);
static void testCopyWithCutUtf8(void);
static void testCopyRegion(void);
static void testReplaceRegion(void);
static void testReplaceRegionExceededRanges(void);
static void testReplaceRegionWithCutUtf8(void);
static void testReplaceRegionBuf(void);
static void testReplaceAll(void);
static void testReplaceAllBadCases(void);
static void testReplaceAllStr(void);
static void testReplaceAllBuf(void);
static void testInsertBuf(void);
static void testInsertStr(void);
static void testDelete(void);
static void testDeleteToEnd(void);
static void testDeleteFromEnd(void);
static void testAppendStr(void);
static void testAppendBuf(void);
static void testAppendInt(void);
static void testAppendHex(void);
static void testAppendWStr(void);
static void testAppendChar(void);
static void testCharAt(void);
static void testCharAtLoops(void);
#ifdef __cplusplus
static void testCrossLanguage(void);
#endif
static void testSplitIn2(void);
static void testSplitIn3(void);
static void testSplitIn4(void);
static void testSplitIn5(void);
static void testJoin(void);
static void testSplitAndJoin(void);
static void testGetEnd(void);

test_suite_t utf8stringbuf_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "utf8StringBufTest", &setUp, &tearDown );
    test_suite_add_test_case( &result, "testInit", &testInit );
    test_suite_add_test_case( &result, "testClear", &testClear );
    test_suite_add_test_case( &result, "testLength", &testLength );
    test_suite_add_test_case( &result, "testEquals", &testEquals );
    test_suite_add_test_case( &result, "testEqualsRegion", &testEqualsRegion );
    test_suite_add_test_case( &result, "testStartsWith", &testStartsWith );
    test_suite_add_test_case( &result, "testEndsWith", &testEndsWith );
    test_suite_add_test_case( &result, "testFindFirst", &testFindFirst );
    test_suite_add_test_case( &result, "testFindNext", &testFindNext );
    test_suite_add_test_case( &result, "testFindLast", &testFindLast );
    test_suite_add_test_case( &result, "testCopyBuf", &testCopyBuf );
    test_suite_add_test_case( &result, "testCopyStr", &testCopyStr );
    test_suite_add_test_case( &result, "testCopyWithCutUtf8", &testCopyWithCutUtf8 );
    test_suite_add_test_case( &result, "testCopyRegion", &testCopyRegion );
    test_suite_add_test_case( &result, "testReplaceRegion", &testReplaceRegion );
    test_suite_add_test_case( &result, "testReplaceRegionExceededRanges", &testReplaceRegionExceededRanges );
    test_suite_add_test_case( &result, "testReplaceRegionWithCutUtf8", &testReplaceRegionWithCutUtf8 );
    test_suite_add_test_case( &result, "testReplaceRegionBuf", &testReplaceRegionBuf );
    test_suite_add_test_case( &result, "testReplaceAll", &testReplaceAll );
    test_suite_add_test_case( &result, "testReplaceAllBadCases", &testReplaceAllBadCases );
    test_suite_add_test_case( &result, "testReplaceAllStr", &testReplaceAllStr );
    test_suite_add_test_case( &result, "testReplaceAllBuf", &testReplaceAllBuf );
    test_suite_add_test_case( &result, "testInsertBuf", &testInsertBuf );
    test_suite_add_test_case( &result, "testInsertStr", &testInsertStr );
    test_suite_add_test_case( &result, "testDelete", &testDelete );
    test_suite_add_test_case( &result, "testDeleteToEnd", &testDeleteToEnd );
    test_suite_add_test_case( &result, "testDeleteFromEnd", &testDeleteFromEnd );
    test_suite_add_test_case( &result, "testAppendStr", &testAppendStr );
    test_suite_add_test_case( &result, "testAppendBuf", &testAppendBuf );
    test_suite_add_test_case( &result, "testAppendInt", &testAppendInt );
    test_suite_add_test_case( &result, "testAppendHex", &testAppendHex );
    test_suite_add_test_case( &result, "testAppendChar", &testAppendChar );
    test_suite_add_test_case( &result, "testAppendWStr", &testAppendWStr );
    test_suite_add_test_case( &result, "testCharAt", &testCharAt );
    test_suite_add_test_case( &result, "testCharAtLoops", &testCharAtLoops );
#ifdef __cplusplus
    test_suite_add_test_case( &result, "testCrossLanguage", &testCrossLanguage );
#endif
    test_suite_add_test_case( &result, "testSplitIn2", &testSplitIn2 );
    test_suite_add_test_case( &result, "testSplitIn3", &testSplitIn3 );
    test_suite_add_test_case( &result, "testSplitIn4", &testSplitIn4 );
    test_suite_add_test_case( &result, "testSplitIn5", &testSplitIn5 );
    test_suite_add_test_case( &result, "testJoin", &testJoin );
    test_suite_add_test_case( &result, "testSplitAndJoin", &testSplitAndJoin );
    test_suite_add_test_case( &result, "testGetEnd", &testGetEnd );
    return result;
}

static void setUp(void)
{
    memset( megaByteArr, 'a', UTF8STRINGBUFTEST_MEGASIZE-1 );
    megaByteArr[UTF8STRINGBUFTEST_MEGASIZE-1] = '\0';
    megaByteArr[1000] = 'Z';
    megaByteArr[1004] = 'Z';
    megaByteArr[900000] = 'Z';
}

static void tearDown(void)
{
}

static void testInit(void)
{
    /* check static initialization */
    TEST_ASSERT( utf8stringbuf_get_string( oneByteBuf ) == oneByteArr );
    TEST_ASSERT_EQUAL_INT( 1, utf8stringbuf_get_size( oneByteBuf ) );
    TEST_ASSERT( utf8stringbuf_get_string( fourByteBuf ) == fourByteArr );
    TEST_ASSERT_EQUAL_INT( 4, utf8stringbuf_get_size( fourByteBuf ) );
    TEST_ASSERT( utf8stringbuf_get_string( megaByteBuf ) == megaByteArr );
    TEST_ASSERT_EQUAL_INT( UTF8STRINGBUFTEST_MEGASIZE, utf8stringbuf_get_size( megaByteBuf ) );

    /* check dynamic initialization */
    char dynTestArr[] = "Hello";
    utf8stringbuf_t dynTestBuf = utf8stringbuf(dynTestArr);
    TEST_ASSERT( utf8stringbuf_get_string( dynTestBuf ) == dynTestArr );
    TEST_ASSERT_EQUAL_INT( 6, utf8stringbuf_get_size( dynTestBuf ) );

    /* compile time initialization on stack */
    char dynTestArr2[17] = "";
    utf8stringbuf_t dynTestBuf2 = UTF8STRINGBUF( dynTestArr2 );
    TEST_ASSERT( utf8stringbuf_get_string( dynTestBuf2 ) == dynTestArr2 );
    TEST_ASSERT_EQUAL_INT( 17, utf8stringbuf_get_size( dynTestBuf2 ) );

    /* check static struct initialization */
    TEST_ASSERT( utf8stringbuf_get_string( structTest.url ) == structTest.urlArr );
    TEST_ASSERT_EQUAL_INT( 8192, utf8stringbuf_get_size( structTest.url ) );

    /* check static struct-array initialization */
    TEST_ASSERT( utf8stringbuf_get_string( structArrTest[0].url ) == structArrTest[0].urlArr );
    TEST_ASSERT_EQUAL_INT( 8192, utf8stringbuf_get_size( structArrTest[0].url ) );
    TEST_ASSERT( utf8stringbuf_equals_str( structArrTest[0].url, "svn://first" ));
    TEST_ASSERT( utf8stringbuf_get_string( structArrTest[1].url ) == structArrTest[1].urlArr );
    TEST_ASSERT_EQUAL_INT( 8192, utf8stringbuf_get_size( structArrTest[0].url ) );
    TEST_ASSERT( utf8stringbuf_equals_str( structArrTest[1].url, "http://second" ));
    TEST_ASSERT( utf8stringbuf_get_string( structArrTest[2].url ) == structArrTest[2].urlArr );
    TEST_ASSERT_EQUAL_INT( 8192, utf8stringbuf_get_size( structArrTest[0].url ) );
    TEST_ASSERT( utf8stringbuf_equals_str( structArrTest[2].url, "ftp://third" ));
    TEST_ASSERT( utf8stringbuf_get_string( structArrTest[3].url ) == structArrTest[3].urlArr );
    TEST_ASSERT_EQUAL_INT( 8192, utf8stringbuf_get_size( structArrTest[0].url ) );
    TEST_ASSERT( utf8stringbuf_equals_str( structArrTest[3].url, "file://last" ));

    /* check static array initialization */
    strcpy( ThousandPathNames[99], "one-hundred" );
    TEST_ASSERT( utf8stringbuf_get_string( ThousandPaths[99] ) == ThousandPathNames[99] );
    TEST_ASSERT_EQUAL_INT( 256, utf8stringbuf_get_size( ThousandPaths[99] ) );
    TEST_ASSERT( utf8stringbuf_equals_str( ThousandPaths[99], "one-hundred" ));
    TEST_ASSERT( utf8stringbuf_get_string( ThousandPaths[100] ) == ThousandPathNames[100] );
    TEST_ASSERT_EQUAL_INT( 256, utf8stringbuf_get_size( ThousandPaths[100] ) );
    TEST_ASSERT( utf8stringbuf_equals_str( ThousandPaths[100], "" ));

    /* check runtime init function */
    utf8stringbuf_t dynTestBuf3 = utf8stringbuf_init( 10, dynTestArr2 );
    TEST_ASSERT( utf8stringbuf_get_string( dynTestBuf3 ) == dynTestArr2 );
    TEST_ASSERT_EQUAL_INT( 10, utf8stringbuf_get_size( dynTestBuf3 ) );
}

static void testClear(void)
{
    /* clear regular string */
    char dynTestArr[] = "Hello";
    utf8stringbuf_t dynTestBuf = UTF8STRINGBUF( dynTestArr );
    utf8stringbuf_clear ( dynTestBuf );
    TEST_ASSERT( 0 == memcmp( utf8stringbuf_get_string( dynTestBuf ), "\0\0\0\0\0", 6 ) );
    TEST_ASSERT_EQUAL_INT( 6, utf8stringbuf_get_size( dynTestBuf ) );

    /* clear NULL */
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf( NULL );
    utf8stringbuf_clear ( dynTestBuf2 );
    TEST_ASSERT( 0 == memcmp( utf8stringbuf_get_string( dynTestBuf2 ), "", 1 ) );
    TEST_ASSERT_EQUAL_INT( 1, utf8stringbuf_get_size( dynTestBuf2 ) );
}

static void testEquals(void)
{
    //  prepare
    int equal;
    char dynTestArr1[] = "Hello";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello World";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);

    //  test utf8stringbuf_equals_buf
    equal = utf8stringbuf_equals_buf( dynTestBuf1, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_buf( dynTestBuf1, dynTestBuf2 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_buf( dynTestBuf2, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_buf( dynTestBuf2, oneByteBuf );
    TEST_ASSERT_EQUAL_INT( 0, equal );

    //  test utf8stringbuf_equals_str
    equal = utf8stringbuf_equals_str( dynTestBuf1, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_str( dynTestBuf2, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_str( oneByteBuf, "Hi" );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_str( oneByteBuf, NULL );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_str( dynTestBuf1, NULL );
    TEST_ASSERT_EQUAL_INT( 0, equal );
}

static void testEqualsRegion(void)
{
    //  prepare
    int equal;
    char dynTestArr1[] = "Hello";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello Hell Hello Hello";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);

    //  test utf8stringbuf_equals_region_buf
    equal = utf8stringbuf_equals_region_buf( dynTestBuf1, 0, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf1, -1, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf1, 1, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf1, 6, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf1, 0, dynTestBuf2 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf2, 0, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf2, 6, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf2, 11, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf2, 17, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_buf( dynTestBuf2, 3, oneByteBuf );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    //  test utf8stringbuf_equals_region_str
    equal = utf8stringbuf_equals_region_str( dynTestBuf1, 0, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_str( dynTestBuf1, 0, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_str( dynTestBuf1, 5, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_str( dynTestBuf1, 0, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_str( dynTestBuf2, 6, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_str( dynTestBuf2, 17, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8stringbuf_equals_region_str( oneByteBuf, 0, "Hi" );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_str( oneByteBuf, 0, NULL );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8stringbuf_equals_region_str( dynTestBuf1, -1, NULL );
    TEST_ASSERT_EQUAL_INT( 0, equal );
}

static void testLength(void)
{
    int len;

    /* check utf8stringbuf_get_length */
    len = utf8stringbuf_get_length( oneByteBuf );
    TEST_ASSERT_EQUAL_INT( 0, len );
    len = utf8stringbuf_get_length( fourByteBuf );
    TEST_ASSERT_EQUAL_INT( 2, len );
    len = utf8stringbuf_get_length( megaByteBuf );
    TEST_ASSERT_EQUAL_INT( UTF8STRINGBUFTEST_MEGASIZE-1, len );
}

static void testStartsWith(void) {
    //  prepare
    int matches;
    char dynTestArr1[] = "Hello";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello World";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);

    //  test utf8stringbuf_starts_with_str
    matches = utf8stringbuf_starts_with_str( dynTestBuf1, dynTestArr2 );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_starts_with_str( dynTestBuf2, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_str( dynTestBuf1, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_str( dynTestBuf1, NULL );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_starts_with_str( utf8stringbuf(""), "" );
    TEST_ASSERT_EQUAL_INT( 1, matches );

    //  test utf8stringbuf_starts_with_buf
    matches = utf8stringbuf_starts_with_buf( dynTestBuf1, dynTestBuf2 );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_starts_with_buf( dynTestBuf2, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_buf( dynTestBuf1, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_buf( dynTestBuf1, utf8stringbuf("") );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_starts_with_buf( utf8stringbuf(""), utf8stringbuf("") );
    TEST_ASSERT_EQUAL_INT( 1, matches );

}

static void testEndsWith(void) {
    //  prepare
    int matches;
    char dynTestArr1[] = "World";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello World";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);

    //  test utf8stringbuf_ends_with_str
    matches = utf8stringbuf_ends_with_str( dynTestBuf1, dynTestArr2 );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_ends_with_str( dynTestBuf2, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_str( dynTestBuf1, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_str( dynTestBuf1, NULL );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_ends_with_str( utf8stringbuf(""), "" );
    TEST_ASSERT_EQUAL_INT( 1, matches );

    //  test utf8stringbuf_ends_with_buf
    matches = utf8stringbuf_ends_with_buf( dynTestBuf1, dynTestBuf2 );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8stringbuf_ends_with_buf( dynTestBuf2, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_buf( dynTestBuf1, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_buf( dynTestBuf1, utf8stringbuf("") );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8stringbuf_ends_with_buf( utf8stringbuf(""), utf8stringbuf("") );
    TEST_ASSERT_EQUAL_INT( 1, matches );

}

static void testCopyBuf(void)
{
    int error;
    int equal;
    char dynTestArr1[] = "World";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello World";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);

    /* check utf8stringbuf_copy_buf */
    error = utf8stringbuf_copy_buf( dynTestBuf1, dynTestBuf2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_buf( oneByteBuf, dynTestBuf2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( oneByteBuf, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_buf( dynTestBuf2, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_buf( dynTestBuf2, oneByteBuf );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testCopyStr(void)
{
    int error;
    int equal;
    char dynTestArr1[] = "World";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);

    /* check utf8stringbuf_copy_str */
    error = utf8stringbuf_copy_str( dynTestBuf1, "Hello World" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( oneByteBuf, "Hi" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( oneByteBuf, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf1, NULL );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf1, "Hi" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hi" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf1, "World" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "World" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

}

static void testCopyWithCutUtf8(void)
{
    int error;
    int equal;
    char dynTestArr1[3] = "Wo";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[8] = "Hello W";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);

    /* check utf8stringbuf_copy_str, \xE2\x82\xAC is the euro symbol in utf8 */
    error = utf8stringbuf_copy_str( dynTestBuf1, "\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "\xE2\x82\xAC\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "__\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "__\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "_\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "_\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "_\xE2\x82\xAC\xE2\x82\xAC_" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "_\xE2\x82\xAC\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_copy_str, \xC2\xA2 and \xF0\xA4\xAD\xA2 are other characters in utf8 */
    error = utf8stringbuf_copy_str( dynTestBuf1, "\xC2\xA2_" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "\xC2\xA2" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf1, "\xF0\xA4\xAD\xA2" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_copy_str, \xC2\xA2 but not \xFC\xA4\xAD\xA2\xAD\xA2 are other characters in utf8 */
    error = utf8stringbuf_copy_str( dynTestBuf2, "\xC2\xA2\xFC\xA4\xAD\xA2\xAD\xA2" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "\xC2\xA2\xFC\xA4\xAD\xA2\xAD" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "\xC2\xA2_\xFC\xA4\xAD\xA2\xAD\xA2" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "\xC2\xA2_\xFC\xA4\xAD\xA2" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_str( dynTestBuf2, "\xC2\xA2_\xC2\xA2_\xC2\xA2" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "\xC2\xA2_\xC2\xA2_" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

}

static void testFindFirst(void)
{
    int pos;
    char srchArr1[] = "aaaaaaaaaaaaaaaaaaaaaaa";
    utf8stringbuf_t srchBuf1 = utf8stringbuf(srchArr1);
    char srchArr2[100] = "aaaaaaaaaaaaaaaaaaaaZaa";
    utf8stringbuf_t srchBuf2 = utf8stringbuf(srchArr2);
    char srchArr3[] = "N/A";
    utf8stringbuf_t srchBuf3 = UTF8STRINGBUF(srchArr3);

    pos = utf8stringbuf_find_first_str( megaByteBuf, srchArr1);
    TEST_ASSERT_EQUAL_INT( 0, pos );

    pos = utf8stringbuf_find_first_str( megaByteBuf, srchArr2);
    TEST_ASSERT_EQUAL_INT( 980, pos );

    pos = utf8stringbuf_find_first_str( megaByteBuf, "");
    TEST_ASSERT_EQUAL_INT( 0, pos );

    pos = utf8stringbuf_find_first_str( megaByteBuf, NULL);
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_first_str( megaByteBuf, srchArr3);
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_first_str( oneByteBuf, srchArr3);
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_first_str( srchBuf3, srchArr3);
    TEST_ASSERT_EQUAL_INT( 0, pos );


    pos = utf8stringbuf_find_first_buf( megaByteBuf, srchBuf1);
    TEST_ASSERT_EQUAL_INT( 0, pos );

    pos = utf8stringbuf_find_first_buf( megaByteBuf, srchBuf2);
    TEST_ASSERT_EQUAL_INT( 980, pos );

    pos = utf8stringbuf_find_first_buf( megaByteBuf, oneByteBuf);
    TEST_ASSERT_EQUAL_INT( 0, pos );

    pos = utf8stringbuf_find_first_buf( megaByteBuf, srchBuf3);
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_first_buf( oneByteBuf, srchBuf3);
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_first_buf( srchBuf3, srchBuf3);
    TEST_ASSERT_EQUAL_INT( 0, pos );
}

static void testFindNext(void)
{
    int pos;
    char srchArr1[10] = "aaaa";
    utf8stringbuf_t srchBuf1 = UTF8STRINGBUF(srchArr1);
    char srchArr2[] = "aaaZaaa";
    utf8stringbuf_t srchBuf2 = utf8stringbuf(srchArr2);
    char srchArr3[] = "N/A";
    utf8stringbuf_t srchBuf3 = UTF8STRINGBUF(srchArr3);

    pos = utf8stringbuf_find_next_buf( oneByteBuf, srchBuf3, -17 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_next_buf( oneByteBuf, srchBuf3, 17 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_next_buf( oneByteBuf, srchBuf3, 0 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_next_buf( srchBuf1, megaByteBuf, 1 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_next_buf( megaByteBuf, srchBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 0, pos );

    pos = utf8stringbuf_find_next_buf( megaByteBuf, srchBuf1, 1 );
    TEST_ASSERT_EQUAL_INT( 1, pos );

    pos = utf8stringbuf_find_next_buf( megaByteBuf, srchBuf2, 1 );
    TEST_ASSERT_EQUAL_INT( 997, pos );

    pos = utf8stringbuf_find_next_buf( megaByteBuf, srchBuf2, 1001 );
    TEST_ASSERT_EQUAL_INT( 1001, pos );

    pos = utf8stringbuf_find_next_buf( megaByteBuf, srchBuf2, 1002 );
    TEST_ASSERT_EQUAL_INT( 899997, pos );

    pos = utf8stringbuf_find_next_buf( megaByteBuf, srchBuf2, 899998 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_next_buf( megaByteBuf, srchBuf3, 1000 );
    TEST_ASSERT_EQUAL_INT( -1, pos );


    pos = utf8stringbuf_find_next_str( megaByteBuf, NULL, 1000 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_next_str( megaByteBuf, srchArr1, 1000 );
    TEST_ASSERT_EQUAL_INT( 1005, pos );

}

static void testFindLast(void)
{
    int pos;
    char srchArr1[10] = "aaaa";
    utf8stringbuf_t srchBuf1 = UTF8STRINGBUF(srchArr1);
    char srchArr2[] = "aaaZaaa";
    utf8stringbuf_t srchBuf2 = utf8stringbuf(srchArr2);
    char srchArr3[] = "N/A";
    utf8stringbuf_t srchBuf3 = UTF8STRINGBUF(srchArr3);

    pos = utf8stringbuf_find_last_buf( oneByteBuf, srchBuf3 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_last_buf( srchBuf1, megaByteBuf );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_last_buf( megaByteBuf, srchBuf1 );
    TEST_ASSERT_EQUAL_INT( UTF8STRINGBUFTEST_MEGASIZE - 1 - 4, pos );

    pos = utf8stringbuf_find_last_buf( megaByteBuf, srchBuf2 );
    TEST_ASSERT_EQUAL_INT( 899997, pos );

    pos = utf8stringbuf_find_last_buf( megaByteBuf, srchBuf3 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_last_buf( srchBuf2, srchBuf2 );
    TEST_ASSERT_EQUAL_INT( 0, pos );

    pos = utf8stringbuf_find_last_str( megaByteBuf, NULL );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringbuf_find_last_str( megaByteBuf, "" );
    TEST_ASSERT_EQUAL_INT( UTF8STRINGBUFTEST_MEGASIZE - 1, pos );

    pos = utf8stringbuf_find_last_str( srchBuf2, "aaa" );
    TEST_ASSERT_EQUAL_INT( 4, pos );
}

static void testCopyRegion(void)
{
    int error;
    int equal;
    char dynTestArr1[3] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
    char dynTestArr2[8] = "Hello W";
    utf8stringbuf_t dynTestBuf2 = UTF8STRINGBUF(dynTestArr2);

    /* check utf8stringbuf_copy_region_from_buf: standard cases */
    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, 0, 2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, 5, 2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, " W" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_copy_region_from_buf: illegal ranges */
    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, -1, 2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, 0, -2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, 0, 0 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_copy_region_from_buf: out of bounds */
    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, 6, 2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "W" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, 8, 2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_buf( dynTestBuf1, dynTestBuf2, 1, 4 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "el" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_copy_region_from_str */
    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 0, 4 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 0, 2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 3, 2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "lo" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 4, 2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "o" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 5, 2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 7, 2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", -1, 2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 3, 0 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "Hello", 3, -1 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "\xC2\xA2_\xC2\xA2", 2, 3 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "_" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "\xC2\xA2_\xC2\xA2", 0, 3 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "\xC2\xA2" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, "\xC2\xA2_\xC2\xA2", 3, 2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "\xC2\xA2" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_copy_region_from_str( dynTestBuf1, NULL, 0, 0 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testReplaceRegion(void)
{
    int error;
    int equal;
    char dynTestArr1[15] = "";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(13,&(dynTestArr1[1]));

    /* check that markers at start and end are not overwritten */
    dynTestArr1[0]=127;
    dynTestArr1[14]=127;

    /* check utf8stringbuf_replace_region_by_str: standard cases */
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 0, 0, "all inserted" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "all inserted" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 0, 12, "all replaced" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "all replaced" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 1, 5, " 5 " );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "a 5 placed" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 1, "333" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    /*printf("%s",dynTestArr1);*/
    equal = utf8stringbuf_equals_str( dynTestBuf1, "a 333 placed" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 1, 10, "" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "ad" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 0, "." );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "ad." );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 0, 3, NULL );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* check that markers at start and end are not overwritten */
    TEST_ASSERT_EQUAL_INT( 127, dynTestArr1[0] );
    TEST_ASSERT_EQUAL_INT( 127, dynTestArr1[14] );
}

static void testReplaceRegionExceededRanges(void) {
    int error;
    int equal;
    char dynTestArr1[15] = "";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(13,&(dynTestArr1[1]));

    /* check that markers at start and end are not overwritten */
    dynTestArr1[0]=127;
    dynTestArr1[14]=127;

    /* check utf8stringbuf_replace_region_by_str: range exceeded cases */
    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, -1, 2, "replaced" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 0, 12, "replaced" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "replaced" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 12, "." );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He." );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 0, 14, "replaced" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "replaced" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    /*TEST_ASSERT_EQUAL_INT( 127, dynTestArr1[14] );*/

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 6, 0, "replaced" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    /*TEST_ASSERT_EQUAL_INT( 127, dynTestArr1[14] );*/

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 5, 0, "12345678" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello1234567" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 0, "12345678" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He12345678ll" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 0, "1234567890" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He1234567890" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 0, "1234567890A" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He1234567890" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* check that markers at start and end are not overwritten */
    TEST_ASSERT_EQUAL_INT( 127, dynTestArr1[0] );
    TEST_ASSERT_EQUAL_INT( 127, dynTestArr1[14] );
}

static void testReplaceRegionWithCutUtf8(void) {
    int error;
    int equal;
    char dynTestArr1[15] = "";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(13,&(dynTestArr1[1]));

    /* check that markers at start and end are not overwritten */
    dynTestArr1[0]=127;
    dynTestArr1[14]=127;

    /* check utf8stringbuf_copy_str, \xE2\x82\xAC is the euro symbol in utf8 */
    utf8stringbuf_copy_str( dynTestBuf1, "Hello \xE2\x82\xAC" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 0, "1234" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He1234llo " );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 2, 0, "12345678\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He12345678" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello " );
    error = utf8stringbuf_replace_region_by_str( dynTestBuf1, 6, 0, "1234\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello 1234" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* check that markers at start and end are not overwritten */
    TEST_ASSERT_EQUAL_INT( 127, dynTestArr1[0] );
    TEST_ASSERT_EQUAL_INT( 127, dynTestArr1[14] );
}

static void testReplaceRegionBuf(void) {
    /* utf8stringbuf_replace_region_by_buf is just a wrapper around utf8stringbuf_replace_region_by_str */
    /* therefore, we do only one test */
    int error;
    int equal;
    char dynTestArr1[7] = "Helllo";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
    char dynTestArr2[] = "ll";
    utf8stringbuf_t dynTestBuf2 = UTF8STRINGBUF(dynTestArr2);

    /* check utf8stringbuf_replace_region_by_buf */
    error = utf8stringbuf_replace_region_by_buf( dynTestBuf1, 2, 3, dynTestBuf2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testReplaceAll(void) {
    int error;
    int equal;
    char dynTestArr1[20] = "He&l<'";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_replace_all */
    error = utf8stringbuf_replace_all( dynTestBuf1, &XML_ENCODE );
    /*printf( "%s", utf8stringbuf_get_string(dynTestBuf1) );*/
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He&amp;l&lt;&apos;" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_all( dynTestBuf1, &SQL_ENCODE );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He&amp;l&lt;&apos;" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_all( dynTestBuf1, &XML_ENCODE );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He&amp;amp;l&amp;lt" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* change nearly everything */
    error = utf8stringbuf_replace_all( megaByteBuf, &TO_UPPER_CASE );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_starts_with_str( megaByteBuf, "AAAAAAAAAA" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* change everything */
    error = utf8stringbuf_replace_all( megaByteBuf, &TO_LOWER_CASE );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_starts_with_str( megaByteBuf, "aaaaaaaaaaaaaaaaaaaaaaaaaaa" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* change nothing */
    error = utf8stringbuf_replace_all( megaByteBuf, &TO_LOWER_CASE );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_starts_with_str( megaByteBuf, "aaaaaaaaaaaaaaaaaaaaaaaaaaa" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* shrink because performance hungry function */
    utf8stringbuf_delete_to_end( megaByteBuf, 20000 );

    /* shrink at each replacement */
    error = utf8stringbuf_replace_all( megaByteBuf, &SHRINK_DUPLICATES_EXCEPT_Z );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_starts_with_str( megaByteBuf, "AAAAAAAAAA" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* grow at each replacement */
    error = utf8stringbuf_replace_all( megaByteBuf, &EXPAND_SINGLES_EXCEPT_Z );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_starts_with_str( megaByteBuf, "aaaaaaaaaaaaaaaaaaaaaaaaaaa" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

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

static void testReplaceAllBadCases(void) {
    int error;
    int equal;
    char dynTestArr1[32] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
    char dynTestArr2[8] = "";
    utf8stringbuf_t dynTestBuf2 = UTF8STRINGBUF(dynTestArr2);

    /* check utf8stringbuf_replace_all */
    utf8stringbuf_copy_str( dynTestBuf1, "Hello World" );
    error = utf8stringbuf_replace_all( dynTestBuf1, &TEST_PATTERNS );
    /*printf( "%s", utf8stringbuf_get_string(dynTestBuf1) );*/
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello World Hello World" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* replace everything, including shrinking */
    utf8stringbuf_copy_str( dynTestBuf2, "\xE2\x82\xAC" "Z" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "EUREUR" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* test null pattern*/
    utf8stringbuf_copy_str( dynTestBuf2, "\xE2\x82\xAC" "Z" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, NULL );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "\xE2\x82\xAC" "Z" "\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* try to cut trailing string code point */
    utf8stringbuf_copy_str( dynTestBuf2, "\xE2\x82\xAC" "Y" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "EURyyy" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* first shrink, then grow, fit exactly*/
    utf8stringbuf_copy_str( dynTestBuf2, "ZZaY" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "ayyyEUR" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* first shrink, then grow, cut after second byte of multibyte code point*/
    utf8stringbuf_copy_str( dynTestBuf2, "ZaaY" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "aayyy" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* first grow, then shrink, cut after first byte of multibyte code point*/
    utf8stringbuf_copy_str( dynTestBuf2, "YZZa" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "yyya" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* first grow, then shrink, cut after second byte of multibyte code point*/
    utf8stringbuf_copy_str( dynTestBuf2, "YZa" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "yyya" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* try to cut replacement string*/
    utf8stringbuf_copy_str( dynTestBuf2, "aZWorld" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "aHello " );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* try to cut replacement code point */
    utf8stringbuf_copy_str( dynTestBuf2, "Y5EHell" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "yyy5 " );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* try to cut trailing string code point */
    utf8stringbuf_copy_str( dynTestBuf2, "a5E" "\xE2\x82\xAC" );
    error = utf8stringbuf_replace_all( dynTestBuf2, &TEST_PATTERNS );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf2, "a5 " "\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

}

static void testReplaceAllStr(void) {
    /* utf8stringbuf_replace_all_str_by_str is just a wrapper around utf8stringbuf_replace_all */
    /* therefore, we do only two tests */
    int error;
    int equal;
    char dynTestArr1[7] = "Helllo";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_replace_all_str_by_str */
    error = utf8stringbuf_replace_all_str_by_str( dynTestBuf1, NULL, "" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Helllo" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    error = utf8stringbuf_replace_all_str_by_str( dynTestBuf1, "lo", "o!" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello!" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testReplaceAllBuf(void) {
    /* utf8stringbuf_replace_all_buf_by_buf is just a wrapper around utf8stringbuf_replace_all */
    /* therefore, we do only one test */
    int error;
    int equal;
    char dynTestArr1[7] = "Helllo";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_replace_all_buf_by_buf */
    error = utf8stringbuf_replace_all_buf_by_buf( dynTestBuf1, utf8stringbuf("ll"), utf8stringbuf("l") );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testInsertBuf(void) {
    /* utf8stringbuf_insert_buf is just a wrapper around utf8stringbuf_replace_region_by_str */
    /* therefore, we do only one test */
    int error;
    int equal;
    char dynTestArr1[6] = "Heo";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
    char dynTestArr2[] = "ll";
    utf8stringbuf_t dynTestBuf2 = UTF8STRINGBUF(dynTestArr2);

    /* check utf8stringbuf_insert_buf */
    error = utf8stringbuf_insert_buf( dynTestBuf1, 2, dynTestBuf2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testInsertStr(void) {
    /* utf8stringbuf_insert_str is just a wrapper around utf8stringbuf_replace_region_by_str */
    /* therefore, we do only one test */
    int error;
    int equal;
    char dynTestArr1[6] = "He";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_insert_str */
    error = utf8stringbuf_insert_str( dynTestBuf1, 2, "llo" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testDelete(void) {
    /* utf8stringbuf_delete is just a wrapper around utf8stringbuf_replace_region_by_str */
    /* therefore, we do only one test */
    int error;
    int equal;
    char dynTestArr1[12] = "Hellllllllo";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_delete */
    error = utf8stringbuf_delete( dynTestBuf1, 4, 6 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testDeleteFromEnd(void) {
    /* utf8stringbuf_delete is just a wrapper around utf8stringbuf_replace_region_by_str */
    /* therefore, we do only one test */
    int error;
    int equal;
    char dynTestArr1[12] = "Hellllllllo";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_delete_from_end */
    error = utf8stringbuf_delete_from_end( dynTestBuf1, 7 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hell" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testDeleteToEnd(void) {
    /* utf8stringbuf_delete is just a wrapper around utf8stringbuf_replace_region_by_str */
    /* therefore, we do only one test */
    int error;
    int equal;
    char dynTestArr1[12] = "Hellllllllo";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_delete_to_end */
    error = utf8stringbuf_delete_to_end( dynTestBuf1, 4 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hell" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testAppendStr(void) {
    int error;
    int equal;
    char dynTestArr1[6] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_append_str */
    utf8stringbuf_copy_str( dynTestBuf1, "Hell" );
    error = utf8stringbuf_append_str( dynTestBuf1, "o" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "" );
    error = utf8stringbuf_append_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_str( dynTestBuf1, NULL );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hell" );
    error = utf8stringbuf_append_str( dynTestBuf1, "\xC2\xA2" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hell" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hell" );
    error = utf8stringbuf_append_str( dynTestBuf1, "o!" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hel" );
    error = utf8stringbuf_append_str( dynTestBuf1, "\xC2\xA2!" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hel\xC2\xA2" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testAppendBuf(void) {
    int error;
    int equal;
    char dynTestArr1[6] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
    char dynTestArr2[] = "l\xC2\xA2";
    utf8stringbuf_t dynTestBuf2 = UTF8STRINGBUF(dynTestArr2);

    /* check utf8stringbuf_append_buf */
    utf8stringbuf_copy_str( dynTestBuf1, "He" );
    error = utf8stringbuf_append_buf( dynTestBuf1, dynTestBuf2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hel\xC2\xA2" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hel" );
    error = utf8stringbuf_append_buf( dynTestBuf1, dynTestBuf2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hell" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_buf( dynTestBuf1, dynTestBuf2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testAppendInt(void) {
    int error;
    int equal;
    char dynTestArr1[6] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_append_int */
    utf8stringbuf_copy_str( dynTestBuf1, "12" );
    error = utf8stringbuf_append_int( dynTestBuf1, 345 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "12345" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "" );
    error = utf8stringbuf_append_int( dynTestBuf1, -1234 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "-1234" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "1234" );
    error = utf8stringbuf_append_int( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "12340" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "12345" );
    error = utf8stringbuf_append_int( dynTestBuf1, 6 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "12345" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "1" );
    error = utf8stringbuf_append_int( dynTestBuf1, 234567 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "12345" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "1234" );
    error = utf8stringbuf_append_int( dynTestBuf1, -1 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "1234-" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testAppendHex(void) {
    int error;
    int equal;
    char dynTestArr1[6] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_append_hex */
    utf8stringbuf_copy_str( dynTestBuf1, "0x" );
    error = utf8stringbuf_append_hex( dynTestBuf1, -1 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "0xfff" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "0x" );
    error = utf8stringbuf_append_hex( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "0x0" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "0x" );
    error = utf8stringbuf_append_hex( dynTestBuf1, 0x1000555 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "0x100" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testAppendChar(void) {
    int error;
    int equal;
    char dynTestArr1[6] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_append_char */
    utf8stringbuf_copy_str( dynTestBuf1, "Hell" );
    error = utf8stringbuf_append_char( dynTestBuf1, 'o' );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* append one byte too much */
    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_char( dynTestBuf1, 'o' );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_char */
    utf8stringbuf_copy_str( dynTestBuf1, "He" );
    error = utf8stringbuf_append_char( dynTestBuf1, 0x20ac );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* append one byte too much */
    utf8stringbuf_copy_str( dynTestBuf1, "Hel" );
    error = utf8stringbuf_append_char( dynTestBuf1, 0x20ac );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hel" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* append illegal code point */
    utf8stringbuf_clear( dynTestBuf1 );
    error = utf8stringbuf_append_char( dynTestBuf1, 0x80000000 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_NOT_A_CODEPOINT, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testAppendWStr(void) {
    int error;
    int equal;
    char dynTestArr1[6] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_append_wstr */
    wchar_t *wideString = L"lo";
    utf8stringbuf_copy_str( dynTestBuf1, "Hel" );
    error = utf8stringbuf_append_wstr( dynTestBuf1, wideString );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_wstr with multibyte */
    const wchar_t *wideString2 = L"e\x20ac";
    utf8stringbuf_copy_str( dynTestBuf1, "H" );
    error = utf8stringbuf_append_wstr( dynTestBuf1, wideString2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He\xE2\x82\xAC" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_wstr with truncation */
    utf8stringbuf_copy_str( dynTestBuf1, "He" );
    error = utf8stringbuf_append_wstr( dynTestBuf1, wideString2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hee" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    utf8stringbuf_copy_str( dynTestBuf1, "Hello" );
    error = utf8stringbuf_append_wstr( dynTestBuf1, wideString2 );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    /* check utf8stringbuf_append_wstr with NULL */
    utf8stringbuf_copy_str( dynTestBuf1, "He" );
    error = utf8stringbuf_append_wstr( dynTestBuf1, NULL );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_NULL_PARAM, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "He" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}

static void testCharAt(void) {
    utf8codepoint_t result;
    char dynTestArr1[6] = "He\xE2\x82\xAC";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check utf8stringbuf_get_char_at */
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 'H', utf8codepoint_get_char(result) );

    result = utf8stringbuf_get_char_at( dynTestBuf1, 2 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 3, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x20ac, utf8codepoint_get_char(result) );

    result = utf8stringbuf_get_char_at( dynTestBuf1, 3 );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x0, utf8codepoint_get_char(result) );

    result = utf8stringbuf_get_char_at( dynTestBuf1, 5 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x0, utf8codepoint_get_char(result) );

    /* check example from documentation */
    char testArr[] = "\xC2\xAE 2005 A.A. \xC2\xAE 2006-2012 B.B.";
    utf8stringbuf_t testBuf = UTF8STRINGBUF(testArr);
    result = utf8stringbuf_get_char_at( testBuf, 13 );
    TEST_ASSERT_EQUAL_INT( 0xae, utf8codepoint_get_char(result) );

    /* check undefined behaviour */
    result = utf8stringbuf_get_char_at( dynTestBuf1, 6 );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x0, utf8codepoint_get_char(result) );
}

static void testCharAtLoops(void) {
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
    TEST_ASSERT_EQUAL_INT( 3, countCodePoints );

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
    TEST_ASSERT_EQUAL_INT( 3, countCodePoints );

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
    TEST_ASSERT_EQUAL_INT( 3, countCodePoints );

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
    TEST_ASSERT_EQUAL_INT( 3, countCodePoints );
}

#ifdef __cplusplus

static void testCrossLanguage(void) {
    int success;
    int equal;
    char dynTestArr1[] = "World, Hello!";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
    static const char dynTestString2[] = "Hello World!";
    utf8stringbuf_t dynTestBuf2;

#ifdef __cplusplus
    TEST_FAIL( "testCrossLanguage was run in c++ linkage environment." );
#endif

    dynTestBuf2 = utf8stringbuf_cross_language_create_stringbuf( dynTestString2 );
    equal = utf8stringbuf_equals_str( dynTestBuf2, dynTestString2 );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    success = utf8stringbuf_cross_language_check_stringbuf( dynTestBuf1, "World, Hello!" );
    TEST_ASSERT_EQUAL_INT( 1, success );
}

#endif

static void testSplitIn2(void) {
    utf8string2tuple_t tuple;
    int equals;

    /* test good case */
    {
        char dynTestArr10_withBorders[9] = " H\xE2\x82\xAC\0z ";
        char *dynTestArr10 = &(dynTestArr10_withBorders[1]);
        utf8stringbuf_t dynTestBuf10 = utf8stringbuf_init(6,dynTestArr10);

        tuple = utf8stringbuf_split_in_2( dynTestBuf10, 1 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, tuple.error );
        equals = memcmp( "H\0\xE2\x82\xAC", dynTestArr10, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr10[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr10[2]) == tuple.second );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr10_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr10_withBorders[7] );
    }

    /* test good case, empty first string */
    {
        char dynTestArr11_withBorders[9] = " H\xE2\x82\xAC\0z ";
        char *dynTestArr11 = &(dynTestArr11_withBorders[1]);
        utf8stringbuf_t dynTestBuf11 = utf8stringbuf_init(6,dynTestArr11);

        tuple = utf8stringbuf_split_in_2( dynTestBuf11, 0 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, tuple.error );
        equals = memcmp( "\0H\xE2\x82\xAC", dynTestArr11, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr11[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr11[1]) == tuple.second );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr11_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr11_withBorders[7] );
    }

    /* test good case, empty second string */
    {
        char dynTestArr12_withBorders[9] = " H\xE2\x82\xAC\0z ";
        char *dynTestArr12 = &(dynTestArr12_withBorders[1]);
        utf8stringbuf_t dynTestBuf12 = utf8stringbuf_init(6,dynTestArr12);

        tuple = utf8stringbuf_split_in_2( dynTestBuf12, 4 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, tuple.error );
        equals = memcmp( "H\xE2\x82\xAC\0", dynTestArr12, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr12[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr12[5]) == tuple.second );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr12_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr12_withBorders[7] );
    }

    /* test good case, both strings empty */
    {
        char dynTestArr13_withBorders[9] = " \0trash ";
        char *dynTestArr13 = &(dynTestArr13_withBorders[1]);
        utf8stringbuf_t dynTestBuf13 = utf8stringbuf_init(6,dynTestArr13);

        tuple = utf8stringbuf_split_in_2( dynTestBuf13, 0 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, tuple.error );
        equals = memcmp( "\0\0\0\0\0", dynTestArr13, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr13[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr13[1]) == tuple.second );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr13_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr13_withBorders[7] );
    }

    /* test truncation of second string only */
    {
        char dynTestArr1_withBorders[9] = " He\xE2\x82\xAC\0 ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(6,dynTestArr1);

        tuple = utf8stringbuf_split_in_2( dynTestBuf1, 1 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, tuple.error );
        equals = memcmp( "H\0e\0\0", dynTestArr1, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[2]) == tuple.second );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[7] );
    }

    /* test truncation of both strings */
    {
        /* this is not possible, but this is a try: */
        char dynTestArr2_withBorders[9] = " He\xE2\x82\xAC\0 ";
        char *dynTestArr2 = &(dynTestArr2_withBorders[1]);
        utf8stringbuf_t dynTestBuf2 = utf8stringbuf_init(6,dynTestArr2);

        tuple = utf8stringbuf_split_in_2( dynTestBuf2, 5 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, tuple.error );
        equals = memcmp( "He\xE2\x82\xAC", dynTestArr2, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr2[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr2[5]) == tuple.second );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr2_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr2_withBorders[7] );
    }

    /* test for illegal parameters: -1 */
    {
        char dynTestArr3_withBorders[9] = " He\xE2\x82\xAC\0 ";
        char *dynTestArr3 = &(dynTestArr3_withBorders[1]);
        utf8stringbuf_t dynTestBuf3 = utf8stringbuf_init(6,dynTestArr3);

        tuple = utf8stringbuf_split_in_2( dynTestBuf3, -1 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, tuple.error );
        equals = memcmp( "He\xE2\x82\xAC", dynTestArr3, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr3[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr3[5]) == tuple.second );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr3_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr3_withBorders[7] );
    }

    /* test for illegal parameters: 5 */
    {
        char dynTestArr4_withBorders[9] = " H\xE2\x82\xAC\0z ";
        char *dynTestArr4 = &(dynTestArr4_withBorders[1]);
        utf8stringbuf_t dynTestBuf4 = utf8stringbuf_init(6,dynTestArr4);

        tuple = utf8stringbuf_split_in_2( dynTestBuf4, 5 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, tuple.error );
        equals = memcmp( "H\xE2\x82\xAC", dynTestArr4, 5 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr4[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr4[4]) == tuple.second );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr4_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr4_withBorders[7] );
    }

}

static void testSplitIn3(void) {
    utf8string3tuple_t tuple;
    int equals;

    /* test standard case */
    {
        char dynTestArr1_withBorders[9] = " He\0yzz ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(6,dynTestArr1);

        tuple = utf8stringbuf_split_in_3( dynTestBuf1, 0, 1 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, tuple.error );
        equals = memcmp( "\0H\0e\0", dynTestArr1, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[1]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[3]) == tuple.third );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[7] );
    }

    /* test full buffer case, no truncation */
    {
        char dynTestArr1_withBorders[9] = " \xE2\x82\xAC\0yz ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(6,dynTestArr1);

        tuple = utf8stringbuf_split_in_3( dynTestBuf1, 0, 0 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, tuple.error );
        equals = memcmp( "\0\0\xE2\x82\xAC", dynTestArr1, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[1]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[2]) == tuple.third );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[7] );
    }

    /* test full buffer case, no truncation */
    {
        char dynTestArr1_withBorders[9] = " \xE2\x82\xAC\0yz ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(6,dynTestArr1);

        tuple = utf8stringbuf_split_in_3( dynTestBuf1, 3, 3 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, tuple.error );
        equals = memcmp( "\xE2\x82\xAC\0\0", dynTestArr1, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[4]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[5]) == tuple.third );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[7] );
    }

    /* test min buffer case */
    {
        char dynTestArr3_withBorders[6] = " \0yz ";
        char *dynTestArr3 = &(dynTestArr3_withBorders[1]);
        utf8stringbuf_t dynTestBuf3 = utf8stringbuf_init(3,dynTestArr3);

        tuple = utf8stringbuf_split_in_3( dynTestBuf3, 0, 0 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, tuple.error );
        equals = memcmp( "\0\0", dynTestArr3, 3 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr3[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr3[1]) == tuple.second );
        TEST_ASSERT( &(dynTestArr3[2]) == tuple.third );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr3_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr3_withBorders[4] );
    }

    /* test small buffer case */
    {
        char dynTestArr4_withBorders[5] = " y\0 ";
        char *dynTestArr4 = &(dynTestArr4_withBorders[1]);
        utf8stringbuf_t dynTestBuf4 = utf8stringbuf_init(2,dynTestArr4);

        tuple = utf8stringbuf_split_in_3( dynTestBuf4, 0, 0 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, tuple.error );
        equals = memcmp( "\0", dynTestArr4, 2 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr4[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr4[1]) == tuple.second );
        TEST_ASSERT( &(dynTestArr4[1]) == tuple.third );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr4_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr4_withBorders[3] );
    }

    /* test full buffer case, truncate 3rd */
    {
        char dynTestArr1_withBorders[9] = " z\xE2\x82\xAC\0y ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(6,dynTestArr1);

        tuple = utf8stringbuf_split_in_3( dynTestBuf1, 4, 4 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, tuple.error );
        equals = memcmp( "z\xE2\x82\xAC\0", dynTestArr1, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[5]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[5]) == tuple.third );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[7] );
    }

    /* test full buffer case, truncate 2nd and 3rd */
    {
        char dynTestArr1_withBorders[9] = " yz\xE2\x82\xAC\0 ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(6,dynTestArr1);

        tuple = utf8stringbuf_split_in_3( dynTestBuf1, 5, 5 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, tuple.error );
        equals = memcmp( "yz\xE2\x82\xAC", dynTestArr1, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[5]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[5]) == tuple.third );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[7] );
    }

    /* test out of range case */
    {
        char dynTestArr1_withBorders[9] = " \xE2\x82\xAC\0yz ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(6,dynTestArr1);

        tuple = utf8stringbuf_split_in_3( dynTestBuf1, 3, 0 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, tuple.error );
        equals = memcmp( "\xE2\x82\xAC\0\0", dynTestArr1, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[3]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[3]) == tuple.third );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[7] );
    }

    /* test truncate third string only case */
    {
        char dynTestArr1_withBorders[9] = " He\xC2\xA2\0y ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(6,dynTestArr1);

        tuple = utf8stringbuf_split_in_3( dynTestBuf1, 1, 2 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, tuple.error );
        equals = memcmp( "H\0e\0\0", dynTestArr1, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[2]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[4]) == tuple.third );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[7] );
    }

    /* test truncate second and third strings case */
    {
        char dynTestArr1_withBorders[9] = " H\xE2\x82\xACy\0 ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(6,dynTestArr1);

        tuple = utf8stringbuf_split_in_3( dynTestBuf1, 4, 5 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, tuple.error );
        equals = memcmp( "H\xE2\x82\xAC\0", dynTestArr1, 6 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[5]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[5]) == tuple.third );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[7] );
    }

    /* test for illegal parameters: -1 */
    {
        char dynTestArr4_withBorders[9] = " H\xE2\x82\xAC\0z ";
        char *dynTestArr4 = &(dynTestArr4_withBorders[1]);
        utf8stringbuf_t dynTestBuf4 = utf8stringbuf_init(6,dynTestArr4);

        tuple = utf8stringbuf_split_in_3( dynTestBuf4, 1, -1 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, tuple.error );
        equals = memcmp( "H\xE2\x82\xAC", dynTestArr4, 5 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr4[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr4[4]) == tuple.second );
        TEST_ASSERT( &(dynTestArr4[4]) == tuple.third );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr4_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr4_withBorders[7] );
    }
}

static void testSplitIn4(void) {
    int equals;
    utf8string4tuple_t tuple;

    /* test standard case */
    {
        char dynTestArr1_withBorders[13] = " H\xE2\x82\xAC\0yyzzz ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(10,dynTestArr1);

        tuple = utf8stringbuf_split_in_4( dynTestBuf1, 0, 1, 4 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, tuple.error );
        equals = memcmp( "\0H\0\xE2\x82\xAC\0\0\0", dynTestArr1, 10 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[1]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[3]) == tuple.third );
        TEST_ASSERT( &(dynTestArr1[7]) == tuple.fourth );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[11] );
    }

    /* test buffer full case, no truncation */
    {
        char dynTestArr1_withBorders[13] = " H\xE2\x82\xACzz\0yzz ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(10,dynTestArr1);

        tuple = utf8stringbuf_split_in_4( dynTestBuf1, 0, 1, 4 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, tuple.error );
        equals = memcmp( "\0H\0\xE2\x82\xAC\0zz", dynTestArr1, 10 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[1]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[3]) == tuple.third );
        TEST_ASSERT( &(dynTestArr1[7]) == tuple.fourth );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[11] );
    }

    /* test truncation of 3rd and 4th */
    {
        char dynTestArr1_withBorders[13] = " H\xE2\x82\xACzzz\0yz ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(10,dynTestArr1);

        tuple = utf8stringbuf_split_in_4( dynTestBuf1, 1, 4, 7 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, tuple.error );
        equals = memcmp( "H\0\xE2\x82\xAC\0zz\0", dynTestArr1, 10 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[2]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[6]) == tuple.third );
        TEST_ASSERT( &(dynTestArr1[9]) == tuple.fourth );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[11] );
    }

    /* test truncation of 3rd */
    {
        char dynTestArr1_withBorders[13] = " Hqqqq\xE2\x82\xAC\0z ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(10,dynTestArr1);

        tuple = utf8stringbuf_split_in_4( dynTestBuf1, 4, 8, 8 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, tuple.error );
        equals = memcmp( "Hqqq\0q\0\0\0", dynTestArr1, 10 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[5]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[9]) == tuple.third );
        TEST_ASSERT( &(dynTestArr1[9]) == tuple.fourth );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[11] );
    }

    /* test for illegal parameters: 5 */
    {
        char dynTestArr4_withBorders[9] = " H\xE2\x82\xAC\0z ";
        char *dynTestArr4 = &(dynTestArr4_withBorders[1]);
        utf8stringbuf_t dynTestBuf4 = utf8stringbuf_init(6,dynTestArr4);

        tuple = utf8stringbuf_split_in_4( dynTestBuf4, 1, 4, 5 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, tuple.error );
        equals = memcmp( "H\xE2\x82\xAC", dynTestArr4, 5 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr4[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr4[4]) == tuple.second );
        TEST_ASSERT( &(dynTestArr4[4]) == tuple.third );
        TEST_ASSERT( &(dynTestArr4[4]) == tuple.fourth );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr4_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr4_withBorders[7] );
    }

}

static void testSplitIn5(void) {
    int equals;
    utf8string5tuple_t tuple;

    /* test standard case */
    {
        char dynTestArr1_withBorders[13] = " H\xE2\x82\xAC\0yyzzz ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(10,dynTestArr1);

        tuple = utf8stringbuf_split_in_5( dynTestBuf1, 0, 1, 4, 4 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, tuple.error );
        equals = memcmp( "\0H\0\xE2\x82\xAC\0\0\0", dynTestArr1, 10 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[1]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[3]) == tuple.third );
        TEST_ASSERT( &(dynTestArr1[7]) == tuple.fourth );
        TEST_ASSERT( &(dynTestArr1[8]) == tuple.fifth );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[11] );
    }

    /* test buffer full case, no truncation */
    {
        char dynTestArr1_withBorders[13] = " H\xE2\x82\xACq\0yyzz ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(10,dynTestArr1);

        tuple = utf8stringbuf_split_in_5( dynTestBuf1, 0, 1, 4, 4 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, tuple.error );
        equals = memcmp( "\0H\0\xE2\x82\xAC\0\0q", dynTestArr1, 10 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[1]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[3]) == tuple.third );
        TEST_ASSERT( &(dynTestArr1[7]) == tuple.fourth );
        TEST_ASSERT( &(dynTestArr1[8]) == tuple.fifth );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[11] );
    }

    /* test truncation of 4th, 4th has own terminating zero, and 5th */
    {
        char dynTestArr1_withBorders[13] = " H\xE2\x82\xACqq\0yyz ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(10,dynTestArr1);

        tuple = utf8stringbuf_split_in_5( dynTestBuf1, 1, 4, 5, 6 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, tuple.error );
        equals = memcmp( "H\0\xE2\x82\xAC\0q\0\0", dynTestArr1, 10 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[2]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[6]) == tuple.third );
        TEST_ASSERT( &(dynTestArr1[8]) == tuple.fourth );
        TEST_ASSERT( &(dynTestArr1[9]) == tuple.fifth );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[11] );
    }

    /* test truncation of 4th, 4th has no own terminating zero anymore, and 5th */
    {
        char dynTestArr1_withBorders[13] = " Hqq\xE2\x82\xAC\0yyz ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(10,dynTestArr1);

        tuple = utf8stringbuf_split_in_5( dynTestBuf1, 3, 6, 6, 6 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, tuple.error );
        equals = memcmp( "Hqq\0\xE2\x82\xAC\0\0", dynTestArr1, 10 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[4]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[8]) == tuple.third );
        TEST_ASSERT( &(dynTestArr1[9]) == tuple.fourth );
        TEST_ASSERT( &(dynTestArr1[9]) == tuple.fifth );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[11] );
    }

    /* test truncation of 3rd, 4th and 5th */
    {
        char dynTestArr1_withBorders[13] = " Hqqzzz\xE2\x82\xAC\0 ";
        char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
        utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(10,dynTestArr1);

        tuple = utf8stringbuf_split_in_5( dynTestBuf1, 5, 9, 9, 9 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, tuple.error );
        equals = memcmp( "Hqqzz\0z\0\0", dynTestArr1, 10 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr1[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr1[6]) == tuple.second );
        TEST_ASSERT( &(dynTestArr1[9]) == tuple.third );
        TEST_ASSERT( &(dynTestArr1[9]) == tuple.fourth );
        TEST_ASSERT( &(dynTestArr1[9]) == tuple.fifth );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[11] );
    }

    /* test for illegal parameters: wrong order */
    {
        char dynTestArr4_withBorders[9] = " H\xE2\x82\xAC\0z ";
        char *dynTestArr4 = &(dynTestArr4_withBorders[1]);
        utf8stringbuf_t dynTestBuf4 = utf8stringbuf_init(6,dynTestArr4);

        tuple = utf8stringbuf_split_in_5( dynTestBuf4, 1, 4, 1, 4 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, tuple.error );
        equals = memcmp( "H\xE2\x82\xAC", dynTestArr4, 5 );
        TEST_ASSERT_EQUAL_INT( 0, equals );
        TEST_ASSERT( &(dynTestArr4[0]) == tuple.first );
        TEST_ASSERT( &(dynTestArr4[4]) == tuple.second );
        TEST_ASSERT( &(dynTestArr4[4]) == tuple.third );
        TEST_ASSERT( &(dynTestArr4[4]) == tuple.fourth );
        TEST_ASSERT( &(dynTestArr4[4]) == tuple.fifth );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr4_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr4_withBorders[7] );
    }
}

static void testJoin(void) {
    int equals;

    char dynTestArr1_withBorders[12] = " ab\0cd\0efg ";
    char *dynTestArr1 = &(dynTestArr1_withBorders[1]);
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf_init(9,dynTestArr1);

    /* test one join, one string-end and some terminating nonsense */
    {
        utf8stringbuf_join( dynTestBuf1 );
        equals = memcmp( dynTestArr1, "abcd\0\0\0\0", 9 );
        TEST_ASSERT_EQUAL_INT( 0, equals );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[10] );
    }

    /* test calling the function again on the result */
    {
        utf8stringbuf_join( dynTestBuf1 );
        equals = memcmp( dynTestArr1, "abcd\0\0\0\0", 9 );
        TEST_ASSERT_EQUAL_INT( 0, equals );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr1_withBorders[10] );
    }

    /* test two string-ends right at start and some terminating nonsense */
    {
        char dynTestArr2_withBorders[12] = " \0\0a\0bcdef ";
        char *dynTestArr2 = &(dynTestArr2_withBorders[1]);
        utf8stringbuf_t dynTestBuf2 = utf8stringbuf_init(9,dynTestArr2);

        utf8stringbuf_join( dynTestBuf2 );
        equals = memcmp( dynTestArr2, "a\0\0\0\0\0\0\0", 9 );
        TEST_ASSERT_EQUAL_INT( 0, equals );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr2_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr2_withBorders[10] );
    }

    /* test one single full string without trailing data */
    {
        char dynTestArr3_withBorders[12] = " abcdefgh\0 ";
        char *dynTestArr3 = &(dynTestArr3_withBorders[1]);
        utf8stringbuf_t dynTestBuf3 = utf8stringbuf_init(9,dynTestArr3);

        utf8stringbuf_join( dynTestBuf3 );
        equals = memcmp( dynTestArr3, "abcdefgh", 9 );
        TEST_ASSERT_EQUAL_INT( 0, equals );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr3_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr3_withBorders[10] );
    }

    /* test 5 substrings without trailing data */
    {
        char dynTestArr4_withBorders[12] = " \0a\0b\0c\0d\0 ";
        char *dynTestArr4 = &(dynTestArr4_withBorders[1]);
        utf8stringbuf_t dynTestBuf4 = utf8stringbuf_init(9,dynTestArr4);

        utf8stringbuf_join( dynTestBuf4 );
        equals = memcmp( dynTestArr4, "abcd\0\0\0\0", 9 );
        TEST_ASSERT_EQUAL_INT( 0, equals );

        /* check buffer boundaries */
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr4_withBorders[0] );
        TEST_ASSERT_EQUAL_INT( ' ', dynTestArr4_withBorders[10] );
    }
}

static void testSplitAndJoin(void) {
    /* test standard case */
    {
        char dynTestArr1[10] = "He\xE2\x82\xACllo";
        utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
        utf8string2tuple_t tuple;

        tuple = utf8stringbuf_split_in_2( dynTestBuf1, 2 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, tuple.error );
        TEST_ASSERT_EQUAL_STRING( "He", tuple.first );
        TEST_ASSERT_EQUAL_STRING( "\xE2\x82\xACllo", tuple.second );
        utf8stringbuf_join( dynTestBuf1 );
        TEST_ASSERT_EQUAL_STRING( "He\xE2\x82\xACllo", dynTestBuf1.buf );
    }
    /* test trailing non-zeroes */
    {
        char dynTestArr2[10] = "Hello\0QQQ";
        utf8stringbuf_t dynTestBuf2 = UTF8STRINGBUF(dynTestArr2);
        utf8string2tuple_t tuple2;

        tuple2 = utf8stringbuf_split_in_2( dynTestBuf2, 2 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, tuple2.error );
        TEST_ASSERT_EQUAL_STRING( "He", tuple2.first );
        TEST_ASSERT_EQUAL_STRING( "llo", tuple2.second );
        utf8stringbuf_join( dynTestBuf2 );
        TEST_ASSERT_EQUAL_STRING( "Hello", dynTestBuf2.buf );
    }
    /* test truncation */
    {
        char dynTestArr3[10] = "Hello \xE2\x82\xAC";
        utf8stringbuf_t dynTestBuf3 = UTF8STRINGBUF(dynTestArr3);
        utf8string2tuple_t tuple3;

        tuple3 = utf8stringbuf_split_in_2( dynTestBuf3, 2 );
        TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, tuple3.error );
        TEST_ASSERT_EQUAL_STRING( "He", tuple3.first );
        TEST_ASSERT_EQUAL_STRING( "llo ", tuple3.second );
        utf8stringbuf_join( dynTestBuf3 );
        TEST_ASSERT_EQUAL_STRING( "Hello ", dynTestBuf3.buf );
    }
}

static void testGetEnd(void)
{
    int error;
    int equal;
    char dynTestArr1[4] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);
    utf8stringbuf_t dynTestEnd;

    //  test utf8stringbuf_get_end
    dynTestEnd = utf8stringbuf_get_end( dynTestBuf1 );
    error = utf8stringbuf_append_str( dynTestEnd, "Aa" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_SUCCESS, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "Aa" );
    TEST_ASSERT_EQUAL_INT( 1, equal );

    dynTestEnd = utf8stringbuf_get_end( dynTestBuf1 );
    error = utf8stringbuf_append_str( dynTestEnd, "Bb" );
    TEST_ASSERT_EQUAL_INT( UTF8ERROR_TRUNCATED, error );
    equal = utf8stringbuf_equals_str( dynTestBuf1, "AaB" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
}


/*
 * Copyright 2012-2021 Andreas Warnke
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


