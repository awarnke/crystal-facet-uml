/* File: gui_attributes_editor_types_types.h; Copyright and License: see below */

#ifndef GUI_ATTRIBUTES_EDITOR_TYPES_H
#define GUI_ATTRIBUTES_EDITOR_TYPES_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Provides type lists to be displayed/used by gui_attributes_editor
 */

#include "gui_resources.h"
#include "data_classifier.h"
#include "data_diagram.h"
#include "data_feature.h"
#include "data_relationship.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the gui_attributes_editor_types_t
 */
struct gui_attributes_editor_types_struct {
    gui_resources_t *resources;  /*!< pointer to external resource provider */

    GtkListStore *no_types;  /* a list representing only n/a */
    GtkListStore *diagram_types;
    GtkListStore *classifier_types;
    GtkListStore *relationship_types;
    GtkListStore *feature_types;
    GtkListStore *feature_lifeline_type;
};

typedef struct gui_attributes_editor_types_struct gui_attributes_editor_types_t;

/*!
 *  \brief initializes the gui_attributes_editor_types_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param resources pointer to a resource provider
 */
void gui_attributes_editor_types_init ( gui_attributes_editor_types_t *this_, gui_resources_t *resources );

/*!
 *  \brief destroys the gui_attributes_editor_types_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_attributes_editor_types_destroy ( gui_attributes_editor_types_t *this_ );

/*!
 *  \brief fetches the undef type names
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to a GtkListStore that holds the names of undef types
 */
static inline const GtkListStore* gui_attributes_editor_types_get_undef ( const gui_attributes_editor_types_t *this_ );

/*!
 *  \brief fetches the diagram type names
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to a GtkListStore that holds the names of diagram types
 */
static inline const GtkListStore* gui_attributes_editor_types_get_diagrams ( const gui_attributes_editor_types_t *this_ );

/*!
 *  \brief fetches the classifier type names
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to a GtkListStore that holds the names of classifier types
 */
static inline const GtkListStore* gui_attributes_editor_types_get_classifiers ( const gui_attributes_editor_types_t *this_ );

/*!
 *  \brief fetches the relationship type names
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to a GtkListStore that holds the names of relationship types
 */
static inline const GtkListStore* gui_attributes_editor_types_get_relationships ( const gui_attributes_editor_types_t *this_ );

/*!
 *  \brief fetches the feature type names, except lifeline
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to a GtkListStore that holds the names of feature types
 */
static inline const GtkListStore* gui_attributes_editor_types_get_features ( const gui_attributes_editor_types_t *this_ );

/*!
 *  \brief fetches the feature_lifeline type name
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to a GtkListStore that holds the name of feature_lifeline type
 */
static inline const GtkListStore* gui_attributes_editor_types_get_feature_lifeline ( const gui_attributes_editor_types_t *this_ );

#include "gui_attributes_editor_types.inl"

#endif  /* GUI_ATTRIBUTES_EDITOR_TYPES_H */


/*
Copyright 2016-2020 Andreas Warnke

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
