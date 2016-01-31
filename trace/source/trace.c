/* File: util/trace.c; Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

#include "util/trace.h"
#include <stdio.h>
#include <time.h>

__thread int trace_indent_depth = 0;
const char trace_indent_pattern[2*(TRACE_INDENT_MAX-1)+1] = ": : : : : : : : : : : : : : : \0";