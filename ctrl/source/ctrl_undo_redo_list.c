/* File: ctrl_undo_redo_list.c; Copyright and License: see below */

#include "ctrl_undo_redo_list.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

u8_error_t ctrl_undo_redo_list_remove_boundary_from_end ( ctrl_undo_redo_list_t *this_ )
{
    assert( (*this_).start < CTRL_UNDO_REDO_LIST_MAX_SIZE );
    assert( (*this_).length <= CTRL_UNDO_REDO_LIST_MAX_SIZE );
    assert( (*this_).current <= (*this_).length );

    u8_error_t result = U8_ERROR_NONE;
    ctrl_undo_redo_entry_t *boundary_entry;

    if ( (*this_).current == 0 )
    {
        /* there is no entry - therefore no boundary - to be removed */
        result = U8_ERROR_INVALID_REQUEST;
    }
    else
    {
        /* check if current is a boundary */
        uint32_t index;
        ctrl_undo_redo_entry_type_t action;

        index = ((*this_).start + (*this_).current + (CTRL_UNDO_REDO_LIST_MAX_SIZE-1)) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
        boundary_entry = &((*this_).buffer[index]);
        action = ctrl_undo_redo_entry_get_action_type ( boundary_entry );
        if ( CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY != action )
        {
            /* current is not a boundary */
            result = U8_ERROR_INVALID_REQUEST;
        }
        else
        {
            /* drop all list-entries newer than the current position */
            if ( (*this_).current < (*this_).length )
            {
                /* call destructor of all later entries */
                for ( uint32_t pos = (*this_).current; pos < (*this_).length; pos ++ )
                {
                    uint32_t del_index = ((*this_).start + pos) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
                    ctrl_undo_redo_entry_destroy( &((*this_).buffer[del_index]) );
                }

                /* shrink the list */
                (*this_).length = (*this_).current;
            }

            /* call destructor of boundary and remove it */
            ctrl_undo_redo_entry_destroy( boundary_entry );

            /* remove the boundary */
            (*this_).length --;
            (*this_).current --;
        }
    }

    return result;
}

u8_error_t ctrl_undo_redo_list_undo ( ctrl_undo_redo_list_t *this_, data_stat_t *io_stat )
{
    U8_TRACE_BEGIN();
    assert ( NULL != io_stat );

    u8_error_t result = U8_ERROR_NONE;

    if ( 2 > ctrl_undo_redo_list_private_count_boundaries( this_, (*this_).start, (*this_).current ) )
    {
        if ( (*this_).buffer_incomplete )
        {
            result = U8_ERROR_ARRAY_BUFFER_EXCEEDED;
        }
        else
        {
            result = U8_ERROR_INVALID_REQUEST;
        }
    }
    else
    {
        bool finished = false;
        for ( uint32_t pos = 0; (pos < CTRL_UNDO_REDO_LIST_MAX_SIZE) && (! finished); pos ++ )
        {
            /* move the current pointer back in the list */
            (*this_).current --;

            /* check if we are done */
            const uint32_t index
                = ((*this_).start + (*this_).current + (CTRL_UNDO_REDO_LIST_MAX_SIZE-1)) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
            ctrl_undo_redo_entry_t *const cur_entry = &((*this_).buffer[index]);

            if ( CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY == ctrl_undo_redo_entry_get_action_type( cur_entry ) )
            {
                U8_TRACE_INFO("boundary");
                const data_revision_t revision = ctrl_undo_redo_entry_get_boundary_revision( cur_entry );
                data_database_writer_set_revision( (*this_).db_writer, revision );
                finished = true;
            }
            else
            {
                U8_TRACE_INFO("undo");
                const uint32_t current_before = (*this_).current;
                const u8_error_t cur_err = ctrl_undo_redo_list_private_do_action( this_, cur_entry, true );
                ctrl_undo_redo_entry_to_statistics ( cur_entry, true /*=undo*/, (U8_ERROR_NONE!=cur_err), io_stat );
                result |= cur_err;
                if ( (*this_).current != current_before )
                {
                     U8_LOG_ERROR("ctrl_undo_redo_list_t was modified while performing undo.");
                     /* try to continue undo, this is the most likely way to get the db back to a consistent state */
                }
            }
        }
    }

    U8_TRACE_INFO_INT_INT( "current, length:", (*this_).current, (*this_).length );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_undo_redo_list_redo ( ctrl_undo_redo_list_t *this_, data_stat_t *io_stat )
{
    U8_TRACE_BEGIN();
    assert ( NULL != io_stat );

    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).current == (*this_).length )
    {
        /* current points already to the end of the list - no redo possible */
        result = U8_ERROR_INVALID_REQUEST;
    }
    else
    {
        bool finished = false;
        for ( uint32_t pos = 0; (pos < CTRL_UNDO_REDO_LIST_MAX_SIZE) && (! finished); pos ++ )
        {
            /* move the current pointer forward in the list */
            (*this_).current ++;

            /* check if we are done */
            const uint32_t index
                = ((*this_).start + (*this_).current + (CTRL_UNDO_REDO_LIST_MAX_SIZE-1)) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
            ctrl_undo_redo_entry_t *const cur_entry = &((*this_).buffer[index]);

            if ( CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY == ctrl_undo_redo_entry_get_action_type( cur_entry ) )
            {
                U8_TRACE_INFO("boundary");
                const data_revision_t revision = ctrl_undo_redo_entry_get_boundary_revision( cur_entry );
                data_database_writer_set_revision( (*this_).db_writer, revision );
                finished = true;
            }
            else if ( (*this_).current == (*this_).length )
            {
                U8_TRACE_INFO("boundary");
                finished = true;
            }
            if ( ! finished )
            {
                U8_TRACE_INFO("redo");
                const uint32_t current_before = (*this_).current;
                const u8_error_t cur_err = ctrl_undo_redo_list_private_do_action( this_, cur_entry, false );
                ctrl_undo_redo_entry_to_statistics ( cur_entry, false /*=undo*/, (U8_ERROR_NONE!=cur_err), io_stat );
                result |= cur_err;
                if ( (*this_).current != current_before )
                {
                    U8_LOG_ERROR("ctrl_undo_redo_list_t was modified while performing redo.");
                    result |= U8_ERROR_INVALID_REQUEST;
                    finished = true;
                }
            }
        }
    }

    U8_TRACE_INFO_INT_INT( "current, length:", (*this_).current, (*this_).length );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_undo_redo_list_get_undo_iterator ( const ctrl_undo_redo_list_t *this_,
                                                   ctrl_undo_redo_iterator_t *out_undo_iterator )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_undo_iterator );
    assert( (*this_).start < CTRL_UNDO_REDO_LIST_MAX_SIZE );
    assert( (*this_).length <= CTRL_UNDO_REDO_LIST_MAX_SIZE );
    assert( (*this_).current <= (*this_).length );  /* current is in 0..length, relative to start */
    /*    B0   A1   A2   A3   B1   A4   B2   X   X    */
    /*  0    1    2    3    4    5    6    7   8   9  */
    /*  ^ start                            ^ current  */
    /*  Boundaries B0..2, Actions A1..4 */
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).current > 0 )
    {
        const uint32_t last
            = ( (*this_).start + (*this_).current + CTRL_UNDO_REDO_LIST_MAX_SIZE - 2 ) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
        const uint32_t undo_length = (*this_).current - 1;  /* total number of undo entries */
        uint32_t count = 0;
        {
            ctrl_undo_redo_iterator_t pre_scan;
            ctrl_undo_redo_iterator_init( &pre_scan,
                                          &((*this_).buffer),
                                          CTRL_UNDO_REDO_LIST_MAX_SIZE,
                                          false, /* NOT iterate_upwards */
                                          last,
                                          undo_length
                                        );
            for( bool finished = false; ctrl_undo_redo_iterator_has_next( &pre_scan ) && ( ! finished ); )
            {
                const ctrl_undo_redo_entry_t *next = ctrl_undo_redo_iterator_next( &pre_scan );
                if ( CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY == ctrl_undo_redo_entry_get_action_type( next ) )
                {
                    finished = true;
                }
                else
                {
                    count ++;
                }
            }
            ctrl_undo_redo_iterator_destroy( &pre_scan );
        }
        if ( count == undo_length )
        {
            /* no boundary found */
            if ( (*this_).buffer_incomplete )
            {
                result = U8_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            else
            {
                assert( count == 0 );  /* this should happen only when (*this_).current is at the initial boundary */
                result = U8_ERROR_INVALID_REQUEST;
            }
        }
        else
        {
            assert( count < undo_length );
            ctrl_undo_redo_iterator_reinit( out_undo_iterator,
                                            &((*this_).buffer),
                                            CTRL_UNDO_REDO_LIST_MAX_SIZE,
                                            false, /* NOT iterate_upwards */
                                            last,
                                            count
                                          );
        }
    }
    else
    {
        assert( false );  /* already at the start of the list, not even a single boundary is there anymore */
        result = U8_ERROR_ARRAY_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_undo_redo_list_get_redo_iterator ( const ctrl_undo_redo_list_t *this_,
                                                   ctrl_undo_redo_iterator_t *out_redo_iterator )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_redo_iterator );
    assert( (*this_).start < CTRL_UNDO_REDO_LIST_MAX_SIZE );
    assert( (*this_).length <= CTRL_UNDO_REDO_LIST_MAX_SIZE );
    assert( (*this_).current <= (*this_).length );  /* current is in 0..length, relative to start */
    /*    B0   A1   A2   A3   B1   A4   B2   X   X    */
    /*  0    1    2    3    4    5    6    7   8   9  */
    /*  ^ start                            ^ current  */
    /*  Boundaries B0..2, Actions A1..4 */
    u8_error_t result = U8_ERROR_NONE;

    const uint32_t next
        = ( (*this_).start + (*this_).current ) % CTRL_UNDO_REDO_LIST_MAX_SIZE;
    const uint32_t redo_length = (*this_).length - (*this_).current;  /* total number of redo entries */
    if ( redo_length > 0 )
    {
        uint32_t count = 0;
        {
            ctrl_undo_redo_iterator_t pre_scan;
            ctrl_undo_redo_iterator_init( &pre_scan,
                                          &((*this_).buffer),
                                          CTRL_UNDO_REDO_LIST_MAX_SIZE,
                                          true, /* iterate_upwards */
                                          next,
                                          redo_length
                                        );
            for( bool finished = false; ctrl_undo_redo_iterator_has_next( &pre_scan ) && ( ! finished ); )
            {
                const ctrl_undo_redo_entry_t *next = ctrl_undo_redo_iterator_next( &pre_scan );
                if ( CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY == ctrl_undo_redo_entry_get_action_type( next ) )
                {
                    finished = true;
                }
                else
                {
                    count ++;
                }
            }
            ctrl_undo_redo_iterator_destroy( &pre_scan );
        }
        assert( count < redo_length );
        ctrl_undo_redo_iterator_reinit( out_redo_iterator,
                                        &((*this_).buffer),
                                        CTRL_UNDO_REDO_LIST_MAX_SIZE,
                                        true, /* iterate_upwards */
                                        next,
                                        count
                                      );
    }
    else
    {
        result = U8_ERROR_INVALID_REQUEST;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ private ================================ */

ctrl_undo_redo_entry_t *ctrl_undo_redo_list_private_add_entry_ptr ( ctrl_undo_redo_list_t *this_ )
{
    U8_TRACE_BEGIN();
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

    U8_TRACE_INFO_INT_INT( "current, length:", (*this_).current, (*this_).length );

    U8_TRACE_END();
    return result;
}

u8_error_t ctrl_undo_redo_list_private_do_action ( ctrl_undo_redo_list_t *this_, ctrl_undo_redo_entry_t *action, bool undo )
{
    U8_TRACE_BEGIN();

    u8_error_t result = U8_ERROR_NONE;

    switch ( ctrl_undo_redo_entry_get_action_type( action ) )
    {
        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAM:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAM" );
            const data_diagram_t *diag = ctrl_undo_redo_entry_get_diagram_before_action_const ( action );
            if ( undo )
            {
                result |= (u8_error_t) data_database_writer_create_diagram ( (*this_).db_writer, diag, NULL );
            }
            else
            {
                data_row_t obj_id = data_diagram_get_row_id ( diag );
                result |= (u8_error_t) data_database_writer_delete_diagram ( (*this_).db_writer, obj_id, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAM:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAM" );
            const data_diagram_t *diag;
            if ( undo )
            {
                diag = ctrl_undo_redo_entry_get_diagram_before_action_const ( action );
            }
            else
            {
                diag = ctrl_undo_redo_entry_get_diagram_after_action_const ( action );
            }
            data_row_t diag_id = data_diagram_get_row_id ( diag );
            data_row_t diag_parent_id = data_diagram_get_parent_row_id ( diag );
            data_diagram_type_t diag_type = data_diagram_get_diagram_type ( diag );
            const char* diag_stereotype = data_diagram_get_stereotype_const ( diag );
            const char* diag_name = data_diagram_get_name_const ( diag );
            const char* diag_description = data_diagram_get_description_const ( diag );
            int32_t diag_list_oder = data_diagram_get_list_order ( diag );
            result |= (u8_error_t) data_database_writer_update_diagram_parent_id ( (*this_).db_writer, diag_id, diag_parent_id, NULL );
            result |= (u8_error_t) data_database_writer_update_diagram_type ( (*this_).db_writer, diag_id, diag_type, NULL );
            result |= (u8_error_t) data_database_writer_update_diagram_stereotype ( (*this_).db_writer, diag_id, diag_stereotype, NULL );
            result |= (u8_error_t) data_database_writer_update_diagram_name ( (*this_).db_writer, diag_id, diag_name, NULL );
            result |= (u8_error_t) data_database_writer_update_diagram_description ( (*this_).db_writer, diag_id, diag_description, NULL );
            result |= (u8_error_t) data_database_writer_update_diagram_list_order ( (*this_).db_writer, diag_id, diag_list_oder, NULL );
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAM:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAM" );
            const data_diagram_t *diag = ctrl_undo_redo_entry_get_diagram_after_action_const ( action );
            if ( undo )
            {
                data_row_t obj_id = data_diagram_get_row_id ( diag );
                result |= (u8_error_t) data_database_writer_delete_diagram ( (*this_).db_writer, obj_id, NULL );
            }
            else
            {
                result |= (u8_error_t) data_database_writer_create_diagram ( (*this_).db_writer, diag, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAMELEMENT:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAMELEMENT" );
            const data_diagramelement_t *diag_ele = ctrl_undo_redo_entry_get_diagramelement_before_action_const ( action );
            if ( undo )
            {
                result |= (u8_error_t) data_database_writer_create_diagramelement ( (*this_).db_writer, diag_ele, NULL );
            }
            else
            {
                data_row_t obj_id = data_diagramelement_get_row_id ( diag_ele );
                result |= (u8_error_t) data_database_writer_delete_diagramelement ( (*this_).db_writer, obj_id, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAMELEMENT:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAMELEMENT" );
            const data_diagramelement_t *diag_element;
            if ( undo )
            {
                diag_element = ctrl_undo_redo_entry_get_diagramelement_before_action_const ( action );
            }
            else
            {
                diag_element = ctrl_undo_redo_entry_get_diagramelement_after_action_const ( action );
            }
            data_row_t diag_elem_id = data_diagramelement_get_row_id ( diag_element );
            data_diagramelement_flag_t diag_elem_display_flags = data_diagramelement_get_display_flags ( diag_element );
            data_row_t diag_feature_id = data_diagramelement_get_focused_feature_row_id ( diag_element );
            result |= (u8_error_t) data_database_writer_update_diagramelement_display_flags ( (*this_).db_writer, diag_elem_id, diag_elem_display_flags, NULL );
            result |= (u8_error_t) data_database_writer_update_diagramelement_focused_feature_id ( (*this_).db_writer, diag_elem_id, diag_feature_id, NULL );
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAMELEMENT:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAMELEMENT" );
            const data_diagramelement_t *diag_ele = ctrl_undo_redo_entry_get_diagramelement_after_action_const ( action );
            if ( undo )
            {
                data_row_t obj_id = data_diagramelement_get_row_id ( diag_ele );
                result |= (u8_error_t) data_database_writer_delete_diagramelement ( (*this_).db_writer, obj_id, NULL );
            }
            else
            {
                result |= (u8_error_t) data_database_writer_create_diagramelement ( (*this_).db_writer, diag_ele, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_CLASSIFIER:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_CLASSIFIER" );
            const data_classifier_t *classfy = ctrl_undo_redo_entry_get_classifier_before_action_const ( action );
            if ( undo )
            {
                result |= (u8_error_t) data_database_writer_create_classifier ( (*this_).db_writer, classfy, NULL );
            }
            else
            {
                data_row_t obj_id = data_classifier_get_row_id ( classfy );
                result |= (u8_error_t) data_database_writer_delete_classifier ( (*this_).db_writer, obj_id, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_CLASSIFIER:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_CLASSIFIER" );
            const data_classifier_t *classfy;
            if ( undo )
            {
                classfy = ctrl_undo_redo_entry_get_classifier_before_action_const ( action );
            }
            else
            {
                classfy = ctrl_undo_redo_entry_get_classifier_after_action_const ( action );
            }
            data_row_t classfy_id = data_classifier_get_row_id ( classfy );
            data_classifier_type_t classfy_main_type = data_classifier_get_main_type ( classfy );
            const char* classfy_stereotype = data_classifier_get_stereotype_const ( classfy );
            const char* classfy_name = data_classifier_get_name_const ( classfy );
            const char* classfy_description = data_classifier_get_description_const ( classfy );
            int32_t classfy_x_order = data_classifier_get_x_order ( classfy );
            int32_t classfy_y_order = data_classifier_get_y_order ( classfy );
            int32_t classfy_list_order = data_classifier_get_list_order ( classfy );
            result |= (u8_error_t) data_database_writer_update_classifier_main_type ( (*this_).db_writer, classfy_id, classfy_main_type, NULL );
            result |= (u8_error_t) data_database_writer_update_classifier_stereotype ( (*this_).db_writer, classfy_id, classfy_stereotype, NULL );
            result |= (u8_error_t) data_database_writer_update_classifier_name ( (*this_).db_writer, classfy_id, classfy_name, NULL );
            result |= (u8_error_t) data_database_writer_update_classifier_description ( (*this_).db_writer, classfy_id, classfy_description, NULL );
            result |= (u8_error_t) data_database_writer_update_classifier_x_order ( (*this_).db_writer, classfy_id, classfy_x_order, NULL );
            result |= (u8_error_t) data_database_writer_update_classifier_y_order ( (*this_).db_writer, classfy_id, classfy_y_order, NULL );
            result |= (u8_error_t) data_database_writer_update_classifier_list_order ( (*this_).db_writer, classfy_id, classfy_list_order, NULL );
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_CLASSIFIER:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_CLASSIFIER" );
            const data_classifier_t *classfy = ctrl_undo_redo_entry_get_classifier_after_action_const ( action );
            if ( undo )
            {
                data_row_t obj_id = data_classifier_get_row_id ( classfy );
                result |= (u8_error_t) data_database_writer_delete_classifier ( (*this_).db_writer, obj_id, NULL );
            }
            else
            {
                result |= (u8_error_t) data_database_writer_create_classifier ( (*this_).db_writer, classfy, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_FEATURE:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_FEATURE" );
            const data_feature_t *feat = ctrl_undo_redo_entry_get_feature_before_action_const ( action );
            if ( undo )
            {
                result |= (u8_error_t) data_database_writer_create_feature ( (*this_).db_writer, feat, NULL );
            }
            else
            {
                data_row_t obj_id = data_feature_get_row_id ( feat );
                result |= (u8_error_t) data_database_writer_delete_feature ( (*this_).db_writer, obj_id, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_FEATURE:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_FEATURE" );
            const data_feature_t *feat;
            if ( undo )
            {
                feat = ctrl_undo_redo_entry_get_feature_before_action_const ( action );
            }
            else
            {
                feat = ctrl_undo_redo_entry_get_feature_after_action_const ( action );
            }
            data_row_t feature_id = data_feature_get_row_id ( feat );
            data_feature_type_t new_feature_type = data_feature_get_main_type ( feat );
            const char* new_feature_key = data_feature_get_key_const ( feat );
            const char* new_feature_value = data_feature_get_value_const ( feat );
            const char* new_feature_description = data_feature_get_description_const ( feat );
            int32_t new_feature_list_order = data_feature_get_list_order ( feat );
            result |= (u8_error_t) data_database_writer_update_feature_main_type ( (*this_).db_writer, feature_id, new_feature_type, NULL );
            result |= (u8_error_t) data_database_writer_update_feature_key ( (*this_).db_writer, feature_id, new_feature_key, NULL );
            result |= (u8_error_t) data_database_writer_update_feature_value ( (*this_).db_writer, feature_id, new_feature_value, NULL );
            result |= (u8_error_t) data_database_writer_update_feature_description ( (*this_).db_writer, feature_id, new_feature_description, NULL );
            result |= (u8_error_t) data_database_writer_update_feature_list_order ( (*this_).db_writer, feature_id, new_feature_list_order, NULL );
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_FEATURE:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_FEATURE" );
            const data_feature_t *feat = ctrl_undo_redo_entry_get_feature_after_action_const ( action );
            if ( undo )
            {
                data_row_t obj_id = data_feature_get_row_id ( feat );
                result |= (u8_error_t) data_database_writer_delete_feature ( (*this_).db_writer, obj_id, NULL );
            }
            else
            {
                result |= (u8_error_t) data_database_writer_create_feature ( (*this_).db_writer, feat, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_RELATIONSHIP:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_RELATIONSHIP" );
            const data_relationship_t *relation = ctrl_undo_redo_entry_get_relationship_before_action_const ( action );
            if ( undo )
            {
                result |= (u8_error_t) data_database_writer_create_relationship ( (*this_).db_writer, relation, NULL );
            }
            else
            {
                data_row_t obj_id = data_relationship_get_row_id ( relation );
                result |= (u8_error_t) data_database_writer_delete_relationship ( (*this_).db_writer, obj_id, NULL );
            }
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_RELATIONSHIP:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_RELATIONSHIP" );
            const data_relationship_t *relation;
            if ( undo )
            {
                relation = ctrl_undo_redo_entry_get_relationship_before_action_const ( action );
            }
            else
            {
                relation = ctrl_undo_redo_entry_get_relationship_after_action_const ( action );
            }
            data_row_t relationship_id = data_relationship_get_row_id ( relation );
            data_relationship_type_t new_relationship_type = data_relationship_get_main_type ( relation );
            const char* new_relationship_stereotype = data_relationship_get_stereotype_const ( relation );
            const char* new_relationship_name = data_relationship_get_name_const ( relation );
            const char* new_relationship_description = data_relationship_get_description_const ( relation );
            int32_t new_relationship_list_order = data_relationship_get_list_order ( relation );
            result |= (u8_error_t) data_database_writer_update_relationship_main_type ( (*this_).db_writer, relationship_id, new_relationship_type, NULL );
            result |= (u8_error_t) data_database_writer_update_relationship_stereotype ( (*this_).db_writer, relationship_id, new_relationship_stereotype, NULL );
            result |= (u8_error_t) data_database_writer_update_relationship_name ( (*this_).db_writer, relationship_id, new_relationship_name, NULL );
            result |= (u8_error_t) data_database_writer_update_relationship_description ( (*this_).db_writer, relationship_id, new_relationship_description, NULL );
            result |= (u8_error_t) data_database_writer_update_relationship_list_order ( (*this_).db_writer, relationship_id, new_relationship_list_order, NULL );
        }
        break;

        case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_RELATIONSHIP:
        {
            U8_TRACE_INFO( "CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_RELATIONSHIP" );
            const data_relationship_t *relation = ctrl_undo_redo_entry_get_relationship_after_action_const ( action );
            if ( undo )
            {
                data_row_t obj_id = data_relationship_get_row_id ( relation );
                result |= (u8_error_t) data_database_writer_delete_relationship ( (*this_).db_writer, obj_id, NULL );
            }
            else
            {
                result |= (u8_error_t) data_database_writer_create_relationship ( (*this_).db_writer, relation, NULL );
            }
        }
        break;

        default:
        {
            U8_LOG_ERROR( "unexptected ctrl_undo_redo_entry_type_t" );
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2025 Andreas Warnke

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
