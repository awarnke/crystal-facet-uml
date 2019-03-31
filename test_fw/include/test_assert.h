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

#define TEST_FAIL() {fprintf(stderr,"FAIL at %s:%d\n",__FILE__,__LINE__);exit(-1);}
#define TEST_ASSERT(COND) if (!(COND)) TEST_FAIL()
#define TEST_ASSERT_EQUAL_INT(EXPECTED,ACTUAL) if ((EXPECTED)!=(ACTUAL)) TEST_FAIL()
#define TEST_ASSERT_EQUAL_STRING(EXPECTED,ACTUAL) if (((EXPECTED)==NULL)||((ACTUAL)==NULL)||(0!=strcmp((EXPECTED),(ACTUAL)))) TEST_FAIL()
/* EPSILON(x) = 2.2204460493 e−016 */
/* double precision of mantissa: 2.22e-16 - adding factor 100 to compensate rounding errors of previous calculations */
#define TEST_DOUBLE_EPSILON 2.22e-14
/* TINIEST((x) = 4.9406564584 e−324 */
/* double tiniest: 4.94e-324 - adding factor 100 to compensate rounding errors of previous calculations - relevant if EXPECTED==0.0 */
#define TEST_DOUBLE_TINIEST 4.94e-322
#define TEST_ASSERT_EQUAL_DOUBLE(EXPECTED,ACTUAL) if ( fabs((EXPECTED)-(ACTUAL)) > ((fabs(EXPECTED)*TEST_DOUBLE_EPSILON)+TEST_DOUBLE_TINIEST) ) TEST_FAIL()
/* EPSILON(x) = 1.192092896 e-07 */
/* float precision of mantissa: 1.19e-7 - adding factor 100 to compensate rounding errors of previous calculations */
#define TEST_FLOAT_EPSILON 1.19e-5
/* TINIEST((x) = 1.401298464 e−45 */
/* double tiniest: 1.40e-45 - adding factor 100 to compensate rounding errors of previous calculations - relevant if EXPECTED==0.0 */
#define TEST_FLOAT_TINIEST 1.40e-43
#define TEST_ASSERT_EQUAL_FLOAT(EXPECTED,ACTUAL) if ( fabs((EXPECTED)-(ACTUAL)) > ((fabs(EXPECTED)*TEST_FLOAT_EPSILON)+TEST_FLOAT_TINIEST) ) TEST_FAIL()

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
