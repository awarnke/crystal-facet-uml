/* File: xml_writer.inl; Copyright and License: see below */

#include <assert.h>
#include "utf8stringbuf/utf8error.h"

static inline int txt_writer_write_plain ( txt_writer_t *this_, const char *text )
{
    assert ( NULL != text );
    assert ( NULL != (*this_).output );
    int write_err;

    const size_t text_len = strlen(text);
    write_err = universal_output_stream_write ( (*this_).output, text, text_len );

    return ( write_err );
}


/*
Copyright 2019-2023 Andreas Warnke

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
