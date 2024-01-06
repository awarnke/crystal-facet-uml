/* File: draw_svg_path_data.h; Copyright and License: see below */

#ifndef DRAW_SVG_PATH_DATA_H
#define DRAW_SVG_PATH_DATA_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Converts svg path data commands to a path in cairo drawing context
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

/*!
 *  \brief attributes of the draw svg path data functions
 */
struct draw_svg_path_data_struct {
    int dummy;  /*!< This object is a collection of stateless drawing functions */
};

typedef struct draw_svg_path_data_struct draw_svg_path_data_t;

/*!
 *  \brief initializes the draw_svg_path_data_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void draw_svg_path_data_init( draw_svg_path_data_t *this_ );

/*!
 *  \brief destroys the draw_svg_path_data_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void draw_svg_path_data_destroy( draw_svg_path_data_t *this_ );

/*!
 *  \brief parses the svg path data drawing commands, determines the svg view bounds
 *
 *  \param this_ pointer to own object attributes
 *  \param tok_iterator token iterator of drawing directives. The first token shall be the first draw command.
 *                      double quotes or end-of-stream end the processing, the double quotes token is already consumed.
 *                      the token iterator must be in mode UTF8STRINGVIEWTOKENMODE_FLOAT_ONLY.
 *  \param[out] out_view_rect bounding rectangle of the svg drawing_directives
 *  \param[out] out_err_info pointer to an error_info_t data struct;
 *                           in case of U8_ERROR_PARSER_STRUCTURE, it provides an error description when returning
 *  \return U8_ERROR_NONE if the image was drawn,
 *          U8_ERROR_NOT_FOUND if no image was found,
 *          U8_ERROR_PARSER_STRUCTURE if expected tokens in the input image were missing or in wrong order
 */
static inline u8_error_t draw_svg_path_data_parse_bounds ( const draw_svg_path_data_t *this_,
                                                           utf8stringviewtokenizer_t *tok_iterator,
                                                           geometry_rectangle_t *out_view_rect,
                                                           u8_error_info_t *out_err_info
                                                         );

/*!
 *  \brief draws the svg path data drawing commands into the target_bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param tok_iterator token iterator of drawing directives. The first token shall be the first draw command.
 *                      double quotes or end-of-stream end the processing, the double quotes token is already consumed.
 *                      the token iterator must be in mode UTF8STRINGVIEWTOKENMODE_FLOAT_ONLY.
 *  \param[in] in_view_rect bounding rectangle of the svg drawing_directives; this is needed as input.
 *  \param[out] out_err_info pointer to an error_info_t data struct;
 *                           in case of U8_ERROR_PARSER_STRUCTURE, it provides an error description when returning
 *  \param target_bounds bounding rectangle of the cairo drawing directives, to which io_view_rect is scaled to.
 *  \param cr a cairo drawing context
 *  \return U8_ERROR_NONE if the image was drawn,
 *          U8_ERROR_NOT_FOUND if no image was found,
 *          U8_ERROR_PARSER_STRUCTURE if expected tokens in the input image were missing or in wrong order
 */
static inline u8_error_t draw_svg_path_data_draw ( const draw_svg_path_data_t *this_,
                                                   utf8stringviewtokenizer_t *tok_iterator,
                                                   const geometry_rectangle_t *in_view_rect,
                                                   u8_error_info_t *out_err_info,
                                                   const geometry_rectangle_t *target_bounds,
                                                   cairo_t *cr
                                                 );

/*!
 *  \brief parses the svg path data drawing commands and/or draws these into the target_bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param draw false if only the view_rect shall be determined, true if the drawing_directives shall be drawn
 *  \param tok_iterator token iterator of drawing directives. The first token shall be the first draw command.
 *                      double quotes or end-of-stream end the processing, the double quotes token is already consumed.
 *                      the token iterator must be in mode UTF8STRINGVIEWTOKENMODE_FLOAT_ONLY.
 *  \param[in,out] io_view_rect bounding rectangle of the svg drawing_directives;
 *                              in all cases this is provided as output,
 *                              in case of draw==true this is needed as input,
 *                              in case of draw==false this is written as output.
 *  \param[out] out_err_info pointer to an error_info_t data struct;
 *                           in case of U8_ERROR_PARSER_STRUCTURE, it provides an error description when returning
 *  \param target_bounds bounding rectangle of the cairo drawing directives, to which io_view_rect is scaled to.
 *  \param cr a cairo drawing context or NULL in case draw==false
 *  \return U8_ERROR_NONE if the image was drawn,
 *          U8_ERROR_NOT_FOUND if no image was found,
 *          U8_ERROR_PARSER_STRUCTURE if expected tokens in the input image were missing or in wrong order
 */
u8_error_t draw_svg_path_data_private_parse ( const draw_svg_path_data_t *this_,
                                              bool draw,
                                              utf8stringviewtokenizer_t *tok_iterator,
                                              geometry_rectangle_t *io_view_rect,
                                              u8_error_info_t *out_err_info,
                                              const geometry_rectangle_t *target_bounds,
                                              cairo_t *cr
                                            );

/*!
 *  \brief determines the angle between two 2-dim vectors.
 *
 *  \param this_ pointer to own object attributes
 *  \param u_x x value of the u vector
 *  \param u_y y value of the u vector
 *  \param v_x x value of the v vector
 *  \param v_y y value of the v vector
 *  \return angle in rad, range -pi..pi
 */
static inline double draw_svg_path_data_private_get_angle ( const draw_svg_path_data_t *this_,
                                                            double u_x,
                                                            double u_y,
                                                            double v_x,
                                                            double v_y
                                                          );

/*!
 *  \brief determines the center, start and delta angles of the arc given the endpoints.
 *
 *  This function is an implementation of the proposed conversion rules in
 *  https://www.w3.org/TR/SVG/implnote.html#ArcConversionEndpointToCenter
 *
 *  \param this_ pointer to own object attributes
 *  \param start_x abscissa of the starting point
 *  \param start_y ordinate of the starting point
 *  \param end_x abscissa of the end point
 *  \param end_y ordinate of the end point
 *  \param large_arc true if the arc is spanning more than 180 degree / 1*pi
 *  \param sweep_positive_dir true if the arc is traversed in positive-angle direction
 *  \param r_x major ellipsis radius
 *  \param r_y minor ellipsis radius
 *  \param phi angle between major ellipsis radius and x-axis (unit: rad)
 *  \param[out] out_center_x abscissa of the arc-center point
 *  \param[out] out_center_y ordinate of the arc-center point
 *  \param[out] out_start_angle angle of the starting point (unit: rad)
 *  \param[out] out_delta_angle delta angle from the start to the end point (unit: rad)
 *  \return U8_ERROR_NONE if the provided values are a valid arc,
 *          U8_ERROR_VALUE_OUT_OF_RANGE if provided values are not valid, e.g. start and end points are equal.
 *          U8_ERROR_EDGE_CASE_PARAM if the arc degrades to a straight line from start to end
 */
static inline u8_error_t draw_svg_path_data_private_get_arc_center ( const draw_svg_path_data_t *this_,
                                                                     double start_x,
                                                                     double start_y,
                                                                     double end_x,
                                                                     double end_y,
                                                                     bool large_arc,
                                                                     bool sweep_positive_dir,
                                                                     double r_x,
                                                                     double r_y,
                                                                     double phi,
                                                                     double *out_center_x,
                                                                     double *out_center_y,
                                                                     double *out_start_angle,
                                                                     double *out_delta_angle
                                                                   );

#include "draw_svg_path_data.inl"

#endif  /* DRAW_SVG_PATH_DATA_H */


/*
Copyright 2023-2024 Andreas Warnke

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
