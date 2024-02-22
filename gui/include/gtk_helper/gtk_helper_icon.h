/* File: gtk_helper_icon.h; Copyright and License: see below */

#ifndef GTK_HELPER_ICON_H
#define GTK_HELPER_ICON_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief provides types and functions that were still there in GTK 4.0.0
 *         but deprecated later on and so had to be replaced by more modern concepts
 */

#include "u8/u8_error.h"
#include <gtk/gtk.h>

/*!
 *  \brief draws the icon
 *
 *  \param icon the icon to draw
 *  \param left left coordinate
 *  \param top top coordinate
 *  \param cr cairo drawing context
 *  \return U8_ERROR_NONE in case of success,
 *          U8_ERROR_EDGE_CASE_PARAM is the icon could not be drawn (overlap to surface boundary)
 */
static inline u8_error_t gtk_helper_icon_draw_texture( GdkTexture *icon, double left, double top, cairo_t *cr );

#include "gtk_helper/gtk_helper_icon.inl"

#endif  /* GTK_HELPER_ICON_H */


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
