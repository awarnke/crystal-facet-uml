/* File: trace.h; Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

#ifndef TRACE_H
#define TRACE_H

/* public file for the doxygen documentation: */
/*! \file */

#include <stdio.h>

#define TRACE_OUT_STREAM stdout
#define TRACE_INDENT_MAX (16)
extern __thread int trace_indent_depth;
extern const char trace_indent_pattern[2*(TRACE_INDENT_MAX-1)+1];
#define TRACE_INDENT (&(trace_indent_pattern[((16*TRACE_INDENT_MAX-1-trace_indent_depth)%TRACE_INDENT_MAX)*2]))

/*!
 *  \brief traces a string
 */
#define TRACE_INFO(x) { const char *string_test = x; fprintf(TRACE_OUT_STREAM,"%s%s\n",TRACE_INDENT,string_test); }

/*!
 *  \brief traces a string and an integer
 */
#define TRACE_INFO_INT(x,i) { const char *string_test = x; const int int_test = i; fprintf(TRACE_OUT_STREAM,"%s%s %i\n",TRACE_INDENT,string_test,int_test); }

/*!
 *  \brief traces a string and two integers
 */
#define TRACE_INFO_INT_INT(x,i,j) { const char *string_test = x; const int int_test = i; const int int_test2 = j; fprintf(TRACE_OUT_STREAM,"%s%s %i %i\n",TRACE_INDENT,string_test,int_test,int_test2); }

/*!
 *  \brief traces a function start
 * 
 *  Note: For every TRACE_BEGIN, one TRACE_END shall be called to create a nicely indented trace output
 */
#define TRACE_BEGIN() { fprintf(TRACE_OUT_STREAM,"%s%s [begin]\n",TRACE_INDENT,__func__); trace_indent_depth++; }

/*!
 *  \brief traces a function return
 */
#define TRACE_END() { trace_indent_depth--; fprintf(TRACE_OUT_STREAM,"%s%s [end]\n",TRACE_INDENT,__func__); }

/*!
 *  \brief traces a function return and an error code if the error does not equal 0
 */
#define TRACE_END_ERR(e) { if ( e==0 ) {TRACE_END();} else { trace_indent_depth--; const int int_test = e; fprintf(TRACE_OUT_STREAM,"%s%s [end] ERR=%i\n",TRACE_INDENT,__func__,int_test); }}

#endif  /* TRACE_H */


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
