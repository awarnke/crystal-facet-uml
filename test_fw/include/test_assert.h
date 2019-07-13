/* File: test_assert.h; Copyright and License: see below */

#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief provides assert functions for test cases
 */

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define TEST_FAIL()\
{fprintf(stderr,"TEST FAILED at %s:%d\n",__FILE__,__LINE__);exit(-1);}

#define TEST_ASSERT(COND)\
if (!(COND)) \
{fprintf(stderr,"TEST FAILED (!(%s)) at %s:%d\n",#COND,__FILE__,__LINE__);exit(-1);}

#define TEST_ASSERT_EQUAL_INT(EXPECTED,ACTUAL)\
{const int exp = (EXPECTED); const int act = (ACTUAL); if (exp!=act) \
{fprintf(stderr,"TEST FAILED ((%s)==%d!=%d==(%s)) at %s:%d\n",\
#EXPECTED,exp,act,#ACTUAL,__FILE__,__LINE__);exit(-1);} \
}

#define TEST_ASSERT_EQUAL_STRING(EXPECTED,ACTUAL)\
{const char *exp = (EXPECTED); const char *act = (ACTUAL);\
if ((exp==NULL)||(act==NULL)||(0!=strcmp(exp,act))) \
{fprintf(stderr,"TEST FAILED ((%s)==%s!=%s==(%s)) at %s:%d\n",\
#EXPECTED,exp,act,#ACTUAL,__FILE__,__LINE__);exit(-1);} \
}

/* EPSILON(x) = 2.2204460493 e−016 */
/* double precision of mantissa: 2.22e-16 - adding factor 100 to compensate rounding errors of previous calculations */
#define TEST_DOUBLE_EPSILON 2.22e-14
/* TINIEST((x) = 4.9406564584 e−324 */
/* double tiniest: 4.94e-324 - adding factor 100 to compensate rounding errors of previous calculations - relevant if EXPECTED==0.0 */
#define TEST_DOUBLE_TINIEST 4.94e-322
#define TEST_ASSERT_EQUAL_DOUBLE(EXPECTED,ACTUAL)\
{const double exp = (EXPECTED); const double act = (ACTUAL);\
if ( fabs(exp-act) > ((fabs(exp)*TEST_DOUBLE_EPSILON)+TEST_DOUBLE_TINIEST) ) \
{fprintf(stderr,"TEST FAILED ((%s)==%f!=%f==(%s)) at %s:%d\n",\
#EXPECTED,exp,act,#ACTUAL,__FILE__,__LINE__);exit(-1);} \
}

/* EPSILON(x) = 1.192092896 e-07 */
/* float precision of mantissa: 1.19e-7 - adding factor 100 to compensate rounding errors of previous calculations */
#define TEST_FLOAT_EPSILON 1.19e-5
/* TINIEST((x) = 1.401298464 e−45 */
/* float tiniest: 1.40e-45 - adding factor 100 to compensate rounding errors of previous calculations - relevant if EXPECTED==0.0 */
#define TEST_FLOAT_TINIEST 1.40e-43
#define TEST_ASSERT_EQUAL_FLOAT(EXPECTED,ACTUAL)\
{const float exp = (EXPECTED); const float act = (ACTUAL);\
if ( fabs(exp-act) > ((fabs(exp)*TEST_FLOAT_EPSILON)+TEST_FLOAT_TINIEST) ) \
{fprintf(stderr,"TEST FAILED ((%s)==%f!=%f==(%s)) at %s:%d\n",\
#EXPECTED,exp,act,#ACTUAL,__FILE__,__LINE__);exit(-1);} \
}

/* Assertion to throw if test environment is wrong (not if test case failed). */
/* In contrast to assert.h, this always terminates test execution, even in NDEBUG mode. */
#define TEST_ENVIRONMENT_ASSERT(COND)\
if (!(COND)) \
{fprintf(stderr,"TEST ENVIRONMENT ERROR (!(%s)) at %s:%d\n",#COND,__FILE__,__LINE__);exit(-1);}

#endif  /* TEST_ASSERT_H */


/*
Copyright 2019-2019 Andreas Warnke

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
