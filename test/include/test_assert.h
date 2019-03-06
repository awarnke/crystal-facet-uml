/* File: test_assert.h; Copyright and License: see below */

#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief provides assert functions for test cases
 */

#include <stdbool.h>
#include <stdio.h>

#define TEST_ASSERT(COND) if (!(COND)) {fprintf(stderr,"FAIL at %s:%d\n",__FILE__,__LINE__);exit(-1);}
#define TEST_ASSERT_EQUAL_INT(EXPECTED,ACTUAL) if (EXPECTED!=ACTUAL) {fprintf(stderr,"FAIL at %s:%d\n",__FILE__,__LINE__);exit(-1);}

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
