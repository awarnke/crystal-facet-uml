/* File: data_json_constants.h; Copyright and License: see below */

#ifndef DATA_JSON_CONSTANTS_H
#define DATA_JSON_CONSTANTS_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines constants for key-strings in json format.
 */

/* structure */

#define DATA_JSON_CONSTANTS_BEGIN_OBJECT_NL "{\n"
#define DATA_JSON_CONSTANTS_END_OBJECT_NL "}\n"
#define DATA_JSON_CONSTANTS_BEGIN_OBJECT "{"
#define DATA_JSON_CONSTANTS_END_OBJECT "}"
#define DATA_JSON_CONSTANTS_TAB "  "
#define DATA_JSON_CONSTANTS_QUOTE "\""
#define DATA_JSON_CONSTANTS_DEF ": "
#define DATA_JSON_CONSTANTS_BEGIN_ARRAY "["
#define DATA_JSON_CONSTANTS_END_ARRAY "]"
#define DATA_JSON_CONSTANTS_NEXT_NL ",\n"
#define DATA_JSON_CONSTANTS_NL "\n"
#define DATA_JSON_CONSTANTS_TRUE "true"
#define DATA_JSON_CONSTANTS_FALSE "false"
#define DATA_JSON_CONSTANTS_NULL "null"

#define DATA_JSON_CONSTANTS_CHAR_NL '\n'
#define DATA_JSON_CONSTANTS_CHAR_CR '\r'
#define DATA_JSON_CONSTANTS_CHAR_TAB '\t'
#define DATA_JSON_CONSTANTS_CHAR_SPACE ' '
#define DATA_JSON_CONSTANTS_CHAR_BEGIN_OBJECT '{'
#define DATA_JSON_CONSTANTS_CHAR_END_OBJECT '}'
#define DATA_JSON_CONSTANTS_CHAR_BEGIN_ARRAY '['
#define DATA_JSON_CONSTANTS_CHAR_END_ARRAY ']'
#define DATA_JSON_CONSTANTS_CHAR_BEGIN_STRING '\"'
#define DATA_JSON_CONSTANTS_CHAR_END_STRING '\"'
#define DATA_JSON_CONSTANTS_CHAR_NAME_SEPARATOR ':'
#define DATA_JSON_CONSTANTS_CHAR_VALUE_SEPARATOR ','
#define DATA_JSON_CONSTANTS_CHAR_ESC '\\'

/* content */

#define DATA_JSON_CONSTANTS_KEY_SET "set"

#define DATA_JSON_CONSTANTS_KEY_DIAGRAM "diagram"
#define DATA_JSON_CONSTANTS_KEY_DIAGRAM_ID "id"
/* parent_id not part of json export */
#define DATA_JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE "diagram_type"
#define DATA_JSON_CONSTANTS_KEY_DIAGRAM_NAME "name"
#define DATA_JSON_CONSTANTS_KEY_DIAGRAM_DESCRIPTION "description"
#define DATA_JSON_CONSTANTS_KEY_DIAGRAM_LIST_ORDER "list_order"

/* data_diagramelement_t not part of json export */

#define DATA_JSON_CONSTANTS_KEY_CLASSIFIER "classifier"
#define DATA_JSON_CONSTANTS_KEY_CLASSIFIER_ID "id"
#define DATA_JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE "main_type"
#define DATA_JSON_CONSTANTS_KEY_CLASSIFIER_STEREOTYPE "stereotype"
#define DATA_JSON_CONSTANTS_KEY_CLASSIFIER_NAME "name"
#define DATA_JSON_CONSTANTS_KEY_CLASSIFIER_DESCRIPTION "description"
#define DATA_JSON_CONSTANTS_KEY_CLASSIFIER_X_ORDER "x_order"
#define DATA_JSON_CONSTANTS_KEY_CLASSIFIER_Y_ORDER "y_order"
#define DATA_JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES "features"

#define DATA_JSON_CONSTANTS_KEY_FEATURE_ID "id"
/* classifier_id not part of json export */
#define DATA_JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE "main_type"
#define DATA_JSON_CONSTANTS_KEY_FEATURE_KEY "key"
#define DATA_JSON_CONSTANTS_KEY_FEATURE_VALUE "value"
#define DATA_JSON_CONSTANTS_KEY_FEATURE_DESCRIPTION "description"
#define DATA_JSON_CONSTANTS_KEY_FEATURE_LIST_ORDER "list_order"

#define DATA_JSON_CONSTANTS_KEY_RELATIONSHIP "relationship"
#define DATA_JSON_CONSTANTS_KEY_RELATIONSHIP_ID "id"
#define DATA_JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE "main_type"
#define DATA_JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_ID "from_classifier_id"
#define DATA_JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_ID "to_classifier_id"
#define DATA_JSON_CONSTANTS_KEY_RELATIONSHIP_NAME "name"
#define DATA_JSON_CONSTANTS_KEY_RELATIONSHIP_DESCRIPTION "description"
#define DATA_JSON_CONSTANTS_KEY_RELATIONSHIP_LIST_ORDER "list_order"

#endif  /* DATA_JSON_CONSTANTS_H */


/*
Copyright 2016-2018 Andreas Warnke

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
