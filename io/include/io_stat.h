/* File: io_stat.h; Copyright and License: see below */

#ifndef IO_STAT_H
#define IO_STAT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides statistics on imported and exported, planned and actual numbers of different-typed data objects
 */

#include <stdint.h>

/*!
 *  \brief attributes of the input/output statistics io_stat
 */
struct io_stat_struct {
    uint32_t diagrams;  /*!< counter for objects of type: diagram */
    uint32_t diagramelements;  /*!< counter for objects of type: diagramelement */
    uint32_t classifiers;  /*!< counter for objects of type: classifier */
    uint32_t uncond_features;  /*!< counter for objects of type: unconditional feature (e.g. property, port, operation) */
    uint32_t scenario_features;  /*!< counter for objects of type: scenario feature (lifeline) */
    uint32_t uncond_relationships;  /*!< counter for objects of type: unconditional relationship (no end connects to a scenario feature) */
    uint32_t scenario_relationships;  /*!< counter for objects of type: scenario relationship (e.g. message in a sequence diagram) */
};

typedef struct io_stat_struct io_stat_t;

/*!
 *  \brief initializes the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 */
static inline void io_stat_init( io_stat_t *this_ );

/*!
 *  \brief destroys the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 */
static inline void io_stat_destroy( io_stat_t *this_ );

/*!
 *  \brief sets a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \param count new value to set
 */
static inline void io_stat_set_diagrams( io_stat_t *this_, uint32_t count );

/*!
 *  \brief gets a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \return the requested value
 */
static inline uint32_t io_stat_get_diagrams( const io_stat_t *this_ );

/*!
 *  \brief increases a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \param count number by which to increase the current value
 */
static inline void io_stat_inc_diagrams( io_stat_t *this_, uint32_t count );

/*!
 *  \brief sets a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \param count new value to set
 */
static inline void io_stat_set_diagramelements( io_stat_t *this_, uint32_t count );

/*!
 *  \brief gets a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \return the requested value
 */
static inline uint32_t io_stat_get_diagramelements( const io_stat_t *this_ );

/*!
 *  \brief increases a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \param count number by which to increase the current value
 */
static inline void io_stat_inc_diagramelements( io_stat_t *this_, uint32_t count );

/*!
 *  \brief sets a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \param count new value to set
 */
static inline void io_stat_set_classifiers( io_stat_t *this_, uint32_t count );

/*!
 *  \brief gets a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \return the requested value
 */
static inline uint32_t io_stat_get_classifiers( const io_stat_t *this_ );

/*!
 *  \brief increases a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \param count number by which to increase the current value
 */
static inline void io_stat_inc_classifiers( io_stat_t *this_, uint32_t count );

/*!
 *  \brief sets a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \param count new value to set
 */
static inline void io_stat_set_uncond_features( io_stat_t *this_, uint32_t count );

/*!
 *  \brief gets a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \return the requested value
 */
static inline uint32_t io_stat_get_uncond_features( const io_stat_t *this_ );

/*!
 *  \brief increases a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \param count number by which to increase the current value
 */
static inline void io_stat_inc_uncond_features( io_stat_t *this_, uint32_t count );

/*!
 *  \brief sets a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \param count new value to set
 */
static inline void io_stat_set_scenario_features( io_stat_t *this_, uint32_t count );

/*!
 *  \brief gets a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \return the requested value
 */
static inline uint32_t io_stat_get_scenario_features( const io_stat_t *this_ );

/*!
 *  \brief increases a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \param count number by which to increase the current value
 */
static inline void io_stat_inc_scenario_features( io_stat_t *this_, uint32_t count );

/*!
 *  \brief sets a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \param count new value to set
 */
static inline void io_stat_set_uncond_relationships( io_stat_t *this_, uint32_t count );

/*!
 *  \brief gets a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \return the requested value
 */
static inline uint32_t io_stat_get_uncond_relationships( const io_stat_t *this_ );

/*!
 *  \brief increases a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \param count number by which to increase the current value
 */
static inline void io_stat_inc_uncond_relationships( io_stat_t *this_, uint32_t count );

/*!
 *  \brief sets a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \param count new value to set
 */
static inline void io_stat_set_scenario_relationships( io_stat_t *this_, uint32_t count );

/*!
 *  \brief gets a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \return the requested value
 */
static inline uint32_t io_stat_get_scenario_relationships( const io_stat_t *this_ );

/*!
 *  \brief increases a value of the input/output statistics io_stat
 *
 *  \param this_ pointer to own object attributes
 *  \param count number by which to increase the current value
 */
static inline void io_stat_inc_scenario_relationships( io_stat_t *this_, uint32_t count );

#include "io_stat.inl"

#endif  /* IO_STAT_H */


/*
Copyright 2019-2019 Andreas Warnke

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
