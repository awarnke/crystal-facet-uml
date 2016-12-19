/* File: data_error.h; Copyright and License: see below */

#ifndef DATA_ERROR_H
#define DATA_ERROR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Notifies errors in the data module
 */

/*!
 *  \brief error constants which explain errors which occurred in the data module
 *
 *  To be able to collect multiple errors over several statements,
 *  and evaluate multiple errors only once at the end of a block, the pattern for values is
 *  0xff000020 where all unused higher bits are set and only one of the lower 6*4=24 bits is set.
 *
 */
enum data_error_enum {
    DATA_ERROR_NONE = (0x00),  /*!< 0x0: success */
    DATA_ERROR_NO_DB = (~(0x00fffffe)),  /*!< 0x000001: database not open/loaded */
    DATA_ERROR_DB_STRUCTURE = (~(0x00fffffd)),  /*!< 0x000002: the structure of the database is corrupted */
    DATA_ERROR_STRING_BUFFER_EXCEEDED = (~(0x00fffffb)),  /*!< 0x000004: a string does not fit to the string storage buffer */
    DATA_ERROR_ARRAY_BUFFER_EXCEEDED = (~(0x00fffff7)),  /*!< 0x000008: a set ob objects does not fit to the array storage buffer */
    DATA_ERROR_INVALID_REQUEST = (~(0x00ffffef)),  /*!< 0x000010: function call not allowed or parameters wrong */
    DATA_ERROR_INPUT_EMPTY = (~(0x00ffffdf)),  /*!< 0x000020: input parameter is empty or void */
    DATA_ERROR_OBJECT_STILL_REFERENCED = (~(0x00ffffbf)),  /*!< 0x000040: object cannot be deleted, it is still referenced */
    DATA_ERROR_LEXICAL_STRUCTURE = (~(0x00ffff7f)),  /*!< 0x000080: the lexical structure of the input-string is corrupted */
    DATA_ERROR_PARSER_STRUCTURE = (~(0x00fffeff)),  /*!< 0x000100: the parser structure of the input-string is corrupted */
    DATA_ERROR_AT_MUTEX = (~(0x00feffff)),  /*!< 0x010000: unexpected internal error at mutex */
    DATA_ERROR_AT_DB = (~(0x00fdffff)),  /*!< 0x020000: unexpected internal error at database */
    DATA_ERROR_DUPLICATE_ID = (~(0x00fbffff)),  /*!< 0x040000: unexpected internal error: an id is used twice */
    DATA_ERROR_DUPLICATE_NAME = (~(0x00f7ffff)),  /*!< 0x080000: internal error: a name is used twice */
    DATA_ERROR_VALUE_OUT_OF_RANGE = (~(0x00efffff)),  /*!< 0x100000: unexpected internal error: enum/integer out of range */
    DATA_ERROR_NOT_YET_IMPLEMENTED = (~(0x007fffff)),  /*!< 0x800000: internal error: function not yet implemented */
    DATA_ERROR_MASK = (0x00ffffff),  /*!< 0xffffff: a mask to filter error bits after collecting possibly multiple errors */
};

typedef enum data_error_enum data_error_t;

#endif  /* DATA_ERROR_H */


/*
Copyright 2016-2016 Andreas Warnke

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
