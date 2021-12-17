/* File: json_importer.c; Copyright and License: see below */

#include "json/json_importer.h"
#include "u8/u8_error.h"
#include "util/string/utf8string.h"
#include "trace.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <stdbool.h>

void json_importer_init( json_importer_t *this_,
                         io_import_elements_t *elements_importer,
                         data_stat_t *io_stat )
{
    TRACE_BEGIN();
    assert( NULL != elements_importer );
    assert( NULL != io_stat );

    (*this_).elements_importer = elements_importer;
    (*this_).stat = io_stat;

    data_rules_init ( &((*this_).data_rules) );

    TRACE_END();
}

void json_importer_destroy( json_importer_t *this_ )
{
    TRACE_BEGIN();
    assert( NULL != (*this_).elements_importer );
    assert( NULL != (*this_).stat );

    data_rules_destroy ( &((*this_).data_rules) );

    (*this_).elements_importer = NULL;
    (*this_).stat = NULL;

    TRACE_END();
}

u8_error_t json_importer_import_stream( json_importer_t *this_,
                                          universal_input_stream_t *json_text,
                                          uint32_t *out_read_line )
{
    TRACE_BEGIN();
    assert( NULL != json_text );
    assert( NULL != out_read_line );

    u8_error_t parse_error = U8_ERROR_NONE;

    json_deserializer_init( &((*this_).temp_deserializer), json_text );

    /* read header */
    if ( U8_ERROR_NONE == parse_error )
    {
        parse_error = json_deserializer_expect_header( &((*this_).temp_deserializer) );
    }

    if ( U8_ERROR_NONE == parse_error )
    {
        parse_error = json_importer_private_import_views( this_ );
    }

    if ( U8_ERROR_NONE == parse_error )
    {
        parse_error = json_importer_private_import_nodes( this_ );
    }

    if ( U8_ERROR_NONE == parse_error )
    {
        parse_error = json_importer_private_import_edges( this_ );
    }

    if ( U8_ERROR_NONE == parse_error )
    {
        parse_error = json_deserializer_expect_footer( &((*this_).temp_deserializer) );
    }

    json_deserializer_get_read_line ( &((*this_).temp_deserializer), out_read_line );
    json_deserializer_destroy( &((*this_).temp_deserializer) );

    TRACE_END_ERR( parse_error );
    return parse_error;
}

u8_error_t json_importer_private_import_views( json_importer_t *this_ )
{
    TRACE_BEGIN();

    u8_error_t parse_error = U8_ERROR_NONE;

    if ( U8_ERROR_NONE == parse_error )
    {
        parse_error = json_deserializer_expect_begin_data( &((*this_).temp_deserializer) );
    }

    if ( U8_ERROR_NONE == parse_error )
    {
        data_table_t next_object_type;
        bool set_end = false;  /* end of data set reached or error at parsing */
        parse_error = json_deserializer_check_end_data( &((*this_).temp_deserializer), &set_end );
        bool any_error = ( parse_error != U8_ERROR_NONE );  /* consolidation of parser errors and writer errors */
        static const uint32_t MAX_LOOP_COUNTER = (CTRL_UNDO_REDO_LIST_MAX_SIZE/2)-2;  /* do not import more things than can be undone */
        for ( int count = 0; ( ! set_end ) && ( ! any_error ) && ( count < MAX_LOOP_COUNTER ); count ++ )
        {
            parse_error = json_deserializer_expect_begin_type_of_element( &((*this_).temp_deserializer), &next_object_type );
            if ( U8_ERROR_NONE == parse_error )
            {
                switch ( next_object_type )
                {
                    case DATA_TABLE_VOID:
                    {
                        /* we are finished */
                        set_end = true;
                    }
                    break;

                    case DATA_TABLE_DIAGRAM:
                    {
                        data_diagram_t new_diagram;
                        char diag_parent_buf[DATA_UUID_STRING_SIZE] = "";
                        utf8stringbuf_t diag_parent_uuid = UTF8STRINGBUF(diag_parent_buf);
                        parse_error = json_deserializer_get_next_diagram ( &((*this_).temp_deserializer),
                                                                           &new_diagram,
                                                                           diag_parent_uuid
                                                                         );
                        if ( U8_ERROR_NONE != parse_error )
                        {
                            /* parser error, break loop: */
                            any_error = true;
                        }
                        else
                        {
                            any_error = io_import_elements_sync_diagram( (*this_).elements_importer,
                                                                         &new_diagram,
                                                                         utf8stringbuf_get_string( diag_parent_uuid )
                                                                       );
                            data_diagram_destroy( &new_diagram );
                        }
                    }
                    break;

                    default:
                    {
                        TSLOG_ERROR( "unexpected token error" );
                        any_error = true;
                    }
                }
                if ( U8_ERROR_NONE == parse_error )
                {
                    parse_error = json_deserializer_expect_end_type_of_element( &((*this_).temp_deserializer) );
                    /* parser error, break loop: */
                    any_error = ( any_error ||( U8_ERROR_NONE != parse_error ));
                }
            }
            else
            {
                /* parser error, break loop: */
                any_error = true;
            }

            if ( U8_ERROR_NONE == parse_error )
            {
                parse_error = json_deserializer_check_end_data( &((*this_).temp_deserializer), &set_end );
                /* parser error, break loop: */
                any_error = ( any_error ||( U8_ERROR_NONE != parse_error ));
            }
        }
    }

    TRACE_END_ERR( parse_error );
    return parse_error;
}

u8_error_t json_importer_private_import_nodes( json_importer_t *this_ )
{
    TRACE_BEGIN();

    u8_error_t parse_error = U8_ERROR_NONE;

    if ( U8_ERROR_NONE == parse_error )
    {
        parse_error = json_deserializer_expect_begin_data( &((*this_).temp_deserializer) );
    }

    if ( U8_ERROR_NONE == parse_error )
    {
        data_table_t next_object_type;
        bool set_end = false;  /* end of data set reached or error at parsing */
        parse_error = json_deserializer_check_end_data( &((*this_).temp_deserializer), &set_end );
        bool any_error = ( parse_error != U8_ERROR_NONE );  /* consolidation of parser errors and writer errors */
        static const uint32_t MAX_LOOP_COUNTER = (CTRL_UNDO_REDO_LIST_MAX_SIZE/2)-2;  /* do not import more things than can be undone */
        for ( int count = 0; ( ! set_end ) && ( ! any_error ) && ( count < MAX_LOOP_COUNTER ); count ++ )
        {
            parse_error = json_deserializer_expect_begin_type_of_element( &((*this_).temp_deserializer), &next_object_type );
            if ( U8_ERROR_NONE == parse_error )
            {
                switch ( next_object_type )
                {
                    case DATA_TABLE_VOID:
                    {
                        /* we are finished */
                        set_end = true;
                    }
                    break;

                    case DATA_TABLE_CLASSIFIER:
                    {
                        data_classifier_t new_classifier;
                        uint32_t feature_count;
                        parse_error = json_deserializer_get_next_classifier ( &((*this_).temp_deserializer),
                                                                              &new_classifier,
                                                                              JSON_IMPORT_TO_DATABASE_MAX_FEATURES,
                                                                              &((*this_).temp_features),
                                                                              &feature_count
                                                                            );
                        if ( U8_ERROR_NONE != parse_error )
                        {
                            /* parser error, break loop: */
                            any_error = true;
                        }
                        else
                        {
                            any_error = io_import_elements_sync_classifier( (*this_).elements_importer,
                                                                            &new_classifier );

                            if ( any_error == 0 )
                            {
                                /* create also the features */
                                for ( int f_index = 0; f_index < feature_count; f_index ++ )
                                {
                                    data_feature_t *current_feature = &((*this_).temp_features[f_index]);

                                    any_error = io_import_elements_sync_feature( (*this_).elements_importer,
                                                                                 current_feature,
                                                                                 data_classifier_get_uuid_const( &new_classifier )
                                                                               );
                                }
                            }

                            data_classifier_destroy( &new_classifier );
                        }
                    }
                    break;

                    default:
                    {
                        TSLOG_ERROR( "unexpected token error" );
                        any_error = true;
                    }
                }
                if ( U8_ERROR_NONE == parse_error )
                {
                    parse_error = json_deserializer_expect_end_type_of_element( &((*this_).temp_deserializer) );
                    /* parser error, break loop: */
                    any_error = ( any_error ||( U8_ERROR_NONE != parse_error ));
                }
            }
            else
            {
                /* parser error, break loop: */
                any_error = true;
            }

            if ( U8_ERROR_NONE == parse_error )
            {
                parse_error = json_deserializer_check_end_data( &((*this_).temp_deserializer), &set_end );
                /* parser error, break loop: */
                any_error = ( any_error ||( U8_ERROR_NONE != parse_error ));
            }
        }
    }

    TRACE_END_ERR( parse_error );
    return parse_error;
}

u8_error_t json_importer_private_import_edges( json_importer_t *this_ )
{
    TRACE_BEGIN();

    u8_error_t parse_error = U8_ERROR_NONE;

    if ( U8_ERROR_NONE == parse_error )
    {
        parse_error = json_deserializer_expect_begin_data( &((*this_).temp_deserializer) );
    }

    if ( U8_ERROR_NONE == parse_error )
    {
        data_table_t next_object_type;
        bool set_end = false;  /* end of data set reached or error at parsing */
        parse_error = json_deserializer_check_end_data( &((*this_).temp_deserializer), &set_end );
        bool any_error = ( parse_error != U8_ERROR_NONE );  /* consolidation of parser errors and writer errors */
        static const uint32_t MAX_LOOP_COUNTER = (CTRL_UNDO_REDO_LIST_MAX_SIZE/2)-2;  /* do not import more things than can be undone */
        for ( int count = 0; ( ! set_end ) && ( ! any_error ) && ( count < MAX_LOOP_COUNTER ); count ++ )
        {
            parse_error = json_deserializer_expect_begin_type_of_element( &((*this_).temp_deserializer), &next_object_type );
            if ( U8_ERROR_NONE == parse_error )
            {
                switch ( next_object_type )
                {
                    case DATA_TABLE_VOID:
                    {
                        /* we are finished */
                        set_end = true;
                    }
                    break;

                    case DATA_TABLE_RELATIONSHIP:
                    {
                        data_relationship_t new_relationship;
                        char rel_from_node_buf[DATA_UUID_STRING_SIZE] = "";
                        utf8stringbuf_t rel_from_node_uuid = UTF8STRINGBUF(rel_from_node_buf);
                        char rel_to_node_buf[DATA_UUID_STRING_SIZE] = "";
                        utf8stringbuf_t rel_to_node_uuid = UTF8STRINGBUF(rel_to_node_buf);
                        parse_error = json_deserializer_get_next_relationship ( &((*this_).temp_deserializer),
                                                                                &new_relationship,
                                                                                rel_from_node_uuid,
                                                                                rel_to_node_uuid
                                                                              );

                        if ( U8_ERROR_NONE != parse_error )
                        {
                            /* parser error, break loop: */
                            any_error = true;
                        }
                        else
                        {
                            any_error = io_import_elements_sync_relationship( (*this_).elements_importer,
                                                                              &new_relationship,
                                                                              utf8stringbuf_get_string( rel_from_node_uuid ),
                                                                              utf8stringbuf_get_string( rel_to_node_uuid )
                                                                            );

                            /* cleanup */
                            data_relationship_destroy ( &new_relationship );
                        }
                    }
                    break;

                    default:
                    {
                        TSLOG_ERROR( "unexpected token error" );
                        any_error = true;
                    }
                }
                if ( U8_ERROR_NONE == parse_error )
                {
                    parse_error = json_deserializer_expect_end_type_of_element( &((*this_).temp_deserializer) );
                    /* parser error, break loop: */
                    any_error = ( any_error ||( U8_ERROR_NONE != parse_error ));
                }
            }
            else
            {
                /* parser error, break loop: */
                any_error = true;
            }

            if ( U8_ERROR_NONE == parse_error )
            {
                parse_error = json_deserializer_check_end_data( &((*this_).temp_deserializer), &set_end );
                /* parser error, break loop: */
                any_error = ( any_error ||( U8_ERROR_NONE != parse_error ));
            }
        }
    }

    TRACE_END_ERR( parse_error );
    return parse_error;
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
