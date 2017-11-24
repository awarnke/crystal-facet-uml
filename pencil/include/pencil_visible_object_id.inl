/* File: pencil_visible_object_id.inl; Copyright and License: see below */

static inline void pencil_visible_object_id_init_void ( pencil_visible_object_id_t *this_ )
{
    data_id_init_void( &((*this_).visible_id) );
    data_id_init_void( &((*this_).model_id) );
}

static inline void pencil_visible_object_id_reinit_void ( pencil_visible_object_id_t *this_ )
{
    data_id_reinit_void( &((*this_).visible_id) );
    data_id_reinit_void( &((*this_).model_id) );
}

static inline void pencil_visible_object_id_init ( pencil_visible_object_id_t *this_, data_id_t visible_id, data_id_t model_id )
{
    (*this_).visible_id = visible_id;
    (*this_).model_id = model_id;
}

static inline void pencil_visible_object_id_reinit ( pencil_visible_object_id_t *this_, data_id_t visible_id, data_id_t model_id )
{
    (*this_).visible_id = visible_id;
    (*this_).model_id = model_id;
}

static inline void pencil_visible_object_id_init_by_table_and_id ( pencil_visible_object_id_t *this_,
                                                                   data_table_t visible_table,
                                                                   int64_t visible_row_id,
                                                                   data_table_t model_table,
                                                                   int64_t model_row_id )
{
    data_id_init ( &((*this_).visible_id), visible_table, visible_row_id );
    data_id_init ( &((*this_).model_id), model_table, model_row_id );
}

static inline void pencil_visible_object_id_reinit_by_table_and_id ( pencil_visible_object_id_t *this_,
                                                                     data_table_t visible_table,
                                                                     int64_t visible_row_id,
                                                                     data_table_t model_table,
                                                                     int64_t model_row_id )
{
    data_id_reinit ( &((*this_).visible_id), visible_table, visible_row_id );
    data_id_reinit ( &((*this_).model_id), model_table, model_row_id );
}

static inline void pencil_visible_object_id_destroy ( pencil_visible_object_id_t *this_ )
{
    data_id_destroy( &((*this_).visible_id) );
    data_id_destroy( &((*this_).model_id) );
}

static inline data_id_t pencil_visible_object_id_get_visible_id ( pencil_visible_object_id_t *this_ )
{
    return (*this_).visible_id;
}

static inline data_id_t *pencil_visible_object_id_get_visible_id_ptr ( pencil_visible_object_id_t *this_ )
{
    return &((*this_).visible_id);
}

static inline data_id_t pencil_visible_object_id_get_model_id ( pencil_visible_object_id_t *this_ )
{
    return (*this_).model_id;
}

static inline data_id_t *pencil_visible_object_id_get_model_id_ptr ( pencil_visible_object_id_t *this_ )
{
    return &((*this_).model_id);
}

static inline bool pencil_visible_object_id_is_valid ( pencil_visible_object_id_t *this_ )
{
    return ( data_id_is_valid( &((*this_).visible_id) ) && data_id_is_valid( &((*this_).model_id) ) );
}

static inline void pencil_visible_object_id_trace ( pencil_visible_object_id_t *this_ )
{
    data_id_trace( &((*this_).visible_id) );
    data_id_trace( &((*this_).model_id) );
}


/*
Copyright 2017-2017 Andreas Warnke

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
