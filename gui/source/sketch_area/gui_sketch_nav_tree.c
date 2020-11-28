/* File: gui_sketch_nav_tree.c; Copyright and License: see below */

#include "sketch_area/gui_sketch_nav_tree.h"
#include "util/geometry/geometry_rectangle.h"
#include "trace.h"
#include "tslog.h"
#include <gdk/gdk.h>

static const int STANDARD_FONT_SIZE = 12;
static const char *STANDARD_FONT_FAMILY = "Sans";
static const uint32_t NAV_TREE_FIRST_LINE = 1;

void gui_sketch_nav_tree_init( gui_sketch_nav_tree_t *this_, gui_resources_t *resources )
{
    TRACE_BEGIN();
    assert( resources != NULL );

    (*this_).ancestors_count = 0;
    (*this_).siblings_count = 0;
    (*this_).children_count = 0;
    (*this_).siblings_self_index = -1;

    (*this_).line_idx_new_child = -1;
    (*this_).line_idx_new_sibling = -1;
    (*this_).line_idx_new_root = -1;
    (*this_).line_idx_ancestors_start = NAV_TREE_FIRST_LINE;
    (*this_).line_idx_siblings_start = NAV_TREE_FIRST_LINE;
    (*this_).line_idx_siblings_next_after_self = NAV_TREE_FIRST_LINE;
    (*this_).line_idx_self = NAV_TREE_FIRST_LINE;
    (*this_).line_idx_children_start = NAV_TREE_FIRST_LINE;
    (*this_).line_cnt_ancestors = 0;
    (*this_).line_cnt_siblings_to_incl_self = 0;
    (*this_).line_cnt_siblings_after_self = 0;
    (*this_).line_cnt_children = 0;

    (*this_).visible = false;
    shape_int_rectangle_init( &((*this_).bounds), 0, 0, 0, 0 );

    /* init the pango font renering engine stuff: */
    (*this_).standard_font_description = pango_font_description_new ();
    pango_font_description_set_family_static ( (*this_).standard_font_description, STANDARD_FONT_FAMILY );
    pango_font_description_set_style ( (*this_).standard_font_description, PANGO_STYLE_NORMAL );
    pango_font_description_set_weight ( (*this_).standard_font_description, PANGO_WEIGHT_MEDIUM );
    pango_font_description_set_stretch ( (*this_).standard_font_description, PANGO_STRETCH_NORMAL );
    pango_font_description_set_size ( (*this_).standard_font_description, STANDARD_FONT_SIZE * PANGO_SCALE );

    gui_sketch_marker_init( &((*this_).sketch_marker) );
    (*this_).resources = resources;

    TRACE_END();
}

void gui_sketch_nav_tree_destroy( gui_sketch_nav_tree_t *this_ )
{
    TRACE_BEGIN();

    (*this_).resources = NULL;
    gui_sketch_marker_destroy( &((*this_).sketch_marker) );

    /* destroy the pango font renering engine stuff: */
    pango_font_description_free ( (*this_).standard_font_description );
    (*this_).standard_font_description = NULL;

    gui_sketch_nav_tree_invalidate_data( this_ );

    shape_int_rectangle_destroy( &((*this_).bounds) );

    TRACE_END();
}

void gui_sketch_nav_tree_load_data( gui_sketch_nav_tree_t *this_, data_row_id_t diagram_id, data_database_reader_t *db_reader )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );

    gui_sketch_nav_tree_invalidate_data( this_ );

    data_error_t db_err = DATA_ERROR_NONE;

    /* get ancestors */
    bool finished = false;
    for ( unsigned int anc_index = 0; ( anc_index < GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS ) && ( db_err == DATA_ERROR_NONE ) && ( ! finished ); anc_index ++ )
    {
        data_row_id_t id_to_load;
        if ( anc_index == 0 )
        {
            id_to_load = diagram_id;
        }
        else
        {
            id_to_load = data_diagram_get_parent_row_id( &((*this_).ancestor_diagrams[anc_index-1]) );
        }

        if ( id_to_load != DATA_ROW_ID_VOID )
        {
            db_err = data_database_reader_get_diagram_by_id ( db_reader, id_to_load, &((*this_).ancestor_diagrams[anc_index]) );
            if ( db_err == DATA_ERROR_NONE )
            {
                (*this_).ancestors_count = anc_index+1;
            }
        }
        else
        {
            finished = true;
        }
    }

    if ( (*this_).ancestors_count == 0 )
    {
        db_err = DATA_ERROR_INVALID_REQUEST;
        TSLOG_ANOMALY_INT( "gui_sketch_nav_tree_load_data cannot load diagram", diagram_id );
    }

    /* get siblings */
    if ( db_err == DATA_ERROR_NONE )
    {
        data_row_id_t parent_id;
        parent_id = data_diagram_get_parent_row_id( &((*this_).ancestor_diagrams[0]) );
        db_err = data_database_reader_get_diagrams_by_parent_id ( db_reader,
                                                                  parent_id,
                                                                  GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS,
                                                                  &( (*this_).sibling_diagrams ),
                                                                  &( (*this_).siblings_count )
                                                                );

        /* search self in list of siblings */
        {
            assert( (*this_).siblings_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS );

            (*this_).siblings_self_index = -1;
            for ( int sib_index = 0; sib_index < (*this_).siblings_count; sib_index ++ )
            {
                if ( diagram_id == data_diagram_get_row_id( &((*this_).sibling_diagrams[sib_index]) ) )
                {
                    (*this_).siblings_self_index = sib_index;
                }
            }
        }
    }

    /* get children */
    if ( db_err == DATA_ERROR_NONE )
    {

        db_err = data_database_reader_get_diagrams_by_parent_id ( db_reader,
                                                                  diagram_id,
                                                                  GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN,
                                                                  &( (*this_).child_diagrams ),
                                                                  &( (*this_).children_count )
                                                                );
    }

    gui_sketch_nav_tree_private_do_layout( this_ );

    TRACE_END();
}

void gui_sketch_nav_tree_private_do_layout( gui_sketch_nav_tree_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).ancestors_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS );
    assert( (*this_).siblings_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS );
    assert( (*this_).children_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN );

    /* calculate the "new" button positions */
    if ( (*this_).ancestors_count == 0 )
    {
        /* cannot create a child without a parent */
        (*this_).line_idx_new_child = -1;
        /* cannot create a sibling without a root */
        (*this_).line_idx_new_sibling = -1;
        /* show only a new root button */
        (*this_).line_idx_new_root = NAV_TREE_FIRST_LINE;

    }
    else
    {
        (*this_).line_idx_new_child = NAV_TREE_FIRST_LINE + (*this_).ancestors_count - 1
                + (*this_).siblings_self_index + 1 + (*this_).children_count;
        (*this_).line_idx_new_root = -1;
        if ( (*this_).ancestors_count == 1 )
        {
            /* cannot create a sibling to the root */
            (*this_).line_idx_new_sibling = -1;
        }
        else
        {
            (*this_).line_idx_new_sibling = NAV_TREE_FIRST_LINE + (*this_).ancestors_count - 1
                    + (*this_).siblings_count + (*this_).children_count + 1;  /* +1 for the new_child_line */
        }
    }

    (*this_).line_idx_ancestors_start = NAV_TREE_FIRST_LINE;
    (*this_).line_cnt_ancestors = ( (*this_).ancestors_count == 0 ) ? 0 : ( (*this_).ancestors_count-1 );
    (*this_).line_idx_siblings_start = ( (*this_).ancestors_count == 0 )
            ? NAV_TREE_FIRST_LINE
            : ( NAV_TREE_FIRST_LINE + (*this_).ancestors_count - 1 );

    if (( (*this_).siblings_self_index != -1 ) && ( (*this_).siblings_count > (*this_).siblings_self_index ))
    {
        (*this_).line_idx_self = (*this_).line_idx_siblings_start + (*this_).siblings_self_index;
        (*this_).line_idx_siblings_next_after_self = (*this_).line_idx_self + (*this_).children_count + 1 + 1;  /* +1 for new child button */
        (*this_).line_idx_children_start = (*this_).line_idx_self + 1;
        (*this_).line_cnt_siblings_to_incl_self = (*this_).siblings_self_index + 1;
        (*this_).line_cnt_siblings_after_self = (*this_).siblings_count - 1 - (*this_).siblings_self_index;
    }
    else
    {
        TRACE_INFO("there is no self diagram in the current diagram tree!")
        (*this_).line_idx_self = NAV_TREE_FIRST_LINE;
        (*this_).line_idx_siblings_next_after_self = NAV_TREE_FIRST_LINE;
        (*this_).line_idx_children_start = NAV_TREE_FIRST_LINE;
        (*this_).line_cnt_siblings_to_incl_self = 0;
        (*this_).line_cnt_siblings_after_self = 0;
    }
    (*this_).line_cnt_children = (*this_).children_count;

    TRACE_END();
}

void gui_sketch_nav_tree_invalidate_data( gui_sketch_nav_tree_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).ancestors_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS );
    assert( (*this_).siblings_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS );
    assert( (*this_).children_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN );

    /* clear data */
    for ( int anc_index = 0; anc_index < (*this_).ancestors_count; anc_index ++ )
    {
        data_diagram_destroy( &((*this_).ancestor_diagrams[anc_index]) );
    }
    (*this_).ancestors_count = 0;

    for ( int sib_index = 0; sib_index < (*this_).siblings_count; sib_index ++ )
    {
        data_diagram_destroy( &((*this_).sibling_diagrams[sib_index]) );
    }
    (*this_).siblings_count = 0;
    (*this_).siblings_self_index = -1;

    for ( int chi_index = 0; chi_index < (*this_).children_count; chi_index ++ )
    {
        data_diagram_destroy( &((*this_).child_diagrams[chi_index]) );
    }
    (*this_).children_count = 0;

    /* clear layout infos */
    (*this_).line_idx_new_child = -1;
    (*this_).line_idx_new_sibling = -1;
    (*this_).line_idx_new_root = -1;
    (*this_).line_idx_ancestors_start = NAV_TREE_FIRST_LINE;
    (*this_).line_idx_siblings_start = NAV_TREE_FIRST_LINE;
    (*this_).line_idx_siblings_next_after_self = NAV_TREE_FIRST_LINE;
    (*this_).line_idx_self = NAV_TREE_FIRST_LINE;
    (*this_).line_idx_children_start = NAV_TREE_FIRST_LINE;
    (*this_).line_cnt_siblings_to_incl_self = 0;
    (*this_).line_cnt_siblings_after_self = 0;
    (*this_).line_cnt_children = 0;
    (*this_).line_cnt_ancestors = 0;

    TRACE_END();
}

gui_error_t gui_sketch_nav_tree_get_gap_info_at_pos ( const gui_sketch_nav_tree_t *this_,
                                                      int32_t x,
                                                      int32_t y,
                                                      data_id_t *out_parent_id,
                                                      int32_t *out_list_order,
                                                      shape_int_rectangle_t *out_gap_line )
{
    TRACE_BEGIN();
    assert ( NULL != out_parent_id );
    assert ( NULL != out_list_order );
    assert ( NULL != out_gap_line );
    assert( (*this_).ancestors_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS );
    assert( (*this_).line_cnt_ancestors <= (*this_).ancestors_count );
    assert( (*this_).siblings_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS );
    assert( (*this_).line_cnt_siblings_to_incl_self + (*this_).line_cnt_siblings_after_self == (*this_).siblings_count );
    assert( (*this_).children_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN );
    assert( (*this_).line_cnt_children == (*this_).children_count );

    gui_error_t ret_error;

    if ( shape_int_rectangle_contains( &((*this_).bounds), x, y ) )
    {
        /* determine index of line, top linie has index 0 */
        int32_t top;
        top = shape_int_rectangle_get_top( &((*this_).bounds) );
        uint32_t gap_index;  /* index of the top-most border is 0, index of the first gap is 1 */
        {
            uint32_t half_line_height = (GUI_SKETCH_NAV_TREE_LINE_HEIGHT/2);
            gap_index = ( y + half_line_height - top ) / GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
        }
        uint32_t gap_depth = 0;  /* tree depth at gap position */

        /* default: no gap */
        ret_error = GUI_ERROR_OUT_OF_BOUNDS;

        /* is this the ancester region ? - note: ancestors have gaps only on top of an ancestor */
        if ( ( gap_index >= (*this_).line_idx_ancestors_start )
            && ( gap_index < (*this_).line_idx_ancestors_start + (*this_).line_cnt_ancestors ))
        {
            gap_depth = gap_index - (*this_).line_idx_ancestors_start;

            /* formula differs from others because backwards order and index 0 reserved or self: */
            uint32_t ancester_idx = (*this_).ancestors_count - 1 - (gap_index - (*this_).line_idx_ancestors_start);

            data_id_reinit( out_parent_id,
                            DATA_TABLE_DIAGRAM,
                            data_diagram_get_parent_row_id( &((*this_).ancestor_diagrams[ancester_idx]) )
                          );
            *out_list_order = 0;
            ret_error = GUI_ERROR_NONE;
        }

        /* is this the children region ? - note: every child has a gap before and after itself */
        else if ( ( gap_index >= (*this_).line_idx_children_start )
            && ( gap_index <= (*this_).line_idx_children_start + (*this_).line_cnt_children )
            && ( (*this_).siblings_self_index >= 0 ) /* self exists */ )
        {
            gap_depth = (*this_).line_cnt_ancestors + 1;

            uint32_t child_idx = gap_index - (*this_).line_idx_children_start;
            /* use self as parent id because this shall work even if there are no children */
            data_id_reinit( out_parent_id,
                            DATA_TABLE_DIAGRAM,
                            data_diagram_get_row_id( &((*this_).sibling_diagrams[(*this_).siblings_self_index]) )
                            );
            if ( (*this_).line_cnt_children == 0 )
            {
                *out_list_order = 0;
            }
            else if ( child_idx == 0 )
            {
                *out_list_order = data_diagram_get_list_order( &((*this_).child_diagrams[child_idx]) ) - 32768;
            }
            else if ( child_idx == (*this_).children_count )
            {
                *out_list_order = data_diagram_get_list_order( &((*this_).child_diagrams[child_idx-1]) ) + 32768;
            }
            else
            {
                *out_list_order = (
                    data_diagram_get_list_order( &((*this_).child_diagrams[child_idx-1]) )
                    + data_diagram_get_list_order( &((*this_).child_diagrams[child_idx]) )
                    ) / 2;
            }
            ret_error = GUI_ERROR_NONE;
        }

        /* is this the first half of the siblings region ? - note: gaps exist on top of each sibling-before-and-self */
        else if ( ( gap_index >= (*this_).line_idx_siblings_start )
            && ( gap_index < (*this_).line_idx_siblings_start + (*this_).line_cnt_siblings_to_incl_self ))
        {
            gap_depth = (*this_).line_cnt_ancestors;

            uint32_t sibl1_idx = gap_index - (*this_).line_idx_siblings_start;
            data_id_reinit( out_parent_id,
                            DATA_TABLE_DIAGRAM,
                            data_diagram_get_parent_row_id( &((*this_).sibling_diagrams[sibl1_idx]) )
                          );
            if ( sibl1_idx == 0 )
            {
                *out_list_order = data_diagram_get_list_order( &((*this_).sibling_diagrams[sibl1_idx]) ) - 32768;
            }
            else
            {
                *out_list_order = (
                    data_diagram_get_list_order( &((*this_).sibling_diagrams[sibl1_idx-1]) )
                    + data_diagram_get_list_order( &((*this_).sibling_diagrams[sibl1_idx]) )
                    ) / 2;
            }
            ret_error = GUI_ERROR_NONE;
        }

        /* is this the second half of the siblings region ? - note: gaps exist on top and bottom of each sibling-after */
        else if ( ( gap_index >= (*this_).line_idx_siblings_next_after_self )
            && ( gap_index <= (*this_).line_idx_siblings_next_after_self + (*this_).line_cnt_siblings_after_self )
            && ( (*this_).line_cnt_siblings_to_incl_self > 0) /* otherwise no self */ )
        {
            gap_depth = (*this_).line_cnt_ancestors;

            uint32_t sibl2_idx = gap_index - (*this_).line_idx_siblings_next_after_self + (*this_).line_cnt_siblings_to_incl_self;
            data_id_reinit( out_parent_id,
                            DATA_TABLE_DIAGRAM,
                            data_diagram_get_parent_row_id( &((*this_).sibling_diagrams[sibl2_idx-1]) )
                          );
            if ( sibl2_idx == (*this_).siblings_count )
            {
                *out_list_order = data_diagram_get_list_order( &((*this_).sibling_diagrams[sibl2_idx-1]) ) + 32768;
            }
            else
            {
                *out_list_order = (
                    data_diagram_get_list_order( &((*this_).sibling_diagrams[sibl2_idx-1]) )
                    + data_diagram_get_list_order( &((*this_).sibling_diagrams[sibl2_idx]) )
                    ) / 2;
            }
            ret_error = GUI_ERROR_NONE;
        }

        /* finally, initialize the gap box */
        {
            int32_t gap_y_top = gap_index*GUI_SKETCH_NAV_TREE_LINE_HEIGHT-1;
            if ( gap_y_top < top )
            {
                gap_y_top = top;
            }
            int32_t gap_x_left = gap_depth * GUI_SKETCH_NAV_TREE_INDENT;
            shape_int_rectangle_init( out_gap_line,
                                      shape_int_rectangle_get_left( &((*this_).bounds) ) + gap_x_left,
                                      gap_y_top,
                                      shape_int_rectangle_get_width( &((*this_).bounds) ) - gap_x_left,
                                      2
                                    );
        }
    }
    else
    {
        ret_error = GUI_ERROR_OUT_OF_BOUNDS;
    }

    TRACE_END_ERR( ret_error );
    return ret_error;
}

void gui_sketch_nav_tree_get_object_id_at_pos ( const gui_sketch_nav_tree_t *this_,
                                                int32_t x,
                                                int32_t y,
                                                data_id_t *out_selected_id )
{
    TRACE_BEGIN();
    assert ( NULL != out_selected_id );
    assert( (*this_).ancestors_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS );
    assert( (*this_).line_cnt_ancestors <= (*this_).ancestors_count );
    assert( (*this_).siblings_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS );
    assert( (*this_).line_cnt_siblings_to_incl_self + (*this_).line_cnt_siblings_after_self == (*this_).siblings_count );
    assert( (*this_).children_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN );
    assert( (*this_).line_cnt_children == (*this_).children_count );

    if ( shape_int_rectangle_contains( &((*this_).bounds), x, y ) )
    {
        /* determine index of line, top linie has index 0 */
        uint32_t line_index;
        {
            int32_t top;
            top = shape_int_rectangle_get_top( &((*this_).bounds) );
            line_index = ( y - top ) / GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
        }

        /* default: no line */
        data_id_reinit_void( out_selected_id );

        /* is this the ancester region ? */
        if ( ( line_index >= (*this_).line_idx_ancestors_start )
            && ( line_index < (*this_).line_idx_ancestors_start + (*this_).line_cnt_ancestors ))
        {
            /* formula differs from others because backwards order and index 0 reserved or self: */
            uint32_t ancester_idx = (*this_).ancestors_count - 1 - (line_index - (*this_).line_idx_ancestors_start);

            data_id_reinit( out_selected_id,
                            DATA_TABLE_DIAGRAM,
                            data_diagram_get_row_id( &((*this_).ancestor_diagrams[ancester_idx]) )
                          );
        }

        /* is this the children region */
        else if ( ( line_index >= (*this_).line_idx_children_start )
            && ( line_index < (*this_).line_idx_children_start + (*this_).line_cnt_children ))
        {
            uint32_t child_idx = line_index - (*this_).line_idx_children_start;
            data_id_reinit( out_selected_id,
                            DATA_TABLE_DIAGRAM,
                            data_diagram_get_row_id( &((*this_).child_diagrams[child_idx]) )
                          );
        }

        /* is this the first half of the siblings region ? */
        else if ( ( line_index >= (*this_).line_idx_siblings_start )
            && ( line_index < (*this_).line_idx_siblings_start + (*this_).line_cnt_siblings_to_incl_self ))
        {
            uint32_t sibl1_idx = line_index - (*this_).line_idx_siblings_start;
            data_id_reinit( out_selected_id,
                            DATA_TABLE_DIAGRAM,
                            data_diagram_get_row_id( &((*this_).sibling_diagrams[sibl1_idx]) )
                          );
        }

        /* is this the second half of the siblings region ? */
        else if ( ( line_index >= (*this_).line_idx_siblings_next_after_self )
            && ( line_index < (*this_).line_idx_siblings_next_after_self + (*this_).line_cnt_siblings_after_self ))
        {
            uint32_t sibl2_idx = line_index - (*this_).line_idx_siblings_next_after_self + (*this_).line_cnt_siblings_to_incl_self;
            data_id_reinit( out_selected_id,
                            DATA_TABLE_DIAGRAM,
                            data_diagram_get_row_id( &((*this_).sibling_diagrams[sibl2_idx]) )
                          );
        }
    }
    else
    {
        data_id_reinit_void( out_selected_id );
    }

    TRACE_END();
}

static const double BLACK_R = 0.0;
static const double BLACK_G = 0.0;
static const double BLACK_B = 0.0;
static const double BLACK_A = 1.0;
static const double GREY_R = 0.8;
static const double GREY_G = 0.8;
static const double GREY_B = 0.8;
static const double GREY_A = 1.0;

void gui_sketch_nav_tree_draw ( gui_sketch_nav_tree_t *this_, gui_marked_set_t *marker, cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != marker );
    assert( NULL != cr );
    assert( (*this_).ancestors_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS );
    assert( (*this_).siblings_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS );
    assert( (*this_).children_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN );

    if ( (*this_).visible )
    {
        const int32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
        const int32_t top = shape_int_rectangle_get_top( &((*this_).bounds) );
        const uint32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );
        const uint32_t height = shape_int_rectangle_get_height( &((*this_).bounds) );

        /* draw the background */
        {
            cairo_set_source_rgba( cr, GREY_R, GREY_G, GREY_B, GREY_A );
            cairo_rectangle ( cr, left, top, width, height );
            cairo_fill (cr);
        }

        /* draw the elements */
        {
            PangoLayout *layout;
            layout = pango_cairo_create_layout (cr);
            pango_layout_set_font_description ( layout, (*this_).standard_font_description );

            /* EXAMPLE: precalculate text dimensions to vertically center the text */
            /*
            pango_layout_set_text ( layout, "PANGO Demo", -1 );
            int text2_width;
            int text2_height;
            pango_layout_get_pixel_size (layout, &text2_width, &text2_height);
            double y_adjust = ( height - text2_height ) / 2.0;
            */

            shape_int_rectangle_t destination_rect;
            if ( (*this_).ancestors_count >= 2 )
            {
                /* there is at least one ancestor (and self) */
                for ( int anc_index = (*this_).ancestors_count-1; anc_index > 0 /* 0 equals self */; anc_index -- )
                {
                    destination_rect = gui_sketch_nav_tree_private_get_ancestor_bounds( this_, anc_index );

                    gui_sketch_marker_prepare_draw( &((*this_).sketch_marker),
                                                    DATA_TABLE_DIAGRAM,
                                                    data_diagram_get_row_id( &((*this_).ancestor_diagrams[anc_index]) ),
                                                    marker,
                                                    destination_rect,
                                                    cr
                                                  );

                    const GdkPixbuf *ancestors_folder_icon;
                    if ( anc_index == 1 )
                    {
                        ancestors_folder_icon = gui_resources_get_navigate_open_folder( (*this_).resources );
                    }
                    else
                    {
                        ancestors_folder_icon = gui_resources_get_navigate_breadcrumb_folder( (*this_).resources );
                    }
                    gui_sketch_nav_tree_private_draw_icon_and_label( this_,
                                                                     ancestors_folder_icon,
                                                                     data_diagram_get_name_ptr( &((*this_).ancestor_diagrams[anc_index]) ),
                                                                     shape_int_rectangle_get_left( &destination_rect ),
                                                                     shape_int_rectangle_get_top( &destination_rect ),
                                                                     layout,
                                                                     cr
                                                                   );
                }
            }

            for ( int sib_index = 0; sib_index < (*this_).siblings_count; sib_index ++ )
            {
                destination_rect = gui_sketch_nav_tree_private_get_sibling_bounds( this_, sib_index );

                gui_sketch_marker_prepare_draw( &((*this_).sketch_marker),
                                                DATA_TABLE_DIAGRAM,
                                                data_diagram_get_row_id( &((*this_).sibling_diagrams[sib_index]) ),
                                                marker,
                                                destination_rect,
                                                cr
                                              );

                const GdkPixbuf *siblings_folder_icon;
                if ( sib_index == (*this_).siblings_self_index )
                {
                    siblings_folder_icon = gui_resources_get_navigate_open_folder( (*this_).resources );
                }
                else
                {
                    siblings_folder_icon = gui_resources_get_navigate_closed_folder( (*this_).resources );
                }
                gui_sketch_nav_tree_private_draw_icon_and_label( this_,
                                                                 siblings_folder_icon,
                                                                 data_diagram_get_name_ptr( &((*this_).sibling_diagrams[sib_index]) ),
                                                                 shape_int_rectangle_get_left( &destination_rect ),
                                                                 shape_int_rectangle_get_top( &destination_rect ),
                                                                 layout,
                                                                 cr
                                                               );
            }
            if ( (*this_).ancestors_count != 1 )  /* create sibling if not root diagram */
            {
                destination_rect = gui_sketch_nav_tree_private_get_sibling_bounds( this_, (*this_).siblings_count );
                cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
                gui_sketch_nav_tree_private_draw_icon_and_label( this_,
                                                                 gui_resources_get_navigate_create_sibling( (*this_).resources ),
                                                                 "",
                                                                 shape_int_rectangle_get_left( &destination_rect ),
                                                                 shape_int_rectangle_get_top( &destination_rect ),
                                                                 layout,
                                                                 cr
                                                               );
            }

            for ( int chi_index = 0; chi_index < (*this_).children_count; chi_index ++ )
            {
                destination_rect = gui_sketch_nav_tree_private_get_child_bounds( this_, chi_index );

                gui_sketch_marker_prepare_draw( &((*this_).sketch_marker),
                                                DATA_TABLE_DIAGRAM,
                                                data_diagram_get_row_id( &((*this_).child_diagrams[chi_index]) ),
                                                marker,
                                                destination_rect,
                                                cr
                                              );

                gui_sketch_nav_tree_private_draw_icon_and_label( this_,
                                                                 gui_resources_get_navigate_closed_folder( (*this_).resources ),
                                                                 data_diagram_get_name_ptr( &((*this_).child_diagrams[chi_index]) ),
                                                                 shape_int_rectangle_get_left( &destination_rect ),
                                                                 shape_int_rectangle_get_top( &destination_rect ),
                                                                 layout,
                                                                 cr
                                                               );
            }
            if ( (*this_).ancestors_count != 0 )  /* create children if parent exists */
            {
                destination_rect = gui_sketch_nav_tree_private_get_child_bounds( this_, (*this_).children_count );
                cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
                gui_sketch_nav_tree_private_draw_icon_and_label( this_,
                                                                 gui_resources_get_navigate_create_child( (*this_).resources ),
                                                                 "",
                                                                 shape_int_rectangle_get_left( &destination_rect ),
                                                                 shape_int_rectangle_get_top( &destination_rect ),
                                                                 layout,
                                                                 cr
                                                               );
            }

            g_object_unref(layout);
        }
    }

    TRACE_END();
}

void gui_sketch_nav_tree_private_draw_icon_and_label( gui_sketch_nav_tree_t *this_,
                                                      const GdkPixbuf *icon_1,
                                                      const char *label_1,
                                                      int x,
                                                      int y,
                                                      PangoLayout *layout,
                                                      cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );
    assert( NULL != layout );
    assert( NULL != icon_1 );
    assert( NULL != label_1 );

    /* determine coodinates */
    double icon_width = gdk_pixbuf_get_width ( icon_1 );
    double icon_height = gdk_pixbuf_get_height ( icon_1 );

    /* draw text first, use the pre-set color and font */
    cairo_move_to ( cr, x+icon_width, y );
    pango_layout_set_text ( layout, label_1, -1 );
    pango_cairo_show_layout ( cr, layout );

    /* draw the icon */
    gdk_cairo_set_source_pixbuf( cr, icon_1, x, y );
    cairo_rectangle ( cr, x, y, x+icon_width, y+icon_height );
    cairo_fill (cr);

    TRACE_END();
}


/*
Copyright 2018-2020 Andreas Warnke

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

