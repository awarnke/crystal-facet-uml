/* File: io_export_flat_traversal.c; Copyright and License: see below */

#include "io_export_flat_traversal.h"
#include "u8/u8_trace.h"
#include "entity/data_diagram.h"
#include "entity/data_classifier.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void io_export_flat_traversal_init( io_export_flat_traversal_t *this_,
                                     data_database_reader_t *db_reader,
                                     data_stat_t *io_export_stat,
                                     io_element_writer_t *out_element_writer )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != io_export_stat );
    assert( NULL != out_element_writer );

    (*this_).db_reader = db_reader;
    (*this_).export_stat = io_export_stat;
    (*this_).element_writer = out_element_writer;

    U8_TRACE_END();
}

void io_export_flat_traversal_destroy( io_export_flat_traversal_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).export_stat = NULL;
    (*this_).element_writer = NULL;

    U8_TRACE_END();
}

u8_error_t io_export_flat_traversal_iterate_classifiers( io_export_flat_traversal_t *this_, bool hierarchical  )
{
    U8_TRACE_BEGIN();
    u8_error_t write_err = U8_ERROR_NONE;

    {
        u8_error_t data_err;
        data_classifier_iterator_t classifier_iterator;

        /* init the iterator */
        data_classifier_iterator_init_empty( &classifier_iterator );
        data_err = data_database_reader_get_all_classifiers ( (*this_).db_reader, hierarchical, &classifier_iterator );
        if ( data_err != U8_ERROR_NONE )
        {
            write_err |= data_err;
        }
        else
        {
            while( data_classifier_iterator_has_next( &classifier_iterator ) && ( write_err == U8_ERROR_NONE ) )
            {
                data_err = data_classifier_iterator_next( &classifier_iterator, &((*this_).temp_classifier) );
                if ( data_err != U8_ERROR_NONE )
                {
                    write_err |= data_err;
                }
                else
                {
                    write_err |= io_export_flat_traversal_private_traverse_classifier( this_, &((*this_).temp_classifier) );

                    data_classifier_destroy( &((*this_).temp_classifier) );
                }
            }
        }
        data_err = data_classifier_iterator_destroy( &classifier_iterator );
        if ( data_err != U8_ERROR_NONE )
        {
            write_err |= data_err;
        }
    }

    U8_TRACE_END_ERR( write_err );
    return write_err;
}

u8_error_t io_export_flat_traversal_private_traverse_classifier( io_export_flat_traversal_t *this_,
                                                                 const data_classifier_t *classifier )
{
    U8_TRACE_BEGIN();
    assert( NULL != classifier );
    assert( data_classifier_is_valid( classifier ) );
    u8_error_t write_err = U8_ERROR_NONE;

    /* start classifier */
    {
        const data_classifier_type_t host_type = DATA_CLASSIFIER_TYPE_VOID;
        write_err |= io_element_writer_start_classifier( (*this_).element_writer, host_type, classifier );
        write_err |= io_element_writer_assemble_classifier( (*this_).element_writer, host_type, classifier );
    }

    /* start features and relationships */
    {
        u8_error_t data_err_3 = U8_ERROR_NONE;

        /* write the features */
        data_feature_iterator_t feature_iterator;
        data_err_3 |= data_feature_iterator_init_empty( &feature_iterator );
        data_err_3 |= data_database_reader_get_features_by_classifier_id( (*this_).db_reader,
                                                                          data_classifier_get_row_id( classifier ),
                                                                          &feature_iterator
                                                                        );
        write_err |= io_export_flat_traversal_private_iterate_features( this_, classifier, &feature_iterator );
        data_err_3 |= data_feature_iterator_destroy( &feature_iterator );

        /* write the relationships */
        data_relationship_iterator_t rel_iterator;
        data_err_3 |= data_relationship_iterator_init_empty( &rel_iterator );
        data_err_3 |= data_database_reader_get_relationships_by_classifier_id( (*this_).db_reader,
                                                                               data_classifier_get_row_id( classifier ),
                                                                               &rel_iterator
                                                                             );
        write_err |= io_export_flat_traversal_private_iterate_relationships( this_, classifier, &rel_iterator );
        data_err_3 |= data_relationship_iterator_destroy( &rel_iterator );

        /* if errors at reading the database, add these to the result */
        if ( data_err_3 != U8_ERROR_NONE )
        {
            write_err |= data_err_3;
        }
    }

    /* end classifier */
    {
        const data_classifier_type_t host_type = DATA_CLASSIFIER_TYPE_VOID;
        write_err |= io_element_writer_end_classifier( (*this_).element_writer, host_type, classifier );
    }

    U8_TRACE_END_ERR( write_err );
    return write_err;
}

u8_error_t io_export_flat_traversal_private_iterate_features( io_export_flat_traversal_t *this_,
                                                              const data_classifier_t *classifier,
                                                              data_feature_iterator_t *feature_iterator )
{
    U8_TRACE_BEGIN();
    assert( classifier != NULL );
    assert( data_classifier_is_valid( classifier ) );
    assert( feature_iterator != NULL );
    u8_error_t write_err = U8_ERROR_NONE;
    u8_error_t data_err = U8_ERROR_NONE;

    /* iterate over all features */
    while ( data_feature_iterator_has_next( feature_iterator ) )
    {
        /* get feature */
        data_err |= data_feature_iterator_next( feature_iterator, &((*this_).temp_from_feature) );
        if ( data_feature_is_valid( &((*this_).temp_from_feature) ) )
        {
            const data_classifier_type_t classifier_type = data_classifier_get_main_type( classifier );
            write_err |= io_element_writer_start_feature( (*this_).element_writer, classifier_type, &((*this_).temp_from_feature) );
            write_err |= io_element_writer_assemble_feature( (*this_).element_writer, classifier, &((*this_).temp_from_feature) );
            write_err |= io_element_writer_end_feature( (*this_).element_writer, classifier_type, &((*this_).temp_from_feature) );
        }
        else
        {
            assert( false );
        }
    }

    write_err |= data_err;  /* result is the combination of writing errors and reading errors */
    U8_TRACE_END_ERR( write_err );
    return write_err;
}

u8_error_t io_export_flat_traversal_private_iterate_relationships( io_export_flat_traversal_t *this_,
                                                                   const data_classifier_t *classifier,
                                                                   data_relationship_iterator_t *relationship_iterator )
{
    U8_TRACE_BEGIN();
    assert( classifier != NULL );
    assert( relationship_iterator != NULL );
    assert( data_classifier_is_valid( classifier ) );
    assert( relationship_iterator != NULL );
    u8_error_t write_err = U8_ERROR_NONE;
    u8_error_t data_err = U8_ERROR_NONE;

    const data_classifier_type_t host_type = data_classifier_get_main_type( classifier );

    /* iterate over all relationships */
    while ( data_relationship_iterator_has_next( relationship_iterator ) )
    {
        /* get relationship */
        data_err |= data_relationship_iterator_next( relationship_iterator, &((*this_).temp_relationship) );
        const data_relationship_t *const relation = &((*this_).temp_relationship);
        if ( data_relationship_is_valid( relation ) )
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
                                                                              classifier,
                                                                              relation,
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
                    write_err |= d_err;
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

    write_err |= data_err;  /* result is the combination of writing errors and reading errors */
    U8_TRACE_END_ERR( write_err );
    return write_err;
}

u8_error_t io_export_flat_traversal_private_get_relationship_ends( io_export_flat_traversal_t *this_,
                                                                   const data_classifier_t *classifier,
                                                                   const data_relationship_t *relation,
                                                                   data_classifier_t *out_from_c,
                                                                   data_feature_t *out_from_f,
                                                                   data_classifier_t *out_to_c,
                                                                   data_feature_t *out_to_f )
{
    U8_TRACE_BEGIN();
    assert( classifier != NULL );
    assert( relation != NULL );
    assert( out_from_c != NULL );
    assert( out_from_f != NULL );
    assert( out_to_c != NULL );
    assert( out_to_f != NULL );
    u8_error_t data_err = U8_ERROR_NONE;

    {
        /* get from classifier */
        const data_row_t from_classifier_row_id = data_relationship_get_from_classifier_row_id( relation );
        if ( from_classifier_row_id == data_classifier_get_row_id ( classifier ) )
        {
            data_classifier_replace( out_from_c, classifier );
        }
        else
        {
            data_err |= data_database_reader_get_classifier_by_id( (*this_).db_reader,
                                                                   from_classifier_row_id,
                                                                   out_from_c
                                                                 );
        }

        /* get from feature */
        const data_row_t from_feature_row_id = data_relationship_get_from_feature_row_id( relation );
        if ( from_feature_row_id != DATA_ROW_VOID )
        {
            data_err |=  data_database_reader_get_feature_by_id( (*this_).db_reader,
                                                                 from_feature_row_id,
                                                                 out_from_f
                                                               );
        }
        else
        {
            data_feature_init_empty( out_from_f );
        }

        /* get to classifier */
        const data_row_t to_classifier_row_id = data_relationship_get_to_classifier_row_id( relation );
        if ( to_classifier_row_id == data_classifier_get_row_id ( classifier ) )
        {
            data_classifier_replace( out_to_c, classifier );
        }
        else
        {
            data_err |= data_database_reader_get_classifier_by_id( (*this_).db_reader,
                                                                   to_classifier_row_id,
                                                                   out_to_c
                                                                 );
        }

        /* get to feature */
        const data_row_t to_feature_row_id = data_relationship_get_to_feature_row_id( relation );
        if ( to_feature_row_id != DATA_ROW_VOID )
        {
            data_err |= data_database_reader_get_feature_by_id( (*this_).db_reader,
                                                                to_feature_row_id,
                                                                out_to_f
                                                              );
        }
        else
        {
            data_feature_init_empty( out_to_f );
        }
    }

    if ( data_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_INT( "A relationship references classifier(s) and/or feature(s) that do not exist:",
                          data_relationship_get_row_id ( relation )
                        );
    }
    U8_TRACE_END_ERR( data_err );
    return data_err;
}


/*
Copyright 2021-2026 Andreas Warnke

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
