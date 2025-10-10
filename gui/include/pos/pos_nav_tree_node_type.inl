/* File: pos_nav_tree_node_type.inl; Copyright and License: see below */

#include <assert.h>

static inline GdkTexture * pos_nav_tree_node_type_get_icon( pos_nav_tree_node_type_t this_,
                                                            bool highlight,
                                                            const gui_resources_t *resources )
{
    assert( resources != NULL );
    GdkTexture * result;
    switch( this_ )
    {
        case POS_NAV_TREE_NODE_TYPE_ANCESTOR:
        {
            result = gui_resources_get_navigate_breadcrumb_folder( resources );
        }
        break;

        case POS_NAV_TREE_NODE_TYPE_OPEN:
        {
            result = gui_resources_get_navigate_open_folder( resources );
        }
        break;

        case POS_NAV_TREE_NODE_TYPE_CLOSED:
        {
            result = gui_resources_get_navigate_closed_folder( resources );
        }
        break;

        case POS_NAV_TREE_NODE_TYPE_NEW_ROOT:
        {
            /* same icon as for POS_NAV_TREE_NODE_TYPE_NEW_SIBLING */
            result
                = highlight
                ? gui_resources_get_navigate_create_sibling( resources )
                : gui_resources_get_navigate_create_sibling_0( resources );
        }
        break;

        case POS_NAV_TREE_NODE_TYPE_NEW_SIBLING:
        {
            result
                = highlight
                ? gui_resources_get_navigate_create_sibling( resources )
                : gui_resources_get_navigate_create_sibling_0( resources );
        }
        break;

        case POS_NAV_TREE_NODE_TYPE_NEW_CHILD:
        {
            result
                = highlight
                ? gui_resources_get_navigate_create_child( resources )
                : gui_resources_get_navigate_create_child_0( resources );
        }
        break;

        default:
        {
            /* e.g. POS_NAV_TREE_NODE_TYPE_VOID */
            result = gui_resources_get_type_undef( resources );
        }
    }
    return result;
}


/*
Copyright 2021-2025 Andreas Warnke

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
