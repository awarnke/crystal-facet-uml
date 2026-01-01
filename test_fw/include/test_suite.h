/* File: test_suite.h; Copyright and License: see below */

#ifndef TEST_SUITE_H
#define TEST_SUITE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides a set of unit test functions - in line with the xunit architecture.
 *  see https://en.wikipedia.org/wiki/XUnit
 */

#include "test_case_result.h"
#include "test_case.h"
#include "test_fixture.h"
#include <stdbool.h>

/*!
 *  \brief constants of simple test suite
 */
enum test_suite_max_enum {
    TEST_SUITE_MAX_TEST_CASES = 48,  /*!< maximum number test cases per test suite */
};

/*!
 *  \brief attributes of a test suite: fixture-setup, fixture-tear-down, set of test cases
 */
struct test_suite_struct {
    const char *name;
    test_category_t default_category;  /*!< default category of test cases */
    test_fixture_t * (*setup) (void);  /*!< pointer to setup function of test fixture */
    void (*teardown) (test_fixture_t *test_env);  /*!< pointer to teardown function of test fixture */
    unsigned int test_case_count;  /*!< number of test cases */
    test_case_t test_case[TEST_SUITE_MAX_TEST_CASES];  /*!< array of test case definitions */
};

typedef struct test_suite_struct test_suite_t;

/*!
 *  \brief initializes the test_suite_t
 *
 *  \param name name of test suite
 *  \param default_category default category of test cases
 *  \param setup function pointer to setup function
 *  \param teardown function pointer to teardown function
 *  \param this_ pointer to own object attributes
 */
static inline void test_suite_init( test_suite_t *this_,
                                    const char *name,
                                    test_category_t default_category,
                                    test_fixture_t * (*setup) (void),
                                    void (*teardown) ( test_fixture_t *test_env )
                                  );

/*!
 *  \brief destroys the test_suite_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void test_suite_destroy( test_suite_t *this_ );

/*!
 *  \brief adds a test case to the test_suite_t using the default_category
 *
 *  \param this_ pointer to own object attributes
 *  \param name name of test case
 *  \param test_case function pointer to test_case function
 */
static inline void test_suite_add_test_case( test_suite_t *this_,
                                             const char *name,
                                             test_case_result_t (*test_case) ( test_fixture_t *test_env )
                                           );

/*!
 *  \brief adds a test case to the test_suite_t
 *
 *  \param this_ pointer to own object attributes
 *  \param name name of test case
 *  \param category category of test case; the default_category of this test_suite_t is ignored.
 *  \param test_case function pointer to test_case function
 */
static inline void test_suite_add_special_test_case( test_suite_t *this_,
                                                     const char *name,
                                                     test_category_t category,
                                                     test_case_result_t (*test_case) ( test_fixture_t *test_env )
                                                   );

/*!
 *  \brief adds a test case to the test_suite_t
 *
 *  \param this_ pointer to own object attributes
 *  \return number of test cases
 */
static inline unsigned int test_suite_get_test_case_count( test_suite_t *this_ );

/*!
 *  \brief executes a test case
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the test case, value between 0 and (test_case_count-1)
 *  \return ok or error, indicating the success of the test case
 */
static inline test_case_result_t test_suite_run_test_case( test_suite_t *this_, unsigned int index );

/*!
 *  \brief gets the name of the test_suite_t
 *
 *  \param this_ pointer to own object attributes
 *  \return name of test suite
 */
static inline const char* test_suite_get_name( test_suite_t *this_ );

/*!
 *  \brief gets the test_case_t in the test_suite_t
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the test case, value between 0 and (test_case_count-1)
 *  \return a const pointer to the test_case_t
 */
static inline const test_case_t* test_suite_get_test_case( test_suite_t *this_, unsigned int index );

#include "test_suite.inl"

#endif  /* TEST_SUITE_H */


/*
Copyright 2019-2026 Andreas Warnke

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
