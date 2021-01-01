/* File: data_small_set.inl; Copyright and License: see below */

#include "trace.h"
#include "tslog.h"
#include <assert.h>

static inline void data_small_set_init ( data_small_set_t *this_ )
{
    (*this_).count = 0;
}

static inline void data_small_set_destroy ( data_small_set_t *this_ )
{
    assert( (*this_).count <= DATA_SMALL_SET_MAX_SET_SIZE );

    for ( uint32_t index = 0; index < (*this_).count; index ++ )
    {
        data_id_destroy( &((*this_).id_set[index]) );
    }
    (*this_).count = 0;
}

static inline void data_small_set_trace ( const data_small_set_t *this_ )
{
    assert( (*this_).count <= DATA_SMALL_SET_MAX_SET_SIZE );

    TRACE_INFO( "data_small_set_t" );
    TRACE_INFO_INT( "- count:", (*this_).count );
    for ( uint32_t index = 0; index < (*this_).count; index ++ )
    {
        switch ( data_id_get_table( &((*this_).id_set[index]) ))
        {
            case DATA_TABLE_VOID:
                TRACE_INFO_INT("- []: table = DATA_TABLE_VOID, row_id =", data_id_get_row_id( &((*this_).id_set[index]) ) );
                break;
            case DATA_TABLE_CLASSIFIER:
                TRACE_INFO_INT("- []: table = DATA_TABLE_CLASSIFIER, row_id =", data_id_get_row_id( &((*this_).id_set[index]) ) );
                break;
            case DATA_TABLE_FEATURE:
                TRACE_INFO_INT("- []: table = DATA_TABLE_FEATURE, row_id =", data_id_get_row_id( &((*this_).id_set[index]) ) );
                break;
            case DATA_TABLE_RELATIONSHIP:
                TRACE_INFO_INT("- []: table = DATA_TABLE_RELATIONSHIP, row_id =", data_id_get_row_id( &((*this_).id_set[index]) ) );
                break;
            case DATA_TABLE_DIAGRAMELEMENT:
                TRACE_INFO_INT("- []: table = DATA_TABLE_DIAGRAMELEMENT, row_id =", data_id_get_row_id( &((*this_).id_set[index]) ) );
                break;
            case DATA_TABLE_DIAGRAM:
                TRACE_INFO_INT("- []: table = DATA_TABLE_DIAGRAM, row_id =", data_id_get_row_id( &((*this_).id_set[index]) ) );
                break;
            default:
                TSLOG_ERROR("- []: illegal value");
                break;
        }
    }
}

static inline bool data_small_set_is_empty ( const data_small_set_t *this_ )
{
    return ( 0 == (*this_).count );
}

static inline bool data_small_set_contains ( const data_small_set_t *this_, data_id_t obj_id )
{
    assert( (*this_).count <= DATA_SMALL_SET_MAX_SET_SIZE );
    bool result;
    result = false;

    for ( uint32_t index = 0; ( index < (*this_).count ) && ( false == result ); index ++ )
    {
        if ( data_id_equals( &obj_id, &((*this_).id_set[index]) ) )
        {
            result = true;
        }
    }

    return result;
}

static inline bool data_small_set_contains_row_id ( const data_small_set_t *this_, data_table_t table, data_row_id_t row_id )
{
    bool result;
    data_id_t my_id;
    data_id_init( &my_id, table, row_id );
    result = data_small_set_contains( this_, my_id );
    data_id_destroy( &my_id );
    return result;
}

static inline data_error_t data_small_set_add_obj ( data_small_set_t *this_, data_id_t obj_id )
{
    assert( (*this_).count <= DATA_SMALL_SET_MAX_SET_SIZE );
    data_error_t result;
    result = DATA_ERROR_NONE;

    if ( data_id_is_valid( &obj_id ) )
    {
        for ( uint32_t index = 0; index < (*this_).count; index ++ )
        {
            if ( data_id_equals( &obj_id, &((*this_).id_set[index]) ) )
            {
                result = DATA_ERROR_DUPLICATE_ID;
            }
        }
        if ( result == DATA_ERROR_NONE )
        {
            if ( (*this_).count < DATA_SMALL_SET_MAX_SET_SIZE )
            {
                (*this_).id_set[(*this_).count] = obj_id;
                (*this_).count ++;
            }
            else
            {
                result = DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
        }
    }
    else
    {
        result = DATA_ERROR_INVALID_REQUEST;
    }

    return result;
}

static inline data_error_t data_small_set_add_row_id ( data_small_set_t *this_, data_table_t table, data_row_id_t row_id )
{
    bool result;
    data_id_t my_id;
    data_id_init( &my_id, table, row_id );
    result = data_small_set_add_obj( this_, my_id );
    data_id_destroy( &my_id );
    return result;
}

static inline data_error_t data_small_set_delete_obj ( data_small_set_t *this_, data_id_t obj_id )
{
    assert( (*this_).count <= DATA_SMALL_SET_MAX_SET_SIZE );
    data_error_t result;
    result = DATA_ERROR_INVALID_REQUEST;

    for ( uint32_t index = 0; index < (*this_).count; index ++ )
    {
        if ( data_id_equals( &obj_id, &((*this_).id_set[index]) ) )
        {
            result = DATA_ERROR_NONE;
            data_id_destroy( &((*this_).id_set[index]) );

            (*this_).count --;
            (*this_).id_set[index] = (*this_).id_set[(*this_).count];
        }
    }

    return result;
}

static inline data_error_t data_small_set_toggle_obj ( data_small_set_t *this_, data_id_t obj_id )
{
    data_error_t result;

    if ( data_small_set_contains( this_, obj_id ) )
    {
        result = data_small_set_delete_obj( this_, obj_id );
        assert( result == DATA_ERROR_NONE );
    }
    else
    {
        result = data_small_set_add_obj( this_, obj_id );
    }

    return result;
}

static inline void data_small_set_clear ( data_small_set_t *this_ )
{
    assert( (*this_).count <= DATA_SMALL_SET_MAX_SET_SIZE );

    for ( uint32_t index = 0; index < (*this_).count; index ++ )
    {
        data_id_destroy( &((*this_).id_set[index]) );
    }
    (*this_).count = 0;
}

static inline uint32_t data_small_set_get_count ( const data_small_set_t *this_ )
{
    return (*this_).count;
}

static inline data_id_t data_small_set_get_id ( const data_small_set_t *this_, uint32_t index )
{
    assert( (*this_).count <= DATA_SMALL_SET_MAX_SET_SIZE );
    data_id_t result;

    if ( index < (*this_).count )
    {
        result = (*this_).id_set[index];
    }
    else
    {
        data_id_init_void( &result );
    }

    return result;
}

/*
Copyright 2016-2021 Andreas Warnke

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
