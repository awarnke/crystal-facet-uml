/* File: u8_error.h; Copyright and License: see below */

#ifndef U8_ERROR_H
#define U8_ERROR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Declares a common error type base and common error flags
 */

/*!
 *  \brief error constants which explain errors which occurred in the data module
 *
 *  It is possible to bitwise-or multiple errors to collect errors over several statements,
 *  e.g. { my_err |= data_database_reader_get_classifier_by_name(...);
 *         my_err |= data_database_reader_get_diagrams_by_classifier_id(...); },
 *  and evaluate multiple errors only once at the end.
 *
 *  This enumeration defines classes of errors so that the error handling can be simplified,
 *  e.g.
 *  all U8_ERROR_MODE errors could be printed to syslog,
 *  all U8_ERROR_CORRUPT errors could terminate the program,
 *  all U8_ERROR_INPUT errors could be shown to the user,
 *  all U8_ERROR_PARAM errors could be asserted in DEBUG MODE, ignored in RELEASE mode,
 *  all U8_ERROR_ENV errors could be retried at a later point in time
 */
enum u8_error_enum {
    U8_ERROR_NONE = 0x0,      /*!< success */

    U8_ERROR_MODE = 0x01,     /*!< wrong mode: The own software module is not in the right mode for the triggering event. */
                              /*!< e.g. a function is triggered but the module was not initialized, database is read-only, */
                              /*!<      database not open, wrong diagram type to add element, function not yet implemented */
    U8_ERROR_CORRUPT = 0x02,  /*!< internal data corruption: This should not have happened, function should be terminated. */
                              /*!< e.g. an unexpected enumeration value, NULL pointer, id not in container or db, */
                              /*!<      duplicate identifiers */
    U8_ERROR_INPUT = 0x04,    /*!< unexpected input: Input from external programs or user is unexpected but reproducible. */
                              /*!< e.g. error in stream format, too long strings/arrays */
    U8_ERROR_PARAM = 0x10,    /*!< illegal parameter: The input from a caller of the own software subsystem is illegal. */
                              /*!< e.g. parameter out of range */
    U8_ERROR_ENV = 0x80,      /*!< unexpected environment: The execution environment reacts unexpectedly, */
                              /*!< possibly not reproducible. */
                              /*!< e.g. timeout, spontaneous error at reading/writing a file, disk full, */
                              /*!<      missing access rights */

    U8_ERROR_CLASS = 0xff,  /*!< mask to filter for an error class */
};

typedef enum u8_error_enum u8_error_t;

#endif  /* U8_ERROR_H */


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
