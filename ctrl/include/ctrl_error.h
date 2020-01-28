/* File: ctrl_error.h; Copyright and License: see below */

#ifndef CTRL_ERROR_H
#define CTRL_ERROR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Notifies errors in the ctrl module
 */

#include "data_error.h"

/*!
 *  \brief error constants which explain errors which occurred in the ctrl module
 *
 *  \note to allow easy conversion from data_error_t, this is a superset of data_error_t.
 */
enum ctrl_error_enum {
    CTRL_ERROR_NONE = DATA_ERROR_NONE,  /*!< success */
    
    /* application layer errors */
    CTRL_ERROR_NO_DB = DATA_ERROR_NO_DB,  /*!< database not open/loaded */
    CTRL_ERROR_DB_STRUCTURE = DATA_ERROR_DB_STRUCTURE,  /*!< the structure of the database is corrupted */
    CTRL_ERROR_STRING_BUFFER_EXCEEDED = DATA_ERROR_STRING_BUFFER_EXCEEDED,  /*!< a string does not fit to the string storage buffer */
    CTRL_ERROR_ARRAY_BUFFER_EXCEEDED = DATA_ERROR_ARRAY_BUFFER_EXCEEDED,  /*!< a set of objects does not fit to the array storage buffer */
    CTRL_ERROR_INVALID_REQUEST = DATA_ERROR_INVALID_REQUEST,  /*!< function call not allowed or parameters wrong */
    CTRL_ERROR_INPUT_EMPTY = DATA_ERROR_INPUT_EMPTY,  /*!< input parameter is empty or void */
    CTRL_ERROR_OBJECT_STILL_REFERENCED = DATA_ERROR_OBJECT_STILL_REFERENCED,  /*!< object cannot be deleted, it is still referenced */
    
    CTRL_ERROR_LEXICAL_STRUCTURE = DATA_ERROR_LEXICAL_STRUCTURE,  /*!< the lexical structure of the input-string is corrupted (contains invalid tokens) */
    CTRL_ERROR_PARSER_STRUCTURE = DATA_ERROR_PARSER_STRUCTURE,  /*!< the parser structure of the input-string is corrupted (wrong order of tokens) */

    CTRL_ERROR_DIAGRAM_HIDES_RELATIONSHIPS = DATA_ERROR_DIAGRAM_HIDES_RELATIONSHIPS,  /*!< the diagram type does not show the relationship type */
    CTRL_ERROR_DIAGRAM_HIDES_FEATURES = DATA_ERROR_DIAGRAM_HIDES_FEATURES,  /*!< the diagram type does not show the feature type */
    CTRL_ERROR_CLASSIFIER_REFUSES_FEATURE = DATA_ERROR_CLASSIFIER_REFUSES_FEATURE,  /*!< the classifier type does not allow the feature type */

    /* database layer errors */
    CTRL_ERROR_AT_MUTEX = DATA_ERROR_AT_MUTEX,  /*!< unexpected internal error at mutex */
    CTRL_ERROR_AT_DB = DATA_ERROR_AT_DB,  /*!< unexpected internal error at database */
    CTRL_ERROR_DUPLICATE_ID = DATA_ERROR_DUPLICATE_ID,  /*!< unexpected internal error: an id is used twice */
    CTRL_ERROR_DUPLICATE_NAME = DATA_ERROR_DUPLICATE_NAME,  /*!< internal error: a name is used twice */
    CTRL_ERROR_VALUE_OUT_OF_RANGE = DATA_ERROR_VALUE_OUT_OF_RANGE,  /*!< unexpected internal db error: enum/integer out of range */
    
    CTRL_ERROR_NOT_YET_IMPLEMENTED = DATA_ERROR_NOT_YET_IMPLEMENTED,  /*!< internal error: function not yet implemented */
};

typedef enum ctrl_error_enum ctrl_error_t;

#endif  /* CTRL_ERROR_H */


/*
Copyright 2016-2020 Andreas Warnke

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
