/* File: consistency_stat.h; Copyright and License: see below */

#ifndef CONSISTENCY_STAT_H
#define CONSISTENCY_STAT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a set of statistics to monitor what consistency functions changed in addition to a request.
 *
 *  In contrast to data_stat, this struct only reports created and deleted objects,
 *  no modified, no errors, no warnings when someting happend that may not have been anticipated by the caller.
 *  These categories (errors and warnings) would require an understanding of what was expected.
 *  The consistency checks cannot provide this:
 *  for example when changing the diagram type and consistency checks also delete a feature;
 *  it is hard to estimate if this is an error or if this action was expected by the caller.
 */

#include <stdint.h>

/*!
 *  \brief the statistical numbers of all tables and series
 */
struct consistency_stat_struct {
    int32_t classifiers;  /*!< number of created classifiers, negative if classifiers are deleted */
    int32_t features;  /*!< number of created features, negative if features are deleted */
    int32_t lifelines;  /*!< number of created lifelines, negative if lifelines are deleted */
    int32_t relationships;  /*!< number of created relationships, negative if relationships are deleted */
};

typedef struct consistency_stat_struct consistency_stat_t;

/*!
 *  \def CONSISTENCY_STAT_EMPTY
 *  \brief Macro to facilitate static initialisation of a consistency_stat_t
 */
#define CONSISTENCY_STAT_EMPTY (consistency_stat_t){.classifiers=0,.features=0,.lifelines=0,.relationships=0,}

/*!
 *  \brief initializes the consistency_stat_t struct.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void consistency_stat_init ( consistency_stat_t *this_ );

/*!
 *  \brief destroys the consistency_stat_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void consistency_stat_destroy ( consistency_stat_t *this_ );

/*!
 *  \brief gets the sum of all changes
 *
 *  This may be zero even if some objects were created and others were deleted.
 *  It may be negative if more objects were deleted than created.
 *
 *  \param this_ pointer to own object attributes
 *  \return the count of the data entity
 */
static inline int32_t consistency_stat_get_total_count ( const consistency_stat_t *this_ );

/*!
 *  \brief adds one classifier
 *
 *  \param this_ pointer to own object attributes
 */
static inline void consistency_stat_increment_classifiers ( consistency_stat_t *this_ );

/*!
 *  \brief decrements classifiers by one
 *
 *  \param this_ pointer to own object attributes
 */
static inline void consistency_stat_decrement_classifiers ( consistency_stat_t *this_ );

/*!
 *  \brief gets number of classifiers
 *
 *  \param this_ pointer to own object attributes
 */
static inline int32_t consistency_stat_get_classifiers ( const consistency_stat_t *this_ );

/*!
 *  \brief adds one feature
 *
 *  \param this_ pointer to own object attributes
 */
static inline void consistency_stat_increment_features ( consistency_stat_t *this_ );

/*!
 *  \brief decrements features by one
 *
 *  \param this_ pointer to own object attributes
 */
static inline void consistency_stat_decrement_features ( consistency_stat_t *this_ );

/*!
 *  \brief gets number of features
 *
 *  \param this_ pointer to own object attributes
 */
static inline int32_t consistency_stat_get_features ( const consistency_stat_t *this_ );

/*!
 *  \brief adds one lifeline
 *
 *  \param this_ pointer to own object attributes
 */
static inline void consistency_stat_increment_lifelines ( consistency_stat_t *this_ );

/*!
 *  \brief decrements lifelines by one
 *
 *  \param this_ pointer to own object attributes
 */
static inline void consistency_stat_decrement_lifelines ( consistency_stat_t *this_ );

/*!
 *  \brief gets number of lifelines
 *
 *  \param this_ pointer to own object attributes
 */
static inline int32_t consistency_stat_get_lifelines ( const consistency_stat_t *this_ );

/*!
 *  \brief adds one relationship
 *
 *  \param this_ pointer to own object attributes
 */
static inline void consistency_stat_increment_relationships ( consistency_stat_t *this_ );

/*!
 *  \brief decrements relationships by one
 *
 *  \param this_ pointer to own object attributes
 */
static inline void consistency_stat_decrement_relationships ( consistency_stat_t *this_ );

/*!
 *  \brief gets number of relationships
 *
 *  \param this_ pointer to own object attributes
 */
static inline int32_t consistency_stat_get_relationships ( const consistency_stat_t *this_ );

/*!
 *  \brief traces consistency_stat_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void consistency_stat_trace ( const consistency_stat_t *this_ );

#include "consistency_stat.inl"

#endif  /* CONSISTENCY_STAT_H */


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
