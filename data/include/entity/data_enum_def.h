/* File: data_enum_def.h; Copyright and License: see below */

#ifndef DATA_ENUM_DEF_H
#define DATA_ENUM_DEF_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines an enumeration value by id and name
 */

#include <stdbool.h>

/*!
 *  \brief attributes of a data_enum_def_t
 */
struct data_enum_def_struct {
    int value;
    const char* name;
};

typedef struct data_enum_def_struct data_enum_def_t;

#define DATA_ENUM_DEF_VOID ((data_enum_def_t){.value=-1,.name=""})
#define DATA_ENUM_DEF(enum_value,enum_name) ((data_enum_def_t){.value=enum_value,.name=enum_name})

/*!
 *  \brief initializes the data_enum_def_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param value value of the enumeration definition
 *  \param name name of the enumeration definition
 */
static inline void data_enum_def_init ( data_enum_def_t *this_, int value, const char* name );

/*!
 *  \brief destroys the data_enum_def_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_enum_def_destroy ( data_enum_def_t *this_ );

/*!
 *  \brief gets the value of data_enum_def_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline int data_enum_def_get_value ( const data_enum_def_t *this_ );

/*!
 *  \brief gets the name of data_enum_def_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline const char* data_enum_def_get_name ( const data_enum_def_t *this_ );

/*!
 *  \brief prints the data_enum_def_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_enum_def_trace ( const data_enum_def_t *this_ );

/*!
 *  \brief checks if data_enum_def has a value not equal to -1
 *
 *  \param this_ pointer to own object attributes
 *  \return true if valid, false if void
 */
static inline bool data_enum_def_is_valid ( const data_enum_def_t *this_ );

#include "entity/data_enum_def.inl"

#endif  /* DATA_ENUM_DEF_H */


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
