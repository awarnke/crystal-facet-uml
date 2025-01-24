/* File: geometry_connector.h; Copyright and License: see below */

#ifndef GEOMETRY_CONNECTOR_H
#define GEOMETRY_CONNECTOR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores coordinates of a connector.
 *
 *  The connector consists of a source-end,
 *  connected to a main-line, connected to a destination-end.
 */

#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_point.h"
#include "geometry/geometry_3dir.h"
#include "geometry/geometry_connector_segment.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief attributes of a connector
 */
struct geometry_connector_struct {
    double source_end_x;
    double source_end_y;
    double main_line_source_x;
    double main_line_source_y;
    double main_line_destination_x;
    double main_line_destination_y;
    double destination_end_x;
    double destination_end_y;
};

typedef struct geometry_connector_struct geometry_connector_t;

/*!
 *  \brief initializes the geometry_connector_t struct for a vertical main-line
 *
 *  \param this_ pointer to own object attributes
 *  \param source_end_x x coordinate of the source end point
 *  \param source_end_y y coordinate of the source end point
 *  \param destination_end_x x coordinate of the destination end point
 *  \param destination_end_y y coordinate of the destination end point
 *  \param main_line_x x coordinate of the vertical main bar
 */
static inline void geometry_connector_init_vertical ( geometry_connector_t *this_,
                                                      double source_end_x,
                                                      double source_end_y,
                                                      double destination_end_x,
                                                      double destination_end_y,
                                                      double main_line_x
                                                    );

/*!
 *  \brief initializes the geometry_connector_t struct for a horizontal main-line
 *
 *  \param this_ pointer to own object attributes
 *  \param source_end_x x coordinate of the source end point
 *  \param source_end_y y coordinate of the source end point
 *  \param destination_end_x x coordinate of the destination end point
 *  \param destination_end_y y coordinate of the destination end point
 *  \param main_line_y y coordinate of the horizontal main bar
 */
static inline void geometry_connector_init_horizontal ( geometry_connector_t *this_,
                                                        double source_end_x,
                                                        double source_end_y,
                                                        double destination_end_x,
                                                        double destination_end_y,
                                                        double main_line_y
                                                      );

/*!
 *  \brief re-initializes the geometry_connector_t struct for a vertical main-line
 *
 *  \param this_ pointer to own object attributes
 *  \param source_end_x x coordinate of the source end point
 *  \param source_end_y y coordinate of the source end point
 *  \param destination_end_x x coordinate of the destination end point
 *  \param destination_end_y y coordinate of the destination end point
 *  \param main_line_x x coordinate of the vertical main bar
 */
static inline void geometry_connector_reinit_vertical ( geometry_connector_t *this_,
                                                        double source_end_x,
                                                        double source_end_y,
                                                        double destination_end_x,
                                                        double destination_end_y,
                                                        double main_line_x
                                                      );

/*!
 *  \brief re-initializes the geometry_connector_t struct for a horizontal main-line
 *
 *  \param this_ pointer to own object attributes
 *  \param source_end_x x coordinate of the source end point
 *  \param source_end_y y coordinate of the source end point
 *  \param destination_end_x x coordinate of the destination end point
 *  \param destination_end_y y coordinate of the destination end point
 *  \param main_line_y y coordinate of the horizontal main bar
 */
static inline void geometry_connector_reinit_horizontal ( geometry_connector_t *this_,
                                                          double source_end_x,
                                                          double source_end_y,
                                                          double destination_end_x,
                                                          double destination_end_y,
                                                          double main_line_y
                                                        );

/*!
 *  \brief copies original to this uninitialized geometry_connector_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void geometry_connector_copy ( geometry_connector_t *this_, const geometry_connector_t *original );

/*!
 *  \brief replaces the already initialized geometry_connector_t struct by a copy of original
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void geometry_connector_replace ( geometry_connector_t *this_, const geometry_connector_t *original );

/*!
 *  \brief initializes the geometry_connector_t struct to a connector from 0,0 to 0,0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_connector_init_empty ( geometry_connector_t *this_ );

/*!
 *  \brief re-initializes the geometry_connector_t struct to a connector from 0,0 to 0,0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_connector_reinit_empty ( geometry_connector_t *this_ );

/*!
 *  \brief destroys the geometry_connector_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_connector_destroy ( geometry_connector_t *this_ );

/*!
 *  \brief gets the attribute source_end_x from geometry_connector_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_connector_get_source_end_x ( const geometry_connector_t *this_ );

/*!
 *  \brief gets the attribute source_end_y from geometry_connector_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_connector_get_source_end_y ( const geometry_connector_t *this_ );

/*!
 *  \brief gets the attribute main_line_source_x from geometry_connector_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_connector_get_main_line_source_x ( const geometry_connector_t *this_ );

/*!
 *  \brief gets the attribute main_line_source_y from geometry_connector_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_connector_get_main_line_source_y ( const geometry_connector_t *this_ );

/*!
 *  \brief gets the attribute main_line_destination_x from geometry_connector_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_connector_get_main_line_destination_x ( const geometry_connector_t *this_ );

/*!
 *  \brief gets the attribute main_line_destination_y from geometry_connector_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_connector_get_main_line_destination_y ( const geometry_connector_t *this_ );

/*!
 *  \brief gets the attribute destination_end_x from geometry_connector_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_connector_get_destination_end_x ( const geometry_connector_t *this_ );

/*!
 *  \brief gets the attribute destination_end_y from geometry_connector_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_connector_get_destination_end_y ( const geometry_connector_t *this_ );

/*!
 *  \brief calculates the the point of the connector line after distance_covered
 *
 *  \param this_ pointer to own object attributes
 *  \param distance_covered distance from source, expected values range from 0.0 .. geometry_connector_get_length
 *  \return point on the connector line that has distance distance_covered from source end.
 *          If distance_covered < 0.0, the source point is returned.
 *          If distance > geometry_connector_get_length, the destination point is returned.
 */
static inline geometry_point_t geometry_connector_calculate_waypoint ( const geometry_connector_t *this_,
                                                                       double distance_covered
                                                                     );

/*!
 *  \brief gets the length of the connector line
 *
 *  \param this_ pointer to own object attributes
 *  \return length of the connector
 */
static inline double geometry_connector_get_length ( const geometry_connector_t *this_ );

/*!
 *  \brief gets the length of the source segment of the connector line
 *
 *  \param this_ pointer to own object attributes
 *  \return length of the source-segment of the connector
 */
static inline double geometry_connector_get_source_length ( const geometry_connector_t *this_ );

/*!
 *  \brief gets the length of the main segment of the connector line
 *
 *  \param this_ pointer to own object attributes
 *  \return length of the main-segment of the connector
 */
static inline double geometry_connector_get_main_length ( const geometry_connector_t *this_ );

/*!
 *  \brief gets the length of the destination segment of the connector line
 *
 *  \param this_ pointer to own object attributes
 *  \return length of the destination-segment of the connector
 */
static inline double geometry_connector_get_destination_length ( const geometry_connector_t *this_ );

/*!
 *  \brief determines if a given coordinate is close to geometry_connector_t
 *
 *  \param this_ pointer to own object attributes
 *  \param x x coordinate
 *  \param y y coordinate
 *  \param max_distance maximum distance between (x,y) to the connector
 *  \return true if the location is close to the connector.
 */
static inline bool geometry_connector_is_close ( const geometry_connector_t *this_, double x, double y, double max_distance );

/*!
 *  \brief determines if a given rectangle is overlapped (touching excluded)
 *
 *  \param this_ pointer to own object attributes
 *  \param rect rectangle that is checked for overlaps
 *  \return true if the rectangle is intersected
 */
static inline bool geometry_connector_is_intersecting_rectangle ( const geometry_connector_t *this_,
                                                                  const geometry_rectangle_t *rect
                                                                );

/*!
 *  \brief determines the length of a connector that is intersecting a given rectangle (touching included)
 *
 *  \param this_ pointer to own object attributes
 *  \param rect rectangle that is checked for overlaps
 *  \return length of the connector-part that is contained within rect
 */
static inline double geometry_connector_get_transit_length ( const geometry_connector_t *this_,
                                                             const geometry_rectangle_t *rect
                                                           );

/*!
 *  \brief counts intersections of two connector objects
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to connector where intersections shall be determined
 *  \return number of intersections. 0 if there are no intersections, 9 max (3x3, one for each segment)
 */
static inline uint32_t geometry_connector_count_connector_intersects ( const geometry_connector_t *this_,
                                                                       const geometry_connector_t *that
                                                                     );

/*!
 *  \brief determines the length of the sub-path where the connector has the same location and direction
 *         as the border of the rectangle.
 *
 *  \param this_ pointer to own object attributes
 *  \param rect pointer to rectangle where the length of the same path shall be determined
 *  \param max_distance maximum distance between the connector and the rectangle
 *  \return length of the connector-part that is the same with the rect boundary
 */
static inline double geometry_connector_get_same_path_length_rect ( const geometry_connector_t *this_,
                                                                    const geometry_rectangle_t *rect,
                                                                    double max_distance
                                                                  );

/*!
 *  \brief determines the length of the sub-path where both connectors have the same location and direction.
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to connector where the length of the same path shall be determined
 *  \param max_distance maximum distance between the two connectors
 *  \return length of the connector-parts that are the same
 */
static inline double geometry_connector_get_same_path_length_conn ( const geometry_connector_t *this_,
                                                                    const geometry_connector_t *that,
                                                                    double max_distance
                                                                  );

/*!
 *  \brief gets the bounding rectangle of geometry_connector_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline geometry_rectangle_t geometry_connector_get_bounding_rectangle ( const geometry_connector_t *this_ );

/*!
 *  \brief gets the bounding rectangle of geometry_connector_t
 *
 *  \param this_ pointer to own object attributes
 *  \param segment_id segment for which to determine the bounding rectangle
 */
static inline geometry_rectangle_t geometry_connector_get_segment_bounds ( const geometry_connector_t *this_,
                                                                           geometry_connector_segment_t segment_id
                                                                         );

/*!
 *  \brief gets the directions-pattern that is described by the connector
 *
 *  \param this_ pointer to own object attributes
 *  \return three geometry_direction_t, index 0 for source, 1 for mainline, 2 for destination
 */
static inline geometry_3dir_t geometry_connector_get_directions ( const geometry_connector_t *this_ );

/*!
 *  \brief prints the geometry_connector_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_connector_trace ( const geometry_connector_t *this_ );

#include "geometry/geometry_connector.inl"

#endif  /* GEOMETRY_CONNECTOR_H */


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
