/* File: data_stat.h; Copyright and License: see below */

#ifndef DATA_STAT_H
#define DATA_STAT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a set of statistics that can be used to analyze the result of a function
 *         that is supposed to do multiple things.
 *
 *  Example functions: import, export, repair db, check db, select all,
 *                     cut, copy, paste, delete, undo, redo, search, mark, instantiate.
 */

#include "data_table.h"
#include <stdint.h>

/*!
 *  \brief constants for data series
 */
enum data_stat_series_enum {
    DATA_STAT_SERIES_CREATED = 0,  /*!< number of things created */
    DATA_STAT_SERIES_MODIFIED = 1,  /*!< number of things modified */
    DATA_STAT_SERIES_DELETED = 2,  /*!< number of things deleted */
    DATA_STAT_SERIES_IGNORED = 3,  /*!< number of things ignored, maybe because already existing or unmodified */
    DATA_STAT_SERIES_WARNINGS = 4,  /*!< number of things with possibly unexpected results */
    DATA_STAT_SERIES_ERRORS = 5,  /*!< number of things not processed as expected */
    DATA_STAT_SERIES_MAX = 6
};

typedef enum data_stat_series_enum data_stat_series_t;

enum data_stat_tables_enum {
    DATA_STAT_TABLES_MAX = (DATA_TABLE_DIAGRAM+1)
};

/*!
 *  \brief the statistical numbers of all tables and series
 */
typedef uint_fast32_t data_stat_t[DATA_STAT_TABLES_MAX][DATA_STAT_SERIES_MAX];

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
 *  \param series the data series to retrieve
 *  \param table the table entry in the data series to retrieve
 *  \return the count of the data entity
 */
static inline uint_fast32_t data_stat_get_count ( const data_stat_t *this_,
                                                  data_stat_series_t series,
                                                  data_table_t table
                                                );

/*!
 *  \brief increases the counter of one data entity
 *
 *  \param this_ pointer to own object attributes
 *  \param series the data series to retrieve
 *  \param table the table entry in the data series to retrieve
 */
static inline void data_stat_inc_count ( data_stat_t *this_,
                                         data_stat_series_t series,
                                         data_table_t table
                                       );

/*!
 *  \brief traces data_stat_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_stat_trace ( const data_stat_t *this_ );

#include "set/data_stat.inl"

#endif  /* DATA_STAT_H */


/*
Copyright 2020-2020 Andreas Warnke

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
