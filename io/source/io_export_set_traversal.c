/* File: io_export_set_traversal.c; Copyright and License: see below */

#include "io_export_set_traversal.h"

#include "json/json_element_reader.h"
#include "utf8stringbuf/utf8string.h"
#include "u8stream/universal_memory_output_stream.h"
#include "u8/u8_trace.h"
#include <assert.h>
#include "io_gtk.h"
#include <stdbool.h>

void io_export_set_traversal_init ( io_export_set_traversal_t *this_,
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

void io_export_set_traversal_destroy ( io_export_set_traversal_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).export_stat = NULL;
    (*this_).element_writer = NULL;

    U8_TRACE_END();
}

u8_error_t io_export_set_traversal_export_set( io_export_set_traversal_t *this_,
                                               const data_small_set_t *set_to_be_exported )
{
    U8_TRACE_BEGIN();
    assert( NULL != set_to_be_exported );

    u8_error_t serialize_error = U8_ERROR_NONE;

    const char *const document_title = "";
    serialize_error |= io_element_writer_start_main( (*this_).element_writer, document_title );

    /* first pass: serialize the diagram(s) if there is one/some */
    for ( int index = 0; index < data_small_set_get_count( set_to_be_exported ); index ++ )
    {
        data_id_t current_id;
        current_id = data_small_set_get_id( set_to_be_exported, index );
        switch ( data_id_get_table( &current_id ) )
        {
            case DATA_TABLE_DIAGRAM:
            {
                serialize_error |= io_export_set_traversal_private_export_diagram( this_, current_id );
            }
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            {
                serialize_error |= io_export_set_traversal_private_export_diagramelement( this_, current_id );
            }
            break;

            case DATA_TABLE_CLASSIFIER:
            {
                serialize_error |= io_export_set_traversal_private_export_classifier( this_, current_id );
            }
            break;

            case DATA_TABLE_FEATURE:
            {
                serialize_error |= io_export_set_traversal_private_export_feature( this_, current_id );
            }
            break;

            case DATA_TABLE_RELATIONSHIP:
            {
                serialize_error |= io_export_set_traversal_private_export_relationship( this_, current_id );
            }
            break;

            default:
            break;
        }
    }

    serialize_error |= io_element_writer_end_main( (*this_).element_writer );

    U8_TRACE_END_ERR(serialize_error);
    return serialize_error;
}

u8_error_t io_export_set_traversal_private_export_diagram( io_export_set_traversal_t *this_, data_id_t id )
{
    U8_TRACE_BEGIN();
    assert( data_id_is_valid( &id ) );
    assert( DATA_TABLE_DIAGRAM == data_id_get_table( &id ) );
    u8_error_t serialize_error = U8_ERROR_NONE;
    u8_error_t read_error;

    data_diagram_t out_diagram;
    read_error = data_database_reader_get_diagram_by_id ( (*this_).db_reader,
                                                          data_id_get_row_id( &id ),
                                                          &out_diagram
                                                        );

    if ( read_error == U8_ERROR_NONE )
    {
        /* write diagram */
        const char *const diagram_file_base_name = "";
        serialize_error |= io_element_writer_start_diagram( (*this_).element_writer, &out_diagram );
        serialize_error |= io_element_writer_assemble_diagram( (*this_).element_writer,
                                                               NULL,  /* parent, currently not needed here */
                                                               &out_diagram,
                                                               diagram_file_base_name
                                                             );
        serialize_error |= io_element_writer_end_diagram( (*this_).element_writer, &out_diagram );
    }
    else
    {
        /* program internal error */
        U8_LOG_ERROR( "io_export_set_traversal_export_set could not read all data of the set." );
        data_stat_inc_count ( (*this_).export_stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_ERROR );
    }

    U8_TRACE_END_ERR(serialize_error);
    return serialize_error;
}

u8_error_t io_export_set_traversal_private_export_diagramelement( io_export_set_traversal_t *this_, data_id_t id )
{
    U8_TRACE_BEGIN();
    assert( data_id_is_valid( &id ) );
    assert( DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( &id ) );
    u8_error_t serialize_error = U8_ERROR_NONE;
    u8_error_t read_error;

    data_classifier_t out_classifier;
    data_diagramelement_t out_diagramelement;
    /* data_diagram_t out_diagram; */

    read_error = data_database_reader_get_diagramelement_by_id( (*this_).db_reader,
                                                                data_id_get_row_id( &id ),
                                                                &out_diagramelement
                                                              );

    if ( read_error == U8_ERROR_NONE )
    {
        /* get classifier */
        const data_row_t classifier_id = data_diagramelement_get_classifier_row_id( &out_diagramelement );
        read_error = data_database_reader_get_classifier_by_id( (*this_).db_reader,
                                                                classifier_id,
                                                                &out_classifier
                                                              );

        /* get focused_feature */
        /*
        assert ( IO_EXPORT_SET_TRAVERSAL_MAX_FEATURES >= 1 );
        const data_row_t focused_feature_id = data_diagramelement_get_focused_feature_row_id( &out_diagramelement );
        if ( DATA_ROW_VOID == focused_feature_id )
        {
            data_feature_init_empty( &((*this_).temp_features[0]) );
        }
        else
        {
            read_error |= data_database_reader_get_feature_by_id ( (*this_).db_reader,
                                                                   focused_feature_id,
                                                                   &((*this_).temp_features[0])
                                                                 );
        }
        */

        /* get diagram */
        /*
        const data_row_t diag_id = data_diagramelement_get_diagram_row_id( &out_diagramelement );
        read_error |= data_database_reader_get_diagram_by_id ( (*this_).db_reader,
                                                               diag_id,
                                                               &out_diagram
                                                             );
        */

        if ( read_error == U8_ERROR_NONE )
        {
            /* intentionally not supported: diagramelements */
            U8_TRACE_INFO( "io_export_set_traversal_export_set does not copy single diagramelements, only referenced classifiers." );
            /*
            serialize_error |= io_element_writer_start_diagramelement( (*this_).element_writer,
                                                                       &out_diagram,
                                                                       &out_diagramelement
                                                                     );

            serialize_error |= io_element_writer_assemble_diagramelement( (*this_).element_writer,
                                                                          &out_diagram,
                                                                          &out_diagramelement,
                                                                          &out_classifier,
                                                                          &((*this_).temp_features[0]
                                                                        );

            serialize_error |= io_element_writer_end_diagramelement( (*this_).element_writer,
                                                                     &out_diagram,
                                                                     &out_diagramelement
                                                                   );
            */
        }

        data_feature_iterator_t feature_iterator;
        read_error |= data_feature_iterator_init_empty( &feature_iterator );
        read_error |= data_database_reader_get_features_by_classifier_id ( (*this_).db_reader, classifier_id, &feature_iterator );
        if ( read_error == U8_ERROR_NONE )
        {
            /* write classifier */
            serialize_error |= io_element_writer_start_classifier( (*this_).element_writer,
                                                                   DATA_CLASSIFIER_TYPE_VOID,  /* host_type */
                                                                   &out_classifier
                                                                 );
            serialize_error |= io_element_writer_assemble_classifier( (*this_).element_writer,
                                                                      DATA_CLASSIFIER_TYPE_VOID,  /* host_type */
                                                                      &out_classifier
                                                                    );

            /* write classifier-features */
            while ( data_feature_iterator_has_next( &feature_iterator ) )
            {
                read_error |= data_feature_iterator_next( &feature_iterator, &((*this_).temp_feature_0) );
                const data_classifier_type_t parent_type = data_classifier_get_main_type( &out_classifier );
                serialize_error |= io_element_writer_start_feature( (*this_).element_writer,
                                                                    parent_type,
                                                                    &((*this_).temp_feature_0)
                                                                  );

                serialize_error |= io_element_writer_assemble_feature( (*this_).element_writer,
                                                                       &out_classifier,
                                                                       &((*this_).temp_feature_0)
                                                                     );

                serialize_error |= io_element_writer_end_feature( (*this_).element_writer,
                                                                  parent_type,
                                                                  &((*this_).temp_feature_0)
                                                                );
            }

            serialize_error |= io_element_writer_end_classifier( (*this_).element_writer,
                                                                 DATA_CLASSIFIER_TYPE_VOID,  /* host_type */
                                                                 &out_classifier
                                                               );
        }
        else
        {
            /* program internal error */
            U8_LOG_ERROR( "io_export_set_traversal_export_set could not read referenced data of the set." );
            data_stat_inc_count ( (*this_).export_stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_ERROR );
        }
        read_error |= data_feature_iterator_destroy( &feature_iterator );
    }
    else
    {
        /* program internal error */
        U8_LOG_ERROR( "io_export_set_traversal_export_set could not read all data of the set." );
        data_stat_inc_count ( (*this_).export_stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_ERROR );
    }

    U8_TRACE_END_ERR(serialize_error);
    return serialize_error;
}

u8_error_t io_export_set_traversal_private_export_classifier( io_export_set_traversal_t *this_, data_id_t id )
{
    U8_TRACE_BEGIN();
    assert( data_id_is_valid( &id ) );
    assert( DATA_TABLE_CLASSIFIER == data_id_get_table( &id ) );
    u8_error_t serialize_error = U8_ERROR_NONE;
    u8_error_t read_error;

    data_classifier_t out_classifier;
    read_error = data_database_reader_get_classifier_by_id( (*this_).db_reader,
                                                            data_id_get_row_id( &id ),
                                                            &out_classifier
                                                          );

    data_feature_iterator_t feature_iterator;
    read_error |= data_feature_iterator_init_empty( &feature_iterator );
    read_error |= data_database_reader_get_features_by_classifier_id ( (*this_).db_reader, data_id_get_row_id( &id ), &feature_iterator );
    if ( read_error == U8_ERROR_NONE )
    {
        /* write classifier */
        serialize_error |= io_element_writer_start_classifier( (*this_).element_writer,
                                                               DATA_CLASSIFIER_TYPE_VOID,  /* host_type */
                                                               &out_classifier
                                                             );
        serialize_error |= io_element_writer_assemble_classifier( (*this_).element_writer,
                                                                  DATA_CLASSIFIER_TYPE_VOID,  /* host_type */
                                                                  &out_classifier
                                                                );

        /* write classifier-features */
        while ( data_feature_iterator_has_next( &feature_iterator ) )
        {
            read_error |= data_feature_iterator_next( &feature_iterator, &((*this_).temp_feature_0) );
            const data_classifier_type_t parent_type = data_classifier_get_main_type( &out_classifier );
            serialize_error |= io_element_writer_start_feature( (*this_).element_writer,
                                                                parent_type,
                                                                &((*this_).temp_feature_0)
                                                              );

            serialize_error |= io_element_writer_assemble_feature( (*this_).element_writer,
                                                                   &out_classifier,
                                                                   &((*this_).temp_feature_0)
                                                                 );

            serialize_error |= io_element_writer_end_feature( (*this_).element_writer,
                                                              parent_type,
                                                              &((*this_).temp_feature_0)
                                                            );
        }

        serialize_error |= io_element_writer_end_classifier( (*this_).element_writer,
                                                             DATA_CLASSIFIER_TYPE_VOID,  /* host_type */
                                                             &out_classifier
                                                           );
    }
    else
    {
        /* program internal error */
        U8_LOG_ERROR( "io_export_set_traversal_export_set could not read all data of the set." );
        data_stat_inc_count ( (*this_).export_stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_ERROR );
    }
    read_error |= data_feature_iterator_destroy( &feature_iterator );

    U8_TRACE_END_ERR(serialize_error);
    return serialize_error;
}

u8_error_t io_export_set_traversal_private_export_feature( io_export_set_traversal_t *this_, data_id_t id )
{
    U8_TRACE_BEGIN();
    assert( data_id_is_valid( &id ) );
    assert( DATA_TABLE_FEATURE == data_id_get_table( &id ) );
    u8_error_t serialize_error = U8_ERROR_NONE;
    u8_error_t read_error;

    data_feature_t out_feature;
    read_error = data_database_reader_get_feature_by_id ( (*this_).db_reader,
                                                          data_id_get_row_id( &id ),
                                                          &out_feature
                                                        );

    if ( read_error == U8_ERROR_NONE )
    {
        const data_feature_type_t feat_type = data_feature_get_main_type( &out_feature );
        const data_stat_table_t feat_or_lifeline
            = ( feat_type == DATA_FEATURE_TYPE_LIFELINE ) ? DATA_STAT_TABLE_LIFELINE : DATA_STAT_TABLE_FEATURE;

        /* intentionally not supported */
        U8_TRACE_INFO( "io_export_set_traversal_export_set does not copy single features, only complete classifiers." );
        data_stat_inc_count ( (*this_).export_stat, feat_or_lifeline, DATA_STAT_SERIES_IGNORED );
    }
    else
    {
        /* program internal error */
        U8_LOG_ERROR( "io_export_set_traversal_export_set could not read all data of the set." );
        data_stat_inc_count ( (*this_).export_stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_ERROR );
    }

    U8_TRACE_END_ERR(serialize_error);
    return serialize_error;
}

u8_error_t io_export_set_traversal_private_export_relationship( io_export_set_traversal_t *this_, data_id_t id )
{
    U8_TRACE_BEGIN();
    assert( data_id_is_valid( &id ) );
    assert( DATA_TABLE_RELATIONSHIP == data_id_get_table( &id ) );
    u8_error_t serialize_error = U8_ERROR_NONE;
    u8_error_t read_error;

    data_relationship_t out_relation;
    read_error = data_database_reader_get_relationship_by_id ( (*this_).db_reader,
                                                               data_id_get_row_id( &id ),
                                                               &out_relation
                                                             );

    if ( read_error == U8_ERROR_NONE )
    {
        data_classifier_t from_classifier;
        data_classifier_t to_classifier;

        /* get source */
        read_error |= data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                  data_relationship_get_from_classifier_row_id( &out_relation ),
                                                                  &from_classifier
                                                                );
        if ( DATA_ROW_VOID == data_relationship_get_from_feature_row_id( &out_relation ) )
        {
            data_feature_init_empty( &((*this_).temp_feature_0) );
        }
        else
        {
            read_error |= data_database_reader_get_feature_by_id ( (*this_).db_reader,
                                                                   data_relationship_get_from_feature_row_id( &out_relation ),
                                                                   &((*this_).temp_feature_0)
                                                                 );
        }

        /* get destination */
        read_error |= data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                  data_relationship_get_to_classifier_row_id( &out_relation ),
                                                                  &to_classifier
                                                                );
        if ( DATA_ROW_VOID == data_relationship_get_to_feature_row_id( &out_relation ) )
        {
            data_feature_init_empty( &((*this_).temp_feature_1) );
        }
        else
        {
            read_error |= data_database_reader_get_feature_by_id ( (*this_).db_reader,
                                                                   data_relationship_get_to_feature_row_id( &out_relation ),
                                                                   &((*this_).temp_feature_1)
                                                                 );
        }

        /* serialize */
        if ( read_error == U8_ERROR_NONE )
        {
            /* write relationship */
            serialize_error |= io_element_writer_start_relationship( (*this_).element_writer,
                                                                     DATA_CLASSIFIER_TYPE_VOID,  /* host_type */
                                                                     &out_relation
                                                                   );
            serialize_error |= io_element_writer_assemble_relationship( (*this_).element_writer,
                                                                        NULL,  /* host */
                                                                        &out_relation,
                                                                        &from_classifier,
                                                                        &((*this_).temp_feature_0),
                                                                        &to_classifier,
                                                                        &((*this_).temp_feature_1)
                                                                      );
            serialize_error |= io_element_writer_end_relationship( (*this_).element_writer,
                                                                   DATA_CLASSIFIER_TYPE_VOID,  /* host_type */
                                                                   &out_relation
                                                                 );
        }
        else
        {
            /* program internal error */
            U8_LOG_ERROR( "io_export_set_traversal_export_set could not read referenced data of the set." );
            data_stat_inc_count ( (*this_).export_stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR );
        }
    }
    else
    {
        /* program internal error */
        U8_LOG_ERROR( "io_export_set_traversal_export_set could not read all data of the set." );
        data_stat_inc_count ( (*this_).export_stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR );
    }

    U8_TRACE_END_ERR(serialize_error);
    return serialize_error;
}


/*
Copyright 2016-2025 Andreas Warnke

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
