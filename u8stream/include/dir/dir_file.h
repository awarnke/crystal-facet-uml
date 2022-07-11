/* File: universal_file_output_stream.h; Copyright and License: see below */

#ifndef DIR_FILE_H
#define DIR_FILE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief implements basic functions to handle directories nad contained files
 */

#include "u8/u8_error.h"

/*!
 *  \brief a dir_file_t is simply a string
 */
typedef const char * dir_file_t;

/*!
 *  \brief removes the file from the file system
 *
 *  \param this_ pointer to own object attributes
 *  \param filepath path identifying the file to remove
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t dir_file_remove( dir_file_t filepath );

#endif  /* DIR_FILE_H */


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
