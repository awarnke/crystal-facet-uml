/* File: universal_input_stream.inl; Copyright and License: see below */

#include <assert.h>

static inline void universal_input_stream_private_init( universal_input_stream_t *this_,
                                                        const universal_input_stream_if_t *interface,
                                                        universal_input_stream_impl_t* objectdata )
{
    assert( interface != NULL );
    assert( objectdata != NULL );
    (*this_).interface = interface;
    (*this_).objectdata = objectdata;
}

static inline u8_error_t universal_input_stream_private_destroy( universal_input_stream_t *this_ )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    (*this_).interface = NULL;
    (*this_).objectdata = NULL;
    return U8_ERROR_NONE;
}

static inline const universal_input_stream_if_t* universal_input_stream_get_interface( universal_input_stream_t *this_ )
{
    assert( (*this_).interface != NULL );
    return (*this_).interface;
}

static inline universal_input_stream_impl_t* universal_input_stream_get_objectdata( universal_input_stream_t *this_ )
{
    assert( (*this_).objectdata != NULL );
    return (*this_).objectdata;
}

static inline u8_error_t universal_input_stream_read( universal_input_stream_t* this_,
                                                      void *out_buffer,
                                                      size_t max_size,
                                                      size_t *out_length )
{
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).read != NULL );
    return (*(  (*((*this_).interface)).read  )) ( (*this_).objectdata, out_buffer, max_size, out_length );
}


/*
Copyright 2021-2021 Andreas Warnke

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
