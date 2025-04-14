/* File: layout_quality.h; Copyright and License: see below */

#ifndef LAYOUT_QUALITY_H
#define LAYOUT_QUALITY_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines functions to measure the layouting quality of a diagram.
 *
 *  This measurement can be performed during layouting to select the best option
 *  or afterwards to determine the overall layouting quality.
 */

#include "pencil_size.h"
#include "geometry/geometry_offset.h"
#include <stdint.h>

/*!
 *  \brief attributes of the layout_quality
 *
 *  These attributes are needed to determine the layout quality.
 *
 *  The methods of layout_quality_t return a debt value that is zero if the layout is perfect,
 *  positive if there are unwanted overlaps of objects or other suboptimal characteristics.
 *
 *  A debt value is roughly linear to the 2-dimensional area ( width * height ) that is affected.
 *  In some less important cases, it is linear to a distance ( which equals a width of 1.0 ).
 *  A weight factor ( LAYOUT_QUALITY_WEIGHT_* ) reflects the severity of the layouting defect.
 */
struct layout_quality_struct {
    const pencil_size_t *pencil_size;  /*!< the pencil size proposes preferred distances */
};

typedef struct layout_quality_struct layout_quality_t;

/*!
 *  \brief initializes the layout_quality_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param pencil_size proposal for preferred distances
 */
static inline void layout_quality_init ( layout_quality_t *this_, const pencil_size_t *pencil_size );

/*!
 *  \brief initializes a layout_quality_t struct
 *
 *  \param pencil_size proposal for preferred distances
 *  \return own object attributes, may be used to initialize a const layout_quality_t
 */
static inline layout_quality_t layout_quality_new ( const pencil_size_t *pencil_size );

/*!
 *  \brief destroys the layout_quality_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_quality_destroy ( layout_quality_t *this_ );

/*!
 *  \brief determines the quality debts for drawing the visible classifier to the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a completely layouted visible classifier
 *  \param order_target the delta from order-based location to the current classifiers location
 *  \param other the diagram to which to draw to
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_class_diag ( const layout_quality_t *this_,
                                                       const layout_visible_classifier_t *probe,
                                                       const geometry_offset_t *order_target,
                                                       const layout_diagram_t *other
                                                     );

/*!
 *  \brief determines the quality debts for drawing both visible classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a completely layouted visible classifier
 *  \param other another completely layouted visible classifier
 *  \param layout_data the visible set allows to determine if probe is parent or child of other
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_class_class ( const layout_quality_t *this_,
                                                        const layout_visible_classifier_t *probe,
                                                        const layout_visible_classifier_t *other,
                                                        const layout_visible_set_t *layout_data
                                                      );

/*!
 *  \brief determines the quality debts for drawing the connector of a relationship to the diagram
 *
 *  This function considers the coordinates of the diagram and the ones of the relationship.
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly layouted relationship
 *  \param source_rect the rectangle from which the connector shall start
 *  \param dest_rect the rectangle at which the connector shall end
 *  \param other the diagram to which to draw to
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_conn_diag ( const layout_quality_t *this_,
                                                      const geometry_connector_t *probe,
                                                      const geometry_rectangle_t *source_rect,
                                                      const geometry_rectangle_t *dest_rect,
                                                      const layout_diagram_t *other
                                                    );

/*!
 *  \brief determines the quality debts for drawing the connector of a relationship and the visible classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly layouted relationship
 *  \param other a completely layouted visible classifier
 *  \param is_ancestor_of_source other is an ancestor of the probe's source end (from)
 *  \param is_ancestor_of_destination other is an ancestor of the probe's destination end (to)
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_conn_class ( const layout_quality_t *this_,
                                                       const geometry_connector_t *probe,
                                                       const layout_visible_classifier_t *other,
                                                       const bool is_ancestor_of_source,
                                                       const bool is_ancestor_of_destination
                                                     );

/*!
 *  \brief determines the quality debts for drawing the connector of a relationship and the symbol box of a feature
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly layouted relationship
 *  \param other a partly layouted feature
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_conn_sym ( const layout_quality_t *this_,
                                                     const geometry_connector_t *probe,
                                                     const geometry_rectangle_t *other
                                                   );

/*!
 *  \brief determines the quality debts for drawing both connectors of relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly layouted relationship
 *  \param other another partly layouted relationship
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_conn_conn ( const layout_quality_t *this_,
                                                      const geometry_connector_t *probe,
                                                      const geometry_connector_t *other
                                                    );

/*!
 *  \brief determines the quality debts for drawing the label to the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly layouted feature or relationship
 *  \param target_point the point where the object is located that the label refers to
 *  \param other the diagram to which to draw to
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_label_diag ( const layout_quality_t *this_,
                                                       const geometry_rectangle_t *probe,
                                                       const geometry_point_t *target_point,
                                                       const layout_diagram_t *other
                                                     );

/*!
 *  \brief determines the quality debts for drawing the label and the visible classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly layouted feature or relationship
 *  \param other a completely layouted visible classifier
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_label_class ( const layout_quality_t *this_,
                                                        const geometry_rectangle_t *probe,
                                                        const layout_visible_classifier_t *other
                                                      );

/*!
 *  \brief determines the quality debts for drawing the label and the feature
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly layouted feature or relationship
 *  \param other a completely layouted feature
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_label_feat ( const layout_quality_t *this_,
                                                       const geometry_rectangle_t *probe,
                                                       const layout_feature_t *other
                                                     );

/*!
 *  \brief determines the quality debts for drawing the label and the relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly layouted feature or relationship
 *  \param other a completely layouted relationship
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_label_rel ( const layout_quality_t *this_,
                                                      const geometry_rectangle_t *probe,
                                                      const layout_relationship_t *other
                                                    );


#include "layout_quality.inl"

#endif  /* LAYOUT_QUALITY_H */


/*
Copyright 2025-2025 Andreas Warnke

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
