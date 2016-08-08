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

        index = ((*this_).start + (*this_).current + (CTRL_UNDO_REDO_LIST_MAX_SIZE-1)) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
        result = &((*this_).buffer[index]);
    }
    else if ( (*this_).current < CTRL_UNDO_REDO_LIST_MAX_SIZE )
    {
        /* add a new entry */
        /* (*this_).start stays untouched */
        (*this_).current ++;
        (*this_).length ++;

        /* call the constructor */
        index = ((*this_).start + (*this_).current + (CTRL_UNDO_REDO_LIST_MAX_SIZE-1)) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
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

        index = ((*this_).start + (*this_).current + (CTRL_UNDO_REDO_LIST_MAX_SIZE-1)) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
        result = &((*this_).buffer[index]);
    }

    TRACE_INFO_INT_INT( "current, length:", (*this_).current, (*this_).length );

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
            index = ((*this_).start + (*this_).current + (CTRL_UNDO_REDO_LIST_MAX_SIZE-1)) % CTRL_UNDO_REDO_LIST_MAX_SIZE;

            if ( CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY == ctrl_undo_redo_entry_get_action_type( &((*this_).buffer[index]) ) )
            {
                TRACE_INFO("boundary");
                finished = true;
            }
            else
            {
                TRACE_INFO("undo");
                result |= ctrl_undo_redo_list_private_do_action( this_, &((*this_).buffer[index]), true );
            }
        }
    }

    TRACE_INFO_INT_INT( "current, length:", (*this_).current, (*this_).length );

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
            index = ((*this_).start + (*this_).current + (CTRL_UNDO_REDO_LIST_MAX_SIZE-1)) % CTRL_UNDO_REDO_LIST_MAX_SIZE;

            if ( CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY == ctrl_undo_redo_entry_get_action_type( &((*this_).buffer[index]) ) )
            {
                TRACE_INFO("boundary");
                finished = true;
            }
            else if ( (*this_).current == (*this_).length )
            {
                TRACE_INFO("boundary");
                finished = true;
            }
            if ( ! finished )
            {
                TRACE_INFO("redo");
                result |= ctrl_undo_redo_list_private_do_action( this_, &((*this_).buffer[index]), false );
            }
        }
    }

    TRACE_INFO_INT_INT( "current, length:", (*this_).current, (*this_).length );

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_undo_redo_list_private_do_action ( ctrl_undo_redo_list_t *this_, ctrl_undo_redo_entry_t *action, bool undo )
{
    TRACE_BEGIN();

    ctrl_error_t result = CTRL_ERROR_NONE;

    switch ( ctrl_undo_redo_entry_get_action_type( action ) )
    {
        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAM:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAM" );
            data_diagram_t *diag = ctrl_undo_redo_entry_get_diagram_before_action_ptr ( action );
            if ( undo )
            {
                result |= (ctrl_error_t) data_database_writer_create_diagram ( (*this_).db_writer, diag, NULL );
            }
            else
            {
                int64_t obj_id = data_diagram_get_id ( diag );
                result |= (ctrl_error_t) data_database_writer_delete_diagram ( (*this_).db_writer, obj_id, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAM:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAM" );
            if ( undo )
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
            else
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAM:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAM" );
            data_diagram_t *diag = ctrl_undo_redo_entry_get_diagram_after_action_ptr ( action );
            if ( undo )
            {
                int64_t obj_id = data_diagram_get_id ( diag );
                result |= (ctrl_error_t) data_database_writer_delete_diagram ( (*this_).db_writer, obj_id, NULL );
            }
            else
            {
                result |= (ctrl_error_t) data_database_writer_create_diagram ( (*this_).db_writer, diag, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAMELEMENT:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAMELEMENT" );
            data_diagramelement_t *diag_ele = ctrl_undo_redo_entry_get_diagramelement_before_action_ptr ( action );
            if ( undo )
            {
                result |= (ctrl_error_t) data_database_writer_create_diagramelement ( (*this_).db_writer, diag_ele, NULL );
            }
            else
            {
                int64_t obj_id = data_diagramelement_get_id ( diag_ele );
                result |= (ctrl_error_t) data_database_writer_delete_diagramelement ( (*this_).db_writer, obj_id, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAMELEMENT:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAMELEMENT" );
            if ( undo )
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
            else
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAMELEMENT:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAMELEMENT" );
            data_diagramelement_t *diag_ele = ctrl_undo_redo_entry_get_diagramelement_after_action_ptr ( action );
            if ( undo )
            {
                int64_t obj_id = data_diagramelement_get_id ( diag_ele );
                result |= (ctrl_error_t) data_database_writer_delete_diagramelement ( (*this_).db_writer, obj_id, NULL );
            }
            else
            {
                result |= (ctrl_error_t) data_database_writer_create_diagramelement ( (*this_).db_writer, diag_ele, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_CLASSIFIER:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_CLASSIFIER" );
            data_classifier_t *classfy = ctrl_undo_redo_entry_get_classifier_before_action_ptr ( action );
            if ( undo )
            {
                result |= (ctrl_error_t) data_database_writer_create_classifier ( (*this_).db_writer, classfy, NULL );
            }
            else
            {
                int64_t obj_id = data_classifier_get_id ( classfy );
                result |= (ctrl_error_t) data_database_writer_delete_classifier ( (*this_).db_writer, obj_id, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_CLASSIFIER:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_CLASSIFIER" );
            if ( undo )
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
            else
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_CLASSIFIER:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_CLASSIFIER" );
            data_classifier_t *classfy = ctrl_undo_redo_entry_get_classifier_after_action_ptr ( action );
            if ( undo )
            {
                int64_t obj_id = data_classifier_get_id ( classfy );
                result |= (ctrl_error_t) data_database_writer_delete_classifier ( (*this_).db_writer, obj_id, NULL );
            }
            else
            {
                result |= (ctrl_error_t) data_database_writer_create_classifier ( (*this_).db_writer, classfy, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_FEATURE:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_FEATURE" );
            if ( undo )
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
            else
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_FEATURE:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_FEATURE" );
            if ( undo )
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
            else
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_FEATURE:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_FEATURE" );
            if ( undo )
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
            else
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_RELATIONSHIP:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_RELATIONSHIP" );
            if ( undo )
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
            else
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_RELATIONSHIP:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_RELATIONSHIP" );
            if ( undo )
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
            else
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_RELATIONSHIP:
        {
            TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_RELATIONSHIP" );
            if ( undo )
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
            else
            {
                result |= CTRL_ERROR_NOT_YET_IMPLEMENTED_ID;
            }
        }
        break;

        default:
        {
            LOG_ERROR( "unexptected ctrl_undo_redo_entry_type_t" );
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
