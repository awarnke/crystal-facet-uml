/* File: io_export_diagram_traversal.c; Copyright and License: see below */

#include "io_export_diagram_traversal.h"
#include "trace.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void io_export_diagram_traversal_init( io_export_diagram_traversal_t *this_,
                                       data_database_reader_t *db_reader,
                                       data_visible_set_t *input_data,
                                       data_stat_t *io_export_stat,
                                       io_element_writer_t *out_element_writer )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != input_data );
    assert( NULL != io_export_stat );
    assert( NULL != out_element_writer );

    (*this_).db_reader = db_reader;
    (*this_).input_data = input_data;
    data_rules_init ( &((*this_).filter_rules) );
    (*this_).export_stat = io_export_stat;
    (*this_).element_writer = out_element_writer;

    TRACE_END();
}

void io_export_diagram_traversal_destroy( io_export_diagram_traversal_t *this_ )
{
    TRACE_BEGIN();

    data_rules_destroy ( &((*this_).filter_rules) );
    (*this_).input_data = NULL;
    (*this_).db_reader = NULL;
    (*this_).export_stat = NULL;
    (*this_).element_writer = NULL;

    TRACE_END();
}

int io_export_diagram_traversal_begin_and_walk_diagram ( io_export_diagram_traversal_t *this_,
                                                         data_id_t diagram_id,
                                                         const char *diagram_file_base_name )
{
    TRACE_BEGIN();
    assert( data_id_is_valid( &diagram_id ) );
    assert( data_id_get_table( &diagram_id ) == DATA_TABLE_DIAGRAM );
    int write_err = 0;

    /* load data to be drawn */
    data_visible_set_init( (*this_).input_data );
    const u8_error_t d_err
        = data_visible_set_load( (*this_).input_data, data_id_get_row_id( &diagram_id ), (*this_).db_reader );
    if( d_err != U8_ERROR_NONE )
    {
        write_err = -1;
        assert(false);
    }
    else
    {
        assert( data_visible_set_is_valid ( (*this_).input_data ) );

        /* write diagram */
        const data_diagram_t *diag_ptr = data_visible_set_get_diagram_const( (*this_).input_data );
        assert( diag_ptr != NULL );
        assert( data_diagram_is_valid( diag_ptr ) );
        TRACE_INFO_INT("printing diagram with id",data_diagram_get_row_id(diag_ptr));

        /* load parent diagram if there is one */
        data_diagram_init_empty( &((*this_).temp_parent_diag) );
        const data_row_id_t parent_id = data_diagram_get_parent_row_id( diag_ptr );
        if ( DATA_ROW_ID_VOID != parent_id )
        {
            const u8_error_t d_err2
                = data_database_reader_get_diagram_by_id( (*this_).db_reader, parent_id, &((*this_).temp_parent_diag) );
            if( d_err2 != U8_ERROR_NONE )
            {
                write_err = -1;
                assert(false);
            }
        }

        write_err |= io_element_writer_start_diagram( (*this_).element_writer, diag_ptr );
        write_err |= io_element_writer_assemble_diagram( (*this_).element_writer,
                                                         &((*this_).temp_parent_diag),
                                                         diag_ptr,
                                                         diagram_file_base_name
                                                       );

        data_diagram_destroy( &((*this_).temp_parent_diag) );

        /* write all classifiers */
        write_err |= io_export_diagram_traversal_private_iterate_diagram_classifiers( this_, (*this_).input_data );
    }

    data_visible_set_destroy( (*this_).input_data );

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_diagram_traversal_end_diagram ( io_export_diagram_traversal_t *this_,
                                              data_id_t diagram_id )
{
    TRACE_BEGIN();
    assert( data_id_is_valid( &diagram_id ) );
    assert( data_id_get_table( &diagram_id ) == DATA_TABLE_DIAGRAM );
    int write_err = 0;

    /* load diagram only to be drawn */
    data_diagram_t *const diagram_ptr = data_visible_set_get_diagram_ptr ( (*this_).input_data );
    const u8_error_t d_err
        = data_database_reader_get_diagram_by_id( (*this_).db_reader, data_id_get_row_id( &diagram_id ), diagram_ptr );
    if( d_err != U8_ERROR_NONE )
    {
        write_err = -1;
        assert(false);
    }
    else
    {
        assert( data_diagram_is_valid( diagram_ptr ) );

        /* write footer */
        write_err |= io_element_writer_end_diagram( (*this_).element_writer, diagram_ptr );

        data_diagram_destroy( diagram_ptr );
    }
    TRACE_END_ERR( write_err );
    return write_err;
}

int io_export_diagram_traversal_private_iterate_diagram_classifiers ( io_export_diagram_traversal_t *this_,
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
        const data_visible_classifier_t *const visible_classifier
            = data_visible_set_get_visible_classifier_const ( diagram_data, index );
        if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
        {
            const data_classifier_t *classifier
                = data_visible_classifier_get_classifier_const( visible_classifier );
            const data_id_t classifier_id = data_classifier_get_data_id( classifier );
            TRACE_INFO_INT( "printing classifier with id", data_id_get_row_id( &classifier_id ) );

            /* start+assemble classifier */
            write_err |= io_element_writer_start_classifier( (*this_).element_writer,
                                                             DATA_CLASSIFIER_TYPE_VOID,  /* no host */
                                                             classifier
                                                           );

            write_err |= io_element_writer_assemble_classifier( (*this_).element_writer,
                                                                DATA_CLASSIFIER_TYPE_VOID,   /* no host */
                                                                classifier
                                                              );

            /* print all features of the classifier */
            write_err |= io_export_diagram_traversal_private_iterate_classifier_features( this_,
                                                                                          diagram_data,
                                                                                          classifier_id
                                                                                        );

            /* end classifier */
            write_err |= io_element_writer_end_classifier( (*this_).element_writer,
                                                           DATA_CLASSIFIER_TYPE_VOID,  /* no host */
                                                           classifier
                                                         );

            /* print all relationships starting from classifier_id */
            write_err |= io_export_diagram_traversal_private_iterate_classifier_relationships( this_,
                                                                                               diagram_data,
                                                                                               classifier_id
                                                                                             );

            const data_diagramelement_t *diagele
                = data_visible_classifier_get_diagramelement_const( visible_classifier );
            const data_id_t diagele_id = data_diagramelement_get_data_id( diagele );
            const data_diagram_t *const diagram_ptr = data_visible_set_get_diagram_ptr ( (*this_).input_data );
            const data_row_id_t focused_feature_id = data_diagramelement_get_focused_feature_row_id( diagele );
            const data_feature_t *const focused_f_or_null
                = ( focused_feature_id == DATA_ROW_ID_VOID )
                ? NULL
                : data_visible_set_get_feature_by_id_const( diagram_data, focused_feature_id );

            TRACE_INFO_INT( "printing diagramelement with id", data_id_get_row_id( &diagele_id ) );

            /* start+assemble+end diagramelement */
            write_err |= io_element_writer_start_diagramelement( (*this_).element_writer,
                                                                 diagram_ptr,
                                                                 diagele
                                                               );
            write_err |= io_element_writer_assemble_diagramelement( (*this_).element_writer,
                                                                    diagram_ptr,
                                                                    diagele,
                                                                    classifier,
                                                                    focused_f_or_null
                                                                  );
            write_err |= io_element_writer_end_diagramelement( (*this_).element_writer,
                                                               diagram_ptr,
                                                               diagele
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

int io_export_diagram_traversal_private_iterate_classifier_features ( io_export_diagram_traversal_t *this_,
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
                                                                           data_feature_get_row_id( feature )
                                                                         );
                const bool is_lifeline
                    =( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type( feature ) );

                if ( is_visible && ( ! is_lifeline ) )
                {
                    write_err |=  io_element_writer_assemble_feature( (*this_).element_writer,
                                                                      DATA_CLASSIFIER_TYPE_VOID,  /* not needed */
                                                                      feature
                                                                    );
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

int io_export_diagram_traversal_private_iterate_classifier_relationships ( io_export_diagram_traversal_t *this_,
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
                const data_id_t relation_id = data_relationship_get_data_id( relation );
                const bool is_visible = data_rules_diagram_shows_relationship ( &((*this_).filter_rules),
                                                                                diagram_data,
                                                                                data_id_get_row_id( &relation_id )
                                                                              );

                if ( is_visible /* no filter for duplicates */ )
                {
                    const data_row_id_t to_classifier_id = data_relationship_get_to_classifier_row_id( relation );
                    const data_classifier_t *dest_classifier = data_visible_set_get_classifier_by_id_const ( diagram_data,
                                                                                                             to_classifier_id
                                                                                                           );
                    if ( dest_classifier != NULL )
                    {
                        /* destination classifier found, print the relation */
                        write_err |= io_element_writer_assemble_relationship( (*this_).element_writer,
                                                                              NULL,
                                                                              relation,
                                                                              NULL,
                                                                              NULL,
                                                                              dest_classifier,
                                                                              NULL
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
Copyright 2016-2021 Andreas Warnke

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
