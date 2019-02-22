/* File: draw_geometry.c; Copyright and License: see below */

#include "draw/draw_geometry.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*! where to place the control points of a bezier curve to get a good approximation for a 90 degree curve */
const static double BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE = 0.552284749831;

void draw_geometry_init( draw_geometry_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void draw_geometry_destroy( draw_geometry_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void draw_geometry_get_shape_border_dimensions( const draw_geometry_t *this_,
                                                data_classifier_type_t classifier_type,
                                                const pencil_size_t *pencil_size,
                                                double *out_top_border,
                                                double *out_left_border,
                                                double *out_bottom_border,
                                                double *out_right_border )
{
    assert( NULL != pencil_size );
    assert( NULL != out_top_border );
    assert( NULL != out_left_border );
    assert( NULL != out_bottom_border );
    assert( NULL != out_right_border );

    double gap = pencil_size_get_standard_object_border( pencil_size );
    double double_gap = 2.0 * gap;  /* a line has the gap disance on both sides to the next object */

    switch ( classifier_type )
    {
        case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
        {
            /* within a use case, space is limited: */
            double v_offset = pencil_size_get_standard_font_size( pencil_size );
            double h_offset = 1.5 * pencil_size_get_standard_font_size( pencil_size );

            *out_top_border = double_gap + v_offset;
            *out_left_border = double_gap + h_offset;
            *out_bottom_border = double_gap + v_offset;
            *out_right_border = double_gap + h_offset;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_NODE:
        {
            /* the 3d border of a node shrinks the space */
            double offset_3d = double_gap;

            *out_top_border = double_gap + offset_3d;
            *out_left_border = double_gap;
            *out_bottom_border = double_gap;
            *out_right_border = double_gap + offset_3d;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTOR:
        case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
        case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
        {
            /* the symbol icon height is part of the shape border  */
            double symbol_icon_height = pencil_size_get_classifier_symbol_height( pencil_size );

            *out_top_border = double_gap + symbol_icon_height;
            *out_left_border = gap;
            *out_bottom_border = gap;
            *out_right_border = gap;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
        {
            double top_ornament_height = pencil_size_get_standard_font_size( pencil_size );

            *out_top_border = double_gap + top_ornament_height;
            *out_left_border = double_gap;
            *out_bottom_border = double_gap;
            *out_right_border = double_gap;
        }
        break;

        case DATA_CLASSIFIER_TYPE_BLOCK:
        case DATA_CLASSIFIER_TYPE_FEATURE:
        case DATA_CLASSIFIER_TYPE_REQUIREMENT:
        case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
        case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
        case DATA_CLASSIFIER_TYPE_UML_STATE:
        case DATA_CLASSIFIER_TYPE_UML_COMPONENT:
        case DATA_CLASSIFIER_TYPE_UML_PART:
        case DATA_CLASSIFIER_TYPE_UML_INTERFACE:
        case DATA_CLASSIFIER_TYPE_UML_CLASS:
        case DATA_CLASSIFIER_TYPE_UML_OBJECT:
        case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
        case DATA_CLASSIFIER_TYPE_UML_COMMENT:
        case DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY:
        case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
        case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT :
        case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
        {
            /* standard size */
            *out_top_border = double_gap;
            *out_left_border = double_gap;
            *out_bottom_border = double_gap;
            *out_right_border = double_gap;
        }
        break;

        default:
        {
            TSLOG_ERROR("unknown data_classifier_type_t in draw_geometry_get_shape_border_dimensions()");
            *out_top_border = 0.0;
            *out_left_border = 0.0;
            *out_bottom_border = 0.0;
            *out_right_border = 0.0;
        }
        break;
    }
}


/*
Copyright 2016-2019 Andreas Warnke
    http://www.apache.org/licenses/LICENSE-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at


Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
