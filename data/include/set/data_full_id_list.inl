/* File: data_full_id_list.inl; Copyright and License: see below */

static inline void data_full_id_list_init ( data_full_id_list_t *this_,
                                            unsigned int max_elements,
                                            data_full_id_t (*elements)[] )
{
    universal_array_list_init( (universal_array_list_t*) this_,
                               max_elements,
                               (void*)elements,
                               sizeof(data_full_id_t),
                               sizeof(data_full_id_t),
                               NULL,
                               NULL,
                               NULL
                             );
}

static inline void data_full_id_list_destroy ( data_full_id_list_t *this_ )
{
    universal_array_list_destroy( (universal_array_list_t*) this_ );
}

static inline void data_full_id_list_trace ( const data_full_id_list_t *this_ )
{
    universal_array_list_trace( (universal_array_list_t*) this_ );
    for ( unsigned int idx = 0; idx < data_full_id_list_get_length(this_); idx ++ )
    {
        data_full_id_trace( data_full_id_list_get_const(this_,idx) );
    }
}

static inline bool data_full_id_list_is_empty ( const data_full_id_list_t *this_ )
{
    return universal_array_list_is_empty( (universal_array_list_t*) this_ );
}

static inline u8_error_t data_full_id_list_add ( data_full_id_list_t *this_, const data_full_id_t* element )
{
    return universal_array_list_append( (universal_array_list_t*) this_, element );
}

static inline u8_error_t data_full_id_list_add_all ( data_full_id_list_t *this_, const data_full_id_list_t *that )
{
    return universal_array_list_append_all( (universal_array_list_t*) this_, (const universal_array_list_t*) that );
}

static inline data_full_id_t *data_full_id_list_get_ptr ( data_full_id_list_t *this_, unsigned int index )
{
    return (data_full_id_t*) universal_array_list_get_ptr( (universal_array_list_t*) this_, index );
}

static inline data_full_id_t const *data_full_id_list_get_const ( const data_full_id_list_t *this_, unsigned int index )
{
    return (data_full_id_t const*) universal_array_list_get_const( (universal_array_list_t*) this_, index );
}

static inline void data_full_id_list_clear ( data_full_id_list_t *this_ )
{
    universal_array_list_clear( (universal_array_list_t*) this_ );
}

static inline unsigned int data_full_id_list_get_length ( const data_full_id_list_t *this_ )
{
    return universal_array_list_get_length( (universal_array_list_t*) this_ );
}


/*
Copyright 2023-2024 Andreas Warnke

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
