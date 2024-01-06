/* File: gui_attribute_type_of_feature.h; Copyright and License: see below */

#ifndef GUI_ATTRIBUTE_TYPE_OF_FEATURE_H
#define GUI_ATTRIBUTE_TYPE_OF_FEATURE_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Provides a selected element type and a gui_attributes_editor_t that handles the event
 */

#include "gui_attributes_editor.h"
#include "data_feature_type.h"

/*!
 *  \brief attributes of the gui_attribute_type_of_feature_t
 */
struct gui_attribute_type_of_feature_struct {
    data_feature_type_t selected_type;  /*!< selected type id for a feature */
    gui_attributes_editor_t * editor;  /*!< pointer to external gui_attributes_editor_t */
};

typedef struct gui_attribute_type_of_feature_struct gui_attribute_type_of_feature_t;

/*!
 *  \brief initializes the gui_attribute_type_of_feature_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param selected_type the type that is selected by the button
 *  \param editor pointer to a gui_attributes_editor_t that handles the button event
 */
static inline void gui_attribute_type_of_feature_init ( gui_attribute_type_of_feature_t *this_,
                                                        data_feature_type_t selected_type,
                                                        gui_attributes_editor_t *editor
                                                      );

/*!
 *  \brief destroys the gui_attribute_type_of_feature_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_attribute_type_of_feature_destroy ( gui_attribute_type_of_feature_t *this_ );

/*!
 *  \brief fetches the selected_type
 *
 *  \param this_ pointer to own object attributes
 *  \return the selected_type
 */
static inline data_feature_type_t gui_attribute_type_of_feature_get_selected_type ( const gui_attribute_type_of_feature_t *this_ );

/*!
 *  \brief fetches the gui_attributes_editor_t
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to gui_attributes_editor_t that handles the button event
 */
static inline gui_attributes_editor_t* gui_attribute_type_of_feature_get_editor ( const gui_attribute_type_of_feature_t *this_ );

#include "gui_attribute_type_of_feature.inl"

#endif  /* GUI_ATTRIBUTE_TYPE_OF_FEATURE_H */


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
