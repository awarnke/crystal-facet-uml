/* File: gui_button.h; Copyright and License: see below */

#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief A structure of icon, label, box and button.
 *
 *  These four gtk elements are needed to display a button and react on it.
 */

#include <gtk/gtk.h>
#include <gdk/gdk.h>

/*!
 *  \brief attributes of the button
 *
 *  The notation of the struct name is required by glib
 */
struct gui_button_struct {
    GtkImage * icon;  /*!<  the icon on the button */
    GtkLabel * label;  /*!<  the label on the button */
    GtkBox * box;  /*!<  the box that encompasses icon and label */
    GtkButton * button;  /*!<  the button that contains the box as child */
};

typedef struct gui_button_struct gui_button_t;

/*!
 *  \brief initializes the gui_button_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param icon_source a gdk paintable object that allows to render an icon
 *  \param label_text the text to display on the label
 *  \param tooltip_text the text to display on the tooltip
 */
static inline void gui_button_init ( gui_button_t *this_,
                                     GdkPaintable * icon_source,
                                     const char *label_text,
                                     const char *tooltip_text
                                   );

/*!
 *  \brief destroys the gui_button_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_button_destroy ( gui_button_t *this_ );

/*!
 *  \brief gets the icon image
 *
 *  This function can be used as type GEqualFunc
 *
 *  \param this_ pointer to own object attributes
 *  \return icon image
 */
static inline GtkImage * gui_button_get_icon_ptr ( const gui_button_t *this_ );

/*!
 *  \brief gets the label
 *
 *  \param this_ pointer to own object attributes
 *  \return label
 */
static inline GtkLabel * gui_button_get_label_ptr ( const gui_button_t *this_ );

/*!
 *  \brief gets the button
 *
 *  \param this_ pointer to own object attributes
 *  \return button
 */
static inline GtkButton * gui_button_get_button_ptr ( const gui_button_t *this_ );

/*!
 *  \brief gets the button, typecast as widget
 *
 *  \param this_ pointer to own object attributes
 *  \return button
 */
static inline GtkWidget * gui_button_get_widget_ptr ( const gui_button_t *this_ );

#include "gui_button.inl"

#endif  /* GUI_BUTTON_H */


/*
Copyright 2024-2024 Andreas Warnke

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
