/* File: gui_type_resource_list.h; Copyright and License: see below */

#ifndef GUI_TYPE_RESOURCE_LIST_H
#define GUI_TYPE_RESOURCE_LIST_H

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
enum gui_type_resource_list_max_enum {
    GUI_TYPE_RESOURCE_LIST_MAX_TYPES = 80,
};

/* all diagram types, no exclusions */
#define GUI_TYPE_RESOURCE_LIST_DIAGRAMS ( DATA_DIAGRAM_TYPE_COUNT )
/* all classifier types, exclusions: DATA_CLASSIFIER_TYPE_INTERACTION */
#define GUI_TYPE_RESOURCE_LIST_CLASSIFIERS ( DATA_CLASSIFIER_TYPE_COUNT - 1 )
/* all feature types, no exclusions */
/* separated to invariant features and scenario-specific features (DATA_FEATURE_TYPE_LIFELINE) */
#define GUI_TYPE_RESOURCE_LIST_INV_FEATURES ( DATA_FEATURE_TYPE_COUNT - 1 )
#define GUI_TYPE_RESOURCE_LIST_SCENARIOS ( 1 )
#define GUI_TYPE_RESOURCE_LIST_FEATURES ( GUI_TYPE_RESOURCE_LIST_INV_FEATURES + GUI_TYPE_RESOURCE_LIST_SCENARIOS )
/* all diagram types, no exclusions */
#define GUI_TYPE_RESOURCE_LIST_RELATIONS ( DATA_RELATIONSHIP_TYPE_COUNT )

/*!
 *  \brief attributes of the gui_type_resource_list_t
 */
struct gui_type_resource_list_struct {
    gui_resources_t *resources;
    gui_type_resource_t type_name_icon_list[GUI_TYPE_RESOURCE_LIST_MAX_TYPES];
    gui_type_resource_t type_undef;
    unsigned int diagram_types_start;
    unsigned int diagram_types_length;
    unsigned int feature_types_start;
    unsigned int feature_types_length;
    unsigned int classifier_types_start;
    unsigned int classifier_types_length;
    unsigned int relationship_types_start;
    unsigned int relationship_types_length;

    GListStore *no_types;  /* a list representing only n/a */
    GListStore *diagram_types;
    GListStore *classifier_types;
    GListStore *relationship_types;
    GListStore *feature_types;  /* a list representing features except lifelines */
    GListStore *feature_lifeline_type;
};

typedef struct gui_type_resource_list_struct gui_type_resource_list_t;

/*!
 *  \brief initializes the gui_type_resource_list_t
 *
 *  \param this_ pointer to own object attributes
 *  \param resources pointer to resources
 */
void gui_type_resource_list_init ( gui_type_resource_list_t *this_, gui_resources_t *resources );

/*!
 *  \brief destroys the gui_type_resource_list_t
 *
 *  \param this_ pointer to own object attributes
 */
void gui_type_resource_list_destroy ( gui_type_resource_list_t *this_ );

/*!
 *  \brief gets the icon for the specified element type
 *
 *  \param this_ pointer to own object attributes
 *  \param table identifier for the table
 *  \param type the element type for which to retrieve the icon
 *  \return pointer to the requested gui_type_resource_t struct or &type_undef if unknown table or type
 */
gui_type_resource_t * gui_type_resource_list_get_type ( gui_type_resource_list_t *this_,
                                                        data_table_t table,
                                                        int type
                                                      );

/*!
 *  \brief gets the icon for the specified diagram type
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_type the diagram type for which to retrieve the icon
 *  \return pointer to the requested gui_type_resource_t struct or &type_undef if unknown diag_type
 */
static inline gui_type_resource_t * gui_type_resource_list_get_diagram_type ( gui_type_resource_list_t *this_,
                                                                              data_diagram_type_t diag_type
                                                                            );

/*!
 *  \brief gets the icon for the specified classifier type
 *
 *  \param this_ pointer to own object attributes
 *  \param clas_type the classifier type for which to retrieve the icon
 *  \return pointer to the requested gui_type_resource_t struct or &type_undef if unknown clas_type
 */
static inline gui_type_resource_t * gui_type_resource_list_get_classifier_type ( gui_type_resource_list_t *this_,
                                                                                 data_classifier_type_t clas_type
                                                                               );

/*!
 *  \brief gets the icon for the specified feature type
 *
 *  \param this_ pointer to own object attributes
 *  \param feat_type the feature type for which to retrieve the icon
 *  \return pointer to the requested gui_type_resource_t struct or &type_undef if unknown feat_type
 */
static inline gui_type_resource_t * gui_type_resource_list_get_feature_type ( gui_type_resource_list_t *this_,
                                                                              data_feature_type_t feat_type
                                                                            );

/*!
 *  \brief gets the icon for the specified relationship type
 *
 *  \param this_ pointer to own object attributes
 *  \param rel_type the relationship type for which to retrieve the icon
 *  \return pointer to the requested gui_type_resource_t struct or &type_undef if unknown rel_type
 */
static inline gui_type_resource_t * gui_type_resource_list_get_relationship_type ( gui_type_resource_list_t *this_,
                                                                                   data_relationship_type_t rel_type
                                                                                 );

/*!
 *  \brief gets a pointer to an array of gui_type_resource_t containing diagram types
 *
 *  \param this_ pointer to own object attributes
 *  \param out_types a memory address where to store a pointer to an array of gui_type_resource_t
 *  \param out_length a memory address where to store the length of the array
 */
static inline void gui_type_resource_list_get_all_diagram_types ( const gui_type_resource_list_t *this_,
                                                                  const gui_type_resource_t (**out_types)[],
                                                                  unsigned int *out_length
                                                                );

/*!
 *  \brief gets a pointer to an array of gui_type_resource_t containing classifier types
 *
 *  \param this_ pointer to own object attributes
 *  \param out_types a memory address where to store a pointer to an array of gui_type_resource_t
 *  \param out_length a memory address where to store the length of the array
 */
static inline void gui_type_resource_list_get_all_classifier_types ( const gui_type_resource_list_t *this_,
                                                                     const gui_type_resource_t (**out_types)[],
                                                                     unsigned int *out_length
                                                                   );

/*!
 *  \brief gets a pointer to an array of gui_type_resource_t containing feature types
 *
 *  \param this_ pointer to own object attributes
 *  \param out_types a memory address where to store a pointer to an array of gui_type_resource_t
 *  \param out_length a memory address where to store the length of the array
 */
static inline void gui_type_resource_list_get_all_feature_types ( const gui_type_resource_list_t *this_,
                                                                  const gui_type_resource_t (**out_types)[],
                                                                  unsigned int *out_length
                                                                );

/*!
 *  \brief gets a pointer to an array of gui_type_resource_t containing invariant feature types
 *
 *  The scenario-specific type DATA_FEATURE_TYPE_LIFELINE is excluded.
 *
 *  \param this_ pointer to own object attributes
 *  \param out_types a memory address where to store a pointer to an array of gui_type_resource_t
 *  \param out_length a memory address where to store the length of the array
 */
static inline void gui_type_resource_list_get_invariant_feature_types ( const gui_type_resource_list_t *this_,
                                                                        const gui_type_resource_t (**out_types)[],
                                                                        unsigned int *out_length
                                                                      );

/*!
 *  \brief gets a pointer to an array of gui_type_resource_t containing relationship types
 *
 *  \param this_ pointer to own object attributes
 *  \param out_types a memory address where to store a pointer to an array of gui_type_resource_t
 *  \param out_length a memory address where to store the length of the array
 */
static inline void gui_type_resource_list_get_all_relationship_types ( const gui_type_resource_list_t *this_,
                                                                       const gui_type_resource_t (**out_types)[],
                                                                       unsigned int *out_length
                                                                     );

/*!
 *  \brief fetches the undef type names
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to a GListStore that holds the names of undef types
 */
static inline GListStore* gui_type_resource_list_get_undef ( const gui_type_resource_list_t *this_ );

/*!
 *  \brief fetches the diagram type names
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to a GListStore that holds the names of diagram types
 */
static inline GListStore* gui_type_resource_list_get_diagrams ( const gui_type_resource_list_t *this_ );

/*!
 *  \brief fetches the classifier type names
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to a GListStore that holds the names of classifier types
 */
static inline GListStore* gui_type_resource_list_get_classifiers ( const gui_type_resource_list_t *this_ );

/*!
 *  \brief fetches the relationship type names
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to a GListStore that holds the names of relationship types
 */
static inline GListStore* gui_type_resource_list_get_relationships ( const gui_type_resource_list_t *this_ );

/*!
 *  \brief fetches the feature type names, except lifeline
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to a GListStore that holds the names of feature types (except lifeline)
 */
static inline GListStore* gui_type_resource_list_get_features ( const gui_type_resource_list_t *this_ );

/*!
 *  \brief fetches the feature_lifeline type name
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to a GListStore that holds the name of feature_lifeline type
 */
static inline GListStore* gui_type_resource_list_get_feature_lifeline ( const gui_type_resource_list_t *this_ );

#include "gui_type_resource_list.inl"

#endif  /* GUI_TYPE_RESOURCE_LIST_H */


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
