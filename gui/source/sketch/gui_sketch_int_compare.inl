/* File: gui_sketch_int_compare.inl; Copyright and License: see below */

static inline int32_t gui_sketch_int_compare_min_i32 ( int32_t this_, int32_t that )
{
    return ( this_ < that ) ? this_ : that;
}

static inline int32_t gui_sketch_int_compare_max_i32 ( int32_t this_, int32_t that )
{
    return ( this_ > that ) ? this_ : that;
}


/*
Copyright 2021-2024 Andreas Warnke

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
