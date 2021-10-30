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
 *  It is possible to bitwise-or multiple errors to collect errors over several statements,
 *  e.g. { strerr |= data_database_reader_get_classifier_by_name(...);
 *         strerr |= data_database_reader_get_diagrams_by_classifier_id(...); },
 *  and evaluate multiple errors only once at the end.
 */
enum data_error_enum {
    DATA_ERROR_NONE = 0x0,  /*!< success */

    /* application layer errors */
    DATA_ERROR_NO_DB = 0x01,  /*!< database not open/loaded */
    DATA_ERROR_DB_STRUCTURE = 0x02,  /*!< the structure of the database is corrupted */
    DATA_ERROR_STRING_BUFFER_EXCEEDED = 0x04,  /*!< a string does not fit to the string storage buffer */
    DATA_ERROR_ARRAY_BUFFER_EXCEEDED = 0x08,  /*!< a set of objects does not fit to the array storage buffer */
    DATA_ERROR_INVALID_REQUEST = 0x10,  /*!< function call not allowed or parameters wrong */
    DATA_ERROR_INPUT_EMPTY = 0x20,  /*!< input parameter is empty or void */
    DATA_ERROR_OBJECT_STILL_REFERENCED = 0x40,  /*!< object cannot be deleted, it is still referenced */

    DATA_ERROR_LEXICAL_STRUCTURE = 0x0100,  /*!< the lexical structure of the input-string is corrupted (contains invalid tokens) */
    DATA_ERROR_PARSER_STRUCTURE = 0x0200,  /*!< the parser structure of the input-string is corrupted (wrong order of tokens) */

    DATA_ERROR_DIAGRAM_HIDES_RELATIONSHIPS = 0x1000,  /*!< the diagram type does not show the relationship type */
    DATA_ERROR_DIAGRAM_HIDES_FEATURES = 0x2000,  /*!< the diagram type does not show the feature type */
    DATA_ERROR_CLASSIFIER_REFUSES_FEATURE = 0x4000,  /*!< the classifier type does not allow the feature type */

    /* os/file system layer errors */
    DATA_ERROR_AT_FILE_READ = 0x00100000,  /*!< unexpected error at reading a file/stream */
    DATA_ERROR_AT_FILE_WRITE = 0x00200000,  /*!< unexpected error at writing a file/stream */

    /* database layer errors */
    DATA_ERROR_AT_MUTEX = 0x01000000,  /*!< unexpected internal error at mutex */
    DATA_ERROR_AT_DB = 0x02000000,  /*!< unexpected internal error at database */
    DATA_ERROR_DUPLICATE_ID = 0x04000000,  /*!< unexpected internal error: an id is used twice */
    DATA_ERROR_DUPLICATE_NAME = 0x08000000,  /*!< internal error: a name is used twice */
    DATA_ERROR_VALUE_OUT_OF_RANGE = 0x10000000,  /*!< unexpected internal db error: enum/integer out of range */
    DATA_ERROR_READ_ONLY_DB = 0x20000000,  /*!< data cannot be changed, database is read only */

    DATA_ERROR_NOT_YET_IMPLEMENTED = 0x40000000,  /*!< internal error: function not yet implemented */
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
