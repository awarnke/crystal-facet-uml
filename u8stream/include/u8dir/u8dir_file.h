/* File: u8dir_file.h; Copyright and License: see below */

#ifndef U8DIR_FILE_H
#define U8DIR_FILE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief implements basic functions to handle directories and contained files
 */

#include "u8/u8_error.h"
#include <errno.h>

/*!
 *  \brief a u8dir_file_t is simply a string identifying a file
 */
typedef const char * u8dir_file_t;

/*!
 *  \brief removes the file from the file system
 *
 *  It is recommended to use this function even if it is unclear if this program can remove this_ file to ensure that
 *  time-of-check is time-of-use (TOCTOU).
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success,
 *          U8_ERROR_FILE_ALREADY_REMOVED if file was not there,
 *          U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t u8dir_file_remove( u8dir_file_t this_ );

#endif  /* U8DIR_FILE_H */


/*
Copyright 2022-2024 Andreas Warnke

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
