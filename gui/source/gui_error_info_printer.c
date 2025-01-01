/* File: gui_error_info_printer.c; Copyright and License: see below */

#include "gui_error_info_printer.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

void gui_error_info_printer_init ( gui_error_info_printer_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).dummy = 0;  /* prevent warnings on uninitialized usage */

    U8_TRACE_END();
}

void gui_error_info_printer_destroy ( gui_error_info_printer_t *this_ )
{
    U8_TRACE_BEGIN();

    U8_TRACE_END();
}

u8_error_t gui_error_info_printer_show_error_info ( gui_error_info_printer_t *this_,
                                                    const u8_error_info_t *err_info,
                                                    utf8stringbuf_t out_buf )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    /* update content text: */
    switch ( u8_error_info_get_error( err_info ) )
    {
        case U8_ERROR_LEXICAL_STRUCTURE:
        {
            result |= utf8stringbuf_append_str( &out_buf, "Lexical error in input" );
        }
        break;

        case U8_ERROR_PARSER_STRUCTURE:
        {
            result |= utf8stringbuf_append_str( &out_buf, "Parser error in input" );
        }
        break;

        case U8_ERROR_STRING_BUFFER_EXCEEDED:
        {
            result |= utf8stringbuf_append_str( &out_buf, "String too long in input" );
        }
        break;

        case U8_ERROR_VALUE_OUT_OF_RANGE:
        {
            result |= utf8stringbuf_append_str( &out_buf, "Illegal value in input" );
        }
        break;

        default:
        {
            result |= utf8stringbuf_append_str( &out_buf, "Error x" );
            result |= utf8stringbuf_append_hex( &out_buf, u8_error_info_get_error( err_info ) );
            result |= utf8stringbuf_append_str( &out_buf, " occurred" );
        }
        break;
    }

    switch ( u8_error_info_get_unit ( err_info ) )
    {
        case U8_ERROR_INFO_UNIT_LINE:
        {
            result |= utf8stringbuf_append_str( &out_buf, " at line " );
            result |= utf8stringbuf_append_int( &out_buf, u8_error_info_get_line( err_info ) );
        }
        break;

        case U8_ERROR_INFO_UNIT_NAME:
        {
            result |= utf8stringbuf_append_str( &out_buf, " at name " );
            result |= utf8stringbuf_append_str( &out_buf, u8_error_info_get_name( err_info ) );
        }
        break;

        case U8_ERROR_INFO_UNIT_LINE_NAME:
        {
            result |= utf8stringbuf_append_str( &out_buf, " at name " );
            result |= utf8stringbuf_append_str( &out_buf, u8_error_info_get_name( err_info ) );
            result |= utf8stringbuf_append_str( &out_buf, " at line " );
            result |= utf8stringbuf_append_int( &out_buf, u8_error_info_get_line( err_info ) );
        }
        break;

        default:
        {
            /* no further information to add */
        }
        break;
    }

    U8_TRACE_END_ERR( result );
    return result;
}


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
