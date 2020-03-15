/* File: gui_resources.c; Copyright and License: see below */

#include "gui_resources.h"
#include "trace.h"
#include <gtk/gtk.h>
#include <stdbool.h>

/* including resource files */

#if __GNUC__ >= 8
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverlength-strings"
#endif

#include "resources/crystal_facet_uml.c"
#include "resources/edit_attributes_sect.c"
#include "resources/edit_commit.c"
#include "resources/edit_copy.c"
#include "resources/edit_cut.c"
#include "resources/edit_delete.c"
#include "resources/edit_paste.c"
#include "resources/edit_instantiate.c"
#include "resources/edit_highlight.c"
#include "resources/edit_redo.c"
#include "resources/edit_undo.c"
#include "resources/edit_reset.c"
#include "resources/file_export.c"
#include "resources/file_new_window.c"
#include "resources/file_use_db.c"
#include "resources/message_error.c"
#include "resources/message_info.c"
#include "resources/message_warn.c"
#include "resources/message_user_doc.c"
#include "resources/tool_search.c"
#include "resources/tool_navigate.c"
#include "resources/tool_create.c"
#include "resources/tool_edit.c"
#include "resources/navigate_breadcrumb_folder.c"
#include "resources/navigate_closed_folder.c"
#include "resources/navigate_create_child.c"
#include "resources/navigate_create_sibling.c"
#include "resources/navigate_open_folder.c"
#include "resources/background_column.c"

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
#include "resources/type_clas_actor.c"
#include "resources/type_clas_artifact.c"
#include "resources/type_clas_block.c"
#include "resources/type_clas_boundary.c"
#include "resources/type_clas_class.c"
#include "resources/type_clas_comment.c"
#include "resources/type_clas_component.c"
#include "resources/type_clas_constraint.c"
#include "resources/type_clas_decision.c"
#include "resources/type_clas_deephistory.c"
#include "resources/type_clas_diagram.c"
#include "resources/type_clas_final.c"
#include "resources/type_clas_fork.c"
#include "resources/type_clas_history.c"
#include "resources/type_clas_initial.c"
#include "resources/type_clas_join.c"
#include "resources/type_clas_node.c"
#include "resources/type_clas_package.c"
#include "resources/type_clas_part.c"
#include "resources/type_clas_region.c"
#include "resources/type_clas_requirement.c"
#include "resources/type_clas_send.c"
#include "resources/type_clas_state.c"
#include "resources/type_clas_time.c"
#include "resources/type_clas_usecase.c"

#include "resources/type_feat_life.c"
#include "resources/type_feat_operation.c"
#include "resources/type_feat_port.c"
#include "resources/type_feat_property.c"
#include "resources/type_feat_provide.c"
#include "resources/type_feat_require.c"

#include "resources/type_rel_aggregate.c"
#include "resources/type_rel_associate.c"
#include "resources/type_rel_compose.c"
#include "resources/type_rel_contain.c"
#include "resources/type_rel_depend.c"
#include "resources/type_rel_deploy.c"
#include "resources/type_rel_extend.c"
#include "resources/type_rel_generalize.c"
#include "resources/type_rel_include.c"
#include "resources/type_rel_manifest.c"
#include "resources/type_rel_realize.c"
#include "resources/type_rel_refine.c"
#include "resources/type_rel_return.c"
#include "resources/type_rel_sync.c"
#include "resources/type_rel_trace.c"

#if __GNUC__ >= 8
#pragma GCC diagnostic pop
#endif

#define GIMP_PIXBUF_DATA(STRUCTNAME) &(STRUCTNAME.pixel_data[0]), GDK_COLORSPACE_RGB, true /* alpha */, 8, \
                                     STRUCTNAME.width, STRUCTNAME.height, STRUCTNAME.width * STRUCTNAME.bytes_per_pixel, \
                                     NULL, NULL

void gui_resources_init ( gui_resources_t *this_ )
{
    TRACE_BEGIN();

    (*this_).crystal_facet_uml = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( crystal_facet_uml ) );

    (*this_).edit_attributes_sect = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( edit_attributes_sect ) );

    (*this_).edit_commit = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( edit_commit ) );
    (*this_).edit_copy = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( edit_copy ) );
    (*this_).edit_cut = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( edit_cut ) );
    (*this_).edit_delete = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( edit_delete ) );
    (*this_).edit_paste = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( edit_paste ) );
    (*this_).edit_instantiate = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( edit_instantiate ) );
    (*this_).edit_highlight = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( edit_highlight ) );
    (*this_).edit_redo = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( edit_redo ) );
    (*this_).edit_undo = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( edit_undo ) );
    (*this_).edit_reset = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( edit_reset ) );

    (*this_).file_export = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( file_export ) );
    (*this_).file_new_window = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( file_new_window ) );
    (*this_).file_use_db = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( file_use_db ) );

    (*this_).message_error = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( message_error ) );
    (*this_).message_info = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( message_info ) );
    (*this_).message_warn = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( message_warn ) );

    (*this_).message_user_doc = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( message_user_doc ) );

    (*this_).tool_search = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( tool_search ) );
    (*this_).tool_navigate = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( tool_navigate ) );
    (*this_).tool_create = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( tool_create ) );
    (*this_).tool_edit = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( tool_edit ) );

    (*this_).navigate_breadcrumb_folder = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( navigate_breadcrumb_folder ) );
    (*this_).navigate_closed_folder = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( navigate_closed_folder ) );
    (*this_).navigate_create_child = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( navigate_create_child ) );
    (*this_).navigate_create_sibling = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( navigate_create_sibling ) );
    (*this_).navigate_open_folder = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( navigate_open_folder ) );

    (*this_).background_column = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( background_column ) );

    (*this_).type_undef = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_undef ) );

    (*this_).type_diag_activity = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_activity ) );
    (*this_).type_diag_block = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_block ) );
    (*this_).type_diag_box = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_box ) );
    (*this_).type_diag_class = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_class ) );
    (*this_).type_diag_communication = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_communication ) );
    (*this_).type_diag_component = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_component ) );
    (*this_).type_diag_composite = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_composite ) );
    (*this_).type_diag_deployment = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_deployment ) );
    (*this_).type_diag_internal = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_internal ) );
    (*this_).type_diag_list = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_list ) );
    (*this_).type_diag_overview = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_overview ) );
    (*this_).type_diag_package = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_package ) );
    (*this_).type_diag_parametric = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_parametric ) );
    (*this_).type_diag_profile = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_profile ) );
    (*this_).type_diag_requirement = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_requirement ) );
    (*this_).type_diag_sequence = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_sequence ) );
    (*this_).type_diag_state = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_state ) );
    (*this_).type_diag_timing = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_timing ) );
    (*this_).type_diag_usecase = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_diag_usecase ) );

    (*this_).type_clas_accept = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_accept ) );
    (*this_).type_clas_actor = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_actor ) );
    (*this_).type_clas_artifact = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_artifact ) );
    (*this_).type_clas_block = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_block ) );
    (*this_).type_clas_boundary = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_boundary ) );
    (*this_).type_clas_class = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_class ) );
    (*this_).type_clas_comment = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_comment ) );
    (*this_).type_clas_component = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_component ) );
    (*this_).type_clas_constraint = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_constraint ) );
    (*this_).type_clas_decision = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_decision ) );
    (*this_).type_clas_deephistory = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_deephistory ) );
    (*this_).type_clas_diagram = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_diagram ) );
    (*this_).type_clas_final = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_final ) );
    (*this_).type_clas_fork = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_fork ) );
    (*this_).type_clas_history = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_history ) );
    (*this_).type_clas_initial = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_initial ) );
    (*this_).type_clas_join = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_join ) );
    (*this_).type_clas_node = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_node ) );
    (*this_).type_clas_package = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_package ) );
    (*this_).type_clas_part = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_part ) );
    (*this_).type_clas_region = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_region ) );
    (*this_).type_clas_requirement = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_requirement ) );
    (*this_).type_clas_send = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_send ) );
    (*this_).type_clas_state = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_state ) );
    (*this_).type_clas_time = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_time ) );
    (*this_).type_clas_usecase = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_clas_usecase ) );

    (*this_).type_feat_life = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_feat_life ) );
    (*this_).type_feat_operation = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_feat_operation ) );
    (*this_).type_feat_port = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_feat_port ) );
    (*this_).type_feat_property = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_feat_property ) );
    (*this_).type_feat_provide = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_feat_provide ) );
    (*this_).type_feat_require = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_feat_require ) );

    (*this_).type_rel_aggregate = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_aggregate ) );
    (*this_).type_rel_associate = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_associate ) );
    (*this_).type_rel_compose = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_compose ) );
    (*this_).type_rel_contain = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_contain ) );
    (*this_).type_rel_depend = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_depend ) );
    (*this_).type_rel_deploy = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_deploy ) );
    (*this_).type_rel_extend = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_extend ) );
    (*this_).type_rel_generalize = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_generalize ) );
    (*this_).type_rel_include = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_include ) );
    (*this_).type_rel_manifest = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_manifest ) );
    (*this_).type_rel_realize = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_realize ) );
    (*this_).type_rel_refine = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_refine ) );
    (*this_).type_rel_return = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_return ) );
    (*this_).type_rel_sync = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_sync ) );
    (*this_).type_rel_trace = gdk_pixbuf_new_from_data( GIMP_PIXBUF_DATA( type_rel_trace ) );

    TRACE_END();
}

/*!
 *  \brief destroys the gui_resources_t
 */
void gui_resources_destroy ( gui_resources_t *this_ )
{
    TRACE_BEGIN();

    g_object_unref ((*this_).crystal_facet_uml);

    g_object_unref ((*this_).edit_attributes_sect);

    g_object_unref ((*this_).edit_commit);
    g_object_unref ((*this_).edit_copy);
    g_object_unref ((*this_).edit_cut);
    g_object_unref ((*this_).edit_delete);
    g_object_unref ((*this_).edit_paste);
    g_object_unref ((*this_).edit_instantiate);
    g_object_unref ((*this_).edit_highlight);
    g_object_unref ((*this_).edit_redo);
    g_object_unref ((*this_).edit_undo);
    g_object_unref ((*this_).edit_reset);

    g_object_unref ((*this_).file_export);
    g_object_unref ((*this_).file_new_window);
    g_object_unref ((*this_).file_use_db);

    g_object_unref ((*this_).message_error);
    g_object_unref ((*this_).message_info);
    g_object_unref ((*this_).message_warn);

    g_object_unref ((*this_).message_user_doc);

    g_object_unref ((*this_).tool_search);
    g_object_unref ((*this_).tool_navigate);
    g_object_unref ((*this_).tool_create);
    g_object_unref ((*this_).tool_edit);

    g_object_unref ((*this_).navigate_breadcrumb_folder);
    g_object_unref ((*this_).navigate_closed_folder);
    g_object_unref ((*this_).navigate_create_child);
    g_object_unref ((*this_).navigate_create_sibling);
    g_object_unref ((*this_).navigate_open_folder);

    g_object_unref ((*this_).background_column);

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
    g_object_unref ((*this_).type_clas_actor);
    g_object_unref ((*this_).type_clas_artifact);
    g_object_unref ((*this_).type_clas_block);
    g_object_unref ((*this_).type_clas_boundary);
    g_object_unref ((*this_).type_clas_class);
    g_object_unref ((*this_).type_clas_comment);
    g_object_unref ((*this_).type_clas_component);
    g_object_unref ((*this_).type_clas_constraint);
    g_object_unref ((*this_).type_clas_decision);
    g_object_unref ((*this_).type_clas_deephistory);
    g_object_unref ((*this_).type_clas_diagram);
    g_object_unref ((*this_).type_clas_final);
    g_object_unref ((*this_).type_clas_fork);
    g_object_unref ((*this_).type_clas_history);
    g_object_unref ((*this_).type_clas_initial);
    g_object_unref ((*this_).type_clas_join);
    g_object_unref ((*this_).type_clas_node);
    g_object_unref ((*this_).type_clas_package);
    g_object_unref ((*this_).type_clas_part);
    g_object_unref ((*this_).type_clas_region);
    g_object_unref ((*this_).type_clas_requirement);
    g_object_unref ((*this_).type_clas_send);
    g_object_unref ((*this_).type_clas_state);
    g_object_unref ((*this_).type_clas_time);
    g_object_unref ((*this_).type_clas_usecase);

    g_object_unref ((*this_).type_feat_life);
    g_object_unref ((*this_).type_feat_operation);
    g_object_unref ((*this_).type_feat_port);
    g_object_unref ((*this_).type_feat_property);
    g_object_unref ((*this_).type_feat_provide);
    g_object_unref ((*this_).type_feat_require);

    g_object_unref ((*this_).type_rel_aggregate);
    g_object_unref ((*this_).type_rel_associate);
    g_object_unref ((*this_).type_rel_compose);
    g_object_unref ((*this_).type_rel_contain);
    g_object_unref ((*this_).type_rel_depend);
    g_object_unref ((*this_).type_rel_deploy);
    g_object_unref ((*this_).type_rel_extend);
    g_object_unref ((*this_).type_rel_generalize);
    g_object_unref ((*this_).type_rel_include);
    g_object_unref ((*this_).type_rel_manifest);
    g_object_unref ((*this_).type_rel_realize);
    g_object_unref ((*this_).type_rel_refine);
    g_object_unref ((*this_).type_rel_return);
    g_object_unref ((*this_).type_rel_sync);
    g_object_unref ((*this_).type_rel_trace);

    TRACE_END();
}


/*
Copyright 2016-2020 Andreas Warnke

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
