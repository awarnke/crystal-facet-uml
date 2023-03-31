/* File: draw_stereotype_image.h; Copyright and License: see below */

#ifndef DRAW_STEREOTYPE_IMAGE_H
#define DRAW_STEREOTYPE_IMAGE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders a stereotype image into a cairo drawing context
 */

#include "pencil_size.h"
#include "geometry/geometry_h_align.h"
#include "geometry/geometry_v_align.h"
#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_dimensions.h"
#include "data_classifier_type.h"
#include "set/data_profile_part.h"
#include "utf8stringbuf/utf8stringviewtokenizer.h"
#include "u8/u8_error.h"
#include "u8/u8_error_info.h"
#include <cairo.h>
#include <stdint.h>

/*! \brief aspect ratio of the stereotype image */
extern const double DRAW_STEREOTYPE_IMAGE_WIDTH_TO_HEIGHT;

/*!
 *  \brief attributes of the draw stereotype image functions
 */
struct draw_stereotype_image_struct {
    int dummy;  /*!< This object is a collection of stateless drawing functions */
};

typedef struct draw_stereotype_image_struct draw_stereotype_image_t;

/*!
 *  \brief initializes the draw_stereotype_image_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void draw_stereotype_image_init( draw_stereotype_image_t *this_ );

/*!
 *  \brief destroys the draw_stereotype_image_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void draw_stereotype_image_destroy( draw_stereotype_image_t *this_ );

/*!
 *  \brief determines the dimensions of the stereotype image.
 *
 *  Type icons are e.g. artifact-icon or component-icon.
 *
 *  \param this_ pointer to own object attributes
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \return dimension of the stereotype image.
 */
static inline geometry_dimensions_t draw_stereotype_image_get_dimensions ( const draw_stereotype_image_t *this_,
                                                                           const pencil_size_t *pencil_size
                                                                         );

/*!
 *  \brief calculates the bounding rectangle for the stereotype image
 *
 *  \param this_ pointer to own object attributes
 *  \param x x location where to draw the stereotype image
 *  \param y y location where to draw the stereotype image
 *  \param h_align alignment of the stereotype image versus the given x coordinate
 *  \param v_align alignment of the stereotype image versus the given y coordinate
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \return bounding box rectangle of the stereotype image
 */
static inline geometry_rectangle_t draw_stereotype_image_get_bounds ( const draw_stereotype_image_t *this_,
                                                                      double x,
                                                                      double y,
                                                                      geometry_h_align_t h_align,
                                                                      geometry_v_align_t v_align,
                                                                      const pencil_size_t *pencil_size
                                                                    );

/*!
 *  \brief draws the stereotype image into the bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param stereotype name of the stereotype(s) to be drawn
 *  \param profile pointer to the profile-part that provides the stereotypes of the elements to be drawn
 *  \param out_err_info pointer to an error_info_t data struct that may provide an error description when returning
 *  \param bounds bounding rectangle of the stereotype image
 *  \param cr a cairo drawing context
 *  \return U8_ERROR_NONE if one (or more) images were drawn,
 *          U8_ERROR_NOT_FOUND if no image was found,
 *          U8_ERROR_PARSER_STRUCTURE if expected tokens in the input image were missing or in wrong order
 */
u8_error_t draw_stereotype_image_draw ( const draw_stereotype_image_t *this_,
                                        const char *stereotype,
                                        const data_profile_part_t *profile,
                                        u8_error_info_t *out_err_info,
                                        const geometry_rectangle_t *bounds,
                                        cairo_t *cr
                                      );

/*!
 *  \brief draws the stereotype image into the bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param draw false if only the view_rect shall be determined, true if the drawing_directives shall be drawn
 *  \param drawing_directives sequence of drawing directives
 *  \param io_view_rect bounding rectangle of the drawing_directives;
 *                      in all cases this is provided as output, in case of draw==true this is needed as imput.
 *  \param out_err_info pointer to an error_info_t data struct that may provide an error description when returning
 *  \param target_bounds bounding rectangle of the drawing directives
 *  \param cr a cairo drawing context
 *  \return U8_ERROR_NONE if the image was drawn,
 *          U8_ERROR_NOT_FOUND if no image was found,
 *          U8_ERROR_PARSER_STRUCTURE if expected tokens in the input image were missing or in wrong order
 */
u8_error_t draw_stereotype_image_private_parse_svg_xml ( const draw_stereotype_image_t *this_,
                                                         bool draw,
                                                         const char *drawing_directives,
                                                         geometry_rectangle_t *io_view_rect,
                                                         u8_error_info_t *out_err_info,
                                                         const geometry_rectangle_t *target_bounds,
                                                         cairo_t *cr
                                                       );

/*!
 *  \brief draws the stereotype image into the bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param draw false if only the view_rect shall be determined, true if the drawing_directives shall be drawn
 *  \param tok_iterator token iterator of drawing directives. The first token shall be the first draw command.
 *                      double quotes or end-of-stream end the processing, the double quotes token is already consumed.
 *  \param io_view_rect bounding rectangle of the drawing_directives;
 *                      in all cases this is provided as output, in case of draw==true this is needed as imput.
 *  \param out_err_info pointer to an error_info_t data struct that may provide an error description when returning
 *  \param target_bounds bounding rectangle of the drawing directives
 *  \param cr a cairo drawing context
 *  \return U8_ERROR_NONE if the image was drawn,
 *          U8_ERROR_NOT_FOUND if no image was found,
 *          U8_ERROR_PARSER_STRUCTURE if expected tokens in the input image were missing or in wrong order
 */
u8_error_t draw_stereotype_image_private_parse_drawing ( const draw_stereotype_image_t *this_,
                                                         bool draw,
                                                         utf8stringviewtokenizer_t *tok_iterator,
                                                         geometry_rectangle_t *io_view_rect,
                                                         u8_error_info_t *out_err_info,
                                                         const geometry_rectangle_t *target_bounds,
                                                         cairo_t *cr
                                                       );

#include "draw_stereotype_image.inl"

#endif  /* DRAW_STEREOTYPE_IMAGE_H */


/*
Copyright 2023-2023 Andreas Warnke

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
