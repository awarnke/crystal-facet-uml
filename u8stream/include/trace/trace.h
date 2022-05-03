/* File: trace.h; Copyright and License: see below */

#ifndef TRACE_H
#define TRACE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Prints traces to a byte stream
 */

#include <stdio.h>
#include <time.h>

#define TRACE_OUT_STREAM stdout
/* #define TRACE_OUT_STREAM stderr */  /* stderr helps to locate sudden termination errors */
#define TRACE_INDENT_MOD (32u)
#define TRACE_INDENT_MAX (31u)
#define TRACE_INDENT_STEP (2u)
#define TRACE_NULLTERM_SIZE (1u)
extern __thread unsigned int trace_indent_depth;
extern const char trace_indent_pattern_begin[ TRACE_INDENT_STEP * TRACE_INDENT_MAX + TRACE_NULLTERM_SIZE ];
extern const char trace_indent_pattern_end[ TRACE_INDENT_STEP * TRACE_INDENT_MAX + TRACE_NULLTERM_SIZE ];
extern const char trace_indent_pattern_info[ TRACE_INDENT_STEP * TRACE_INDENT_MAX + TRACE_NULLTERM_SIZE ];
#define TRACE_INDENT_BEGIN \
    (&(trace_indent_pattern_begin[((TRACE_INDENT_MAX-trace_indent_depth)%TRACE_INDENT_MOD)*TRACE_INDENT_STEP]))
#define TRACE_INDENT_END \
    (&(trace_indent_pattern_end[((TRACE_INDENT_MAX-trace_indent_depth)%TRACE_INDENT_MOD)*TRACE_INDENT_STEP]))
#define TRACE_INDENT_INFO \
    (&(trace_indent_pattern_info[((TRACE_INDENT_MAX-trace_indent_depth)%TRACE_INDENT_MOD)*TRACE_INDENT_STEP]))

#ifndef NDEBUG  /* SWITCH */

/*!
 *  \brief a condition to determine if tracing is active
 */
#define TRACE_ACTIVE (true)

/*!
 *  \brief traces a string
 */
#define TRACE_INFO(x) { const char *string_test = (x); fprintf(TRACE_OUT_STREAM,"%s%s\n",TRACE_INDENT_INFO,string_test); }

/*!
 *  \brief traces a string and an integer
 */
#define TRACE_INFO_INT(x,i) { const char *string_test = (x); const int int_test = (i); fprintf(TRACE_OUT_STREAM,"%s%s %i\n",TRACE_INDENT_INFO,string_test,int_test); }

/*!
 *  \brief traces a string and two integers
 */
#define TRACE_INFO_INT_INT(x,i,j) { const char *string_test = (x); const int int_test = (i); const int int_test2 = (j); fprintf(TRACE_OUT_STREAM,"%s%s %i, %i\n",TRACE_INDENT_INFO,string_test,int_test,int_test2); }

/*!
 *  \brief traces a string and a hexadecimal integer
 */
#define TRACE_INFO_HEX(x,i) { const char *string_test = (x); const unsigned int int_test = (i); fprintf(TRACE_OUT_STREAM,"%s%s 0x%x\n",TRACE_INDENT_INFO,string_test,int_test); }

/*!
 *  \brief traces a string and a pointer
 */
#define TRACE_INFO_PTR(x,p) { const char *string_test = (x); const void *ptr_test = (p); fprintf(TRACE_OUT_STREAM,"%s%s @:%p\n",TRACE_INDENT_INFO,string_test,ptr_test); }

/*!
 *  \brief traces a string and and an information string
 */
#define TRACE_INFO_STR(x,s) { const char *string_test = (x); const char *string2_test = (s); fprintf(TRACE_OUT_STREAM,"%s%s %s\n",TRACE_INDENT_INFO,string_test,string2_test); }

/*!
 *  \brief traces a string and a double
 */
#define TRACE_INFO_FLT(x,r) { const char *string_test = (x); const double real_test = (r); fprintf(TRACE_OUT_STREAM,"%s%s %f\n",TRACE_INDENT_INFO,string_test,real_test); }

/*!
 *  \brief traces a function start
 *
 *  Note: For every TRACE_BEGIN, one TRACE_END shall be called to create a nicely indented trace output
 */
#define TRACE_BEGIN() const unsigned int trace_symmetry_test=trace_indent_depth; { fprintf(TRACE_OUT_STREAM,"%s%s [begin]\n",TRACE_INDENT_BEGIN,__func__); trace_indent_depth++; }

/*!
 *  \brief traces a function return
 */
#define TRACE_END() { trace_indent_depth--; fprintf(TRACE_OUT_STREAM,"%s%s [end]\n",TRACE_INDENT_END,__func__); (void)trace_symmetry_test; }

/*!
 *  \brief traces a function return and an error code if the error does not equal 0
 */
#define TRACE_END_ERR(e) { const int int_test = (e); if ( int_test==0 ) {TRACE_END();} else { trace_indent_depth--; fprintf(TRACE_OUT_STREAM,"%s%s [end] ERR=0x%x\n",TRACE_INDENT_END,__func__,int_test); } (void)trace_symmetry_test; }

/*!
 *  \brief ensures to flush all data.
 *
 *  Call this macro before fork() or when you expect a process crash (e.g. SIGSEGV).
 */
#define TRACE_FLUSH() { fflush(TRACE_OUT_STREAM); }

/*!
 *  \brief traces a timestamp
 */
#ifdef _WIN32
#define TRACE_TIMESTAMP() { const clock_t now = clock(); fprintf(TRACE_OUT_STREAM,"%s[%i+%i/CLOCKS_PER_SEC]\n",TRACE_INDENT_INFO,(int)(now/CLOCKS_PER_SEC),(int)(now%CLOCKS_PER_SEC)); }
#else
#define TRACE_TIMESTAMP() { struct timespec tp; const int err = clock_gettime(CLOCK_MONOTONIC,&tp); fprintf(TRACE_OUT_STREAM,"%s[%i.%06i %s]\n",TRACE_INDENT_INFO,(int)(tp.tv_sec),(int)(tp.tv_nsec/1000),(err!=0)?"?":"sec"); }
#endif

#else  /* SWITCH */

#ifdef __GNUC__
#define ATTR_UNUSED __attribute__((unused))
#else
#define ATTR_UNUSED
#endif

/*!
 *  \brief a condition to determine if tracing is active
 */
#define TRACE_ACTIVE (false)

/*!
 *  \brief traces a string
 */
#define TRACE_INFO(x) { const char *string_test ATTR_UNUSED = (x); }

/*!
 *  \brief traces a string and an integer
 */
#define TRACE_INFO_INT(x,i) { const char *string_test ATTR_UNUSED = (x); const int int_test ATTR_UNUSED = (i); }

/*!
 *  \brief traces a string and two integers
 */
#define TRACE_INFO_INT_INT(x,i,j) { const char *string_test ATTR_UNUSED = (x); const int int_test ATTR_UNUSED = (i); const int int_test2 ATTR_UNUSED = (j); }

/*!
 *  \brief traces a string and a hexadecimal integer
 */
#define TRACE_INFO_HEX(x,i) { const char *string_test ATTR_UNUSED = (x); const int int_test ATTR_UNUSED = (i); }

/*!
 *  \brief traces a string and a pointer
 */
#define TRACE_INFO_PTR(x,p) { const char *string_test ATTR_UNUSED = (x); const void *ptr_test ATTR_UNUSED = (p); }

/*!
 *  \brief traces a string and and an information string
 */
#define TRACE_INFO_STR(x,s) { const char *string_test ATTR_UNUSED = (x); const char *string2_test ATTR_UNUSED = (s); }

/*!
 *  \brief traces a string and a double
 */
#define TRACE_INFO_FLT(x,r) { const char *string_test ATTR_UNUSED = (x); const double real_test ATTR_UNUSED = (r); }

/*!
 *  \brief traces a function start
 *
 *  Note: For every TRACE_BEGIN, one TRACE_END shall be called to create a nicely indented trace output
 */
#define TRACE_BEGIN() {}

/*!
 *  \brief traces a function return
 */
#define TRACE_END() {}

/*!
 *  \brief traces a function return and an error code if the error does not equal 0
 */
#define TRACE_END_ERR(e) { const int int_test ATTR_UNUSED = (e); }

/*!
 *  \brief ensures to flush all data.
 *
 *  Call this macro before fork() or when you expect a process crash (e.g. SIGSEGV).
 */
#define TRACE_FLUSH() {}

/*!
 *  \brief traces a timestamp
 */
#define TRACE_TIMESTAMP() {}

#endif  /* SWITCH */

#endif  /* TRACE_H */


/*
Copyright 2016-2022 Andreas Warnke

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
