/* File: gui_sketch_card.inl; Copyright and License: see below */

static inline void gui_sketch_card_load_data( gui_sketch_card_t *this_, int64_t diagram_id, data_database_reader_t *db_reader )
{
    /* load data to be drawn */
    pencil_input_data_load( &((*this_).painter_input_data), diagram_id, db_reader );
}

static inline void gui_sketch_card_invalidate_data( gui_sketch_card_t *this_ )
{
    pencil_input_data_invalidate( &((*this_).painter_input_data) );
}

static inline shape_int_rectangle_t gui_sketch_card_get_bounds( gui_sketch_card_t *this_ )
{
    return (*this_).bounds;
}

static inline void gui_sketch_card_set_bounds( gui_sketch_card_t *this_, shape_int_rectangle_t bounds )
{
    (*this_).bounds = bounds;
}

static inline bool gui_sketch_card_is_visible( gui_sketch_card_t *this_ )
{
    return (*this_).visible;
}

static inline void gui_sketch_card_set_visible( gui_sketch_card_t *this_, bool visible )
{
    (*this_).visible = visible;
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
