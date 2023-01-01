/* File: pencil_relationship_2d_layouter.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline u8_error_t pencil_relationship_2d_layouter_private_find_space_for_h_line ( pencil_relationship_2d_layouter_t *this_,
                                                                                         const geometry_rectangle_t *search_rect,
                                                                                         double min_gap,
                                                                                         double *io_ordinate )
{
    return pencil_relationship_2d_layouter_private_find_space_for_line ( this_,
                                                                         search_rect,
                                                                         true, /* horizontal_line */
                                                                         min_gap,
                                                                         io_ordinate
                                                                       );
}

static inline u8_error_t pencil_relationship_2d_layouter_private_find_space_for_v_line ( pencil_relationship_2d_layouter_t *this_,
                                                                                         const geometry_rectangle_t *search_rect,
                                                                                         double min_gap,
                                                                                         double *io_abscissa )
{
    return pencil_relationship_2d_layouter_private_find_space_for_line ( this_,
                                                                         search_rect,
                                                                         false, /* horizontal_line */
                                                                         min_gap,
                                                                         io_abscissa
                                                                       );
}


/*
Copyright 2022-2023 Andreas Warnke

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
