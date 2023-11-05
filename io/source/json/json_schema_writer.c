/* File: json_schema_writer.c; Copyright and License: see below */

#include "json/json_schema_writer.h"
#include "json/json_constants.h"
#include "utf8stringbuf/utf8string.h"
#include "u8/u8_trace.h"
//#include "u8/u8_log.h"
//#include <stdbool.h>
#include <assert.h>

/* IO_FILE_FORMAT_SCHEMA */

static const char SCHEMA_ALL[]
="{\n"
"  \"$schema\": \"https://json-schema.org/draft/2020-12/schema\",\n"
"  \"$id\": \"https://localhost/1d0f422c-9c68-4fe4-a295-87bc8e8d080d/crystal-facet-uml.schema.json\",\n"
"  \"title\": \"crystal-facet-uml model\",\n"
"  \"description\": \"defines the structure of json objects to store a uml/sysml model\",\n"
"  \"type\": \"object\",\n"
"  \"required\": [\"" JSON_CONSTANTS_KEY_HEAD "\"],\n"
"  \"properties\":\n"
"  {\n"
"    \"" JSON_CONSTANTS_KEY_HEAD "\":\n"
"    {\n"
"      \"description\": \"defines properties of the json file\",\n"
"      \"type\": \"object\"\n"
"    }\n"
"  }\n"
"}\n";

void json_schema_writer_init ( json_schema_writer_t *this_,
                               universal_output_stream_t *output )
{
    U8_TRACE_BEGIN();
    assert( NULL != output );

    (*this_).output = output;

    U8_TRACE_END();
}

void json_schema_writer_destroy( json_schema_writer_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).output = NULL;

    U8_TRACE_END();
}

u8_error_t json_schema_writer_write_schema( json_schema_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    utf8string_t text = SCHEMA_ALL;
    const size_t text_len = utf8string_get_length(text);
    export_err |= universal_output_stream_write( (*this_).output, text, text_len);

    U8_TRACE_END_ERR( export_err );
    return export_err;
}


/*
Copyright 2023-2023 Andreas Warnke

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
