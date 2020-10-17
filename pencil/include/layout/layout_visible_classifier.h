/* File: layout_visible_classifier.h; Copyright and License: see below */

#ifndef LAYOUT_VISIBLE_CLASSIFIER_H
#define LAYOUT_VISIBLE_CLASSIFIER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines shape and location of a visible_classifier
 */

#include "util/geometry/geometry_rectangle.h"
#include "util/geometry/geometry_h_align.h"
#include "util/geometry/geometry_v_align.h"
#include "set/data_visible_classifier.h"

/*!
 *  \brief shape and location of a visible_classifier
 */
struct layout_visible_classifier_struct {
    geometry_rectangle_t symbol_box;  /*!< symbol_box of a classifier shape. The label_box may be outside of the symbol_box. symbol_box is stated in absolute coordinates. */
    geometry_rectangle_t space;  /*!< inner space of a classifier record where properties, operations or contained classifiers are drawn. Absolute coordinates. */
    geometry_rectangle_t label_box;  /*!< bounding box of the label of the visible classifier. label_box is stated in absolute coordinates. */
    geometry_h_align_t label_h_anchor;  /*!< side of the label box that stays fix in case of expand */
    geometry_v_align_t label_v_anchor;  /*!< side of the label box that stays fix in case of expand */
    const data_visible_classifier_t *data;  /*!< pointer to the data object of the visible classifier */
};

typedef struct layout_visible_classifier_struct layout_visible_classifier_t;

/*!
 *  \brief initializes the layout_visible_classifier_t struct with a pointer to the corresponding data_visible_classifier_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_classifier_data pointer to data object which is layouted
 */
static inline void layout_visible_classifier_init ( layout_visible_classifier_t *this_, const data_visible_classifier_t *visible_classifier_data );

/*!
 *  \brief destroys the layout_visible_classifier_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_visible_classifier_destroy ( layout_visible_classifier_t *this_ );

/*!
 *  \brief checks if the layout_visible_classifier data is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if (*this_).data points to valid data
 */
static inline bool layout_visible_classifier_is_valid ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the symbol_box of the classifier
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_rectangle_t *layout_visible_classifier_get_symbol_box_ptr ( layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the symbol_box of the classifier
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline const geometry_rectangle_t *layout_visible_classifier_get_symbol_box_const ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief sets the symbol_box of the classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param symbol_box coordinates of new symbol_box
 */
static inline void layout_visible_classifier_set_symbol_box ( layout_visible_classifier_t *this_, const geometry_rectangle_t *symbol_box );

/*!
 *  \brief gets the inner space of the classifier
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_rectangle_t *layout_visible_classifier_get_space_ptr ( layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the inner space of the classifier
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline const geometry_rectangle_t *layout_visible_classifier_get_space_const ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief sets the inner space  of the classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param space coordinates of new space for contained classifiers and features
 */
static inline void layout_visible_classifier_set_space ( layout_visible_classifier_t *this_, const geometry_rectangle_t *space );

/*!
 *  \brief gets the label bounds of the classifier
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_rectangle_t *layout_visible_classifier_get_label_box_ptr ( layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the label bounds of the classifier
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline const geometry_rectangle_t *layout_visible_classifier_get_label_box_const ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief sets the label bounds of the classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param label_box coordinates of new label bounds
 */
static inline void layout_visible_classifier_set_label_box ( layout_visible_classifier_t *this_, const geometry_rectangle_t *label_box );

/*!
 *  \brief sets the bounding box of the classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param label_h_anchor side of the label that stays fix in case of expand
 *  \param label_v_anchor side of the label that stays fix in case of expand
 */
static inline void layout_visible_classifier_set_label_anchor ( layout_visible_classifier_t *this_,
                                                                geometry_h_align_t label_h_anchor,
                                                                geometry_v_align_t label_v_anchor
                                                              );

/*!
 *  \brief calculates the envelope bounds of the classifier.
 *
 *  The envelope contains the label and the symbol box and implicitly the (inner) space.
 *
 *  \param this_ pointer to own object attributes
 *  \return geometry_rectangle_t ehich is the envelope to label and symbol-box.
 */
static inline geometry_rectangle_t layout_visible_classifier_calc_envelope_box ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief moves the layout_visible_classifier_t
 *
 *  \param this_ pointer to own object attributes
 *  \param delta_x delta-x difference by which to shift the layout_visible_classifier_t
 *  \param delta_y delta-y difference by which to shift the layout_visible_classifier_t
 */
static inline void layout_visible_classifier_shift ( layout_visible_classifier_t *this_, double delta_x, double delta_y );

/*!
 *  \brief expands or shrinks the layout_visible_classifier_t
 *
 *  The top left corner remains fix.
 *
 *  \param this_ pointer to own object attributes
 *  \param delta_width value by which to increase the width. In case of a negative value, the layout_visible_classifier shrinks (but not below 0).
 *  \param delta_height value by which to increase the height. In case of a negative value, the layout_visible_classifier shrinks (but not below 0).
 */
static inline void layout_visible_classifier_expand ( layout_visible_classifier_t *this_, double delta_width, double delta_height );

/*!
 *  \brief gets the pointer to the data_visible_classifier_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to a data_visible_classifier_t object.
 */
static inline const data_visible_classifier_t *layout_visible_classifier_get_data_const ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the pointer to the const classifier
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to a classifier.
 */
static inline const data_classifier_t *layout_visible_classifier_get_classifier_const ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the pointer to the const diagramelement
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to a diagramelement.
 */
static inline const data_diagramelement_t *layout_visible_classifier_get_diagramelement_const ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief returns true if the classifier is drawn as fixed-size symbol
 *
 *  The following classifier types are fixed-sized symbols: (use-case:) actor, (activity:) start, end, fork, join, timeout, history
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the classifier
 */
static inline bool layout_visible_classifier_is_fix_sized_symbol ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the row id of the classifier
 *
 *  This is a shortcut for data_classifier_get_id( data_visible_classifier_get_classifier_const ( (*this_).data ) )
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the classifier
 */
static inline data_row_id_t layout_visible_classifier_get_classifier_id ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the row id of the diagramelement
 *
 *  This is a shortcut for data_diagramelement_get_id( data_visible_classifier_get_diagramelement_const ( (*this_).data ) )
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the diagramelement
 */
static inline data_row_id_t layout_visible_classifier_get_diagramelement_id ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief checks if the ids of the associated diagramelements are identical
 *
 *  This is a shortcut for layout_visible_classifier_get_diagramelement_id( this_ )==layout_visible_classifier_get_diagramelement_id( that )
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to others object attributes
 *  \return true if ids of the associated diagramelements are identical
 */
static inline bool layout_visible_classifier_is_equal_diagramelement_id ( const layout_visible_classifier_t *this_,
                                                                          const layout_visible_classifier_t *that
                                                                        );

#include "layout_visible_classifier.inl"

#endif  /* LAYOUT_VISIBLE_CLASSIFIER_H */


/*
Copyright 2018-2020 Andreas Warnke

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
