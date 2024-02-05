/* File: gui_attributes_editor_types.c; Copyright and License: see below */

#include "gui_attributes_editor_types.h"
#include "gtk_helper/gtk_helper_tree_model.h"
#include "u8/u8_trace.h"
#include "data_table.h"
#include "data_id.h"
#include <gtk/gtk.h>

void gui_attributes_editor_types_init ( gui_attributes_editor_types_t *this_, gui_resources_t *resources )
{
    U8_TRACE_BEGIN();
    assert( NULL != resources );

    (*this_).resources = resources;
    gui_type_resource_list_init ( &((*this_).selector), resources );

    /* init undef */

    (*this_).no_types = gtk_list_store_new( 3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF );

    {
        const GdkTexture *icon_undef_type = gui_resources_get_type_undef( (*this_).resources );

        GtkTreeIter iter;
        gtk_list_store_append( (*this_).no_types, &iter);
        gtk_list_store_set ( (*this_).no_types, &iter, 0, 0x0, 1, "-- n/a --", 2, icon_undef_type, -1 );
    }

    const gui_type_resource_t ((*types)[]);
    unsigned int types_len;

    /* init diagrams */

    (*this_).diagram_types = gtk_list_store_new( 3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF );

    gui_type_resource_list_get_all_diagram_types ( &((*this_).selector), &types, &types_len );
    for ( unsigned int idx = 0; idx < types_len; idx ++ )
    {
        const gui_type_resource_t *res = &((*types)[idx]);
        int type = data_type_get_type_as_int( gui_type_resource_get_type_id( res ) );
        const char *name = gui_type_resource_get_name( res );
        const GdkTexture *icon = gui_type_resource_get_icon( res );

        GtkTreeIter iter_d;
        gtk_list_store_append( (*this_).diagram_types, &iter_d);
        gtk_list_store_set ( (*this_).diagram_types, &iter_d, 0, type, 1, name, 2, icon, -1 );
    }

    /* init classifiers */

    (*this_).classifier_types = gtk_list_store_new( 3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF );

    gui_type_resource_list_get_all_classifier_types ( &((*this_).selector), &types, &types_len );
    for ( unsigned int idx = 0; idx < types_len; idx ++ )
    {
        const gui_type_resource_t *res = &((*types)[idx]);
        int type = data_type_get_type_as_int( gui_type_resource_get_type_id( res ) );
        const char *name = gui_type_resource_get_name( res );
        const GdkTexture *icon = gui_type_resource_get_icon( res );

        GtkTreeIter iter_c;
        gtk_list_store_append( (*this_).classifier_types, &iter_c);
        gtk_list_store_set ( (*this_).classifier_types, &iter_c, 0, type, 1, name, 2, icon, -1 );
    }

    /* init features (2 lists, the lifeline list should not be needed) */

    (*this_).feature_types = gtk_list_store_new( 3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF );
    (*this_).feature_lifeline_type = gtk_list_store_new( 3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF );

    gui_type_resource_list_get_all_feature_types ( &((*this_).selector), &types, &types_len );
    for ( unsigned int idx = 0; idx < types_len; idx ++ )
    {
        const gui_type_resource_t *res = &((*types)[idx]);
        int type = data_type_get_type_as_int( gui_type_resource_get_type_id( res ) );
        const char *name = gui_type_resource_get_name( res );
        const GdkTexture *icon = gui_type_resource_get_icon( res );

        if ( type == DATA_FEATURE_TYPE_LIFELINE )
        {
            GtkTreeIter iter_fl;
            gtk_list_store_append( (*this_).feature_lifeline_type, &iter_fl);
            gtk_list_store_set ( (*this_).feature_lifeline_type, &iter_fl, 0, type, 1, name, 2, icon, -1 );
        }
        else
        {
            GtkTreeIter iter_f;
            gtk_list_store_append( (*this_).feature_types, &iter_f);
            gtk_list_store_set ( (*this_).feature_types, &iter_f, 0, type, 1, name, 2, icon, -1 );
        }
    }

    /* init relationships */

    (*this_).relationship_types = gtk_list_store_new( 3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF );

    gui_type_resource_list_get_all_relationship_types ( &((*this_).selector), &types, &types_len );
    for ( unsigned int idx = 0; idx < types_len; idx ++ )
    {
        const gui_type_resource_t *res = &((*types)[idx]);
        int type = data_type_get_type_as_int( gui_type_resource_get_type_id( res ) );
        const char *name = gui_type_resource_get_name( res );
        const GdkTexture *icon = gui_type_resource_get_icon( res );

        GtkTreeIter iter_r;
        gtk_list_store_append( (*this_).relationship_types, &iter_r);
        gtk_list_store_set ( (*this_).relationship_types, &iter_r, 0, type, 1, name, 2, icon, -1 );
    }

    U8_TRACE_END();
}

void gui_attributes_editor_types_destroy ( gui_attributes_editor_types_t *this_ )
{
    U8_TRACE_BEGIN();

    gui_type_resource_list_destroy ( &((*this_).selector) );

    g_object_unref((*this_).no_types);
    (*this_).no_types = NULL;

    g_object_unref((*this_).diagram_types);
    (*this_).diagram_types = NULL;

    g_object_unref((*this_).classifier_types);
    (*this_).classifier_types = NULL;

    g_object_unref((*this_).feature_types);
    (*this_).feature_types = NULL;

    g_object_unref((*this_).feature_lifeline_type);
    (*this_).feature_lifeline_type = NULL;

    g_object_unref((*this_).relationship_types);
    (*this_).relationship_types = NULL;

    (*this_).resources = NULL;

    U8_TRACE_END();
}


/*
Copyright 2016-2024 Andreas Warnke

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
