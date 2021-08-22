/* File: pencil_classifier_1d_layouter.c; Copyright and License: see below */

#include "pencil_classifier_1d_layouter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pencil_classifier_1d_layouter_init( pencil_classifier_1d_layouter_t *this_,
                                         pencil_layout_data_t *layout_data,
                                         const pencil_size_t *pencil_size )
{
    TRACE_BEGIN();
    assert( NULL != layout_data );
    assert( NULL != pencil_size );

    (*this_).layout_data = layout_data;
    (*this_).pencil_size = pencil_size;
    pencil_classifier_composer_init( &((*this_).classifier_composer) );

    /* get draw area */
    {
        layout_diagram_t *diagram_layout;
        diagram_layout = pencil_layout_data_get_diagram_ptr( (*this_).layout_data );
        (*this_).diagram_draw_area = layout_diagram_get_draw_area_const( diagram_layout );
    }

    TRACE_END();
}

void pencil_classifier_1d_layouter_destroy( pencil_classifier_1d_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_classifier_composer_destroy( &((*this_).classifier_composer) );

    TRACE_END();
}

void pencil_classifier_1d_layouter_layout_for_list( pencil_classifier_1d_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* get preferred object distance */
    const double obj_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    /* const double gap = pencil_size_get_standard_object_border( (*this_).pencil_size ); */

    /* get the draw coordinates */
    geometry_rectangle_t draw_area;
    geometry_rectangle_copy( &draw_area, (*this_).diagram_draw_area );
    geometry_rectangle_enlarge( &draw_area, 0.0, (- obj_dist) );
    double diag_w = geometry_rectangle_get_width( &draw_area );
    double diag_h = geometry_rectangle_get_height( &draw_area );

    const double phi = 1.6180339; /* minor=0.618, major=1.0, sum=1.618 => (sum/major)==(major/minor) */
    const double golden_ratio_width = diag_w/phi;
    const double golden_ratio_height = diag_h/phi;

    /* sort the classifiers according to their list_order */
    universal_array_index_sorter_t sorted_classifiers;
    universal_array_index_sorter_init( &sorted_classifiers );

    /* calculate preferred classifier bounds/envelopes */
    const uint_fast32_t c_count = pencil_layout_data_get_visible_classifier_count( (*this_).layout_data );
    for ( uint_fast32_t plain_idx = 0; plain_idx < c_count; plain_idx ++ )
    {
        layout_visible_classifier_t *const visible_classifier1
            = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, plain_idx );
        const data_visible_classifier_t *const visible_classifier_data
            = layout_visible_classifier_get_data_const ( visible_classifier1 );
        const data_classifier_t *const classifier1 = data_visible_classifier_get_classifier_const( visible_classifier_data );

        /* sort the classifiers according to their list_order */
        {
            const double weight = (const double) data_classifier_get_list_order( classifier1 );
            const int insert_err = universal_array_index_sorter_insert( &sorted_classifiers, plain_idx, weight );
            if ( 0 != insert_err )
            {
                TSLOG_ERROR ( "universal_array_index_sorter_t list is full." );
            }
        }

        /* set the preferred bounds, space and label_box of the classifier layout */
        {
            geometry_rectangle_t envelope_box;
            geometry_rectangle_init( &envelope_box, 0.0, 0.0, golden_ratio_width, (golden_ratio_height/c_count) );

            const bool has_contained_children = false;  /* this diagram type does not embrace children */
            pencil_classifier_composer_set_envelope_box( &((*this_).classifier_composer),
                                                         &envelope_box,
                                                         has_contained_children,
                                                         (*this_).pencil_size,
                                                         font_layout,
                                                         visible_classifier1
                                                       );

            geometry_rectangle_destroy( &envelope_box );
        }
    }

    /* layout list */
    pencil_classifier_1d_layouter_private_layout_vertical( this_,
                                                           &sorted_classifiers,
                                                           &draw_area,
                                                           GEOMETRY_H_ALIGN_CENTER
                                                         );

    /* cleanup sorted array indices and area-rectangles */
    universal_array_index_sorter_destroy( &sorted_classifiers );
    geometry_rectangle_destroy( &draw_area );

    TRACE_END();
}

void pencil_classifier_1d_layouter_layout_for_sequence( pencil_classifier_1d_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* get preferred object distance */
    const double obj_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    /* const double gap = pencil_size_get_standard_object_border( (*this_).pencil_size ); */

    /* get the diagram coordinates */
    geometry_rectangle_t draw_area;
    geometry_rectangle_copy( &draw_area, (*this_).diagram_draw_area );
    geometry_rectangle_shift( &draw_area, obj_dist, 0.0 );
    geometry_rectangle_enlarge( &draw_area, (-2.0 * obj_dist), (- obj_dist) );
    double diag_w = geometry_rectangle_get_width( &draw_area );
    double diag_h = geometry_rectangle_get_height( &draw_area );

    const double phi = 1.6180339; /* minor=0.618, major=1.0, sum=1.618 => (sum/major)==(major/minor) */
    const double minor_ratio = (1.0 - 0.6180339);
    const double golden_ratio_width = diag_w/phi;
    const double minor_minor_height = diag_h * minor_ratio * minor_ratio;
    const double half_minor_width = diag_w * minor_ratio * 0.5;

    geometry_rectangle_t left_area;
    geometry_rectangle_copy( &left_area, &draw_area );
    geometry_rectangle_enlarge( &left_area, (- half_minor_width), 0.0 );

    geometry_rectangle_t right_column;
    geometry_rectangle_copy( &right_column, &draw_area );
    geometry_rectangle_shift( &right_column, (diag_w-half_minor_width), minor_minor_height );
    geometry_rectangle_enlarge( &right_column, (half_minor_width-diag_w), (- minor_minor_height) );

    /* sort the classifiers according to their list_order */
    universal_array_index_sorter_t sorted_notes_reqs;
    universal_array_index_sorter_init( &sorted_notes_reqs );
    universal_array_index_sorter_t sorted_diag_refs;
    universal_array_index_sorter_init( &sorted_diag_refs );
    universal_array_index_sorter_t sorted_acting_classifiers;
    universal_array_index_sorter_init( &sorted_acting_classifiers );

    /* calculate preferred classifier bounds */
    const uint_fast32_t c_count = pencil_layout_data_get_visible_classifier_count( (*this_).layout_data );
    for ( uint_fast32_t plain_idx = 0; plain_idx < c_count; plain_idx ++ )
    {
        layout_visible_classifier_t *const visible_classifier1
            = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, plain_idx );
        const data_visible_classifier_t *const visible_classifier_data
            = layout_visible_classifier_get_data_const ( visible_classifier1 );
        const data_classifier_t *const classifier1 = data_visible_classifier_get_classifier_const( visible_classifier_data );
        const data_classifier_type_t c1_type = data_classifier_get_main_type ( classifier1 );

        /* check classifier type and sort into corresponding list */
        {
            int insert_err = 0;
            const double weight = (const double) data_classifier_get_list_order( classifier1 );
            if (( c1_type == DATA_CLASSIFIER_TYPE_REQUIREMENT )
                || ( c1_type == DATA_CLASSIFIER_TYPE_COMMENT ))
            {
                insert_err = universal_array_index_sorter_insert( &sorted_notes_reqs, plain_idx, weight );
            }
            else if ( c1_type == DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE )
            {
                insert_err = universal_array_index_sorter_insert( &sorted_diag_refs, plain_idx, weight );
            }
            else
            {
                insert_err = universal_array_index_sorter_insert( &sorted_acting_classifiers, plain_idx, weight );
            }
            if ( 0 != insert_err )
            {
                TSLOG_ERROR ( "universal_array_index_sorter_t list is full." );
            }
        }

        /* set the preferred bounds, space and label_box of the classifier layout */
        {
            geometry_rectangle_t envelope_box;
            if ( c1_type == DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE )
            {
                geometry_rectangle_init( &envelope_box, 0.0, 0.0, (diag_w-half_minor_width), (minor_minor_height/2.0) );
            }
            else
            {
                geometry_rectangle_init( &envelope_box, 0.0, 0.0, (golden_ratio_width/c_count), minor_minor_height );
            }

            const bool has_contained_children = false;  /* this diagram type does not embrace children */
            pencil_classifier_composer_set_envelope_box( &((*this_).classifier_composer),
                                                         &envelope_box,
                                                         has_contained_children,
                                                         (*this_).pencil_size,
                                                         font_layout,
                                                         visible_classifier1
                                                       );

            geometry_rectangle_destroy( &envelope_box );
        }
    }
    assert( c_count
            == universal_array_index_sorter_get_count( &sorted_notes_reqs )
            + universal_array_index_sorter_get_count( &sorted_diag_refs )
            + universal_array_index_sorter_get_count( &sorted_acting_classifiers )
          );

    /* layout acting classifiers */
    pencil_classifier_1d_layouter_private_layout_horizontal( this_,
                                                             &sorted_acting_classifiers,
                                                             &left_area,
                                                             GEOMETRY_V_ALIGN_TOP
                                                           );
    /* layout digram references */
    pencil_classifier_1d_layouter_private_linear_vertical( this_,
                                                           &sorted_diag_refs,
                                                           &left_area,
                                                           GEOMETRY_H_ALIGN_LEFT
                                                         );
    /* layout notes/comments and requirements */
    pencil_classifier_1d_layouter_private_linear_vertical( this_,
                                                           &sorted_notes_reqs,
                                                           &right_column,
                                                           GEOMETRY_H_ALIGN_RIGHT
                                                         );

    /* cleanup sorted array indices and area-rectangles */
    universal_array_index_sorter_destroy( &sorted_acting_classifiers );
    universal_array_index_sorter_destroy( &sorted_diag_refs );
    universal_array_index_sorter_destroy( &sorted_notes_reqs );
    geometry_rectangle_destroy( &right_column );
    geometry_rectangle_destroy( &left_area );
    geometry_rectangle_destroy( &draw_area );

    TRACE_END();
}

void pencil_classifier_1d_layouter_layout_for_timing( pencil_classifier_1d_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* get preferred object distance */
    const double obj_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    /* const double gap = pencil_size_get_standard_object_border( (*this_).pencil_size ); */

    /* get the diagram coordinates */
    geometry_rectangle_t draw_area;
    geometry_rectangle_copy( &draw_area, (*this_).diagram_draw_area );
    geometry_rectangle_shift( &draw_area, obj_dist, 0.0 );
    geometry_rectangle_enlarge( &draw_area, (-2.0 * obj_dist), (- obj_dist) );
    double diag_w = geometry_rectangle_get_width( &draw_area );
    double diag_h = geometry_rectangle_get_height( &draw_area );

    const double phi = 1.6180339; /* minor=0.618, major=1.0, sum=1.618 => (sum/major)==(major/minor) */
    const double minor_ratio = (1.0 - 0.6180339);
    const double golden_ratio_width = diag_w/phi;
    const double golden_ratio_height = diag_h/phi;
    const double minor_minor_width = diag_w * minor_ratio * minor_ratio;
    const double minor_minor_height = diag_h * minor_ratio * minor_ratio;

    geometry_rectangle_t top_row;
    geometry_rectangle_copy( &top_row, &draw_area );
    geometry_rectangle_shift( &top_row, minor_minor_width, 0.0 );
    geometry_rectangle_enlarge( &top_row, (- minor_minor_width), (minor_minor_width-diag_h) );

    geometry_rectangle_t center_area;
    geometry_rectangle_copy( &center_area, &draw_area );
    geometry_rectangle_shift( &center_area, 0.0, minor_minor_height );
    geometry_rectangle_enlarge( &center_area, 0.0, (- minor_minor_height) );

    /* sort the classifiers according to their list_order */
    universal_array_index_sorter_t sorted_notes_reqs;
    universal_array_index_sorter_init( &sorted_notes_reqs );
    universal_array_index_sorter_t sorted_acting_classifiers;
    universal_array_index_sorter_init( &sorted_acting_classifiers );

    /* calculate preferred classifier bounds */
    const uint_fast32_t c_count = pencil_layout_data_get_visible_classifier_count( (*this_).layout_data );
    for ( uint_fast32_t plain_idx = 0; plain_idx < c_count; plain_idx ++ )
    {
        layout_visible_classifier_t *const visible_classifier1
            = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, plain_idx );
        const data_visible_classifier_t *const visible_classifier_data
            = layout_visible_classifier_get_data_const ( visible_classifier1 );
        const data_classifier_t *const classifier1 = data_visible_classifier_get_classifier_const( visible_classifier_data );
        const data_classifier_type_t c1_type = data_classifier_get_main_type ( classifier1 );

        /* check classifier type and sort into corresponding list */
        {
            int insert_err = 0;
            const double weight = (const double) data_classifier_get_list_order( classifier1 );
            if (( c1_type == DATA_CLASSIFIER_TYPE_REQUIREMENT )
                || ( c1_type == DATA_CLASSIFIER_TYPE_COMMENT ))
            {
                insert_err = universal_array_index_sorter_insert( &sorted_notes_reqs, plain_idx, weight );
            }
            else
            {
                insert_err = universal_array_index_sorter_insert( &sorted_acting_classifiers, plain_idx, weight );
            }
            if ( 0 != insert_err )
            {
                TSLOG_ERROR ( "universal_array_index_sorter_t list is full." );
            }
        }

        /* set the preferred bounds, space and label_box of the classifier layout */
        {
            geometry_rectangle_t envelope_box;
            geometry_rectangle_init( &envelope_box, 0.0, 0.0, ((diag_w-golden_ratio_width)/2.0), (golden_ratio_height/c_count) );

            const bool has_contained_children = false;  /* this diagram type does not embrace children */
            pencil_classifier_composer_set_envelope_box( &((*this_).classifier_composer),
                                                         &envelope_box,
                                                         has_contained_children,
                                                         (*this_).pencil_size,
                                                         font_layout,
                                                         visible_classifier1
                                                       );

            geometry_rectangle_destroy( &envelope_box );
        }
    }
    assert( c_count
            == universal_array_index_sorter_get_count( &sorted_notes_reqs )
            + universal_array_index_sorter_get_count( &sorted_acting_classifiers )
          );

    /* layout acting classifiers */
    pencil_classifier_1d_layouter_private_layout_vertical( this_,
                                                           &sorted_acting_classifiers,
                                                           &center_area,
                                                           GEOMETRY_H_ALIGN_LEFT
                                                         );
    /* layout notes/comments and requirements */
    pencil_classifier_1d_layouter_private_linear_horizontal( this_,
                                                             &sorted_notes_reqs,
                                                             &top_row,
                                                             GEOMETRY_V_ALIGN_TOP
                                                           );

    /* cleanup sorted array indices and area-rectangles */
    universal_array_index_sorter_destroy( &sorted_acting_classifiers );
    universal_array_index_sorter_destroy( &sorted_notes_reqs );
    geometry_rectangle_destroy( &center_area );
    geometry_rectangle_destroy( &top_row );
    geometry_rectangle_destroy( &draw_area );

    TRACE_END();
}

void pencil_classifier_1d_layouter_private_layout_horizontal( const pencil_classifier_1d_layouter_t *this_,
                                                              const universal_array_index_sorter_t *classifier_list,
                                                              const geometry_rectangle_t *dest_rect,
                                                              geometry_v_align_t v_alignment )
{
    TRACE_BEGIN();

    /* get the destination rectangle coordinates */
    double diag_x = geometry_rectangle_get_left( dest_rect );
    double diag_w = geometry_rectangle_get_width( dest_rect );

    /* calculate sum of wished envelope widths */
    double total_wish_width = 0.0;
    const uint_fast32_t count_classifiers = universal_array_index_sorter_get_count ( classifier_list );
    for ( uint_fast32_t sort_idx = 0; sort_idx < count_classifiers; sort_idx ++ )
    {
        uint_fast32_t index = universal_array_index_sorter_get_array_index( classifier_list, sort_idx );
        const layout_visible_classifier_t *const visible_classifier1
            = pencil_layout_data_get_visible_classifier_const( (*this_).layout_data, index );

        /* update sum of wished envelope widths */
        const geometry_rectangle_t envelope
            = layout_visible_classifier_calc_envelope_box( visible_classifier1 );
        total_wish_width += geometry_rectangle_get_width( &envelope );
    }
    const double dx_spaces = (count_classifiers==0) ? diag_w : ((diag_w - total_wish_width)/count_classifiers);

    /* update the classifier coordinates */
    double current_x = diag_x;
    for ( uint_fast32_t sort_idx = 0; sort_idx < count_classifiers; sort_idx ++ )
    {
        uint_fast32_t index = universal_array_index_sorter_get_array_index( classifier_list, sort_idx );
        layout_visible_classifier_t *const visible_classifier2
            = pencil_layout_data_get_visible_classifier_ptr ( (*this_).layout_data, index );

        const geometry_rectangle_t envelope
            = layout_visible_classifier_calc_envelope_box( visible_classifier2 );
        const double envelope_top = geometry_rectangle_get_top( &envelope );
        const double envelope_height = geometry_rectangle_get_height( &envelope );
        const double envelope_left = geometry_rectangle_get_left( &envelope );
        const double envelope_width = geometry_rectangle_get_width( &envelope );

        const double dest_top = geometry_v_align_get_top( &v_alignment,
                                                          envelope_height,
                                                          geometry_rectangle_get_top( dest_rect ),
                                                          geometry_rectangle_get_height( dest_rect )
                                                        );
        const double delta_y = dest_top - envelope_top;

        if ( dx_spaces > 0.0 )
        {
            /* equal spaces if there are spaces */
            const double delta_x = current_x + (dx_spaces/2.0) - envelope_left;

            layout_visible_classifier_shift( visible_classifier2, delta_x, delta_y );

            current_x += dx_spaces + envelope_width;
        }
        else
        {
            /* const double wish2avail_ratio = (diag_h == 0.0) ? 1.0 : (total_wish_width/diag_h); */
            const double avail2wish_ratio = (total_wish_width == 0.0) ? 1.0 : (diag_w/total_wish_width);
            const double available_width = envelope_width * avail2wish_ratio;
            const double x_align_ratio = (count_classifiers==1) ? 0.5 : sort_idx/(count_classifiers-1.0);
            const double x_align_envelope = envelope_width * x_align_ratio;
            const double x_align_available = available_width * x_align_ratio;

            const double delta_x = (current_x+x_align_available) - (envelope_left+x_align_envelope);

            layout_visible_classifier_shift( visible_classifier2, delta_x, delta_y );

            current_x += available_width;
        }
    }

    TRACE_END();
}

void pencil_classifier_1d_layouter_private_layout_vertical( const pencil_classifier_1d_layouter_t *this_,
                                                            const universal_array_index_sorter_t *classifier_list,
                                                            const geometry_rectangle_t *dest_rect,
                                                            geometry_h_align_t h_alignment )
{
    TRACE_BEGIN();

    /* get the destination rectangle coordinates */
    double diag_y = geometry_rectangle_get_top( dest_rect );
    double diag_h = geometry_rectangle_get_height( dest_rect );

    /* calculate sum of wished envelope heights */
    double total_wish_height = 0.0;
    const uint_fast32_t count_classifiers = universal_array_index_sorter_get_count ( classifier_list );
    for ( uint_fast32_t sort_idx = 0; sort_idx < count_classifiers; sort_idx ++ )
    {
        uint_fast32_t index = universal_array_index_sorter_get_array_index( classifier_list, sort_idx );
        const layout_visible_classifier_t *const visible_classifier1
            = pencil_layout_data_get_visible_classifier_const( (*this_).layout_data, index );

        /* update sum of wished envelope heights */
        const geometry_rectangle_t envelope
            = layout_visible_classifier_calc_envelope_box( visible_classifier1 );
        total_wish_height += geometry_rectangle_get_height( &envelope );
    }
    const double dy_spaces = (count_classifiers==0) ? diag_h : ((diag_h - total_wish_height)/count_classifiers);

    /* update the classifier coordinates */
    double current_y = diag_y;
    for ( uint_fast32_t sort_idx = 0; sort_idx < count_classifiers; sort_idx ++ )
    {
        uint_fast32_t index = universal_array_index_sorter_get_array_index( classifier_list, sort_idx );
        layout_visible_classifier_t *const visible_classifier2
            = pencil_layout_data_get_visible_classifier_ptr ( (*this_).layout_data, index );

        const geometry_rectangle_t envelope
            = layout_visible_classifier_calc_envelope_box( visible_classifier2 );
        const double envelope_top = geometry_rectangle_get_top( &envelope );
        const double envelope_height = geometry_rectangle_get_height( &envelope );
        const double envelope_left = geometry_rectangle_get_left( &envelope );
        const double envelope_width = geometry_rectangle_get_width( &envelope );

        const double dest_left = geometry_h_align_get_left( &h_alignment,
                                                            envelope_width,
                                                            geometry_rectangle_get_left( dest_rect ),
                                                            geometry_rectangle_get_width( dest_rect )
                                                          );
        const double delta_x = dest_left - envelope_left;

        if ( dy_spaces > 0.0 )
        {
            /* equal spaces if there are spaces */
            const double delta_y = current_y + (dy_spaces/2.0) - envelope_top;

            layout_visible_classifier_shift( visible_classifier2, delta_x, delta_y );

            current_y += dy_spaces + envelope_height;
        }
        else
        {
            /* keep available-to-wish ratio if classifiers overlap */
            /* const double wish2avail_ratio = (diag_h == 0.0) ? 1.0 : (total_wish_height/diag_h); */
            const double avail2wish_ratio = (total_wish_height == 0.0) ? 1.0 : (diag_h/total_wish_height);
            const double available_height = envelope_height * avail2wish_ratio;
            const double y_align_ratio = (count_classifiers==1) ? 0.5 : sort_idx/(count_classifiers-1.0);
            const double y_align_envelope = envelope_height * y_align_ratio;
            const double y_align_available = available_height * y_align_ratio;

            const double delta_y = (current_y+y_align_available) - (envelope_top+y_align_envelope);

            layout_visible_classifier_shift( visible_classifier2, delta_x, delta_y );

            current_y += available_height;
        }
    }

    TRACE_END();
}

void pencil_classifier_1d_layouter_private_linear_horizontal( const pencil_classifier_1d_layouter_t *this_,
                                                              const universal_array_index_sorter_t *classifier_list,
                                                              const geometry_rectangle_t *dest_rect,
                                                              geometry_v_align_t v_alignment )
{
    TRACE_BEGIN();

    /* get the destination rectangle coordinates */
    double diag_x = geometry_rectangle_get_left( (*this_).diagram_draw_area );
    double diag_w = geometry_rectangle_get_width( (*this_).diagram_draw_area );
    double dest_left = geometry_rectangle_get_left( dest_rect );
    double dest_right = geometry_rectangle_get_right( dest_rect );

    /* update the classifier coordinates */
    const uint_fast32_t count_classifiers = universal_array_index_sorter_get_count ( classifier_list );
    for ( uint_fast32_t sort_idx = 0; sort_idx < count_classifiers; sort_idx ++ )
    {
        uint_fast32_t index = universal_array_index_sorter_get_array_index( classifier_list, sort_idx );
        layout_visible_classifier_t *const visible_classifier2
            = pencil_layout_data_get_visible_classifier_ptr ( (*this_).layout_data, index );

        /* get envelope */
        const geometry_rectangle_t envelope
            = layout_visible_classifier_calc_envelope_box( visible_classifier2 );
        const double envelope_top = geometry_rectangle_get_top( &envelope );
        const double envelope_height = geometry_rectangle_get_height( &envelope );
        const double envelope_left = geometry_rectangle_get_left( &envelope );
        const double envelope_width = geometry_rectangle_get_width( &envelope );

        /* calc x */
        const data_visible_classifier_t *const visible_classifier_data
            = layout_visible_classifier_get_data_const ( visible_classifier2 );
        const data_classifier_t *const classifier2 = data_visible_classifier_get_classifier_const( visible_classifier_data );
        const int32_t list_order = data_classifier_get_list_order( classifier2 );
        const double x_value_rel = (list_order/((double)UINT32_MAX))+0.5;
        double new_left = diag_x + (diag_w-envelope_width)*x_value_rel;
        if ( (new_left+envelope_width) > dest_right )
        {
            new_left = dest_right - envelope_width;
        }
        if ( new_left < dest_left )
        {
            new_left = dest_left;
        }
        const double delta_x = new_left - envelope_left;

        /* calc y */
        const double dest_top = geometry_v_align_get_top( &v_alignment,
                                                          envelope_height,
                                                          geometry_rectangle_get_top( dest_rect ),
                                                          geometry_rectangle_get_height( dest_rect )
                                                        );
        const double delta_y = dest_top - envelope_top;

        layout_visible_classifier_shift( visible_classifier2, delta_x, delta_y );
    }

    TRACE_END();
}

void pencil_classifier_1d_layouter_private_linear_vertical( const pencil_classifier_1d_layouter_t *this_,
                                                            const universal_array_index_sorter_t *classifier_list,
                                                            const geometry_rectangle_t *dest_rect,
                                                            geometry_h_align_t h_alignment )
{
    TRACE_BEGIN();

    /* get the destination rectangle coordinates */
    double diag_y = geometry_rectangle_get_top( (*this_).diagram_draw_area );
    double diag_h = geometry_rectangle_get_height( (*this_).diagram_draw_area );
    double dest_top = geometry_rectangle_get_top( dest_rect );
    double dest_bottom = geometry_rectangle_get_bottom( dest_rect );

    /* update the classifier coordinates */
    const uint_fast32_t count_classifiers = universal_array_index_sorter_get_count ( classifier_list );
    for ( uint_fast32_t sort_idx = 0; sort_idx < count_classifiers; sort_idx ++ )
    {
        uint_fast32_t index = universal_array_index_sorter_get_array_index( classifier_list, sort_idx );
        layout_visible_classifier_t *const visible_classifier2
            = pencil_layout_data_get_visible_classifier_ptr ( (*this_).layout_data, index );

        /* get envelope */
        const geometry_rectangle_t envelope
            = layout_visible_classifier_calc_envelope_box( visible_classifier2 );
        const double envelope_top = geometry_rectangle_get_top( &envelope );
        const double envelope_height = geometry_rectangle_get_height( &envelope );
        const double envelope_left = geometry_rectangle_get_left( &envelope );
        const double envelope_width = geometry_rectangle_get_width( &envelope );

        /* calc x */
        const double dest_left = geometry_h_align_get_left( &h_alignment,
                                                            envelope_width,
                                                            geometry_rectangle_get_left( dest_rect ),
                                                            geometry_rectangle_get_width( dest_rect )
                                                          );
        const double delta_x = dest_left - envelope_left;

        /* calc y */
        const data_visible_classifier_t *const visible_classifier_data
            = layout_visible_classifier_get_data_const ( visible_classifier2 );
        const data_classifier_t *const classifier2 = data_visible_classifier_get_classifier_const( visible_classifier_data );
        const int32_t list_order = data_classifier_get_list_order( classifier2 );
        const double y_value_rel = (list_order/((double)UINT32_MAX))+0.5;
        double new_top = diag_y + (diag_h-envelope_height)*y_value_rel;
        if ( (new_top+envelope_height) > dest_bottom )
        {
            new_top = dest_bottom - envelope_height;
        }
        if ( new_top < dest_top )
        {
            new_top = dest_top;
        }
        const double delta_y = new_top - envelope_top;

        layout_visible_classifier_shift( visible_classifier2, delta_x, delta_y );

    }

    TRACE_END();
}


/*
Copyright 2017-2021 Andreas Warnke

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
