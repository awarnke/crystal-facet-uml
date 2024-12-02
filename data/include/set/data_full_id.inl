/* File: data_full_id.inl; Copyright and License: see below */

#include <assert.h>

static inline void data_full_id_init_void ( data_full_id_t *this_ )
{
    data_id_init_void( &((*this_).primary_id) );
    data_id_init_void( &((*this_).secondary_id) );
}

static inline void data_full_id_reinit_void ( data_full_id_t *this_ )
{
    data_id_reinit_void( &((*this_).primary_id) );
    data_id_reinit_void( &((*this_).secondary_id) );
}

static inline void data_full_id_init ( data_full_id_t *this_, const data_id_t *primary_id, const data_id_t *secondary_id )
{
    assert ( ( DATA_TABLE_VOID == data_id_get_table( secondary_id ) )
             || ( DATA_TABLE_CLASSIFIER == data_id_get_table( secondary_id ) )
           );

    (*this_).primary_id = *primary_id;
    (*this_).secondary_id = *secondary_id;
}

static inline void data_full_id_reinit ( data_full_id_t *this_, const data_id_t *primary_id, const data_id_t *secondary_id )
{
    assert ( ( DATA_TABLE_VOID == data_id_get_table( secondary_id ) )
             || ( DATA_TABLE_CLASSIFIER == data_id_get_table( secondary_id ) )
           );

    (*this_).primary_id = *primary_id;
    (*this_).secondary_id = *secondary_id;
}

static inline void data_full_id_init_solo ( data_full_id_t *this_, const data_id_t *primary_id )
{
    (*this_).primary_id = *primary_id;
    data_id_init_void( &((*this_).secondary_id) );
}

static inline void data_full_id_reinit_solo ( data_full_id_t *this_, const data_id_t *primary_id )
{
    (*this_).primary_id = *primary_id;
    data_id_reinit_void( &((*this_).secondary_id) );
}

static inline void data_full_id_init_by_table_and_id ( data_full_id_t *this_,
                                                       data_table_t primary_table,
                                                       data_row_t primary_row_id,
                                                       data_table_t secondary_table,
                                                       data_row_t secondary_row_id )
{
    assert ( ( DATA_TABLE_VOID == secondary_table )
             || ( DATA_TABLE_CLASSIFIER == secondary_table )
           );

    data_id_init ( &((*this_).primary_id), primary_table, primary_row_id );
    data_id_init ( &((*this_).secondary_id), secondary_table, secondary_row_id );
}

static inline void data_full_id_reinit_by_table_and_id ( data_full_id_t *this_,
                                                         data_table_t primary_table,
                                                         data_row_t primary_row_id,
                                                         data_table_t secondary_table,
                                                         data_row_t secondary_row_id )
{
    assert ( ( DATA_TABLE_VOID == secondary_table )
             || ( DATA_TABLE_CLASSIFIER == secondary_table )
           );

    data_id_reinit ( &((*this_).primary_id), primary_table, primary_row_id );
    data_id_reinit ( &((*this_).secondary_id), secondary_table, secondary_row_id );
}

static inline void data_full_id_copy ( data_full_id_t *this_, const data_full_id_t *that )
{
    data_id_copy ( &((*this_).primary_id), &((*that).primary_id) );
    data_id_copy ( &((*this_).secondary_id), &((*that).secondary_id) );
}

static inline void data_full_id_replace ( data_full_id_t *this_, const data_full_id_t *that )
{
    data_id_replace ( &((*this_).primary_id), &((*that).primary_id) );
    data_id_replace ( &((*this_).secondary_id), &((*that).secondary_id) );
}

static inline void data_full_id_destroy ( data_full_id_t *this_ )
{
    data_id_destroy( &((*this_).primary_id) );
    data_id_destroy( &((*this_).secondary_id) );
}

static inline data_id_t data_full_id_get_primary_id ( const data_full_id_t *this_ )
{
    return (*this_).primary_id;
}

static inline const data_id_t *data_full_id_get_primary_id_const ( const data_full_id_t *this_ )
{
    return &((*this_).primary_id);
}

static inline data_id_t data_full_id_get_secondary_id ( const data_full_id_t *this_ )
{
    return (*this_).secondary_id;
}

static inline const data_id_t *data_full_id_get_secondary_id_const ( const data_full_id_t *this_ )
{
    return &((*this_).secondary_id);
}

static inline bool data_full_id_equals ( const data_full_id_t *this_, const data_full_id_t *that )
{
    return data_id_equals( &((*this_).primary_id), &((*that).primary_id) );
}

static inline bool data_full_id_is_valid ( const data_full_id_t *this_ )
{
    return data_id_is_valid( &((*this_).primary_id) );
}

static inline void data_full_id_trace ( const data_full_id_t *this_ )
{
    data_id_trace( &((*this_).primary_id) );
    data_id_trace( &((*this_).secondary_id) );
}


/*
Copyright 2017-2024 Andreas Warnke

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
