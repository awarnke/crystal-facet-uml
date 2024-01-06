/* File: gui_type_resource.h; Copyright and License: see below */

#ifndef GUI_TYPE_RESOURCE_H
#define GUI_TYPE_RESOURCE_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Data objects needed to display an element type: name and icon
 */

#include "data_table.h"
#include "data_relationship_type.h"
#include "data_feature_type.h"
#include "data_classifier_type.h"
#include "data_diagram_type.h"
#include "data_type.h"
#include <gdk/gdk.h>
#include <stdint.h>

/*!
 *  \brief attributes of the type resource
 */
struct gui_type_resource_struct {
    data_type_t type;  /*!<  element type for which this type resource applies */
    const char * name;  /*!<  display name */
    GdkPixbuf * icon;  /*!<  icon to be displayed */
};

typedef struct gui_type_resource_struct gui_type_resource_t;

/*!
 *  \brief initializes the gui_type_resource_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param type type id
 *  \param name name of the type
 *  \param icon icon of the type
 */
static inline void gui_type_resource_init_diagram ( gui_type_resource_t *this_,
                                                    data_diagram_type_t type,
                                                    const char * name,
                                                    GdkPixbuf * icon
                                                  );

/*!
 *  \brief initializes the gui_type_resource_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param type type id
 *  \param name name of the type
 *  \param icon icon of the type
 */
static inline void gui_type_resource_init_classifier ( gui_type_resource_t *this_,
                                                       data_classifier_type_t type,
                                                       const char * name,
                                                       GdkPixbuf * icon
                                                     );

/*!
 *  \brief initializes the gui_type_resource_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param type type id
 *  \param name name of the type
 *  \param icon icon of the type
 */
static inline void gui_type_resource_init_feature ( gui_type_resource_t *this_,
                                                    data_feature_type_t type,
                                                    const char * name,
                                                    GdkPixbuf * icon
                                                  );

/*!
 *  \brief initializes the gui_type_resource_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param type type id
 *  \param name name of the type
 *  \param icon icon of the type
 */
static inline void gui_type_resource_init_relationship ( gui_type_resource_t *this_,
                                                         data_relationship_type_t type,
                                                         const char * name,
                                                         GdkPixbuf * icon
                                                       );

/*!
 *  \brief destroys the gui_type_resource_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_type_resource_destroy ( gui_type_resource_t *this_ );

/*!
 *  \brief gets the type
 *
 *  \param this_ pointer to own object attributes
 *  \return type of this gui_type_resource_t
 */
static inline const data_type_t * gui_type_resource_get_type ( const gui_type_resource_t *this_ );

/*!
 *  \brief gets the name
 *
 *  \param this_ pointer to own object attributes
 *  \return name of this gui_type_resource_t
 */
static inline const char * gui_type_resource_get_name ( const gui_type_resource_t *this_ );

/*!
 *  \brief gets the icon
 *
 *  GdkPixbuf cannot be const because most gtk functions require mutable GdkPixbuf as parameter.
 *
 *  \param this_ pointer to own object attributes
 *  \return icon of this gui_type_resource_t
 */
static inline GdkPixbuf * gui_type_resource_get_icon ( const gui_type_resource_t *this_ );

#include "gui_type_resource.inl"

#endif  /* GUI_TYPE_RESOURCE_H */


/*
Copyright 2020-2024 Andreas Warnke

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
