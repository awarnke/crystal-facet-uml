/* File: draw_relationship_label.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline char const * draw_relationship_label_private_stereotype_from_type( const draw_relationship_label_t *this_,
                                                                                 data_relationship_type_t rel_type )
{
    char const * result = "";
    switch ( rel_type )
    {
        case DATA_RELATIONSHIP_TYPE_UML_EXTEND:
        {
            result = "extends";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_INCLUDE:
        {
            result = "includes";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_DEPLOY:
        {
            result = "deploy";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_MANIFEST:
        {
            result = "manifest";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_REFINE:
        {
            result = "refine";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_TRACE:
        {
            result = "trace";
        }
        break;

        default:
        {
            /* other types do not show a stereotype label */
            result = "";
        }
        break;
    }
    return result;
}


/*
Copyright 2019-2024 Andreas Warnke

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
