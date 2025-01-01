/* File: pencil_diagram_maker_test.h; Copyright and License: see below */

#ifndef PENCIL_DIAGRAM_MAKER_TEST_H
#define PENCIL_DIAGRAM_MAKER_TEST_H

/*!
 *  \file
 *  \brief MODULE TEST for pencil_diagram_maker
 */

#include "test_suite.h"
#include "geometry/geometry_rectangle.h"

/*!
 *  \brief gets the test suite for pencil_diagram_maker
 *
 *  \return test suite
 */
test_suite_t pencil_diagram_maker_test_get_suite(void);

/*!
 *  \brief draws rectangles for layout_visible_set_analyze to indicate overlaps
 *
 *  \param data pointer that is passed to overlap_callback
 *  \param rect_a pointer to a rectangle that overlaps with rect_b
 *  \param rect_b pointer to a rectangle that overlaps with rect_a
 */
void pencil_diagram_maker_test_draw_rects_callback ( void *data,
                                                     const geometry_rectangle_t *rect_a,
                                                     const geometry_rectangle_t *rect_b
                                                   );

#endif  /* PENCIL_DIAGRAM_MAKER_TEST_H */


/*
 * Copyright 2021-2025 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
