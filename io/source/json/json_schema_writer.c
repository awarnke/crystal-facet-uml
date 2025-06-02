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

    json_type_name_map_init( &((*this_).enum_map) );
    utf8stream_writer_init( &((*this_).writer), output );
    (*this_).indent = 7;

    U8_TRACE_END();
}

void json_schema_writer_destroy( json_schema_writer_t *this_ )
{
    U8_TRACE_BEGIN();

    utf8stream_writer_destroy( &((*this_).writer) );
    json_type_name_map_destroy( &((*this_).enum_map) );

    U8_TRACE_END();
}

u8_error_t json_schema_writer_write_schema( json_schema_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    static const char *const ANY_ID = "id is a proposal, may be changed automatically";
    static const char *const ANY_UUID = "uuid is an identifier, all letters must be lowercase";
    static const char *const ANY_STEREO = "stereotype is available since version 1.47.0";
    static const char *const ANY_DESCR = "a part of the description (to be concatenated)";
    static const char *const ANY_ORDER
        = "a value between –2147483648 and 2147483647 by which to order elements in 1 or 2 dimensions";

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_HEADER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_VIEWS );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_DIAGRAM_HEADER );

    (*this_).indent = 7;
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_DIAGRAM_ID, ANY_ID );
    static const char *const D_PARENT_NAME
        = "name of " JSON_CONSTANTS_KEY_DIAGRAM_PARENT " diagram, exported for reviews by humans, ignored at import";
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_DIAGRAM_PARENT_NAME, D_PARENT_NAME );
    static const char *const D_PARENT_ID = "uuid of the parent diagram, omitted by the root diagram";
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_DIAGRAM_PARENT, D_PARENT_ID, true );
    static const char *const D_DIAGTYPE_ID = "id of diagram type, see source file data/include/entity/data_diagram_type.h";
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE, D_DIAGTYPE_ID );
    static const char *const D_DIAGTYPE_NAME
        = JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE " as string, exported for reviews by humans, ignored at import";
    /* generate an enum listing all options */
    const char *const * D_TYPE_VALUES = json_type_name_map_get_diagram_types_list( &((*this_).enum_map) );
    export_err |= json_schema_writer_private_declare_enum( this_,
                                                           JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE_NAME,
                                                           D_DIAGTYPE_NAME,
                                                           D_TYPE_VALUES
                                                         );
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_DIAGRAM_STEREOTYPE, ANY_STEREO );
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_DIAGRAM_NAME, "" );
    export_err |= json_schema_writer_private_declare_array_of_string( this_, JSON_CONSTANTS_KEY_DIAGRAM_DESCRIPTION, "", ANY_DESCR );
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_DIAGRAM_LIST_ORDER, ANY_ORDER );
    static const char *const D_FLAGS = "set of flags, see source file data/include/entity/data_diagram_flag.h";
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAGS, D_FLAGS );
    static const char *const D_FLAG_NAME
        = JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAGS " as string, exported for reviews by humans, ignored at import";
    /* generate an enum listing all options */
    const char *const * D_FLAG_VALUES = json_type_name_map_get_diagram_tags_list( &((*this_).enum_map) );
    export_err |= json_schema_writer_private_declare_enum( this_,
                                                           JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAG_NAMES,
                                                           D_FLAG_NAME,
                                                           D_FLAG_VALUES
                                                         );
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_UUID, ANY_UUID, true /*elements follow*/ );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_DIAGRAM_ELEMENTS_HEADER );

    (*this_).indent = 10;
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_ID, ANY_ID );
    static const char *const E_NODE_C_NAME
        = "name of " JSON_CONSTANTS_KEY_DIAGRAMELEMENT_NODE "/classifier, exported for reviews by humans, ignored at import";
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_CLASSIFIER_NAME, E_NODE_C_NAME );
    static const char *const E_NODE_F_NAME
        = "name of " JSON_CONSTANTS_KEY_DIAGRAMELEMENT_NODE "/feature (if lifeline), optional, exported for reviews by humans, ignored at import";
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_FOCUSED_FEATURE_NAME, E_NODE_F_NAME );
    static const char *const E_FLAGS = "set of flags, see source file data/include/entity/data_diagramelement_flag.h";
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAGS, E_FLAGS );
    static const char *const E_FLAG_NAME
        = JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAGS " as string, exported for reviews by humans, ignored at import";
    /* generate an enum listing all options */
    const char *const * E_FLAG_VALUES = json_type_name_map_get_diagramelement_tags_list( &((*this_).enum_map) );
    export_err |= json_schema_writer_private_declare_enum( this_,
                                                           JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAG_NAMES,
                                                           E_FLAG_NAME,
                                                           E_FLAG_VALUES
                                                         );
    //export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAG_NAMES, E_FLAG_NAME );
    static const char *const E_NODE = "the uuid of either the classifier or the feature (type lifeline only)";
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_NODE, E_NODE, true );
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_UUID, ANY_UUID, false );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_DIAGRAM_ELEMENTS_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_DIAGRAM_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_NODES );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_CLASSIFIER_HEADER );

    (*this_).indent = 7;
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_CLASSIFIER_ID, ANY_ID );
    static const char *const C_TYPE_ID = "id of classifier type, see source file data/include/entity/data_classifier_type.h";
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE, C_TYPE_ID );
    static const char *const C_TYPE_NAME
        = "name of " JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE ", exported for reviews by humans, ignored at import";
    /* generate an enum listing all options */
    const char *const * C_TYPE_VALUES = json_type_name_map_get_classifier_types_list( &((*this_).enum_map) );
    export_err |= json_schema_writer_private_declare_enum( this_,
                                                           JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE_NAME,
                                                           C_TYPE_NAME,
                                                           C_TYPE_VALUES
                                                         );
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_CLASSIFIER_STEREOTYPE, "" );
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_CLASSIFIER_NAME, "" );
    export_err |= json_schema_writer_private_declare_array_of_string( this_, JSON_CONSTANTS_KEY_CLASSIFIER_DESCRIPTION, "", ANY_DESCR );
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_CLASSIFIER_X_ORDER, ANY_ORDER );
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_CLASSIFIER_Y_ORDER, ANY_ORDER );
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_CLASSIFIER_LIST_ORDER, ANY_ORDER );
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_UUID, ANY_UUID, true /*features follow*/ );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_FEATURES_HEADER );

    (*this_).indent = 10;
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_FEATURE_ID, ANY_ID );
    static const char *const F_TYPE_ID = "id of feature type, see source file data/include/entity/data_feature_type.h";
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE, F_TYPE_ID );
    static const char *const F_TYPE_NAME
        = "name of " JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE ", exported for reviews by humans, ignored at import";
    /* generate an enum listing all options */
    const char *const * F_TYPE_VALUES = json_type_name_map_get_feature_types_list( &((*this_).enum_map) );
    export_err |= json_schema_writer_private_declare_enum( this_,
                                                           JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE_NAME,
                                                           F_TYPE_NAME,
                                                           F_TYPE_VALUES
                                                         );
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_FEATURE_KEY, "name" );
    static const char *const F_VALUE = "valuetype in case of properties, value in case of tagges values, stereotype otherwise";
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_FEATURE_VALUE, F_VALUE );
    export_err |= json_schema_writer_private_declare_array_of_string( this_, JSON_CONSTANTS_KEY_FEATURE_DESCRIPTION, "", ANY_DESCR );
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_FEATURE_LIST_ORDER, ANY_ORDER );
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_UUID, ANY_UUID, false );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_FEATURES_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_CLASSIFIER_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_EDGES );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_RELATIONSHIP_HEADER );

    (*this_).indent = 7;
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_ID, ANY_ID );
    static const char *const R_TYPE_ID = "id of relationship type, see source file data/include/entity/data_relationship_type.h";
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE, R_TYPE_ID );
    static const char *const R_TYPE_NAME
        = "name of " JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE ", exported for reviews by humans, ignored at import";
    /* generate an enum listing all options */
    const char *const * R_TYPE_VALUES = json_type_name_map_get_relationship_types_list( &((*this_).enum_map) );
    export_err |= json_schema_writer_private_declare_enum( this_,
                                                           JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE_NAME,
                                                           R_TYPE_NAME,
                                                           R_TYPE_VALUES
                                                         );
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_STEREOTYPE, ANY_STEREO );
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_NAME, "" );
    export_err |= json_schema_writer_private_declare_array_of_string( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_DESCRIPTION, "", ANY_DESCR );
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_LIST_ORDER, ANY_ORDER );
    static const char *const E_FROM_NODE_C_NAME
        = "name of " JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_NODE "/classifier, exported for reviews by humans, ignored at import";
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_NAME, E_FROM_NODE_C_NAME );
    static const char *const E_FROM_NODE_F_NAME
        = "key of " JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_NODE "/feature if applicable, exported for reviews by humans, ignored at import";
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_FEATURE_KEY, E_FROM_NODE_F_NAME );
    static const char *const E_FROM_NODE = "the uuid of the source: either the classifier or the feature";
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_NODE, E_FROM_NODE, true );
    static const char *const E_TO_NODE_C_NAME
        = "name of " JSON_CONSTANTS_KEY_RELATIONSHIP_TO_NODE "/classifier, exported for reviews by humans, ignored at import";
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_NAME, E_TO_NODE_C_NAME );
    static const char *const E_TO_NODE_F_NAME
        = "key of " JSON_CONSTANTS_KEY_RELATIONSHIP_TO_NODE "/feature if applicable, exported for reviews by humans, ignored at import";
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_TO_FEATURE_KEY, E_TO_NODE_F_NAME );
    static const char *const E_TO_NODE = "the uuid of the destination: either the classifier or the feature";
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_TO_NODE, E_TO_NODE, true );
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_UUID, ANY_UUID, false );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_RELATIONSHIP_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_FOOTER );

    export_err |= utf8stream_writer_flush( &((*this_).writer) );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t json_schema_writer_private_declare_integer( json_schema_writer_t *this_,
                                                       const char* name,
                                                       const char* description )
{
    U8_TRACE_BEGIN();
    assert( name != NULL );
    assert( description != NULL );
    u8_error_t export_err = U8_ERROR_NONE;
    const char *const indent = ((*this_).indent==10) ? INDENT_10 : INDENT_7;

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), name );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\":\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "{\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"description\": \"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), description );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\",\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"type\": \"number\"\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "},\n" );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t json_schema_writer_private_declare_string( json_schema_writer_t *this_,
                                                      const char* name,
                                                      const char* description )
{
    U8_TRACE_BEGIN();
    assert( name != NULL );
    assert( description != NULL );
    u8_error_t export_err = U8_ERROR_NONE;
    const char *const indent = ((*this_).indent==10) ? INDENT_10 : INDENT_7;

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), name );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\":\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "{\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"description\": \"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), description );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\",\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"type\": \"string\"\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "},\n" );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t json_schema_writer_private_declare_array_of_string( json_schema_writer_t *this_,
                                                               const char* name,
                                                               const char* description,
                                                               const char* element_description )
{
    U8_TRACE_BEGIN();
    assert( name != NULL );
    assert( description != NULL );
    assert( element_description != NULL );
    u8_error_t export_err = U8_ERROR_NONE;
    const char *const indent = ((*this_).indent==10) ? INDENT_10 : INDENT_7;

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent  );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), name );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\":\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "{\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"description\": \"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), description );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\",\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"type\": \"array\",\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"items\":\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  {\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "    \"description\": \"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), element_description );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\",\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "    \"type\": \"string\"\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  }\n" );


    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "},\n");

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t json_schema_writer_private_declare_enum( json_schema_writer_t *this_,
                                                    const char* name,
                                                    const char* description,
                                                    const char *const * value_list )
{
    U8_TRACE_BEGIN();
    assert( name != NULL );
    assert( description != NULL );
    assert( value_list != NULL );
    u8_error_t export_err = U8_ERROR_NONE;
    const char *const indent = ((*this_).indent==10) ? INDENT_10 : INDENT_7;

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), name );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\":\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "{\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"description\": \"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), description );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\",\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"enum\":\n" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  [\n" );

    unsigned int index = 0;
    while ( value_list[index] != NULL )
    {
        const bool has_next = ( value_list[ index + 1 ] != NULL );
        export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
        export_err |= utf8stream_writer_write_str( &((*this_).writer), "    \"" );
        export_err |= utf8stream_writer_write_str( &((*this_).writer), value_list[index] );
        export_err |= utf8stream_writer_write_str( &((*this_).writer), has_next ? "\",\n": "\"\n" );

        index ++;
    }

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  ]\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "},\n" );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t json_schema_writer_private_declare_uuid( json_schema_writer_t *this_,
                                                    const char* name,
                                                    const char* description,
                                                    bool has_next )
{
    U8_TRACE_BEGIN();
    assert( name != NULL );
    assert( description != NULL );
    u8_error_t export_err = U8_ERROR_NONE;
    const char *const indent = ((*this_).indent==10) ? INDENT_10 : INDENT_7;

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), name );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\":\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "{\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"description\": \"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), description );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\",\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"type\": \"string\"\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), has_next ? "},\n" : "}\n" );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}


/*
Copyright 2023-2025 Andreas Warnke

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
