/* File: gui_type_resource.h; Copyright and License: see below */

#ifndef GUI_TYPE_RESOURCE_H
#define GUI_TYPE_RESOURCE_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Data objects needed to display an element type: name and icon
 *
 *  This data object is a gobject to be able to store it in a GListModel like GListStore.
 */

#include "entity/data_table.h"
#include "entity/data_relationship_type.h"
#include "entity/data_feature_type.h"
#include "entity/data_classifier_type.h"
#include "entity/data_diagram_type.h"
#include "entity/data_type.h"
#include <gdk/gdk.h>
#include <glib-object.h>
#include <stdint.h>

G_BEGIN_DECLS

/*
 * Type declaration.
 */
#define GUI_TYPE_TYPE_RESOURCE gui_type_resource_get_type()
G_DECLARE_FINAL_TYPE (GuiTypeResource, gui_type_resource, GUI, TYPE_RESOURCE, GObject)

/*!
 *  \brief attributes of the type resource
 *
 *  The notation of the struct name is required by glib
 */
struct _GuiTypeResource {
    GObject parent_instance;  /*!<  glib parent object */
    data_type_t type_id;  /*!<  element type for which this type resource applies */
    const char * name;  /*!<  display name */
    GdkTexture * icon;  /*!<  icon to be displayed */
};

/*
 * Method definitions.
 */
GuiTypeResource *gui_type_resource_new (void);

G_END_DECLS

typedef struct _GuiTypeResource gui_type_resource_t;

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
                                                    GdkTexture * icon
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
                                                       GdkTexture * icon
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
                                                    GdkTexture * icon
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
                                                         GdkTexture * icon
                                                       );

/*!
 *  \brief initializes the gui_type_resource_t struct with a copy
 *
 *  parent_instance is not modified.
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes to copy
 */
static inline void gui_type_resource_replace ( gui_type_resource_t *this_, const gui_type_resource_t *original );

/*!
 *  \brief destroys the gui_type_resource_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_type_resource_destroy ( gui_type_resource_t *this_ );

/*!
 *  \brief checks if two gui_type_resource_t are equal
 *
 *  This function can be used as type GEqualFunc
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to other object attributes
 *  \return TRUE if equal
 */
gboolean gui_type_resource_equal( const gui_type_resource_t *this_, const gui_type_resource_t *that );

/*!
 *  \brief gets the type
 *
 *  \param this_ pointer to own object attributes
 *  \return type_id of this gui_type_resource_t
 */
static inline const data_type_t * gui_type_resource_get_type_id ( const gui_type_resource_t *this_ );

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
 *  GdkTexture cannot be const because most gtk functions require mutable GdkTexture as parameter.
 *
 *  \param this_ pointer to own object attributes
 *  \return icon of this gui_type_resource_t
 */
static inline GdkTexture * gui_type_resource_get_icon ( const gui_type_resource_t *this_ );

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
