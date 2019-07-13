/* File: io_diagram_text_exporter.c; Copyright and License: see below */

#include "io_diagram_text_exporter.h"
#include "trace.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void io_diagram_text_exporter_init( io_diagram_text_exporter_t *this_, const data_visible_set_t *input_data )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    (*this_).input_data = input_data;
    data_rules_init ( &((*this_).filter_rules) );

    TRACE_END();
}

void io_diagram_text_exporter_destroy( io_diagram_text_exporter_t *this_ )
{
    TRACE_BEGIN();

    data_rules_destroy ( &((*this_).filter_rules) );
    (*this_).input_data = NULL;

    TRACE_END();
}

int io_diagram_text_exporter_write_all ( io_diagram_text_exporter_t *this_, io_format_writer_t *format_writer )
{
    TRACE_BEGIN();
    assert( NULL != format_writer );
    int write_err = 0;

    /* write diagram */
    const data_diagram_t *diag_ptr = data_visible_set_get_diagram_const( (*this_).input_data );
    assert( diag_ptr != NULL );
    assert( data_diagram_is_valid( diag_ptr ) );
    TRACE_INFO_INT("printing diagram with id",data_diagram_get_id(diag_ptr));

    write_err |= io_format_writer_write_header( format_writer, "DUMMY_TITLE" );
    write_err |= io_format_writer_start_diagram( format_writer );
    write_err |= io_format_writer_write_diagram( format_writer,
                                                 diag_ptr,
                                                 "NO_IMAGE_FILE"
                                               );
    /* write all classifiers */
    write_err |= io_diagram_text_exporter_write_classifiers( this_, format_writer );

    /* write footer */
    write_err |= io_format_writer_end_diagram( format_writer );
    write_err |= io_format_writer_write_footer( format_writer );

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_diagram_text_exporter_write_classifiers ( io_diagram_text_exporter_t *this_, io_format_writer_t *format_writer )
{
    TRACE_BEGIN();
    assert( NULL != format_writer );
    assert( data_visible_set_is_valid( (*this_).input_data ) );
    int write_err = 0;

    /* iterate over all classifiers */
    uint32_t count;
    count = data_visible_set_get_visible_classifier_count ( (*this_).input_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get classifier */
        const data_visible_classifier_t *visible_classifier;
        visible_classifier = data_visible_set_get_visible_classifier_const ( (*this_).input_data, index );
        if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
        {
            const data_classifier_t *classifier;
            classifier = data_visible_classifier_get_classifier_const( visible_classifier );
            const int64_t classifier_id = data_classifier_get_id(classifier);
            TRACE_INFO_INT( "printing classifier with id", classifier_id );

            write_err |=  io_format_writer_start_classifier( format_writer );

            write_err |=  io_format_writer_write_classifier( format_writer, classifier );

            /* print all features */
            write_err |= io_diagram_text_exporter_private_write_features_of_classifier( this_,
                                                                                        classifier_id,
                                                                                        format_writer );

            /* print all relationships */
            write_err |= io_diagram_text_exporter_private_write_relations_of_classifier( this_,
                                                                                         classifier_id,
                                                                                         format_writer );

            /* finish */
            write_err |=  io_format_writer_end_classifier( format_writer );
        }
        else
        {
            assert( false );
        }
    }

    TRACE_END_ERR( write_err );
    return write_err;
}

int io_diagram_text_exporter_private_write_features_of_classifier ( io_diagram_text_exporter_t *this_,
                                                                    int64_t classifier_id,
                                                                    io_format_writer_t *format_writer )
{
    TRACE_BEGIN();
    assert( NULL != format_writer );
    assert( DATA_ID_VOID_ID != classifier_id );
    int write_err = 0;

    /* iterate over all features */
    uint32_t count;
    count = data_visible_set_get_feature_count ( (*this_).input_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get feature */
        const data_feature_t *feature;
        feature = data_visible_set_get_feature_const ( (*this_).input_data, index );
        if (( feature != NULL ) && ( data_feature_is_valid( feature ) ))
        {
            if ( classifier_id == data_feature_get_classifier_id( feature ) )
            {
                const bool show = data_rules_diagram_shows_feature ( &((*this_).filter_rules),
                                                                     (*this_).input_data,
                                                                     data_feature_get_id( feature )
                                                                   );

                if ( show && (DATA_FEATURE_TYPE_LIFELINE != data_feature_get_main_type( feature ) ))
                {
                    write_err |=  io_format_writer_write_feature( format_writer, feature );
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

int io_diagram_text_exporter_private_write_relations_of_classifier ( io_diagram_text_exporter_t *this_,
                                                                     int64_t from_classifier_id,
                                                                     io_format_writer_t *format_writer )
{
    TRACE_BEGIN();
    assert( NULL != format_writer );
    assert( DATA_ID_VOID_ID != from_classifier_id );
    int write_err = 0;

    /* iterate over all relationships */
    uint32_t count;
    count = data_visible_set_get_relationship_count ( (*this_).input_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get relationship */
        const data_relationship_t *relation;
        relation = data_visible_set_get_relationship_const ( (*this_).input_data, index );
        if (( relation != NULL ) && ( data_relationship_is_valid( relation ) ))
        {
            if ( from_classifier_id == data_relationship_get_from_classifier_id( relation ) )
            {
                const bool show = data_rules_diagram_shows_relationship ( &((*this_).filter_rules),
                                                                          (*this_).input_data,
                                                                          data_relationship_get_id( relation )
                                                                        );

                if ( show )
                {
                    const int64_t to_classifier_id = data_relationship_get_to_classifier_id( relation );
                    const data_classifier_t *dest_classifier = data_visible_set_get_classifier_by_id_const ( (*this_).input_data,
                                                                                                             to_classifier_id
                                                                                                           );
                    if ( dest_classifier != NULL )
                    {
                        /* destination classifier found, print the relation */
                        write_err |= io_format_writer_write_relationship( format_writer,
                                                                          relation,
                                                                          dest_classifier
                                                                        );
                    }
                    else
                    {
                        assert ( false );  /* show should not be true if dest_classifier == NULL */
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
Copyright 2016-2019 Andreas Warnke

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
