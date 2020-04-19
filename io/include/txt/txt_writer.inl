/* File: xml_writer.inl; Copyright and License: see below */

#include <assert.h>
#include "util/string/utf8error.h"

static inline int txt_writer_write_plain ( txt_writer_t *this_, const char *text )
{
    int export_err = 0;
    assert ( NULL != text );
    assert ( NULL != (*this_).output );

    const size_t text_len = strlen(text);
    size_t out_count;
    out_count = fwrite( text, 1 /* size of char */, text_len, (*this_).output );
    export_err = (out_count != text_len) ? -1 : 0;

    return ( export_err );
}


/*
Copyright 2019-2020 Andreas Warnke

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
