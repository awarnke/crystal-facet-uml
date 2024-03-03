/* File: gui_sketch_texture.h; Copyright and License: see below */

#ifndef GUI_SKETCH_TEXTURE_H
#define GUI_SKETCH_TEXTURE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief provides types and functions that were still there in GTK 4.0.0
 *         but deprecated later on and so had to be replaced by more modern concepts
 */

#include "u8/u8_error.h"
#include <gtk/gtk.h>

/*!
 *  \brief constants of gui_sketch_texture_t
 */
enum gui_sketch_texture_enum {
    GUI_SKETCH_TEXTURE_MAX_WIDTH = 80,  /*!< maximum width of an icon to be drawn */
    GUI_SKETCH_TEXTURE_MAX_HEIGHT = 80,  /*!< maximum height of an icon to be drawn */
    GUI_SKETCH_TEXTURE_BYTES_PER_PIXEL = 4,  /*!< number of bits per pixel */
};

/*!
 *  \brief attributes of the gui_sketch_texture_t
 */
struct gui_sketch_texture_struct {
    cairo_surface_t *surface;  /*!< cairo drawing surface wrapping a data buffer */
    unsigned int memory_fence_1;  /* a memory fence to detect if gdk or cairo have been used in an unforseen way */
    unsigned char buf[ GUI_SKETCH_TEXTURE_MAX_WIDTH * GUI_SKETCH_TEXTURE_MAX_HEIGHT * GUI_SKETCH_TEXTURE_BYTES_PER_PIXEL ];
                       /*!< buffer to download a texture into a cairo drawing surface */
    unsigned int memory_fence_2;  /* a memory fence to detect if gdk or cairo have been used in an unforseen way */
};

typedef struct gui_sketch_texture_struct gui_sketch_texture_t;

/*!
 *  \brief initializes the gui_sketch_texture
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_texture_init( gui_sketch_texture_t *this_ );

/*!
 *  \brief destroys the gui_sketch_texture
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_texture_destroy( gui_sketch_texture_t *this_ );

/*!
 *  \brief draws the icon
 *
 *  \param this_ pointer to own object attributes
 *  \param icon the icon to draw
 *  \param left left coordinate
 *  \param top top coordinate
 *  \param cr cairo drawing context
 *  \return U8_ERROR_NONE in case of success,
 *          U8_ERROR_EDGE_CASE_PARAM if the icon could not be drawn (overlap to surface boundary)
 */
static inline u8_error_t gui_sketch_texture_draw( gui_sketch_texture_t *this_,
                                                  GdkTexture *icon,
                                                  double left,
                                                  double top,
                                                  cairo_t *cr
                                                );

#include "sketch/gui_sketch_texture.inl"

#endif  /* GUI_SKETCH_TEXTURE_H */


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
