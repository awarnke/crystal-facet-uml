/* File: u8_error_info.h; Copyright and License: see below */

#ifndef U8_ERROR_INFO_H
#define U8_ERROR_INFO_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a struct defining detailed error information
 */

#include "u8/u8_error.h"
#include "u8/u8_error_info_unit.h"

/*!
 *  \brief constants for max string sizes
 */
enum u8_error_info_max_enum {
    U8_ERROR_INFO_MAX_NAME_SIZE = 48,
};

/*!
 *  \brief struct of error code and additional information
 *
 *  This struct is intended to be provided as out-parameter (pointer-to-struct) to a function call.
 *
 *  In case of an error, the function shall return an error code and optionally fill in the u8_error_info_struct.
 *  There is no need that return error code and error info refer to the same error code, especially since
 *  the return error may be a bitwise-or of several errors whereas the error info struct refers to exactly one error.
 */
struct u8_error_info_struct {
    u8_error_t error;
    u8_error_info_unit_t unit;
    int32_t position;
    char name[U8_ERROR_INFO_MAX_NAME_SIZE];
};

typedef struct u8_error_info_struct u8_error_info_t;

/*!
 *  \brief initializes the u8_error_info_t struct to no error
 *
 *  \param this_ pointer to own object attributes
 */
static inline void u8_error_info_init_void ( u8_error_info_t *this_ );

/*!
 *  \brief initializes the u8_error_info_t struct with an error code
 *
 *  \param this_ pointer to own object attributes
 *  \param error error code to set
 */
static inline void u8_error_info_init ( u8_error_info_t *this_, u8_error_t error );

/*!
 *  \brief initializes the u8_error_info_t struct with an error code and a position
 *
 *  \param this_ pointer to own object attributes
 *  \param error error code to set
 *  \param line line position of origin
 */
static inline void u8_error_info_init_line ( u8_error_info_t *this_, u8_error_t error, int32_t line );

/*!
 *  \brief initializes the u8_error_info_t struct with an error code and a position and a name
 *
 *  \param this_ pointer to own object attributes
 *  \param error error code to set
 *  \param name name of origin-object
 */
static inline void u8_error_info_init_name ( u8_error_info_t *this_, u8_error_t error, const char* name );

/*!
 *  \brief initializes the u8_error_info_t struct with an error code and a position and a name
 *
 *  \param this_ pointer to own object attributes
 *  \param error error code to set
 *  \param line line position of origin
 *  \param name name of origin-object
 */
static inline void u8_error_info_init_line_name ( u8_error_info_t *this_, u8_error_t error, int32_t line, const char* name );

/*!
 *  \brief destroys the u8_error_info_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void u8_error_info_destroy ( u8_error_info_t *this_ );

/*!
 *  \brief gets the error code
 *
 *  \param this_ pointer to own object attributes
 *  \return the error code
 */
static inline u8_error_t u8_error_info_get_error ( const u8_error_info_t *this_ );

/*!
 *  \brief gets the error unit
 *
 *  \param this_ pointer to own object attributes
 *  \return the semantics and unit of additional data
 */
static inline u8_error_info_unit_t u8_error_info_get_unit ( const u8_error_info_t *this_ );

/*!
 *  \brief gets the error position
 *
 *  \param this_ pointer to own object attributes
 *  \return the origin position of the error
 */
static inline int32_t u8_error_info_get_position ( const u8_error_info_t *this_ );

/*!
 *  \brief gets the error-origin name
 *
 *  \param this_ pointer to own object attributes
 *  \return the object name at the error origin
 */
static inline const char* u8_error_info_get_name ( const u8_error_info_t *this_ );

#include "u8/u8_error_info.inl"

#endif  /* U8_ERROR_INFO_H */


/*
Copyright 2022-2022 Andreas Warnke

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
