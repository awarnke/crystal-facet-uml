/* File: gui_sketch_area.h; Copyright and License: see below */

#ifndef GUI_SKETCH_AREA_H
#define GUI_SKETCH_AREA_H

/* public file for the doxygen documentation: */
/*! \file */

#include "data_database.h"
#include "gui_sketch_tools.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief data which is linked to the sketch area widget
 */
struct gui_sketch_area_struct {
    data_database_t database;
    bool mark_active;
    int32_t mark_start_x;
    int32_t mark_start_y;
    int32_t mark_end_x;
    int32_t mark_end_y;
    gui_sketch_tools_t *tools;
};

typedef struct gui_sketch_area_struct gui_sketch_area_t;

/*!
 *  \brief initializes the gui_sketch_area_t struct
 */
void gui_sketch_area_init ( gui_sketch_area_t *this_, gui_sketch_tools_t *tools );

/*!
 *  \brief destroys the gui_sketch_area_t struct
 */
void gui_sketch_area_destroy ( gui_sketch_area_t *this_ );

/*!
 *  \brief callback that redraws the sketch_area widget
 */
gboolean gui_sketch_area_draw_callback ( GtkWidget *widget, cairo_t *cr, gpointer data );

/*!
 *  \brief callback that informs that the mouse cursor left the sketch_area widget 
 */
gboolean gui_sketch_area_leave_notify_callback( GtkWidget* widget, GdkEventCrossing* evt, gpointer data );

/*!
 *  \brief callback that informs that the mouse cursor moved on the sketch_area widget 
 */
gboolean gui_sketch_area_mouse_motion_callback( GtkWidget* widget, GdkEventMotion* evt, gpointer data );

/*!
 *  \brief callback that informs that the mouse button was pressed on the sketch_area widget 
 */
gboolean gui_sketch_area_button_press_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data );

/*!
 *  \brief callback that informs that the mouse button was released on the sketch_area widget 
 */
gboolean gui_sketch_area_button_release_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data );

#endif  /* GUI_SKETCH_AREA_H */


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
