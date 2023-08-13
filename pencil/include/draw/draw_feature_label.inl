/* File: draw_feature_label.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline void draw_feature_label_init( draw_feature_label_t *this_ )
{
    draw_stereotype_image_init( &((*this_).image_renderer) );
}

static inline void draw_feature_label_destroy( draw_feature_label_t *this_ )
{
    draw_stereotype_image_destroy( &((*this_).image_renderer) );
}


/*
Copyright 2019-2023 Andreas Warnke

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
