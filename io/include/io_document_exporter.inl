/* File: io_document_exporter.inl; Copyright and License: see below */

#include <assert.h>
#include "util/string/utf8error.h"

static inline int io_document_exporter_private_write_plain ( io_document_exporter_t *this_, 
                                                             const char *text, 
                                                             FILE *output )
{
    assert ( NULL != text );
    assert ( NULL != output );
    size_t text_len = strlen(text);
    size_t out_count;
    out_count = fwrite( text, 1 /* size of char */, text_len, output );
    return ( (out_count != text_len) ? -1 : 0 );
}

static inline int io_document_exporter_private_write_xml_enc ( io_document_exporter_t *this_, 
                                                               const char *text, 
                                                               FILE *output )
{
    assert ( NULL != text );
    assert ( NULL != output );
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).temp_output, text );
    strerr |= utf8stringbuf_replace_all( (*this_).temp_output, (*this_).xml_encode_table );
    size_t text_len = utf8stringbuf_get_length( (*this_).temp_output );
    size_t out_count;
    out_count = fwrite( utf8stringbuf_get_string( (*this_).temp_output ), 1 /* size of char */, text_len, output );
    return ( ((out_count != text_len)||( UTF8ERROR_SUCCESS != strerr)) ? -1 : 0 );
}


/*
Copyright 2019-2019 Andreas Warnke

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
