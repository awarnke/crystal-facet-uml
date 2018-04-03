/* File: data_id_pair.inl; Copyright and License: see below */

static inline void data_id_pair_init_void ( data_id_pair_t *this_ )
{
    data_id_init_void( &((*this_).visible_id) );
    data_id_init_void( &((*this_).model_id) );
}

static inline void data_id_pair_reinit_void ( data_id_pair_t *this_ )
{
    data_id_reinit_void( &((*this_).visible_id) );
    data_id_reinit_void( &((*this_).model_id) );
}

static inline void data_id_pair_init ( data_id_pair_t *this_, data_id_t visible_id, data_id_t model_id )
{
    (*this_).visible_id = visible_id;
    (*this_).model_id = model_id;
}

static inline void data_id_pair_reinit ( data_id_pair_t *this_, data_id_t visible_id, data_id_t model_id )
{
    (*this_).visible_id = visible_id;
    (*this_).model_id = model_id;
}

static inline void data_id_pair_init_by_table_and_id ( data_id_pair_t *this_,
                                                                   data_table_t visible_table,
                                                                   int64_t visible_row_id,
                                                                   data_table_t model_table,
                                                                   int64_t model_row_id )
{
    data_id_init ( &((*this_).visible_id), visible_table, visible_row_id );
    data_id_init ( &((*this_).model_id), model_table, model_row_id );
}

static inline void data_id_pair_reinit_by_table_and_id ( data_id_pair_t *this_,
                                                                     data_table_t visible_table,
                                                                     int64_t visible_row_id,
                                                                     data_table_t model_table,
                                                                     int64_t model_row_id )
{
    data_id_reinit ( &((*this_).visible_id), visible_table, visible_row_id );
    data_id_reinit ( &((*this_).model_id), model_table, model_row_id );
}

static inline void data_id_pair_destroy ( data_id_pair_t *this_ )
{
    data_id_destroy( &((*this_).visible_id) );
    data_id_destroy( &((*this_).model_id) );
}

static inline data_id_t data_id_pair_get_visible_id ( data_id_pair_t *this_ )
{
    return (*this_).visible_id;
}

static inline data_id_t *data_id_pair_get_visible_id_ptr ( data_id_pair_t *this_ )
{
    return &((*this_).visible_id);
}

static inline data_id_t data_id_pair_get_model_id ( data_id_pair_t *this_ )
{
    return (*this_).model_id;
}

static inline data_id_t *data_id_pair_get_model_id_ptr ( data_id_pair_t *this_ )
{
    return &((*this_).model_id);
}

static inline bool data_id_pair_is_valid ( data_id_pair_t *this_ )
{
    return ( data_id_is_valid( &((*this_).visible_id) ) && data_id_is_valid( &((*this_).model_id) ) );
}

static inline void data_id_pair_trace ( data_id_pair_t *this_ )
{
    data_id_trace( &((*this_).visible_id) );
    data_id_trace( &((*this_).model_id) );
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
