/* File: gui_sketch_background.h; Copyright and License: see below */

#ifndef GUI_SKETCH_BACKGROUND_H
#define GUI_SKETCH_BACKGROUND_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Draws background graphics below diagrams
 */

#include "gui_resources.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the sketch background
 */
struct gui_sketch_background_struct {
    gui_resources_t *res;  /*!< pointer to external resources */
};

typedef struct gui_sketch_background_struct gui_sketch_background_t;

/*!
 *  \brief initializes the sketch background
 *
 *  \param this_ pointer to own object attributes
 *  \param res pointer to a resource provider
 */
void gui_sketch_background_init( gui_sketch_background_t *this_, gui_resources_t *res );

/*!
 *  \brief destroys the sketch background
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_background_destroy( gui_sketch_background_t *this_ );

/*!
 *  \brief draws background graphics to the cairo context, including the introduction
 *
 *  \param this_ pointer to own object attributes
 *  \param x coordinate of the drawing area
 *  \param y coordinate of the drawing area
 *  \param width size of the drawing area
 *  \param height size of the drawing area
 *  \param cr cairo drawing context
 */
void gui_sketch_background_draw_introduction( gui_sketch_background_t *this_,
                                              int x,
                                              int y,
                                              unsigned int width,
                                              unsigned int height,
                                              cairo_t *cr
                                            );

/*!
 *  \brief draws background graphics to the cairo context for navigation mode
 *
 *  \param this_ pointer to own object attributes
 *  \param x coordinate of the drawing area
 *  \param y coordinate of the drawing area
 *  \param width size of the drawing area
 *  \param height size of the drawing area
 *  \param cr cairo drawing context
 */
void gui_sketch_background_draw_navigation( gui_sketch_background_t *this_,
                                            int x,
                                            int y,
                                            unsigned int width,
                                            unsigned int height,
                                            cairo_t *cr
                                          );

/*!
 *  \brief draws background graphics to the cairo context for edit mode
 *
 *  \param this_ pointer to own object attributes
 *  \param x coordinate of the drawing area
 *  \param y coordinate of the drawing area
 *  \param width size of the drawing area
 *  \param height size of the drawing area
 *  \param cr cairo drawing context
 */
void gui_sketch_background_draw_edit( gui_sketch_background_t *this_,
                                      int x,
                                      int y,
                                      unsigned int width,
                                      unsigned int height,
                                      cairo_t *cr
                                    );

#endif  /* GUI_SKETCH_BACKGROUND_H */


/*
Copyright 2017-2017 Andreas Warnke

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
