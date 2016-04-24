/* File: log.h; Copyright and License: see below */

#ifndef LOG_H
#define LOG_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Prints error and event logs to a byte stream
 */

#include <stdio.h>
#include <stdlib.h>

#define LOG_OUT_STREAM stdout

/*!
 *  \brief logs an error if the argument is false, and terminates the program
 *
 *  Guideline: Use LOG_ASSERT whenever you check a condition that is always valid if the current software module is bug-free.
 *  Use LOG_ERROR_xxx when you check a condition that should be valid assumed remote software modules are bug-free.
 *  Goal: The current software module should be robust in case remote software modules are using the interfaces in a wrong way.
 */
#define LOG_ASSERT(x) { if (!(x)) { fprintf(LOG_OUT_STREAM,"\nERR : ASSERTION %s FAILED AT %s:%i\n",#x,__FILE__,__LINE__); exit(-1); } }

/*!
 *  \brief logs an error string
 */
#define LOG_ERROR(x) { const char *string_test = x; fprintf(LOG_OUT_STREAM,"\nERR : %s\n",string_test); }

/*!
 *  \brief logs an error string and an integer
 */
#define LOG_ERROR_INT(x,i) { const char *string_test = x; const int int_test = i; fprintf(LOG_OUT_STREAM,"\nERR : %s %i\n",string_test,int_test); }

/*!
 *  \brief logs an error string and a hexadecimal integer
 */
#define LOG_ERROR_HEX(x,i) { const char *string_test = x; const unsigned int int_test = i; fprintf(LOG_OUT_STREAM,"\nERR : %s 0x%x\n",string_test,int_test); }

/*!
 *  \brief logs an error string and an information string
 */
#define LOG_ERROR_STR(x,s) { const char *string_test = x; const char *string2_test = s; fprintf(LOG_OUT_STREAM,"\nERR : %s %s\n",string_test,string2_test); }

/*!
 *  \brief logs a warning string
 */
#define LOG_WARNING(x) { const char *string_test = x; fprintf(LOG_OUT_STREAM,"\nWARN: %s\n",string_test); }

/*!
 *  \brief logs a warning string and an integer
 */
#define LOG_WARNING_INT(x,i) { const char *string_test = x; const int int_test = i; fprintf(LOG_OUT_STREAM,"\nWARN: %s %i\n",string_test,int_test); }

/*!
 *  \brief logs a warning string and a hexadecimal integer
 */
#define LOG_WARNING_HEX(x,i) { const char *string_test = x; const unsigned int int_test = i; fprintf(LOG_OUT_STREAM,"\nWARN: %s 0x%x\n",string_test,int_test); }

/*!
 *  \brief logs a warning string and an information string
 */
#define LOG_WARNING_STR(x,s) { const char *string_test = x; const char *string2_test = s; fprintf(LOG_OUT_STREAM,"\nWARN: %s %s\n",string_test,string2_test); }

/*!
 *  \brief logs an event string
 */
#define LOG_EVENT(x) { const char *string_test = x; fprintf(LOG_OUT_STREAM,"EVT : %s\n",string_test); }

/*!
 *  \brief logs an event string and an integer
 */
#define LOG_EVENT_INT(x,i) { const char *string_test = x; const int int_test = i; fprintf(LOG_OUT_STREAM,"EVT : %s %i\n",string_test,int_test); }

/*!
 *  \brief logs an event string and a hexadecimal integer
 */
#define LOG_EVENT_HEX(x,i) { const char *string_test = x; const unsigned int int_test = i; fprintf(LOG_OUT_STREAM,"EVT : %s 0x%x\n",string_test,int_test); }

/*!
 *  \brief logs an event string and an information string
 */
#define LOG_EVENT_STR(x,s) { const char *string_test = x; const char *string2_test = s; fprintf(LOG_OUT_STREAM,"EVT : %s %s\n",string_test,string2_test); }

#endif  /* LOG_H */


/*
Copyright 2016-2016 Andreas Warnke

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
