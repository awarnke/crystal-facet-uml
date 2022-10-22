/* File: u8_error_cat.h; Copyright and License: see below */

#ifndef U8_ERROR_CAT_H
#define U8_ERROR_CAT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Declares a common error base type and common error category flags
 */

/*!
 *  \brief error constants which define error categories
 *
 *  It is possible to bitwise-or multiple errors to collect errors over several statements,
 *  e.g. { my_err |= data_database_reader_get_classifier_by_name(...);
 *         my_err |= data_database_reader_get_diagrams_by_classifier_id(...); },
 *  and evaluate multiple errors only once at the end.
 *
 *  This enumeration defines classes of errors so that the error handling can be simplified,
 *  e.g.
 *  all U8_ERROR_CAT_USE_* errors could be shown to the user,
 *  all U8_ERROR_CAT_LOGIC_* errors could terminate the program,
 *  all U8_ERROR_CAT_OP_ENV_RUN errors could be retried at a later point in time
 */
enum u8_error_cat_enum {

    U8_ERROR_CAT_NONE          = 0x00000000,  /*!< success */

    U8_ERROR_CAT_LOGIC_ANOMALY =-2147483648,  /*!< CAUSE:    Unexpected result of function */
                                              /*!< REACTION: TRACE, handle by caller */
                                              /*!< EXAMPLE:  key in set not found, file was already deleted */
    U8_ERROR_CAT_LOGIC_STATE   = 0x40000000,  /*!< CAUSE:    Wrong internal state: */
                                              /*!<           The own software module is not in the right state */
                                              /*!<           for the triggering event. */
                                              /*!< REACTION: TERMINATE, notify developer */
                                              /*!< EXAMPLE:  A function is triggered but the module was not initialized before; */
                                              /*!<           The function not yet implemented */
    U8_ERROR_CAT_LOGIC_PARAMS  = 0x20000000,  /*!< CAUSE:    Illegal parameter: */
                                              /*!<           The input from a caller of the _own_ software subsystem is illegal. */
                                              /*!< REACTION: TERMINATE, notify developer */
                                              /*!< EXAMPLE:  Parameter out of range */
    U8_ERROR_CAT_LOGIC_CORRUPT = 0x10000000,  /*!< CAUSE:    Internal data/program corruption: */
                                              /*!<           Temporary or persisted data is inconsistent */
                                              /*!< REACTION: TERMINATE, notify developer */
                                              /*!< EXAMPLE:  An unexpected enumeration value or a NULL pointer; */
                                              /*!<           An ID is not contained in a set; */
                                              /*!<           Same (duplicate) identifier for different objects; */
                                              /*!<           A mutex is not released; */
    U8_ERROR_CAT_OP_ENV_START  = 0x08000000,  /*!< CAUSE:    Unexpected environment at system start: */
                                              /*!< REACTION: CANCEL START, notify operator */
                                              /*!< EXAMPLE:  Not enough memory available; */
                                              /*!<           network unreachable. */
    U8_ERROR_CAT_OP_ENV_RUN    = 0x04000000,  /*!< CAUSE:    Unexpected environment: */
                                              /*!<           The execution environment reacts unexpectedly, */
                                              /*!<           possibly not reproducible. */
                                              /*!< REACTION: CONTINUE, notify operator */
                                              /*!< EXAMPLE:  Spontaneous error at reading/writing a file, disk full, */
                                              /*!<           unreliable network connectivity. */
    U8_ERROR_CAT_OP_TIMEOUT    = 0x02000000,  /*!< CAUSE:    Missing response/reaction from communication partner */
                                              /*!< REACTION: CONTINUE, notify operator */
                                              /*!< EXAMPLE:  A remote prodedure call times out; */
                                              /*!<           A callback is not called; */
    U8_ERROR_CAT_OP_SYS_ACCESS = 0x01000000,  /*!< CAUSE:    The system has not sufficient access rights to access resources */
                                              /*!< REACTION: CONTINUE, notify operator */
                                              /*!< EXAMPLE:  Mounting a disk is not permitted; */
                                              /*!<           Accessing network is prohibited. */
    U8_ERROR_CAT_USE_MODE      = 0x00800000,  /*!< CAUSE:    Wrong mode: */
                                              /*!<           The own software module is not in the right state */
                                              /*!<           for the requested operation. */
                                              /*!< REACTION: CONTINUE, notify user */
                                              /*!< EXAMPLE:  database not open; */
                                              /*!<           Wrong diagram type to add an element. */
    U8_ERROR_CAT_USE_INPUT     = 0x00400000,  /*!< CAUSE:    Unexpected input:   */
                                              /*!<           Input from external programs or user is unexpected but reproducible. */
                                              /*!< REACTION: CONTINUE, notify user */
                                              /*!< EXAMPLE:  Error in stream format; too long strings/arrays */
    U8_ERROR_CAT_USE_OUTPUT    = 0x00200000,  /*!< CAUSE:    Output is limited or not possible */
                                              /*!< REACTION: CONTINUE, notify user */
                                              /*!< EXAMPLE:  A disk selected by the user is full. */
    U8_ERROR_CAT_USE_ACCESS    = 0x00100000,  /*!< CAUSE:    The user has not sufficient access rights */
                                              /*!<           for the requested operation. */
                                              /*!< REACTION: CONTINUE, notify user */
                                              /*!< EXAMPLE:  A database that was selected by the user is read-only. */

};

typedef enum u8_error_cat_enum u8_error_cat_t;

#endif  /* U8_ERROR_CAT_H */


/*
Copyright 2016-2022 Andreas Warnke

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
