/* File: u8_trace.c; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <stdio.h>
#include <time.h>

__thread unsigned int u8_trace_indent_depth = 0;

const char u8_trace_indent_pattern_begin[ U8_TRACE_INDENT_STEP * U8_TRACE_INDENT_MAX + U8_TRACE_NULLTERM_SIZE ]
    = "| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | + \0";
const char u8_trace_indent_pattern_end[ U8_TRACE_INDENT_STEP * U8_TRACE_INDENT_MAX + U8_TRACE_NULLTERM_SIZE ]
    = "| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | \' \0";
const char u8_trace_indent_pattern_info[ U8_TRACE_INDENT_STEP * U8_TRACE_INDENT_MAX  +U8_TRACE_NULLTERM_SIZE ]
    = "| | | | | | | | | | | | | | | | | | | | | | | | | | | | | |   \0";


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
