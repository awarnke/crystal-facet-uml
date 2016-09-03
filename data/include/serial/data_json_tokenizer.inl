/* File: data_json_tokenizer.inl; Copyright and License: see below */

#include "serial/data_json_constants.h"
#include <assert.h>

static inline void data_json_tokenizer_private_skip_whitespace ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos )
{
    assert( NULL != in_data );
    assert( NULL != io_read_pos );

    bool ws_end_reached = false;
    uint32_t pos;
    for ( pos = *io_read_pos; ( ! ws_end_reached ) && ( pos < DATA_JSON_TOKENIZER_MAX_INPUT_SIZE ); pos ++ )
    {
        char current = in_data[pos];
        if ( ( DATA_JSON_CONSTANTS_CHAR_NL != current )
            && ( DATA_JSON_CONSTANTS_CHAR_CR != current )
            && ( DATA_JSON_CONSTANTS_CHAR_TAB != current )
            && ( DATA_JSON_CONSTANTS_CHAR_SPACE != current ))
        {
            ws_end_reached = true;
        }
    }
    *io_read_pos = (pos-1);
}


/*
Copyright 2016-2016 Andreas Warnke

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
