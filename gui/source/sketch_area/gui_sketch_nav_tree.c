/* File: gui_sketch_nav_tree.c; Copyright and License: see below */

#include "sketch_area/gui_sketch_nav_tree.h"
#include "geometry/geometry_rectangle.h"
#include "gui_sketch_int_compare.h"
#include "trace/trace.h"
#include "tslog/tslog.h"
#include <gdk/gdk.h>

static const int GUI_SKETCH_NAV_TREE_INDENT = 12;
static const int OBJ_GAP = 3;
static const int GAP_HEIGHT = 2;
static const int GUI_SKETCH_NAV_TREE_PANGO_AUTO_DETECT_LENGTH = -1;  /*!< pango automatically determines the string length */

void gui_sketch_nav_tree_init( gui_sketch_nav_tree_t *this_, gui_resources_t *resources )
{
    TRACE_BEGIN();
    assert( resources != NULL );

    (*this_).ancestors_count = 0;
    (*this_).siblings_count = 0;
    (*this_).children_count = 0;
    (*this_).siblings_self_index = -1;

    (*this_).node_count = 0;
    (*this_).gap_count = 0;

    (*this_).visible = false;
    shape_int_rectangle_init( &((*this_).bounds), 0, 0, 0, 0 );

    gui_sketch_style_init( &((*this_).sketch_style) );
    gui_sketch_marker_init( &((*this_).sketch_marker), true );
    (*this_).resources = resources;

    TRACE_END();
}

void gui_sketch_nav_tree_destroy( gui_sketch_nav_tree_t *this_ )
{
    TRACE_BEGIN();

    (*this_).resources = NULL;
    gui_sketch_marker_destroy( &((*this_).sketch_marker) );
    gui_sketch_style_destroy( &((*this_).sketch_style) );

    gui_sketch_nav_tree_invalidate_data( this_ );

    shape_int_rectangle_destroy( &((*this_).bounds) );

    TRACE_END();
}

void gui_sketch_nav_tree_load_data( gui_sketch_nav_tree_t *this_, data_row_id_t diagram_id, data_database_reader_t *db_reader )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );

    gui_sketch_nav_tree_invalidate_data( this_ );

    u8_error_t db_err = U8_ERROR_NONE;

    /* get ancestors */
    bool finished = false;
    for ( unsigned int anc_index = 0; ( anc_index < GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS ) && ( db_err == U8_ERROR_NONE ) && ( ! finished ); anc_index ++ )
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
            if ( db_err == U8_ERROR_NONE )
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
        db_err = U8_ERROR_INVALID_REQUEST;
        TSLOG_ANOMALY_INT( "gui_sketch_nav_tree_load_data cannot load diagram", diagram_id );
    }

    /* get siblings */
    if ( db_err == U8_ERROR_NONE )
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
    if ( db_err == U8_ERROR_NONE )
    {

        db_err = data_database_reader_get_diagrams_by_parent_id ( db_reader,
                                                                  diagram_id,
                                                                  GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN,
                                                                  &( (*this_).child_diagrams ),
                                                                  &( (*this_).children_count )
                                                                );
    }

    /* invalidate layout positions */
    {
        (*this_).node_count = 0;
        (*this_).gap_count = 0;
    }

    TRACE_END();
}

void gui_sketch_nav_tree_do_layout( gui_sketch_nav_tree_t *this_, cairo_t *cr )
{
    TRACE_BEGIN();
    assert( (*this_).ancestors_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS );
    assert( (*this_).siblings_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS );
    assert( (*this_).children_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN );

    /* create the font_layout */
    PangoLayout *font_layout;
    {
        font_layout = pango_cairo_create_layout (cr);
        const PangoFontDescription *const std_font
            = gui_sketch_style_get_standard_font_description( &((*this_).sketch_style ) );
        pango_layout_set_font_description( font_layout, std_font );
    }

    int32_t y_pos = shape_int_rectangle_get_top( &((*this_).bounds) ) + OBJ_GAP;
    const int_fast32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
    const uint_fast32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );

    (*this_).node_count = 0;
    (*this_).gap_count = 0;

    if ( (*this_).ancestors_count == 0 )
    {
        pos_nav_tree_node_t *const new_root_node = &((*this_).node_pos[0]);
        (*this_).node_count = 1;

        /* show only a new root button */
        assert( 1 <= GUI_SKETCH_NAV_TREE_CONST_MAX_NODES );
        pos_nav_tree_node_init( new_root_node, POS_NAV_TREE_NODE_TYPE_NEW_ROOT, NULL );
        gui_sketch_nav_tree_private_layout_node( this_, new_root_node, 0, &y_pos, font_layout );

        /* no gaps in this case */
        assert( 0 == (*this_).gap_count );
        assert( 1 == (*this_).node_count );
    }
    else
    {
        /* layout ancestors excluding self (at index 0 in ancestor_diagrams) */
        const unsigned int anc_count = (*this_).ancestors_count;
        assert( anc_count > 0 );
        assert( anc_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS );
        assert( (*this_).node_count + (anc_count-1) <= GUI_SKETCH_NAV_TREE_CONST_MAX_NODES );
        for ( unsigned int anc_idx = 1; anc_idx < anc_count; anc_idx ++ )
        {
            const data_diagram_t *const anc_diag = &((*this_).ancestor_diagrams[anc_count-anc_idx]);

            /* layout upper-gap of ancestor */
            {
                pos_nav_tree_gap_t *const upper_gap = &((*this_).gap_pos[(*this_).gap_count]);
                (*this_).gap_count ++;

                pos_nav_tree_gap_init( upper_gap, data_diagram_get_parent_data_id(anc_diag), 0 );
                const int indent = (anc_idx-1) * GUI_SKETCH_NAV_TREE_INDENT;

                pos_nav_tree_gap_set_gap_box_coords( upper_gap, left+indent, y_pos, width-indent, GAP_HEIGHT );
                y_pos += GAP_HEIGHT;
            }

            /* layout ancestor node */
            {
                pos_nav_tree_node_t *const anc_node = &((*this_).node_pos[(*this_).node_count]);
                (*this_).node_count ++;

                const pos_nav_tree_node_type_t n_type
                    = ((anc_idx+1)==anc_count) ? POS_NAV_TREE_NODE_TYPE_OPEN : POS_NAV_TREE_NODE_TYPE_ANCESTOR;
                pos_nav_tree_node_init( anc_node, n_type, anc_diag );
                gui_sketch_nav_tree_private_layout_node( this_, anc_node, (anc_idx-1), &y_pos, font_layout );
            }
        }

        const unsigned int tree_depth = (*this_).ancestors_count-1;

        /* layout siblings including self */
        const unsigned int sibl_count = (*this_).siblings_count;
        assert( sibl_count > 0 );
        assert( sibl_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS );
        const unsigned int child_count = (*this_).children_count;
        assert( child_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN );
        assert( (*this_).node_count + sibl_count + child_count + 2 <= GUI_SKETCH_NAV_TREE_CONST_MAX_NODES );
        int32_t previous_sibl_order = INT32_MIN;
        for ( unsigned int sibl_idx = 0; sibl_idx < sibl_count; sibl_idx ++ )
        {
            const data_diagram_t *const sibl_diag = &((*this_).sibling_diagrams[sibl_idx]);
            const bool is_self = ( sibl_idx == (*this_).siblings_self_index );

            /* layout upper-gap of sibling */
            {
                pos_nav_tree_gap_t *const upper_gap = &((*this_).gap_pos[(*this_).gap_count]);
                (*this_).gap_count ++;

                const int32_t gap_sibl_order
                    = (previous_sibl_order/2)+(data_diagram_get_list_order( sibl_diag )/2);  /* no overrun */
                pos_nav_tree_gap_init( upper_gap, data_diagram_get_parent_data_id(sibl_diag), gap_sibl_order );
                const int indent = tree_depth * GUI_SKETCH_NAV_TREE_INDENT;
                pos_nav_tree_gap_set_gap_box_coords( upper_gap, left+indent, y_pos, width-indent, GAP_HEIGHT );
                y_pos += GAP_HEIGHT;
            }

            /* layout sibling node */
            {
                pos_nav_tree_node_t *const sibl_node = &((*this_).node_pos[(*this_).node_count]);
                (*this_).node_count ++;

                const pos_nav_tree_node_type_t n_type = is_self ? POS_NAV_TREE_NODE_TYPE_OPEN : POS_NAV_TREE_NODE_TYPE_CLOSED;
                pos_nav_tree_node_init( sibl_node, n_type, sibl_diag );
                gui_sketch_nav_tree_private_layout_node( this_, sibl_node, tree_depth, &y_pos, font_layout );
            }

            /* layout children */
            if ( is_self )
            {
                int32_t prev_child_order = INT32_MIN;
                for ( unsigned int chld_idx = 0; chld_idx < child_count; chld_idx ++ )
                {
                    const data_diagram_t *const chld_diag = &((*this_).child_diagrams[chld_idx]);

                    /* layout upper-gap of child */
                    {
                        pos_nav_tree_gap_t *const upper_gap = &((*this_).gap_pos[(*this_).gap_count]);
                        (*this_).gap_count ++;

                        const int32_t gap_child_order
                            = (prev_child_order/2)+(data_diagram_get_list_order( chld_diag )/2);  /* no overrun */
                        pos_nav_tree_gap_init( upper_gap, data_diagram_get_parent_data_id(chld_diag), gap_child_order );
                        const int indent = (tree_depth+1) * GUI_SKETCH_NAV_TREE_INDENT;
                        pos_nav_tree_gap_set_gap_box_coords( upper_gap, left+indent, y_pos, width-indent, GAP_HEIGHT );
                        y_pos += GAP_HEIGHT;
                    }

                    /* layout child node */
                    {
                        pos_nav_tree_node_t *const chld_node = &((*this_).node_pos[(*this_).node_count]);
                        (*this_).node_count ++;

                        pos_nav_tree_node_init( chld_node, POS_NAV_TREE_NODE_TYPE_CLOSED, chld_diag );
                        gui_sketch_nav_tree_private_layout_node( this_, chld_node, tree_depth+1, &y_pos, font_layout );
                    }

                    prev_child_order = data_diagram_get_list_order( sibl_diag );
                }

                /* layout lower-gap of child (if any) */
                {
                    pos_nav_tree_gap_t *const lower_gap = &((*this_).gap_pos[(*this_).gap_count]);
                    (*this_).gap_count ++;

                    const int32_t gap_child_order = (prev_child_order/2)+(INT32_MAX/2);  /* no overrun */
                    pos_nav_tree_gap_init( lower_gap, data_diagram_get_data_id(sibl_diag), gap_child_order );
                    const int indent = (tree_depth+1) * GUI_SKETCH_NAV_TREE_INDENT;
                    pos_nav_tree_gap_set_gap_box_coords( lower_gap, left+indent, y_pos, width-indent, GAP_HEIGHT );
                    y_pos += GAP_HEIGHT;
                }

                /* add a new child button */
                {
                    pos_nav_tree_node_t *const new_chld_node = &((*this_).node_pos[(*this_).node_count]);
                    (*this_).node_count ++;

                    pos_nav_tree_node_init( new_chld_node, POS_NAV_TREE_NODE_TYPE_NEW_CHILD, NULL );
                    gui_sketch_nav_tree_private_layout_node( this_, new_chld_node, tree_depth+1, &y_pos, font_layout );
                }
            }

            previous_sibl_order = data_diagram_get_list_order( sibl_diag );
        }

        const bool self_is_root = ( (*this_).ancestors_count == 1 );
        if ( ! self_is_root )
        {
            /* layout lower-gap of siblings */
            const data_diagram_t *const self_diag = &((*this_).ancestor_diagrams[0]);
            {
                pos_nav_tree_gap_t *const lower_gap = &((*this_).gap_pos[(*this_).gap_count]);
                (*this_).gap_count ++;

                const int32_t gap_sibl_order = (previous_sibl_order/2)+(INT32_MAX/2);  /* no overrun */
                pos_nav_tree_gap_init( lower_gap, data_diagram_get_parent_data_id(self_diag), gap_sibl_order );
                const int indent = tree_depth * GUI_SKETCH_NAV_TREE_INDENT;
                pos_nav_tree_gap_set_gap_box_coords( lower_gap, left+indent, y_pos, width-indent, GAP_HEIGHT );
                y_pos += GAP_HEIGHT;
            }

            /* show a new sibling button unless root */
            {
                pos_nav_tree_node_t *const new_sibl_node = &((*this_).node_pos[(*this_).node_count]);
                (*this_).node_count ++;

                pos_nav_tree_node_init( new_sibl_node, POS_NAV_TREE_NODE_TYPE_NEW_SIBLING, NULL );
                gui_sketch_nav_tree_private_layout_node( this_, new_sibl_node, tree_depth, &y_pos, font_layout );
            }
        }

        assert( (*this_).gap_count == (*this_).node_count );
    }

    /* release the font_layout */
    g_object_unref(font_layout);

    TRACE_END();
}

void gui_sketch_nav_tree_private_layout_node ( gui_sketch_nav_tree_t *this_,
                                               pos_nav_tree_node_t *node,
                                               uint32_t tree_depth,
                                               int32_t *io_y_pos,
                                               PangoLayout *font_layout )
{
    TRACE_BEGIN();
    assert( NULL != node );
    assert( NULL != io_y_pos );
    assert( NULL != font_layout );

    const int_fast32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
    const uint_fast32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );
    const uint_fast32_t indent = tree_depth*GUI_SKETCH_NAV_TREE_INDENT;
    const data_diagram_t *data_or_null = pos_nav_tree_node_get_data_const( node );

    /* determine icon dimensions */
    {
        const pos_nav_tree_node_type_t node_type = pos_nav_tree_node_get_type( node );
        const GdkPixbuf *icon = pos_nav_tree_node_type_get_icon( node_type, false, (*this_).resources );
        const double icon_width = gdk_pixbuf_get_width( icon );
        const double icon_height = gdk_pixbuf_get_height( icon );

        const shape_int_rectangle_t new_icon_box = (shape_int_rectangle_t) {
            .left=left+indent+OBJ_GAP,
            .top=(*io_y_pos)+OBJ_GAP,
            .width=icon_width+0.999,
            .height=icon_height+0.999 };
        pos_nav_tree_node_set_icon_box( node, &new_icon_box );
    }

    /* determine label dimensions */
    const shape_int_rectangle_t *const icon_box = pos_nav_tree_node_get_icon_box_const( node );
    shape_int_rectangle_t new_label_box;
    if ( data_or_null == NULL )
    {
        shape_int_rectangle_init( &new_label_box, shape_int_rectangle_get_right(icon_box), (*io_y_pos), 0, 0 );
    }
    else
    {
        int_fast32_t proposed_pango_width = width - indent - shape_int_rectangle_get_width(icon_box) - (4*OBJ_GAP);
        pango_layout_set_text( font_layout,
                               data_diagram_get_name_const( data_or_null ),
                               GUI_SKETCH_NAV_TREE_PANGO_AUTO_DETECT_LENGTH
                             );
        pango_layout_set_width(font_layout, proposed_pango_width * PANGO_SCALE );
        int text_width;
        int text_height;
        pango_layout_get_pixel_size(font_layout, &text_width, &text_height);

        int_fast32_t x_pos = shape_int_rectangle_get_right(icon_box);
        shape_int_rectangle_init( &new_label_box, x_pos+OBJ_GAP, (*io_y_pos)+OBJ_GAP, text_width, text_height );
    }
    pos_nav_tree_node_set_label_box( node, &new_label_box );

    *io_y_pos
        = gui_sketch_int_compare_max_i32( shape_int_rectangle_get_bottom(icon_box), shape_int_rectangle_get_bottom(&new_label_box) )
        + OBJ_GAP;

    shape_int_rectangle_destroy( &new_label_box );

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
    assert( (*this_).node_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_NODES );
    for ( int node_index = 0; node_index < (*this_).node_count; node_index ++ )
    {
        pos_nav_tree_node_destroy( &((*this_).node_pos[node_index]) );
    }
    (*this_).node_count = 0;

    assert( (*this_).gap_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_GAPS );
    for ( int gap_index = 0; gap_index < (*this_).gap_count; gap_index ++ )
    {
        pos_nav_tree_gap_destroy( &((*this_).gap_pos[gap_index]) );
    }
    (*this_).gap_count = 0;

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

    gui_error_t ret_error;

    /* search closest gap */
    if ( shape_int_rectangle_contains( &((*this_).bounds), x, y ) )
    {
        const unsigned int gap_count = (*this_).gap_count;
        assert( gap_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_GAPS );
        if ( gap_count == 0 )
        {
            ret_error = GUI_ERROR_OUT_OF_BOUNDS;
        }
        else
        {
            const pos_nav_tree_gap_t *closest = &((*this_).gap_pos[0]);
            int closest_dist = INT32_MAX;
            for ( unsigned int idx = 0; idx < gap_count; idx ++ )
            {
                const pos_nav_tree_gap_t *const current = &((*this_).gap_pos[idx]);
                const shape_int_rectangle_t *const current_box = pos_nav_tree_gap_get_gap_box_const( current );
                const int current_dist
                    = ( y < shape_int_rectangle_get_top( current_box ) )
                    ? ( shape_int_rectangle_get_top( current_box ) - y )
                    : (( y > shape_int_rectangle_get_bottom( current_box ) )
                    ? ( y - shape_int_rectangle_get_bottom( current_box ) )
                    : 0 );
                if ( current_dist < closest_dist )
                {
                    closest = current;
                    closest_dist = current_dist;
                }
            }
            *out_parent_id = pos_nav_tree_gap_get_parent_id( closest );
            *out_list_order = pos_nav_tree_gap_get_list_order( closest );
            shape_int_rectangle_replace( out_gap_line, pos_nav_tree_gap_get_gap_box_const( closest ) );
            ret_error = GUI_ERROR_NONE;
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

    /* default in case no object found */
    {
        data_id_reinit_void( out_selected_id );
    }

    /* search object */
    if ( shape_int_rectangle_contains( &((*this_).bounds), x, y ) )
    {
        const unsigned int count = (*this_).node_count;
        assert( count <= GUI_SKETCH_NAV_TREE_CONST_MAX_NODES );
        for ( unsigned int idx = 0; idx < count; idx ++ )
        {
            const pos_nav_tree_node_t *const node = &((*this_).node_pos[idx]);
            const shape_int_rectangle_t *icon_box = pos_nav_tree_node_get_icon_box_const( node );
            const shape_int_rectangle_t *label_box = pos_nav_tree_node_get_label_box_const( node );

            if ( shape_int_rectangle_contains( icon_box, x, y ) || shape_int_rectangle_contains( label_box, x, y ) )
            {
                /* const pos_nav_tree_node_type_t node_type = pos_nav_tree_node_get_type( node ); */
                const data_diagram_t *const data_or_null = pos_nav_tree_node_get_data_const( node );
                if ( data_or_null != NULL )
                {
                    *out_selected_id = data_diagram_get_data_id( data_or_null );
                    break;
                }
            }
        }
    }

    TRACE_END();
}

static const double GREY_R = 0.8;
static const double GREY_G = 0.8;
static const double GREY_B = 0.8;
static const double GREY_A = 1.0;

void gui_sketch_nav_tree_draw ( gui_sketch_nav_tree_t *this_, gui_marked_set_t *marker, cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != marker );
    assert( NULL != cr );

    if ( (*this_).visible )
    {
        PangoLayout *font_layout = pango_cairo_create_layout (cr);
        {
            const PangoFontDescription *const std_font
                = gui_sketch_style_get_standard_font_description( &((*this_).sketch_style ) );
            pango_layout_set_font_description ( font_layout, std_font );
        }

        /* draw background */
        {
            const int_fast32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
            const int_fast32_t top = shape_int_rectangle_get_top( &((*this_).bounds) );
            const uint_fast32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );
            const uint_fast32_t height = shape_int_rectangle_get_height( &((*this_).bounds) );

            cairo_set_source_rgba( cr, GREY_R, GREY_G, GREY_B, GREY_A );
            cairo_rectangle ( cr, left, top, width, height );
            cairo_fill (cr);
        }

        /* draw icons and text */
        const unsigned int count = (*this_).node_count;
        assert( count <= GUI_SKETCH_NAV_TREE_CONST_MAX_NODES );
        for ( unsigned int idx = 0; idx < count; idx ++ )
        {
            const pos_nav_tree_node_t *const node = &((*this_).node_pos[idx]);
            gui_sketch_nav_tree_private_draw_node( this_, node, marker, font_layout, cr );
        }

        g_object_unref(font_layout);
    }

    TRACE_END();
}

void gui_sketch_nav_tree_private_draw_node( gui_sketch_nav_tree_t *this_,
                                            const pos_nav_tree_node_t *node,
                                            const gui_marked_set_t *marker,
                                            PangoLayout *font_layout,
                                            cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );
    assert( NULL != node );
    assert( NULL != marker );
    assert( NULL != font_layout );

    const data_diagram_t *const diag_or_null = pos_nav_tree_node_get_data_const( node );

    /* draw marker and set color */
    if ( diag_or_null != NULL )
    {
        shape_int_rectangle_t destination_rect;
        shape_int_rectangle_init_by_bounds( &destination_rect,
                                            pos_nav_tree_node_get_icon_box_const(node),
                                            pos_nav_tree_node_get_label_box_const(node)
                                          );

        gui_sketch_marker_prepare_draw( &((*this_).sketch_marker),
                                        data_diagram_get_data_id( diag_or_null ),
                                        marker,
                                        destination_rect,
                                        cr
                                      );

        shape_int_rectangle_destroy( &destination_rect );
    }
    else
    {
        const GdkRGBA std_color = gui_sketch_style_get_standard_color( &((*this_).sketch_style) );
        cairo_set_source_rgba( cr, std_color.red, std_color.green, std_color.blue, std_color.alpha );
    }

    /* draw text first, use the above set color and font */
    if ( diag_or_null != NULL )
    {
        /* what to draw */
        const char *const label = data_diagram_get_name_const( diag_or_null );

        /* where to draw to */
        const shape_int_rectangle_t *const label_box
            = pos_nav_tree_node_get_label_box_const( node );

        /* do draw */
        cairo_move_to( cr, shape_int_rectangle_get_left(label_box), shape_int_rectangle_get_top(label_box) );
        pango_layout_set_text( font_layout, label, GUI_SKETCH_NAV_TREE_PANGO_AUTO_DETECT_LENGTH );
        const unsigned int text_width
            = shape_int_rectangle_get_width(label_box)
            +(2.0*OBJ_GAP);  /* add gap to avoid line breaks by rounding errors and whitespace character widths */
        pango_layout_set_width(font_layout, text_width * PANGO_SCALE );
        pango_cairo_show_layout( cr, font_layout );
    }

    /* draw the icon */
    {
        /* what to draw */
        const pos_nav_tree_node_type_t node_type = pos_nav_tree_node_get_type( node );
        const gui_sketch_action_t btn_act = gui_marked_set_get_highlighted_button( marker );
        const bool highlight
            = (( node_type == POS_NAV_TREE_NODE_TYPE_NEW_ROOT )&&( btn_act == GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM ))
            || (( node_type == POS_NAV_TREE_NODE_TYPE_NEW_SIBLING )&&( btn_act == GUI_SKETCH_ACTION_NEW_SIBLING_DIAGRAM ))
            || (( node_type == POS_NAV_TREE_NODE_TYPE_NEW_CHILD )&&( btn_act == GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM ));
        const GdkPixbuf *icon = pos_nav_tree_node_type_get_icon( node_type, highlight, (*this_).resources );

        /* where to draw to */
        const shape_int_rectangle_t *const icon_box
            = pos_nav_tree_node_get_icon_box_const( node );
        const int x = shape_int_rectangle_get_left(icon_box);
        const int y = shape_int_rectangle_get_top(icon_box);
        const double icon_width = gdk_pixbuf_get_width ( icon );
        const double icon_height = gdk_pixbuf_get_height ( icon );

        /* do draw */
        gdk_cairo_set_source_pixbuf( cr, icon, x, y );
        cairo_rectangle ( cr, x, y, x+icon_width, y+icon_height );
        cairo_fill (cr);
    }

    TRACE_END();
}


/*
Copyright 2018-2022 Andreas Warnke

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

