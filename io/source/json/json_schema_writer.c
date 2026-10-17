/* File: json_schema_writer.c; Copyright and License: see below */

#include "json/json_schema_writer.h"
#include "json/json_constants.h"
#include "entity/data_head.h"
#include "entity/data_classifier.h"
#include "entity/data_feature.h"
#include "entity/data_relationship.h"
#include "entity/data_diagram.h"
#include "entity/data_diagramelement_flag.h"
#include "entity/data_enum_iterator.h"
#include "utf8stringbuf/utf8string.h"
#include "u8/u8_trace.h"
#include <assert.h>

#define SCHEMA_STR_HELPER(x) #x
#define SCHEMA_STR(x) SCHEMA_STR_HELPER(x)

static const char SCHEMA_GRAPH_HEADER[]
    = "{\n"
    "  \"$schema\": \"https://json-schema.org/draft/2020-12/schema\",\n"
    "  \"$id\": \"https://localhost/1d0f422c-9c68-4fe4-a295-87bc8e8d080d/crystal-facet-uml.schema.json\",\n"
    "  \"title\": \"crystal-facet-uml model\",\n"
    "  \"description\": \"This schema defines the structure of json objects to store a uml/sysml model.\",\n"
    "  \"type\": \"object\",\n"
    "  \"required\": [\"" JSON_CONSTANTS_KEY_HEAD "\"],\n"
    "  \"properties\":\n"
    "  {\n";

static const char SCHEMA_GRAPH_HEAD_HEADER[]
    = "    \"" JSON_CONSTANTS_KEY_HEAD "\":\n"
    "    {\n"
    "      \"description\": \""
    JSON_CONSTANTS_KEY_HEAD " defines properties of the json file."
    "\",\n"
    "      \"type\": \"object\",\n"
    "      \"required\":\n"
    "      [\n"
    "        \"" JSON_CONSTANTS_KEY_HEAD_ENCODING "\",\n"
    "        \"" JSON_CONSTANTS_KEY_HEAD_STRUCTURE "\",\n"
    "        \"" JSON_CONSTANTS_KEY_HEAD_FORMAT "\",\n"
    "        \"" JSON_CONSTANTS_KEY_HEAD_MAJOR_VERSION "\",\n"
    "        \"" JSON_CONSTANTS_KEY_HEAD_MINOR_VERSION "\"\n"
    "      ],\n"
    "      \"properties\":\n"
    "      {\n" ;

static const char SCHEMA_GRAPH_HEAD_FOOTER[]
    = "      },\n"
    "      \"additionalProperties\": true\n"
    "    },\n";

static const char SCHEMA_GRAPH_VIEWS_HEADER[]
    = "    \"" JSON_CONSTANTS_KEY_VIEWS "\":\n"
    "    {\n"
    "      \"description\": \"" JSON_CONSTANTS_KEY_VIEWS " is an array whose elements describe the views on the model.\",\n"
    "      \"type\": \"array\",\n"
    "      \"items\":\n"
    "      {\n"
    "        \"description\": \""
    "An element of " JSON_CONSTANTS_KEY_VIEWS " is an object that specifies a "
    JSON_CONSTANTS_KEY_DIAGRAM "."
    "\",\n"
    "        \"type\": \"object\",\n"
    "        \"required\": [\"" JSON_CONSTANTS_KEY_DIAGRAM "\"],\n"
    "        \"properties\":\n"
    "        {\n" ;

static const char SCHEMA_DIAGRAM_HEADER[]
    = "          \"" JSON_CONSTANTS_KEY_DIAGRAM "\":\n"
    "          {\n"
    "            \"description\": \"" JSON_CONSTANTS_KEY_DIAGRAM " is a view that is specified by listing the nodes to be shown in "
#ifdef JSON_CONSTANTS_NEW_KEYS
    JSON_CONSTANTS_KEY_DIAGRAM_ELEMENT_LIST ".\",\n"
#else
    JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS ".\",\n"
#endif
    "            \"type\": \"object\",\n"
    "            \"properties\":\n"
    "            {\n";

static const char SCHEMA_DIAGRAM_ELEMENTS_HEADER[]
    =
#ifdef JSON_CONSTANTS_NEW_KEYS
    "              \"" JSON_CONSTANTS_KEY_DIAGRAM_ELEMENT_LIST "\":\n"
#else
    "              \"" JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS "\":\n"
#endif
    "              {\n"
#ifdef JSON_CONSTANTS_NEW_KEYS
    "                \"description\": \"" JSON_CONSTANTS_KEY_DIAGRAM_ELEMENT_LIST " is the list of elements shown in the diagram.\",\n"
#else
    "                \"description\": \"" JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS " is the list of elements shown in the diagram.\",\n"
#endif
    "                \"type\": \"array\",\n"
    "                \"items\":\n"
    "                {\n"
#ifdef JSON_CONSTANTS_NEW_KEYS
    "                  \"description\": \"An element of " JSON_CONSTANTS_KEY_DIAGRAM_ELEMENT_LIST " specifies what and how to show.\",\n"
#else
    "                  \"description\": \"An element of " JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS " specifies what and how to show.\",\n"
#endif
    "                  \"type\": \"object\",\n"
    "                  \"properties\":\n"
    "                  {\n";

static const char SCHEMA_DIAGRAM_ELEMENTS_FOOTER[]
=
"                  },\n"
"                  \"additionalProperties\": false\n"
"                }\n"
"              }\n"
;

static const char SCHEMA_DIAGRAM_FOOTER[]
=
"            },\n"
"            \"additionalProperties\": false\n"
"          }\n"
;

static const char SCHEMA_GRAPH_VIEWS_FOOTER[]
    = "        }\n"
    "      }\n"
    "    },\n";

static const char SCHEMA_GRAPH_NODES_HEADER[]
    = "    \"" JSON_CONSTANTS_KEY_NODES "\":\n"
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
    "        {\n";

static const char SCHEMA_CLASSIFIER_HEADER[]
    = "          \"" JSON_CONSTANTS_KEY_CLASSIFIER "\":\n"
    "          {\n"
    "            \"description\": \"" JSON_CONSTANTS_KEY_CLASSIFIER " is a node that contains a list of subnodes called " JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES ".\",\n"
    "            \"type\": \"object\",\n"
    "            \"properties\":\n"
    "            {\n";

static const char SCHEMA_FEATURES_HEADER[]
    = "              \"" JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES "\":\n"
    "              {\n"
    "                \"description\": \"" JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES " is the list of features of the classifier.\",\n"
    "                \"type\": \"array\",\n"
    "                \"items\":\n"
    "                {\n"
    "                  \"description\": \"An element of " JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES " specifies a feature or port or lifeline.\",\n"
    "                  \"type\": \"object\",\n"
    "                  \"properties\":\n"
    "                  {\n";

static const char SCHEMA_FEATURES_FOOTER[]
    = "                  },\n"
    "                  \"additionalProperties\": false\n"
    "                }\n"
    "              }\n";

static const char SCHEMA_CLASSIFIER_FOOTER[]
    = "            },\n"
    "            \"additionalProperties\": false\n"
    "          }\n";

static const char SCHEMA_GRAPH_NODES_FOOTER[]
    = "        }\n"
    "      }\n"
    "    },\n";

static const char SCHEMA_GRAPH_EDGES_HEADER[]
    = "    \"" JSON_CONSTANTS_KEY_EDGES "\":\n"
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
    "        {\n";

static const char SCHEMA_RELATIONSHIP_HEADER[]
    = "          \"" JSON_CONSTANTS_KEY_RELATIONSHIP "\":\n"
    "          {\n"
    "            \"description\": \"" JSON_CONSTANTS_KEY_RELATIONSHIP " is an edge with 1 source and 1 destination end.\",\n"
    "            \"type\": \"object\",\n"
    "            \"properties\":\n"
    "            {\n";

static const char SCHEMA_RELATIONSHIP_FOOTER[]
    = "            },\n"
    "            \"additionalProperties\": false\n"
    "          }\n";

static const char SCHEMA_GRAPH_EDGES_FOOTER[]
    = "        }\n"
    "      }\n"
    "    }\n";

static const char SCHEMA_GRAPH_FOOTER[]
    = "  }\n"
    "}\n";

static const char INDENT_4[] = "        ";
static const char INDENT_7[] = "              ";
static const char INDENT_10[] = "                    ";

void json_schema_writer_init ( json_schema_writer_t *this_,
                               universal_output_stream_t *output )
{
    U8_TRACE_BEGIN();
    assert( NULL != output );

    utf8stream_writer_init( &((*this_).writer), output );
    (*this_).indent = 4;

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

    static const char *const ANY_ID
        = "This integer number identifies the object within the database.\\n"
        " The id is a proposal; it may be changed automatically.";
    static const char *const ANY_UUID = "The uuid is an identifier according to rfc-9562, all letters must be lowercase.";
    static const char *const ANY_STEREO = "This is the stereotype; it is available since version 1.47.0.";
    static const char *const ANY_DESCR = "This is a part of the description (to be concatenated).";
    static const char *const ANY_ORDER
        = "This is a value between –2147483648 and 2147483647 by which to order elements in 1 or 2 dimensions";

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_HEADER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_HEAD_HEADER );

    (*this_).indent = 4;
    static const char *const HEAD_ENC_INFO = "Only \\\"utf-8\\\" is supported.";
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_HEAD_ENCODING, HEAD_ENC_INFO, 0 );

    static const char *const HEAD_STRUCT_INFO
        = "Only \\\"rfc-8259 w/o hexadecimal escapes\\\" is supported; "
        "hexadecimal escapes are not needed because utf-8 can encode all unicode characters.";
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_HEAD_STRUCTURE, HEAD_STRUCT_INFO, 0 );

    static const char *const HEAD_FORMAT_INFO = "Only \\\"cfu-json\\\" is supported.";
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_HEAD_FORMAT, HEAD_FORMAT_INFO, 0 );

    static const char *const HEAD_MAJOR_INFO
        = "The current major version is " SCHEMA_STR(JSON_CONSTANTS_VALUE_HEAD_MAJOR_VERSION) ", "
        "the major version changes if an older json reader cannot read a new format.";
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_HEAD_MAJOR_VERSION, HEAD_MAJOR_INFO );

    static const char *const HEAD_MINOR_INFO
        = "The current minor version is " SCHEMA_STR(JSON_CONSTANTS_VALUE_HEAD_MINOR_VERSION) ", "
        "the minor version changes if an older json reader may not process all information.";
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_HEAD_MINOR_VERSION, HEAD_MINOR_INFO );

    static const char *const HEAD_GEN_NAME_INFO = "This is the name of the generator, can be chosen freely-";
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_HEAD_GENERATOR_NAME,
                                                             HEAD_GEN_NAME_INFO,
                                                             DATA_HEAD_MAX_VALUE_LENGTH
                                                           );

    static const char *const HEAD_GEN_VERSION_INFO = "This is the version of the generator, can be chosen freely.";
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_HEAD_GENERATOR_VERSION,
                                                             HEAD_GEN_VERSION_INFO,
                                                             DATA_HEAD_MAX_VALUE_LENGTH
                                                           );

    static const char *const HEAD_ID_PREFIX_INFO = "This is a prefix string that is prepended to ids within this model.";
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_HEAD_MODEL_ID_PREFIX,
                                                             HEAD_ID_PREFIX_INFO,
                                                             15
                                                           );

    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_HEAD_MODEL_UUID, ANY_UUID, false /*no elements follow*/ );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_HEAD_FOOTER );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_VIEWS_HEADER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_DIAGRAM_HEADER );

    (*this_).indent = 7;
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_DIAGRAM_ID, ANY_ID );

    static const char *const D_PARENT_NAME
        = "This is the name of the parent diagram, exported for reviews by humans, ignored at import";
    export_err |= json_schema_writer_private_declare_string( this_, JSON_CONSTANTS_KEY_DIAGRAM_PARENT_NAME, D_PARENT_NAME, 0 );

    static const char *const D_PARENT_ID = "The uuid of the parent diagram, it is omitted only by the root diagram.";
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_DIAGRAM_PARENT, D_PARENT_ID, true );

    /* generate an enum listing all options */
    static const char *const D_DIAGTYPE_NAME
#ifdef JSON_CONSTANTS_NEW_KEYS
        = JSON_CONSTANTS_KEY_TYPE_ID " as string, exported for reviews by humans, ignored at import.";
#else
        = JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE " as string, exported for reviews by humans, ignored at import.";
#endif
    export_err |= json_schema_writer_private_declare_enum_name( this_,
                                                                JSON_CONSTANTS_KEY_TYPE_NAME,
                                                                D_DIAGTYPE_NAME,
                                                                DATA_ENUM_ITERATOR( DATA_DIAGRAM_TYPE_ENUM_ARRAY )
                                                              );

    static const char *const D_DIAGTYPE_ID = "This is the numeric value of the diagram type.";
#ifdef JSON_CONSTANTS_NEW_KEYS
    export_err |= json_schema_writer_private_declare_enum_number( this_,
                                                                  JSON_CONSTANTS_KEY_TYPE_ID,
                                                                  D_DIAGTYPE_ID,
                                                                  DATA_ENUM_ITERATOR( DATA_DIAGRAM_TYPE_ENUM_ARRAY )
                                                                );
#else
    export_err |= json_schema_writer_private_declare_enum_number( this_,
                                                                  JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE,
                                                                  D_DIAGTYPE_ID,
                                                                  DATA_ENUM_ITERATOR( DATA_DIAGRAM_TYPE_ENUM_ARRAY )
                                                                );
#endif

    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_DIAGRAM_STEREOTYPE,
                                                             ANY_STEREO,
                                                             DATA_DIAGRAM_MAX_STEREOTYPE_LENGTH
                                                           );

    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_DIAGRAM_NAME,
                                                             "This is the title of the diagram.",
                                                             DATA_DIAGRAM_MAX_NAME_LENGTH
                                                           );

    export_err |= json_schema_writer_private_declare_array_of_string( this_,
                                                                      JSON_CONSTANTS_KEY_DIAGRAM_DESCRIPTION,
                                                                      "This is the description of the diagram.",
                                                                      DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH,
                                                                      ANY_DESCR
                                                                    );

#ifdef JSON_CONSTANTS_NEW_KEYS
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_ORDER, ANY_ORDER );
#else
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_DIAGRAM_LIST_ORDER, ANY_ORDER );
#endif

    /* generate an enum listing all options */
    static const char *const D_FLAG_NAME
        = JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAGS " as string, exported for reviews by humans, ignored at import.";
    export_err |= json_schema_writer_private_declare_enum_name( this_,
                                                                JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAG_NAMES,
                                                                D_FLAG_NAME,
                                                                DATA_ENUM_ITERATOR( DATA_DIAGRAM_FLAG_ENUM_ARRAY )
                                                              );

    static const char *const D_FLAGS = "This is a set of flags that influence the layout of a diagram.";
    export_err |= json_schema_writer_private_declare_enum_number( this_,
                                                                  JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAGS,
                                                                  D_FLAGS,
                                                                  DATA_ENUM_ITERATOR( DATA_DIAGRAM_FLAG_ENUM_ARRAY )
                                                                );

    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_UUID, ANY_UUID, true /*elements follow*/ );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_DIAGRAM_ELEMENTS_HEADER );

    (*this_).indent = 10;
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_ID, ANY_ID );

    static const char *const E_NODE_C_NAME
        = "The name of " JSON_CONSTANTS_KEY_DIAGRAMELEMENT_NODE "/classifier is exported for reviews by humans, ignored at import.";
#ifdef JSON_CONSTANTS_NEW_KEYS
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_DIAGRAMELEMENT_NODE_NAME,
                                                             E_NODE_C_NAME
                                                             0
                                                           );
#else
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_DIAGRAMELEMENT_CLASSIFIER_NAME,
                                                             E_NODE_C_NAME,
                                                             0
                                                           );

    static const char *const E_NODE_F_NAME
        = "The name of " JSON_CONSTANTS_KEY_DIAGRAMELEMENT_NODE
            "/feature (if lifeline) is optionally exported for reviews by humans, ignored at import.";
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_DIAGRAMELEMENT_FOCUSED_FEATURE_NAME,
                                                             E_NODE_F_NAME,
                                                             0
                                                           );
#endif

    static const char *const E_NODE = "This is the uuid of either the classifier or the feature (type lifeline only).";
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_NODE, E_NODE, true );

    /* generate an enum listing all options */
    static const char *const E_FLAG_NAME
        = JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAGS " as string is exported for reviews by humans, ignored at import.";
    export_err |= json_schema_writer_private_declare_enum_name( this_,
                                                                JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAG_NAMES,
                                                                E_FLAG_NAME,
                                                                DATA_ENUM_ITERATOR( DATA_DIAGRAMELEMENT_FLAG_ENUM_ARRAY )
                                                              );

    static const char *const E_FLAGS = "This is a set of flags that influence the layout of a diagramelement.";
    export_err |= json_schema_writer_private_declare_enum_number( this_,
                                                                  JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAGS,
                                                                  E_FLAGS,
                                                                  DATA_ENUM_ITERATOR( DATA_DIAGRAMELEMENT_FLAG_ENUM_ARRAY )
                                                                );

    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_UUID, ANY_UUID, false );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_DIAGRAM_ELEMENTS_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_DIAGRAM_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_VIEWS_FOOTER );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_NODES_HEADER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_CLASSIFIER_HEADER );

    (*this_).indent = 7;
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_CLASSIFIER_ID, ANY_ID );

    /* generate an enum listing all options */
    static const char *const C_TYPE_NAME
#ifdef JSON_CONSTANTS_NEW_KEYS
        = "The name of " JSON_CONSTANTS_KEY_TYPE_ID " is exported for reviews by humans, ignored at import.";
#else
        = "The name of " JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE " is exported for reviews by humans, ignored at import.";
#endif
    export_err |= json_schema_writer_private_declare_enum_name( this_,
                                                                JSON_CONSTANTS_KEY_TYPE_NAME,
                                                                C_TYPE_NAME,
                                                                DATA_ENUM_ITERATOR( DATA_CLASSIFIER_TYPE_ENUM_ARRAY )
                                                              );

    static const char *const C_TYPE_ID = "This is the numeric value of classifier type.";
#ifdef JSON_CONSTANTS_NEW_KEYS
    export_err |= json_schema_writer_private_declare_enum_number( this_,
                                                                  JSON_CONSTANTS_KEY_TYPE_ID,
                                                                  C_TYPE_ID,
                                                                  DATA_ENUM_ITERATOR( DATA_CLASSIFIER_TYPE_ENUM_ARRAY )
                                                                );
#else
    export_err |= json_schema_writer_private_declare_enum_number( this_,
                                                                  JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE,
                                                                  C_TYPE_ID,
                                                                  DATA_ENUM_ITERATOR( DATA_CLASSIFIER_TYPE_ENUM_ARRAY )
                                                                );
#endif

    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_CLASSIFIER_STEREOTYPE,
                                                             "This is the stereotype of the classifier.",
                                                             DATA_CLASSIFIER_MAX_STEREOTYPE_LENGTH
                                                           );

    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_CLASSIFIER_NAME,
                                                             "This is the name of the classifier.\\n"
                                                                 " The name must be unique among all classifiers.",
                                                             DATA_CLASSIFIER_MAX_NAME_LENGTH
                                                           );

    export_err |= json_schema_writer_private_declare_array_of_string( this_,
                                                                      JSON_CONSTANTS_KEY_CLASSIFIER_DESCRIPTION,
                                                                      "This is the description of the classifier.",
                                                                      DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH,
                                                                      ANY_DESCR
                                                                    );

    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_CLASSIFIER_X_ORDER, ANY_ORDER );

    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_CLASSIFIER_Y_ORDER, ANY_ORDER );

#ifdef JSON_CONSTANTS_NEW_KEYS
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_ORDER, ANY_ORDER );
#else
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_CLASSIFIER_LIST_ORDER, ANY_ORDER );
#endif

    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_UUID, ANY_UUID, true /*features follow*/ );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_FEATURES_HEADER );

    (*this_).indent = 10;
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_FEATURE_ID, ANY_ID );

    /* generate an enum listing all options */
    static const char *const F_TYPE_NAME
#ifdef JSON_CONSTANTS_NEW_KEYS
        = "The name of " JSON_CONSTANTS_KEY_TYPE_ID " is exported for reviews by humans, ignored at import.";
#else
        = "The name of " JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE " is exported for reviews by humans, ignored at import.";
#endif
    export_err |= json_schema_writer_private_declare_enum_name( this_,
                                                                JSON_CONSTANTS_KEY_TYPE_NAME,
                                                                F_TYPE_NAME,
                                                                DATA_ENUM_ITERATOR( DATA_FEATURE_TYPE_ENUM_ARRAY )
                                                              );

    static const char *const F_TYPE_ID = "This is the numeric value of the feature type.";
#ifdef JSON_CONSTANTS_NEW_KEYS
    export_err |= json_schema_writer_private_declare_enum_number( this_,
                                                                  JSON_CONSTANTS_KEY_TYPE_ID,
                                                                  F_TYPE_ID,
                                                                  DATA_ENUM_ITERATOR( DATA_FEATURE_TYPE_ENUM_ARRAY )
                                                                );
#else
    export_err |= json_schema_writer_private_declare_enum_number( this_,
                                                                  JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE,
                                                                  F_TYPE_ID,
                                                                  DATA_ENUM_ITERATOR( DATA_FEATURE_TYPE_ENUM_ARRAY )
                                                                );
#endif

    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_FEATURE_KEY,
                                                             "This is the name of the feature.",
                                                             DATA_FEATURE_MAX_KEY_LENGTH
                                                           );

    static const char *const F_VALUE
        = "This is the valuetype in case of properties, the value in case of tagges values, the stereotype otherwise.";
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_FEATURE_VALUE,
                                                             F_VALUE,
                                                             DATA_FEATURE_MAX_VALUE_LENGTH
                                                           );

    export_err |= json_schema_writer_private_declare_array_of_string( this_,
                                                                      JSON_CONSTANTS_KEY_FEATURE_DESCRIPTION,
                                                                      "This is the description of the feature.",
                                                                      DATA_FEATURE_MAX_DESCRIPTION_LENGTH,
                                                                      ANY_DESCR
                                                                    );

#ifdef JSON_CONSTANTS_NEW_KEYS
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_ORDER, ANY_ORDER );
#else
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_FEATURE_LIST_ORDER, ANY_ORDER );
#endif

    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_UUID, ANY_UUID, false );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_FEATURES_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_CLASSIFIER_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_NODES_FOOTER );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_EDGES_HEADER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_RELATIONSHIP_HEADER );

    (*this_).indent = 7;
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_ID, ANY_ID );

    /* generate an enum listing all options */
    static const char *const R_TYPE_NAME
#ifdef JSON_CONSTANTS_NEW_KEYS
        = "The name of " JSON_CONSTANTS_KEY_TYPE_ID " is exported for reviews by humans, ignored at import.";
#else
        = "The name of " JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE " is exported for reviews by humans, ignored at import.";
#endif
    export_err |= json_schema_writer_private_declare_enum_name( this_,
                                                                JSON_CONSTANTS_KEY_TYPE_NAME,
                                                                R_TYPE_NAME,
                                                                DATA_ENUM_ITERATOR( DATA_RELATIONSHIP_TYPE_ENUM_ARRAY )
                                                              );

    static const char *const R_TYPE_ID = "This is the numeric value of relationship type.";
#ifdef JSON_CONSTANTS_NEW_KEYS
    export_err |= json_schema_writer_private_declare_enum_number( this_,
                                                                  JSON_CONSTANTS_KEY_TYPE_ID,
                                                                  R_TYPE_ID,
                                                                  DATA_ENUM_ITERATOR( DATA_RELATIONSHIP_TYPE_ENUM_ARRAY )
                                                                );
#else
    export_err |= json_schema_writer_private_declare_enum_number( this_,
                                                                  JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE,
                                                                  R_TYPE_ID,
                                                                  DATA_ENUM_ITERATOR( DATA_RELATIONSHIP_TYPE_ENUM_ARRAY )
                                                                );
#endif

    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_RELATIONSHIP_STEREOTYPE,
                                                             ANY_STEREO,
                                                             DATA_RELATIONSHIP_MAX_STEREOTYPE_LENGTH
                                                           );

    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_RELATIONSHIP_NAME,
                                                             "This is the name of the relationship.",
                                                             DATA_RELATIONSHIP_MAX_NAME_LENGTH
                                                           );

    export_err |= json_schema_writer_private_declare_array_of_string( this_,
                                                                      JSON_CONSTANTS_KEY_RELATIONSHIP_DESCRIPTION,
                                                                      "This is the description of the relationship.",
                                                                      DATA_RELATIONSHIP_MAX_DESCRIPTION_LENGTH,
                                                                      ANY_DESCR
                                                                    );

#ifdef JSON_CONSTANTS_NEW_KEYS
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_ORDER, ANY_ORDER );
#else
    export_err |= json_schema_writer_private_declare_integer( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_LIST_ORDER, ANY_ORDER );
#endif

#ifdef JSON_CONSTANTS_NEW_KEYS
    static const char *const E_FROM_NODE_C_NAME
        = "The name of " JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_NODE_UUID "/classifier is exported for reviews by humans, ignored at import.";
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_NODE_NAME,
                                                             E_FROM_NODE_C_NAME,
                                                             0
                                                           );
#else
    static const char *const E_FROM_NODE_C_NAME
        = "The name of " JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_NODE "/classifier is exported for reviews by humans, ignored at import.";
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_NAME,
                                                             E_FROM_NODE_C_NAME,
                                                             0
                                                           );
#endif

#ifdef JSON_CONSTANTS_NEW_KEYS
    static const char *const E_FROM_NODE_F_NAME
        = "The key of " JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_NODE_UUID
            "/feature is exported (if applicable) for reviews by humans, ignored at import";
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_PORT_NAME,
                                                             E_FROM_NODE_F_NAME,
                                                             0
                                                           );
#else
    static const char *const E_FROM_NODE_F_NAME
        = "The key of " JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_NODE
            "/feature is exported (if applicable) for reviews by humans, ignored at import";
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_FEATURE_KEY,
                                                             E_FROM_NODE_F_NAME,
                                                             0
                                                           );
#endif

    static const char *const E_FROM_NODE = "The uuid of the source: either the classifier or the feature.";
#ifdef JSON_CONSTANTS_NEW_KEYS
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_NODE_UUID, E_FROM_NODE, true );
#else
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_NODE, E_FROM_NODE, true );
#endif

#ifdef JSON_CONSTANTS_NEW_KEYS
    static const char *const E_TO_NODE_C_NAME
        = "The name of " JSON_CONSTANTS_KEY_RELATIONSHIP_TO_NODE_UUID "/classifier is exported for reviews by humans, ignored at import.";
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_RELATIONSHIP_TO_NODE_NAME,
                                                             E_TO_NODE_C_NAME,
                                                             0
                                                           );
#else
    static const char *const E_TO_NODE_C_NAME
        = "The name of " JSON_CONSTANTS_KEY_RELATIONSHIP_TO_NODE "/classifier is exported for reviews by humans, ignored at import.";
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_NAME,
                                                             E_TO_NODE_C_NAME,
                                                             0
                                                           );
#endif

#ifdef JSON_CONSTANTS_NEW_KEYS
    static const char *const E_TO_NODE_F_NAME
        = "The key of " JSON_CONSTANTS_KEY_RELATIONSHIP_TO_NODE_UUID
        "/feature is exported (if applicable) for reviews by humans, ignored at import.";
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_RELATIONSHIP_TO_PORT_NAME,
                                                             E_TO_NODE_F_NAME,
                                                             0
                                                           );
#else
    static const char *const E_TO_NODE_F_NAME
        = "The key of " JSON_CONSTANTS_KEY_RELATIONSHIP_TO_NODE
        "/feature is exported (if applicable) for reviews by humans, ignored at import.";
    export_err |= json_schema_writer_private_declare_string( this_,
                                                             JSON_CONSTANTS_KEY_RELATIONSHIP_TO_FEATURE_KEY,
                                                             E_TO_NODE_F_NAME,
                                                             0
                                                           );
#endif

    static const char *const E_TO_NODE = "The uuid of the destination: either the classifier or the feature.";
#ifdef JSON_CONSTANTS_NEW_KEYS
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_TO_NODE_UUID, E_TO_NODE, true );
#else
    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_RELATIONSHIP_TO_NODE, E_TO_NODE, true );
#endif

    export_err |= json_schema_writer_private_declare_uuid( this_, JSON_CONSTANTS_KEY_UUID, ANY_UUID, false );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_RELATIONSHIP_FOOTER );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), SCHEMA_GRAPH_EDGES_FOOTER );
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
    const char *const indent = ((*this_).indent==10) ? INDENT_10 : ((*this_).indent==7) ? INDENT_7 : INDENT_4;

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
                                                      const char* description,
                                                      unsigned int max_utf8_byte_length
                                                    )
{
    U8_TRACE_BEGIN();
    assert( name != NULL );
    assert( description != NULL );
    u8_error_t export_err = U8_ERROR_NONE;
    const char *const indent = ((*this_).indent==10) ? INDENT_10 : ((*this_).indent==7) ? INDENT_7 : INDENT_4;

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), name );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\":\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "{\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"description\": \"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), description );
    if ( max_utf8_byte_length != 0 )
    {
        export_err |= utf8stream_writer_write_str( &((*this_).writer),
                                                   "\\n The utf-8 encoding of the string must not exceed "
                                                 );
        export_err |= utf8stream_writer_write_int( &((*this_).writer), max_utf8_byte_length );
        export_err |= utf8stream_writer_write_str( &((*this_).writer), " bytes." );
    }
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
                                                               unsigned int max_utf8_byte_length,
                                                               const char* element_description )
{
    U8_TRACE_BEGIN();
    assert( name != NULL );
    assert( description != NULL );
    assert( element_description != NULL );
    u8_error_t export_err = U8_ERROR_NONE;
    const char *const indent = ((*this_).indent==10) ? INDENT_10 : ((*this_).indent==7) ? INDENT_7 : INDENT_4;

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent  );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), name );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\":\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "{\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"description\": \"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), description );
    if ( max_utf8_byte_length != 0 )
    {
        export_err |= utf8stream_writer_write_str( &((*this_).writer),
                                                   "\\n The utf-8 encoding of the concatenated strings must not exceed "
                                                 );
        export_err |= utf8stream_writer_write_int( &((*this_).writer), max_utf8_byte_length );
        export_err |= utf8stream_writer_write_str( &((*this_).writer), " bytes." );
    }
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

u8_error_t json_schema_writer_private_declare_enum_name( json_schema_writer_t *this_,
                                                         const char* name,
                                                         const char* description,
                                                         data_enum_iterator_t enum_list )
{
    U8_TRACE_BEGIN();
    assert( name != NULL );
    assert( description != NULL );
    u8_error_t export_err = U8_ERROR_NONE;
    const char *const indent = ((*this_).indent==10) ? INDENT_10 : ((*this_).indent==7) ? INDENT_7 : INDENT_4;

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

    while ( data_enum_iterator_has_next( &enum_list ) )
    {
        data_enum_def_t data_enum;
        const u8_error_t iter_err = data_enum_iterator_next( &enum_list, &data_enum );
        assert( iter_err == U8_ERROR_NONE );
        (void) iter_err;
        const bool has_next = data_enum_iterator_has_next( &enum_list );

        export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
        export_err |= utf8stream_writer_write_str( &((*this_).writer), "    \"" );
        export_err |= utf8stream_writer_write_str( &((*this_).writer), data_enum_def_get_name( &data_enum ) );
        export_err |= utf8stream_writer_write_str( &((*this_).writer), has_next ? "\",\n": "\"\n" );
    }

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  ]\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "},\n" );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t json_schema_writer_private_declare_enum_number( json_schema_writer_t *this_,
                                                           const char* name,
                                                           const char* description,
                                                           data_enum_iterator_t enum_list )
{
    U8_TRACE_BEGIN();
    assert( name != NULL );
    assert( description != NULL );
    u8_error_t export_err = U8_ERROR_NONE;
    const char *const indent = ((*this_).indent==10) ? INDENT_10 : ((*this_).indent==7) ? INDENT_7 : INDENT_4;

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), name );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\":\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "{\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"description\": \"" );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), description );
    while ( data_enum_iterator_has_next( &enum_list ) )
    {
        data_enum_def_t data_enum;
        const u8_error_t iter_err = data_enum_iterator_next( &enum_list, &data_enum );
        assert( iter_err == U8_ERROR_NONE );
        (void) iter_err;

        export_err |= utf8stream_writer_write_str( &((*this_).writer), "\\n " );
        export_err |= utf8stream_writer_write_int( &((*this_).writer), data_enum_def_get_number( &data_enum ) );
        export_err |= utf8stream_writer_write_str( &((*this_).writer), ": " );
        export_err |= utf8stream_writer_write_str( &((*this_).writer), data_enum_def_get_name( &data_enum ) );
    }
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "\",\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"type\": \"number\"\n" );

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
    const char *const indent = ((*this_).indent==10) ? INDENT_10 : ((*this_).indent==7) ? INDENT_7 : INDENT_4;

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
    export_err |= utf8stream_writer_write_str( &((*this_).writer), "  \"type\": \"string\",\n" );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer),
                                               "  \"pattern\": \"^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$\"\n"
                                             );

    export_err |= utf8stream_writer_write_str( &((*this_).writer), indent );
    export_err |= utf8stream_writer_write_str( &((*this_).writer), has_next ? "},\n" : "}\n" );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}


/*
Copyright 2023-2026 Andreas Warnke

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
