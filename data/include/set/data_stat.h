/* File: data_stat.h; Copyright and License: see below */

#ifndef DATA_STAT_H
#define DATA_STAT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a set of statistics that can be used to analyze the result of a function
 *         that is supposed to do multiple things.
 *
 *  Example functions: import, export, repair db, check db, select none, select all,
 *                     cut, copy, paste, delete, undo, redo, search, mark, instantiate.
 */

#include "entity/data_table.h"
#include <stdint.h>

/*!
 *  \brief constants for data series
 */
enum data_stat_series_enum {
    DATA_STAT_SERIES_CREATED = 0,  /*!< number of things created (or exported) successfully */
    DATA_STAT_SERIES_EXPORTED = 0,  /*!< number of things exported (or created) successfully */
    DATA_STAT_SERIES_MODIFIED = 1,  /*!< number of things modified/un-/selected/found successfully */
    DATA_STAT_SERIES_DELETED = 2,  /*!< number of things deleted successfully */
    DATA_STAT_SERIES_IGNORED = 3,  /*!< number of things ignored, maybe because already existing or unmodified */
    DATA_STAT_SERIES_WARNING = 4,  /*!< number of things with possibly unexpected results; */
                                   /*!< these items are counted twice: e.g. one _MODIFIED and one _WARNING */
    DATA_STAT_SERIES_ERROR = 5,  /*!< number of things not processed as expected */
    DATA_STAT_SERIES_MAX = 6
};

typedef enum data_stat_series_enum data_stat_series_t;

/*!
 *  \brief constants for record types
 */
enum data_stat_table_enum {
    DATA_STAT_TABLE_LIFELINE = 0,  /*!< statistics on type lifeline (or other invisible features) */
    DATA_STAT_TABLE_CLASSIFIER = DATA_TABLE_CLASSIFIER,  /*!< statistics on type classifier */
    DATA_STAT_TABLE_FEATURE = DATA_TABLE_FEATURE,  /*!< statistics on type feature, except lifelines */
    DATA_STAT_TABLE_RELATIONSHIP = DATA_TABLE_RELATIONSHIP,  /*!< statistics on type relationship */
    DATA_STAT_TABLE_DIAGRAMELEMENT = DATA_TABLE_DIAGRAMELEMENT,  /*!< statistics on type diagram_element */
    DATA_STAT_TABLE_DIAGRAM = DATA_TABLE_DIAGRAM,  /*!< statistics on type diagram */
    DATA_STAT_TABLE_MAX = 6
};

typedef enum data_stat_table_enum data_stat_table_t;

/*!
 *  \brief the statistical numbers of all tables and series
 */
struct data_stat_struct {
    uint_fast32_t data[DATA_STAT_TABLE_MAX][DATA_STAT_SERIES_MAX];
};

typedef struct data_stat_struct data_stat_t;

/*!
 *  \brief initializes the data_stat_t struct.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_stat_init ( data_stat_t *this_ );

/*!
 *  \brief destroys the data_stat_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_stat_destroy ( data_stat_t *this_ );

/*!
 *  \brief gets the counter of one data entity
 *
 *  \param this_ pointer to own object attributes
 *  \param table the table entry in the data series to retrieve
 *  \param series the data series to retrieve
 *  \return the count of the data entity
 */
static inline uint_fast32_t data_stat_get_count ( const data_stat_t *this_,
                                                  data_stat_table_t table,
                                                  data_stat_series_t series
                                                );

/*!
 *  \brief increments the counter of one data entity
 *
 *  \param this_ pointer to own object attributes
 *  \param table the table entry in the data series to retrieve
 *  \param series the data series to retrieve
 */
static inline void data_stat_inc_count ( data_stat_t *this_,
                                         data_stat_table_t table,
                                         data_stat_series_t series
                                       );

/*!
 *  \brief adds increment to the counter of one data entity
 *
 *  \param this_ pointer to own object attributes
 *  \param table the table entry in the data series to retrieve
 *  \param series the data series to retrieve
 *  \param increment the value to add to the counter
 */
static inline void data_stat_add_count ( data_stat_t *this_,
                                         data_stat_table_t table,
                                         data_stat_series_t series,
                                         int_fast32_t increment
                                       );

/*!
 *  \brief adds all values of a stat
 *
 *  \param this_ pointer to own object attributes
 *  \param that the values to add
 */
static inline void data_stat_add ( data_stat_t *this_, const data_stat_t *that );

/*!
 *  \brief gets the sum of all data entities of one series
 *
 *  \param this_ pointer to own object attributes
 *  \param series the data series to retrieve
 *  \return the sum of all data entities of one series
 */
static inline uint_fast32_t data_stat_get_series_count ( const data_stat_t *this_,
                                                         data_stat_series_t series
                                                       );

/*!
 *  \brief gets the sum of all data entities of one table
 *
 *  \param this_ pointer to own object attributes
 *  \param table the table entry in the data series to retrieve
 *  \return the sum of all data entities of one table
 */
static inline uint_fast32_t data_stat_get_table_count ( const data_stat_t *this_,
                                                        data_stat_table_t table
                                                      );

/*!
 *  \brief gets the sum of all data entities
 *
 *  \param this_ pointer to own object attributes
 *  \return the sum of all data entities
 */
static inline uint_fast32_t data_stat_get_total_count ( const data_stat_t *this_ );

/*!
 *  \brief gets the sum of all data entities
 *
 *  \param this_ pointer to own object attributes
 *  \param series the data series to reset
 */
static void inline data_stat_reset_series( data_stat_t *this_, data_stat_series_t series );

/*!
 *  \brief traces data_stat_t
 *
 *  \param this_ pointer to own object attributes
 */
void data_stat_trace ( const data_stat_t *this_ );

#include "set/data_stat.inl"

#endif  /* DATA_STAT_H */


/*
Copyright 2020-2025 Andreas Warnke

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
