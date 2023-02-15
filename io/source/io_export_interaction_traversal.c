/* File: io_export_interaction_traversal.c; Copyright and License: see below */

#include "io_export_interaction_traversal.h"
#include "xmi/xmi_element_info.h"
#include "xmi/xmi_element_info_map.h"
#include "u8/u8_trace.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void io_export_interaction_traversal_init( io_export_interaction_traversal_t *this_,
                                           data_database_reader_t *db_reader,
                                           data_visible_set_t *input_data,
                                           universal_array_list_t *io_written_id_set,
                                           data_stat_t *io_export_stat,
                                           io_element_writer_t *out_element_writer )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != input_data );
    assert( NULL != io_written_id_set );
    assert( NULL != io_export_stat );
    assert( NULL != out_element_writer );

    (*this_).db_reader = db_reader;
    (*this_).input_data = input_data;
    data_rules_init ( &((*this_).filter_rules) );
    (*this_).written_id_set = io_written_id_set;
    (*this_).export_stat = io_export_stat;
    (*this_).element_writer = out_element_writer;

    data_classifier_init_empty( &((*this_).fake_interaction_classifier) );
    data_classifier_set_main_type( &((*this_).fake_interaction_classifier), DATA_CLASSIFIER_TYPE_INTERACTION );
    data_feature_init_empty( &((*this_).fake_lifeline_feature) );
    data_feature_set_main_type( &((*this_).fake_lifeline_feature), DATA_FEATURE_TYPE_LIFELINE );

    U8_TRACE_END();
}

void io_export_interaction_traversal_destroy( io_export_interaction_traversal_t *this_ )
{
    U8_TRACE_BEGIN();

    data_classifier_destroy( &((*this_).fake_interaction_classifier) );
    data_feature_destroy( &((*this_).fake_lifeline_feature) );
    data_rules_destroy ( &((*this_).filter_rules) );
    (*this_).input_data = NULL;
    (*this_).db_reader = NULL;
    (*this_).written_id_set = NULL;
    (*this_).export_stat = NULL;
    (*this_).element_writer = NULL;

    U8_TRACE_END();
}

int io_export_interaction_traversal_iterate_classifier_occurrences ( io_export_interaction_traversal_t *this_,
                                                                     data_classifier_type_t nesting_type,
                                                                     data_id_t classifier_id )
{
    U8_TRACE_BEGIN();
    assert( data_id_is_valid( &classifier_id ) );
    int write_err = 0;

    data_small_set_t out_showing_diagram_ids;
    data_small_set_init( &out_showing_diagram_ids );
    {
        const u8_error_t data_err = data_database_reader_get_diagram_ids_by_classifier_id ( (*this_).db_reader,
                                                                                            data_id_get_row_id( &classifier_id ),
                                                                                            &out_showing_diagram_ids
                                                                                          );
        if( data_err == U8_ERROR_NONE )
        {
            const uint32_t diag_count = data_small_set_get_count( &out_showing_diagram_ids );
            for ( uint32_t diag_idx = 0; diag_idx < diag_count; diag_idx ++ )
            {
                const data_id_t diag_id = data_small_set_get_id( &out_showing_diagram_ids, diag_idx );

                const bool duplicate_diagram
                    =( -1 != universal_array_list_get_index_of( (*this_).written_id_set, &diag_id ) );

                if ( ! duplicate_diagram )
                {
                    write_err |= io_export_interaction_traversal_private_walk_diagram( this_, nesting_type, diag_id );
                }
            }
        }
        else
        {
            write_err = -1;
            assert(false);
        }
    }
    data_small_set_destroy( &out_showing_diagram_ids );

    U8_TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_interaction_traversal_private_walk_diagram ( io_export_interaction_traversal_t *this_,
                                                           data_classifier_type_t nesting_type,
                                                           data_id_t diagram_id )
{
    U8_TRACE_BEGIN();
    assert( data_id_get_table( &diagram_id ) == DATA_TABLE_DIAGRAM );
    int write_err = 0;

    /* write part for current diagram */
    if ( data_id_is_valid( &diagram_id ) )
    {
        /* load data to be drawn */
        data_visible_set_init( (*this_).input_data );
        const u8_error_t d_err = data_visible_set_load( (*this_).input_data,
                                                          data_id_get_row_id( &diagram_id ),
                                                          (*this_).db_reader
                                                        );
        if( d_err != U8_ERROR_NONE )
        {
            write_err = -1;
            assert(false);
        }
        assert( data_visible_set_is_valid ( (*this_).input_data ) );

        /* write diagram */
        const data_diagram_t *diag_ptr = data_visible_set_get_diagram_const( (*this_).input_data );
        assert( diag_ptr != NULL );
        assert( data_diagram_is_valid( diag_ptr ) );
        const data_diagram_type_t diag_type = data_diagram_get_diagram_type( diag_ptr );
        const bool is_interaction_type
            = (( diag_type == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM )
            || ( diag_type == DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM )
            || ( diag_type == DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM )
            || ( diag_type == DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM ));

        if ( is_interaction_type )
        {
            U8_TRACE_INFO_INT("exporting diagram as interaction, id:",data_diagram_get_row_id(diag_ptr));

            /* add this classifier to the already written elements */
            write_err |= universal_array_list_append( (*this_).written_id_set, &diagram_id );

            data_classifier_t *const fake_interaction = &((*this_).fake_interaction_classifier);
            write_err |= io_export_interaction_traversal_private_fake_interaction( this_,
                                                                                   diag_ptr,
                                                                                   fake_interaction
                                                                                 );

            //write_err |= xmi_interaction_writer_start_diagram( &((*this_).interaction_writer), nesting_type, diag_ptr );
            write_err |= io_element_writer_start_classifier( (*this_).element_writer, nesting_type, fake_interaction );
            write_err |= io_element_writer_assemble_classifier( (*this_).element_writer, nesting_type, fake_interaction);

            /* write all classifiers */
            write_err |= io_export_interaction_traversal_private_iterate_diagram_classifiers( this_,
                                                                                              (*this_).input_data,
                                                                                              fake_interaction
                                                                                            );

            //write_err |= xmi_interaction_writer_end_diagram( &((*this_).interaction_writer), nesting_type );
            write_err |= io_element_writer_end_classifier( (*this_).element_writer, nesting_type, fake_interaction);
        }

        data_visible_set_destroy( (*this_).input_data );
    }

    U8_TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_interaction_traversal_private_iterate_diagram_classifiers ( io_export_interaction_traversal_t *this_,
                                                                          const data_visible_set_t *diagram_data,
                                                                          const data_classifier_t *fake_interaction )
{
    U8_TRACE_BEGIN();
    assert( diagram_data != NULL );
    assert( data_visible_set_is_valid( diagram_data ) );
    int write_err = 0;

    /* iterate over all classifiers */
    uint32_t count;
    count = data_visible_set_get_visible_classifier_count ( diagram_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get classifier */
        const data_visible_classifier_t *visible_classifier;
        visible_classifier = data_visible_set_get_visible_classifier_const ( diagram_data, index );
        if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
        {
            const data_classifier_t *classifier;
            classifier = data_visible_classifier_get_classifier_const( visible_classifier );
            const data_id_t classifier_id = data_classifier_get_data_id(classifier);
            U8_TRACE_INFO_INT( "printing classifier with id", data_id_get_row_id( &classifier_id ) );
            const data_diagramelement_t *diagele;
            diagele = data_visible_classifier_get_diagramelement_const( visible_classifier );
            const data_id_t focused_feature_id = data_diagramelement_get_focused_feature_data_id( diagele );

            /* print classifiers if of type comment or interaction-diagram-reference */
            {
                const data_classifier_type_t parent_type = DATA_CLASSIFIER_TYPE_INTERACTION;  /* fake parent type */
                const data_classifier_type_t classifier_type = data_classifier_get_main_type(classifier);
                const bool is_classifier_compliant_here = io_element_writer_can_classifier_nest_classifier ( (*this_).element_writer,
                                                                                                              parent_type,
                                                                                                              classifier_type
                                                                                                            );
                const bool is_duplicate
                    = ( -1 != universal_array_list_get_index_of( (*this_).written_id_set, &classifier_id ) );
                if ( is_classifier_compliant_here && ( ! is_duplicate ) )
                {
                    /* add the classifier to the duplicates list */
                    write_err |= universal_array_list_append( (*this_).written_id_set, &classifier_id );

                    /* print */
                    write_err |= io_element_writer_start_classifier( (*this_).element_writer, parent_type, classifier );
                    write_err |= io_element_writer_assemble_classifier( (*this_).element_writer, parent_type, classifier );
                    write_err |= io_export_interaction_traversal_private_iterate_node_features( this_,
                                                                                                diagram_data,
                                                                                                classifier
                                                                                              );
                    write_err |= io_element_writer_end_classifier( (*this_).element_writer, parent_type, classifier );
                }
            }

            /* print focused features (lifelines) of the visible classifier */
            if ( data_id_is_valid( &focused_feature_id ) )
            {
                write_err |= io_export_interaction_traversal_private_look_for_focused_feature( this_,
                                                                                               diagram_data,
                                                                                               focused_feature_id,
                                                                                               fake_interaction
                                                                                             );

                /* print all relationships starting from focused feature (lifeline) of classifier_id */
                write_err |= io_export_interaction_traversal_private_iterate_feature_relationships( this_,
                                                                                                    diagram_data,
                                                                                                    classifier_id,
                                                                                                    focused_feature_id,
                                                                                                    fake_interaction
                                                                                                  );
            }
        }
        else
        {
            assert( false );
        }
    }

    U8_TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_interaction_traversal_private_look_for_focused_feature ( io_export_interaction_traversal_t *this_,
                                                                       const data_visible_set_t *diagram_data,
                                                                       data_id_t focused_feature_id,
                                                                       const data_classifier_t *fake_interaction )
{
    U8_TRACE_BEGIN();
    assert( diagram_data != NULL );
    assert( data_visible_set_is_valid( diagram_data ) );
    assert( DATA_TABLE_FEATURE == data_id_get_table( &focused_feature_id ) );
    assert( DATA_ROW_ID_VOID != data_id_get_row_id( &focused_feature_id) );
    assert( fake_interaction != NULL );
    int write_err = 0;

    /* iterate over all features */
    uint32_t count;
    count = data_visible_set_get_feature_count ( diagram_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get feature */
        const data_feature_t *feature;
        feature = data_visible_set_get_feature_const ( diagram_data, index );

        if (( feature != NULL ) && ( data_feature_is_valid( feature ) ))
        {
            const data_id_t feat_id = data_feature_get_data_id( feature );
            if ( data_id_equals( &focused_feature_id, &feat_id ) )
            {
                const bool is_visible = data_rules_diagram_shows_feature ( &((*this_).filter_rules),
                                                                           diagram_data,
                                                                           data_feature_get_row_id( feature )
                                                                         );
                const bool is_lifeline
                    =( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type( feature ) );

                if ( is_visible && is_lifeline )
                {
                    const data_classifier_t *const parent_classifier
                        = data_visible_set_get_classifier_by_id_const( diagram_data, data_feature_get_classifier_row_id( feature ));
                    if ( parent_classifier != NULL )
                    {
                        /* add the lifeline to the duplicates list */
                        write_err |= universal_array_list_append( (*this_).written_id_set, &feat_id );

                        const data_classifier_type_t fake_type = data_classifier_get_main_type( fake_interaction );
                        write_err |= io_element_writer_start_feature( (*this_).element_writer, fake_type, feature );
                        write_err |= io_element_writer_assemble_feature( (*this_).element_writer, fake_interaction, feature );
                        write_err |= io_element_writer_end_feature( (*this_).element_writer, fake_type, feature );
                    }
                    else
                    {
                        assert ( false );  /* is_visible should not be true if parent_classifier == NULL */
                    }
                }
            }
        }
        else
        {
            assert( false );
        }
    }

    U8_TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_interaction_traversal_private_iterate_feature_relationships( io_export_interaction_traversal_t *this_,
                                                                           const data_visible_set_t *diagram_data,
                                                                           data_id_t from_classifier_id,
                                                                           data_id_t focused_feature_id,
                                                                           const data_classifier_t *fake_interaction )
{
    U8_TRACE_BEGIN();
    assert( diagram_data != NULL );
    assert( data_visible_set_is_valid( diagram_data ) );
    assert( DATA_TABLE_CLASSIFIER == data_id_get_table( &from_classifier_id ) );
    assert( DATA_ROW_ID_VOID != data_id_get_row_id( &from_classifier_id) );
    int write_err = 0;

    /* iterate over all relationships */
    const uint32_t count = data_visible_set_get_relationship_count ( diagram_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get relationship */
        const data_relationship_t *const relation
            = data_visible_set_get_relationship_const ( diagram_data, index );
        if (( relation != NULL ) && ( data_relationship_is_valid( relation ) ))
        {
            /*const data_id_t r_from_classifier_id = data_relationship_get_from_classifier_data_id( relation );*/
            const data_id_t rel_from_feature_id = data_relationship_get_from_feature_data_id( relation );
            const data_id_t rel_to_feature_id = data_relationship_get_to_feature_data_id( relation );
            /*if ( data_id_equals( &from_classifier_id, &r_from_classifier_id ) )*/  /* do not care if send or receive */
            if (( data_id_equals( &focused_feature_id, &rel_from_feature_id ) )
                ||( data_id_equals( &focused_feature_id, &rel_to_feature_id ) ))
            {
                const data_id_t relation_id = data_relationship_get_data_id( relation );
                const bool is_visible = data_rules_diagram_shows_relationship ( &((*this_).filter_rules),
                                                                                diagram_data,
                                                                                data_id_get_row_id( &relation_id )
                                                                              );

                const bool is_relationship_compliant_here
                    = io_element_writer_can_classifier_nest_relationship ( (*this_).element_writer,
                                                                            DATA_CLASSIFIER_TYPE_INTERACTION,  /* fake parent type */
                                                                            data_relationship_get_main_type( relation )
                                                                          );

                const bool duplicate_relationship
                    = ( -1 != universal_array_list_get_index_of( (*this_).written_id_set, &relation_id ) );

                /* is message */
                const data_relationship_type_t relation_type = data_relationship_get_main_type( relation );
                const xmi_element_info_t *relation_info;
                int map_err = xmi_element_info_map_get_relationship( &xmi_element_info_map_standard,
                                                                     false, /* interaction context, not state */
                                                                     relation_type,
                                                                     &relation_info
                                                                   );
                if ( map_err != 0 )
                {
                    U8_LOG_WARNING_INT("xmi_element_info_map_get_relationship could not map type", relation_type );
                }


                if ( is_visible && ( ! duplicate_relationship ) && is_relationship_compliant_here )
                {
                    /* add the relationship to the duplicates list */
                    write_err |= universal_array_list_append( (*this_).written_id_set, &relation_id );

                    /* destination classifier found, print the relation */
                    write_err |= io_element_writer_start_relationship( (*this_).element_writer,
                                                                        DATA_CLASSIFIER_TYPE_INTERACTION,  /* fake parent type */
                                                                        relation
                                                                      );
                    write_err |= io_element_writer_assemble_relationship( (*this_).element_writer,
                                                                          fake_interaction,
                                                                          relation,
                                                                          &((*this_).fake_interaction_classifier),  /* fake from classifier type */
                                                                          &((*this_).fake_lifeline_feature),  /* guess from feature type */
                                                                          &((*this_).fake_interaction_classifier),  /* fake to classifier type */
                                                                          &((*this_).fake_lifeline_feature)  /* guess to feature type */
                                                                        );
                    write_err |= io_element_writer_end_relationship( (*this_).element_writer,
                                                                      DATA_CLASSIFIER_TYPE_INTERACTION,  /* fake parent type */
                                                                      relation
                                                                    );
                }
            }
        }
        else
        {
            assert( false );
        }
    }

    U8_TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_interaction_traversal_private_fake_interaction( io_export_interaction_traversal_t *this_,
                                                              const data_diagram_t *interaction_diagram,
                                                              data_classifier_t *out_fake_classifier )
{
    U8_TRACE_BEGIN();
    assert( out_fake_classifier != NULL );
    assert( interaction_diagram != NULL );
    assert( data_diagram_is_valid( interaction_diagram ) );
    int write_err = 0;

    data_classifier_set_row_id( out_fake_classifier, 1000000 + data_diagram_get_row_id( interaction_diagram ) );
    data_classifier_set_main_type( out_fake_classifier, DATA_CLASSIFIER_TYPE_INTERACTION );
    write_err |= data_classifier_set_stereotype( out_fake_classifier, "" );
    write_err |= data_classifier_set_name( out_fake_classifier, data_diagram_get_name_const( interaction_diagram ) );
    write_err |= data_classifier_set_description( out_fake_classifier, data_diagram_get_description_const( interaction_diagram ) );
    data_classifier_set_x_order( out_fake_classifier, data_diagram_get_list_order( interaction_diagram ) );
    data_classifier_set_y_order( out_fake_classifier, data_diagram_get_list_order( interaction_diagram ) );
    data_classifier_set_list_order( out_fake_classifier, data_diagram_get_list_order( interaction_diagram ) );
    write_err |= data_classifier_set_uuid( out_fake_classifier, data_diagram_get_uuid_const( interaction_diagram ) );

    U8_TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_interaction_traversal_private_iterate_node_features( io_export_interaction_traversal_t *this_,
                                                                   const data_visible_set_t *diagram_data,
                                                                   const data_classifier_t *parent_classifier )
{
    U8_TRACE_BEGIN();
    assert( diagram_data != NULL );
    assert( data_visible_set_is_valid( diagram_data ) );
    assert( parent_classifier != NULL );
    int write_err = 0;

    /* get parent classifier if */
    const data_row_id_t classifier_id = data_classifier_get_row_id( parent_classifier );

    /* iterate over all features */
    const uint32_t count = data_visible_set_get_feature_count ( diagram_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get feature */
        const data_feature_t *const feature
            = data_visible_set_get_feature_const ( diagram_data, index );
        if (( feature != NULL ) && ( data_feature_is_valid( feature ) ))
        {
            const bool is_lifeline
                =( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type( feature ) );
            const bool is_child
                =( classifier_id == data_feature_get_row_id( feature ) );

            if (( ! is_lifeline )&&( is_child ))
            {
                const data_classifier_type_t classifier_type = data_classifier_get_main_type( parent_classifier );
                write_err |= io_element_writer_start_feature( (*this_).element_writer, classifier_type, feature );
                write_err |= io_element_writer_assemble_feature( (*this_).element_writer, parent_classifier, feature );
                write_err |= io_element_writer_end_feature( (*this_).element_writer, classifier_type, feature );
            }
            /* else: */
            /* lifelines are handled in io_export_interaction_traversal_private_look_for_focused_feature */
        }
        else
        {
            assert( false );
        }
    }

    U8_TRACE_END_ERR( write_err );
    return write_err;
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
