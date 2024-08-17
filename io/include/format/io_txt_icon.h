/* File: io_txt_icon.h; Copyright and License: see below */

#ifndef IO_TXT_ICON_H
#define IO_TXT_ICON_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides textual icons for relationship types
 */

#include "entity/data_relationship_type.h"

/*!
 *  \brief attributes of the txt icon provider
 */
struct io_txt_icon_struct {
    int dummy;  /*!< io_txt_icon_t has no attributes */
};

typedef struct io_txt_icon_struct io_txt_icon_t;

/*!
 *  \brief initializes the txt icon provider
 *
 *  \param this_ pointer to own object attributes
 */
static inline void io_txt_icon_init( io_txt_icon_t *this_ );

/*!
 *  \brief destroys the txt icon provider
 *
 *  \param this_ pointer to own object attributes
 */
static inline void io_txt_icon_destroy( io_txt_icon_t *this_ );

/*!
 *  \brief gets a textual icon for arelationship type
 *
 *  \param this_ pointer to own object attributes
 *  \param type data_relationship_type_t to represent as textual icon
 *  \return textual icon of the relationship, such as "-->" or "(+)--" or "<>--"
 */
static inline const char* io_txt_icon_get_relationship ( io_txt_icon_t *this_, data_relationship_type_t type );

#include "io_txt_icon.inl"

#endif  /* IO_TXT_ICON_H */


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
