/* File: io_diagram_text_exporter.c; Copyright and License: see below */

#include "io_diagram_text_exporter.h"
#include "trace.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void io_diagram_text_exporter_init( io_diagram_text_exporter_t *this_,
                                    io_filter_flag_t filter_flags,  
                                    const data_visible_set_t *input_data )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    (*this_).filter_flags = filter_flags;
    (*this_).input_data = input_data;
    data_rules_init ( &((*this_).filter_rules) );

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

void io_diagram_text_exporter_reinit( io_diagram_text_exporter_t *this_,
                                      io_filter_flag_t filter_flags,
                                      const data_visible_set_t *input_data )
{
    TRACE_BEGIN();
    io_diagram_text_exporter_destroy( this_ );
    io_diagram_text_exporter_init( this_, filter_flags, input_data );
    TRACE_END();
}

void io_diagram_text_exporter_destroy( io_diagram_text_exporter_t *this_ )
{
    TRACE_BEGIN();

    universal_array_list_destroy ( &((*this_).written_id_set) );

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
    write_err |= io_format_writer_start_diagram( format_writer, data_diagram_get_data_id(diag_ptr) );
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
                write_err |= io_format_writer_start_classifier( format_writer );

                write_err |= io_format_writer_write_classifier( format_writer, classifier );

                /* print all features of the classifier */
                write_err |= io_diagram_text_exporter_private_write_features_of_classifier( this_,
                                                                                            classifier_id,
                                                                                            format_writer
                                                                                          );

                /* end classifier */
                write_err |=  io_format_writer_end_classifier( format_writer );
            }
            
            /* print all relationships starting from classifier_id */
            write_err |= io_diagram_text_exporter_private_write_relations_of_classifier( this_,
                                                                                         classifier_id,
                                                                                         format_writer
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

int io_diagram_text_exporter_private_write_features_of_classifier ( io_diagram_text_exporter_t *this_,
                                                                    data_id_t classifier_id,
                                                                    io_format_writer_t *format_writer )
{
    TRACE_BEGIN();
    assert( NULL != format_writer );
    assert( DATA_TABLE_CLASSIFIER == data_id_get_table( &classifier_id ) );
    assert( DATA_ID_VOID_ID != data_id_get_row_id( &classifier_id) );
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
            const data_id_t f_classifier_id = data_feature_get_classifier_data_id( feature );
            if ( data_id_equals( &classifier_id, &f_classifier_id ) )
            {
                const bool is_visible = data_rules_diagram_shows_feature ( &((*this_).filter_rules),
                                                                           (*this_).input_data,
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
                                                                     data_id_t from_classifier_id,
                                                                     io_format_writer_t *format_writer )
{
    TRACE_BEGIN();
    assert( NULL != format_writer );
    assert( DATA_TABLE_CLASSIFIER == data_id_get_table( &from_classifier_id ) );
    assert( DATA_ID_VOID_ID != data_id_get_row_id( &from_classifier_id) );
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
            const data_id_t r_from_classifier_id = data_relationship_get_from_classifier_data_id( relation );
            if ( data_id_equals( &from_classifier_id, &r_from_classifier_id ) )
            {
                const bool is_visible = data_rules_diagram_shows_relationship ( &((*this_).filter_rules),
                                                                              (*this_).input_data,
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
                    const int64_t to_classifier_id = data_relationship_get_to_classifier_id( relation );
                    const data_classifier_t *dest_classifier = data_visible_set_get_classifier_by_id_const ( (*this_).input_data,
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
                        write_err |= io_format_writer_write_relationship( format_writer,
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
