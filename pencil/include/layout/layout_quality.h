/* File: layout_quality.h; Copyright and License: see below */

#ifndef LAYOUT_QUALITY_H
#define LAYOUT_QUALITY_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines functinos to measure the layouting quality of a diagram.
 *
 *  This measurement can be performed during layouting to select the best option
 *  or afterwards to determine the overall layouting quality.
 */

#include "pencil_size.h"
#include <stdint.h>

/*!
 *  \brief attributes of the layout_quality
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
 *  \brief determines the quality debts if the visible classifier is drawn to the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a completely layouted visible classifier
 *  \param other the diagram to which to draw to
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_class_diag ( const layout_quality_t *this_,
                                                       const layout_visible_classifier_t *probe,
                                                       const layout_diagram_t *other
                                                     );

/*!
 *  \brief determines the quality debts for drawing both visible classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a completely layouted visible classifier
 *  \param other another completely layouted visible classifier
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_class_class ( const layout_quality_t *this_,
                                                        const layout_visible_classifier_t *probe,
                                                        const layout_visible_classifier_t *other
                                                      );

/*!
 *  \brief determines the quality debts if the feature is drawn to the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a completely layouted feature
 *  \param other the diagram to which to draw to
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_feat_diag ( const layout_quality_t *this_,
                                                      const layout_feature_t *probe,
                                                      const layout_diagram_t *other
                                                    );

/*!
 *  \brief determines the quality debts for drawing the feature and the visible classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a completely layouted feature
 *  \param other a completely layouted visible classifier
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_feat_class ( const layout_quality_t *this_,
                                                       const layout_feature_t *probe,
                                                       const layout_visible_classifier_t *other
                                                     );

/*!
 *  \brief determines the quality debts for drawing both features
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a completely layouted feature
 *  \param other another completely layouted feature
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_feat_feat ( const layout_quality_t *this_,
                                                      const layout_feature_t *probe,
                                                      const layout_feature_t *other
                                                    );

/*!
 *  \brief determines the quality debts if the relationship is drawn to the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a completely layouted relationship
 *  \param other the diagram to which to draw to
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_rel_diag ( const layout_quality_t *this_,
                                                     const layout_relationship_t *probe,
                                                     const layout_diagram_t *other
                                                   );

/*!
 *  \brief determines the quality debts for drawing the relationship and the visible classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a completely layouted relationship
 *  \param other a completely layouted visible classifier
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_rel_class ( const layout_quality_t *this_,
                                                      const layout_relationship_t *probe,
                                                      const layout_visible_classifier_t *other
                                                    );

/*!
 *  \brief determines the quality debts for drawing the relationship and the feature
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a completely layouted relationship
 *  \param other a completely layouted feature
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_rel_feat ( const layout_quality_t *this_,
                                                     const layout_relationship_t *probe,
                                                     const layout_feature_t *other
                                                   );

/*!
 *  \brief determines the quality debts if both relationships are drawn into the same diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a completely layouted relationship
 *  \param other another completely layouted relationship
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_rel_rel ( const layout_quality_t *this_,
                                                    const layout_relationship_t *probe,
                                                    const layout_relationship_t *other
                                                  );

/*!
 *  \brief determines the quality debts for drawing both symbol boxes of features
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly layouted feature
 *  \param other another partly layouted feature
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_sym_sym ( const layout_quality_t *this_,
                                                    const geometry_rectangle_t *probe,
                                                    const geometry_rectangle_t *other
                                                  );

/*!
 *  \brief determines the quality debts if the connector of a relationship is drawn to the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly layouted relationship
 *  \param other the diagram to which to draw to
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_conn_diag ( const layout_quality_t *this_,
                                                      const geometry_connector_t *probe,
                                                      const layout_diagram_t *other
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
                                                      const geometry_rectangle_t *other
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
