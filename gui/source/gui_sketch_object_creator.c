/* File: gui_sketch_object_creator.c; Copyright and License: see below */

#include "gui_sketch_object_creator.h"
#include "util/geometry/geometry_rectangle.h"
#include "data_table.h"
#include "data_id.h"
#include "trace.h"
#include "tslog.h"
#include <stdint.h>
#include <stdbool.h>

void gui_sketch_object_creator_init ( gui_sketch_object_creator_t *this_,
                                     ctrl_controller_t *controller,
                                     data_database_reader_t *db_reader )
{
    TRACE_BEGIN();

    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    data_rules_init ( &((*this_).data_rules) );

    TRACE_END();
}

void gui_sketch_object_creator_destroy ( gui_sketch_object_creator_t *this_ )
{
    TRACE_BEGIN();

    data_rules_destroy ( &((*this_).data_rules) );
    (*this_).db_reader = NULL;
    (*this_).controller = NULL;

    TRACE_END();
}

ctrl_error_t gui_sketch_object_creator_create_classifier ( gui_sketch_object_creator_t *this_,
                                                           int64_t diagram_id,
                                                           int32_t x_order,
                                                           int32_t y_order,
                                                           int64_t *out_diagramelement_id,
                                                           int64_t *out_classifier_id )
{
    TRACE_BEGIN();
    assert ( NULL != out_classifier_id );
    assert ( NULL != out_diagramelement_id );

    ctrl_error_t c_result;

    ctrl_classifier_controller_t *classifier_control;
    classifier_control = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

    static char *(NAMES[8]) = {"off-","on-","debug-","persistence-","communication-","bootloader-","driver-","application-"};
    static uint8_t my_counter = 0;
    char newname_buf[24];
    utf8stringbuf_t full_new_name = UTF8STRINGBUF( newname_buf );
    utf8stringbuf_copy_str( full_new_name, NAMES[(x_order+y_order)&0x07] );
    utf8stringbuf_append_int( full_new_name, my_counter++ );

    int64_t new_classifier_id;
    int64_t new_diagele_id;
    c_result = ctrl_classifier_controller_create_classifier_in_diagram ( classifier_control,
                                                                         diagram_id,
                                                                         DATA_CLASSIFIER_TYPE_BLOCK,
                                                                         utf8stringbuf_get_string( full_new_name ),
                                                                         x_order,
                                                                         y_order,
                                                                         out_diagramelement_id,
                                                                         out_classifier_id
                                                                       );

    TRACE_END_ERR( c_result );
    return c_result;
}


/*
Copyright 2017-2017 Andreas Warnke

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

