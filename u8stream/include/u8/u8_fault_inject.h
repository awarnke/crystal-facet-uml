/* File: u8_fault_inject.h; Copyright and License: see below */

#ifndef U8_FAULT_INJECT_H
#define U8_FAULT_INJECT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides functions and macros to inject faults during unit test cases
 *
 *  The macros shall avoid accidential activation in productive code.
 *  Like asserts, these shall be active during testing.
 *
 *  The macros shall not introduce branches into the code.
 */

/*! \brief condition that activates selected fault injection macros */
extern __thread unsigned int u8_fault_inject_condition_id;

#ifndef NDEBUG

/*! When calling U8_FAULT_INJECT_COND( COND_ID, ERR_VALUE, OK_VALUE ) it is important to select the OK_VALUE
 *  expression in a way that the result of U8_FAULT_INJECT_COND does not change the behavior in this OK case.
 *
 *  example calls:
 *  result_err |= U8_FAULT_INJECT_COND( TEST_COND_17, U8_ERROR_LOGIC_CORRUPT, 0 );
 *  pointer = U8_FAULT_INJECT_COND( TEST_COND_18, NULL, pointer );
 *
 *  WARNING: This macro is not dummy-proof. Check that you do not modify the logic in NDEBUG mode!
 */
#define U8_FAULT_INJECT_COND(COND_ID,ERR_VALUE,OK_VALUE) \
    (((COND_ID)==u8_fault_inject_condition_id)?(ERR_VALUE):(OK_VALUE))

#define U8_FAULT_INJECT_SETUP(COND_ID) \
    {u8_fault_inject_condition_id=(COND_ID);};

#define U8_FAULT_INJECT_RESET() \
    {u8_fault_inject_condition_id=0;};

#else  // NDEBUG == Release

/*! When calling U8_FAULT_INJECT_COND( COND_ID, ERR_VALUE, OK_VALUE ) it is important to select the OK_VALUE
 *  expression in a way that the result of U8_FAULT_INJECT_COND does not change the behavior in this OK case.
 *
 *  example calls:
 *  result_err |= U8_FAULT_INJECT_COND( TEST_COND_17, U8_ERROR_LOGIC_CORRUPT, 0 );
 *  pointer = U8_FAULT_INJECT_COND( TEST_COND_18, NULL, pointer );
 *
 *  WARNING: This macro is not dummy-proof. Check that you do not modify the logic in NDEBUG mode!
 */
#define U8_FAULT_INJECT_COND(COND_ID,ERR_CODE,OK_VALUE) (OK_VALUE)

#endif  // NDEBUG

#endif  /* U8_FAULT_INJECT_H */


/*
Copyright 2023-2024 Andreas Warnke

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
