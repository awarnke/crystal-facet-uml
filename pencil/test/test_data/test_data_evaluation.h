/* File: test_data_evaluation.h; Copyright and License: see below */

#ifndef TEST_DATA_EVALUATION_H
#define TEST_DATA_EVALUATION_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores the layout data for input data.
 */

#include "geometry/geometry_connector.h"
#include "geometry/geometry_rectangle.h"
#include "layout/layout_diagram.h"
#include "layout/layout_feature.h"
#include "layout/layout_relationship.h"
#include "layout/layout_visible_classifier.h"
#include "layout/layout_visible_set.h"
#include "filter/pencil_visibility.h"
#include "set/data_visible_set.h"
#include "set/data_stat.h"
#include "data_rules.h"
#include <cairo.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief attributes of the test_data_evaluation_t
 *
 *  test_data_evaluation_t analyzes the quality of a layout_visible_set_t:
 */
struct test_data_evaluation_struct
{
    data_rules_t filter_rules;  /*!< own instance of uml and sysml consistency rules */
};

typedef struct test_data_evaluation_struct test_data_evaluation_t;

/*!
 *  \brief initializes the painter input data layout
 *
 *  \param this_ pointer to own object attributes
 */
void test_data_evaluation_init( test_data_evaluation_t *this_ );

/*!
 *  \brief destroys the painter input data layout
 *
 *  \param this_ pointer to own object attributes
 */
void test_data_evaluation_destroy( test_data_evaluation_t *this_ );


/*!
 *  \brief analyzes test_data_evaluation: gets statistics and calls back a function in case of overlaps
 *
 *  \note
 *  The analyze function to evaluate test results must not use the evaluation functions
 *  of the layout_quality class. Otherwise the layouting quality is determined by the same
 *  methods that produce the layout - which is not a comparable, objective result.
 *
 *  \param this_ pointer to own object attributes
 *  \param input_layout pointer to the data that was layouted
 *  \param io_layout_stat pointer to already initialized statistics object where layouting statistics are added
 *  \param overlap_callback pointer to a function that is called in case of overlaps
 *  \param data pointer that is passed to overlap_callback
 */
void test_data_evaluation_analyze ( const test_data_evaluation_t *this_,
                                    const layout_visible_set_t *input_layout,
                                    data_stat_t *io_layout_stat,
                                    void (*overlap_callback)(void *data, const geometry_rectangle_t *a, const geometry_rectangle_t *b),
                                    void *data
                                  );

#endif  /* TEST_DATA_EVALUATION_H */


/*
Copyright 2017-2026 Andreas Warnke

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
