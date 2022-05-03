/* File: io_export_flat_traversal.c; Copyright and License: see below */

#include "io_export_flat_traversal.h"
#include "trace/trace.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void io_export_flat_traversal_init( io_export_flat_traversal_t *this_,
                                     data_database_reader_t *db_reader,
                                     data_stat_t *io_export_stat,
                                     io_element_writer_t *out_element_writer )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != io_export_stat );
    assert( NULL != out_element_writer );

    (*this_).db_reader = db_reader;
    (*this_).export_stat = io_export_stat;
    (*this_).element_writer = out_element_writer;

    TRACE_END();
}

void io_export_flat_traversal_destroy( io_export_flat_traversal_t *this_ )
{
    TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).export_stat = NULL;
    (*this_).element_writer = NULL;

    TRACE_END();
}

int io_export_flat_traversal_iterate_classifiers ( io_export_flat_traversal_t *this_ )
{
    TRACE_BEGIN();
    int write_err = 0;

    {
        u8_error_t data_err;
        data_database_iterator_classifiers_t classifier_iterator;

        /* init the iterator */
        data_database_iterator_classifiers_init_empty( &classifier_iterator );
        data_err = data_database_reader_get_all_classifiers_iterator ( (*this_).db_reader, &classifier_iterator );
        if ( data_err != U8_ERROR_NONE )
        {
            write_err = -1;
        }
        else
        {
            while( data_database_iterator_classifiers_has_next( &classifier_iterator ) && ( write_err==0 ) )
            {
                data_err = data_database_iterator_classifiers_next( &classifier_iterator, &((*this_).temp_classifier) );
                if ( data_err != U8_ERROR_NONE )
                {
                    write_err = -1;
                }
                else
                {
                    write_err |= io_export_flat_traversal_private_traverse_classifier( this_, &((*this_).temp_classifier) );

                    data_classifier_destroy( &((*this_).temp_classifier) );
                }
            }
        }
        data_err = data_database_iterator_classifiers_destroy( &classifier_iterator );
        if ( data_err != U8_ERROR_NONE )
        {
            write_err = -1;
        }
    }

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_flat_traversal_private_traverse_classifier ( io_export_flat_traversal_t *this_,
                                                           const data_classifier_t *classifier )
{
    TRACE_BEGIN();
    assert( NULL != classifier );
    assert( data_classifier_is_valid( classifier ) );
    int write_err = 0;

    /* start classifier */
    {
        const data_classifier_type_t host_type = DATA_CLASSIFIER_TYPE_VOID;
        write_err |= io_element_writer_start_classifier( (*this_).element_writer, host_type, classifier );
        write_err |= io_element_writer_assemble_classifier( (*this_).element_writer, host_type, classifier );
    }

    /* start features and relationships */
    {
        data_node_set_init( &((*this_).temp_node_data) );
        const u8_error_t data_err_2
            = data_node_set_load( &((*this_).temp_node_data),
                                  data_classifier_get_row_id( classifier ),
                                  (*this_).db_reader
                                );

        if ( data_err_2 != U8_ERROR_NONE )
        {
            write_err = -1;
        }
        else
        {
            /* write the features */
            write_err |= io_export_flat_traversal_private_iterate_features( this_, &((*this_).temp_node_data) );

            /* write the relationships */
            write_err |= io_export_flat_traversal_private_iterate_relationships( this_, &((*this_).temp_node_data) );
        }

        data_node_set_destroy( &((*this_).temp_node_data) );
    }

    /* end classifier */
    {
        const data_classifier_type_t host_type = DATA_CLASSIFIER_TYPE_VOID;
        write_err |= io_element_writer_end_classifier( (*this_).element_writer, host_type, classifier );
    }

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_flat_traversal_private_iterate_features ( io_export_flat_traversal_t *this_,
                                                        const data_node_set_t *node_data )
{
    TRACE_BEGIN();
    assert( node_data != NULL );
    assert( data_node_set_is_valid( node_data ) );
    int write_err = 0;

    /* get parent classifier */
    const data_classifier_t *const classifier
        = data_node_set_get_classifier_const ( node_data );

    /* iterate over all features */
    const uint32_t count = data_node_set_get_feature_count ( node_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get feature */
        const data_feature_t *feature;
        feature = data_node_set_get_feature_const ( node_data, index );
        if (( feature != NULL ) && ( data_feature_is_valid( feature ) ))
        {
            write_err |= io_element_writer_start_feature( (*this_).element_writer,
                                                          data_classifier_get_main_type( classifier ),
                                                          feature
                                                        );
            write_err |= io_element_writer_assemble_feature( (*this_).element_writer,
                                                             data_classifier_get_main_type( classifier ),
                                                             feature
                                                           );
            write_err |= io_element_writer_end_feature( (*this_).element_writer,
                                                        data_classifier_get_main_type( classifier ),
                                                        feature
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

int io_export_flat_traversal_private_iterate_relationships ( io_export_flat_traversal_t *this_,
                                                             const data_node_set_t *node_data )
{
    TRACE_BEGIN();
    assert( node_data != NULL );
    assert( data_node_set_is_valid( node_data ) );
    int write_err = 0;

    const data_classifier_t *const classifier = data_node_set_get_classifier_const ( node_data );
    const data_classifier_type_t host_type = data_classifier_get_main_type( classifier );

    /* iterate over all relationships */
    const uint32_t count = data_node_set_get_relationship_count ( node_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get relationship */
        const data_relationship_t *relation;
        relation = data_node_set_get_relationship_const ( node_data, index );
        if (( relation != NULL ) && ( data_relationship_is_valid( relation ) ))
        {
            /* determine if the relationship is outgoing */
            const bool is_outgoing
                = ( data_relationship_get_from_classifier_row_id( relation ) == data_classifier_get_row_id( classifier ) );
            if ( is_outgoing )
            {
                /* get the element types at both ends of the relationship */
                data_classifier_init_empty( &((*this_).temp_from_classifier) );
                data_feature_init_empty( &((*this_).temp_from_feature) );
                data_classifier_init_empty( &((*this_).temp_to_classifier) );
                data_feature_init_empty( &((*this_).temp_to_feature) );

                const u8_error_t d_err
                    = io_export_flat_traversal_private_get_relationship_ends( this_,
                                                                              relation,
                                                                              node_data,
                                                                              &((*this_).temp_from_classifier),
                                                                              &((*this_).temp_from_feature),
                                                                              &((*this_).temp_to_classifier),
                                                                              &((*this_).temp_to_feature)
                                                                            );

                if ( d_err == U8_ERROR_NONE )
                {
                    /* all classifiers found, print the relation */
                    write_err |= io_element_writer_start_relationship( (*this_).element_writer, host_type, relation );
                    write_err |= io_element_writer_assemble_relationship( (*this_).element_writer,
                                                                          classifier,
                                                                          relation,
                                                                          &((*this_).temp_from_classifier),
                                                                          &((*this_).temp_from_feature),
                                                                          &((*this_).temp_to_classifier),
                                                                          &((*this_).temp_to_feature)
                                                                        );
                    write_err |= io_element_writer_end_relationship( (*this_).element_writer, host_type, relation );
                }
                else
                {
                    write_err |= -1;
                }

                data_classifier_destroy( &((*this_).temp_from_classifier) );
                data_feature_destroy( &((*this_).temp_from_feature) );
                data_classifier_destroy( &((*this_).temp_to_classifier) );
                data_feature_destroy( &((*this_).temp_to_feature) );
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

u8_error_t io_export_flat_traversal_private_get_relationship_ends( io_export_flat_traversal_t *this_,
                                                                     const data_relationship_t *relation,
                                                                     const data_node_set_t *node_data,
                                                                     data_classifier_t *out_from_c,
                                                                     data_feature_t *out_from_f,
                                                                     data_classifier_t *out_to_c,
                                                                     data_feature_t *out_to_f )
{
    TRACE_BEGIN();
    assert( relation != NULL );
    assert( node_data != NULL );
    assert( out_from_c != NULL );
    assert( out_from_f != NULL );
    assert( out_to_c != NULL );
    assert( out_to_f != NULL );
    u8_error_t data_err = U8_ERROR_NONE;

    {
        /* get from classifier */
        const data_row_id_t from_clcassifier_row_id = data_relationship_get_from_classifier_row_id ( relation );
        bool from_classifier_found = false;
        if ( node_data != NULL )
        {
            const data_classifier_t *node_classifier
                = data_node_set_get_classifier_const( node_data );
            if (( node_classifier != NULL )
                &&( from_clcassifier_row_id == data_classifier_get_row_id ( node_classifier ) ))
            {
                data_classifier_replace( out_from_c, node_classifier );
                from_classifier_found = true;
            }
        }
        if ( ! from_classifier_found )
        {
            data_err |=  data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                     from_clcassifier_row_id,
                                                                     out_from_c
                                                                   );
        }

        /* get from feature */
        const data_row_id_t from_feature_row_id = data_relationship_get_from_feature_row_id ( relation );
        bool from_feature_found = ( from_feature_row_id == DATA_ROW_ID_VOID );
        if (( node_data != NULL )&&( ! from_feature_found ))
        {
            const data_feature_t *node_feature
                = data_node_set_get_feature_by_id_const ( node_data, from_feature_row_id );
            if ( node_feature != NULL )
            {
                data_feature_replace( out_from_f, node_feature );
                from_feature_found = true;
            }
        }
        if ( ! from_feature_found )
        {
            data_err |=  data_database_reader_get_feature_by_id ( (*this_).db_reader,
                                                                  from_feature_row_id,
                                                                  out_from_f
                                                                );
        }

        /* get to classifier */
        const data_row_id_t to_classifier_row_id = data_relationship_get_to_classifier_row_id ( relation );
        bool to_classifier_found = false;
        if ( node_data != NULL )
        {
            const data_classifier_t *node_classifier
                = data_node_set_get_classifier_const( node_data );
            if (( node_classifier != NULL )
                &&( to_classifier_row_id == data_classifier_get_row_id ( node_classifier ) ))
            {
                data_classifier_replace( out_to_c, node_classifier );
                to_classifier_found = true;
            }
        }
        if ( ! to_classifier_found )
        {
            data_err |=  data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                     to_classifier_row_id,
                                                                     out_to_c
                                                                   );
        }

        /* get to feature */
        const data_row_id_t to_feature_row_id = data_relationship_get_to_feature_row_id ( relation );
        bool to_feature_found = ( to_feature_row_id == DATA_ROW_ID_VOID );
        if (( node_data != NULL )&&( ! to_feature_found ))
        {
            const data_feature_t *node_feature
                = data_node_set_get_feature_by_id_const ( node_data, to_feature_row_id );
            if ( node_feature != NULL )
            {
                data_feature_replace( out_to_f, node_feature );
                to_feature_found = true;
            }
        }
        if ( ! to_feature_found )
        {
            data_err |=  data_database_reader_get_feature_by_id ( (*this_).db_reader,
                                                                  to_feature_row_id,
                                                                  out_to_f
                                                                );
        }
    }

    if ( data_err != U8_ERROR_NONE )
    {
        TSLOG_ERROR_INT( "A relationship references classifier(s) and/or feature(s) that do not exist:",
                         data_relationship_get_row_id ( relation )
                       );
    }
    TRACE_END_ERR( data_err );
    return data_err;
}


/*
Copyright 2021-2022 Andreas Warnke

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
