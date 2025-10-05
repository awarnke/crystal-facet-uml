/* File: gui_sketch_background.h; Copyright and License: see below */

#ifndef GUI_SKETCH_BACKGROUND_H
#define GUI_SKETCH_BACKGROUND_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Draws background graphics below diagrams
 */

#include "gui_resources.h"
#include "sketch/gui_sketch_texture.h"
#include "shape/shape_int_rectangle.h"
#include "gui_gtk.h"
#include <stdbool.h>

/*!
 *  \brief attributes of the sketch background
 */
struct gui_sketch_background_struct {
    shape_int_rectangle_t card_bounds;  /*!< bounding box of the background of the cards area */
    shape_int_rectangle_t label_bounds;  /*!< bounding box of the background of the nav_tree or the search result list */
    gui_resources_t *resources;  /*!< pointer to external resources */
    gui_sketch_texture_t *texture_downloader;  /*!< pointer to external gui_sketch_texture_t */
};

typedef struct gui_sketch_background_struct gui_sketch_background_t;

/*!
 *  \brief initializes the sketch background
 *
 *  \param this_ pointer to own object attributes
 *  \param resources pointer to a resource provider
 *  \param texture_downloader pointer to a gui_sketch_texture_t that can draw a texture onto a cairo drawing context
 */
void gui_sketch_background_init( gui_sketch_background_t *this_,
                                 gui_resources_t *resources,
                                 gui_sketch_texture_t *texture_downloader
                               );

/*!
 *  \brief destroys the sketch background
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_background_destroy( gui_sketch_background_t *this_ );

/*!
 *  \brief gets the bounds rectangle of the cards area
 *
 *  \param this_ pointer to own object attributes
 *  \return returns the bounding box of this sketch areas card area
 */
static inline const shape_int_rectangle_t *gui_sketch_background_get_card_bounds( gui_sketch_background_t *this_ );

/*!
 *  \brief gets the bounds rectangle of the list labels
 *
 *  \param this_ pointer to own object attributes
 *  \return returns the bounding box of this sketch areas list label area
 */
static inline const shape_int_rectangle_t *gui_sketch_background_get_label_bounds( gui_sketch_background_t *this_ );

/*!
 *  \brief sets the bounds rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param card_bounds bounding box of this sketch areas card area
 *  \param label_bounds bounding box of this sketch areas list label area
 */
static inline void gui_sketch_background_set_bounds( gui_sketch_background_t *this_,
                                                     const shape_int_rectangle_t *card_bounds,
                                                     const shape_int_rectangle_t *label_bounds
                                                   );

/*!
 *  \brief draws background graphics to the cairo context, including the introduction
 *
 *  \param this_ pointer to own object attributes
 *  \param cr cairo drawing context
 */
void gui_sketch_background_draw_introduction( gui_sketch_background_t *this_,
                                              cairo_t *cr
                                            );

/*!
 *  \brief draws background graphics to the cairo context for navigation mode
 *
 *  \param this_ pointer to own object attributes
 *  \param tree_depth depth of the current diagram in the tree. 0 represents the root node or no valid diagram.
 *  \param num_children number of children at the current diagram.
 *  \param cr cairo drawing context
 */
void gui_sketch_background_draw_navigation( gui_sketch_background_t *this_,
                                            unsigned int tree_depth,
                                            unsigned int num_children,
                                            cairo_t *cr
                                          );

/*!
 *  \brief draws background graphics to the cairo context for search mode
 *
 *  \param this_ pointer to own object attributes
 *  \param cr cairo drawing context
 */
void gui_sketch_background_draw_search( gui_sketch_background_t *this_, cairo_t *cr );

/*!
 *  \brief draws background graphics to the cairo context for edit mode
 *
 *  \param this_ pointer to own object attributes
 *  \param cr cairo drawing context
 */
void gui_sketch_background_draw_edit( gui_sketch_background_t *this_, cairo_t *cr );

/*!
 *  \brief draws background graphics to the cairo context for create mode
 *
 *  \param this_ pointer to own object attributes
 *  \param cr cairo drawing context
 */
void gui_sketch_background_draw_create( gui_sketch_background_t *this_, cairo_t *cr );

/*!
 *  \brief draws a connector line from the object box to the diagram card box
 *
 *  \param this_ pointer to own object attributes
 *  \param label_box bounding box of icon and label of the object
 *  \param card_box bounding box of the diagram card
 *  \param cr cairo drawing context
 */
void gui_sketch_background_draw_appears_link( gui_sketch_background_t *this_,
                                              const shape_int_rectangle_t *label_box,
                                              const shape_int_rectangle_t *card_box,
                                              cairo_t *cr
                                            );

/*!
 *  \brief draws a quick-introduction to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param cr cairo drawing context
 */
void gui_sketch_background_private_draw_quick_introduction( gui_sketch_background_t *this_, cairo_t *cr );

/*!
 *  \brief draws an icon and two lines of text
 *
 *  \param this_ pointer to own object attributes
 *  \param icon_1 a pointer to an icon
 *  \param text_1 a pointer to a text message, line 1
 *  \param text_2 a pointer to a text message, line 2
 *  \param x coordinate of the output location
 *  \param y coordinate of the output location
 *  \param cr cairo drawing context
 */
void gui_sketch_background_private_draw_icon_and_message( gui_sketch_background_t *this_,
                                                          GdkTexture *icon_1,
                                                          const char *text_1,
                                                          const char *text_2,
                                                          int x,
                                                          int y,
                                                          cairo_t *cr
                                                        );

#include "gui_sketch_background.inl"

#endif  /* GUI_SKETCH_BACKGROUND_H */


/*
Copyright 2017-2025 Andreas Warnke

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
