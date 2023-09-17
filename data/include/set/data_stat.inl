/* File: data_stat.inl; Copyright and License: see below */

#include "utf8stringbuf/utf8stringbuf.h"
#include "u8/u8_trace.h"
#include <assert.h>

static inline void data_stat_init ( data_stat_t *this_ )
{
    assert( (int)DATA_TABLE_CLASSIFIER < (int)DATA_STAT_TABLE_MAX );
    assert( (int)DATA_TABLE_FEATURE < (int)DATA_STAT_TABLE_MAX );
    assert( (int)DATA_TABLE_RELATIONSHIP < (int)DATA_STAT_TABLE_MAX );
    assert( (int)DATA_TABLE_DIAGRAMELEMENT < (int)DATA_STAT_TABLE_MAX );
    assert( (int)DATA_TABLE_DIAGRAM < (int)DATA_STAT_TABLE_MAX );

    for ( int series_idx = 0; series_idx < DATA_STAT_SERIES_MAX; series_idx ++ )
    {
        for ( int tables_idx = 0; tables_idx < DATA_STAT_TABLE_MAX; tables_idx ++ )
        {
            (*this_).data[tables_idx][series_idx] = 0;
        }
    }
}

static inline void data_stat_destroy ( data_stat_t *this_ )
{
}

static inline uint_fast32_t data_stat_get_count ( const data_stat_t *this_,
                                                  data_stat_table_t table,
                                                  data_stat_series_t series )
{
    assert( table < DATA_STAT_TABLE_MAX );
    assert( series < DATA_STAT_SERIES_MAX );
    return (*this_).data[table][series];
}

static inline void data_stat_inc_count ( data_stat_t *this_,
                                         data_stat_table_t table,
                                         data_stat_series_t series )
{
    assert( table < DATA_STAT_TABLE_MAX );
    assert( series < DATA_STAT_SERIES_MAX );
    (*this_).data[table][series]++;
}

static inline void data_stat_add_count ( data_stat_t *this_,
                                         data_stat_table_t table,
                                         data_stat_series_t series,
                                         int_fast32_t increment )
{
    assert( table < DATA_STAT_TABLE_MAX );
    assert( series < DATA_STAT_SERIES_MAX );
    (*this_).data[table][series] += increment;
}

static inline void data_stat_add ( data_stat_t *this_, const data_stat_t *that )
{
    assert( that != NULL );
    for ( int series_idx = 0; series_idx < DATA_STAT_SERIES_MAX; series_idx ++ )
    {
        for ( int tables_idx = 0; tables_idx < DATA_STAT_TABLE_MAX; tables_idx ++ )
        {
            (*this_).data[tables_idx][series_idx] += (*that).data[tables_idx][series_idx];
        }
    }
}

static inline uint_fast32_t data_stat_get_series_count ( const data_stat_t *this_,
                                                         data_stat_series_t series )
{
    assert( series < DATA_STAT_SERIES_MAX );
    uint_fast32_t result = 0;
    for ( int tables_idx = 0; tables_idx < DATA_STAT_TABLE_MAX; tables_idx ++ )
    {
        result += (*this_).data[tables_idx][series];
    }
    return result;
}

static inline uint_fast32_t data_stat_get_table_count ( const data_stat_t *this_,
                                                        data_stat_table_t table )
{
    assert( table < DATA_STAT_TABLE_MAX );
    uint_fast32_t result = 0;
    for ( int series_idx = 0; series_idx < DATA_STAT_SERIES_MAX; series_idx ++ )
    {
        result += (*this_).data[table][series_idx];
    }
    return result;
}

static inline uint_fast32_t data_stat_get_total_count ( const data_stat_t *this_ )
{
    uint_fast32_t result = 0;
    for ( int series_idx = 0; series_idx < DATA_STAT_SERIES_MAX; series_idx ++ )
    {
        for ( int tables_idx = 0; tables_idx < DATA_STAT_TABLE_MAX; tables_idx ++ )
        {
            result += (*this_).data[tables_idx][series_idx];
        }
    }
    return result;
}

static void inline data_stat_reset_series( data_stat_t *this_, data_stat_series_t series )
{
    assert( series < DATA_STAT_SERIES_MAX );
    for ( int tables_idx = 0; tables_idx < DATA_STAT_TABLE_MAX; tables_idx ++ )
    {
        (*this_).data[tables_idx][series] = 0;
    }
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
