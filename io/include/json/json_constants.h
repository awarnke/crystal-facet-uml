/* File: json_constants.h; Copyright and License: see below */

#ifndef JSON_CONSTANTS_H
#define JSON_CONSTANTS_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines constants for key-strings in json format.
 */

/* structure */

#define JSON_CONSTANTS_BEGIN_OBJECT_NL "{\n"
#define JSON_CONSTANTS_END_OBJECT_NL "}\n"
#define JSON_CONSTANTS_BEGIN_OBJECT "{"
#define JSON_CONSTANTS_END_OBJECT "}"
#define JSON_CONSTANTS_TAB "  "
#define JSON_CONSTANTS_QUOTE "\""
#define JSON_CONSTANTS_DEF ": "
#define JSON_CONSTANTS_BEGIN_ARRAY "["
#define JSON_CONSTANTS_END_ARRAY "]"
#define JSON_CONSTANTS_NEXT_NL ",\n"
#define JSON_CONSTANTS_NL "\n"
#define JSON_CONSTANTS_TRUE "true"
#define JSON_CONSTANTS_FALSE "false"
#define JSON_CONSTANTS_NULL "null"

#define JSON_CONSTANTS_CHAR_NL '\n'
#define JSON_CONSTANTS_CHAR_CR '\r'
#define JSON_CONSTANTS_CHAR_TAB '\t'
#define JSON_CONSTANTS_CHAR_SPACE ' '
#define JSON_CONSTANTS_CHAR_BEGIN_OBJECT '{'
#define JSON_CONSTANTS_CHAR_END_OBJECT '}'
#define JSON_CONSTANTS_CHAR_BEGIN_ARRAY '['
#define JSON_CONSTANTS_CHAR_END_ARRAY ']'
#define JSON_CONSTANTS_CHAR_BEGIN_STRING '\"'
#define JSON_CONSTANTS_CHAR_END_STRING '\"'
#define JSON_CONSTANTS_CHAR_NAME_SEPARATOR ':'
#define JSON_CONSTANTS_CHAR_VALUE_SEPARATOR ','
#define JSON_CONSTANTS_CHAR_ESC '\\'
#define JSON_CONSTANTS_CHAR_BEGIN_TRUE 't'
#define JSON_CONSTANTS_CHAR_BEGIN_FALSE 'f'
#define JSON_CONSTANTS_CHAR_BEGIN_NULL 'n'

/* content */

#define JSON_CONSTANTS_KEY_HEAD "head"
#define JSON_CONSTANTS_KEY_DATA "data"
/*
#define JSON_CONSTANTS_KEY_NODES "nodes"
#define JSON_CONSTANTS_KEY_EDGES "edges"
#define JSON_CONSTANTS_KEY_VIEWS "views"
#define JSON_CONSTANTS_KEY_META "meta"
*/
#define JSON_CONSTANTS_KEY_UUID "uuid"

#define JSON_CONSTANTS_KEY_DIAGRAM "diagram"
#define JSON_CONSTANTS_KEY_DIAGRAM_ID "id"
#define JSON_CONSTANTS_KEY_DIAGRAM_PARENT_ID "parent_id"
#define JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE "diagram_type"
#define JSON_CONSTANTS_KEY_DIAGRAM_NAME "name"
#define JSON_CONSTANTS_KEY_DIAGRAM_DESCRIPTION "description"
#define JSON_CONSTANTS_KEY_DIAGRAM_LIST_ORDER "list_order"
#define JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAGS "display_flags"

#define JSON_CONSTANTS_KEY_DIAGRAMELEMENT "diagramelement"
#define JSON_CONSTANTS_KEY_DIAGRAMELEMENT_ID "id"
#define JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DIAGRAM_ID "diagram_id"
#define JSON_CONSTANTS_KEY_DIAGRAMELEMENT_CLASSIFIER_ID "classifier_id"
#define JSON_CONSTANTS_KEY_DIAGRAMELEMENT_FOCUSED_FEATURE_ID "focused_feature_id"
#define JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAGS "display_flags"

#define JSON_CONSTANTS_KEY_CLASSIFIER "classifier"
#define JSON_CONSTANTS_KEY_CLASSIFIER_ID "id"
#define JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE "main_type"
#define JSON_CONSTANTS_KEY_CLASSIFIER_STEREOTYPE "stereotype"
#define JSON_CONSTANTS_KEY_CLASSIFIER_NAME "name"
#define JSON_CONSTANTS_KEY_CLASSIFIER_DESCRIPTION "description"
#define JSON_CONSTANTS_KEY_CLASSIFIER_X_ORDER "x_order"
#define JSON_CONSTANTS_KEY_CLASSIFIER_Y_ORDER "y_order"
#define JSON_CONSTANTS_KEY_CLASSIFIER_LIST_ORDER "list_order"
#define JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES "features"

#define JSON_CONSTANTS_KEY_FEATURE_ID "id"
#define JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE "main_type"
#define JSON_CONSTANTS_KEY_FEATURE_KEY "key"
#define JSON_CONSTANTS_KEY_FEATURE_VALUE "value"
#define JSON_CONSTANTS_KEY_FEATURE_DESCRIPTION "description"
#define JSON_CONSTANTS_KEY_FEATURE_LIST_ORDER "list_order"

#define JSON_CONSTANTS_KEY_RELATIONSHIP "relationship"
#define JSON_CONSTANTS_KEY_RELATIONSHIP_ID "id"
#define JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_ID "from_classifier_id"
#define JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_NAME "from_classifier_name"
#define JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_FEATURE_ID "from_feature_id"
#define JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_FEATURE_KEY "from_feature_key"
#define JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_ID "to_classifier_id"
#define JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_NAME "to_classifier_name"
#define JSON_CONSTANTS_KEY_RELATIONSHIP_TO_FEATURE_ID "to_feature_id"
#define JSON_CONSTANTS_KEY_RELATIONSHIP_TO_FEATURE_KEY "to_feature_key"
#define JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE "main_type"
#define JSON_CONSTANTS_KEY_RELATIONSHIP_NAME "name"
#define JSON_CONSTANTS_KEY_RELATIONSHIP_DESCRIPTION "description"
#define JSON_CONSTANTS_KEY_RELATIONSHIP_LIST_ORDER "list_order"

#endif  /* JSON_CONSTANTS_H */


/*
Copyright 2016-2021 Andreas Warnke

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
