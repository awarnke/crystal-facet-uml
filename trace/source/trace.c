/* File: trace.c; Copyright and License: see below */

#include "trace.h"
#include <stdio.h>
#include <time.h>

__thread int trace_indent_depth = 0;

const char trace_indent_pattern_begin[2*(TRACE_INDENT_MAX-1)+1] = "| | | | | | | | | | | | | | | | | | | | | | + \0";
const char trace_indent_pattern_end[2*(TRACE_INDENT_MAX-1)+1] =   "| | | | | | | | | | | | | | | | | | | | | | \' \0";
const char trace_indent_pattern_info[2*(TRACE_INDENT_MAX-1)+1] =  "| | | | | | | | | | | | | | | | | | | | | |   \0";


/*
Copyright 2016-2019 Andreas Warnke

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
