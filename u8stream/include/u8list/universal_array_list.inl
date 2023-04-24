/* File: universal_array_list.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>
#include <string.h>

static inline void universal_array_list_init ( universal_array_list_t *this_,
                                               unsigned int max_elements,
                                               void *elements,
                                               size_t element_size,
                                               ptrdiff_t step_size,
                                               void (*copy_ctor)(void* to_instance, const void* from_instance),
                                               bool (*equal)(const void* instance_1, const void* instance_2),
                                               void (*dtor)(void* instance) )
{
    U8_TRACE_INFO_INT( "- max_elements:", max_elements );
    U8_TRACE_INFO_INT( "- element_size:", element_size );
    U8_TRACE_INFO_INT( "- step_size:", step_size );
    assert( element_size <= step_size );
    assert( (element_size+sizeof(int)) > step_size );  /* something is wrong if there is more padding than an int */
    assert( elements != NULL );
    assert( max_elements != 0 );  /* a list of size 0 could be valid - but most likely this function is called with wrong arguments */

    (*this_).length = 0;
    (*this_).elements = elements;
    (*this_).max_elements = max_elements;
    (*this_).element_size = element_size;
    (*this_).step_size = step_size;
    (*this_).copy_ctor = copy_ctor;
    (*this_).equal = equal;
    (*this_).dtor = dtor;
}

static inline void universal_array_list_destroy ( universal_array_list_t *this_ )
{
    assert( (*this_).length <= (*this_).max_elements );
    assert( (*this_).elements != NULL );

    universal_array_list_clear( this_ );
}

static inline void universal_array_list_trace ( const universal_array_list_t *this_ )
{
    assert( (*this_).length <= (*this_).max_elements );
    assert( (*this_).elements != NULL );

    U8_TRACE_INFO( "universal_array_list_t" );
    U8_TRACE_INFO_INT( "- length:", (*this_).length );
    U8_TRACE_INFO_INT( "- max_elements:", (*this_).max_elements );
    U8_TRACE_INFO_INT( "- element_size:", (*this_).element_size );
    U8_TRACE_INFO_STR( "- copy_ctor:", ((*this_).copy_ctor == NULL)?"NULL":"exists" );
    U8_TRACE_INFO_STR( "- dtor:", ((*this_).dtor == NULL)?"NULL":"exists" );
}

static inline bool universal_array_list_is_empty ( const universal_array_list_t *this_ )
{
    return ( 0 == (*this_).length );
}

static inline int universal_array_list_append ( universal_array_list_t *this_, const void* element )
{
    assert( (*this_).length <= (*this_).max_elements );
    assert( (*this_).elements != NULL );

    int err_result;

    if ( (*this_).length < (*this_).max_elements )
    {
        unsigned int index = (*this_).length;
        (*this_).length ++;
        void *pos = universal_array_list_get_ptr( this_, index );
        if ( (*this_).copy_ctor != NULL )
        {
            (*this_).copy_ctor( pos, element );
        }
        else
        {
            memcpy( pos, element, (*this_).element_size );
        }
        err_result = 0;
    }
    else
    {
        err_result = -1;
    }

    return err_result;
}

static inline int universal_array_list_append_all ( universal_array_list_t *this_, const universal_array_list_t *that )
{
    assert( that != NULL );

    int err_result = 0;
    const unsigned int len = universal_array_list_get_length(that);
    for ( unsigned int idx = 0; (idx < len)&&(err_result == 0); idx ++ )
    {
        err_result = universal_array_list_append( this_, universal_array_list_get_const( that, idx ) );
    }

    return err_result;
}

static inline void *universal_array_list_get_ptr ( universal_array_list_t *this_, unsigned int index )
{
    assert( (*this_).length <= (*this_).max_elements );
    assert( (*this_).elements != NULL );

    void *result;

    if ( index < (*this_).length )
    {
        result = ((char*)(*this_).elements) + ((*this_).step_size * index);
    }
    else
    {
        result = NULL;
    }

    return result;
}

static inline void const *universal_array_list_get_const ( const universal_array_list_t *this_, unsigned int index )
{
    assert( (*this_).length <= (*this_).max_elements );
    assert( (*this_).elements != NULL );

    void const *result;

    if ( index < (*this_).length )
    {
        result = ((const char*)(*this_).elements) + ((*this_).step_size * index);
    }
    else
    {
        result = NULL;
    }

    return result;
}

static inline int universal_array_list_get_index_of ( const universal_array_list_t *this_, const void* element )
{
    assert( (*this_).length <= (*this_).max_elements );
    assert( (*this_).elements != NULL );
    assert( element != NULL );

    int result = -1;

    if ( (*this_).equal != NULL )
    {
        for ( unsigned int index = 0; ( index < (*this_).length )&&( result == -1 ); index ++ )
        {
            const void *current = universal_array_list_get_const( this_, index );
            if ( (*this_).equal( current, element ) )
            {
                result = index;
            }
        }
    }
    else
    {
        for ( unsigned int index = 0; ( index < (*this_).length )&&( result == -1 ); index ++ )
        {
            const void *current = universal_array_list_get_const( this_, index );
            if ( 0 == memcmp( current, element, (*this_).element_size ) )
            {
                result = index;
            }
        }
    }

    return result;
}

static inline void universal_array_list_clear ( universal_array_list_t *this_ )
{
    assert( (*this_).length <= (*this_).max_elements );
    assert( (*this_).elements != NULL );

    if ( (*this_).dtor != NULL )
    {
        for ( unsigned int index = 0; index < (*this_).length; index ++ )
        {
            void *current = universal_array_list_get_ptr( this_, index );
            (*this_).dtor( current );
        }
    }
    (*this_).length = 0;
}

static inline unsigned int universal_array_list_get_length ( const universal_array_list_t *this_ )
{
    return (*this_).length;
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
