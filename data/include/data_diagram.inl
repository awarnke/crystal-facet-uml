/* File: data:diagram.inl; Copyright and License: see below */

static inline void data_diagram_init_new ( data_diagram_t *this_, int32_t parent_diagram_id, data_diagram_type_t diagram_type, const char* diagram_name )
{
    (*this_).id = DATA_DIAGRAM_ID_NEW_ID;
    (*this_).parent_id = parent_diagram_id;
    (*this_).diagram_type = diagram_type;
    /* todo: diagram_name */
}

static inline void data_diagram_init ( data_diagram_t *this_, int32_t diagram_id, int32_t parent_diagram_id, data_diagram_type_t diagram_type, const char* diagram_name )
{
    (*this_).id = diagram_id;
    (*this_).parent_id = parent_diagram_id;
    (*this_).diagram_type = diagram_type;
    /* todo: diagram_name */
}

static inline void data_diagram_destroy ( data_diagram_t *this_ )
{
    (*this_).id = DATA_DIAGRAM_ID_UNINITIALIZED_ID;
    (*this_).parent_id = DATA_DIAGRAM_ID_UNINITIALIZED_ID;
    (*this_).diagram_type = 0;
    /* todo: diagram_name */
}

/*
Copyright 2016-2016 Andreas Warnke

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
