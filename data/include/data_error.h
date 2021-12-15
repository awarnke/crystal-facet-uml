/* File: data_error.h; Copyright and License: see below */

#ifndef DATA_ERROR_H
#define DATA_ERROR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Notifies errors in the data module
 */

#include "u8_error.h"
#include "util/string/utf8error.h"

/*!
 *  \brief error constants which explain errors which occurred in the data module
 *
 *  It is possible to bitwise-or multiple errors to collect errors over several statements,
 *  e.g. { strerr |= data_database_reader_get_classifier_by_name(...);
 *         strerr |= data_database_reader_get_diagrams_by_classifier_id(...); },
 *  and evaluate multiple errors only once at the end.
 *
 *  The bitmasks are compatible with u8_error_t and utf8error_t
 */
enum data_error_enum {

    /* Origin bit patterns: */
    /* APP_SPECI = 0x000x */
    /* EMPTY     = 0x0040 */
    /* MEM_SIZE  = 0x0080 */
    /* FILE      = 0x0100 */
    /* DB_SQL    = 0x0200 */
    /* DB_STRUCT = 0x0400 */
    /* MUTEX     = 0x0800 */

    DATA_ERROR_NONE          = U8_ERROR_NONE,
    DATA_ERROR_LOGIC_ANOMALY = U8_ERROR_LOGIC_ANOMALY,
    DATA_ERROR_DUPLICATE_ID           = U8_ERROR_LOGIC_ANOMALY + 0x0001,  /*!< unexpected internal error: an id is used twice */
    DATA_ERROR_DUPLICATE_NAME         = U8_ERROR_LOGIC_ANOMALY + 0x0002,  /*!< internal error: a name is used twice */

    DATA_ERROR_LOGIC_STATE   = U8_ERROR_LOGIC_STATE,
    DATA_ERROR_NOT_YET_IMPLEMENTED    = U8_ERROR_LOGIC_STATE,  /*!< internal error: function not yet implemented */

    DATA_ERROR_LOGIC_PARAMS  = U8_ERROR_LOGIC_PARAMS,
    DATA_ERROR_LOGIC_CORRUPT = U8_ERROR_LOGIC_CORRUPT,
    DATA_ERROR_DB_STRUCTURE           = U8_ERROR_LOGIC_CORRUPT + 0x0400,  /*!< the structure of the database is corrupted */
    DATA_ERROR_AT_MUTEX               = U8_ERROR_LOGIC_CORRUPT + 0x0800,  /*!< unexpected internal error at mutex */
    DATA_ERROR_AT_DB                  = U8_ERROR_LOGIC_CORRUPT + 0x0200,  /*!< unexpected internal error at database */

    DATA_ERROR_OP_ENV_START  = U8_ERROR_OP_ENV_START,
    DATA_ERROR_OP_ENV_RUN    = U8_ERROR_OP_ENV_RUN,
    DATA_ERROR_OP_TIMEOUT    = U8_ERROR_OP_TIMEOUT,
    DATA_ERROR_OP_SYS_ACCESS = U8_ERROR_OP_SYS_ACCESS,

    DATA_ERROR_USE_MODE      = U8_ERROR_USE_MODE,
    DATA_ERROR_NO_DB                  = U8_ERROR_USE_MODE + 0x0200,  /*!< database not open/loaded */
    DATA_ERROR_OBJECT_STILL_REFERENCED = U8_ERROR_USE_MODE + 0x0400,  /*!< object cannot be deleted, it is still referenced */
    DATA_ERROR_DIAGRAM_HIDES_RELATIONSHIPS = U8_ERROR_USE_MODE + 0x0004,  /*!< the diagram type does not show the relationship type */
    DATA_ERROR_DIAGRAM_HIDES_FEATURES = U8_ERROR_USE_MODE + 0x0008,  /*!< the diagram type does not show the feature type */
    DATA_ERROR_CLASSIFIER_REFUSES_FEATURE = U8_ERROR_USE_MODE + 0x0001,  /*!< the classifier type does not allow the feature type */

    DATA_ERROR_USE_INPUT     = U8_ERROR_USE_INPUT,
    DATA_ERROR_STRING_BUFFER_EXCEEDED = UTF8ERROR_TRUNCATED,          /*!< a string does not fit to the string storage buffer */
    DATA_ERROR_ARRAY_BUFFER_EXCEEDED  = U8_ERROR_USE_INPUT + 0x0080,  /*!< a set of objects does not fit to the array storage buffer */
    DATA_ERROR_INVALID_REQUEST        = U8_ERROR_USE_INPUT + 0x0010,  /*!< function call not allowed or parameters wrong */
    DATA_ERROR_INPUT_EMPTY            = U8_ERROR_USE_INPUT + 0x0040,  /*!< input parameter is empty or void */
    DATA_ERROR_LEXICAL_STRUCTURE      = U8_ERROR_USE_INPUT + 0x0001,  /*!< the lexical structure of the input-string is corrupted */
                                                                      /*!< (contains invalid tokens) */
    DATA_ERROR_PARSER_STRUCTURE       = U8_ERROR_USE_INPUT + 0x0002,  /*!< the parser structure of the input-string is corrupted */
                                                                      /*!< (wrong order of tokens) */
    DATA_ERROR_AT_FILE_READ           = U8_ERROR_USE_INPUT + 0x0100,  /*!< unexpected error at reading a file/stream */
    DATA_ERROR_VALUE_OUT_OF_RANGE     = UTF8ERROR_OUT_OF_RANGE,       /*!< unexpected internal db error: enum/integer out of range */

    DATA_ERROR_USE_OUTPUT    = U8_ERROR_USE_OUTPUT,
    DATA_ERROR_AT_FILE_WRITE          = U8_ERROR_USE_OUTPUT + 0x0100,  /*!< unexpected error at writing a file/stream */

    DATA_ERROR_USE_ACCESS    = U8_ERROR_USE_ACCESS,
    DATA_ERROR_READ_ONLY_DB           = U8_ERROR_USE_ACCESS + 0x0200,  /*!< data cannot be changed, database is read only */

};

typedef enum data_error_enum data_error_t;

#endif  /* DATA_ERROR_H */


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
