/* File: universal_buffer_output_stream.inl; Copyright and License: see below */

#include <assert.h>

static inline int universal_buffer_output_stream_write2 ( universal_buffer_output_stream_t* this_, const void *start, size_t length )
{
#if 0
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).write != NULL );
    return (*(  (*((*this_).interface)).write  )) ( (*this_).objectdata, start, length );
#endif
}

static inline int universal_buffer_output_stream_flush2 ( universal_buffer_output_stream_t* this_ )
{
#if 0
    assert( (*this_).interface != NULL );
    assert( (*this_).objectdata != NULL );
    assert( (*((*this_).interface)).flush != NULL );
    return (*(  (*((*this_).interface)).flush  )) ( (*this_).objectdata );
#endif
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
