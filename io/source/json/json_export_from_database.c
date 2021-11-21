/* File: json_export_from_database.c; Copyright and License: see below */

#include "json/json_export_from_database.h"
#include "json/json_serializer.h"
#include "json/json_deserializer.h"
#include "util/string/utf8string.h"
#include "stream/universal_memory_output_stream.h"
#include "trace.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <stdbool.h>

void json_export_from_database_init ( json_export_from_database_t *this_,
                                      data_database_reader_t *db_reader )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );

    (*this_).db_reader = db_reader;

    TRACE_END();
}

void json_export_from_database_destroy ( json_export_from_database_t *this_ )
{
    TRACE_BEGIN();

    (*this_).db_reader = NULL;

    TRACE_END();
}

int json_export_from_database_export_set_to_buf( json_export_from_database_t *this_,
                                                 const data_small_set_t *set_to_be_copied,
                                                 data_stat_t *io_stat,
                                                 utf8stringbuf_t out_buf )
{
    TRACE_BEGIN();
    assert( NULL != set_to_be_copied );
    assert( NULL != io_stat );

    int serialize_error = 0;
    data_error_t read_error;
    json_serializer_t serializer;

    universal_memory_output_stream_t out_to_memory;
    universal_memory_output_stream_init( &out_to_memory,
                                         utf8stringbuf_get_string( out_buf ),
                                         utf8stringbuf_get_size( out_buf )
                                       );
    json_serializer_init( &serializer, universal_memory_output_stream_get_output_stream(&out_to_memory) );

    serialize_error |= json_serializer_write_header( &serializer );
    serialize_error |= json_serializer_begin_data( &serializer );

    /* first pass: serialize the diagram(s) if there is one/some */
    for ( int index = 0; index < data_small_set_get_count( set_to_be_copied ); index ++ )
    {
        data_id_t current_id;
        current_id = data_small_set_get_id( set_to_be_copied, index );
        switch ( data_id_get_table( &current_id ) )
        {
            case DATA_TABLE_CLASSIFIER:
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            break;

            case DATA_TABLE_DIAGRAM:
            {
                data_diagram_t out_diagram;
                read_error = data_database_reader_get_diagram_by_id ( (*this_).db_reader,
                                                                      data_id_get_row_id( &current_id ),
                                                                      &out_diagram
                                                                    );

                if ( read_error == DATA_ERROR_NONE )
                {
                    serialize_error |= json_serializer_append_diagram( &serializer, &out_diagram );
                    data_stat_inc_count ( io_stat,
                                          DATA_TABLE_DIAGRAM,
                                          (0==serialize_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                                        );
                }
                else
                {
                    /* program internal error */
                    TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
                    data_stat_inc_count ( io_stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_ERROR );
                }
            }
            break;

            case DATA_TABLE_FEATURE:
            break;

            case DATA_TABLE_RELATIONSHIP:
            break;

            default:
            break;
        }
    }

    /* second pass: serialize all classifiers, diagram elements and features */
    for ( int index = 0; index < data_small_set_get_count( set_to_be_copied ); index ++ )
    {
        data_id_t current_id;
        current_id = data_small_set_get_id( set_to_be_copied, index );
        switch ( data_id_get_table( &current_id ) )
        {
            case DATA_TABLE_CLASSIFIER:
            {
                data_classifier_t out_classifier;
                read_error = data_database_reader_get_classifier_by_id( (*this_).db_reader,
                                                                        data_id_get_row_id( &current_id ),
                                                                        &out_classifier
                                                                      );

                if ( read_error == DATA_ERROR_NONE )
                {
                    uint32_t feature_count;
                    read_error = data_database_reader_get_features_by_classifier_id( (*this_).db_reader,
                                                                                     data_id_get_row_id( &current_id ),
                                                                                     JSON_EXPORT_FROM_DATABASE_MAX_FEATURES,
                                                                                     &((*this_).temp_features),
                                                                                     &feature_count
                                                                                   );

                    if ( read_error == DATA_ERROR_NONE )
                    {
                        serialize_error |= json_serializer_begin_classifier( &serializer,
                                                                             &out_classifier
                                                                           );

                        for ( uint32_t index = 0; index < feature_count; index ++ )
                        {
                            serialize_error |= json_serializer_append_feature( &serializer,
                                                                               &(((*this_).temp_features)[index])
                                                                             );
                        }

                        serialize_error |= json_serializer_end_classifier( &serializer,
                                                                           &out_classifier
                                                                         );

                        data_stat_inc_count( io_stat,
                                             DATA_TABLE_CLASSIFIER,
                                             (0==serialize_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                                           );
                        data_stat_add_count( io_stat,
                                             DATA_TABLE_FEATURE,
                                             (0==serialize_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR,
                                             feature_count
                                           );
                    }
                    else
                    {
                        /* program internal error */
                        TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all features of the classifier of the set." );
                        data_stat_inc_count ( io_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_ERROR );
                    }
                }
                else
                {
                    /* program internal error */
                    TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
                    data_stat_inc_count ( io_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_ERROR );
                }
            }
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            {
                data_classifier_t out_classifier;
                data_diagramelement_t out_diagramelement;
                data_row_id_t classifier_id;

                read_error = data_database_reader_get_diagramelement_by_id( (*this_).db_reader,
                                                                            data_id_get_row_id( &current_id ),
                                                                            &out_diagramelement
                                                                          );

                if ( read_error == DATA_ERROR_NONE )
                {
                    classifier_id = data_diagramelement_get_classifier_row_id( &out_diagramelement );

                    read_error = data_database_reader_get_classifier_by_id( (*this_).db_reader,
                                                                            classifier_id,
                                                                            &out_classifier
                                                                          );

                    if ( read_error == DATA_ERROR_NONE )
                    {
                        uint32_t feature_count;
                        read_error = data_database_reader_get_features_by_classifier_id( (*this_).db_reader,
                                                                                         classifier_id,
                                                                                         JSON_EXPORT_FROM_DATABASE_MAX_FEATURES,
                                                                                         &((*this_).temp_features),
                                                                                         &feature_count
                                                                                       );

                        if ( read_error == DATA_ERROR_NONE )
                        {
                            serialize_error |= json_serializer_begin_classifier( &serializer,
                                                                                 &out_classifier
                                                                               );

                            for ( uint32_t index = 0; index < feature_count; index ++ )
                            {
                                serialize_error |= json_serializer_append_feature( &serializer,
                                                                                   &(((*this_).temp_features)[index])
                                                                                 );
                            }

                            serialize_error |= json_serializer_end_classifier( &serializer,
                                                                               &out_classifier
                                                                             );

                            data_stat_inc_count( io_stat,
                                                 DATA_TABLE_CLASSIFIER,
                                                 (0==serialize_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                                               );
                            data_stat_add_count( io_stat,
                                                 DATA_TABLE_FEATURE,
                                                 (0==serialize_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR,
                                                 feature_count
                                               );
                        }
                        else
                        {
                            /* program internal error */
                            TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all features of the classifier of the set." );
                            data_stat_inc_count ( io_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_ERROR );
                        }
                    }
                    else
                    {
                        /* program internal error */
                        TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
                        data_stat_inc_count ( io_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_ERROR );
                    }
                }
                else
                {
                    /* program internal error */
                    TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
                    data_stat_inc_count ( io_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_ERROR );
                }
            }
            break;

            case DATA_TABLE_DIAGRAM:
            {
                /* diagrams are serialized in first pass */
            }
            break;

            case DATA_TABLE_FEATURE:
            {
                /* intentionally not supported */
                TRACE_INFO( "gui_toolbox_private_copy_set_to_clipboard does not copy single features, only complete classifiers." );
                data_stat_inc_count ( io_stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_WARNING );
            }
            break;

            case DATA_TABLE_RELATIONSHIP:
            {
                /* relationships are serialized in third pass */
            }
            break;

            default:
            {
                serialize_error |= -1;
            }
            break;
        }
    }

    /* third pass: serialize all relationships after the source and destination classifiers */
    for ( int index = 0; index < data_small_set_get_count( set_to_be_copied ); index ++ )
    {
        data_id_t current_id;
        current_id = data_small_set_get_id( set_to_be_copied, index );
        switch ( data_id_get_table( &current_id ) )
        {
            case DATA_TABLE_CLASSIFIER:
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            break;

            case DATA_TABLE_DIAGRAM:
            break;

            case DATA_TABLE_FEATURE:
            break;

            case DATA_TABLE_RELATIONSHIP:
            {
                data_relationship_t out_relation;
                read_error = data_database_reader_get_relationship_by_id ( (*this_).db_reader,
                                                                           data_id_get_row_id( &current_id ),
                                                                           &out_relation
                                                                         );

                if ( read_error == DATA_ERROR_NONE )
                {
                    data_classifier_t from_classifier;
                    data_classifier_t to_classifier;
                    assert ( JSON_EXPORT_FROM_DATABASE_MAX_FEATURES >= 2 );

                    /* get source */
                    read_error |= data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                              data_relationship_get_from_classifier_row_id( &out_relation ),
                                                                              &from_classifier
                                                                            );
                    if ( DATA_ROW_ID_VOID == data_relationship_get_from_feature_row_id( &out_relation ) )
                    {
                        data_feature_init_empty( &((*this_).temp_features[0]) );
                    }
                    else
                    {
                        read_error |= data_database_reader_get_feature_by_id ( (*this_).db_reader,
                                                                               data_relationship_get_from_feature_row_id( &out_relation ),
                                                                               &((*this_).temp_features[0])
                                                                             );
                    }

                    /* get destination */
                    read_error |= data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                              data_relationship_get_to_classifier_row_id( &out_relation ),
                                                                              &to_classifier
                                                                            );
                    if ( DATA_ROW_ID_VOID == data_relationship_get_to_feature_row_id( &out_relation ) )
                    {
                        data_feature_init_empty( &((*this_).temp_features[1]) );
                    }
                    else
                    {
                        read_error |= data_database_reader_get_feature_by_id ( (*this_).db_reader,
                                                                               data_relationship_get_to_feature_row_id( &out_relation ),
                                                                               &((*this_).temp_features[1])
                                                                             );
                    }

                    /* serialize */
                    if ( read_error == DATA_ERROR_NONE )
                    {
                        serialize_error |= json_serializer_append_relationship( &serializer,
                                                                                &out_relation,
                                                                                &from_classifier,
                                                                                &((*this_).temp_features[0]),
                                                                                &to_classifier,
                                                                                &((*this_).temp_features[1])
                                                                              );
                        data_stat_inc_count ( io_stat,
                                              DATA_TABLE_RELATIONSHIP,
                                              (0==serialize_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                                            );
                    }
                    else
                    {
                        /* program internal error */
                        TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all features of the classifier of the set." );
                        data_stat_inc_count ( io_stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR );
                    }
                }
                else
                {
                    /* program internal error */
                    TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
                    data_stat_inc_count ( io_stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR );
                }
            }
            break;

            default:
            break;
        }
    }

    serialize_error |= json_serializer_end_data( &serializer );
    serialize_error |= json_serializer_write_footer( &serializer );

    json_serializer_destroy( &serializer );
    serialize_error |= universal_memory_output_stream_write_0term( &out_to_memory );
    universal_memory_output_stream_destroy( &out_to_memory );

    TRACE_END_ERR(serialize_error);
    return serialize_error;
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
