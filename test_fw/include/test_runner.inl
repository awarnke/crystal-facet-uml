/* File: test_runner.inl; Copyright and License: see below */

#include "test_case_result.h"
#include <stdio.h>
#include <unistd.h>

static inline void test_runner_init( test_runner_t *this_ )
{
    test_result_init( &((*this_).result) );
}

static inline void test_runner_destroy( test_runner_t *this_ )
{
    test_result_destroy( &((*this_).result) );
}

static inline void test_runner_set_filter( test_runner_t *this_, test_category_t mask, test_category_t pattern )
{
    (*this_).mask = mask;
    (*this_).pattern = pattern;
}

static inline void test_runner_run_suite( test_runner_t *this_, test_suite_t test_suite )
{
    test_result_t suite_local_result;
    test_result_init( &suite_local_result );
    fprintf( stdout, "test suite: %s\n", test_suite_get_name( &test_suite ) );
    unsigned int count = test_suite_get_test_case_count( &test_suite );
    for ( unsigned int idx = 0; idx < count; idx ++ )
    {
        const test_case_t *current_case = test_suite_get_test_case( &test_suite, idx );
        assert( current_case != NULL );
        test_category_t current_cat = test_case_get_catgegory( current_case );
        if (( current_cat & (*this_).mask ) == (*this_).pattern )
        {
            fprintf( stdout, "  test case: %s\n", test_case_get_name( current_case ) );
            fflush( stdout );  /* help to localize errors in case of sudden termination */
            test_case_result_t tc_result;
            tc_result = test_suite_run_test_case( &test_suite, idx );
            test_result_add_test_case_result( &((*this_).result), tc_result );
            test_result_add_test_case_result( &suite_local_result, tc_result );
        }
        else
        {
            test_result_add_skipped( &((*this_).result) );
            test_result_add_skipped( &suite_local_result );
        }
    }
    fprintf( stdout,
             "test result: skipped: %d, passed %d, failed: %d\n",
             test_result_get_skipped( &suite_local_result ),
             test_result_get_passed( &suite_local_result ),
             test_result_get_failed( &suite_local_result )
           );
    if ( test_result_get_failed( &suite_local_result ) != 0 )
    {
        /* wait some seconds for the user to interrupt here */
        sleep( 3 );
    }
    test_result_destroy( &suite_local_result );
}

static inline test_result_t test_get_result( test_runner_t *this_ )
{
    return (*this_).result;
}


/*
Copyright 2019-2025 Andreas Warnke

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
