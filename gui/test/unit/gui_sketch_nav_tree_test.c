/* File: gui_sketch_nav_tree_test.c; Copyright and License: see below */

#include "gui_sketch_nav_tree_test.h"
#include "sketch/gui_sketch_nav_tree.h"
#include "shape/shape_int_rectangle.h"
#include "gui_resources.h"
#include "data_diagram.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_get_object_at_pos_on_no_diagram( test_fixture_t *fix );  /* extreme case */
static test_case_result_t test_get_object_at_pos_on_single_diagram( test_fixture_t *fix );  /* no parent, no siblings, no children */
static test_case_result_t test_get_object_at_pos_on_1parent_1child_diagram( test_fixture_t *fix );  /* standard case, but no siblings */
static test_case_result_t test_get_object_at_pos_on_2parent_2siblings_diagram( test_fixture_t *fix );  /* standard case, but no children */

test_suite_t gui_sketch_nav_tree_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "gui_sketch_nav_tree_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_get_object_at_pos_on_no_diagram", &test_get_object_at_pos_on_no_diagram );
    test_suite_add_test_case( &result, "test_get_object_at_pos_on_single_diagram", &test_get_object_at_pos_on_single_diagram );
    test_suite_add_test_case( &result, "test_get_object_at_pos_on_1parent_1child_diagram", &test_get_object_at_pos_on_1parent_1child_diagram );
    test_suite_add_test_case( &result, "test_get_object_at_pos_on_2parent_2siblings_diagram", &test_get_object_at_pos_on_2parent_2siblings_diagram );
    return result;
}

static const int LEFT = 100;
static const int TOP = 57;
static const int WIDTH = 50;
static const int HEIGHT = 400;

struct test_fixture_struct {
    gui_resources_t res;
    shape_int_rectangle_t bounds;
    data_diagram_t self;
    data_diagram_t parent;
    data_diagram_t child;
    data_diagram_t other_child;

    /* cairo and pango objects */
    cairo_surface_t *surface;
    cairo_t *cr;
    PangoLayout *font_layout;
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    /* init cairo and pango objects */
    {
        (*fix).surface = cairo_image_surface_create( CAIRO_FORMAT_ARGB32, 640, 480 );
        TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == cairo_surface_status( (*fix).surface ) );
        (*fix).cr = cairo_create( (*fix).surface );
        TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == cairo_status( (*fix).cr ) );
        (*fix).font_layout = pango_cairo_create_layout( (*fix).cr );
    }

    gui_resources_init( &((*fix).res) );
    shape_int_rectangle_init( &((*fix).bounds), LEFT, TOP, WIDTH, HEIGHT );
    data_diagram_init_empty( &((*fix).parent) );
    data_diagram_set_row_id( &((*fix).parent), 1000 );
    data_diagram_init_empty( &((*fix).self) );
    data_diagram_set_row_id( &((*fix).self), 1001 );
    data_diagram_set_parent_row_id( &((*fix).self), 1000 );
    data_diagram_init_empty( &((*fix).child) );
    data_diagram_set_row_id( &((*fix).child), 1002 );
    data_diagram_set_parent_row_id( &((*fix).child), 1001 );
    data_diagram_init_empty( &((*fix).other_child) );
    data_diagram_set_row_id( &((*fix).other_child), 1003 );
    data_diagram_set_parent_row_id( &((*fix).other_child), 1001 );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    data_diagram_destroy( &((*fix).parent) );
    data_diagram_destroy( &((*fix).self) );
    data_diagram_destroy( &((*fix).child) );
    data_diagram_destroy( &((*fix).other_child) );
    shape_int_rectangle_destroy( &((*fix).bounds) );
    gui_resources_destroy( &((*fix).res) );

    /* destroy cairo and pango objects */
    {
        g_object_unref( (*fix).font_layout );
        cairo_destroy( (*fix).cr );
        cairo_surface_finish( (*fix).surface );
        cairo_surface_destroy( (*fix).surface );
    }
}

test_case_result_t get_node_coords( const gui_sketch_nav_tree_t *testee, uint32_t idx, int32_t *out_x, int32_t *out_y )
{
    const uint32_t cnt = gui_sketch_nav_tree_get_node_count( testee );
    TEST_EXPECT( idx < cnt );
    const pos_nav_tree_node_t *pos = gui_sketch_nav_tree_get_node_pos_const( testee, idx );
    const shape_int_rectangle_t *pos_icon = pos_nav_tree_node_get_icon_box_const( pos );
    *out_x = shape_int_rectangle_get_left(pos_icon) + 1;
    *out_y = shape_int_rectangle_get_top(pos_icon) + 1;
    /* printf("x:%d,y:%d\n",*out_x,*out_y); */
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_get_object_at_pos_on_no_diagram( test_fixture_t *fix )
{
    assert( fix != NULL );

    /* init the testee, because a gui_sketch_nav_tree_t contains some hundred diagrams, each abobe 10kB */
    static gui_sketch_nav_tree_t testee;
    {
        gui_sketch_nav_tree_init( &testee, &((*fix).res) );
        gui_sketch_nav_tree_set_bounds( &testee, ((*fix).bounds) );
        gui_sketch_nav_tree_do_layout( &testee, (*fix).cr );
    }

    /* get position */
    int32_t x;
    int32_t y;
    const test_case_result_t res = get_node_coords( &testee, 0, &x, &y );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    TEST_EXPECT_EQUAL_INT( 1, gui_sketch_nav_tree_get_node_count( &testee ) );

    /* test button type */
    gui_sketch_action_t action_id;

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x, y, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x, (y-2), &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT-1, y, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    /* test object id */
    data_id_t selected_id;
    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x, y, &selected_id );
    TEST_EXPECT( ! data_id_is_valid(&selected_id) );

    /* test gap info */
    gui_error_t g_err;
    data_id_t parent_id;
    int32_t list_order;
    shape_int_rectangle_t gap_line;

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x, y, &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_OUT_OF_BOUNDS, g_err );

    /* destroy the testee */
    gui_sketch_nav_tree_destroy( &testee );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_get_object_at_pos_on_single_diagram( test_fixture_t *fix )
{
    assert( fix != NULL );

    /* init the testee, because a gui_sketch_nav_tree_t contains some hundred diagrams, each abobe 10kB */
    static gui_sketch_nav_tree_t testee;
    {
        gui_sketch_nav_tree_init( &testee, &((*fix).res) );
        gui_sketch_nav_tree_set_bounds( &testee, (*fix).bounds );
        {
            testee.ancestors_count = 1;
            testee.ancestor_diagrams[0] = (*fix).parent;
            testee.siblings_count = 1;
            testee.sibling_diagrams[0] = (*fix).parent;
            testee.siblings_self_index = 0;
        }
        gui_sketch_nav_tree_do_layout( &testee, (*fix).cr );
    }

    /* node positions */
    test_case_result_t res;
    int32_t x0;
    int32_t y0;
    res = get_node_coords( &testee, 0, &x0, &y0 );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    int32_t x1;
    int32_t y1;
    res = get_node_coords( &testee, 1, &x1, &y1 );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    TEST_EXPECT_EQUAL_INT( 2, gui_sketch_nav_tree_get_node_count( &testee ) );

    /* test button type */
    gui_sketch_action_t action_id;

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x0, y0, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x1, y1, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM, action_id );

    /* test object id */
    data_id_t selected_id;

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x0, y0, &selected_id );
    TEST_EXPECT( data_id_is_valid(&selected_id) );
    TEST_EXPECT_EQUAL_INT( 1000, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x1, y1, &selected_id );
    TEST_EXPECT( ! data_id_is_valid(&selected_id) );

    /* test gap info */
    gui_error_t g_err;
    data_id_t parent_id;
    int32_t list_order;
    shape_int_rectangle_t gap_line;

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x0, (y0-2), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT( ! data_id_is_valid(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x1, (y1-2), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT( data_id_is_valid(&parent_id) );
    TEST_EXPECT_EQUAL_INT( 1000, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x1, y1+(y1-y0), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT( data_id_is_valid(&parent_id) );
    TEST_EXPECT_EQUAL_INT( 1000, data_id_get_row_id(&parent_id) );  /* same gap as above */

    /* destroy the testee */
    gui_sketch_nav_tree_destroy( &testee );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_get_object_at_pos_on_1parent_1child_diagram( test_fixture_t *fix )
{
    assert( fix != NULL );

    /* init the testee, because a gui_sketch_nav_tree_t contains some hundred diagrams, each abobe 10kB */
    static gui_sketch_nav_tree_t testee;
    {
        gui_sketch_nav_tree_init( &testee, &((*fix).res) );
        gui_sketch_nav_tree_set_bounds( &testee, (*fix).bounds );
        {
            testee.ancestors_count = 2;
            testee.ancestor_diagrams[0] = (*fix).self;
            testee.ancestor_diagrams[1] = (*fix).parent;
            testee.siblings_count = 1;
            testee.sibling_diagrams[0] = (*fix).self;
            testee.siblings_self_index = 0;
            testee.children_count = 1;
            testee.child_diagrams[0] = (*fix).child;
        }
        gui_sketch_nav_tree_do_layout( &testee, (*fix).cr );
    }

    /* node positions */
    test_case_result_t res;
    int32_t x0;
    int32_t y0;
    res = get_node_coords( &testee, 0, &x0, &y0 );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    int32_t x1;
    int32_t y1;
    res = get_node_coords( &testee, 1, &x1, &y1 );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    int32_t x2;
    int32_t y2;
    res = get_node_coords( &testee, 2, &x2, &y2 );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    int32_t x3;
    int32_t y3;
    res = get_node_coords( &testee, 3, &x3, &y3 );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    int32_t x4;
    int32_t y4;
    res = get_node_coords( &testee, 4, &x4, &y4 );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    TEST_EXPECT_EQUAL_INT( 5, gui_sketch_nav_tree_get_node_count( &testee ) );

    /* test button type */
    gui_sketch_action_t action_id;

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x0, y0, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x1, y1, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x2, y2, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x3, y3, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x4, y4, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NEW_SIBLING_DIAGRAM, action_id );

    /* test object id */
    data_id_t selected_id;

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x0, y0, &selected_id );
    TEST_EXPECT_EQUAL_INT( 1000, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x1, y1, &selected_id );
    TEST_EXPECT_EQUAL_INT( 1001, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x2, y2, &selected_id );
    TEST_EXPECT_EQUAL_INT( 1002, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x3, y3, &selected_id );
    TEST_EXPECT( ! data_id_is_valid(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x4, y4, &selected_id );
    TEST_EXPECT( ! data_id_is_valid(&selected_id) );

    /* test gap info */
    gui_error_t g_err;
    data_id_t parent_id;
    int32_t list_order;
    shape_int_rectangle_t gap_line;

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x0, (y0-2), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT( ! data_id_is_valid(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x1, (y1-2), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT_EQUAL_INT( 1000, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x2, (y2-2), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT_EQUAL_INT( 1001, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x3, (y3-2), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT_EQUAL_INT( 1001, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x4, (y4-2), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT_EQUAL_INT( 1000, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x4, y4+(y4-y3), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT_EQUAL_INT( 1000, data_id_get_row_id(&parent_id) );  /* same gap as above */

    /* destroy the testee */
    gui_sketch_nav_tree_destroy( &testee );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_get_object_at_pos_on_2parent_2siblings_diagram( test_fixture_t *fix )
{
    assert( fix != NULL );

    /* init the testee, because a gui_sketch_nav_tree_t contains some hundred diagrams, each abobe 10kB */
    static gui_sketch_nav_tree_t testee;
    {
        gui_sketch_nav_tree_init( &testee, &((*fix).res) );
        gui_sketch_nav_tree_set_bounds( &testee, ((*fix).bounds) );
        {
            testee.ancestors_count = 3;
            testee.ancestor_diagrams[0] = (*fix).child;
            testee.ancestor_diagrams[1] = (*fix).self;
            testee.ancestor_diagrams[2] = (*fix).parent;
            testee.siblings_count = 3;
            testee.sibling_diagrams[0] = (*fix).other_child;
            testee.sibling_diagrams[1] = (*fix).child;
            testee.sibling_diagrams[2] = (*fix).other_child;
            testee.siblings_self_index = 1;
        }
        gui_sketch_nav_tree_do_layout( &testee, (*fix).cr );
    }

    /* node positions */
    test_case_result_t res;
    int32_t x0;
    int32_t y0;
    res = get_node_coords( &testee, 0, &x0, &y0 );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    int32_t x1;
    int32_t y1;
    res = get_node_coords( &testee, 1, &x1, &y1 );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    int32_t x2;
    int32_t y2;
    res = get_node_coords( &testee, 2, &x2, &y2 );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    int32_t x3;
    int32_t y3;
    res = get_node_coords( &testee, 3, &x3, &y3 );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    int32_t x4;
    int32_t y4;
    res = get_node_coords( &testee, 4, &x4, &y4 );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    int32_t x5;
    int32_t y5;
    res = get_node_coords( &testee, 5, &x5, &y5 );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    int32_t x6;
    int32_t y6;
    res = get_node_coords( &testee, 6, &x6, &y6 );
    TEST_EXPECT_EQUAL_INT( TEST_CASE_RESULT_OK, res );
    TEST_EXPECT_EQUAL_INT( 7, gui_sketch_nav_tree_get_node_count( &testee ) );

    /* test button type */
    gui_sketch_action_t action_id;

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x0, y0, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x1, y1, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x2, y2, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x3, y3, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x4, y4, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x5, y5, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x6, y6, &action_id );
    TEST_EXPECT_EQUAL_INT( GUI_SKETCH_ACTION_NEW_SIBLING_DIAGRAM, action_id );

    /* test object id */
    data_id_t selected_id;

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x0, y0, &selected_id );
    TEST_EXPECT_EQUAL_INT( 1000, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x1, y1, &selected_id );
    TEST_EXPECT_EQUAL_INT( 1001, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x2, y2, &selected_id );
    TEST_EXPECT_EQUAL_INT( 1003, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x3, y3, &selected_id );
    TEST_EXPECT_EQUAL_INT( 1002, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x4, y4, &selected_id );
    TEST_EXPECT( ! data_id_is_valid(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x5, y5, &selected_id );
    TEST_EXPECT_EQUAL_INT( 1003, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x6, y6, &selected_id );
    TEST_EXPECT( ! data_id_is_valid(&selected_id) );

    /* test gap info */
    gui_error_t g_err;
    data_id_t parent_id;
    int32_t list_order;
    shape_int_rectangle_t gap_line;

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x0, (y0-2), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT( ! data_id_is_valid(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x1, (y1-2), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT_EQUAL_INT( 1000, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x2, (y2-2), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT_EQUAL_INT( 1001, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x3, (y3-2), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT_EQUAL_INT( 1001, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x4, (y4-2), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT_EQUAL_INT( 1002, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x5, (y5-2), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT_EQUAL_INT( 1001, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x6, (y6-2), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT_EQUAL_INT( 1001, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x6, y6+(y6-y5), &parent_id, &list_order, &gap_line );
    TEST_EXPECT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_EXPECT_EQUAL_INT( 1001, data_id_get_row_id(&parent_id) );  /* same gap as above */

    /* destroy the testee */
    gui_sketch_nav_tree_destroy( &testee );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2019-2024 Andreas Warnke
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
