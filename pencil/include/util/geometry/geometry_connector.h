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

#include "util/geometry/geometry_rectangle.h"
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
static inline double geometry_connector_destination_end_x ( const geometry_connector_t *this_ );

/*!
 *  \brief gets the attribute destination_end_y from geometry_connector_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_connector_get_destination_end_y ( const geometry_connector_t *this_ );

/*!
 *  \brief gets the length of the connector line
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_connector_get_length ( const geometry_connector_t *this_ );

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
 *  \brief determines if a given rectangle is overlapped
 *
 *  \param this_ pointer to own object attributes
 *  \param rect rectangle that is checked for overlaps
 *  \return true if the rectangle is intersected
 */
static inline bool geometry_connector_is_intersecting_rectangle ( const geometry_connector_t *this_, const geometry_rectangle_t *rect );

/*!
 *  \brief counts intersections of two connector objects
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to connector where intersections shall be determined
 *  \return number of intersections. 0 if there are no intersections, 9 max (3x3, one for each segment)
 */
static inline uint32_t geometry_connector_count_connector_intersects ( const geometry_connector_t *this_, const geometry_connector_t *that );

/*!
 *  \brief gets the bounding rectangle of geometry_connector_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline geometry_rectangle_t geometry_connector_get_bounding_rectangle ( const geometry_connector_t *this_ );

/*!
 *  \brief prints the geometry_connector_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_connector_trace ( const geometry_connector_t *this_ );

#include "util/geometry/geometry_connector.inl"

#endif  /* GEOMETRY_CONNECTOR_H */


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
