/* File: io_file_format.c; Copyright and License: see below */

#include "io_file_format.h"
#include "u8/u8_trace.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <assert.h>

void io_file_format_to_string( io_file_format_t format_set, utf8stringbuf_t out_fileformat )
{
    U8_TRACE_BEGIN();

    int count = 0;
    utf8stringbuf_clear( out_fileformat );

    if ( ( format_set & IO_FILE_FORMAT_PDF ) != 0 )
    {
        /*utf8stringbuf_append_str( out_fileformat, (count==0)?("pdf"):(", pdf") );*/
        utf8stringbuf_append_str( out_fileformat, "pdf" );
        count ++;
    }

    if ( ( format_set & IO_FILE_FORMAT_PNG ) != 0 )
    {
        utf8stringbuf_append_str( out_fileformat, (count==0)?("png"):(", png") );
        count ++;
    }

    if ( ( format_set & IO_FILE_FORMAT_PS ) != 0 )
    {
        utf8stringbuf_append_str( out_fileformat, (count==0)?("ps"):(", ps") );
        count ++;
    }

    if ( ( format_set & IO_FILE_FORMAT_SVG ) != 0 )
    {
        utf8stringbuf_append_str( out_fileformat, (count==0)?("svg"):(", svg") );
        count ++;
    }

    if ( ( format_set & IO_FILE_FORMAT_TXT ) != 0 )
    {
        utf8stringbuf_append_str( out_fileformat, (count==0)?("txt"):(", txt") );
        count ++;
    }

    if ( ( format_set & IO_FILE_FORMAT_DOCBOOK ) != 0 )
    {
        utf8stringbuf_append_str( out_fileformat, (count==0)?("docbook"):(", docbook") );
        count ++;
    }

    if ( ( format_set & IO_FILE_FORMAT_HTML ) != 0 )
    {
        utf8stringbuf_append_str( out_fileformat, (count==0)?("html"):(", html") );
        count ++;
    }

    if ( ( format_set & IO_FILE_FORMAT_CSS ) != 0 )
    {
        utf8stringbuf_append_str( out_fileformat, (count==0)?("css"):(", css") );
        count ++;
    }

    if ( ( format_set & IO_FILE_FORMAT_JSON ) != 0 )
    {
        utf8stringbuf_append_str( out_fileformat, (count==0)?("json"):(", json") );
        count ++;
    }

    if ( ( format_set & IO_FILE_FORMAT_SCHEMA ) != 0 )
    {
        utf8stringbuf_append_str( out_fileformat, (count==0)?("schema"):(", schema") );
        count ++;
    }

    if ( ( format_set & IO_FILE_FORMAT_XMI2 ) != 0 )
    {
        utf8stringbuf_append_str( out_fileformat, (count==0)?("xmi"):(", xmi") );
        count ++;
    }

    if ( count == 0 )
    {
        utf8stringbuf_copy_str( out_fileformat, "none" );
    }

    U8_TRACE_END();
}


/*
Copyright 2019-2024 Andreas Warnke

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
