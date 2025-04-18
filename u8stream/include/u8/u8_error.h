/* File: u8/u8_error.h; Copyright and License: see below */

#ifndef U8_ERROR_H
#define U8_ERROR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Notifies generic errors as bit masks.
 *
 *  Error codes defer the decision how to handle an error to the calling function.
 */

#include "u8/u8_error_cat.h"
#include "u8/u8_error_orig.h"
#include <stdbool.h>

/*!
 *  \brief error constants which explain and categorize errors
 *
 *  It is possible to bitwise-or multiple errors to collect errors over several statements,
 *  e.g. { strerr |= data_database_reader_get_classifier_by_name(...);
 *         strerr |= data_database_reader_get_diagrams_by_classifier_id(...); },
 *  and evaluate multiple errors only once at the end.
 *
 *  The bitmasks are compatible with utf8error_t
 */
enum u8_error_enum {
    U8_ERROR_NONE          = U8_ERROR_CAT_NONE,

    /* section U8_ERROR_CAT_LOGIC_ANOMALY */
    U8_ERROR_DUPLICATE              = U8_ERROR_CAT_LOGIC_ANOMALY + U8_ERROR_ORIG_DATA + 0x01,
                                      /*!< internal error: an entity exists twice */
    U8_ERROR_DUPLICATE_ID           = U8_ERROR_CAT_LOGIC_ANOMALY + U8_ERROR_ORIG_DATA + 0x03,
                                      /*!< internal error: an id is used twice */
    U8_ERROR_DUPLICATE_NAME         = U8_ERROR_CAT_LOGIC_ANOMALY + U8_ERROR_ORIG_DATA + 0x05,
                                      /*!< internal error: a name is used twice */
    U8_ERROR_DUPLICATE_UUID         = U8_ERROR_CAT_LOGIC_ANOMALY + U8_ERROR_ORIG_DATA + 0x09,
                                      /*!< internal error: an uuid is used twice */
    U8_ERROR_EDGE_CASE_PARAM        = U8_ERROR_CAT_LOGIC_ANOMALY + U8_ERROR_ORIG_DATA + 0x10,
                                      /*!< internal error: a parameter is a valid edge case */
                                      /*!<                 that cannot be handled by the called function */
    U8_ERROR_NOT_FOUND              = U8_ERROR_CAT_LOGIC_ANOMALY + U8_ERROR_ORIG_MISS + 0x01,
                                      /*!< internal error: requested data not found */
    U8_ERROR_END_OF_STREAM          = U8_ERROR_CAT_LOGIC_ANOMALY + U8_ERROR_ORIG_NETW + 0x01,
                                      /*!< internal error: a file or stream is unexpectedly at the end */
    U8_ERROR_FILE_ALREADY_REMOVED   = U8_ERROR_CAT_LOGIC_ANOMALY + U8_ERROR_ORIG_FILE + 0x01,
                                      /*!< internal error: a file was already removed */

    /* section U8_ERROR_CAT_LOGIC_STATE */
    U8_ERROR_CONFIG_OUT_OF_RANGE    = U8_ERROR_CAT_LOGIC_STATE + U8_ERROR_ORIG_DATA + 0x01,
                                      /*!< a configuration/settings-value is out of range */
    U8_ERROR_WRONG_STATE            = U8_ERROR_CAT_LOGIC_STATE + U8_ERROR_ORIG_DATA + 0x02,
                                      /*!< a dynamically changing state is out of range */
    U8_ERROR_NOT_YET_IMPLEMENTED    = U8_ERROR_CAT_LOGIC_STATE   + U8_ERROR_ORIG_MISS + 0x10,
                                      /*!< internal error: function not yet implemented */

    /* section U8_ERROR_CAT_LOGIC_PARAMS */
    U8_ERROR_PARAM_MISSING          = U8_ERROR_CAT_LOGIC_PARAMS + U8_ERROR_ORIG_MISS + 0x01,
                                      /*!< the provided parameter is null or empty */
    U8_ERROR_PARAM_OUT_OF_RANGE     = U8_ERROR_CAT_LOGIC_PARAMS + U8_ERROR_ORIG_DATA + 0x01,
                                      /*!< the provided parameters are out of range */

    /* section U8_ERROR_CAT_LOGIC_CORRUPT */
    U8_ERROR_AT_MUTEX               = U8_ERROR_CAT_LOGIC_CORRUPT + U8_ERROR_ORIG_SYNC + 0x01,
                                      /*!< unexpected internal error at mutex */
    U8_ERROR_DB_STRUCTURE           = U8_ERROR_CAT_LOGIC_CORRUPT + U8_ERROR_ORIG_DATA + 0x01,
                                      /*!< the structure of the database is corrupted */
    U8_ERROR_AT_DB                  = U8_ERROR_CAT_LOGIC_CORRUPT + U8_ERROR_ORIG_DATA + 0x02,
                                      /*!< unexpected internal error at database */

    /* section U8_ERROR_CAT_OP_ENV_START */

    /* section U8_ERROR_CAT_OP_ENV_RUN */
    U8_ERROR_LIB_NO_MEMORY           = U8_ERROR_CAT_OP_ENV_RUN + U8_ERROR_ORIG_MEMO + 0x01,
                                      /*!< a library call failed due to insufficient memory */
    U8_ERROR_LIB_FILE_WRITE          = U8_ERROR_CAT_OP_ENV_RUN + U8_ERROR_ORIG_FILE + 0x01,
                                      /*!< a library call failed due to file write error */

    /* section U8_ERROR_CAT_OP_TIMEOUT */

    /* section U8_ERROR_CAT_OP_SYS_ACCESS */

    /* section U8_ERROR_CAT_USE_MODE */
    U8_ERROR_NO_DB                  = U8_ERROR_CAT_USE_MODE      + U8_ERROR_ORIG_SYNC + 0x01,
                                      /*!< database not open/loaded */
    U8_ERROR_OBJECT_STILL_REFERENCED = U8_ERROR_CAT_USE_MODE     + U8_ERROR_ORIG_DATA + 0x01,
                                      /*!< object cannot be deleted, it is still referenced */
    U8_ERROR_DIAGRAM_HIDES_RELATIONSHIPS = U8_ERROR_CAT_USE_MODE + U8_ERROR_ORIG_DATA + 0x02,
                                      /*!< the diagram type does not show the relationship type */
    U8_ERROR_DIAGRAM_HIDES_FEATURES = U8_ERROR_CAT_USE_MODE      + U8_ERROR_ORIG_DATA + 0x04,
                                      /*!< the diagram type does not show the feature type */
    U8_ERROR_CLASSIFIER_REFUSES_FEATURE = U8_ERROR_CAT_USE_MODE  + U8_ERROR_ORIG_DATA + 0x08,
                                      /*!< the classifier type does not allow the feature type */
    U8_ERROR_FOCUS_EMPTY            = U8_ERROR_CAT_USE_MODE      + U8_ERROR_ORIG_MISS + 0x01,
                                      /*!< no focused diagram or focused object */

    /* section U8_ERROR_CAT_USE_INPUT */
    U8_ERROR_STRING_BUFFER_EXCEEDED = U8_ERROR_CAT_USE_INPUT     + U8_ERROR_ORIG_MEMO + 0x01,
                                      /*!< a string does not fit to the string storage buffer */
    U8_ERROR_ARRAY_BUFFER_EXCEEDED  = U8_ERROR_CAT_USE_INPUT     + U8_ERROR_ORIG_MEMO + 0x02,
                                      /*!< a set of objects does not fit to the array storage buffer */
    U8_ERROR_INPUT_EMPTY            = U8_ERROR_CAT_USE_INPUT     + U8_ERROR_ORIG_MISS + 0x01,
                                      /*!< input parameter is empty or void */
    U8_ERROR_INVALID_REQUEST        = U8_ERROR_CAT_USE_INPUT     + U8_ERROR_ORIG_DATA + 0x01,
                                      /*!< function call not allowed or parameters wrong */
    U8_ERROR_VALUE_OUT_OF_RANGE     = U8_ERROR_CAT_USE_INPUT     + U8_ERROR_ORIG_DATA + 0x02,
                                      /*!< unexpected internal db error: enum/integer out of range */
    U8_ERROR_INVALID_ENCODING       = U8_ERROR_CAT_USE_INPUT     + U8_ERROR_ORIG_DATA + 0x04,
                                      /*!< the encoding of the input-string contains illegal bytes */
                                      /*!< (contains invalid tokens) */
    U8_ERROR_LEXICAL_STRUCTURE      = U8_ERROR_CAT_USE_INPUT     + U8_ERROR_ORIG_DATA + 0x08,
                                      /*!< the lexical structure of the input-string is corrupted */
                                      /*!< (contains invalid tokens) */
    U8_ERROR_PARSER_STRUCTURE       = U8_ERROR_CAT_USE_INPUT     + U8_ERROR_ORIG_DATA + 0x10,
                                      /*!< the parser structure of the input-string is corrupted */
                                      /*!< (wrong order of tokens) */
    U8_ERROR_AT_FILE_READ           = U8_ERROR_CAT_USE_INPUT     + U8_ERROR_ORIG_FILE + 0x01,
                                      /*!< unexpected error at reading a file/stream */

    /* section U8_ERROR_CAT_USE_OUTPUT */
    U8_ERROR_AT_FILE_WRITE          = U8_ERROR_CAT_USE_OUTPUT    + U8_ERROR_ORIG_FILE + 0x01,
                                      /*!< unexpected error at writing a file/stream */

    /* section U8_ERROR_CAT_USE_ACCESS */
    U8_ERROR_READ_ONLY_DB           = U8_ERROR_CAT_USE_ACCESS    + U8_ERROR_ORIG_DATA + 0x01,
                                      /*!< data cannot be changed, database is read only */

};

typedef enum u8_error_enum u8_error_t;

/*!
 *  \brief checks if this_ error bitmask mask contains all bits of that error to compare
 *
 *  The inner logic is trivial,
 *  the name of the function shall increase understandability of the outer logic when used.
 *
 *  \param this_ own object
 *  \param that object (error mask) to compare
 *  \return true if all bits of that are set in this_
 */
static inline bool u8_error_contains ( const u8_error_t this_,  const u8_error_t that );

/*!
 *  \brief checks if this_ error bitmask mask contains other bits than that error to compare
 *
 *  The inner logic is trivial,
 *  the name of the function shall increase understandability of the outer logic when used.
 *
 *  \param this_ own object
 *  \param that object (error mask) to compare
 *  \return true if this_ has bit set that are not in that
 */
static inline bool u8_error_more_than ( const u8_error_t this_,  const u8_error_t that );

#include "u8/u8_error.inl"

#endif  /* U8_ERROR_H */


/*
Copyright 2016-2025 Andreas Warnke

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
