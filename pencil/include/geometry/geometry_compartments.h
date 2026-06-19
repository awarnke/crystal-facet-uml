/* File: geometry_compartments.h; Copyright and License: see below */

#ifndef GEOMETRY_COMPARTMENTS_H
#define GEOMETRY_COMPARTMENTS_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores coordinates of several feature compartments
 */

#include "geometry/geometry_rectangle.h"
#include <stdbool.h>

/*!
 *  \brief constants identifying compartments
 */
enum geometry_compartments_type_enum {
    GEOMETRY_COMPARTMENTS_PROPERTIES,
    GEOMETRY_COMPARTMENTS_OPERATIONS,
    GEOMETRY_COMPARTMENTS_TAGGED_VALUES,
    GEOMETRY_COMPARTMENTS_PORT_ON_LEFT,
    GEOMETRY_COMPARTMENTS_PORT_ON_RIGHT,
    GEOMETRY_COMPARTMENTS_PORT_ON_TOP,
    GEOMETRY_COMPARTMENTS_PORT_ON_BOTTOM,
    GEOMETRY_COMPARTMENTS_IF_ON_LEFT,
    GEOMETRY_COMPARTMENTS_IF_ON_RIGHT,
    GEOMETRY_COMPARTMENTS_IF_ON_TOP,
    GEOMETRY_COMPARTMENTS_IF_ON_BOTTOM
};

typedef enum geometry_compartments_type_enum geometry_compartments_type_t;

/*!
 *  \brief attributes of several feature compartments
 */
struct geometry_compartments_struct {
    geometry_rectangle_t feature_compartments;  /*!< this excludes ports and interfaces */
    double port_height_on_left;
    double port_height_on_right;
    double port_width_on_top;
    double port_width_on_bottom;
    double if_height_on_left;
    double if_height_on_right;
    double if_width_on_top;
    double if_width_on_bottom;
    double inner_features_height;  /*!< this includes the needed compartment separator lines */
};

typedef struct geometry_compartments_struct geometry_compartments_t;

/*!
 *  \brief initializes the geometry_compartments_t struct to an empty rect at position 0,0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_compartments_init_empty ( geometry_compartments_t *this_ );

/*!
 *  \brief re-initializes the geometry_compartments_t struct to an empty rect at position 0,0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_compartments_reinit_empty ( geometry_compartments_t *this_ );

/*!
 *  \brief destroys the geometry_compartments_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_compartments_destroy ( geometry_compartments_t *this_ );

/*!
 *  \brief expands the geometry_compartments_t by one additional feature
 *
 *  \param this_ pointer to own object attributes
 *  \param compartment type of compartment
 *  \param feature_bounds bounding rectangle of feature.
 */
static inline void geometry_compartments_add_feature ( geometry_compartments_t *this_,
                                                       geometry_compartments_type_t compartment,
                                                       const geometry_rectangle_t * feature_bounds
                                                     );

/*!
 *  \brief gets the attribute of geometry_compartments_t: feature_compartments
 *
 *  \param this_ pointer to own object attributes
 */
static inline const geometry_rectangle_t * geometry_compartments_get_feature_compartments ( const geometry_compartments_t *this_ );

/*!
 *  \brief gets the attribute of geometry_compartments_t: port_height_on_left
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_port_height_on_left ( const geometry_compartments_t *this_ );

/*!
 *  \brief gets the attribute of geometry_compartments_t: port_height_on_right
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_port_height_on_right ( const geometry_compartments_t *this_ );

/*!
 *  \brief gets the attribute of geometry_compartments_t: port_width_on_top
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_port_width_on_top ( const geometry_compartments_t *this_ );

/*!
 *  \brief gets the attribute of geometry_compartments_t: port_width_on_bottom
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_port_width_on_bottom ( const geometry_compartments_t *this_ );

/*!
 *  \brief gets the attribute of geometry_compartments_t: if_height_on_left
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_if_height_on_left ( const geometry_compartments_t *this_ );

/*!
 *  \brief gets the attribute of geometry_compartments_t: if_height_on_right
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_if_height_on_right ( const geometry_compartments_t *this_ );

/*!
 *  \brief gets the attribute of geometry_compartments_t: if_width_on_top
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_if_width_on_top ( const geometry_compartments_t *this_ );

/*!
 *  \brief gets the attribute of geometry_compartments_t: if_width_on_bottom
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_if_width_on_bottom ( const geometry_compartments_t *this_ );

/*!
 *  \brief gets the attribute of geometry_compartments_t: inner_features_height
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_inner_features_height ( const geometry_compartments_t *this_ );

/*!
 *  \brief prints the geometry_compartments_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_compartments_trace ( const geometry_compartments_t *this_ );

#include "geometry/geometry_compartments.inl"

#endif  /* GEOMETRY_COMPARTMENTS_H */


/*
Copyright 2026-2026 Andreas Warnke

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
