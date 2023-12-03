/* File: txt_icon.h; Copyright and License: see below */

#ifndef TXT_ICON_H
#define TXT_ICON_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides textual icons for relationship types
 */

#include "data_relationship_type.h"

/*!
 *  \brief attributes of the txt icon provider
 */
struct txt_icon_struct {
    int dummy;  /*!< txt_icon_t has no attributes */
};

typedef struct txt_icon_struct txt_icon_t;

/*!
 *  \brief initializes the txt icon provider
 *
 *  \param this_ pointer to own object attributes
 */
static inline void txt_icon_init( txt_icon_t *this_ );

/*!
 *  \brief destroys the txt icon provider
 *
 *  \param this_ pointer to own object attributes
 */
static inline void txt_icon_destroy( txt_icon_t *this_ );

/*!
 *  \brief gets a textual icon for arelationship type
 *
 *  \param this_ pointer to own object attributes
 *  \param type data_relationship_type_t to represent as textual icon
 *  \return textual icon of the relationship, such as "-->" or "(+)--" or "<>--"
 */
static inline const char* txt_icon_get_relationship ( txt_icon_t *this_, data_relationship_type_t type );

#include "txt_icon.inl"

#endif  /* TXT_ICON_H */


/*
Copyright 2023-2023 Andreas Warnke

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
