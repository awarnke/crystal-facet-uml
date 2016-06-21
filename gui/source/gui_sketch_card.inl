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

static inline bool gui_sketch_card_is_valid( gui_sketch_card_t *this_ )
{
    return pencil_input_data_is_valid( &((*this_).painter_input_data) );
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

static inline data_diagram_t *gui_sketch_card_get_diagram_ptr ( gui_sketch_card_t *this_ )
{
    return pencil_input_data_get_diagram_ptr( &((*this_).painter_input_data) );
}

static inline data_id_t gui_sketch_card_get_object_id_at_pos ( gui_sketch_card_t *this_, int32_t x, int32_t y, bool dereference )
{
    geometry_rectangle_t geo_rect;
    int32_t left;
    int32_t top;
    uint32_t width;
    uint32_t height;
    data_id_t result;

    left = shape_int_rectangle_get_left( &((*this_).bounds) );
    top = shape_int_rectangle_get_top( &((*this_).bounds) );
    width = shape_int_rectangle_get_width( &((*this_).bounds) );
    height = shape_int_rectangle_get_height( &((*this_).bounds) );

    geometry_rectangle_init( &geo_rect, left, top, width, height );

    result = pencil_diagram_painter_get_object_id_at_pos( &((*this_).painter),
                                                          &((*this_).painter_input_data),
                                                          (double) x,
                                                          (double) y,
                                                          geo_rect,
                                                          dereference
                                                        );

    return result;
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
