/* File: ctrl_classifier_controller.c; Copyright and License: see below */

#include "ctrl_classifier_controller.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"

void ctrl_classifier_controller_init ( ctrl_classifier_controller_t *this_,
                                       ctrl_undo_redo_list_t *undo_redo_list,
                                       ctrl_classifier_policy_enforcer_t *policy_enforcer,
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
                                                          data_row_id_t* out_new_id )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_classifier );
    data_classifier_t to_be_created;
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_row_id_t new_id;

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
    result = (u8_error_t) data_result;

    data_classifier_destroy( &to_be_created );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_delete_classifier( ctrl_classifier_controller_t *this_,
                                                         data_row_id_t obj_id,
                                                         ctrl_undo_redo_action_boundary_t add_to_latest_undo_set )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;

    /* check if the classifier is still referenced by diagramelements */
    bool is_still_referenced = true;
    {
        data_diagram_t out_diagram[1];
        uint32_t out_diagram_count;
        data_result = data_database_reader_get_diagrams_by_classifier_id ( (*this_).db_reader,
                                                                           obj_id,
                                                                           1,
                                                                           &out_diagram,
                                                                           &out_diagram_count
                                                                         );

        if ( U8_ERROR_ARRAY_BUFFER_EXCEEDED == data_result )
        {
            is_still_referenced = true;
        }
        else if ( U8_ERROR_NONE == data_result )
        {
            is_still_referenced = ( out_diagram_count == 0 ) ? false : true;
        }
        else
        {
            /* some other error */
            result |= (u8_error_t) data_result;
        }
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
            bool no_more_features = false;
            static const uint32_t MAX_FEATURES_PER_CLASSIFIER = 10000;
            for ( uint32_t feature_count = 0; ( feature_count < MAX_FEATURES_PER_CLASSIFIER ) && ( no_more_features == false ); feature_count ++ )
            {
                data_feature_t out_feature[1];
                uint32_t out_feature_count;
                data_result = data_database_reader_get_features_by_classifier_id ( (*this_).db_reader,
                                                                                   obj_id,
                                                                                   1,
                                                                                   &out_feature,
                                                                                   &out_feature_count
                                                                                 );

                if (( U8_ERROR_ARRAY_BUFFER_EXCEEDED == data_result ) || ( out_feature_count == 1 ))
                {
                    data_result = data_database_writer_delete_feature( (*this_).db_writer, data_feature_get_row_id( &(out_feature[0]) ), NULL );

                    result |= (u8_error_t) data_result;
                    if ( U8_ERROR_NONE == data_result )
                    {
                        /* store the deleted feature to the undo redo list */
                        ctrl_undo_redo_list_add_delete_feature( (*this_).undo_redo_list, &(out_feature[0]) );
                    }
                    data_feature_destroy( &(out_feature[0]) );
                }
                else
                {
                    result |= (u8_error_t) data_result;
                    no_more_features = true;
                }
            }

        }

        /* delete all relationships */
        {
            bool no_more_relationships = false;
            static const uint32_t MAX_RELATIONSHIPS_PER_CLASSIFIER = 10000;
            for ( uint32_t relationship_count = 0; ( relationship_count < MAX_RELATIONSHIPS_PER_CLASSIFIER ) && ( no_more_relationships == false ); relationship_count ++ )
            {

                data_relationship_t out_relationship[1];
                uint32_t out_relationship_count;
                data_result = data_database_reader_get_relationships_by_classifier_id ( (*this_).db_reader,
                                                                                        obj_id,
                                                                                        1,
                                                                                        &out_relationship,
                                                                                        &out_relationship_count
                                                                                      );

                if (( U8_ERROR_ARRAY_BUFFER_EXCEEDED == data_result ) || ( out_relationship_count == 1 ))
                {
                    data_result = data_database_writer_delete_relationship( (*this_).db_writer, data_relationship_get_row_id( &(out_relationship[0]) ), NULL );

                    result |= (u8_error_t) data_result;

                    if ( U8_ERROR_NONE == data_result )
                    {
                        /* store the deleted relationship to the undo redo list */
                        ctrl_undo_redo_list_add_delete_relationship( (*this_).undo_redo_list, &(out_relationship[0]) );
                    }
                    data_relationship_destroy( &(out_relationship[0]) );
                }
                else
                {
                    result |= (u8_error_t) data_result;
                    no_more_relationships = true;
                }
            }
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
            else if ( U8_ERROR_NONE != ( data_result & U8_ERROR_OBJECT_STILL_REFERENCED ))
            {
                /* report this unexpected error */
                U8_LOG_ERROR( "The classifier cannot be deleted because it is still referenced." );
                result |= (u8_error_t) data_result;
            }
            else
            {
                /* report this unexpected error */
                result |= (u8_error_t) data_result;
            }
        }

        /* add a boundary to the undo redo list */
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_main_type ( ctrl_classifier_controller_t *this_,
                                                                    data_row_id_t classifier_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_stereotype ( ctrl_classifier_controller_t *this_,
                                                                     data_row_id_t classifier_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_name ( ctrl_classifier_controller_t *this_,
                                                               data_row_id_t classifier_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_description ( ctrl_classifier_controller_t *this_,
                                                                      data_row_id_t classifier_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_x_order ( ctrl_classifier_controller_t *this_,
                                                                  data_row_id_t classifier_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_y_order ( ctrl_classifier_controller_t *this_,
                                                                  data_row_id_t classifier_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_x_order_y_order ( ctrl_classifier_controller_t *this_,
                                                                          data_row_id_t classifier_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_classifier_list_order ( ctrl_classifier_controller_t *this_,
                                                                     data_row_id_t classifier_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ FEATURE ================================ */

u8_error_t ctrl_classifier_controller_create_feature ( ctrl_classifier_controller_t *this_,
                                                       const data_feature_t *new_feature,
                                                       ctrl_undo_redo_action_boundary_t add_to_latest_undo_set,
                                                       data_row_id_t* out_new_id )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_feature );
    data_feature_t to_be_created;
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_row_id_t new_id;

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
    result = (u8_error_t) data_result;

    data_feature_destroy( &to_be_created );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_delete_feature ( ctrl_classifier_controller_t *this_,
                                                       data_row_id_t obj_id,
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
        bool no_more_relationships = false;
        static const uint32_t MAX_RELATIONSHIPS_PER_CLASSIFIER = 10000;
        for ( uint32_t relationship_count = 0; ( relationship_count < MAX_RELATIONSHIPS_PER_CLASSIFIER ) && ( no_more_relationships == false ); relationship_count ++ )
        {
            data_relationship_t out_relationship[1];
            uint32_t out_relationship_count;
            data_result = data_database_reader_get_relationships_by_feature_id ( (*this_).db_reader,
                                                                                 obj_id,
                                                                                 1,
                                                                                 &out_relationship,
                                                                                 &out_relationship_count
                                                                               );

            if (( U8_ERROR_ARRAY_BUFFER_EXCEEDED == data_result ) || ( out_relationship_count == 1 ))
            {
                data_result = data_database_writer_delete_relationship( (*this_).db_writer, data_relationship_get_row_id( &(out_relationship[0]) ), NULL );

                result |= (u8_error_t) data_result;

                if ( U8_ERROR_NONE == data_result )
                {
                    /* store the deleted relationship to the undo redo list */
                    ctrl_undo_redo_list_add_delete_relationship( (*this_).undo_redo_list, &(out_relationship[0]) );
                }
                data_relationship_destroy( &(out_relationship[0]) );
            }
            else
            {
                result |= (u8_error_t) data_result;
                no_more_relationships = true;
            }
        }
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

        result |= (u8_error_t) feature_result;
    }

    /* add boundary to undo-redo-list */
    ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

    /* apply policy rules */
    if ( U8_ERROR_NONE == feature_result )
    {
        result |= ctrl_classifier_policy_enforcer_post_delete_feature ( (*this_).policy_enforcer, &old_feature );
        data_feature_destroy( &old_feature );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_feature_main_type ( ctrl_classifier_controller_t *this_,
                                                                 data_row_id_t feature_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_feature_key ( ctrl_classifier_controller_t *this_,
                                                           data_row_id_t feature_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_feature_value ( ctrl_classifier_controller_t *this_,
                                                             data_row_id_t feature_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_feature_description ( ctrl_classifier_controller_t *this_,
                                                                   data_row_id_t feature_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_feature_list_order ( ctrl_classifier_controller_t *this_,
                                                                  data_row_id_t feature_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ RELATIONSHIP ================================ */

u8_error_t ctrl_classifier_controller_create_relationship ( ctrl_classifier_controller_t *this_,
                                                            const data_relationship_t *new_relationship,
                                                            ctrl_undo_redo_action_boundary_t add_to_latest_undo_set,
                                                            data_row_id_t* out_new_id )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_relationship );
    data_relationship_t to_be_created;
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_row_id_t new_id;

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
    result = (u8_error_t) data_result;

    data_relationship_destroy( &to_be_created );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_delete_relationship ( ctrl_classifier_controller_t *this_,
                                                            data_row_id_t obj_id,
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

    result |= (u8_error_t) current_result5;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_relationship_main_type ( ctrl_classifier_controller_t *this_,
                                                                      data_row_id_t relationship_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_relationship_stereotype ( ctrl_classifier_controller_t *this_,
                                                                       data_row_id_t relationship_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_relationship_name ( ctrl_classifier_controller_t *this_,
                                                                 data_row_id_t relationship_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_relationship_description ( ctrl_classifier_controller_t *this_,
                                                                        data_row_id_t relationship_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_classifier_controller_update_relationship_list_order ( ctrl_classifier_controller_t *this_,
                                                                       data_row_id_t relationship_id,
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
    result = (u8_error_t) data_result;

    U8_TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2023 Andreas Warnke

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
