/* File: io_export_model_traversal.c; Copyright and License: see below */

#include "io_export_model_traversal.h"
#include "trace.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void io_export_model_traversal_init( io_export_model_traversal_t *this_,
                                     data_database_reader_t *db_reader,
                                     data_visible_set_t *input_data,
                                     io_filter_flag_t filter_flags,
                                     io_format_writer_t *format_writer )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != input_data );
    assert( NULL != format_writer );

    (*this_).db_reader = db_reader;
    (*this_).input_data = input_data;
    (*this_).filter_flags = filter_flags;
    data_rules_init ( &((*this_).filter_rules) );
    (*this_).format_writer = format_writer;

    universal_array_list_init ( &((*this_).written_id_set),
                                sizeof((*this_).written_id_set_buf)/sizeof(data_id_t),
                                &((*this_).written_id_set_buf),
                                sizeof(data_id_t),
                                ((char*)(&((*this_).written_id_set_buf[1])))-((char*)(&((*this_).written_id_set_buf[0]))),
                                (void (*)(void *, const void *)) &data_id_copy,
                                (bool (*)(const void *, const void *)) &data_id_equals,
                                (void (*)(void *)) &data_id_destroy
                              );

    TRACE_END();
}

void io_export_model_traversal_destroy( io_export_model_traversal_t *this_ )
{
    TRACE_BEGIN();

    universal_array_list_destroy ( &((*this_).written_id_set) );

    data_rules_destroy ( &((*this_).filter_rules) );
    (*this_).input_data = NULL;
    (*this_).db_reader = NULL;

    TRACE_END();
}

int io_export_model_traversal_begin_and_walk_diagram ( io_export_model_traversal_t *this_,
                                                       data_id_t diagram_id,
                                                       const char *diagram_file_base_name )
{
    TRACE_BEGIN();
    assert( data_id_get_table( &diagram_id ) == DATA_TABLE_DIAGRAM );
    int write_err = 0;

    /* write part for current diagram */
    if ( DATA_ROW_ID_VOID != data_id_get_row_id( &diagram_id ) )
    {
        /* load data to be drawn */
        data_visible_set_init( (*this_).input_data );
        const data_error_t d_err = data_visible_set_load( (*this_).input_data,
                                                          data_id_get_row_id( &diagram_id ),
                                                          (*this_).db_reader
                                                        );
        assert( d_err == DATA_ERROR_NONE );
        assert( data_visible_set_is_valid ( (*this_).input_data ) );

        /* write diagram */
        const data_diagram_t *diag_ptr = data_visible_set_get_diagram_const( (*this_).input_data );
        assert( diag_ptr != NULL );
        assert( data_diagram_is_valid( diag_ptr ) );
        TRACE_INFO_INT("printing diagram with id",data_diagram_get_id(diag_ptr));

        /* write_err |= io_format_writer_write_header( (*this_).format_writer, "DUMMY_TITLE" ); */
        write_err |= io_format_writer_start_diagram( (*this_).format_writer, data_diagram_get_data_id(diag_ptr) );
        write_err |= io_format_writer_write_diagram( (*this_).format_writer,
                                                     diag_ptr,
                                                     diagram_file_base_name
                                                   );

        /* write all classifiers */
        write_err |= io_export_model_traversal_private_iterate_diagram_classifiers( this_, (*this_).input_data );

        data_visible_set_destroy( (*this_).input_data );
    }

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_model_traversal_end_diagram ( io_export_model_traversal_t *this_ )
{
    TRACE_BEGIN();
    int write_err = 0;

    /* write footer */
    write_err |= io_format_writer_end_diagram( (*this_).format_writer );
    /*  write_err |= io_format_writer_write_footer( (*this_).format_writer ); */

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_model_traversal_walk_model_nodes ( io_export_model_traversal_t *this_ )
{
    TRACE_BEGIN();
    int write_err = 0;

    {
        data_error_t data_err;
        data_database_iterator_classifiers_t classifier_iterator;

        /* test the iterator, init */
        data_database_iterator_classifiers_init_empty( &classifier_iterator );
        data_err = data_database_reader_get_all_classifiers_iterator ( (*this_).db_reader, &classifier_iterator );
        if ( data_err != DATA_ERROR_NONE )
        {
            write_err = -1;
        }
        else
        {
            while( data_database_iterator_classifiers_has_next( &classifier_iterator ) && ( write_err==0 ) )
            {
                data_err = data_database_iterator_classifiers_next( &classifier_iterator, &((*this_).temp_classifier) );
                if ( data_err != DATA_ERROR_NONE )
                {
                    write_err = -1;
                }
                else
                {
                    data_node_set_init( &((*this_).temp_node_data) );
                    data_err = data_node_set_load( &((*this_).temp_node_data), data_classifier_get_id(&((*this_).temp_classifier)), (*this_).db_reader );
                    if ( data_err != DATA_ERROR_NONE )
                    {
                        write_err = -1;
                    }
                    else
                    {
                        write_err |= io_format_writer_start_classifier( (*this_).format_writer );
                        write_err |= io_format_writer_write_classifier( (*this_).format_writer, &((*this_).temp_classifier) );
                        write_err |= io_export_model_traversal_private_iterate_node_features( this_, &((*this_).temp_node_data) );
                        write_err |= io_format_writer_end_classifier( (*this_).format_writer );

                        data_small_set_t contained_classifiers;
                        data_small_set_init (&contained_classifiers);
                        write_err |= io_export_model_traversal_private_iterate_node_relationships( this_,
                                                                                                   &((*this_).temp_node_data)
                                                                                                 );
                        write_err |= io_export_model_traversal_private_descend_node_containments( this_, data_classifier_get_data_id(&((*this_).temp_classifier)), 16 );
                        data_small_set_destroy (&contained_classifiers);
                    }

                    data_node_set_destroy( &((*this_).temp_node_data) );
                    data_classifier_destroy( &((*this_).temp_classifier) );
                }
            }
        }
        data_err = data_database_iterator_classifiers_destroy( &classifier_iterator );
        if ( data_err != DATA_ERROR_NONE )
        {
            write_err = -1;
        }
    }

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_model_traversal_private_iterate_node_features ( io_export_model_traversal_t *this_,
                                                              const data_node_set_t *node_data )
{
    TRACE_BEGIN();
    assert( node_data != NULL );
    assert( data_node_set_is_valid( node_data ) );
    int write_err = 0;

    /* iterate over all features */
    const uint32_t count = data_node_set_get_feature_count ( node_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get feature */
        const data_feature_t *feature;
        feature = data_node_set_get_feature_const ( node_data, index );
        if (( feature != NULL ) && ( data_feature_is_valid( feature ) ))
        {
            const bool is_lifeline
                =( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type( feature ) );
            const bool filter_lifelines
                = ( 0 != (IO_FILTER_FLAG_LIFELINES & (*this_).filter_flags) );
            const bool lifeline_filter_passed
                =  ( ! is_lifeline ) || ( ! filter_lifelines );

            if ( lifeline_filter_passed )
            {
                write_err |=  io_format_writer_write_feature( (*this_).format_writer, feature );
            }
        }
        else
        {
            assert( false );
        }
    }

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_model_traversal_private_iterate_node_relationships ( io_export_model_traversal_t *this_,
                                                                   const data_node_set_t *node_data )
{
    TRACE_BEGIN();
    assert( node_data != NULL );
    assert( data_node_set_is_valid( node_data ) );
    int write_err = 0;

    /* iterate over all relationships */
    const uint32_t count = data_node_set_get_relationship_count ( node_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get relationship */
        const data_relationship_t *relation;
        relation = data_node_set_get_relationship_const ( node_data, index );
        if (( relation != NULL ) && ( data_relationship_is_valid( relation ) ))
        {
            /* determine if the relationship is a duplicate */
            const data_id_t relation_id = data_relationship_get_data_id( relation );
            const bool duplicate_relationship
                = ( -1 != universal_array_list_get_index_of( &((*this_).written_id_set), &relation_id ) );
            const bool filter_duplicates
                = ( 0 != (IO_FILTER_FLAG_DUPLICATES & (*this_).filter_flags) );
            const bool filter_relationship
                = ( duplicate_relationship && filter_duplicates );

            const data_id_t to_classifier_id = data_relationship_get_to_classifier_data_id( relation );
            const bool destination_already_written
                = ( -1 != universal_array_list_get_index_of( &((*this_).written_id_set), &to_classifier_id ) );

            if ( destination_already_written && ( ! filter_relationship ))
            {
                /* add the relationship to the duplicates list */
                if ( ! duplicate_relationship )
                {
                    write_err |= universal_array_list_append( &((*this_).written_id_set), &relation_id );
                }

                /* destination classifier found, print the relation */
                write_err |= io_format_writer_write_relationship( (*this_).format_writer,
                                                                  relation,
                                                                  NULL /* destination classifier not at hand here */
                                                                );
            }
        }
        else
        {
            assert( false );
        }
    }

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_model_traversal_private_descend_node_containments ( io_export_model_traversal_t *this_,
                                                                  data_id_t classifier_id,
                                                                  unsigned int max_recursion )
{
    TRACE_BEGIN();
    int write_err = 0;

    {
        /*data_error_t data_err;*/

    }

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_model_traversal_private_iterate_diagram_classifiers ( io_export_model_traversal_t *this_,
                                                                    const data_visible_set_t *diagram_data )
{
    TRACE_BEGIN();
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
            TRACE_INFO_INT( "printing classifier with id", data_id_get_row_id( &classifier_id ) );

            /* determine if the classifier is a duplicate */
            const bool duplicate_classifier
                =( -1 != universal_array_list_get_index_of( &((*this_).written_id_set), &classifier_id ) );
            const bool filter_duplicates
                = ( 0 != (IO_FILTER_FLAG_DUPLICATES & (*this_).filter_flags) );
            const bool filter_classifier
                = ( duplicate_classifier && filter_duplicates );

            if ( ! filter_classifier )
            {
                /* add classifier to duplicates-list */
                if ( ! duplicate_classifier )
                {
                    write_err |= universal_array_list_append( &((*this_).written_id_set), &classifier_id );
                }

                /* start classifier */
                write_err |= io_format_writer_start_classifier( (*this_).format_writer );

                write_err |= io_format_writer_write_classifier( (*this_).format_writer, classifier );

                /* print all features of the classifier */
                write_err |= io_export_model_traversal_private_iterate_classifier_features( this_,
                                                                                            diagram_data,
                                                                                            classifier_id
                                                                                          );

                /* end classifier */
                write_err |=  io_format_writer_end_classifier( (*this_).format_writer );
            }

            /* print all relationships starting from classifier_id */
            write_err |= io_export_model_traversal_private_iterate_classifier_relationships( this_,
                                                                                             diagram_data,
                                                                                             classifier_id
                                                                                           );
        }
        else
        {
            assert( false );
        }
    }

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_model_traversal_private_iterate_classifier_features ( io_export_model_traversal_t *this_,
                                                                    const data_visible_set_t *diagram_data,
                                                                    data_id_t classifier_id )
{
    TRACE_BEGIN();
    assert( diagram_data != NULL );
    assert( data_visible_set_is_valid( diagram_data ) );
    assert( DATA_TABLE_CLASSIFIER == data_id_get_table( &classifier_id ) );
    assert( DATA_ROW_ID_VOID != data_id_get_row_id( &classifier_id) );
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
            const data_id_t f_classifier_id = data_feature_get_classifier_data_id( feature );
            if ( data_id_equals( &classifier_id, &f_classifier_id ) )
            {
                const bool is_visible = data_rules_diagram_shows_feature ( &((*this_).filter_rules),
                                                                           diagram_data,
                                                                           data_feature_get_id( feature )
                                                                         );
                const bool is_lifeline
                    =( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type( feature ) );
                const bool filter_lifelines
                    = ( 0 != (IO_FILTER_FLAG_LIFELINES & (*this_).filter_flags) );
                const bool filter_hidden
                    = ( 0 != (IO_FILTER_FLAG_HIDDEN & (*this_).filter_flags) );

                const bool visible_filter_passed
                    = is_visible || ( ! filter_hidden );
                const bool lifeline_filter_passed
                    =  ( ! is_lifeline ) || ( ! filter_lifelines );

                if ( visible_filter_passed && lifeline_filter_passed )
                {
                    write_err |=  io_format_writer_write_feature( (*this_).format_writer, feature );
                }
            }
        }
        else
        {
            assert( false );
        }
    }

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_model_traversal_private_iterate_classifier_relationships ( io_export_model_traversal_t *this_,
                                                                         const data_visible_set_t *diagram_data,
                                                                         data_id_t from_classifier_id )
{
    TRACE_BEGIN();
    assert( diagram_data != NULL );
    assert( data_visible_set_is_valid( diagram_data ) );
    assert( DATA_TABLE_CLASSIFIER == data_id_get_table( &from_classifier_id ) );
    assert( DATA_ROW_ID_VOID != data_id_get_row_id( &from_classifier_id) );
    int write_err = 0;

    /* iterate over all relationships */
    uint32_t count;
    count = data_visible_set_get_relationship_count ( diagram_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get relationship */
        const data_relationship_t *relation;
        relation = data_visible_set_get_relationship_const ( diagram_data, index );
        if (( relation != NULL ) && ( data_relationship_is_valid( relation ) ))
        {
            const data_id_t r_from_classifier_id = data_relationship_get_from_classifier_data_id( relation );
            if ( data_id_equals( &from_classifier_id, &r_from_classifier_id ) )
            {
                const bool is_visible = data_rules_diagram_shows_relationship ( &((*this_).filter_rules),
                                                                                diagram_data,
                                                                                data_relationship_get_id( relation )
                                                                              );

                /* determine if the relationship is a duplicate */
                const data_id_t relation_id = data_relationship_get_data_id( relation );
                const bool duplicate_relationship
                    =( -1 != universal_array_list_get_index_of( &((*this_).written_id_set), &relation_id ) );
                const bool filter_duplicates
                    = ( 0 != (IO_FILTER_FLAG_DUPLICATES & (*this_).filter_flags) );
                const bool filter_relationship
                    = ( duplicate_relationship && filter_duplicates );

                if ( is_visible && ( ! filter_relationship ) )
                {
                    const data_row_id_t to_classifier_id = data_relationship_get_to_classifier_id( relation );
                    const data_classifier_t *dest_classifier = data_visible_set_get_classifier_by_id_const ( diagram_data,
                                                                                                             to_classifier_id
                                                                                                           );
                    if ( dest_classifier != NULL )
                    {
                        /* add the relationship to the duplicates list */
                        if ( ! duplicate_relationship )
                        {
                            write_err |= universal_array_list_append( &((*this_).written_id_set), &relation_id );
                        }

                        /* destination classifier found, print the relation */
                        write_err |= io_format_writer_write_relationship( (*this_).format_writer,
                                                                          relation,
                                                                          dest_classifier
                                                                        );
                    }
                    else
                    {
                        assert ( false );  /* is_visible should not be true if dest_classifier == NULL */
                    }
                }
            }
        }
        else
        {
            assert( false );
        }
    }

    TRACE_END_ERR( write_err );
    return write_err;
}


/*
Copyright 2016-2020 Andreas Warnke

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
