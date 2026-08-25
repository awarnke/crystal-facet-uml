/* File: geometry_compartments.h; Copyright and License: see below */

#ifndef GEOMETRY_COMPARTMENTS_H
#define GEOMETRY_COMPARTMENTS_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores dimensions of a classifiers feature compartments
 */

#include "geometry/geometry_dimensions.h"
#include "geometry/geometry_compartment_type.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief attributes describing the dimensions of a classifiers feature compartments
 */
struct geometry_compartments_struct {
    geometry_dimensions_t feature_compartments;  /*!< this excludes ports and interfaces */
    bool has_properties;  /*!< true if a compartment of type DATA_FEATURE_TYPE_PROPERTY exists */
    bool has_operations;  /*!< true if a compartment of type DATA_FEATURE_TYPE_OPERATION exists */
    bool has_tagged_values;  /*!< true if a compartment of type DATA_FEATURE_TYPE_TAGGED_VALUE exists */
    double port_height_on_left;
    double port_height_on_right;
    double port_width_on_top;
    double port_width_on_bottom;
    double iface_height_on_left;
    double iface_height_on_right;
    double iface_width_on_top;
    double iface_width_on_bottom;
    double standard_object_border;  /*!< space between compartment border and feature */
    double preferred_object_distance;  /*!< space between compartment border and feature */
};

typedef struct geometry_compartments_struct geometry_compartments_t;

/*!
 *  \brief initializes the geometry_compartments_t struct to an empty rect at position 0,0
 *
 *  \param this_ pointer to own object attributes
 *  \param standard_object_border standard border size of objects, according to pencil_size_t
 *  \param preferred_object_distance preferred distance between two objects or object and connector
 */
static inline void geometry_compartments_init ( geometry_compartments_t *this_,
                                                double standard_object_border,
                                                double preferred_object_distance
                                              );

/*!
 *  \brief re-initializes the geometry_compartments_t struct to an empty rect at position 0,0
 *
 *  \param this_ pointer to own object attributes
 *  \param standard_object_border standard border size of objects, according to pencil_size_t
 *  \param preferred_object_distance preferred distance between two objects or object and connector
 */
static inline void geometry_compartments_reinit ( geometry_compartments_t *this_,
                                                  double standard_object_border,
                                                  double preferred_object_distance
                                                );

/*!
 *  \brief copies the geometry_compartments_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to original object attributes to be copied
 */
static inline void geometry_compartments_copy ( geometry_compartments_t *this_, const geometry_compartments_t *original );

/*!
 *  \brief moves the geometry_compartments_t struct to this uninitialized geometry_compartments_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to original object attributes to be moved
 */
static inline void geometry_compartments_move ( geometry_compartments_t *this_, geometry_compartments_t *that );

/*!
 *  \brief replaces the geometry_compartments_t struct to this already initialized geometry_compartments_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to original object attributes
 */
static inline void geometry_compartments_replace ( geometry_compartments_t *this_, const geometry_compartments_t *original );

/*!
 *  \brief moves the geometry_compartments_t struct to this already initialized geometry_compartments_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to original object attributes to be moved
 */
static inline void geometry_compartments_replacemove ( geometry_compartments_t *this_, geometry_compartments_t *that );

/*!
 *  \brief initializes the geometry_compartments_t struct to an empty rect at position 0,0
 *
 *  The layouting metrics for standard_object_border and preferred_object_distance are also set to 0.0.
 *
 *  \return own object attributes
 */
static inline geometry_compartments_t geometry_compartments_new_empty ();

/*!
 *  \brief initializes the geometry_compartments_t struct to an empty rect at position 0,0
 *
 *  The layouting metrics for standard_object_border and preferred_object_distance are also set to 0.0.
 *
 *  \param compartments dimensions of feature compartments
 *  \param outer dimensions of outer features: ports and interfaces
 *  \return own object attributes
 */
static inline geometry_compartments_t geometry_compartments_new ( const geometry_dimensions_t *compartments,
                                                                  const geometry_dimensions_t *outer );

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
 *  \param feature_dim dimension of feature.
 */
static inline void geometry_compartments_add_feature ( geometry_compartments_t *this_,
                                                       geometry_compartment_type_t compartment,
                                                       const geometry_dimensions_t * feature_dim
                                                     );

/*!
 *  \brief gets the attribute of geometry_compartments_t: feature_compartments
 *
 *  \param this_ pointer to own object attributes
 */
static inline const geometry_dimensions_t * geometry_compartments_get_feature_compartments ( const geometry_compartments_t *this_ );

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
 *  \brief gets the attribute of geometry_compartments_t: iface_height_on_left
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_if_height_on_left ( const geometry_compartments_t *this_ );

/*!
 *  \brief gets the attribute of geometry_compartments_t: iface_height_on_right
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_if_height_on_right ( const geometry_compartments_t *this_ );

/*!
 *  \brief gets the attribute of geometry_compartments_t: iface_width_on_top
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_if_width_on_top ( const geometry_compartments_t *this_ );

/*!
 *  \brief gets the attribute of geometry_compartments_t: iface_width_on_bottom
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_if_width_on_bottom ( const geometry_compartments_t *this_ );

/*!
 *  \brief prints the geometry_compartments_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_compartments_trace ( const geometry_compartments_t *this_ );

/*!
 *  \brief gets the maximum of all interface and port heights
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_outer_height ( const geometry_compartments_t *this_ );

/*!
 *  \brief gets the maximum of all interface and port widths
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_compartments_get_outer_width ( const geometry_compartments_t *this_ );

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
