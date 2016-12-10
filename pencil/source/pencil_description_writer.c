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

    /* print diagram name and description */
    data_diagram_t *diag;
    diag = pencil_input_data_get_diagram_ptr( (*this_).input_data );
    TRACE_INFO_INT("printing diagram with id",data_diagram_get_id(diag));

    /* iterate over all classifiers */
    uint32_t count;
    count = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        data_visible_classifier_t *visible_classifier;
        visible_classifier = pencil_input_data_get_visible_classifier_ptr ( (*this_).input_data, index );

        if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
        {
            data_classifier_t *classifier;
            classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );
            TRACE_INFO_INT("printing classifier with id",data_classifier_get_id(classifier));
        }
    }

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2016 Andreas Warnke

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
