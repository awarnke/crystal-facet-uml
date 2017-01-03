/* File: gui_file_export_format.h; Copyright and License: see below */

#ifndef GUI_FILE_EXPORT_FORMAT
#define GUI_FILE_EXPORT_FORMAT

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief defines the export formats of diagram-image/text files
 */

/*!t
 *  \brief file format of exported images/text
 */
enum gui_file_export_format_enum {
    GUI_FILE_EXPORT_FORMAT_SVG,  /*!< scalable vector graphics */
    GUI_FILE_EXPORT_FORMAT_PNG,  /*!< portable network graphics */
    GUI_FILE_EXPORT_FORMAT_TXT,  /*!< utf8 encoded text */
};

typedef enum gui_file_export_format_enum gui_file_export_format_t;

#endif  /* GUI_FILE_EXPORT_FORMAT */


/*
Copyright 2016-2017 Andreas Warnke

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
