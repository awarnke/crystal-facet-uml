/* File: gui_textedit.c; Copyright and License: see below */

#include "gui_textedit.h"
#include "trace.h"
#include <gtk/gtk.h>

void gui_textedit_init ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();
    GtkTreeIter iter;

    (*this_).diagram_types = gtk_list_store_new( 1, G_TYPE_STRING );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "LIST", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "BLOCK_DIAGRAM", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "REQUIREMENT_DIAGRAM", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_USE_CASE_DIAGRAM", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_ACTIVITY_DIAGRAM", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_STATE_MACHINE_DIAGRAM", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_SEQUENCE_DIAGRAM", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_COMMUNICATION_DIAGRAM", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_INTERACTION_OVERVIEW_DIAGRAM", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_TIMING_DIAGRAM", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_DEPLOYMENT_DIAGRAM", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_COMPONENT_DIAGRAM", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_COMPOSITE_STRUCTURE_DIAGRAM", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_PACKAGE_DIAGRAM", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_CLASS_DIAGRAM", -1 );
    gtk_list_store_append( (*this_).diagram_types, &iter);
    gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_PROFILE_DIAGRAM", -1 );

    TRACE_END();
}

void gui_textedit_destroy ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
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
