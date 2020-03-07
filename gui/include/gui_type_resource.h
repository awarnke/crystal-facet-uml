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
#include <gdk/gdk.h>
#include <stdint.h>

/*!
 *  \brief union of type ids
 */
union gui_type_resource_union {
    data_relationship_type_t relationship;  /*!<  type id in case the element type is DATA_TABLE_RELATIONSHIP */
    data_feature_type_t feature;  /*!<  type id in case the element type is DATA_TABLE_FEATURE */
    data_classifier_type_t classifier;  /*!<  type id in case the element type is DATA_TABLE_CLASSIFIER */
    data_diagram_type_t diagram;  /*!<  type id in case the element type is DATA_TABLE_DIAGRAM */
    int as_int;  /*!<  type id in case the element type is unknown/not of interest */
};

/*!
 *  \brief attributes of the type resource
 */
struct gui_type_resource_struct {
    data_table_t context;  /*!<  element type for which this type resource applies */
    union gui_type_resource_union type;  /*!<  type id for which this type resource applies */
    const char * name;  /*!<  display name */
    const GdkPixbuf * icon;  /*!<  icon to be displayed */
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
                                                    const GdkPixbuf * icon
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
                                                       const GdkPixbuf * icon
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
                                                    const GdkPixbuf * icon
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
                                                         const GdkPixbuf * icon
                                                       );

/*!
 *  \brief destroys the gui_type_resource_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_type_resource_destroy ( gui_type_resource_t *this_ );

/*!
 *  \brief gets the context
 *
 *  \param this_ pointer to own object attributes
 *  \return context of this gui_type_resource_t
 */
static inline data_table_t gui_type_resource_get_context ( const gui_type_resource_t *this_ );

/*!
 *  \brief gets the type
 *
 *  \param this_ pointer to own object attributes
 *  \return type of this gui_type_resource_t
 */
static inline union gui_type_resource_union gui_type_resource_get_type ( const gui_type_resource_t *this_ );

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
 *  \param this_ pointer to own object attributes
 *  \return icon of this gui_type_resource_t
 */
static inline const GdkPixbuf * gui_type_resource_get_icon ( const gui_type_resource_t *this_ );

#include "gui_type_resource.inl"

#endif  /* GUI_TYPE_RESOURCE_H */


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
