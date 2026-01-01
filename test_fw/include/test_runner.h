/* File: test_runner.h; Copyright and License: see below */

#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Executes a set of unit test suites - in line with the xunit architecture.
 *  see https://en.wikipedia.org/wiki/XUnit
 */

#include <stdint.h>
#include <stdbool.h>
#include "test_category.h"
#include "test_result.h"
#include "test_suite.h"

/*!
 *  \brief attributes of a test runner
 */
struct test_runner_struct {
    test_category_t mask;  /*!< ignore the 0-bits of the mask in a test cases test_category */
    test_category_t pattern;  /*!< select the test cases where test_category matches pattern */
    test_result_t result;  /*!< test result statistics */
};

typedef struct test_runner_struct test_runner_t;

/*!
 *  \brief initializes the test_runner_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void test_runner_init( test_runner_t *this_ );

/*!
 *  \brief destroys the test_runner_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void test_runner_destroy( test_runner_t *this_ );

/*!
 *  \brief sets mask and pattern of the test_runner_t
 *
 *  \param this_ pointer to own object attributes
 *  \param mask ignore the 0-bits of the mask in a test cases test_category
 *  \param pattern select the test cases where test_category matches pattern
 */
static inline void test_runner_set_filter( test_runner_t *this_, test_category_t mask, test_category_t pattern );

/*!
 *  \brief test runner - runs a test suite
 *
 *  \param this_ pointer to own object attributes
 *  \param test_suite test suite to run
 */
static inline void test_runner_run_suite( test_runner_t *this_, test_suite_t test_suite );

/*!
 *  \brief get test runner result
 *
 *  \param this_ pointer to own object attributes
 */
static inline test_result_t test_get_result( test_runner_t *this_ );

#include "test_runner.inl"

#endif  /* TEST_RUNNER_H */


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
