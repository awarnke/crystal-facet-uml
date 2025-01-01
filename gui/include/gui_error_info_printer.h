/* File: gui_error_info_printer.h; Copyright and License: see below */

#ifndef GUI_ERROR_INFO_PRINTER_H
#define GUI_ERROR_INFO_PRINTER_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Translates an u8_error_info_t struct to an english message
 */

#include "utf8stringbuf/utf8stringbuf.h"
#include "u8/u8_error_info.h"
#include "u8/u8_error.h"

/*!
 *  \brief attributes of the gui_error_info_printer
 */
struct gui_error_info_printer_struct {
    int dummy;  /*!< a struct must not be empty */
};

typedef struct gui_error_info_printer_struct gui_error_info_printer_t;

/*!
 *  \brief initializes the gui_error_info_printer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_error_info_printer_init ( gui_error_info_printer_t *this_ );

/*!
 *  \brief destroys the gui_error_info_printer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_error_info_printer_destroy ( gui_error_info_printer_t *this_ );

/*!
 *  \brief prints an u8_error_info_t as english message
 *
 *  \param this_ pointer to own object attributes
 *  \param err_info the u8_error_info_t to be printed
 *  \param out_buf the output string buffer to which to append the error message
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t gui_error_info_printer_show_error_info ( gui_error_info_printer_t *this_,
                                                    const u8_error_info_t *err_info,
                                                    utf8stringbuf_t out_buf
                                                  );

#endif  /* GUI_ERROR_INFO_PRINTER_H */


/*
Copyright 2022-2025 Andreas Warnke

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
