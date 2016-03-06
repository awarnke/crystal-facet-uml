/* File: data_error.h; Copyright and License: see below */

#ifndef DATA_ERROR_H
#define DATA_ERROR_H

/* public file for the doxygen documentation: */
/*! \file */

/*!
 *  \brief error constants which explain errors which occurred in the data module
 */
enum data_error_enum {
    DATA_ERROR_NONE = 0,  /*!< success */
    DATA_ERROR_NO_DB = -1,  /*!< database not open/loaded */
    DATA_ERROR_DB_STRUCTURE = -2,  /*!< the structure of the database is corrupted */
    DATA_ERROR_STRING_BUFFER_EXCEEDED = -5,  /*!< a string does not fit to the string storage buffer */
    DATA_ERROR_ARRAY_BUFFER_EXCEEDED = -6,  /*!< a set ob objects does not fit to the array storage buffer */
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
