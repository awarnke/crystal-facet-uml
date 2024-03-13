/* File: utf8stringbuf_performance.c; Copyright and License: see below */

#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf_performance.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>
#include <time.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t testClear( test_fixture_t *fix );
static test_case_result_t testAppendStr( test_fixture_t *fix );
static test_case_result_t testFindFirst( test_fixture_t *fix );
#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
static test_case_result_t testInsertAndDelete( test_fixture_t *fix );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */

test_suite_t utf8stringbuf_performance_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "utf8CodePointTest",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "testClear", &testClear );
    test_suite_add_test_case( &result, "testAppendStr", &testAppendStr );
    test_suite_add_test_case( &result, "testFindFirst", &testFindFirst );
#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
    test_suite_add_test_case( &result, "testInsertAndDelete", &testInsertAndDelete );
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */
    return result;
}

enum testSize_enum{
    TEST_BUF_SIZE=100000,      /*!< string buffer size used for tests */
};
enum testPerformanceReq_enum{
    TEST_MAX_DURATION_PERCENT=125,  /*!< maximum acceptable duration in percent - compared to standard c string library */
    TEST_MAX_DURATION_FACTOR=8,     /*!< maximum acceptable duration as factor for slow functions */
};

struct test_fixture_struct {
    char posix_buf[TEST_BUF_SIZE];
    char utf8_sb_arr[TEST_BUF_SIZE];
    utf8stringbuf_t utf8_sb_buf;
    char temp_arr[TEST_BUF_SIZE];
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static int cpu_perf_factor = 0;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    (*fix).utf8_sb_buf = UTF8STRINGBUF( (*fix).utf8_sb_arr );
    if ( cpu_perf_factor == 0 ) {
        /* measure the time until 3 times new clock measurements were registered */
        clock_t posixCurrent;
        clock_t posixLast;
        int newValues = 0;

        posixLast = clock();
        while ( newValues < 3 ) {
            for ( int loop1 = 0; loop1 < 1000; loop1 ++ ) {
                memset( (*fix).posix_buf, '\0', TEST_BUF_SIZE );
            }
            cpu_perf_factor += 1000;
            posixCurrent = clock();
            if ( posixCurrent != posixLast ) {
                newValues ++;
                posixLast = posixCurrent;
            }
        }
        /* print the time measurement */
        printf( "CPU-Speed correction factor: %d\n", cpu_perf_factor );
    }
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
}

static test_case_result_t testClear( test_fixture_t *fix )
{
    assert( fix != NULL );
    clock_t posixStart;
    clock_t posixEnd;
    clock_t posixDiff;
    clock_t utf8sbStart;
    clock_t utf8sbEnd;
    clock_t utf8sbDiff;
    int loopMax = 40*cpu_perf_factor;

    /* measure the time of the standard posix function */
    posixStart = clock();
    for ( int loop1 = 0; loop1 < loopMax; loop1 ++ ) {
        memset( (*fix).posix_buf, '\0', TEST_BUF_SIZE );
    }
    posixEnd = clock();
    posixDiff = posixEnd - posixStart;

    /* measure the time of the utf8stringbuf function */
    utf8sbStart = clock();
    for ( int loop2 = 0; loop2 < loopMax; loop2 ++ ) {
        utf8stringbuf_clear( (*fix).utf8_sb_buf );
    }
    utf8sbEnd = clock();
    utf8sbDiff = utf8sbEnd - utf8sbStart;

    /* check the result */
    int equal = memcmp( (*fix).posix_buf, (*fix).utf8_sb_arr, TEST_BUF_SIZE );
    TEST_EXPECT_EQUAL_INT( 0, equal );

    /* print the time measurement */
    printf( "CPU-Time: %ld (POSIX) vs %ld (utf8stringbuf) [s/%ld] for clear\n", (long)posixDiff, (long)utf8sbDiff, (long)CLOCKS_PER_SEC );

    /* We are at least TEST_MAX_DURATION_PERCENT as fast as POSIX: */
    TEST_EXPECT( ( utf8sbDiff * 100 ) <= ( posixDiff * TEST_MAX_DURATION_PERCENT ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testAppendStr( test_fixture_t *fix )
{
    assert( fix != NULL );
    clock_t posixStart;
    clock_t posixEnd;
    clock_t posixDiff;
    clock_t utf8sbStart;
    clock_t utf8sbEnd;
    clock_t utf8sbDiff;
    int loopMax = 80*cpu_perf_factor;

    /* measure the time of the standard posix function */
    posixStart = clock();
    for ( int loop1 = 0; loop1 < loopMax; loop1 ++ ) {
        if (( loop1 & 0x3ff ) == 0 ) {
            /* reset */
            (*fix).posix_buf[0] = '\0';
        }
        int pos = strlen( (*fix).posix_buf );
        //strncpy( &((*fix).posix_buf[pos]), "Hello World", TEST_BUF_SIZE-pos );
        strncat( &((*fix).posix_buf[pos]), "Hello World 345678[20]345678[30]34567890", TEST_BUF_SIZE-pos );
    }
    posixEnd = clock();
    posixDiff = posixEnd - posixStart;

    /* measure the time of the utf8stringbuf function */
    utf8sbStart = clock();
    for ( int loop2 = 0; loop2 < loopMax; loop2 ++ ) {
        if (( loop2 & 0x3ff ) == 0 ) {
            /* reset */
            (*fix).utf8_sb_arr[0] = '\0';
        }
        utf8stringbuf_append_str( (*fix).utf8_sb_buf, "Hello World 345678[20]345678[30]34567890" );
    }
    utf8sbEnd = clock();
    utf8sbDiff = utf8sbEnd - utf8sbStart;

    /* check the result */
    //int equal = strcmp( (*fix).posix_buf, (*fix).utf8_sb_arr );
    int equal = memcmp( (*fix).posix_buf, (*fix).utf8_sb_arr, TEST_BUF_SIZE );
    TEST_EXPECT_EQUAL_INT( 0, equal );

    /* print the time measurement */
    printf( "CPU-Time: %ld (POSIX) vs %ld (utf8stringbuf) [s/%ld] for append string\n", (long)posixDiff, (long)utf8sbDiff, (long)CLOCKS_PER_SEC );

    /* We are at least TEST_MAX_DURATION_PERCENT as fast as POSIX: */
    TEST_EXPECT( ( utf8sbDiff * 100 ) <= ( posixDiff * TEST_MAX_DURATION_PERCENT ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testFindFirst( test_fixture_t *fix )
{
    assert( fix != NULL );
    clock_t posixStart;
    clock_t posixEnd;
    clock_t posixDiff;
    clock_t utf8sbStart;
    clock_t utf8sbEnd;
    clock_t utf8sbDiff;
    int loopMax = 20*cpu_perf_factor;

    char* posixLoc = NULL;
    int utf8sbLoc = -1;

    /* measure the time of the standard posix function */
    posixStart = clock();
    for ( int loop1 = 0; loop1 < loopMax; loop1 ++ ) {
        posixLoc = strstr( (*fix).posix_buf, "1234");
    }
    posixEnd = clock();
    posixDiff = posixEnd - posixStart;

    /* measure the time of the utf8stringbuf function */
    utf8sbStart = clock();
    for ( int loop2 = 0; loop2 < loopMax; loop2 ++ ) {
        utf8sbLoc = utf8stringbuf_find_first_str( (*fix).utf8_sb_buf, "1234" );
    }
    utf8sbEnd = clock();
    utf8sbDiff = utf8sbEnd - utf8sbStart;

    /* check the result */
    TEST_EXPECT( posixLoc == NULL );
    TEST_EXPECT_EQUAL_INT( -1, utf8sbLoc );

    /* print the time measurement */
    printf( "CPU-Time: %ld (POSIX) vs %ld (utf8stringbuf) [s/%ld] for find string\n", (long)posixDiff, (long)utf8sbDiff, (long)CLOCKS_PER_SEC );

    /* We are at least TEST_MAX_DURATION_PERCENT as fast as POSIX: */
    TEST_EXPECT( ( utf8sbDiff * 100 ) <= ( posixDiff * TEST_MAX_DURATION_PERCENT ) );
    return TEST_CASE_RESULT_OK;
}

#ifdef UTF8STRINGBUF_UNCHECKED_RANGE
const char EXAMPLE_DATA[] =
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"

    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"

    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"

    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890"
    "12345678[10]345678[20]345678[30]345678[40]345678[50]345678[60]34567890";

/*enum TestPositionsAndSizes { TEST_INDEX = 21, TEST_SHIFT = 678, };*/

static test_case_result_t testInsertAndDelete( test_fixture_t *fix )
{
    assert( fix != NULL );
    clock_t posixStart;
    clock_t posixEnd;
    clock_t posixDiff;
    clock_t utf8sbStart;
    clock_t utf8sbEnd;
    clock_t utf8sbDiff;
    int loopMax = 200*cpu_perf_factor;

    /* prepare test */
    memcpy( (*fix).posix_buf, EXAMPLE_DATA, sizeof(EXAMPLE_DATA) );

    /* measure the time of the standard posix function */
    posixStart = clock();
    for ( int loop1 = 0; loop1 < loopMax; loop1 ++ ) {
        const int TEST_INDEX = (loop1 & 0x1f);
        const int TEST_SHIFT = (loop1 & 0x3ff);
        /* insert TEST_SHIFT at pos TEST_INDEX */
        /* Note: memcpy must not copy between overlapping regions. Therefore, we need to copy to a (*fix).temp_arr first */
        memcpy( (*fix).temp_arr, &((*fix).posix_buf[TEST_INDEX]), sizeof(EXAMPLE_DATA)-TEST_INDEX );
        memcpy( &((*fix).posix_buf[TEST_INDEX+TEST_SHIFT]), (*fix).temp_arr, sizeof(EXAMPLE_DATA)-TEST_INDEX );
        memcpy( &((*fix).posix_buf[TEST_INDEX]), EXAMPLE_DATA, TEST_SHIFT );
        /* delete TEST_SHIFT at pos TEST_INDEX */
        memcpy( (*fix).temp_arr, &((*fix).posix_buf[TEST_INDEX+TEST_SHIFT]), sizeof(EXAMPLE_DATA)-TEST_INDEX );
        memcpy( &((*fix).posix_buf[TEST_INDEX]), (*fix).temp_arr, sizeof(EXAMPLE_DATA)-TEST_INDEX );
    }
    posixEnd = clock();
    posixDiff = posixEnd - posixStart;

    /* prepare test */
    memcpy( (*fix).utf8_sb_arr, EXAMPLE_DATA, sizeof(EXAMPLE_DATA) );

    /* measure the time of the utf8stringbuf function */
    utf8sbStart = clock();
    for ( int loop2 = 0; loop2 < loopMax; loop2 ++ ) {
        const int TEST_INDEX = (loop2 & 0x1f);
        const int TEST_SHIFT = (loop2 & 0x3ff);
        /* insert TEST_SHIFT at pos TEST_INDEX */
        utf8stringbuf_insert_str( (*fix).utf8_sb_buf, TEST_INDEX, &(EXAMPLE_DATA[sizeof(EXAMPLE_DATA)-1-TEST_SHIFT]) );
        /* delete TEST_SHIFT at pos TEST_INDEX */
        utf8stringbuf_delete( (*fix).utf8_sb_buf, TEST_INDEX, TEST_SHIFT );
    }
    utf8sbEnd = clock();
    utf8sbDiff = utf8sbEnd - utf8sbStart;

    /* check the result */
    int equal = memcmp( (*fix).posix_buf, (*fix).utf8_sb_arr, sizeof(EXAMPLE_DATA) );
    TEST_EXPECT_EQUAL_INT( 0, equal );

    /* print the time measurement */
    printf( "CPU-Time: %ld (POSIX) vs %ld (utf8stringbuf) [s/%ld] for insert and delete\n", (long)posixDiff, (long)utf8sbDiff, (long)CLOCKS_PER_SEC );

    /* We are at least TEST_MAX_DURATION_PERCENT as fast as POSIX: */
    TEST_EXPECT( ( utf8sbDiff ) <= ( posixDiff * TEST_MAX_DURATION_FACTOR ) );
    return TEST_CASE_RESULT_OK;
}
#endif  /* UTF8STRINGBUF_UNCHECKED_RANGE */


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
