/* File: ctrl_undo_redo_list.c; Copyright and License: see below */

#include "ctrl_undo_redo_list.h"
#include "trace.h"
#include "log.h"
#include <assert.h>

ctrl_undo_redo_entry_t *ctrl_undo_redo_list_private_add_entry_ptr ( ctrl_undo_redo_list_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).start < CTRL_UNDO_REDO_LIST_MAX_SIZE );
    assert( (*this_).length <= CTRL_UNDO_REDO_LIST_MAX_SIZE );
    assert( (*this_).current <= (*this_).length );

    uint32_t index;
    ctrl_undo_redo_entry_t *result;

    if ( (*this_).current < (*this_).length )
    {
        /* overwrite an existing and new entry */
        /* (*this_).start stays untouched */
        (*this_).current ++;

        /* call destructor of all later entries */
        for ( uint32_t pos = (*this_).current; pos < (*this_).length; pos ++ )
        {
            uint32_t del_index = ((*this_).start + pos) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
            ctrl_undo_redo_entry_destroy( &((*this_).buffer[del_index]) );
        }

        /* shrink the list */
        (*this_).length = (*this_).current;

        index = ((*this_).start + (*this_).current) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
        result = &((*this_).buffer[index]);
    }
    else if ( (*this_).current < CTRL_UNDO_REDO_LIST_MAX_SIZE )
    {
        /* add a new entry */
        /* (*this_).start stays untouched */
        (*this_).current ++;
        (*this_).length ++;

        /* call the constructor */
        index = ((*this_).start + (*this_).current) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
        result = &((*this_).buffer[index]);
        ctrl_undo_redo_entry_init_empty( result );
    }
    else
    {
        /* overwrite an existing old entry */
        (*this_).start = ((*this_).start+1) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
        /* (*this_).current is already CTRL_UNDO_REDO_LIST_MAX_SIZE */
        /* (*this_).length is already CTRL_UNDO_REDO_LIST_MAX_SIZE */
        (*this_).buffer_incomplete = true;

        index = ((*this_).start + (*this_).current) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
        result = &((*this_).buffer[index]);
    }

    TRACE_END();
    return result;
}

ctrl_error_t ctrl_undo_redo_list_undo ( ctrl_undo_redo_list_t *this_ )
{
    TRACE_BEGIN();

    ctrl_error_t result = CTRL_ERROR_NONE;

    if ( 2 > ctrl_undo_redo_list_private_count_boundaries( this_, (*this_).start, (*this_).current ) )
    {
        if ( (*this_).buffer_incomplete )
        {
            result = CTRL_ERROR_ARRAY_BUFFER_EXCEEDED;
        }
        else
        {
            result = CTRL_ERROR_INVALID_REQUEST;
        }
    }
    else
    {
        bool finished = false;
        uint32_t index;
        for ( uint32_t pos = 0; (pos < CTRL_UNDO_REDO_LIST_MAX_SIZE) && (! finished); pos ++ )
        {
            /* move the current pointer back in the list */
            (*this_).current --;

            /* check if we are done */
            index = ((*this_).start + (*this_).current + CTRL_UNDO_REDO_LIST_MAX_SIZE - 1) % CTRL_UNDO_REDO_LIST_MAX_SIZE;

            if ( CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY == ctrl_undo_redo_entry_get_action_type( &((*this_).buffer[index]) ) )
            {
                finished = true;
            }
            else
            {
                TRACE_INFO("undo"); 
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_undo_redo_list_redo ( ctrl_undo_redo_list_t *this_ )
{
    TRACE_BEGIN();

    ctrl_error_t result = CTRL_ERROR_NONE;

    if ( (*this_).current == (*this_).length )
    {
        result = CTRL_ERROR_INVALID_REQUEST;
    }
    else
    {
        bool finished = false;
        uint32_t index;
        for ( uint32_t pos = 0; (pos < CTRL_UNDO_REDO_LIST_MAX_SIZE) && (! finished); pos ++ )
        {
            /* move the current pointer forward in the list */
            (*this_).current ++;

            /* check if we are done */
            index = ((*this_).start + (*this_).current + CTRL_UNDO_REDO_LIST_MAX_SIZE - 1) % CTRL_UNDO_REDO_LIST_MAX_SIZE;

            if ( CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY == ctrl_undo_redo_entry_get_action_type( &((*this_).buffer[index]) ) )
            {
                finished = true;
            }
            if ( (*this_).current == (*this_).length )
            {
                finished = true;
            }
            if ( ! finished )
            {
                TRACE_INFO("redo"); 
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2016 Andreas Warnke

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
