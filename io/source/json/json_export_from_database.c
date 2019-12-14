/* File: json_export_from_database.c; Copyright and License: see below */

#include "json/json_export_from_database.h"
#include "json/json_serializer.h"
#include "json/json_deserializer.h"
#include "util/string/utf8string.h"
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

data_error_t json_export_from_database_export_set_to_buf( json_export_from_database_t *this_,
                                                          const data_small_set_t *set_to_be_copied,
                                                          utf8stringbuf_t out_buf )
{
    TRACE_BEGIN();
    assert( NULL != set_to_be_copied );

    data_error_t serialize_error = DATA_ERROR_NONE;
    data_error_t read_error;
    json_serializer_t serializer;

    json_serializer_init( &serializer );
    utf8stringbuf_clear( out_buf);

    serialize_error |= json_serializer_begin_data( &serializer, out_buf);

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
                    serialize_error |= json_serializer_append_diagram( &serializer, &out_diagram, out_buf);
                }
                else
                {
                    /* program internal error */
                    TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
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
                read_error = data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                         data_id_get_row_id( &current_id ),
                                                                         &out_classifier
                                                                       );

                if ( read_error == DATA_ERROR_NONE )
                {
                    uint32_t out_feature_count;
                    read_error = data_database_reader_get_features_by_classifier_id ( (*this_).db_reader,
                                                                                      data_id_get_row_id( &current_id ),
                                                                                      JSON_EXPORT_FROM_DATABASE_MAX_FEATURES,
                                                                                      &((*this_).temp_features),
                                                                                      &out_feature_count
                                                                                    );

                    if ( read_error == DATA_ERROR_NONE )
                    {
                        serialize_error |= json_serializer_append_classifier( &serializer,
                                                                              &out_classifier,
                                                                              &((*this_).temp_features),
                                                                              out_feature_count,
                                                                              out_buf
                                                                            );
                    }
                    else
                    {
                        /* program internal error */
                        TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all features of the classifier of the set." );
                    }
                }
                else
                {
                    /* program internal error */
                    TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
                }
            }
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            {
                data_classifier_t out_classifier;
                data_diagramelement_t out_diagramelement;
                int64_t classifier_id;

                read_error = data_database_reader_get_diagramelement_by_id ( (*this_).db_reader,
                                                                             data_id_get_row_id( &current_id ),
                                                                             &out_diagramelement
                                                                           );

                if ( read_error == DATA_ERROR_NONE )
                {
                    classifier_id = data_diagramelement_get_classifier_id( &out_diagramelement );

                    read_error = data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                             classifier_id,
                                                                             &out_classifier
                                                                           );

                    if ( read_error == DATA_ERROR_NONE )
                    {
                        uint32_t out_feature_count;
                        read_error = data_database_reader_get_features_by_classifier_id ( (*this_).db_reader,
                                                                                          classifier_id,
                                                                                          JSON_EXPORT_FROM_DATABASE_MAX_FEATURES,
                                                                                          &((*this_).temp_features),
                                                                                          &out_feature_count
                                                                                        );

                        if ( read_error == DATA_ERROR_NONE )
                        {
                            serialize_error |= json_serializer_append_classifier( &serializer,
                                                                                  &out_classifier,
                                                                                  &((*this_).temp_features),
                                                                                  out_feature_count,
                                                                                  out_buf
                                                                                );
                        }
                        else
                        {
                            /* program internal error */
                            TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all features of the classifier of the set." );
                        }
                    }
                    else
                    {
                        /* program internal error */
                        TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
                    }
                }
                else
                {
                    /* program internal error */
                    TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
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
            }
            break;

            case DATA_TABLE_RELATIONSHIP:
            {
                /* relationships are serialized in third pass */
            }
            break;

            default:
            {
                serialize_error |= DATA_ERROR_VALUE_OUT_OF_RANGE;
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
                                                                              data_relationship_get_from_classifier_id( &out_relation ),
                                                                              &from_classifier
                                                                            );
                    if ( DATA_ID_VOID_ID == data_relationship_get_from_feature_id( &out_relation ) )
                    {
                        data_feature_init_empty( &((*this_).temp_features[0]) );
                    }
                    else
                    {
                        read_error |= data_database_reader_get_feature_by_id ( (*this_).db_reader,
                                                                               data_relationship_get_from_feature_id( &out_relation ),
                                                                               &((*this_).temp_features[0])
                                                                             );
                    }

                    /* get destination */
                    read_error |= data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                              data_relationship_get_to_classifier_id( &out_relation ),
                                                                              &to_classifier
                                                                            );
                    if ( DATA_ID_VOID_ID == data_relationship_get_to_feature_id( &out_relation ) )
                    {
                        data_feature_init_empty( &((*this_).temp_features[1]) );
                    }
                    else
                    {
                        read_error |= data_database_reader_get_feature_by_id ( (*this_).db_reader,
                                                                               data_relationship_get_to_feature_id( &out_relation ),
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
                                                                                &((*this_).temp_features[1]),
                                                                                out_buf
                                                                              );
                    }
                    else
                    {
                        /* program internal error */
                        TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all features of the classifier of the set." );
                    }
                }
                else
                {
                    /* program internal error */
                    TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
                }
            }
            break;

            default:
            break;
        }
    }

    serialize_error |= json_serializer_end_data( &serializer, out_buf);

    json_serializer_destroy( &serializer );

    TRACE_END_ERR(serialize_error);
    return serialize_error;
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
