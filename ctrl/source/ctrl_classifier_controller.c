/* File: ctrl_classifier_controller.c; Copyright and License: see below */

#include "ctrl_classifier_controller.h"
#include "trace.h"
#include "tslog.h"

void ctrl_classifier_controller_init ( ctrl_classifier_controller_t *this_,
                                       ctrl_undo_redo_list_t *undo_redo_list,
                                       data_database_t *database,
                                       data_database_reader_t *db_reader,
                                       data_database_writer_t *db_writer )
{
    TRACE_BEGIN();

    (*this_).undo_redo_list = undo_redo_list;
    (*this_).database = database;
    (*this_).db_reader = db_reader;
    (*this_).db_writer = db_writer;
    ctrl_consistency_checker_init ( &((*this_).consistency_checker), database, db_reader, db_writer );

    TRACE_END();
}

void ctrl_classifier_controller_destroy ( ctrl_classifier_controller_t *this_ )
{
    TRACE_BEGIN();

    ctrl_consistency_checker_destroy ( &((*this_).consistency_checker) );
    (*this_).undo_redo_list = NULL;
    (*this_).database = NULL;
    (*this_).db_reader = NULL;
    (*this_).db_writer = NULL;

    TRACE_END();
}

/* ================================ CLASSIFIER ================================ */

ctrl_error_t ctrl_classifier_controller_create_classifier_in_diagram ( ctrl_classifier_controller_t *this_,
                                                                       int64_t diagram_id,
                                                                       data_classifier_type_t classifier_type,
                                                                       const char* classifier_name,
                                                                       int32_t x_order,
                                                                       int32_t y_order,
                                                                       int64_t* out_new_id )
{
    TRACE_BEGIN();
    data_classifier_t to_be_created;
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    int64_t new_id;

    /* create a classifier row */

    data_result = data_classifier_init_new( &to_be_created, classifier_type, "", classifier_name, "", x_order, y_order );
    result |= (ctrl_error_t) data_result;

    data_result = data_database_writer_create_classifier( (*this_).db_writer, &to_be_created, &new_id );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* store new id to classifier object */
        data_classifier_set_id( &to_be_created, new_id );

        /* store the new classifier to the undo redo list */
        ctrl_undo_redo_list_add_create_classifier( (*this_).undo_redo_list, &to_be_created );

        /* copy new id to out parameter */
        if ( NULL != out_new_id )
        {
            *out_new_id = new_id;
        }
    }
    result |= (ctrl_error_t) data_result;

    data_classifier_destroy( &to_be_created );

    /* create a diagramelement row */
    if ( DATA_ERROR_NONE == data_result )
    {
        data_diagramelement_t link_to_be_created;
        int64_t new_link_id;

        data_diagramelement_init_new( &link_to_be_created, diagram_id, new_id, DATA_DIAGRAMELEMENT_FLAG_NONE );

        data_result = data_database_writer_create_diagramelement( (*this_).db_writer, &link_to_be_created, &new_link_id );
        if ( DATA_ERROR_NONE == data_result )
        {
            /* store new id to diagramelement object */
            data_diagramelement_set_id( &link_to_be_created, new_link_id );

            /* store the new diagramelement to the undo redo list */
            ctrl_undo_redo_list_add_create_diagramelement( (*this_).undo_redo_list, &link_to_be_created );
        }
        result |= (ctrl_error_t) data_result;

        data_diagramelement_destroy( &link_to_be_created );

        /* add a boundary to the undo redo list after two creations */
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
    }

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_classifier_controller_create_classifier ( ctrl_classifier_controller_t *this_,
                                                            const data_classifier_t *new_classifier,
                                                            bool add_to_latest_undo_set,
                                                            int64_t* out_new_id )
{
    TRACE_BEGIN();
    assert( NULL != new_classifier );
    data_classifier_t to_be_created;
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    int64_t new_id;

    data_classifier_copy( &to_be_created, new_classifier );
    data_classifier_set_id( &to_be_created, DATA_ID_VOID_ID );

    data_result = data_database_writer_create_classifier( (*this_).db_writer, &to_be_created, &new_id );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* store new id to data_classifier_t object */
        data_classifier_set_id( &to_be_created, new_id );

        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set )
        {
            ctrl_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( CTRL_ERROR_NONE != internal_err )
            {
                TSLOG_ERROR_HEX( "unexpected internal error", internal_err );
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
    result = (ctrl_error_t) data_result;

    data_classifier_destroy( &to_be_created );

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_classifier_controller_update_classifier_stereotype ( ctrl_classifier_controller_t *this_,
                                                                       int64_t classifier_id,
                                                                       const char* new_classifier_stereotype )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_stereotype( (*this_).db_writer, classifier_id, new_classifier_stereotype, &old_classifier );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* prepare the new classifier */
        data_classifier_t new_classifier;
        data_classifier_copy( &new_classifier, &old_classifier );
        data_classifier_set_stereotype( &new_classifier, new_classifier_stereotype );
        /* store the change of the classifier to the undo redo list */
        ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
    }
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_classifier_controller_update_classifier_description ( ctrl_classifier_controller_t *this_,
                                                                        int64_t classifier_id,
                                                                        const char* new_classifier_description )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_description( (*this_).db_writer, classifier_id, new_classifier_description, &old_classifier );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* prepare the new classifier */
        data_classifier_t new_classifier;
        data_classifier_copy( &new_classifier, &old_classifier );
        data_classifier_set_description( &new_classifier, new_classifier_description );
        /* store the change of the classifier to the undo redo list */
        ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
    }
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_classifier_controller_update_classifier_name ( ctrl_classifier_controller_t *this_,
                                                                 int64_t classifier_id,
                                                                 const char* new_classifier_name )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_name( (*this_).db_writer, classifier_id, new_classifier_name, &old_classifier );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* prepare the new classifier */
        data_classifier_t new_classifier;
        data_classifier_copy( &new_classifier, &old_classifier );
        data_classifier_set_name( &new_classifier, new_classifier_name );
        /* store the change of the classifier to the undo redo list */
        ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
    }
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_classifier_controller_update_classifier_main_type ( ctrl_classifier_controller_t *this_,
                                                                      int64_t classifier_id,
                                                                      data_classifier_type_t new_classifier_main_type )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_main_type( (*this_).db_writer, classifier_id, new_classifier_main_type, &old_classifier );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* prepare the new classifier */
        data_classifier_t new_classifier;
        data_classifier_copy( &new_classifier, &old_classifier );
        data_classifier_set_main_type( &new_classifier, new_classifier_main_type );
        /* store the change of the classifier to the undo redo list */
        ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
    }
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_classifier_controller_update_classifier_x_order ( ctrl_classifier_controller_t *this_,
                                                                    int64_t classifier_id,
                                                                    int32_t new_classifier_x_order )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_x_order( (*this_).db_writer, classifier_id, new_classifier_x_order, &old_classifier );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* prepare the new classifier */
        data_classifier_t new_classifier;
        data_classifier_copy( &new_classifier, &old_classifier );
        data_classifier_set_x_order( &new_classifier, new_classifier_x_order );
        /* store the change of the classifier to the undo redo list */
        ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
    }
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_classifier_controller_update_classifier_y_order ( ctrl_classifier_controller_t *this_,
                                                                    int64_t classifier_id,
                                                                    int32_t new_classifier_y_order )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_y_order( (*this_).db_writer, classifier_id, new_classifier_y_order, &old_classifier );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* prepare the new classifier */
        data_classifier_t new_classifier;
        data_classifier_copy( &new_classifier, &old_classifier );
        data_classifier_set_y_order( &new_classifier, new_classifier_y_order );
        /* store the change of the classifier to the undo redo list */
        ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
    }
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_classifier_controller_update_classifier_x_order_y_order ( ctrl_classifier_controller_t *this_,
                                                                            int64_t classifier_id,
                                                                            int32_t new_classifier_x_order,
                                                                            int32_t new_classifier_y_order )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    data_classifier_t old_classifier;

    data_result = data_database_writer_update_classifier_x_order( (*this_).db_writer, classifier_id, new_classifier_x_order, &old_classifier );
    if ( DATA_ERROR_NONE == data_result )
    {
        data_result = data_database_writer_update_classifier_y_order( (*this_).db_writer, classifier_id, new_classifier_y_order, NULL );
        if ( DATA_ERROR_NONE == data_result )
        {
            /* prepare the new classifier */
            data_classifier_t new_classifier;
            data_classifier_copy( &new_classifier, &old_classifier );
            data_classifier_set_x_order( &new_classifier, new_classifier_x_order );
            data_classifier_set_y_order( &new_classifier, new_classifier_y_order );
            /* store the change of the classifier to the undo redo list */
            ctrl_undo_redo_list_add_update_classifier( (*this_).undo_redo_list, &old_classifier, &new_classifier );
            ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
        }
    }
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

/* ================================ COMMON ================================ */

ctrl_error_t ctrl_classifier_controller_delete_set ( ctrl_classifier_controller_t *this_, data_small_set_t objects )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    if ( data_small_set_is_empty( &objects ) )
    {
        result = CTRL_ERROR_INPUT_EMPTY;
    }
    else
    {
        int index;

        /* STEP ONE: Delete all objects that can be immediately deleted */

        for ( index = 0; index < data_small_set_get_count( &objects ); index ++ )
        {
            data_id_t current_id;
            current_id = data_small_set_get_id( &objects, index );
            switch ( data_id_get_table( &current_id ) )
            {
                case DATA_TABLE_CLASSIFIER:
                {
                    /* see step two */
                }
                break;

                case DATA_TABLE_FEATURE:
                {
                    result |= CTRL_ERROR_NOT_YET_IMPLEMENTED;
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    result |= CTRL_ERROR_NOT_YET_IMPLEMENTED;
                }
                break;

                case DATA_TABLE_DIAGRAMELEMENT:
                {
                    data_error_t current_result;
                    data_diagramelement_t old_diagramelement;
                    data_id_trace( &current_id );
                    current_result = data_database_writer_delete_diagramelement( (*this_).db_writer,
                                                                                data_id_get_row_id( &current_id ),
                                                                                &old_diagramelement
                                                                            );
                    result |= (ctrl_error_t) current_result;

                    if ( DATA_ERROR_NONE == current_result )
                    {
                        /* store the deleted classifier to the undo redo list */
                        ctrl_undo_redo_list_add_delete_diagramelement( (*this_).undo_redo_list, &old_diagramelement );
                    }

                    /* try to also delete the classifier, ignore errors */
                    if ( DATA_ERROR_NONE == current_result )
                    {
                        data_error_t my_data_result;
                        data_classifier_t old_classifier;
                        my_data_result = data_database_writer_delete_classifier( (*this_).db_writer,
                                                                                 data_diagramelement_get_classifier_id( &old_diagramelement ),
                                                                                 &old_classifier
                                                                               );

                        if ( DATA_ERROR_NONE == my_data_result )
                        {
                            /* store the deleted classifier to the undo redo list */
                            ctrl_undo_redo_list_add_delete_classifier( (*this_).undo_redo_list, &old_classifier );
                        }
                    }
                }
                break;

                case DATA_TABLE_DIAGRAM:
                {
                    /* see step two */
                }
                break;

                default:
                {
                    result |= CTRL_ERROR_VALUE_OUT_OF_RANGE;
                }
                break;
            }
        }

        /* STEP TWO: Delete all objects that can be deleted after step one */

        for ( index = 0; index < data_small_set_get_count( &objects ); index ++ )
        {
            data_id_t current_id;
            current_id = data_small_set_get_id( &objects, index );
            switch ( data_id_get_table( &current_id ) )
            {
                case DATA_TABLE_CLASSIFIER:
                {
                    data_classifier_t old_classifier2;
                    data_error_t current_result2;
                    current_result2 = data_database_writer_delete_classifier( (*this_).db_writer, data_id_get_row_id( &current_id ), &old_classifier2 );

                    result |= (ctrl_error_t) current_result2;

                    if ( DATA_ERROR_NONE == current_result2 )
                    {
                        /* store the deleted classifier to the undo redo list */
                        ctrl_undo_redo_list_add_delete_classifier( (*this_).undo_redo_list, &old_classifier2 );
                    }
                }
                break;

                case DATA_TABLE_FEATURE:
                {
                    /* see step one */
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    /* see step one */
                }
                break;

                case DATA_TABLE_DIAGRAMELEMENT:
                {
                    /* see step one */
                }
                break;

                case DATA_TABLE_DIAGRAM:
                {
                    data_diagram_t old_diagram;
                    data_error_t current_result3;
                    current_result3 = data_database_writer_delete_diagram ( (*this_).db_writer, data_id_get_row_id( &current_id ), &old_diagram );

                    result |= (ctrl_error_t) current_result3;

                    if ( DATA_ERROR_NONE == current_result3 )
                    {
                        /* store the deleted diagram to the undo redo list */
                        ctrl_undo_redo_list_add_delete_diagram( (*this_).undo_redo_list, &old_diagram );
                    }
                }
                break;

                default:
                {
                    /* see step one */
                }
                break;
            }
        }

        /* add a boundary to the undo redo list after all deletions */
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ FEATURE ================================ */

ctrl_error_t ctrl_classifier_controller_create_feature ( ctrl_classifier_controller_t *this_,
                                                         const data_feature_t *new_feature,
                                                         bool add_to_latest_undo_set,
                                                         int64_t* out_new_id )
{
    TSLOG_ERROR("not yet implemented");
    return CTRL_ERROR_NOT_YET_IMPLEMENTED;
}

ctrl_error_t ctrl_classifier_controller_update_feature_main_type ( ctrl_classifier_controller_t *this_,
                                                                   int64_t feature_id,
                                                                   data_feature_type_t new_feature_type )
{
    TSLOG_ERROR("not yet implemented");
    return CTRL_ERROR_NOT_YET_IMPLEMENTED;
}

ctrl_error_t ctrl_classifier_controller_update_feature_key ( ctrl_classifier_controller_t *this_,
                                                             int64_t feature_id,
                                                             const char* new_feature_key )
{
    TSLOG_ERROR("not yet implemented");
    return CTRL_ERROR_NOT_YET_IMPLEMENTED;
}

ctrl_error_t ctrl_classifier_controller_update_feature_value ( ctrl_classifier_controller_t *this_,
                                                               int64_t feature_id,
                                                               const char* new_feature_value )
{
    TSLOG_ERROR("not yet implemented");
    return CTRL_ERROR_NOT_YET_IMPLEMENTED;
}

ctrl_error_t ctrl_classifier_controller_update_feature_description ( ctrl_classifier_controller_t *this_,
                                                                     int64_t feature_id,
                                                                     const char* new_feature_description )
{
    TSLOG_ERROR("not yet implemented");
    return CTRL_ERROR_NOT_YET_IMPLEMENTED;
}

ctrl_error_t ctrl_classifier_controller_update_feature_list_order ( ctrl_classifier_controller_t *this_,
                                                                    int64_t feature_id,
                                                                    int32_t new_feature_list_order )
{
    TSLOG_ERROR("not yet implemented");
    return CTRL_ERROR_NOT_YET_IMPLEMENTED;
}

/* ================================ RELATIONSHIP ================================ */

ctrl_error_t ctrl_classifier_controller_create_relationship ( ctrl_classifier_controller_t *this_,
                                                              const data_relationship_t *new_relationship,
                                                              bool add_to_latest_undo_set,
                                                              int64_t* out_new_id )
{
    TSLOG_ERROR("not yet implemented");
    return CTRL_ERROR_NOT_YET_IMPLEMENTED;
}

ctrl_error_t ctrl_classifier_controller_update_relationship_main_type ( ctrl_classifier_controller_t *this_,
                                                                        int64_t relationship_id,
                                                                        data_relationship_type_t new_relationship_type )
{
    TSLOG_ERROR("not yet implemented");
    return CTRL_ERROR_NOT_YET_IMPLEMENTED;
}

ctrl_error_t ctrl_classifier_controller_update_relationship_name ( ctrl_classifier_controller_t *this_,
                                                                   int64_t relationship_id,
                                                                   const char* new_relationship_name )
{
    TSLOG_ERROR("not yet implemented");
    return CTRL_ERROR_NOT_YET_IMPLEMENTED;
}

ctrl_error_t ctrl_classifier_controller_update_relationship_description ( ctrl_classifier_controller_t *this_,
                                                                          int64_t relationship_id,
                                                                          const char* new_relationship_description )
{
    TSLOG_ERROR("not yet implemented");
    return CTRL_ERROR_NOT_YET_IMPLEMENTED;
}

ctrl_error_t ctrl_classifier_controller_update_relationship_list_order ( ctrl_classifier_controller_t *this_,
                                                                         int64_t relationship_id,
                                                                         int32_t new_relationship_list_order )
{
    TSLOG_ERROR("not yet implemented");
    return CTRL_ERROR_NOT_YET_IMPLEMENTED;
}


/*
Copyright 2016-2017 Andreas Warnke

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
