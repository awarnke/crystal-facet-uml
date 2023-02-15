/* File: data_stat.c; Copyright and License: see below */

#include "set/data_stat.h"
#include "u8/u8_trace.h"
#include <assert.h>

/*! human readable list of stat-categories for trace output */
static const char *const ( data_stat_private_series[(int)DATA_STAT_SERIES_MAX] )
    = { "CREX", "MODI", "DELE", "IGNO", "WARN", "ERR_" };

void data_stat_trace ( const data_stat_t *this_ )
{
    U8_TRACE_BEGIN();
    U8_TRACE_INFO( "                   ? C F R E D" );
    for ( int series_idx = 0; series_idx < DATA_STAT_SERIES_MAX; series_idx ++ )
    {
        const char prefix[] = "data_stat_t[";
        const char infix1[] = "]: ";
        char stat_buf[sizeof(prefix)-1+4+sizeof(infix1)-1+3+5*DATA_STAT_TABLES_MAX];
        utf8stringbuf_t stat_str = UTF8STRINGBUF( stat_buf );
        utf8stringbuf_copy_str( stat_str, prefix );
        utf8stringbuf_append_str( stat_str, data_stat_private_series[series_idx] );
        utf8stringbuf_append_str( stat_str, infix1 );

        for ( int tables_idx = 0; tables_idx < DATA_STAT_TABLES_MAX; tables_idx ++ )
        {
            utf8stringbuf_append_str( stat_str, (tables_idx==0)?"":"," );
            utf8stringbuf_append_int( stat_str, (*this_).data[tables_idx][series_idx] );
        }
        U8_TRACE_INFO( utf8stringbuf_get_string( stat_str ) );
    }
    U8_TRACE_END();
}


/*
Copyright 2020-2023 Andreas Warnke

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
