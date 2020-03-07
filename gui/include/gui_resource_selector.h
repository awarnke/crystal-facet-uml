/* File: gui_resource_selector.h; Copyright and License: see below */

#ifndef GUI_RESOURCE_SELECTOR_H
#define GUI_RESOURCE_SELECTOR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Selects a resource given an identifier or type
 */

#include "gui_resources.h"
#include "gui_type_resource.h"
#include "data_table.h"
#include "data_relationship_type.h"
#include "data_feature_type.h"
#include "data_diagram_type.h"
#include "data_classifier_type.h"

/*!
 *  \brief enumeration on tools
 */
enum gui_resource_selector_max_enum {
    GUI_RESOURCE_SELECTOR_MAX_TYPES = 74,
};

/*!
 *  \brief attributes of the gui_resource_selector_t
 */
struct gui_resource_selector_struct {
    gui_resources_t *resources;
    gui_type_resource_t type_name_icon_list[GUI_RESOURCE_SELECTOR_MAX_TYPES];
    unsigned int diagram_types_start;
    unsigned int diagram_types_length;
    unsigned int feature_types_start;
    unsigned int feature_types_length;
    unsigned int classifier_types_start;
    unsigned int classifier_types_length;
    unsigned int relationship_types_start;
    unsigned int relationship_types_length;
};

typedef struct gui_resource_selector_struct gui_resource_selector_t;

/*!
 *  \brief initializes the gui_resource_selector_t
 *
 *  \param resources pointer to resources
 *  \param this_ pointer to own object attributes
 */
void gui_resource_selector_init ( gui_resource_selector_t *this_, gui_resources_t *resources );

/*!
 *  \brief destroys the gui_resource_selector_t
 *
 *  \param this_ pointer to own object attributes
 */
void gui_resource_selector_destroy ( gui_resource_selector_t *this_ );

/*!
 *  \brief gets the icon for the specified element type
 *
 *  \param table identifier for the table
 *  \param type the element type for which to retrieve the icon
 *  \param this_ pointer to the specified icon or &type_undef if unknown table or type
 */
const GdkPixbuf *gui_resource_selector_get_icon ( const gui_resource_selector_t *this_, data_table_t table, int type );

/*!
 *  \brief gets the icon for the specified diagram type
 *
 *  \param diag_type the diagram type for which to retrieve the icon
 *  \param this_ pointer to the specified icon or &type_undef if unknown diag_type
 */
static inline const GdkPixbuf *gui_resource_selector_get_diagram_icon ( const gui_resource_selector_t *this_, data_diagram_type_t diag_type );

/*!
 *  \brief gets the icon for the specified classifier type
 *
 *  \param clas_type the classifier type for which to retrieve the icon
 *  \param this_ pointer to the specified icon or &type_undef if unknown clas_type
 */
static inline const GdkPixbuf *gui_resource_selector_get_classifier_icon ( const gui_resource_selector_t *this_, data_classifier_type_t clas_type );

/*!
 *  \brief gets the icon for the specified feature type
 *
 *  \param feat_type the feature type for which to retrieve the icon
 *  \param this_ pointer to the specified icon or &type_undef if unknown feat_type
 */
static inline const GdkPixbuf *gui_resource_selector_get_feature_icon ( const gui_resource_selector_t *this_, data_feature_type_t feat_type );

/*!
 *  \brief gets the icon for the specified relationship type
 *
 *  \param rel_type the relationship type for which to retrieve the icon
 *  \param this_ pointer to the specified icon or &type_undef if unknown rel_type
 */
static inline const GdkPixbuf *gui_resource_selector_get_relationship_icon ( const gui_resource_selector_t *this_, data_relationship_type_t rel_type );

#include "gui_resource_selector.inl"

#endif  /* GUI_RESOURCE_SELECTOR_H */


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
