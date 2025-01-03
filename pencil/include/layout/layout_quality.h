/* File: layout_quality.h; Copyright and License: see below */

#ifndef LAYOUT_QUALITY_H
#define LAYOUT_QUALITY_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Represents an order value for an object, either a list order or an x-/y- order
 */

#include <stdint.h>

/*!
 *  \brief enumeration of pencil internal order types
 */
enum layout_quality_check_enum {
    LAYOUT_QUALITY_CHECK_SHAPE_TO_SHAPE,  /*!< only the bounding box or connector line shall be checked against */
    LAYOUT_QUALITY_CHECK_SHAPE_TO_ALL,  /*!< only the bounding box or connector line shall be checked */
    LAYOUT_QUALITY_CHECK_ALL_TO_ALL,  /*!< all parts shall be ckecked */
};

typedef enum layout_quality_check_enum layout_quality_check_t;

/*!
 *  \brief attributes of the layout_quality
 */
struct layout_quality_struct {
    layout_quality_check_t check_criteria;  /*!< defines the criteria that shall be checked */
};

typedef struct layout_quality_struct layout_quality_t;

/*!
 *  \brief initializes the layout_quality_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param check_criteria criteria that shall be checked
 */
static inline void layout_quality_init ( layout_quality_t *this_, layout_quality_check_t check_criteria );

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
 *  \param environment object to which overlaps shall be determined
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_class_diag ( const layout_quality_t *this_,
                                                       const layout_visible_classifier_t *probe,
                                                       const layout_diagram_t *environment
                                                     );

/*!
 *  \brief determines the quality debts if both visible classifiers are drawn into the same diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a completely layouted visible classifier
 *  \param environment object to which overlaps shall be determined
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_class_class ( const layout_quality_t *this_,
                                                        const layout_visible_classifier_t *probe,
                                                        const layout_visible_classifier_t *environment
                                                      );

/*!
 *  \brief determines the quality debts if the feature is drawn to the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly or completely layouted feature
 *  \param environment object to which overlaps shall be determined
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_feat_diag ( const layout_quality_t *this_,
                                                      const layout_feature_t *probe,
                                                      const layout_diagram_t *environment
                                                    );

/*!
 *  \brief determines the quality debts if the feature and the visible classifier are drawn into the same diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly or completely layouted feature
 *  \param environment object to which overlaps shall be determined
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_feat_class ( const layout_quality_t *this_,
                                                       const layout_feature_t *probe,
                                                       const layout_visible_classifier_t *environment
                                                     );

/*!
 *  \brief determines the quality debts if both features are drawn into the same diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly or completely layouted feature
 *  \param environment object to which overlaps shall be determined
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_feat_feat ( const layout_quality_t *this_,
                                                      const layout_feature_t *probe,
                                                      const layout_feature_t *environment
                                                    );

/*!
 *  \brief determines the quality debts if the relationship is drawn to the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly or completely layouted relationship
 *  \param environment object to which overlaps shall be determined
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_rel_diag ( const layout_quality_t *this_,
                                                     const layout_relationship_t *probe,
                                                     const layout_diagram_t *environment
                                                   );

/*!
 *  \brief determines the quality debts if the relationship and the visible classifier are drawn into the same diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly or completely layouted relationship
 *  \param environment object to which overlaps shall be determined
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_rel_class ( const layout_quality_t *this_,
                                                      const layout_relationship_t *probe,
                                                      const layout_visible_classifier_t *environment
                                                    );

/*!
 *  \brief determines the quality debts if the relationship and the feature are drawn into the same diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly or completely layouted relationship
 *  \param environment object to which overlaps shall be determined
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_rel_feat ( const layout_quality_t *this_,
                                                     const layout_relationship_t *probe,
                                                     const layout_feature_t *environment
                                                   );

/*!
 *  \brief determines the quality debts if both relationships are drawn into the same diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param probe a partly or completely layouted relationship
 *  \param environment object to which overlaps shall be determined
 *  \return 0.0 if there are no overlaps, a positive value otherwise
 */
static inline double layout_quality_debts_rel_rel ( const layout_quality_t *this_,
                                                    const layout_relationship_t *probe,
                                                    const layout_relationship_t *environment
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
