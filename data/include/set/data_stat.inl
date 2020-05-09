/* File: data_stat.inl; Copyright and License: see below */

#include "trace.h"
#include <assert.h>

static inline void data_stat_init ( data_stat_t *this_ )
{
    assert( (int)DATA_TABLE_CLASSIFIER < (int)DATA_STAT_TABLES_MAX );
    assert( (int)DATA_TABLE_FEATURE < (int)DATA_STAT_TABLES_MAX );
    assert( (int)DATA_TABLE_RELATIONSHIP < (int)DATA_STAT_TABLES_MAX );
    assert( (int)DATA_TABLE_DIAGRAMELEMENT < (int)DATA_STAT_TABLES_MAX );
    assert( (int)DATA_TABLE_DIAGRAM < (int)DATA_STAT_TABLES_MAX );

    for ( int series_idx = 0; series_idx < DATA_STAT_SERIES_MAX; series_idx ++ )
    {
        for ( int tables_idx = 0; tables_idx < DATA_STAT_TABLES_MAX; tables_idx ++ )
        {
            (*this_)[tables_idx][series_idx] = 0;
        }
    }
}

static inline void data_stat_destroy ( data_stat_t *this_ )
{
}

static inline uint_fast32_t data_stat_get_count ( const data_stat_t *this_,
                                                  data_stat_series_t series,
                                                  data_table_t table )
{
    return (*this_)[table][series];
}

static inline void data_stat_inc_count ( data_stat_t *this_,
                                         data_stat_series_t series,
                                         data_table_t table )
{
    (*this_)[table][series]++;
}

static inline void data_stat_trace ( const data_stat_t *this_ )
{
    for ( int series_idx = 0; series_idx < DATA_STAT_SERIES_MAX; series_idx ++ )
    {
        TRACE_INFO_INT( "- series_idx:", series_idx );
        for ( int tables_idx = 0; tables_idx < DATA_STAT_TABLES_MAX; tables_idx ++ )
        {
            TRACE_INFO_INT_INT( "- tables_idx, count:", tables_idx, (*this_)[tables_idx][series_idx] );
        }
    }
}


/*
Copyright 2020-2020 Andreas Warnke

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
