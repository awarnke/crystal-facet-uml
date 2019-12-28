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
#include "resources/type_diag_box.c"
#include "resources/type_diag_list.c"
#include "resources/type_diag_state.c"
#include "resources/type_diag_usecase.c"

#if __GNUC__ >= 8
#pragma GCC diagnostic pop
#endif

void gui_resources_init ( gui_resources_t *this_ )
{
    TRACE_BEGIN();

    (*this_).crystal_facet_uml = gdk_pixbuf_new_from_data(
        &(crystal_facet_uml.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        crystal_facet_uml.width,
        crystal_facet_uml.height,
        crystal_facet_uml.width * crystal_facet_uml.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).edit_attributes_sect = gdk_pixbuf_new_from_data(
        &(edit_attributes_sect.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        edit_attributes_sect.width,
        edit_attributes_sect.height,
        edit_attributes_sect.width * edit_attributes_sect.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).edit_commit = gdk_pixbuf_new_from_data(
        &(edit_commit.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        edit_commit.width,
        edit_commit.height,
        edit_commit.width * edit_commit.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).edit_copy = gdk_pixbuf_new_from_data(
        &(edit_copy.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        edit_copy.width,
        edit_copy.height,
        edit_copy.width * edit_copy.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).edit_cut = gdk_pixbuf_new_from_data(
        &(edit_cut.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        edit_cut.width,
        edit_cut.height,
        edit_cut.width * edit_cut.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).edit_delete = gdk_pixbuf_new_from_data(
        &(edit_delete.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        edit_delete.width,
        edit_delete.height,
        edit_delete.width * edit_delete.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).edit_paste = gdk_pixbuf_new_from_data(
        &(edit_paste.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        edit_paste.width,
        edit_paste.height,
        edit_paste.width * edit_paste.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).edit_instantiate = gdk_pixbuf_new_from_data(
        &(edit_instantiate.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        edit_instantiate.width,
        edit_instantiate.height,
        edit_instantiate.width * edit_instantiate.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).edit_highlight = gdk_pixbuf_new_from_data(
        &(edit_highlight.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        edit_highlight.width,
        edit_highlight.height,
        edit_highlight.width * edit_highlight.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).edit_redo = gdk_pixbuf_new_from_data(
        &(edit_redo.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        edit_redo.width,
        edit_redo.height,
        edit_redo.width * edit_redo.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).edit_undo = gdk_pixbuf_new_from_data(
        &(edit_undo.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        edit_undo.width,
        edit_undo.height,
        edit_undo.width * edit_undo.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).edit_reset = gdk_pixbuf_new_from_data(
        &(edit_reset.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        edit_reset.width,
        edit_reset.height,
        edit_reset.width * edit_reset.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).file_export = gdk_pixbuf_new_from_data(
        &(file_export.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        file_export.width,
        file_export.height,
        file_export.width * file_export.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).file_new_window = gdk_pixbuf_new_from_data(
        &(file_new_window.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        file_new_window.width,
        file_new_window.height,
        file_new_window.width * file_new_window.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).file_use_db = gdk_pixbuf_new_from_data(
        &(file_use_db.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        file_use_db.width,
        file_use_db.height,
        file_use_db.width * file_use_db.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).message_error = gdk_pixbuf_new_from_data(
        &(message_error.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        message_error.width,
        message_error.height,
        message_error.width * message_error.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).message_info = gdk_pixbuf_new_from_data(
        &(message_info.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        message_info.width,
        message_info.height,
        message_info.width * message_info.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).message_warn = gdk_pixbuf_new_from_data(
        &(message_warn.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        message_warn.width,
        message_warn.height,
        message_warn.width * message_warn.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).message_user_doc = gdk_pixbuf_new_from_data(
        &(message_user_doc.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        message_user_doc.width,
        message_user_doc.height,
        message_user_doc.width * message_user_doc.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).tool_search = gdk_pixbuf_new_from_data(
        &(tool_search.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        tool_search.width,
        tool_search.height,
        tool_search.width * tool_search.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).tool_navigate = gdk_pixbuf_new_from_data(
        &(tool_navigate.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        tool_navigate.width,
        tool_navigate.height,
        tool_navigate.width * tool_navigate.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).tool_create = gdk_pixbuf_new_from_data(
        &(tool_create.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        tool_create.width,
        tool_create.height,
        tool_create.width * tool_create.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).tool_edit = gdk_pixbuf_new_from_data(
        &(tool_edit.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        tool_edit.width,
        tool_edit.height,
        tool_edit.width * tool_edit.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).navigate_breadcrumb_folder = gdk_pixbuf_new_from_data(
        &(navigate_breadcrumb_folder.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        navigate_breadcrumb_folder.width,
        navigate_breadcrumb_folder.height,
        navigate_breadcrumb_folder.width * navigate_breadcrumb_folder.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).navigate_closed_folder = gdk_pixbuf_new_from_data(
        &(navigate_closed_folder.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        navigate_closed_folder.width,
        navigate_closed_folder.height,
        navigate_closed_folder.width * navigate_closed_folder.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).navigate_create_child = gdk_pixbuf_new_from_data(
        &(navigate_create_child.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        navigate_create_child.width,
        navigate_create_child.height,
        navigate_create_child.width * navigate_create_child.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).navigate_create_sibling = gdk_pixbuf_new_from_data(
        &(navigate_create_sibling.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        navigate_create_sibling.width,
        navigate_create_sibling.height,
        navigate_create_sibling.width * navigate_create_sibling.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).navigate_open_folder = gdk_pixbuf_new_from_data(
        &(navigate_open_folder.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        navigate_open_folder.width,
        navigate_open_folder.height,
        navigate_open_folder.width * navigate_open_folder.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).background_column = gdk_pixbuf_new_from_data(
        &(background_column.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        background_column.width,
        background_column.height,
        background_column.width * background_column.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).type_undef = gdk_pixbuf_new_from_data(
        &(type_undef.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        type_undef.width,
        type_undef.height,
        type_undef.width * type_undef.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).type_diag_box = gdk_pixbuf_new_from_data(
        &(type_diag_box.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        type_diag_box.width,
        type_diag_box.height,
        type_diag_box.width * type_diag_box.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).type_diag_list = gdk_pixbuf_new_from_data(
        &(type_diag_list.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        type_diag_list.width,
        type_diag_list.height,
        type_diag_list.width * type_diag_list.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).type_diag_state = gdk_pixbuf_new_from_data(
        &(type_diag_state.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        type_diag_state.width,
        type_diag_state.height,
        type_diag_state.width * type_diag_state.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).type_diag_usecase = gdk_pixbuf_new_from_data(
        &(type_diag_usecase.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        type_diag_usecase.width,
        type_diag_usecase.height,
        type_diag_usecase.width * type_diag_usecase.bytes_per_pixel,
        NULL,
        NULL
    );

    TRACE_END();
}

/*!
 *  \brief destroys the gui_resources_t
 */
void gui_resources_destroy ( gui_resources_t *this_ )
{
    TRACE_BEGIN();
    TRACE_END();
}


/*
Copyright 2016-2019 Andreas Warnke

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
