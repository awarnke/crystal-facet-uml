/* File: ctrl_error.h; Copyright and License: see below */

#ifndef CTRL_ERROR_H
#define CTRL_ERROR_H

/* public file for the doxygen documentation: */
/*! \file */

#include "data_error.h"

/*!
 *  \brief error constants which explain errors which occurred in the ctrl module
 *
 *  \note to allow easy conversion from data_error_t, this is a superset of data_error_t.
 */
enum ctrl_error_enum {
    CTRL_ERROR_NONE = DATA_ERROR_NONE,  /*!< success */
    CTRL_ERROR_NO_DB = DATA_ERROR_NO_DB,  /*!< database not open/loaded */
    CTRL_ERROR_DB_STRUCTURE = DATA_ERROR_DB_STRUCTURE,  /*!< the structure of the database is corrupted */
    CTRL_ERROR_STRING_BUFFER_EXCEEDED = DATA_ERROR_STRING_BUFFER_EXCEEDED,  /*!< a string does not fit to the string storage buffer */
    CTRL_ERROR_ARRAY_BUFFER_EXCEEDED = DATA_ERROR_ARRAY_BUFFER_EXCEEDED,  /*!< a set ob objects does not fit to the array storage buffer */
    CTRL_ERROR_AT_MUTEX = DATA_ERROR_AT_MUTEX,  /*!< unexpected internal error at mutex */
    CTRL_ERROR_AT_DB = DATA_ERROR_AT_DB,  /*!< unexpected internal error at database */
    CTRL_ERROR_MASK = DATA_ERROR_MASK,  /*!< a mask to filter error bits after collecting possibly multiple errors */
};

typedef enum ctrl_error_enum ctrl_error_t;


#endif  /* CTRL_ERROR_H */


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
