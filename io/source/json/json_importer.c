/* File: json_importer.c; Copyright and License: see below */

#include "json/json_importer.h"
#include "u8/u8_error.h"
#include "utf8stringbuf/utf8string.h"
#include "u8/u8_trace.h"
#include <assert.h>
#include "io_gtk.h"
#include <stdbool.h>

void json_importer_init( json_importer_t *this_, io_import_elements_t *elements_importer )
{
    U8_TRACE_BEGIN();
    assert( NULL != elements_importer );

    (*this_).elements_importer = elements_importer;

    data_rules_init ( &((*this_).data_rules) );

    U8_TRACE_END();
}

void json_importer_destroy( json_importer_t *this_ )
{
    U8_TRACE_BEGIN();
    assert( NULL != (*this_).elements_importer );

    data_rules_destroy ( &((*this_).data_rules) );

    (*this_).elements_importer = NULL;

    U8_TRACE_END();
}

u8_error_t json_importer_import_stream( json_importer_t *this_,
                                        universal_input_stream_t *json_text,
                                        u8_error_info_t *out_err_info )
{
    U8_TRACE_BEGIN();
    assert( NULL != json_text );
    assert( NULL != out_err_info );

    u8_error_t sync_error = U8_ERROR_NONE;

    json_element_reader_init( &((*this_).temp_element_reader), json_text );

    /* read header */
    if ( U8_ERROR_NONE == sync_error )
    {
        sync_error = json_element_reader_expect_header( &((*this_).temp_element_reader) );
    }

    if ( U8_ERROR_NONE == sync_error )
    {
        sync_error = json_importer_private_import_views( this_ );
    }

    if ( U8_ERROR_NONE == sync_error )
    {
        sync_error = json_importer_private_import_nodes( this_ );
    }

    if ( U8_ERROR_NONE == sync_error )
    {
        sync_error = json_importer_private_import_edges( this_ );
    }

    if ( U8_ERROR_NONE == sync_error )
    {
        sync_error = json_element_reader_expect_footer( &((*this_).temp_element_reader) );
    }

    /* report line number of current sync_error */
    const uint32_t current_read_line = json_element_reader_get_read_line ( &((*this_).temp_element_reader) );
    u8_error_info_init_line( out_err_info, sync_error, current_read_line );

    json_element_reader_destroy( &((*this_).temp_element_reader) );

    U8_TRACE_END_ERR( sync_error );
    return sync_error;
}

u8_error_t json_importer_private_import_views( json_importer_t *this_ )
{
    U8_TRACE_BEGIN();

    u8_error_t sync_error = U8_ERROR_NONE;

    if ( U8_ERROR_NONE == sync_error )
    {
        sync_error = json_element_reader_expect_begin_top_array( &((*this_).temp_element_reader) );
    }

    if ( U8_ERROR_NONE == sync_error )
    {
        data_table_t next_object_type;
        bool set_end = false;  /* end of data set reached or error at parsing */
        sync_error = json_element_reader_check_end_top_array( &((*this_).temp_element_reader), &set_end );
        while (( ! set_end ) && ( sync_error == U8_ERROR_NONE ))
        {
            sync_error = json_element_reader_expect_begin_type_of_element( &((*this_).temp_element_reader), &next_object_type );
            if ( U8_ERROR_NONE == sync_error )
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
                        data_diagram_init_empty( &((*this_).temp_diagram) );
                        /* parent is the uuid of the parent diagram (if not root) */
                        char diag_parent_buf[DATA_UUID_STRING_SIZE] = "";
                        utf8stringbuf_t diag_parent_uuid = UTF8STRINGBUF(diag_parent_buf);
                        bool has_diagramelements;
                        sync_error = json_element_reader_get_next_diagram( &((*this_).temp_element_reader),
                                                                           &((*this_).temp_diagram),
                                                                           diag_parent_uuid,
                                                                           &has_diagramelements
                                                                         );
                        if ( U8_ERROR_NONE == sync_error )
                        {
                            sync_error = io_import_elements_sync_diagram( (*this_).elements_importer,
                                                                          &((*this_).temp_diagram),
                                                                          utf8stringbuf_get_string( &diag_parent_uuid )
                                                                        );

                            if ( has_diagramelements )
                            {
                                if ( U8_ERROR_NONE == sync_error )  /* stop reading in case of error */
                                {
                                    const char *const diag_uuid = data_diagram_get_uuid_const( &((*this_).temp_diagram) );
                                    sync_error = json_importer_private_import_diagramelement_array( this_, diag_uuid );
                                }
                                if ( U8_ERROR_NONE == sync_error )  /* stop reading in case of error */
                                {
                                    sync_error = json_element_reader_end_unfinished_object( &((*this_).temp_element_reader) );
                                }
                            }

                            data_diagram_destroy( &((*this_).temp_diagram) );
                        }
                    }
                    break;

                    default:
                    {
                        U8_LOG_ERROR( "unexpected token error" );
                        sync_error = U8_ERROR_PARSER_STRUCTURE;
                    }
                }
                if ( U8_ERROR_NONE == sync_error )
                {
                    sync_error = json_element_reader_expect_end_type_of_element( &((*this_).temp_element_reader) );
                }
            }

            if ( U8_ERROR_NONE == sync_error )
            {
                sync_error = json_element_reader_check_end_top_array( &((*this_).temp_element_reader), &set_end );
            }
        }
    }

    U8_TRACE_END_ERR( sync_error );
    return sync_error;
}

u8_error_t json_importer_private_import_nodes( json_importer_t *this_ )
{
    U8_TRACE_BEGIN();

    u8_error_t sync_error = U8_ERROR_NONE;

    if ( U8_ERROR_NONE == sync_error )
    {
        sync_error = json_element_reader_expect_begin_top_array( &((*this_).temp_element_reader) );
    }

    if ( U8_ERROR_NONE == sync_error )
    {
        data_table_t next_object_type;
        bool set_end = false;  /* end of data set reached or error at parsing */
        sync_error = json_element_reader_check_end_top_array( &((*this_).temp_element_reader), &set_end );
        while (( ! set_end ) && ( sync_error == U8_ERROR_NONE ))
        {
            sync_error = json_element_reader_expect_begin_type_of_element( &((*this_).temp_element_reader), &next_object_type );
            if ( U8_ERROR_NONE == sync_error )
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
                        data_classifier_init_empty( &((*this_).temp_classifier) );
                        bool has_features;
                        sync_error = json_element_reader_get_next_classifier( &((*this_).temp_element_reader),
                                                                              &((*this_).temp_classifier),
                                                                              &has_features
                                                                            );
                        if ( U8_ERROR_NONE == sync_error )
                        {
                            sync_error = io_import_elements_sync_classifier( (*this_).elements_importer,
                                                                             &((*this_).temp_classifier) );

                            if ( has_features )
                            {
                                if ( U8_ERROR_NONE == sync_error )  /* stop reading in case of error */
                                {
                                    const char *const class_uuid = data_classifier_get_uuid_const( &((*this_).temp_classifier) );
                                    sync_error = json_importer_private_import_feature_array( this_, class_uuid );
                                }
                                if ( U8_ERROR_NONE == sync_error )  /* stop reading in case of error */
                                {
                                    sync_error = json_element_reader_end_unfinished_object( &((*this_).temp_element_reader) );
                                }
                            }

                            data_classifier_destroy( &((*this_).temp_classifier) );
                        }
                    }
                    break;

                    default:
                    {
                        U8_LOG_ERROR( "unexpected token error" );
                        sync_error = U8_ERROR_PARSER_STRUCTURE;
                    }
                }
                if ( U8_ERROR_NONE == sync_error )
                {
                    sync_error = json_element_reader_expect_end_type_of_element( &((*this_).temp_element_reader) );
                }
            }

            if ( U8_ERROR_NONE == sync_error )
            {
                sync_error = json_element_reader_check_end_top_array( &((*this_).temp_element_reader), &set_end );
            }
        }
    }

    U8_TRACE_END_ERR( sync_error );
    return sync_error;
}

u8_error_t json_importer_private_import_edges( json_importer_t *this_ )
{
    U8_TRACE_BEGIN();

    u8_error_t sync_error = U8_ERROR_NONE;

    if ( U8_ERROR_NONE == sync_error )
    {
        sync_error = json_element_reader_expect_begin_top_array( &((*this_).temp_element_reader) );
    }

    if ( U8_ERROR_NONE == sync_error )
    {
        data_table_t next_object_type;
        bool set_end = false;  /* end of data set reached or error at parsing */
        sync_error = json_element_reader_check_end_top_array( &((*this_).temp_element_reader), &set_end );
        while (( ! set_end ) && ( sync_error == U8_ERROR_NONE ))
        {
            sync_error = json_element_reader_expect_begin_type_of_element( &((*this_).temp_element_reader), &next_object_type );
            if ( U8_ERROR_NONE == sync_error )
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
                        data_relationship_init_empty( &((*this_).temp_relationship) );
                        /* from_node is the uuid of either the from_classifier or (if defined) the from_feature */
                        char rel_from_node_buf[DATA_UUID_STRING_SIZE] = "";
                        utf8stringbuf_t rel_from_node_uuid = UTF8STRINGBUF(rel_from_node_buf);
                        /* to_node is the uuid of either the to_classifier or (if defined) the to_feature */
                        char rel_to_node_buf[DATA_UUID_STRING_SIZE] = "";
                        utf8stringbuf_t rel_to_node_uuid = UTF8STRINGBUF(rel_to_node_buf);
                        sync_error = json_element_reader_get_next_relationship( &((*this_).temp_element_reader),
                                                                                &((*this_).temp_relationship),
                                                                                rel_from_node_uuid,
                                                                                rel_to_node_uuid
                                                                              );

                        if ( U8_ERROR_NONE == sync_error )
                        {
                            sync_error = io_import_elements_sync_relationship( (*this_).elements_importer,
                                                                               &((*this_).temp_relationship),
                                                                               utf8stringbuf_get_string( &rel_from_node_uuid ),
                                                                               utf8stringbuf_get_string( &rel_to_node_uuid )
                                                                             );

                            /* cleanup */
                            data_relationship_destroy( &((*this_).temp_relationship) );
                        }
                    }
                    break;

                    default:
                    {
                        U8_LOG_ERROR( "unexpected token error" );
                        sync_error = U8_ERROR_PARSER_STRUCTURE;
                    }
                }
                if ( U8_ERROR_NONE == sync_error )
                {
                    sync_error = json_element_reader_expect_end_type_of_element( &((*this_).temp_element_reader) );
                }
            }

            if ( U8_ERROR_NONE == sync_error )
            {
                sync_error = json_element_reader_check_end_top_array( &((*this_).temp_element_reader), &set_end );
            }
        }
    }

    U8_TRACE_END_ERR( sync_error );
    return sync_error;
}

u8_error_t json_importer_private_import_diagramelement_array( json_importer_t *this_, const char *diagram_uuid )
{
    U8_TRACE_BEGIN();
    assert ( NULL != diagram_uuid );
    u8_error_t sync_error = U8_ERROR_NONE;

    sync_error = json_element_reader_expect_begin_sub_array( &((*this_).temp_element_reader) );

    bool end_array = false;
    while (( ! end_array )&&( U8_ERROR_NONE == sync_error ))
    {
        sync_error = json_element_reader_check_end_sub_array( &((*this_).temp_element_reader), &end_array );
        if ( U8_ERROR_NONE == sync_error )
        {
            if ( ! end_array )
            {
                data_diagramelement_init_empty( &((*this_).temp_diagramelement) );
                /* node is the uuid of either the classifier or (if defined) the focused_feature */
                char node_uuid_buf[DATA_UUID_STRING_SIZE] = "";
                utf8stringbuf_t node_uuid = UTF8STRINGBUF(node_uuid_buf);
                sync_error |= json_element_reader_get_next_diagramelement( &((*this_).temp_element_reader),
                                                                           &((*this_).temp_diagramelement),
                                                                           node_uuid
                                                                         );
                if ( U8_ERROR_NONE == sync_error )
                {
                    sync_error |= io_import_elements_sync_diagramelement( (*this_).elements_importer,
                                                                          &((*this_).temp_diagramelement),
                                                                          diagram_uuid,
                                                                          utf8stringbuf_get_string( &node_uuid )
                                                                        );
                    data_diagramelement_destroy( &((*this_).temp_diagramelement) );
                }
                else
                {
                    /* error, break loop */
                    end_array = true;
                }
            }
        }
        else
        {
            /* error, break loop */
            sync_error |= U8_ERROR_PARSER_STRUCTURE;
            end_array = true;
        }
    }

    U8_TRACE_END_ERR( sync_error );
    return sync_error;
}

u8_error_t json_importer_private_import_feature_array( json_importer_t *this_, const char *classifier_uuid )
{
    U8_TRACE_BEGIN();
    assert ( NULL != classifier_uuid );
    u8_error_t sync_error = U8_ERROR_NONE;

    sync_error = json_element_reader_expect_begin_sub_array( &((*this_).temp_element_reader) );

    bool end_array = false;
    while (( ! end_array )&&( U8_ERROR_NONE == sync_error ))
    {
        sync_error = json_element_reader_check_end_sub_array( &((*this_).temp_element_reader), &end_array );
        if ( U8_ERROR_NONE == sync_error )
        {
            if ( ! end_array )
            {
                data_feature_init_empty( &((*this_).temp_feature) );
                sync_error |= json_element_reader_get_next_feature( &((*this_).temp_element_reader), &((*this_).temp_feature) );
                if ( U8_ERROR_NONE == sync_error )
                {
                    sync_error |= io_import_elements_sync_feature( (*this_).elements_importer,
                                                                   &((*this_).temp_feature),
                                                                   classifier_uuid
                                                                 );
                    data_feature_destroy( &((*this_).temp_feature) );
                }
                else
                {
                    /* error, break loop */
                    end_array = true;
                }
            }
        }
        else
        {
            /* error, break loop */
            sync_error |= U8_ERROR_PARSER_STRUCTURE;
            end_array = true;
        }
    }

    U8_TRACE_END_ERR( sync_error );
    return sync_error;
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
