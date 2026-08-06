/* File: geometry_compartment_type.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <assert.h>

static const int32_t GEOMETRY_COMPARTMENT_TYPE_INT32_MIN_HALF = (INT32_MIN/2);
static const int32_t GEOMETRY_COMPARTMENT_TYPE_INT32_MAX_HALF = ((INT32_MAX/2)+1);  /* round to ceiling */

static inline geometry_compartment_type_t geometry_compartment_type_new ( data_feature_type_t feature_type,
                                                                          int32_t list_order )
{
    geometry_compartment_type_t result = GEOMETRY_COMPARTMENT_TYPE_VOID;

    switch ( feature_type )
    {
        case DATA_FEATURE_TYPE_PROPERTY:
        {
            result = GEOMETRY_COMPARTMENT_TYPE_PROPERTIES;
        }
        break;

        case DATA_FEATURE_TYPE_OPERATION:
        {
            result = GEOMETRY_COMPARTMENT_TYPE_OPERATIONS;
        }
        break;

        case DATA_FEATURE_TYPE_TAGGED_VALUE:
        {
            result = GEOMETRY_COMPARTMENT_TYPE_TAGGED_VALUES;
        }
        break;

        case DATA_FEATURE_TYPE_PROVIDED_INTERFACE:  /* or */
        case DATA_FEATURE_TYPE_REQUIRED_INTERFACE:
        {
            if ( list_order < 0 )
            {
                if ( list_order < GEOMETRY_COMPARTMENT_TYPE_INT32_MIN_HALF )  /* SHOW ON RIGHT BORDER */
                {
                    result = GEOMETRY_COMPARTMENT_TYPE_IF_ON_RIGHT;
                }
                else  /* SHOW ON TOP BORDER */
                {
                    result = GEOMETRY_COMPARTMENT_TYPE_IF_ON_TOP;
                }
            }
            else
            {
                if ( list_order < GEOMETRY_COMPARTMENT_TYPE_INT32_MAX_HALF )  /* SHOW ON LEFT BORDER */
                {
                    result = GEOMETRY_COMPARTMENT_TYPE_IF_ON_LEFT;
                }
                else  /* SHOW ON BOTTOM BORDER */
                {
                    result = GEOMETRY_COMPARTMENT_TYPE_IF_ON_BOTTOM;
                }
            }
        }
        break;

        case DATA_FEATURE_TYPE_PORT:  /* or */
        case DATA_FEATURE_TYPE_IN_PORT_PIN:  /* or */
        case DATA_FEATURE_TYPE_OUT_PORT_PIN:  /* or */
        case DATA_FEATURE_TYPE_ENTRY:  /* or */
        case DATA_FEATURE_TYPE_EXIT:
        {
            if ( list_order < 0 )
            {
                if ( list_order < GEOMETRY_COMPARTMENT_TYPE_INT32_MIN_HALF )  /* SHOW ON RIGHT BORDER */
                {
                    result = GEOMETRY_COMPARTMENT_TYPE_PORT_ON_RIGHT;
                }
                else  /* SHOW ON TOP BORDER */
                {
                    result = GEOMETRY_COMPARTMENT_TYPE_PORT_ON_TOP;
                }
            }
            else
            {
                if ( list_order < GEOMETRY_COMPARTMENT_TYPE_INT32_MAX_HALF )  /* SHOW ON LEFT BORDER */
                {
                    result = GEOMETRY_COMPARTMENT_TYPE_PORT_ON_LEFT;
                }
                else  /* SHOW ON BOTTOM BORDER */
                {
                    result = GEOMETRY_COMPARTMENT_TYPE_PORT_ON_BOTTOM;
                }
            }
        }
        break;

        default:
        {
            result = GEOMETRY_COMPARTMENT_TYPE_VOID;
        }
        break;
    }

    return result;
}


/*
 * Copyright 2026-2026 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
