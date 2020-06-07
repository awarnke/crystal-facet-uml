/* File: io_file_format.h; Copyright and License: see below */

#ifndef IO_FILE_FORMAT
#define IO_FILE_FORMAT

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the export formats of diagram-image/text files
 */

#include "util/string/utf8stringbuf.h"

/*!
 *  \brief file format of exported images/text
 */
enum io_file_format_enum {
    IO_FILE_FORMAT_NONE= 0x0000,  /*!< no format selected */
    IO_FILE_FORMAT_SVG = 0x0001,  /*!< scalable vector graphics */
    IO_FILE_FORMAT_PDF = 0x0002,  /*!< portable document format */
    IO_FILE_FORMAT_PS  = 0x0004,  /*!< postscript */
    IO_FILE_FORMAT_PNG = 0x0008,  /*!< portable network graphics */
    IO_FILE_FORMAT_TXT = 0x0010,  /*!< utf8 encoded text */
    IO_FILE_FORMAT_DOCBOOK = 0x0100,  /*!< docbook */
    IO_FILE_FORMAT_XHTML = 0x0200,  /*!< xhtml */
    IO_FILE_FORMAT_CSS = 0x0400,  /*!< css for xhtml */
    IO_FILE_FORMAT_XMI2 = 0x1000,  /*!< xmi version 2.5.1 */
};

typedef enum io_file_format_enum io_file_format_t;

void io_file_format_to_string( io_file_format_t format_set, utf8stringbuf_t out_fileformat );

#endif  /* IO_FILE_FORMAT */


/*
Copyright 2016-2020 Andreas Warnke

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
