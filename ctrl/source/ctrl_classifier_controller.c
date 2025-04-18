/* File: ctrl_classifier_controller.c; Copyright and License: see below */

#include "ctrl_classifier_controller.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"

void ctrl_classifier_controller_init ( ctrl_classifier_controller_t *this_,
                                       ctrl_undo_redo_list_t *undo_redo_list,
                                       ctrl_classifier_trigger_t *policy_enforcer,
                                       data_database_t *database,
                                       data_database_reader_t *db_reader,
                                       data_database_writer_t *db_writer )
{
    U8_TRACE_BEGIN();

    (*this_).undo_redo_list = undo_redo_list;
    (*this_).policy_enforcer = policy_enforcer;
    (*this_).database = database;
    (*this_).db_reader = db_reader;
    (*this_).db_writer = db_writer;
    ctrl_consistency_checker_init ( &((*this_).consistency_checker), database, db_reader, db_writer );

    U8_TRACE_END();
}

void ctrl_classifier_controller_destroy ( ctrl_classifier_controller_t *this_ )
{
    U8_TRACE_BEGIN();

    ctrl_consistency_checker_destroy ( &((*this_).consistency_checker) );
    (*this_).undo_redo_list = NULL;
    (*this_).policy_enforcer = NULL;
    (*this_).database = NULL;
    (*this_).db_reader = NULL;
    (*this_).db_writer = NULL;

    U8_TRACE_END();
}

/* ================================ CLASSIFIER ================================ */

u8_error_t ctrl_classifier_controller_create_classifier ( ctrl_classifier_controller_t *this_,
                                                          const data_classifier_t *new_classifier,
                                                          ctrl_undo_redo_action_boundary_t add_to_latest_undo_set,
                                                          data_row_t* out_new_id )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_classifier );
    data_classifier_t to_be_created;
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_row_t new_id;

    data_classifier_copy( &to_be_created, new_classifier );

    data_result = data_database_writer_create_classifier( (*this_).db_writer, &to_be_created, &new_id );
    if ( U8_ERROR_NONE == data_result )
    {
        /* store new id to data_classifier_t object */
        data_classifier_set_row_id( &to_be_created, new_id );

        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set == CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND )
        {
            u8_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( U8_ERROR_NONE != internal_err )
            {
                U8_LOG_ERROR_HEX( "unexpected internal error", internal_err );
            }
        }

        /* store the new diagram to the undo redo list */
        ctrl_undo_redo_list_add_create_classifier( (*this_).undo_redo_list, &to_be_created );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        /* copy new id to out parameter */
        if ( NULL != out_new_id )
        {
            *out_new_id = new_id;
        }
    }
    result = data_result;

    data_classifier_destroy( &to_be_created );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_delete_classifier( ctrl_classifier_controller_t *this_,
                                                         data_row_t obj_id,
                                                         ctrl_undo_redo_action_boundary_t add_to_latest_undo_set )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;

    /* check if the classifier is still referenced by diagramelements */
    bool is_still_referenced = true;
    {
        data_diagram_iterator_t diagram_iterator;
        result |= data_diagram_iterator_init_empty( &diagram_iterator );
        result |= data_database_reader_get_diagrams_by_classifier_id( (*this_).db_reader,
                                                                      obj_id,
                                                                      &diagram_iterator
                                                                    );

        if (( data_diagram_iterator_has_next( &diagram_iterator ) )||( result != U8_ERROR_NONE ))
        {
            is_still_referenced = true;
        }
        else
        {
            is_still_referenced = false;
        }
        result |= data_diagram_iterator_destroy( &diagram_iterator );
    }

    /* if the classifier is still referenced by diagramelements, do not do anything, report an error */
    if ( is_still_referenced )
    {
        result |= U8_ERROR_OBJECT_STILL_REFERENCED;
    }
    else
    {
        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set == CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND )
        {
            u8_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( U8_ERROR_NONE != internal_err )
            {
                U8_LOG_ERROR_HEX( "unexpected internal error", internal_err );
            }
        }

        /* delete all features */
        {
            data_feature_iterator_t feature_iterator;
            result |= data_feature_iterator_init_empty( &feature_iterator );
            result |= data_database_reader_get_features_by_classifier_id( (*this_).db_reader,
                                                                          obj_id,
                                                                          &feature_iterator
                                                                        );
            while ( data_feature_iterator_has_next( &feature_iterator ) )
            {
                result |= data_feature_iterator_next( &feature_iterator, &((*this_).temp_feature) );
                const data_row_t feat_id = data_feature_get_row_id( &((*this_).temp_feature) );
                data_result = data_database_writer_delete_feature( (*this_).db_writer, feat_id, NULL );
                if ( data_result == U8_ERROR_NONE )
                {
                    /* store the deleted feature to the undo redo list */
                    ctrl_undo_redo_list_add_delete_feature( (*this_).undo_redo_list, &((*this_).temp_feature) );
                }
                else
                {
                    /* report this unexpected error */
                    result |= data_result;
                }
                data_feature_destroy( &((*this_).temp_feature) );
            }
            result |= data_feature_iterator_destroy( &feature_iterator );
        }

        /* delete all relationships */
        {
            data_relationship_iterator_t relationship_iterator;
            result |= data_relationship_iterator_init_empty( &relationship_iterator );
            result |= data_database_reader_get_relationships_by_classifier_id( (*this_).db_reader,
                                                                               obj_id,
                                                                               &relationship_iterator
                                                                             );
            while ( data_relationship_iterator_has_next( &relationship_iterator ) )
            {
                result |= data_relationship_iterator_next( &relationship_iterator, &((*this_).temp_relationship) );
                const data_row_t rel_id = data_relationship_get_row_id( &((*this_).temp_relationship) );
                data_result = data_database_writer_delete_relationship( (*this_).db_writer, rel_id, NULL );
                if ( data_result == U8_ERROR_NONE )
                {
                    /* store the deleted relationship to the undo redo list */
                    ctrl_undo_redo_list_add_delete_relationship( (*this_).undo_redo_list, &((*this_).temp_relationship) );
                }
                else
                {
                    /* report this unexpected error */
                    result |= data_result;
                }
                data_relationship_destroy( &((*this_).temp_relationship) );
            }
            result |= data_relationship_iterator_destroy( &relationship_iterator );
        }

        /* delete the classifier */
        {
            data_classifier_t old_classifier;
            data_result = data_database_writer_delete_classifier( (*this_).db_writer,
                                                                  obj_id,
                                                                  &old_classifier
                                                                );

            if ( U8_ERROR_NONE == data_result )
            {
                /* store the deleted classifier to the undo redo list */
                ctrl_undo_redo_list_add_delete_classifier( (*this_).undo_redo_list, &old_classifier );

                data_classifier_destroy( &old_classifier );
            }
            else if ( u8_error_contains( data_result, U8_ERROR_OBJECT_STILL_REFERENCED ) )
            {
                /* report this unexpected error */
                U8_LOG_ERROR( "The classifier cannot be deleted because it is still referenced." );
                result |= data_result;
            }
            else
            {
                /* report this unexpected error */
                result |= data_result;
            }
        }

        /* add a boundary to the undo redo list */
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_main_type ( ctrl_classifier_controller_t *this_,
                                                                    data_row_t classifier_id,
                                                                    data_classifier_type_t new_classifier_main_type )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_main_type( (*this_).db_writer, classifier_id, new_classifier_main_type, &old_classifier );
    if ( U8_ERROR_NONE == data_result )
    {
        /* prepare the new classifier */
        data_classifier_t new_classifier;
        data_classifier_copy( &new_classifier, &old_classifier );
        data_classifier_set_main_type( &new_classifier, new_classifier_main_type );
        /* store the change of the classifier to the undo redo list */
        ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_classifier_destroy( &new_classifier );
        data_classifier_destroy( &old_classifier );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_stereotype ( ctrl_classifier_controller_t *this_,
                                                                     data_row_t classifier_id,
                                                                     const char* new_classifier_stereotype )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_stereotype( (*this_).db_writer, classifier_id, new_classifier_stereotype, &old_classifier );
    if  (( U8_ERROR_NONE == data_result ) || ( U8_ERROR_STRING_BUFFER_EXCEEDED == data_result ))
    {
        /* prepare the new classifier */
        data_classifier_t new_classifier;
        data_classifier_copy( &new_classifier, &old_classifier );
        data_classifier_set_stereotype( &new_classifier, new_classifier_stereotype );
        /* store the change of the classifier to the undo redo list */
        ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_classifier_destroy( &new_classifier );
        data_classifier_destroy( &old_classifier );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_name ( ctrl_classifier_controller_t *this_,
                                                               data_row_t classifier_id,
                                                               const char* new_classifier_name )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_name( (*this_).db_writer, classifier_id, new_classifier_name, &old_classifier );
    if  (( U8_ERROR_NONE == data_result ) || ( U8_ERROR_STRING_BUFFER_EXCEEDED == data_result ))
    {
        /* prepare the new classifier */
        data_classifier_t new_classifier;
        data_classifier_copy( &new_classifier, &old_classifier );
        data_classifier_set_name( &new_classifier, new_classifier_name );
        /* store the change of the classifier to the undo redo list */
        ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_classifier_destroy( &new_classifier );
        data_classifier_destroy( &old_classifier );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_description ( ctrl_classifier_controller_t *this_,
                                                                      data_row_t classifier_id,
                                                                      const char* new_classifier_description )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_description( (*this_).db_writer, classifier_id, new_classifier_description, &old_classifier );
    if (( U8_ERROR_NONE == data_result ) || ( U8_ERROR_STRING_BUFFER_EXCEEDED == data_result ))
    {
        /* prepare the new classifier */
        data_classifier_t new_classifier;
        data_classifier_copy( &new_classifier, &old_classifier );
        data_classifier_set_description( &new_classifier, new_classifier_description );
        /* store the change of the classifier to the undo redo list */
        ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_classifier_destroy( &new_classifier );
        data_classifier_destroy( &old_classifier );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_x_order ( ctrl_classifier_controller_t *this_,
                                                                  data_row_t classifier_id,
                                                                  int32_t new_classifier_x_order )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_x_order( (*this_).db_writer, classifier_id, new_classifier_x_order, &old_classifier );
    if ( U8_ERROR_NONE == data_result )
    {
        /* prepare the new classifier */
        data_classifier_t new_classifier;
        data_classifier_copy( &new_classifier, &old_classifier );
        data_classifier_set_x_order( &new_classifier, new_classifier_x_order );
        /* store the change of the classifier to the undo redo list */
        ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_classifier_destroy( &new_classifier );
        data_classifier_destroy( &old_classifier );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_y_order ( ctrl_classifier_controller_t *this_,
                                                                  data_row_t classifier_id,
                                                                  int32_t new_classifier_y_order )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_y_order( (*this_).db_writer, classifier_id, new_classifier_y_order, &old_classifier );
    if ( U8_ERROR_NONE == data_result )
    {
        /* prepare the new classifier */
        data_classifier_t new_classifier;
        data_classifier_copy( &new_classifier, &old_classifier );
        data_classifier_set_y_order( &new_classifier, new_classifier_y_order );
        /* store the change of the classifier to the undo redo list */
        ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_classifier_destroy( &new_classifier );
        data_classifier_destroy( &old_classifier );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_x_order_y_order ( ctrl_classifier_controller_t *this_,
                                                                          data_row_t classifier_id,
                                                                          int32_t new_classifier_x_order,
                                                                          int32_t new_classifier_y_order )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_x_order( (*this_).db_writer, classifier_id, new_classifier_x_order, &old_classifier );
    if ( U8_ERROR_NONE == data_result )
    {
        data_result = data_database_writer_update_classifier_y_order( (*this_).db_writer, classifier_id, new_classifier_y_order, NULL );
        if ( U8_ERROR_NONE == data_result )
        {
            /* prepare the new classifier */
            data_classifier_t new_classifier;
            data_classifier_copy( &new_classifier, &old_classifier );
            data_classifier_set_x_order( &new_classifier, new_classifier_x_order );
            data_classifier_set_y_order( &new_classifier, new_classifier_y_order );
            /* store the change of the classifier to the undo redo list */
            ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
            ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

            data_classifier_destroy( &new_classifier );
        }
        data_classifier_destroy( &old_classifier );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_list_order ( ctrl_classifier_controller_t *this_,
                                                                     data_row_t classifier_id,
                                                                     int32_t new_classifier_list_order )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_list_order( (*this_).db_writer, classifier_id, new_classifier_list_order, &old_classifier );
    if ( U8_ERROR_NONE == data_result )
    {
        /* prepare the new classifier */
        data_classifier_t new_classifier;
        data_classifier_copy( &new_classifier, &old_classifier );
        data_classifier_set_list_order( &new_classifier, new_classifier_list_order );
        /* store the change of the classifier to the undo redo list */
        ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_classifier_destroy( &new_classifier );
        data_classifier_destroy( &old_classifier );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ FEATURE ================================ */

u8_error_t ctrl_classifier_controller_create_feature ( ctrl_classifier_controller_t *this_,
                                                       const data_feature_t *new_feature,
                                                       ctrl_undo_redo_action_boundary_t add_to_latest_undo_set,
                                                       data_row_t* out_new_id )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_feature );
    data_feature_t to_be_created;
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_row_t new_id;

    data_feature_copy( &to_be_created, new_feature );

    data_result = data_database_writer_create_feature( (*this_).db_writer, &to_be_created, &new_id );
    if ( U8_ERROR_NONE == data_result )
    {
        /* store new id to data_feature_t object */
        data_feature_set_row_id( &to_be_created, new_id );

        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set == CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND )
        {
            u8_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( U8_ERROR_NONE != internal_err )
            {
                U8_LOG_ERROR_HEX( "unexpected internal error", internal_err );
            }
        }

        /* store the new feature to the undo redo list */
        ctrl_undo_redo_list_add_create_feature( (*this_).undo_redo_list, &to_be_created );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        /* copy new id to out parameter */
        if ( NULL != out_new_id )
        {
            *out_new_id = new_id;
        }
    }
    result = data_result;

    data_feature_destroy( &to_be_created );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_delete_feature ( ctrl_classifier_controller_t *this_,
                                                       data_row_t obj_id,
                                                       ctrl_undo_redo_action_boundary_t add_to_latest_undo_set )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;

    /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
    if ( add_to_latest_undo_set == CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND )
    {
        u8_error_t internal_err;
        internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
        if ( U8_ERROR_NONE != internal_err )
        {
            U8_LOG_ERROR_HEX( "unexpected internal error", internal_err );
        }
    }

    /* delete all relationships to and/or from this feature */
    {
        data_relationship_iterator_t relationship_iterator;
        result |= data_relationship_iterator_init_empty( &relationship_iterator );
        result |= data_database_reader_get_relationships_by_feature_id( (*this_).db_reader,
                                                                        obj_id,
                                                                        &relationship_iterator
                                                                      );
        while ( data_relationship_iterator_has_next( &relationship_iterator ) )
        {
            result |= data_relationship_iterator_next( &relationship_iterator, &((*this_).temp_relationship) );
            const data_row_t rel_id = data_relationship_get_row_id( &((*this_).temp_relationship) );
            data_result = data_database_writer_delete_relationship( (*this_).db_writer, rel_id, NULL );
            if ( U8_ERROR_NONE == data_result )
            {
                /* store the deleted relationship to the undo redo list */
                ctrl_undo_redo_list_add_delete_relationship( (*this_).undo_redo_list, &((*this_).temp_relationship) );
            }
            else
            {
                /* report this unexpected error */
                result |= data_result;
            }
            data_relationship_destroy( &((*this_).temp_relationship) );
        }
        result |= data_relationship_iterator_destroy( &relationship_iterator );
    }

    /* delete the feature */
    data_feature_t old_feature;
    u8_error_t feature_result;
    {
        /* delete feature */
        feature_result = data_database_writer_delete_feature( (*this_).db_writer, obj_id, &old_feature );

        if ( U8_ERROR_NONE == feature_result )
        {
            /* store the deleted feature to the undo redo list */
            ctrl_undo_redo_list_add_delete_feature( (*this_).undo_redo_list, &old_feature );
        }

        result |= feature_result;
    }

    /* add boundary to undo-redo-list */
    ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

    /* apply policy rules */
    if ( U8_ERROR_NONE == feature_result )
    {
        result |= ctrl_classifier_trigger_post_delete_feature ( (*this_).policy_enforcer, &old_feature );
        data_feature_destroy( &old_feature );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_feature_main_type ( ctrl_classifier_controller_t *this_,
                                                                 data_row_t feature_id,
                                                                 data_feature_type_t new_feature_type )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_feature_t old_feature;

    data_result = data_database_writer_update_feature_main_type( (*this_).db_writer, feature_id, new_feature_type, &old_feature );
    if ( U8_ERROR_NONE == data_result )
    {
        /* prepare the new feature */
        data_feature_t new_feature;
        data_feature_copy( &new_feature, &old_feature );
        data_feature_set_main_type( &new_feature, new_feature_type );
        /* store the change of the feature to the undo redo list */
        ctrl_undo_redo_list_add_update_feature( (*this_).undo_redo_list, &old_feature, &new_feature );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_feature_destroy( &new_feature );
        data_feature_destroy( &old_feature );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_feature_key ( ctrl_classifier_controller_t *this_,
                                                           data_row_t feature_id,
                                                           const char* new_feature_key )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_feature_t old_feature;

    data_result = data_database_writer_update_feature_key( (*this_).db_writer, feature_id, new_feature_key, &old_feature );
    if  (( U8_ERROR_NONE == data_result ) || ( U8_ERROR_STRING_BUFFER_EXCEEDED == data_result ))
    {
        /* prepare the new feature */
        data_feature_t new_feature;
        data_feature_copy( &new_feature, &old_feature );
        data_feature_set_key( &new_feature, new_feature_key );
        /* store the change of the feature to the undo redo list */
        ctrl_undo_redo_list_add_update_feature( (*this_).undo_redo_list, &old_feature, &new_feature );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_feature_destroy( &new_feature );
        data_feature_destroy( &old_feature );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_feature_value ( ctrl_classifier_controller_t *this_,
                                                             data_row_t feature_id,
                                                             const char* new_feature_value )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_feature_t old_feature;

    data_result = data_database_writer_update_feature_value( (*this_).db_writer, feature_id, new_feature_value, &old_feature );
    if  (( U8_ERROR_NONE == data_result ) || ( U8_ERROR_STRING_BUFFER_EXCEEDED == data_result ))
    {
        /* prepare the new feature */
        data_feature_t new_feature;
        data_feature_copy( &new_feature, &old_feature );
        data_feature_set_value( &new_feature, new_feature_value );
        /* store the change of the feature to the undo redo list */
        ctrl_undo_redo_list_add_update_feature( (*this_).undo_redo_list, &old_feature, &new_feature );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_feature_destroy( &new_feature );
        data_feature_destroy( &old_feature );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_feature_description ( ctrl_classifier_controller_t *this_,
                                                                   data_row_t feature_id,
                                                                   const char* new_feature_description )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_feature_t old_feature;

    data_result = data_database_writer_update_feature_description( (*this_).db_writer, feature_id, new_feature_description, &old_feature );
    if  (( U8_ERROR_NONE == data_result ) || ( U8_ERROR_STRING_BUFFER_EXCEEDED == data_result ))
    {
        /* prepare the new feature */
        data_feature_t new_feature;
        data_feature_copy( &new_feature, &old_feature );
        data_feature_set_description( &new_feature, new_feature_description );
        /* store the change of the feature to the undo redo list */
        ctrl_undo_redo_list_add_update_feature( (*this_).undo_redo_list, &old_feature, &new_feature );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_feature_destroy( &new_feature );
        data_feature_destroy( &old_feature );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_feature_list_order ( ctrl_classifier_controller_t *this_,
                                                                  data_row_t feature_id,
                                                                  int32_t new_feature_list_order )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_feature_t old_feature;

    data_result = data_database_writer_update_feature_list_order( (*this_).db_writer, feature_id, new_feature_list_order, &old_feature );
    if ( U8_ERROR_NONE == data_result )
    {
        /* prepare the new feature */
        data_feature_t new_feature;
        data_feature_copy( &new_feature, &old_feature );
        data_feature_set_list_order( &new_feature, new_feature_list_order );
        /* store the change of the feature to the undo redo list */
        ctrl_undo_redo_list_add_update_feature( (*this_).undo_redo_list, &old_feature, &new_feature );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_feature_destroy( &new_feature );
        data_feature_destroy( &old_feature );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ RELATIONSHIP ================================ */

u8_error_t ctrl_classifier_controller_create_relationship ( ctrl_classifier_controller_t *this_,
                                                            const data_relationship_t *new_relationship,
                                                            ctrl_undo_redo_action_boundary_t add_to_latest_undo_set,
                                                            data_row_t* out_new_id )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_relationship );
    data_relationship_t to_be_created;
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_row_t new_id;

    data_relationship_copy( &to_be_created, new_relationship );

    data_result = data_database_writer_create_relationship( (*this_).db_writer, &to_be_created, &new_id );
    if ( U8_ERROR_NONE == data_result )
    {
        /* store new id to data_relationship_t object */
        data_relationship_set_row_id( &to_be_created, new_id );

        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set == CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND )
        {
            u8_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( U8_ERROR_NONE != internal_err )
            {
                U8_LOG_ERROR_HEX( "unexpected internal error", internal_err );
            }
        }

        /* store the new relationship to the undo redo list */
        ctrl_undo_redo_list_add_create_relationship( (*this_).undo_redo_list, &to_be_created );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        /* copy new id to out parameter */
        if ( NULL != out_new_id )
        {
            *out_new_id = new_id;
        }
    }
    result = data_result;

    data_relationship_destroy( &to_be_created );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_delete_relationship ( ctrl_classifier_controller_t *this_,
                                                            data_row_t obj_id,
                                                            ctrl_undo_redo_action_boundary_t add_to_latest_undo_set )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    /* delete relationship */
    data_relationship_t old_relation;
    u8_error_t current_result5;
    current_result5 = data_database_writer_delete_relationship( (*this_).db_writer, obj_id, &old_relation );

    if ( U8_ERROR_NONE == current_result5 )
    {
        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set == CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND )
        {
            u8_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( U8_ERROR_NONE != internal_err )
            {
                U8_LOG_ERROR_HEX( "unexpected internal error", internal_err );
            }
        }

        /* store the deleted relationship to the undo redo list */
        ctrl_undo_redo_list_add_delete_relationship( (*this_).undo_redo_list, &old_relation );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_relationship_destroy( &old_relation );
    }

    result |= current_result5;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_relationship_main_type ( ctrl_classifier_controller_t *this_,
                                                                      data_row_t relationship_id,
                                                                      data_relationship_type_t new_relationship_type )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_relationship_t old_relation;

    data_result = data_database_writer_update_relationship_main_type( (*this_).db_writer, relationship_id, new_relationship_type, &old_relation );
    if ( U8_ERROR_NONE == data_result )
    {
        /* prepare the new relation */
        data_relationship_t new_relation;
        data_relationship_copy( &new_relation, &old_relation );
        data_relationship_set_main_type( &new_relation, new_relationship_type );
        /* store the change of the relation to the undo redo list */
        ctrl_undo_redo_list_add_update_relationship( (*this_).undo_redo_list, &old_relation, &new_relation );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_relationship_destroy( &new_relation );
        data_relationship_destroy( &old_relation );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_relationship_stereotype ( ctrl_classifier_controller_t *this_,
                                                                       data_row_t relationship_id,
                                                                       const char* new_relationship_stereotype )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_relationship_t old_relation;

    data_result = data_database_writer_update_relationship_stereotype( (*this_).db_writer, relationship_id, new_relationship_stereotype, &old_relation );
    if  (( U8_ERROR_NONE == data_result ) || ( U8_ERROR_STRING_BUFFER_EXCEEDED == data_result ))
    {
        /* prepare the new relation */
        data_relationship_t new_relation;
        data_relationship_copy( &new_relation, &old_relation );
        data_relationship_set_stereotype( &new_relation, new_relationship_stereotype );
        /* store the change of the relation to the undo redo list */
        ctrl_undo_redo_list_add_update_relationship( (*this_).undo_redo_list, &old_relation, &new_relation );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_relationship_destroy( &new_relation );
        data_relationship_destroy( &old_relation );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_relationship_name ( ctrl_classifier_controller_t *this_,
                                                                 data_row_t relationship_id,
                                                                 const char* new_relationship_name )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_relationship_t old_relation;

    data_result = data_database_writer_update_relationship_name( (*this_).db_writer, relationship_id, new_relationship_name, &old_relation );
    if  (( U8_ERROR_NONE == data_result ) || ( U8_ERROR_STRING_BUFFER_EXCEEDED == data_result ))
    {
        /* prepare the new relation */
        data_relationship_t new_relation;
        data_relationship_copy( &new_relation, &old_relation );
        data_relationship_set_name( &new_relation, new_relationship_name );
        /* store the change of the relation to the undo redo list */
        ctrl_undo_redo_list_add_update_relationship( (*this_).undo_redo_list, &old_relation, &new_relation );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_relationship_destroy( &new_relation );
        data_relationship_destroy( &old_relation );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_relationship_description ( ctrl_classifier_controller_t *this_,
                                                                        data_row_t relationship_id,
                                                                        const char* new_relationship_description )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_relationship_t old_relation;

    data_result = data_database_writer_update_relationship_description( (*this_).db_writer, relationship_id, new_relationship_description, &old_relation );
    if  (( U8_ERROR_NONE == data_result ) || ( U8_ERROR_STRING_BUFFER_EXCEEDED == data_result ))
    {
        /* prepare the new relation */
        data_relationship_t new_relation;
        data_relationship_copy( &new_relation, &old_relation );
        data_relationship_set_description( &new_relation, new_relationship_description );
        /* store the change of the relation to the undo redo list */
        ctrl_undo_redo_list_add_update_relationship( (*this_).undo_redo_list, &old_relation, &new_relation );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_relationship_destroy( &new_relation );
        data_relationship_destroy( &old_relation );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_relationship_list_order ( ctrl_classifier_controller_t *this_,
                                                                       data_row_t relationship_id,
                                                                       int32_t new_relationship_list_order )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_relationship_t old_relation;

    data_result = data_database_writer_update_relationship_list_order( (*this_).db_writer, relationship_id, new_relationship_list_order, &old_relation );
    if ( U8_ERROR_NONE == data_result )
    {
        /* prepare the new relation */
        data_relationship_t new_relation;
        data_relationship_copy( &new_relation, &old_relation );
        data_relationship_set_list_order( &new_relation, new_relationship_list_order );
        /* store the change of the relation to the undo redo list */
        ctrl_undo_redo_list_add_update_relationship( (*this_).undo_redo_list, &old_relation, &new_relation );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_relationship_destroy( &new_relation );
        data_relationship_destroy( &old_relation );
    }
    result = data_result;

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
