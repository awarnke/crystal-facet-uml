/* File: pencil_description_writer.c; Copyright and License: see below */

#include "pencil_description_writer.h"
#include "trace.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void pencil_description_writer_init( pencil_description_writer_t *this_, pencil_input_data_t *input_data )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    (*this_).input_data = input_data;

    TRACE_END();
}

void pencil_description_writer_destroy( pencil_description_writer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).input_data = NULL;

    TRACE_END();
}

int pencil_description_writer_draw ( pencil_description_writer_t *this_, FILE *out )
{
    TRACE_BEGIN();
    assert( NULL != out );
    int result = 0;

    /* write diagram */
    result |= pencil_description_writer_private_write_diagram( this_, out );

    /* write all classifiers */
    result |= pencil_description_writer_private_write_classifiers( this_, out );

    TRACE_END_ERR( result );
    return result;
}

static const char *TWO_NEWLINES = "\n\n";
static const size_t TWO_NEWLINES_LEN = 2;
static const char *LINE_END = "\n";
static const size_t LINE_END_LEN = 1;
static const char *SPACE = "  ";
static const size_t SPACE_LEN = 2;
static const char *COLON_SPACE = ": ";
static const size_t COLON_SPACE_LEN = 2;
static const char *SPACE_ARROW_SPACE = " --> ";
static const size_t SPACE_ARROW_SPACE_LEN = 5;
static const char *TITLE_END = "\n";
static const size_t TITLE_END_LEN = 1;
static const char *SINGLE_INDENT = "| ";
static const char *DOUBLE_INDENT = "  | ";

int pencil_description_writer_private_write_diagram ( pencil_description_writer_t *this_, FILE *out )
{
    TRACE_BEGIN();
    assert( NULL != out );
    int result = 0;
    size_t out_count;  /* checks if the number of written characters matches the expectation */

    /* get diagram */
    data_diagram_t *diag;
    diag = pencil_input_data_get_diagram_ptr( (*this_).input_data );
    TRACE_INFO_INT("printing diagram with id",data_diagram_get_id(diag));

    /* print diagram name */
    const char *diag_name = data_diagram_get_name_ptr(diag);
    size_t diag_name_len = strlen(diag_name);
    out_count = fwrite( diag_name, 1 /* size of char */, diag_name_len, out );
    if ( out_count != diag_name_len )
    {
        TSLOG_ERROR_INT( "not all bytes could be written. missing:", diag_name_len - out_count );
        result = -1;
    }

    /* print an empty line */
    out_count = fwrite( TITLE_END, 1 /* size of char */, TITLE_END_LEN, out );
    if ( out_count != TITLE_END_LEN )
    {
        TSLOG_ERROR_INT( "not all bytes could be written. missing:", TITLE_END_LEN - out_count );
        result = -1;
    }

    /* print diagram description */
    const char *diag_descr = data_diagram_get_description_ptr(diag);
    result |= pencil_description_writer_private_write_indent_multiline_string( this_,
                                                                               SINGLE_INDENT,
                                                                               diag_descr,
                                                                               out
                                                                             );

    TRACE_END_ERR( result );
    return result;
}

int pencil_description_writer_private_write_classifiers ( pencil_description_writer_t *this_, FILE *out )
{
    TRACE_BEGIN();
    assert( NULL != out );
    int result = 0;
    size_t out_count;  /* checks if the number of written characters matches the expectation */

    /* iterate over all classifiers */
    uint32_t count;
    count = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get classifier */
        data_visible_classifier_t *visible_classifier;
        visible_classifier = pencil_input_data_get_visible_classifier_ptr ( (*this_).input_data, index );
        if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
        {
            data_classifier_t *classifier;
            classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );
            int64_t classifier_id = data_classifier_get_id(classifier);
            TRACE_INFO_INT( "printing classifier with id", classifier_id );

            /* print an empty line */
            out_count = fwrite( TWO_NEWLINES, 1 /* size of char */, TWO_NEWLINES_LEN, out );
            if ( out_count != TWO_NEWLINES_LEN )
            {
                TSLOG_ERROR_INT( "not all bytes could be written. missing:", TWO_NEWLINES_LEN - out_count );
                result = -1;
            }

            /* print classifier name */
            const char *classifier_name = data_classifier_get_name_ptr(classifier);
            size_t classifier_name_len = strlen(classifier_name);
            out_count = fwrite( classifier_name, 1 /* size of char */, classifier_name_len, out );
            if ( out_count != classifier_name_len )
            {
                TSLOG_ERROR_INT( "not all bytes could be written. missing:", classifier_name_len - out_count );
                result = -1;
            }

            /* print an empty line */
            out_count = fwrite( TITLE_END, 1 /* size of char */, TITLE_END_LEN, out );
            if ( out_count != TITLE_END_LEN )
            {
                TSLOG_ERROR_INT( "not all bytes could be written. missing:", TITLE_END_LEN - out_count );
                result = -1;
            }

            /* print classifier description */
            const char *classifier_descr = data_classifier_get_description_ptr(classifier);
            result |= pencil_description_writer_private_write_indent_multiline_string( this_,
                                                                                       SINGLE_INDENT,
                                                                                       classifier_descr,
                                                                                       out
                                                                                     );

            /* print all features */
            result |= pencil_description_writer_private_write_features_of_classifier( this_, classifier_id, out );

            /* print all relationships */
            result |= pencil_description_writer_private_write_relations_of_classifier( this_, classifier_id, out );
        }
    }

    TRACE_END_ERR( result );
    return result;
}

int pencil_description_writer_private_write_features_of_classifier ( pencil_description_writer_t *this_, int64_t classifier_id, FILE *out )
{
    TRACE_BEGIN();
    assert( NULL != out );
    assert( DATA_ID_VOID_ID != classifier_id );
    int result = 0;
    size_t out_count;  /* checks if the number of written characters matches the expectation */

    /* iterate over all features */
    uint32_t count;
    count = pencil_input_data_get_feature_count ( (*this_).input_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get feature */
        data_feature_t *feature;
        feature = pencil_input_data_get_feature_ptr ( (*this_).input_data, index );
        if (( feature != NULL ) && ( data_feature_is_valid( feature ) ))
        {
            if ( classifier_id == data_feature_get_classifier_id( feature ) )
            {
                /* print a new line */
                out_count = fwrite( SPACE, 1 /* size of char */, SPACE_LEN, out );
                if ( out_count != SPACE_LEN )
                {
                    TSLOG_ERROR_INT( "not all bytes could be written. missing:", SPACE_LEN - out_count );
                    result = -1;
                }

                /* print feature name */
                const char *feature_key = data_feature_get_key_ptr( feature );
                size_t feature_key_len = strlen(feature_key);
                out_count = fwrite( feature_key, 1 /* size of char */, feature_key_len, out );
                if ( out_count != feature_key_len )
                {
                    TSLOG_ERROR_INT( "not all bytes could be written. missing:", feature_key_len - out_count );
                    result = -1;
                }

                /* print feature value (optional) */
                const char *feature_value = data_feature_get_value_ptr( feature );
                size_t feature_value_len = strlen(feature_value);
                if ( 0 != feature_value_len )
                {
                    /* print colon space */
                    out_count = fwrite( COLON_SPACE, 1 /* size of char */, COLON_SPACE_LEN, out );
                    if ( out_count != COLON_SPACE_LEN )
                    {
                        TSLOG_ERROR_INT( "not all bytes could be written. missing:", COLON_SPACE_LEN - out_count );
                        result = -1;
                    }

                    out_count = fwrite( feature_value, 1 /* size of char */, feature_value_len, out );
                    if ( out_count != feature_value_len )
                    {
                        TSLOG_ERROR_INT( "not all bytes could be written. missing:", feature_value_len - out_count );
                        result = -1;
                    }
                }

                /* print an empty line */
                out_count = fwrite( LINE_END, 1 /* size of char */, LINE_END_LEN, out );
                if ( out_count != LINE_END_LEN )
                {
                    TSLOG_ERROR_INT( "not all bytes could be written. missing:", LINE_END_LEN - out_count );
                    result = -1;
                }

                /* print feature description */
                const char *feature_descr = data_feature_get_description_ptr( feature );
                result |= pencil_description_writer_private_write_indent_multiline_string( this_,
                                                                                           DOUBLE_INDENT,
                                                                                           feature_descr,
                                                                                           out
                                                                                         );
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}

int pencil_description_writer_private_write_relations_of_classifier ( pencil_description_writer_t *this_, int64_t classifier_id, FILE *out )
{
    TRACE_BEGIN();
    assert( NULL != out );
    assert( DATA_ID_VOID_ID != classifier_id );
    int result = 0;
    size_t out_count;  /* checks if the number of written characters matches the expectation */

    /* iterate over all relationships */
    uint32_t count;
    count = pencil_input_data_get_relationship_count ( (*this_).input_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        /* get relationship */
        data_relationship_t *relation;
        relation = pencil_input_data_get_relationship_ptr ( (*this_).input_data, index );
        if (( relation != NULL ) && ( data_relationship_is_valid( relation ) ))
        {
            if ( classifier_id == data_relationship_get_from_classifier_id( relation ) )
            {
                /* find destination classifier */
                uint32_t dest_count;
                dest_count = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );
                for ( uint32_t dest_index = 0; dest_index < dest_count; dest_index ++ )
                {
                    /* get classifier */
                    data_visible_classifier_t *visible_classifier;
                    visible_classifier = pencil_input_data_get_visible_classifier_ptr ( (*this_).input_data, dest_index );
                    if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
                    {
                        data_classifier_t *dest_classifier;
                        dest_classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );
                        int64_t dest_classifier_id = data_classifier_get_id(dest_classifier);

                        if ( dest_classifier_id == data_relationship_get_to_classifier_id( relation ) )
                        {
                            /* destination classifier found, print the relation */

                            /* print a new line */
                            out_count = fwrite( SPACE, 1 /* size of char */, SPACE_LEN, out );
                            if ( out_count != SPACE_LEN )
                            {
                                TSLOG_ERROR_INT( "not all bytes could be written. missing:", SPACE_LEN - out_count );
                                result = -1;
                            }

                            /* print relationship name */
                            const char *relation_name = data_relationship_get_name_ptr( relation );
                            size_t relation_name_len = strlen( relation_name );
                            out_count = fwrite( relation_name, 1 /* size of char */, relation_name_len, out );
                            if ( out_count != relation_name_len )
                            {
                                TSLOG_ERROR_INT( "not all bytes could be written. missing:", relation_name_len - out_count );
                                result = -1;
                            }

                            /* print arrow */
                            out_count = fwrite( SPACE_ARROW_SPACE, 1 /* size of char */, SPACE_ARROW_SPACE_LEN, out );
                            if ( out_count != SPACE_ARROW_SPACE_LEN )
                            {
                                TSLOG_ERROR_INT( "not all bytes could be written. missing:", SPACE_ARROW_SPACE_LEN - out_count );
                                result = -1;
                            }

                            /* print destination classifier name */
                            const char *dest_classifier_name = data_classifier_get_name_ptr( dest_classifier );
                            size_t dest_classifier_name_len = strlen( dest_classifier_name );
                            out_count = fwrite( dest_classifier_name, 1 /* size of char */, dest_classifier_name_len, out );
                            if ( out_count != dest_classifier_name_len )
                            {
                                TSLOG_ERROR_INT( "not all bytes could be written. missing:", dest_classifier_name_len - out_count );
                                result = -1;
                            }

                            /* print an empty line */
                            out_count = fwrite( LINE_END, 1 /* size of char */, LINE_END_LEN, out );
                            if ( out_count != LINE_END_LEN )
                            {
                                TSLOG_ERROR_INT( "not all bytes could be written. missing:", LINE_END_LEN - out_count );
                                result = -1;
                            }

                            /* print relationship description */
                            const char *relation_descr = data_relationship_get_description_ptr( relation );
                            result |= pencil_description_writer_private_write_indent_multiline_string( this_,
                                                                                                       DOUBLE_INDENT,
                                                                                                       relation_descr,
                                                                                                       out
                                                                                                     );
                        }
                        else
                        {
                            TRACE_INFO_INT_INT( "relationship of classifier not printed because destination not in current diagram",
                                                classifier_id,
                                                dest_classifier_id
                                              );
                        }
                    }
                }
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}

int pencil_description_writer_private_write_indent_multiline_string ( pencil_description_writer_t *this_,
                                                                      const char *indent,
                                                                      const char *multiline_string,
                                                                      FILE *out )
{
    TRACE_BEGIN();
    assert( NULL != indent );
    assert( NULL != out );
    int result = 0;
    size_t out_count;  /* checks if the number of written characters matches the expectation */
    size_t indent_length = strlen( indent );

    if ( NULL != multiline_string )
    {
        const char *line_start = multiline_string;
        size_t line_length = 0;
        bool ignore_newline = false;  /* newlines after returns are ignored */

        size_t length = strlen( multiline_string );
        for ( size_t index = 0; index < length; index ++ )
        {
            bool end_of_line = false;

            char current = multiline_string[index];
            if ( '\r' == current )
            {
                ignore_newline = true;
                end_of_line = true;
            }
            else if ( '\n' == current )
            {
                if ( ignore_newline )
                {
                    line_start = &(multiline_string[index+1]);
                }
                else
                {
                    end_of_line = true;
                }
                ignore_newline = false;
            }
            else
            {
                ignore_newline = false;
                line_length ++;
                if ( index+1 == length )
                {
                    end_of_line = true;
                }
            }

            if ( end_of_line )
            {
                /* print indent pattern */
                out_count = fwrite( indent, 1 /* size of char */, indent_length, out );
                if ( out_count != indent_length )
                {
                    TSLOG_ERROR_INT( "not all bytes could be written. missing:", indent_length - out_count );
                    result = -1;
                }

                /* print next line */
                out_count = fwrite( line_start, 1 /* size of char */, line_length, out );
                if ( out_count != line_length )
                {
                    TSLOG_ERROR_INT( "not all bytes could be written. missing:", line_length - out_count );
                    result = -1;
                }

                /* print newline */
                out_count = fwrite( LINE_END, 1 /* size of char */, LINE_END_LEN, out );
                if ( out_count != LINE_END_LEN )
                {
                    TSLOG_ERROR_INT( "not all bytes could be written. missing:", LINE_END_LEN - out_count );
                    result = -1;
                }

                /* reset line indices */
                line_start = &(multiline_string[index+1]);
                line_length = 0;
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2018 Andreas Warnke

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
