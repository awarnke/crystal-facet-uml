/* File: json_schema_writer.c; Copyright and License: see below */

#include "json/json_schema_writer.h"
#include "json/json_constants.h"
#include "utf8stringbuf/utf8string.h"
#include "u8/u8_trace.h"
#include <assert.h>

static const char SCHEMA_ALL[]
="{\n"
"  \"$schema\": \"https://json-schema.org/draft/2020-12/schema\",\n"
"  \"$id\": \"https://localhost/1d0f422c-9c68-4fe4-a295-87bc8e8d080d/crystal-facet-uml.schema.json\",\n"
"  \"title\": \"crystal-facet-uml model\",\n"
"  \"description\": \"This schema defines the structure of json objects to store a uml/sysml model.\",\n"
"  \"type\": \"object\",\n"
"  \"required\": [\"" JSON_CONSTANTS_KEY_HEAD "\"],\n"
"  \"properties\":\n"
"  {\n"
"    \"" JSON_CONSTANTS_KEY_HEAD "\":\n"
"    {\n"
"      \"description\": \"" JSON_CONSTANTS_KEY_HEAD " defines properties of the json file.\",\n"
"      \"type\": \"object\"\n"
"    },\n"
"    \"" JSON_CONSTANTS_KEY_VIEWS "\":\n"
"    {\n"
"      \"description\": \"" JSON_CONSTANTS_KEY_VIEWS " is an array whose elements describe the views on the model.\",\n"
"      \"type\": \"array\",\n"
"      \"items\":\n"
"      {\n"
"        \"description\": \"An element of " JSON_CONSTANTS_KEY_VIEWS " is an object that specifies a "
JSON_CONSTANTS_KEY_DIAGRAM ".\",\n"
"        \"type\": \"object\",\n"
"        \"required\": [\"" JSON_CONSTANTS_KEY_DIAGRAM "\"],\n"
"        \"properties\":\n"
"        {\n"
"          \"" JSON_CONSTANTS_KEY_DIAGRAM "\":\n"
"          {\n"
"            \"description\": \"" JSON_CONSTANTS_KEY_DIAGRAM " is a view that is specified by listing the nodes to be shown in "
JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS ".\",\n"
"            \"type\": \"object\",\n"
"            \"properties\":\n"
"            {\n"
"              \"" JSON_CONSTANTS_KEY_DIAGRAM_ID "\":\n"
"              {\n"
"                \"description\": \"" JSON_CONSTANTS_KEY_DIAGRAM_ID " is a proposal for an id.\",\n"
"                \"type\": \"number\"\n"
"              },\n"
"              \"" JSON_CONSTANTS_KEY_DIAGRAM_NAME "\":\n"
"              {\n"
"                \"description\": \"" JSON_CONSTANTS_KEY_DIAGRAM_NAME " is the title of the diagram.\",\n"
"                \"type\": \"string\"\n"
"              },\n"
"              \"" JSON_CONSTANTS_KEY_DIAGRAM_DESCRIPTION "\":\n"
"              {\n"
"                \"description\": \"" JSON_CONSTANTS_KEY_DIAGRAM_DESCRIPTION " is the description of the diagram.\",\n"
"                \"type\": \"array\",\n"
"                \"items\":\n"
"                {\n"
"                  \"description\": \"A part of the description (to be concatenated).\",\n"
"                  \"type\": \"string\"\n"
"                }\n"
"              },\n"
"              \"" JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS "\":\n"
"              {\n"
"                \"description\": \"" JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS " is the list of elements shown in the diagram.\",\n"
"                \"type\": \"array\",\n"
"                \"items\":\n"
"                {\n"
"                  \"description\": \"An element of " JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS " specifies what and how to show.\",\n"
"                  \"type\": \"object\"\n"
"                }\n"
"              }\n"
"            }\n"
"          }\n"
"        }\n"
"      }\n"
"    },\n"
"    \"" JSON_CONSTANTS_KEY_NODES "\":\n"
"    {\n"
"      \"description\": \"" JSON_CONSTANTS_KEY_NODES " is an array whose elements describe the nodes of the model-graph.\",\n"
"      \"type\": \"array\",\n"
"      \"items\":\n"
"      {\n"
"        \"description\": \"An element of " JSON_CONSTANTS_KEY_NODES " is an object that specifies a "
JSON_CONSTANTS_KEY_CLASSIFIER ".\",\n"
"        \"type\": \"object\",\n"
"        \"required\": [\"" JSON_CONSTANTS_KEY_CLASSIFIER "\"],\n"
"        \"properties\":\n"
"        {\n"
"          \"" JSON_CONSTANTS_KEY_CLASSIFIER "\":\n"
"          {\n"
"            \"description\": \"" JSON_CONSTANTS_KEY_CLASSIFIER " is a node that contains a list of subnodes called " JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES ".\",\n"
"            \"type\": \"object\",\n"
"            \"properties\":\n"
"            {\n"
"              \"" JSON_CONSTANTS_KEY_CLASSIFIER_ID "\":\n"
"              {\n"
"                \"description\": \"" JSON_CONSTANTS_KEY_CLASSIFIER_ID " is a proposal for an id.\",\n"
"                \"type\": \"number\"\n"
"              },\n"
"              \"" JSON_CONSTANTS_KEY_CLASSIFIER_NAME "\":\n"
"              {\n"
"                \"description\": \"" JSON_CONSTANTS_KEY_CLASSIFIER_NAME " is the title of the classifier.\",\n"
"                \"type\": \"string\"\n"
"              },\n"
"              \"" JSON_CONSTANTS_KEY_CLASSIFIER_DESCRIPTION "\":\n"
"              {\n"
"                \"description\": \"" JSON_CONSTANTS_KEY_CLASSIFIER_DESCRIPTION " is the description of the classifier.\",\n"
"                \"type\": \"array\",\n"
"                \"items\":\n"
"                {\n"
"                  \"description\": \"A part of the description (to be concatenated).\",\n"
"                  \"type\": \"string\"\n"
"                }\n"
"              },\n"
"              \"" JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES "\":\n"
"              {\n"
"                \"description\": \"" JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES " is the list of features of the classifier.\",\n"
"                \"type\": \"array\",\n"
"                \"items\":\n"
"                {\n"
"                  \"description\": \"An element of " JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES " specifies a feature or port or lifeline.\",\n"
"                  \"type\": \"object\"\n"
"                }\n"
"              }\n"
"            }\n"
"          }\n"
"        }\n"
"      }\n"
"    },\n"
"    \"" JSON_CONSTANTS_KEY_EDGES "\":\n"
"    {\n"
"      \"description\": \"" JSON_CONSTANTS_KEY_EDGES " is an array whose elements describe the edges of the model-graph.\",\n"
"      \"type\": \"array\",\n"
"      \"items\":\n"
"      {\n"
"        \"description\": \"An element of " JSON_CONSTANTS_KEY_EDGES " is an object that specifies a "
JSON_CONSTANTS_KEY_RELATIONSHIP ".\",\n"
"        \"type\": \"object\",\n"
"        \"required\": [\"" JSON_CONSTANTS_KEY_RELATIONSHIP "\"],\n"
"        \"properties\":\n"
"        {\n"
"          \"" JSON_CONSTANTS_KEY_RELATIONSHIP "\":\n"
"          {\n"
"            \"description\": \"" JSON_CONSTANTS_KEY_RELATIONSHIP " is an edge with 1 source and 1 destination end.\",\n"
"            \"type\": \"object\",\n"
"            \"properties\":\n"
"            {\n"
"              \"" JSON_CONSTANTS_KEY_RELATIONSHIP_ID "\":\n"
"              {\n"
"                \"description\": \"" JSON_CONSTANTS_KEY_RELATIONSHIP_ID " is a proposal for an id.\",\n"
"                \"type\": \"number\"\n"
"              },\n"
"              \"" JSON_CONSTANTS_KEY_RELATIONSHIP_NAME "\":\n"
"              {\n"
"                \"description\": \"" JSON_CONSTANTS_KEY_RELATIONSHIP_NAME " is the name of the relationship.\",\n"
"                \"type\": \"string\"\n"
"              },\n"
"              \"" JSON_CONSTANTS_KEY_RELATIONSHIP_DESCRIPTION "\":\n"
"              {\n"
"                \"description\": \"" JSON_CONSTANTS_KEY_RELATIONSHIP_DESCRIPTION " is the description of the relationship.\",\n"
"                \"type\": \"array\",\n"
"                \"items\":\n"
"                {\n"
"                  \"description\": \"A part of the description (to be concatenated).\",\n"
"                  \"type\": \"string\"\n"
"                }\n"
"              }\n"
"            }\n"
"          }\n"
"        }\n"
"      }\n"
"    }\n"
"  }\n"
"}\n";

void json_schema_writer_init ( json_schema_writer_t *this_,
                               universal_output_stream_t *output )
{
    U8_TRACE_BEGIN();
    assert( NULL != output );

    utf8stream_writer_init( &((*this_).writer), output );

    U8_TRACE_END();
}

void json_schema_writer_destroy( json_schema_writer_t *this_ )
{
    U8_TRACE_BEGIN();

    utf8stream_writer_destroy( &((*this_).writer) );

    U8_TRACE_END();
}

u8_error_t json_schema_writer_write_schema( json_schema_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_ALL );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t json_schema_writer_declare_number( json_schema_writer_t *this_,
                                              const char* indent,
                                              const char* name,
                                              const char* description )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t json_schema_writer_declare_string( json_schema_writer_t *this_,
                                              const char* indent,
                                              const char* name,
                                              const char* description )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

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
