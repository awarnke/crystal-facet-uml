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
    size_t out_count;
    static const char *TWO_NEWLINES = "\n\n";
    static const size_t TWO_NEWLINES_LEN = 2;
    static const char *COLON_NEWLINE = ":\n";
    static const size_t COLON_NEWLINE_LEN = 2;

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
    out_count = fwrite( COLON_NEWLINE, 1 /* size of char */, COLON_NEWLINE_LEN, out );
    if ( out_count != COLON_NEWLINE_LEN )
    {
        TSLOG_ERROR_INT( "not all bytes could be written. missing:", COLON_NEWLINE_LEN - out_count );
        result = -1;
    }

    /* print diagram description */
    const char *diag_descr = data_diagram_get_description_ptr(diag);
    size_t diag_descr_len = strlen(diag_descr);
    out_count = fwrite( diag_descr, 1 /* size of char */, diag_descr_len, out );
    if ( out_count != diag_descr_len )
    {
        TSLOG_ERROR_INT( "not all bytes could be written. missing:", diag_descr_len - out_count );
        result = -1;
    }

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
            TRACE_INFO_INT("printing classifier with id",data_classifier_get_id(classifier));

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
            out_count = fwrite( COLON_NEWLINE, 1 /* size of char */, COLON_NEWLINE_LEN, out );
            if ( out_count != COLON_NEWLINE_LEN )
            {
                TSLOG_ERROR_INT( "not all bytes could be written. missing:", COLON_NEWLINE_LEN - out_count );
                result = -1;
            }

            /* print classifier description */
            const char *classifier_descr = data_classifier_get_description_ptr(classifier);
            size_t classifier_descr_len = strlen(classifier_descr);
            out_count = fwrite( classifier_descr, 1 /* size of char */, classifier_descr_len, out );
            if ( out_count != classifier_descr_len )
            {
                TSLOG_ERROR_INT( "not all bytes could be written. missing:", classifier_descr_len - out_count );
                result = -1;
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2017 Andreas Warnke

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
