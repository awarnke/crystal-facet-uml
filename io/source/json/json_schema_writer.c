/* File: json_schema_writer.c; Copyright and License: see below */

#include "json/json_schema_writer.h"
#include "json/json_constants.h"
#include "utf8stringbuf/utf8string.h"
#include "u8/u8_trace.h"
#include <assert.h>

static const char SCHEMA_GRAPH_HEADER[]
=
"{\n"
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
;

static const char SCHEMA_GRAPH_VIEWS[]
=
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
;

static const char SCHEMA_DIAGRAM_HEADER[]
=
"          \"" JSON_CONSTANTS_KEY_DIAGRAM "\":\n"
"          {\n"
"            \"description\": \"" JSON_CONSTANTS_KEY_DIAGRAM " is a view that is specified by listing the nodes to be shown in "
JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS ".\",\n"
"            \"type\": \"object\",\n"
"            \"properties\":\n"
"            {\n"
;

static const char SCHEMA_DIAGRAM_ELEMENTS_HEADER[]
=
"              \"" JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS "\":\n"
"              {\n"
"                \"description\": \"" JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS " is the list of elements shown in the diagram.\",\n"
"                \"type\": \"array\",\n"
"                \"items\":\n"
"                {\n"
"                  \"description\": \"An element of " JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS " specifies what and how to show.\",\n"
"                  \"type\": \"object\",\n"
"                  \"properties\":\n"
"                  {\n"
;

static const char SCHEMA_DIAGRAM_ELEMENTS_FOOTER[]
=
"                  },\n"
"                  \"additionalProperties\": false\n"
"                },\n"
"              }\n"
;

static const char SCHEMA_DIAGRAM_FOOTER[]
=
"            },\n"
"            \"additionalProperties\": false\n"
"          }\n"
;

static const char SCHEMA_GRAPH_NODES[]
=
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
;

static const char SCHEMA_CLASSIFIER_HEADER[]
=
"          \"" JSON_CONSTANTS_KEY_CLASSIFIER "\":\n"
"          {\n"
"            \"description\": \"" JSON_CONSTANTS_KEY_CLASSIFIER " is a node that contains a list of subnodes called " JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES ".\",\n"
"            \"type\": \"object\",\n"
"            \"properties\":\n"
"            {\n"
;

static const char SCHEMA_FEATURES_HEADER[]
=
"              \"" JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES "\":\n"
"              {\n"
"                \"description\": \"" JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES " is the list of features of the classifier.\",\n"
"                \"type\": \"array\",\n"
"                \"items\":\n"
"                {\n"
"                  \"description\": \"An element of " JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES " specifies a feature or port or lifeline.\",\n"
"                  \"type\": \"object\",\n"
"                  \"properties\":\n"
"                  {\n"
;

static const char SCHEMA_FEATURES_FOOTER[]
=
"                  },\n"
"                  \"additionalProperties\": false\n"
"                }\n"
"              }\n"
;

static const char SCHEMA_CLASSIFIER_FOOTER[]
=
"            },\n"
"            \"additionalProperties\": false\n"
"          }\n"
;

static const char SCHEMA_GRAPH_EDGES[]
=
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
;

static const char SCHEMA_RELATIONSHIP_HEADER[]
=
"          \"" JSON_CONSTANTS_KEY_RELATIONSHIP "\":\n"
"          {\n"
"            \"description\": \"" JSON_CONSTANTS_KEY_RELATIONSHIP " is an edge with 1 source and 1 destination end.\",\n"
"            \"type\": \"object\",\n"
"            \"properties\":\n"
"            {\n"
;

static const char SCHEMA_RELATIONSHIP_FOOTER[]
=
"            },\n"
"            \"additionalProperties\": false\n"
"          }\n"
;

static const char SCHEMA_GRAPH_FOOTER[]
=
"        }\n"
"      }\n"
"    }\n"
"  }\n"
"}\n"
;

static const char INDENT_7[] = "              ";
static const char INDENT_10[] = "                    ";

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

    static const char *const D_ID = "id is a proposal, may be changed automatically";
    static const char *const D_UUID = "uuid is an identifier, all letters must be lowercase";
    static const char *const D_STEREO = "stereotype is invalid before 1.43.0, then ignored, used in 1.47.0";
    static const char *const D_DESCR = "A part of the description (to be concatenated).";

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_HEADER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_VIEWS );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_DIAGRAM_HEADER );

    export_err |= json_schema_writer_private_declare_number( this_, 7, JSON_CONSTANTS_KEY_DIAGRAM_ID, D_ID, true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_DIAGRAM_PARENT_NAME, "", true );
    static const char *const D_PARENT_ID = "uuid of the parent diagram, optional";
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_DIAGRAM_PARENT, D_PARENT_ID, true );
    export_err |= json_schema_writer_private_declare_number( this_, 7, JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE_NAME, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_DIAGRAM_STEREOTYPE, D_STEREO, true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_DIAGRAM_NAME, "", true );
    export_err |= json_schema_writer_private_declare_array_of_string( this_, 7, JSON_CONSTANTS_KEY_DIAGRAM_DESCRIPTION, "", D_DESCR, true );
    export_err |= json_schema_writer_private_declare_number( this_, 7, JSON_CONSTANTS_KEY_DIAGRAM_LIST_ORDER, "", true );
    export_err |= json_schema_writer_private_declare_number( this_, 7, JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAGS, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAG_NAMES, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_UUID, D_UUID, true /*elements follow*/ );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_DIAGRAM_ELEMENTS_HEADER );

    export_err |= json_schema_writer_private_declare_number( this_, 10, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_ID, D_ID, true );
    export_err |= json_schema_writer_private_declare_string( this_, 10, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_CLASSIFIER_NAME, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 10, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_FOCUSED_FEATURE_NAME, "", true );
    export_err |= json_schema_writer_private_declare_number( this_, 10, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAGS, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 10, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAG_NAMES, "", true );
    static const char *const D_NODE = "the uuid of either the classifier or the feature (type lifeline only)";
    export_err |= json_schema_writer_private_declare_string( this_, 10, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_NODE, D_NODE, true );
    export_err |= json_schema_writer_private_declare_string( this_, 10, JSON_CONSTANTS_KEY_UUID, D_UUID, false );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_DIAGRAM_ELEMENTS_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_DIAGRAM_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_NODES );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_CLASSIFIER_HEADER );

    export_err |= json_schema_writer_private_declare_number( this_, 7, JSON_CONSTANTS_KEY_CLASSIFIER_ID, D_ID, true );
    export_err |= json_schema_writer_private_declare_number( this_, 7, JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE_NAME, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_CLASSIFIER_STEREOTYPE, D_STEREO, true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_CLASSIFIER_NAME, "", true );
    export_err |= json_schema_writer_private_declare_array_of_string( this_, 7, JSON_CONSTANTS_KEY_CLASSIFIER_DESCRIPTION, "", D_DESCR, true );
    export_err |= json_schema_writer_private_declare_number( this_, 7, JSON_CONSTANTS_KEY_CLASSIFIER_X_ORDER, "", true );
    export_err |= json_schema_writer_private_declare_number( this_, 7, JSON_CONSTANTS_KEY_CLASSIFIER_Y_ORDER, "", true );
    export_err |= json_schema_writer_private_declare_number( this_, 7, JSON_CONSTANTS_KEY_CLASSIFIER_LIST_ORDER, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_UUID, D_UUID, true /*features follow*/ );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_FEATURES_HEADER );

    export_err |= json_schema_writer_private_declare_number( this_, 10, JSON_CONSTANTS_KEY_FEATURE_ID, D_ID, true );
    export_err |= json_schema_writer_private_declare_number( this_, 10, JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 10, JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE_NAME, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 10, JSON_CONSTANTS_KEY_FEATURE_KEY, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 10, JSON_CONSTANTS_KEY_FEATURE_VALUE, "", true );
    export_err |= json_schema_writer_private_declare_array_of_string( this_, 10, JSON_CONSTANTS_KEY_FEATURE_DESCRIPTION, "", D_DESCR, true );
    export_err |= json_schema_writer_private_declare_number( this_, 10, JSON_CONSTANTS_KEY_FEATURE_LIST_ORDER, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 10, JSON_CONSTANTS_KEY_UUID, D_UUID, false );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_FEATURES_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_CLASSIFIER_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_EDGES );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_RELATIONSHIP_HEADER );

    export_err |= json_schema_writer_private_declare_number( this_, 7, JSON_CONSTANTS_KEY_RELATIONSHIP_ID, D_ID, true );
    export_err |= json_schema_writer_private_declare_number( this_, 7, JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE_NAME, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_RELATIONSHIP_STEREOTYPE, D_STEREO, true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_RELATIONSHIP_NAME, "", true );
    export_err |= json_schema_writer_private_declare_array_of_string( this_, 7, JSON_CONSTANTS_KEY_RELATIONSHIP_DESCRIPTION, "", D_DESCR, true );
    export_err |= json_schema_writer_private_declare_number( this_, 7, JSON_CONSTANTS_KEY_RELATIONSHIP_LIST_ORDER, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_NAME, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_FEATURE_KEY, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_NODE, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_NAME, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_RELATIONSHIP_TO_FEATURE_KEY, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_RELATIONSHIP_TO_NODE, "", true );
    export_err |= json_schema_writer_private_declare_string( this_, 7, JSON_CONSTANTS_KEY_UUID, D_UUID, false );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_RELATIONSHIP_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_FOOTER );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t json_schema_writer_private_declare_number( json_schema_writer_t *this_,
                                                      unsigned int indent,
                                                      const char* name,
                                                      const char* description,
                                                      bool has_next )
{
    U8_TRACE_BEGIN();
    assert( name != NULL );
    assert( description != NULL );
    u8_error_t export_err = U8_ERROR_NONE;

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), name );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\":\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "{\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"description\": \"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), description );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\",\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"type\": \"number\"\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), has_next ? "},\n" : "}\n" );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t json_schema_writer_private_declare_string( json_schema_writer_t *this_,
                                                      unsigned int indent,
                                                      const char* name,
                                                      const char* description,
                                                      bool has_next )
{
    U8_TRACE_BEGIN();
    assert( name != NULL );
    assert( description != NULL );
    u8_error_t export_err = U8_ERROR_NONE;

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), name );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\":\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "{\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"description\": \"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), description );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\",\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"type\": \"string\"\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), has_next ? "},\n" : "}\n" );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t json_schema_writer_private_declare_array_of_string( json_schema_writer_t *this_,
                                                               unsigned int indent,
                                                               const char* name,
                                                               const char* description,
                                                               const char* element_description,
                                                               bool has_next )
{
    U8_TRACE_BEGIN();
    assert( name != NULL );
    assert( description != NULL );
    assert( element_description != NULL );
    u8_error_t export_err = U8_ERROR_NONE;

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7  );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), name );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\":\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "{\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"description\": \"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), description );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\",\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"type\": \"array\",\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"items\":\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  {\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "    \"description\": \"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), element_description );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\",\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "    \"type\": \"string\"\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  }\n" );


    export_err |= utf8stream_writer_write_str( &((*this_).writer), (indent==10) ? INDENT_10 : INDENT_7 );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), has_next ? "},\n" : "}\n" );

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
