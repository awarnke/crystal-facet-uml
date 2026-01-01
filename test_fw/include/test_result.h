/* File: test_result.h; Copyright and License: see below */

#ifndef TEST_RESULT_H
#define TEST_RESULT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides result struct for unit tests.
 */

#include "test_case_result.h"
#include <stdbool.h>

/*!
 *  \brief attributes of a test result
 */
struct test_result_struct {
    unsigned int passed;  /*!< passed number of executed test cases */
    unsigned int failed;  /*!< failed number of executed test cases */
    unsigned int skipped;  /*!< number of skipped/fitered test cases */
};

typedef struct test_result_struct test_result_t;

/*!
 *  \brief initializes the test_result_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void test_result_init( test_result_t *this_ );

/*!
 *  \brief destroys the test_result_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void test_result_destroy( test_result_t *this_ );

/*!
 *  \brief add test result
 *
 *  \param this_ pointer to own object attributes
 *  \param result ok or error depending on the test case result.
 */
static inline void test_result_add_test_case_result( test_result_t *this_, test_case_result_t result );

/*!
 *  \brief add a skipped test case
 *
 *  \param this_ pointer to own object attributes
 */
static inline void test_result_add_skipped( test_result_t *this_ );

/*!
 *  \brief returns the number of passed test cases
 *
 *  \param this_ pointer to own object attributes
 */
static inline unsigned int test_result_get_passed( test_result_t *this_ );

/*!
 *  \brief returns the number of failed test cases
 *
 *  \param this_ pointer to own object attributes
 */
static inline unsigned int test_result_get_failed( test_result_t *this_ );

/*!
 *  \brief returns the number of skipped test cases
 *
 *  \param this_ pointer to own object attributes
 */
static inline unsigned int test_result_get_skipped( test_result_t *this_ );

#include "test_result.inl"

#endif  /* TEST_RESULT_H */


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
