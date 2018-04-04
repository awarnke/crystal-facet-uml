/* File: data_id_pair.inl; Copyright and License: see below */

static inline void data_id_pair_init_void ( data_id_pair_t *this_ )
{
    data_id_init_void( &((*this_).primary_id) );
    data_id_init_void( &((*this_).secondary_id) );
}

static inline void data_id_pair_reinit_void ( data_id_pair_t *this_ )
{
    data_id_reinit_void( &((*this_).primary_id) );
    data_id_reinit_void( &((*this_).secondary_id) );
}

static inline void data_id_pair_init ( data_id_pair_t *this_, data_id_t primary_id, data_id_t secondary_id )
{
    (*this_).primary_id = primary_id;
    (*this_).secondary_id = secondary_id;
}

static inline void data_id_pair_reinit ( data_id_pair_t *this_, data_id_t primary_id, data_id_t secondary_id )
{
    (*this_).primary_id = primary_id;
    (*this_).secondary_id = secondary_id;
}

static inline void data_id_pair_init_by_table_and_id ( data_id_pair_t *this_,
                                                       data_table_t primary_table,
                                                       int64_t primary_row_id,
                                                       data_table_t secondary_table,
                                                       int64_t secondary_row_id )
{
    data_id_init ( &((*this_).primary_id), primary_table, primary_row_id );
    data_id_init ( &((*this_).secondary_id), secondary_table, secondary_row_id );
}

static inline void data_id_pair_reinit_by_table_and_id ( data_id_pair_t *this_,
                                                         data_table_t primary_table,
                                                         int64_t primary_row_id,
                                                         data_table_t secondary_table,
                                                         int64_t secondary_row_id )
{
    data_id_reinit ( &((*this_).primary_id), primary_table, primary_row_id );
    data_id_reinit ( &((*this_).secondary_id), secondary_table, secondary_row_id );
}

static inline void data_id_pair_destroy ( data_id_pair_t *this_ )
{
    data_id_destroy( &((*this_).primary_id) );
    data_id_destroy( &((*this_).secondary_id) );
}

static inline data_id_t data_id_pair_get_primary_id ( const data_id_pair_t *this_ )
{
    return (*this_).primary_id;
}

static inline data_id_t *data_id_pair_get_primary_id_ptr ( data_id_pair_t *this_ )
{
    return &((*this_).primary_id);
}

static inline data_id_t data_id_pair_get_secondary_id ( const data_id_pair_t *this_ )
{
    return (*this_).secondary_id;
}

static inline data_id_t *data_id_pair_get_secondary_id_ptr ( data_id_pair_t *this_ )
{
    return &((*this_).secondary_id);
}

static inline bool data_id_pair_is_valid ( const data_id_pair_t *this_ )
{
    return ( data_id_is_valid( &((*this_).primary_id) ) && data_id_is_valid( &((*this_).secondary_id) ) );
}

static inline void data_id_pair_trace ( const data_id_pair_t *this_ )
{
    data_id_trace( &((*this_).primary_id) );
    data_id_trace( &((*this_).secondary_id) );
}


/*
Copyright 2017-2018 Andreas Warnke

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
