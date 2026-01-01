/* File: io_file_format.h; Copyright and License: see below */

#ifndef IO_FILE_FORMAT_H
#define IO_FILE_FORMAT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the export formats of diagram-image/text files
 */

#include "utf8stringbuf/utf8stringbuf.h"

/*!
 *  \brief file format of exported images/text
 */
enum io_file_format_enum {
    IO_FILE_FORMAT_NONE    = 0x00000,  /*!< no format selected */
    IO_FILE_FORMAT_SVG     = 0x00001,  /*!< scalable vector graphics */
    IO_FILE_FORMAT_PDF     = 0x00002,  /*!< portable document format */
    IO_FILE_FORMAT_PS      = 0x00004,  /*!< postscript */
    IO_FILE_FORMAT_PNG     = 0x00008,  /*!< portable network graphics */
    IO_FILE_FORMAT_TXT     = 0x00010,  /*!< utf8 encoded text */
    IO_FILE_FORMAT_DOCBOOK = 0x00100,  /*!< docbook */
    IO_FILE_FORMAT_HTML   = 0x00200,  /*!< html */
    IO_FILE_FORMAT_CSS     = 0x00400,  /*!< css for html */
    IO_FILE_FORMAT_XMI2    = 0x01000,  /*!< xmi version 2.5.1 */
    IO_FILE_FORMAT_JSON    = 0x10000,  /*!< json; json-ld: https://www.w3.org/TR/json-ld/ */
    IO_FILE_FORMAT_SCHEMA  = 0x20000,  /*!< json schema of Internet Engineering Task Force */
};

typedef enum io_file_format_enum io_file_format_t;

void io_file_format_to_string( io_file_format_t format_set, utf8stringbuf_t out_fileformat );

#endif  /* IO_FILE_FORMAT_H */


/*
Copyright 2016-2026 Andreas Warnke

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
