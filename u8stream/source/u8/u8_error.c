/* File: u8_error.c; Copyright and License: see below */

#include "u8/u8_error.h"

const char * u8_error_get_name( u8_error_t this_ )
{
    const char * result = NULL;
    switch ( this_ )
    {
        case U8_ERROR_NONE: { result = "U8_ERROR_NONE"; }; break;
        case U8_ERROR_DUPLICATE: { result = "U8_ERROR_DUPLICATE"; }; break;
        case U8_ERROR_DUPLICATE_ID: { result = "U8_ERROR_DUPLICATE_ID"; }; break;
        case U8_ERROR_DUPLICATE_NAME: { result = "U8_ERROR_DUPLICATE_NAME"; }; break;
        case U8_ERROR_DUPLICATE_UUID: { result = "U8_ERROR_DUPLICATE_UUID"; }; break;
        case U8_ERROR_EDGE_CASE_PARAM: { result = "U8_ERROR_EDGE_CASE_PARAM"; }; break;
        case U8_ERROR_NOT_FOUND: { result = "U8_ERROR_NOT_FOUND"; }; break;
        case U8_ERROR_END_OF_STREAM: { result = "U8_ERROR_END_OF_STREAM"; }; break;
        case U8_ERROR_FILE_ALREADY_REMOVED: { result = "U8_ERROR_FILE_ALREADY_REMOVED"; }; break;
        case U8_ERROR_CONFIG_OUT_OF_RANGE: { result = "U8_ERROR_CONFIG_OUT_OF_RANGE"; }; break;
        case U8_ERROR_WRONG_STATE: { result = "U8_ERROR_WRONG_STATE"; }; break;
        case U8_ERROR_NOT_YET_IMPLEMENTED: { result = "U8_ERROR_NOT_YET_IMPLEMENTED"; }; break;
        case U8_ERROR_PARAM_MISSING: { result = "U8_ERROR_PARAM_MISSING"; }; break;
        case U8_ERROR_PARAM_OUT_OF_RANGE: { result = "U8_ERROR_PARAM_OUT_OF_RANGE"; }; break;
        case U8_ERROR_AT_MUTEX: { result = "U8_ERROR_AT_MUTEX"; }; break;
        case U8_ERROR_DB_STRUCTURE: { result = "U8_ERROR_DB_STRUCTURE"; }; break;
        case U8_ERROR_AT_DB: { result = "U8_ERROR_AT_DB"; }; break;
        case U8_ERROR_LIB_NO_MEMORY: { result = "U8_ERROR_LIB_NO_MEMORY"; }; break;
        case U8_ERROR_LIB_FILE_WRITE: { result = "U8_ERROR_LIB_FILE_WRITE"; }; break;
        case U8_ERROR_NO_DB: { result = "U8_ERROR_NO_DB"; }; break;
        case U8_ERROR_OBJECT_STILL_REFERENCED: { result = "U8_ERROR_OBJECT_STILL_REFERENCED"; }; break;
        case U8_ERROR_DIAGRAM_HIDES_RELATIONSHIPS: { result = "U8_ERROR_DIAGRAM_HIDES_RELATIONSHIPS"; }; break;
        case U8_ERROR_DIAGRAM_HIDES_FEATURES: { result = "U8_ERROR_DIAGRAM_HIDES_FEATURES"; }; break;
        case U8_ERROR_FOCUS_EMPTY: { result = "U8_ERROR_FOCUS_EMPTY"; }; break;
        case U8_ERROR_STRING_BUFFER_EXCEEDED: { result = "U8_ERROR_STRING_BUFFER_EXCEEDED"; }; break;
        case U8_ERROR_ARRAY_BUFFER_EXCEEDED: { result = "U8_ERROR_ARRAY_BUFFER_EXCEEDED"; }; break;
        case U8_ERROR_INPUT_EMPTY: { result = "U8_ERROR_INPUT_EMPTY"; }; break;
        case U8_ERROR_INVALID_REQUEST: { result = "U8_ERROR_INVALID_REQUEST"; }; break;
        case U8_ERROR_VALUE_OUT_OF_RANGE: { result = "U8_ERROR_VALUE_OUT_OF_RANGE"; }; break;
        case U8_ERROR_INVALID_ENCODING: { result = "U8_ERROR_INVALID_ENCODING"; }; break;
        case U8_ERROR_LEXICAL_STRUCTURE: { result = "U8_ERROR_LEXICAL_STRUCTURE"; }; break;
        case U8_ERROR_PARSER_STRUCTURE: { result = "U8_ERROR_PARSER_STRUCTURE"; }; break;
        case U8_ERROR_AT_FILE_READ: { result = "U8_ERROR_AT_FILE_READ"; }; break;
        case U8_ERROR_AT_FILE_WRITE: { result = "U8_ERROR_AT_FILE_WRITE"; }; break;
        case U8_ERROR_READ_ONLY_DB: { result = "U8_ERROR_READ_ONLY_DB"; }; break;
        default: { result = "U8_ERROR_<multiple>"; }; break;
    }
    assert( result != NULL );
    return result;
}


/*
Copyright 2026-2026 Andreas Warnke

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
