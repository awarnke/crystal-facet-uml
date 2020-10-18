/* File: pencil_feature_layouter.h; Copyright and License: see below */

#ifndef PENCIL_FEATURE_LAYOUTER_H
#define PENCIL_FEATURE_LAYOUTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Calculates positions of features in a diagram
 */

#include "pencil_size.h"
#include "pencil_layout_data.h"
#include "pencil_feature_painter.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/geometry/geometry_dimensions.h"
#include "util/geometry/geometry_non_linear_scale.h"
#include "data_diagram.h"
#include "set/data_visible_set.h"
#include "set/data_small_set.h"
#include "data_id.h"
#include "universal_int32_pair.h"
#include "universal_bool_list.h"
#include "universal_array_index_sorter.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the feature layouter
 */
struct pencil_feature_layouter_struct {
    pencil_layout_data_t *layout_data;  /* pointer to external layout data */

    pencil_size_t *pencil_size;  /*!< pointer to an external pencil_size_t object, defining pen sizes, gap sizes, font sizes and colors */

    pencil_feature_painter_t feature_painter;  /*!< own instance of a painter object to ask for display dimensions */
};

typedef struct pencil_feature_layouter_struct pencil_feature_layouter_t;

/*!
 *  \brief initializes the feature layouter
 *
 *  \param this_ pointer to own object attributes
 *  \param layout_data pointer to the layout information to be used and modified
 *  \param pencil_size pointer to the pencil_size_t object
 */
void pencil_feature_layouter_init( pencil_feature_layouter_t *this_,
                                   pencil_layout_data_t *layout_data,
                                   pencil_size_t *pencil_size
                                 );

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
 *  \param out_features_bounds memory location where the result shall be stored. Must not be NULL.
 */
void pencil_feature_layouter_calculate_features_bounds ( pencil_feature_layouter_t *this_,
                                                         data_row_id_t diagramelement_id,
                                                         PangoLayout *font_layout,
                                                         geometry_dimensions_t *out_features_bounds
                                                       );

/*!
 *  \brief determines the symbol box of a lifeline
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_space drawing space of the diagram
 *  \param diagram_type diagram type of the diagram
 *  \param classifier_symbol_box symbol box of the classifier
 *  \param out_feature_layout output parameter: feature layout coordinates
 */
void pencil_feature_layouter_private_layout_lifeline ( pencil_feature_layouter_t *this_,
                                                       const geometry_rectangle_t *diagram_space,
                                                       data_diagram_type_t diagram_type,
                                                       const geometry_rectangle_t *classifier_symbol_box,
                                                       layout_feature_t *out_feature_layout
                                                     );

/*!
 *  \brief determines the symbol box of a port
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_type type of the parent classifier
 *  \param classifier_symbol_box symbol box of the parent classifier
 *  \param the_feature the feature data to be layouted
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param out_feature_layout output parameter: feature layout coordinates
 */
void pencil_feature_layouter_private_layout_port ( pencil_feature_layouter_t *this_,
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
 *  \param out_feature_layout output parameter: feature layout coordinates
 */
void pencil_feature_layouter_private_layout_interface ( pencil_feature_layouter_t *this_,
                                                        const geometry_rectangle_t *classifier_symbol_box,
                                                        const data_feature_t *the_feature,
                                                        PangoLayout *font_layout,
                                                        layout_feature_t *out_feature_layout
                                                      );

/*!
 *  \brief determines the symbol box of a property or operation
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_space space area in the classifier
 *  \param the_feature the feature data to be layouted
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param out_feature_layout output parameter: feature layout coordinates
 */
void pencil_feature_layouter_private_layout_prop_or_op ( pencil_feature_layouter_t *this_,
                                                         const geometry_rectangle_t *classifier_space,
                                                         const data_feature_t *the_feature,
                                                         PangoLayout *font_layout,
                                                         layout_feature_t *out_feature_layout
                                                       );

#endif  /* PENCIL_FEATURE_LAYOUTER_H */


/*
Copyright 2017-2020 Andreas Warnke

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
