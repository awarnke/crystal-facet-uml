/* File: gui_resources.c; Copyright and License: see below */

#include "gui_resources.h"
#include "u8/u8_trace.h"
#include "gui_gtk.h"
#include <stdbool.h>

/* including resource files */

#include "resources/crystal_facet_uml.c"
#include "resources/edit_attributes_sect.c"

#include "resources/file_new.c"
#include "resources/file_open.c"
#include "resources/file_save.c"
#include "resources/file_saved.c"
#include "resources/file_unsaved.c"
#include "resources/file_export.c"

#include "resources/view_create.c"
#include "resources/view_edit.c"
#include "resources/view_navigate.c"
#include "resources/view_new_window.c"
#include "resources/view_search.c"
#include "resources/view_search_now.c"

#include "resources/tool_sect.c"

#include "resources/edit_copy.c"
#include "resources/edit_cut.c"
#include "resources/edit_delete.c"
#include "resources/edit_paste.c"
#include "resources/edit_instantiate.c"
#include "resources/edit_highlight.c"
#include "resources/edit_redo.c"
#include "resources/edit_undo.c"
#include "resources/edit_reset.c"

#include "resources/message_error.c"
#include "resources/message_info.c"
#include "resources/message_warn.c"
#include "resources/message_user_doc.c"

#include "resources/navigate_breadcrumb_folder.c"
#include "resources/navigate_closed_folder.c"
#include "resources/navigate_create_child.c"
#include "resources/navigate_create_child_0.c"
#include "resources/navigate_create_sibling.c"
#include "resources/navigate_create_sibling_0.c"
#include "resources/navigate_open_folder.c"

#include "resources/sketch_background.c"
#include "resources/sketch_create.c"
#include "resources/sketch_refine.c"
#include "resources/sketch_relate.c"
#include "resources/sketch_move_h.c"
#include "resources/sketch_move_v.c"
#include "resources/sketch_move_2d.c"
#include "resources/sketch_page_down_bold.c"
#include "resources/sketch_page_down_gray.c"
#include "resources/sketch_page_up_bold.c"
#include "resources/sketch_page_up_gray.c"

#include "resources/type_undef.c"
#include "resources/type_diag_activity.c"
#include "resources/type_diag_block.c"
#include "resources/type_diag_box.c"
#include "resources/type_diag_class.c"
#include "resources/type_diag_communication.c"
#include "resources/type_diag_component.c"
#include "resources/type_diag_composite.c"
#include "resources/type_diag_deployment.c"
#include "resources/type_diag_internal.c"
#include "resources/type_diag_list.c"
#include "resources/type_diag_overview.c"
#include "resources/type_diag_package.c"
#include "resources/type_diag_parametric.c"
#include "resources/type_diag_profile.c"
#include "resources/type_diag_requirement.c"
#include "resources/type_diag_sequence.c"
#include "resources/type_diag_state.c"
#include "resources/type_diag_timing.c"
#include "resources/type_diag_usecase.c"

#include "resources/type_clas_accept.c"
#include "resources/type_clas_activity.c"
#include "resources/type_clas_actor.c"
#include "resources/type_clas_artifact.c"
#include "resources/type_clas_block.c"
#include "resources/type_clas_class.c"
#include "resources/type_clas_comment.c"
#include "resources/type_clas_component.c"
#include "resources/type_clas_constraint.c"
#include "resources/type_clas_decision.c"
#include "resources/type_clas_deephistory.c"
#include "resources/type_clas_final.c"
#include "resources/type_clas_fork.c"
#include "resources/type_clas_history.c"
#include "resources/type_clas_image.c"
#include "resources/type_clas_initial.c"
#include "resources/type_clas_interaction_use.c"
#include "resources/type_clas_interface.c"
#include "resources/type_clas_join.c"
#include "resources/type_clas_node.c"
#include "resources/type_clas_object.c"
#include "resources/type_clas_package.c"
#include "resources/type_clas_part.c"
#include "resources/type_clas_region.c"
#include "resources/type_clas_requirement.c"
#include "resources/type_clas_send.c"
#include "resources/type_clas_state.c"
#include "resources/type_clas_stereotype.c"
#include "resources/type_clas_subsystem.c"
#include "resources/type_clas_time.c"
#include "resources/type_clas_usecase.c"

#include "resources/type_feat_entry.c"
#include "resources/type_feat_exit.c"
#include "resources/type_feat_life.c"
#include "resources/type_feat_operation.c"
#include "resources/type_feat_port.c"
#include "resources/type_feat_property.c"
#include "resources/type_feat_provide.c"
#include "resources/type_feat_require.c"
#include "resources/type_feat_rx.c"
#include "resources/type_feat_tag.c"
#include "resources/type_feat_tx.c"

#include "resources/type_rel_aggregate.c"
#include "resources/type_rel_associate.c"
#include "resources/type_rel_async.c"
#include "resources/type_rel_compose.c"
#include "resources/type_rel_communication_path.c"
#include "resources/type_rel_contain.c"
#include "resources/type_rel_control_flow.c"
#include "resources/type_rel_depend.c"
#include "resources/type_rel_deploy.c"
#include "resources/type_rel_extend.c"
#include "resources/type_rel_generalize.c"
#include "resources/type_rel_include.c"
#include "resources/type_rel_manifest.c"
#include "resources/type_rel_object_flow.c"
#include "resources/type_rel_realize.c"
#include "resources/type_rel_refine.c"
#include "resources/type_rel_return.c"
#include "resources/type_rel_sync.c"
#include "resources/type_rel_trace.c"

#define GIMP_PIXBUF_DATA(STRUCTNAME) &(STRUCTNAME.pixel_data[0]), GDK_COLORSPACE_RGB, true /* alpha */, 8, \
                                     STRUCTNAME.width, STRUCTNAME.height, STRUCTNAME.width * STRUCTNAME.bytes_per_pixel, \
                                     NULL, NULL

GdkTexture *gui_resources_new_texture_from_pixbuf_data( const guchar* data,
                                                        GdkColorspace colorspace,
                                                        gboolean has_alpha,
                                                        int bits_per_sample,
                                                        int width,
                                                        int height,
                                                        int rowstride,
                                                        GdkPixbufDestroyNotify destroy_fn,
                                                        gpointer destroy_fn_data
                                                      )
{
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data( data, colorspace, has_alpha, bits_per_sample, width, height, rowstride, destroy_fn, destroy_fn_data );
    GdkTexture *result = gdk_texture_new_for_pixbuf( pixbuf );
    g_object_unref( pixbuf );
    return result;
}

void gui_resources_init ( gui_resources_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).crystal_facet_uml = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( crystal_facet_uml ) );

    (*this_).edit_attributes_sect = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( edit_attributes_sect ) );

    (*this_).file_new = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( file_new ) );
    (*this_).file_open = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( file_open ) );
    (*this_).file_save = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( file_save ) );
    (*this_).file_saved = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( file_saved ) );
    (*this_).file_unsaved = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( file_unsaved ) );
    (*this_).file_export = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( file_export ) );

    (*this_).view_new_window = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( view_new_window ) );
    (*this_).view_search = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( view_search ) );
    (*this_).view_search_now = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( view_search_now ) );
    (*this_).view_navigate = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( view_navigate ) );
    (*this_).view_create = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( view_create ) );
    (*this_).view_edit = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( view_edit ) );

    (*this_).tool_sect = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( tool_sect ) );

    (*this_).edit_copy = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( edit_copy ) );
    (*this_).edit_cut = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( edit_cut ) );
    (*this_).edit_delete = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( edit_delete ) );
    (*this_).edit_paste = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( edit_paste ) );
    (*this_).edit_instantiate = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( edit_instantiate ) );
    (*this_).edit_highlight = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( edit_highlight ) );
    (*this_).edit_redo = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( edit_redo ) );
    (*this_).edit_undo = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( edit_undo ) );
    (*this_).edit_reset = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( edit_reset ) );

    (*this_).message_error = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( message_error ) );
    (*this_).message_info = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( message_info ) );
    (*this_).message_warn = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( message_warn ) );
    (*this_).message_user_doc = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( message_user_doc ) );

    (*this_).navigate_breadcrumb_folder = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( navigate_breadcrumb_folder ) );
    (*this_).navigate_closed_folder = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( navigate_closed_folder ) );
    (*this_).navigate_create_child = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( navigate_create_child ) );
    (*this_).navigate_create_child_0 = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( navigate_create_child_0 ) );
    (*this_).navigate_create_sibling = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( navigate_create_sibling ) );
    (*this_).navigate_create_sibling_0 = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( navigate_create_sibling_0 ) );
    (*this_).navigate_open_folder = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( navigate_open_folder ) );

    (*this_).sketch_background = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( sketch_background ) );
    (*this_).sketch_create = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( sketch_create ) );
    (*this_).sketch_refine = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( sketch_refine ) );
    (*this_).sketch_relate = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( sketch_relate ) );
    (*this_).sketch_move_h = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( sketch_move_h ) );
    (*this_).sketch_move_v = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( sketch_move_v ) );
    (*this_).sketch_move_2d = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( sketch_move_2d ) );
    (*this_).sketch_page_down_bold = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( sketch_page_down_bold ) );
    (*this_).sketch_page_down_gray = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( sketch_page_down_gray ) );
    (*this_).sketch_page_up_bold = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( sketch_page_up_bold ) );
    (*this_).sketch_page_up_gray = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( sketch_page_up_gray ) );

    (*this_).type_undef = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_undef ) );

    (*this_).type_diag_activity = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_activity ) );
    (*this_).type_diag_block = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_block ) );
    (*this_).type_diag_box = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_box ) );
    (*this_).type_diag_class = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_class ) );
    (*this_).type_diag_communication = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_communication ) );
    (*this_).type_diag_component = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_component ) );
    (*this_).type_diag_composite = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_composite ) );
    (*this_).type_diag_deployment = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_deployment ) );
    (*this_).type_diag_internal = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_internal ) );
    (*this_).type_diag_list = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_list ) );
    (*this_).type_diag_overview = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_overview ) );
    (*this_).type_diag_package = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_package ) );
    (*this_).type_diag_parametric = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_parametric ) );
    (*this_).type_diag_profile = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_profile ) );
    (*this_).type_diag_requirement = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_requirement ) );
    (*this_).type_diag_sequence = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_sequence ) );
    (*this_).type_diag_state = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_state ) );
    (*this_).type_diag_timing = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_timing ) );
    (*this_).type_diag_usecase = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_diag_usecase ) );

    (*this_).type_clas_accept = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_accept ) );
    (*this_).type_clas_activity = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_activity ) );
    (*this_).type_clas_actor = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_actor ) );
    (*this_).type_clas_artifact = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_artifact ) );
    (*this_).type_clas_block = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_block ) );
    (*this_).type_clas_class = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_class ) );
    (*this_).type_clas_comment = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_comment ) );
    (*this_).type_clas_component = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_component ) );
    (*this_).type_clas_constraint = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_constraint ) );
    (*this_).type_clas_decision = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_decision ) );
    (*this_).type_clas_deephistory = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_deephistory ) );
    (*this_).type_clas_final = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_final ) );
    (*this_).type_clas_fork = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_fork ) );
    (*this_).type_clas_history = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_history ) );
    (*this_).type_clas_image = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_image ) );
    (*this_).type_clas_initial = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_initial ) );
    (*this_).type_clas_interaction_use = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_interaction_use ) );
    (*this_).type_clas_interface = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_interface ) );
    (*this_).type_clas_join = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_join ) );
    (*this_).type_clas_node = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_node ) );
    (*this_).type_clas_object = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_object ) );
    (*this_).type_clas_package = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_package ) );
    (*this_).type_clas_part = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_part ) );
    (*this_).type_clas_region = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_region ) );
    (*this_).type_clas_requirement = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_requirement ) );
    (*this_).type_clas_send = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_send ) );
    (*this_).type_clas_state = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_state ) );
    (*this_).type_clas_stereotype = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_stereotype ) );
    (*this_).type_clas_subsystem = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_subsystem ) );
    (*this_).type_clas_time = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_time ) );
    (*this_).type_clas_usecase = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_clas_usecase ) );

    (*this_).type_feat_entry = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_feat_entry ) );
    (*this_).type_feat_exit = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_feat_exit ) );
    (*this_).type_feat_life = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_feat_life ) );
    (*this_).type_feat_operation = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_feat_operation ) );
    (*this_).type_feat_port = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_feat_port ) );
    (*this_).type_feat_property = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_feat_property ) );
    (*this_).type_feat_provide = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_feat_provide ) );
    (*this_).type_feat_require = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_feat_require ) );
    (*this_).type_feat_rx = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_feat_rx ) );
    (*this_).type_feat_tag = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_feat_tag ) );
    (*this_).type_feat_tx = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_feat_tx ) );

    (*this_).type_rel_aggregate = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_aggregate ) );
    (*this_).type_rel_associate = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_associate ) );
    (*this_).type_rel_async = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_async ) );
    (*this_).type_rel_communication_path = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_communication_path ) );
    (*this_).type_rel_compose = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_compose ) );
    (*this_).type_rel_contain = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_contain ) );
    (*this_).type_rel_control_flow = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_control_flow ) );
    (*this_).type_rel_depend = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_depend ) );
    (*this_).type_rel_deploy = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_deploy ) );
    (*this_).type_rel_extend = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_extend ) );
    (*this_).type_rel_generalize = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_generalize ) );
    (*this_).type_rel_include = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_include ) );
    (*this_).type_rel_manifest = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_manifest ) );
    (*this_).type_rel_object_flow = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_object_flow ) );
    (*this_).type_rel_realize = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_realize ) );
    (*this_).type_rel_refine = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_refine ) );
    (*this_).type_rel_return = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_return ) );
    (*this_).type_rel_sync = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_sync ) );
    (*this_).type_rel_trace = gui_resources_new_texture_from_pixbuf_data( GIMP_PIXBUF_DATA( type_rel_trace ) );

    U8_TRACE_END();
}

/*!
 *  \brief destroys the gui_resources_t
 */
void gui_resources_destroy ( gui_resources_t *this_ )
{
    U8_TRACE_BEGIN();

    g_object_unref ((*this_).crystal_facet_uml);

    g_object_unref ((*this_).edit_attributes_sect);

    g_object_unref ((*this_).file_new);
    g_object_unref ((*this_).file_open);
    g_object_unref ((*this_).file_save);
    g_object_unref ((*this_).file_saved);
    g_object_unref ((*this_).file_unsaved);
    g_object_unref ((*this_).file_export);

    g_object_unref ((*this_).view_new_window);
    g_object_unref ((*this_).view_search);
    g_object_unref ((*this_).view_search_now);
    g_object_unref ((*this_).view_navigate);
    g_object_unref ((*this_).view_create);
    g_object_unref ((*this_).view_edit);

    g_object_unref ((*this_).tool_sect);

    g_object_unref ((*this_).edit_copy);
    g_object_unref ((*this_).edit_cut);
    g_object_unref ((*this_).edit_delete);
    g_object_unref ((*this_).edit_paste);
    g_object_unref ((*this_).edit_instantiate);
    g_object_unref ((*this_).edit_highlight);
    g_object_unref ((*this_).edit_redo);
    g_object_unref ((*this_).edit_undo);
    g_object_unref ((*this_).edit_reset);

    g_object_unref ((*this_).message_error);
    g_object_unref ((*this_).message_info);
    g_object_unref ((*this_).message_warn);

    g_object_unref ((*this_).message_user_doc);

    g_object_unref ((*this_).navigate_breadcrumb_folder);
    g_object_unref ((*this_).navigate_closed_folder);
    g_object_unref ((*this_).navigate_create_child);
    g_object_unref ((*this_).navigate_create_child_0);
    g_object_unref ((*this_).navigate_create_sibling);
    g_object_unref ((*this_).navigate_create_sibling_0);
    g_object_unref ((*this_).navigate_open_folder);

    g_object_unref ((*this_).sketch_background);  /* Type: GdkPixbuf */
    g_object_unref ((*this_).sketch_create);
    g_object_unref ((*this_).sketch_refine);
    g_object_unref ((*this_).sketch_relate);
    g_object_unref ((*this_).sketch_move_h);
    g_object_unref ((*this_).sketch_move_v);
    g_object_unref ((*this_).sketch_move_2d);
    g_object_unref ((*this_).sketch_page_down_bold);
    g_object_unref ((*this_).sketch_page_down_gray);
    g_object_unref ((*this_).sketch_page_up_bold);
    g_object_unref ((*this_).sketch_page_up_gray);

    g_object_unref ((*this_).type_undef);

    g_object_unref ((*this_).type_diag_activity);
    g_object_unref ((*this_).type_diag_block);
    g_object_unref ((*this_).type_diag_box);
    g_object_unref ((*this_).type_diag_class);
    g_object_unref ((*this_).type_diag_communication);
    g_object_unref ((*this_).type_diag_component);
    g_object_unref ((*this_).type_diag_composite);
    g_object_unref ((*this_).type_diag_deployment);
    g_object_unref ((*this_).type_diag_internal);
    g_object_unref ((*this_).type_diag_list);
    g_object_unref ((*this_).type_diag_overview);
    g_object_unref ((*this_).type_diag_package);
    g_object_unref ((*this_).type_diag_parametric);
    g_object_unref ((*this_).type_diag_profile);
    g_object_unref ((*this_).type_diag_requirement);
    g_object_unref ((*this_).type_diag_sequence);
    g_object_unref ((*this_).type_diag_state);
    g_object_unref ((*this_).type_diag_timing);
    g_object_unref ((*this_).type_diag_usecase);

    g_object_unref ((*this_).type_clas_accept);
    g_object_unref ((*this_).type_clas_activity);
    g_object_unref ((*this_).type_clas_actor);
    g_object_unref ((*this_).type_clas_artifact);
    g_object_unref ((*this_).type_clas_block);
    g_object_unref ((*this_).type_clas_class);
    g_object_unref ((*this_).type_clas_comment);
    g_object_unref ((*this_).type_clas_component);
    g_object_unref ((*this_).type_clas_constraint);
    g_object_unref ((*this_).type_clas_decision);
    g_object_unref ((*this_).type_clas_deephistory);
    g_object_unref ((*this_).type_clas_final);
    g_object_unref ((*this_).type_clas_fork);
    g_object_unref ((*this_).type_clas_history);
    g_object_unref ((*this_).type_clas_image);
    g_object_unref ((*this_).type_clas_initial);
    g_object_unref ((*this_).type_clas_interaction_use);
    g_object_unref ((*this_).type_clas_interface);
    g_object_unref ((*this_).type_clas_join);
    g_object_unref ((*this_).type_clas_node);
    g_object_unref ((*this_).type_clas_object);
    g_object_unref ((*this_).type_clas_package);
    g_object_unref ((*this_).type_clas_part);
    g_object_unref ((*this_).type_clas_region);
    g_object_unref ((*this_).type_clas_requirement);
    g_object_unref ((*this_).type_clas_send);
    g_object_unref ((*this_).type_clas_state);
    g_object_unref ((*this_).type_clas_stereotype);
    g_object_unref ((*this_).type_clas_subsystem);
    g_object_unref ((*this_).type_clas_time);
    g_object_unref ((*this_).type_clas_usecase);

    g_object_unref ((*this_).type_feat_entry);
    g_object_unref ((*this_).type_feat_exit);
    g_object_unref ((*this_).type_feat_life);
    g_object_unref ((*this_).type_feat_operation);
    g_object_unref ((*this_).type_feat_port);
    g_object_unref ((*this_).type_feat_property);
    g_object_unref ((*this_).type_feat_provide);
    g_object_unref ((*this_).type_feat_require);
    g_object_unref ((*this_).type_feat_rx);
    g_object_unref ((*this_).type_feat_tag);
    g_object_unref ((*this_).type_feat_tx);

    g_object_unref ((*this_).type_rel_aggregate);
    g_object_unref ((*this_).type_rel_associate);
    g_object_unref ((*this_).type_rel_async);
    g_object_unref ((*this_).type_rel_compose);
    g_object_unref ((*this_).type_rel_contain);
    g_object_unref ((*this_).type_rel_control_flow);
    g_object_unref ((*this_).type_rel_depend);
    g_object_unref ((*this_).type_rel_deploy);
    g_object_unref ((*this_).type_rel_extend);
    g_object_unref ((*this_).type_rel_generalize);
    g_object_unref ((*this_).type_rel_include);
    g_object_unref ((*this_).type_rel_manifest);
    g_object_unref ((*this_).type_rel_object_flow);
    g_object_unref ((*this_).type_rel_realize);
    g_object_unref ((*this_).type_rel_refine);
    g_object_unref ((*this_).type_rel_return);
    g_object_unref ((*this_).type_rel_sync);
    g_object_unref ((*this_).type_rel_trace);

    U8_TRACE_END();
}


/*
Copyright 2016-2025 Andreas Warnke

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
