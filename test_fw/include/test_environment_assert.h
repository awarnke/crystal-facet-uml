/* File: test_environment_assert.h; Copyright and License: see below */

#ifndef TEST_ENVIRONMENT_ASSERT_H
#define TEST_ENVIRONMENT_ASSERT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief provides assert functions for test environment
 *
 *  These asserts fail even in NDEBUG mode.
 */

#include <stdio.h>
#include <stdlib.h>

/* Assertion to throw if test environment is wrong (not if test case failed). */
/* In contrast to assert.h, this always terminates test execution, even in NDEBUG mode. */
#define TEST_ENVIRONMENT_ASSERT(COND)\
if (!(COND)) \
{fprintf(stderr,"TEST ENVIRONMENT ERROR (!(%s)) at %s:%d\n",#COND,__FILE__,__LINE__);exit(-1);}

/* Assertion to throw if test environment is wrong (not if test case failed). */
/* In contrast to assert.h, this always terminates test execution, even in NDEBUG mode. */
#define TEST_ENVIRONMENT_ASSERT_EQUAL_INT(EXPECTED,ACTUAL)\
{const long long exp = (EXPECTED); const long long act = (ACTUAL); if (exp!=act) \
{fprintf(stderr,"TEST ENVIRONMENT ERROR ((%s)==%lld!=%lld==(%s)) at %s:%d\n",\
#EXPECTED,exp,act,#ACTUAL,__FILE__,__LINE__);exit(-1);} \
}

#endif  /* TEST_ENVIRONMENT_ASSERT_H */


/*
Copyright 2019-2022 Andreas Warnke

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
