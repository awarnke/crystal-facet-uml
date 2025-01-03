/* File: pencil_feature_layouter.h; Copyright and License: see below */

#ifndef PENCIL_FEATURE_LAYOUTER_H
#define PENCIL_FEATURE_LAYOUTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Calculates positions of features in a diagram
 */

#include "pencil_size.h"
#include "layout/layout_visible_set.h"
#include "pencil_feature_painter.h"
#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_dimensions.h"
#include "geometry/geometry_non_linear_scale.h"
#include "entity/data_diagram.h"
#include "set/data_visible_set.h"
#include "set/data_small_set.h"
#include "entity/data_id.h"
#include "data_rules.h"
#include "u8list/universal_array_index_sorter.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the feature layouter
 *
 *  \note This class is stateless. Only the layout_data, pencil_size and profile objects are stateful.
 *        It may either be instantiated once and used many times or be instantiated per use.
 */
struct pencil_feature_layouter_struct {
    layout_visible_set_t *layout_data;  /* pointer to external layout data */
    const data_profile_part_t *profile;  /*!< pointer to an external stereotype-image cache */
    const pencil_size_t *pencil_size;  /*!< pointer to an external pencil_size_t object, */
                                       /*!< defining pen sizes, gap sizes, font sizes and colors */
    data_rules_t rules;  /*!< own instance of modelling rules */
    bool label_dimensions_initialized;  /*!< true if the widths and heights of labels in layout_data are already initialized */

    pencil_feature_painter_t feature_painter;  /*!< own instance of a painter object to ask for display dimensions */
};

typedef struct pencil_feature_layouter_struct pencil_feature_layouter_t;

/*!
 *  \brief initializes the feature layouter
 *
 *  \param this_ pointer to own object attributes
 *  \param layout_data pointer to the layout information to be used and modified
 *  \param profile pointer to the profile-part that provides the stereotypes of the elements to be layouted
 *  \param pencil_size pointer to the pencil_size_t object
 */
void pencil_feature_layouter_init( pencil_feature_layouter_t *this_,
                                   layout_visible_set_t *layout_data,
                                   const data_profile_part_t *profile,
                                   const pencil_size_t *pencil_size
                                 );

/*!
 *  \brief resets the label_dimensions_initialized flag to false
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_feature_layouter_reset( pencil_feature_layouter_t *this_ );

/*!
 *  \brief destroys the feature layouter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_feature_layouter_destroy( pencil_feature_layouter_t *this_ );

/*!
 *  \brief determines the bounding boxes of the features
 *
 *  \param this_ pointer to own object attributes
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 */
void pencil_feature_layouter_do_layout ( pencil_feature_layouter_t *this_, PangoLayout *font_layout );

/*!
 *  \brief calculates feature bounding dimensions of property and operation features for one diagramelement
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_id id of the diagramelement for which to calculate the feature bounds
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param[out] out_features_bounds memory location where the result shall be stored. Must not be NULL.
 */
void pencil_feature_layouter_calculate_features_bounds ( pencil_feature_layouter_t *this_,
                                                         data_row_t diagramelement_id,
                                                         PangoLayout *font_layout,
                                                         geometry_dimensions_t *out_features_bounds
                                                       );

/*!
 *  \brief calculates the label dimensions of all features.
 *
 *  This allows to arrange(move) the features in a subsequent step.
 *
 *  \param this_ pointer to own object attributes
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 */
void pencil_feature_layouter_private_init_label_dimensions( pencil_feature_layouter_t *this_,
                                                            PangoLayout *font_layout
                                                          );

/*!
 *  \brief determines the symbol box of a lifeline
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_space drawing space of the diagram
 *  \param diagram_type diagram type of the diagram
 *  \param classifier_type type of the parent classifier
 *  \param classifier_symbol_box symbol box of the classifier
 *  \param classifier_envelope_box envelope box of the classifier
 *  \param[out] out_feature_layout output parameter: feature layout coordinates
 */
void pencil_feature_layouter_private_layout_lifeline ( pencil_feature_layouter_t *this_,
                                                       const geometry_rectangle_t *diagram_space,
                                                       data_diagram_type_t diagram_type,
                                                       data_classifier_type_t classifier_type,
                                                       const geometry_rectangle_t *classifier_symbol_box,
                                                       const geometry_rectangle_t *classifier_envelope_box,
                                                       layout_feature_t *out_feature_layout
                                                     );

/*!
 *  \brief determines the symbol box of a port or action-pin or entry/exit state
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_type type of the parent classifier
 *  \param classifier_symbol_box symbol box of the parent classifier
 *  \param the_feature the feature data to be layouted
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param[out] out_feature_layout output parameter: feature layout coordinates
 */
void pencil_feature_layouter_private_layout_port_pin ( pencil_feature_layouter_t *this_,
                                                       data_classifier_type_t classifier_type,
                                                       const geometry_rectangle_t *classifier_symbol_box,
                                                       const data_feature_t *the_feature,
                                                       PangoLayout *font_layout,
                                                       layout_feature_t *out_feature_layout
                                                     );

/*!
 *  \brief determines the symbol box of a provided or required interface
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_symbol_box symbol box of the classifier
 *  \param the_feature the feature data to be layouted
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param[out] out_feature_layout output parameter: feature layout coordinates
 */
void pencil_feature_layouter_private_layout_interface ( pencil_feature_layouter_t *this_,
                                                        const geometry_rectangle_t *classifier_symbol_box,
                                                        const data_feature_t *the_feature,
                                                        PangoLayout *font_layout,
                                                        layout_feature_t *out_feature_layout
                                                      );

/*!
 *  \brief determines the symbol box of a property or operation or tagged-value
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_space space area in the classifier
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param[in,out] io_feature_layout in+output parameter: feature layout coordinates
 */
void pencil_feature_layouter_private_layout_compartment ( pencil_feature_layouter_t *this_,
                                                          const geometry_rectangle_t *classifier_space,
                                                          PangoLayout *font_layout,
                                                          layout_feature_t *io_feature_layout
                                                        );

#endif  /* PENCIL_FEATURE_LAYOUTER_H */


/*
Copyright 2017-2025 Andreas Warnke

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
