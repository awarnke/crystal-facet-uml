/* File: data_id.inl; Copyright and License: see below */

#include "trace.h"
#include "tslog.h"

static inline void data_id_init ( data_id_t *this_, data_table_t table, int64_t row_id )
{
    (*this_).table = table;
    (*this_).row_id = row_id;
}

static inline void data_id_reinit ( data_id_t *this_, data_table_t table, int64_t row_id )
{
    (*this_).table = table;
    (*this_).row_id = row_id;
}

static inline void data_id_init_void ( data_id_t *this_ )
{
    (*this_).table = DATA_TABLE_VOID;
    (*this_).row_id = -1;
}

static inline void data_id_reinit_void ( data_id_t *this_ )
{
    (*this_).table = DATA_TABLE_VOID;
    (*this_).row_id = -1;
}

static inline void data_id_destroy ( data_id_t *this_ )
{
    (*this_).table = DATA_TABLE_VOID;
    (*this_).row_id = -1;
}

static inline data_table_t data_id_get_table ( const data_id_t *this_ )
{
    return (*this_).table;
}

static inline int64_t data_id_get_row_id ( const data_id_t *this_ )
{
    return (*this_).row_id;
}

static inline void data_id_trace ( const data_id_t *this_ )
{
    TRACE_INFO( "data_id_t" );
    switch ( (*this_).table )
    {
        case DATA_TABLE_VOID:
            TRACE_INFO("- table: DATA_TABLE_VOID");
            break;
        case DATA_TABLE_CLASSIFIER:
            TRACE_INFO("- table: DATA_TABLE_CLASSIFIER");
            break;
        case DATA_TABLE_FEATURE:
            TRACE_INFO("- table: DATA_TABLE_FEATURE");
            break;
        case DATA_TABLE_RELATIONSHIP:
            TRACE_INFO("- table: DATA_TABLE_RELATIONSHIP");
            break;
        case DATA_TABLE_DIAGRAMELEMENT:
            TRACE_INFO("- table: DATA_TABLE_DIAGRAMELEMENT");
            break;
        case DATA_TABLE_DIAGRAM:
            TRACE_INFO("- table: DATA_TABLE_DIAGRAM");
            break;
        default:
            TSLOG_ERROR("- table: out of range");
            break;
    }
    TRACE_INFO_INT( "- row_id:", (*this_).row_id );
}

static inline bool data_id_equals ( const data_id_t *this_, const data_id_t *that )
{
    return ( ( DATA_ID_VOID_ID != (*this_).row_id )&&( DATA_TABLE_VOID != (*this_).table )
             &&( (*this_).row_id == (*that).row_id )&&( (*this_).table == (*that).table ) );
}

static inline bool data_id_is_valid ( const data_id_t *this_ )
{
    return (( DATA_ID_VOID_ID != (*this_).row_id )&&( DATA_TABLE_VOID != (*this_).table ));
}

static inline bool data_id_equals_id ( const data_id_t *this_, data_table_t table, int64_t row_id )
{
    return ( ( DATA_ID_VOID_ID != (*this_).row_id )&&( DATA_TABLE_VOID != (*this_).table )
    &&( (*this_).row_id == row_id )&&( (*this_).table == table ) );
}


/*
Copyright 2016-2017 Andreas Warnke

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
