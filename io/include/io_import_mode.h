/* File: io_import_mode.h; Copyright and License: see below */

#ifndef IO_IMPORT_MODE_H
#define IO_IMPORT_MODE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the import modes for json import streams
 */

/*!
 *  \brief mode/pass of the json import
 */
enum io_import_mode_enum {
    IO_IMPORT_MODE_CHECK  = 0x00,  /*!< nothing shall be imported, just evaluate the input stream for errors */
    IO_IMPORT_MODE_PASTE  = 0x01,  /*!< the clipboard data is imported(pasted) */
    IO_IMPORT_MODE_CREATE = 0x10,  /*!< after the first pass, all objects shall exist */
    IO_IMPORT_MODE_LINK   = 0x20,  /*!< after the second pass, all objects shall linked */
};

typedef enum io_import_mode_enum io_import_mode_t;

#endif  /* IO_IMPORT_MODE_H */


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
