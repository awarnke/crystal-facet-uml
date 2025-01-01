/* File: test_category.h; Copyright and License: see below */

#ifndef TEST_CATEGORY_H
#define TEST_CATEGORY_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Specifies a set of categories by which test cases can be selected
 *         for test execution.
 */

/*!
 *  \brief bitmask defining the categories of a test suite
 */
enum test_category_enum {
    TEST_CATEGORY_UNIT = 0x01000000,  /*!< The test goal is to verify the inner working of a single software unit */
    TEST_CATEGORY_INTEGRATION = 0x02000000,  /*!< The test goal is to verify the inter-working of several integrated software units */
    TEST_CATEGORY_QUALIFICATION = 0x04000000,  /*!< The test goal is to verify the external interfaces of an integrated software */

    TEST_CATEGORY_CONTINUOUS = 0x00010000,  /*!< The intended test cycle is 'always', at each build */
    TEST_CATEGORY_RELEASE = 0x00020000,  /*!< The intended test cycle is 'once per release' */
    TEST_CATEGORY_QUEST = 0x00040000,  /*!< The intended test cycle is 'none', tests shall only be executed when manually triggered */

    TEST_CATEGORY_COVERAGE = 0x00000100,  /*!< The test is suitable for line and branch coverage tests. */
                                          /*!< Note that a test case that runs through so many lines of code */
                                          /*!< that the final result check can not detect all possible errors */
                                          /*!< is not suitable for measuring line or branch coverage. */
};

typedef enum test_category_enum test_category_t;

#endif  /* TEST_CATEGORY_H */


/*
Copyright 2024-2025 Andreas Warnke

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
