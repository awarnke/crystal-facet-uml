/* File: trace.h; Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

#ifndef TRACE_H
#define TRACE_H

#include <stdio.h>

#define TRACE_OUT_STREAM stdout
#define TRACE_INDENT_MAX (16)
extern __thread int trace_indent_depth;
extern const char trace_indent_pattern[2*(TRACE_INDENT_MAX-1)+1];
#define TRACE_INDENT (&(trace_indent_pattern[((16*TRACE_INDENT_MAX-1-trace_indent_depth)%TRACE_INDENT_MAX)*2]))

#define TRACE_INFO(x) { const char *string_test = x; fprintf(TRACE_OUT_STREAM,"%s%s\n",TRACE_INDENT,string_test); }
#define TRACE_INFO_INT(x,i) { const char *string_test = x; const int int_test = i; fprintf(TRACE_OUT_STREAM,"%s%s %i\n",TRACE_INDENT,string_test,int_test); }
#define TRACE_INFO_INT_INT(x,i,j) { const char *string_test = x; const int int_test = i; const int int_test2 = j; fprintf(TRACE_OUT_STREAM,"%s%s %i %i\n",TRACE_INDENT,string_test,int_test,int_test2); }
#define TRACE_BEGIN() { fprintf(TRACE_OUT_STREAM,"%s%s [begin]\n",TRACE_INDENT,__func__); trace_indent_depth++; }
#define TRACE_END() { trace_indent_depth--; fprintf(TRACE_OUT_STREAM,"%s%s [end]\n",TRACE_INDENT,__func__); }
#define TRACE_END_ERR(e) { if ( e==0 ) {TRACE_END();} else { trace_indent_depth--; const int int_test = e; fprintf(TRACE_OUT_STREAM,"%s%s [end] ERR=%i\n",TRACE_INDENT,__func__,int_test); }}

#endif  /* TRACE_H */

