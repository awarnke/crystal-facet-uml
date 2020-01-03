/* File: gui_resources.h; Copyright and License: see below */

#ifndef GUI_RESOURCES_H
#define GUI_RESOURCES_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides bitmap resources to the gui module
 */

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

/*!
 *  \brief attributes of the gui_resources_t
 */
struct gui_resources_struct {
    GdkPixbuf *crystal_facet_uml;
    GdkPixbuf *edit_attributes_sect;
    GdkPixbuf *edit_commit;
    GdkPixbuf *edit_copy;
    GdkPixbuf *edit_cut;
    GdkPixbuf *edit_delete;
    GdkPixbuf *edit_paste;
    GdkPixbuf *edit_redo;
    GdkPixbuf *edit_undo;
    GdkPixbuf *edit_instantiate;
    GdkPixbuf *edit_highlight;
    GdkPixbuf *edit_reset;
    GdkPixbuf *file_export;
    GdkPixbuf *file_new_window;
    GdkPixbuf *file_use_db;
    GdkPixbuf *message_error;
    GdkPixbuf *message_info;
    GdkPixbuf *message_warn;
    GdkPixbuf *message_user_doc;
    GdkPixbuf *tool_search;
    GdkPixbuf *tool_navigate;
    GdkPixbuf *tool_create;
    GdkPixbuf *tool_edit;
    GdkPixbuf *navigate_breadcrumb_folder;
    GdkPixbuf *navigate_closed_folder;
    GdkPixbuf *navigate_create_child;
    GdkPixbuf *navigate_create_sibling;
    GdkPixbuf *navigate_open_folder;
    GdkPixbuf *background_column;

    const GdkPixbuf *type_undef;
    const GdkPixbuf *type_diag_activity;
    const GdkPixbuf *type_diag_block;
    const GdkPixbuf *type_diag_box;
    const GdkPixbuf *type_diag_deployment;
    const GdkPixbuf *type_diag_class;
    const GdkPixbuf *type_diag_communication;
    const GdkPixbuf *type_diag_component;
    const GdkPixbuf *type_diag_composite;
    const GdkPixbuf *type_diag_internal;
    const GdkPixbuf *type_diag_list;
    const GdkPixbuf *type_diag_overview;
    const GdkPixbuf *type_diag_package;
    const GdkPixbuf *type_diag_parametric;
    const GdkPixbuf *type_diag_profile;
    const GdkPixbuf *type_diag_requirement;
    const GdkPixbuf *type_diag_sequence;
    const GdkPixbuf *type_diag_state;
    const GdkPixbuf *type_diag_timing;
    const GdkPixbuf *type_diag_usecase;

    const GdkPixbuf *type_clas_package;

    const GdkPixbuf *type_feat_life;
    const GdkPixbuf *type_feat_operation;
    const GdkPixbuf *type_feat_port;
    const GdkPixbuf *type_feat_property;
    const GdkPixbuf *type_feat_provide;
    const GdkPixbuf *type_feat_require;

    const GdkPixbuf *type_rel_depend;
};

typedef struct gui_resources_struct gui_resources_t;

/*!
 *  \brief initializes the gui_resources_t
 *
 *  \param this_ pointer to own object attributes
 */
void gui_resources_init ( gui_resources_t *this_ );

/*!
 *  \brief destroys the gui_resources_t
 *
 *  \param this_ pointer to own object attributes
 */
void gui_resources_destroy ( gui_resources_t *this_ );

/*!
 *  \brief gets the crystal_facet_uml
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_crystal_facet_uml ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_attributes_sect
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_attributes_sect ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_commit
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_commit ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_copy
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_copy ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_cut
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_cut ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_delete
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_delete ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_instantiate
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_instantiate ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_highlight
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_highlight ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_paste
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_paste ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_redo
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_redo ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_undo
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_undo ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_reset
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_reset ( gui_resources_t *this_ );

/*!
 *  \brief gets the file_export
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_file_export ( gui_resources_t *this_ );

/*!
 *  \brief gets the file_new_window
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_file_new_window ( gui_resources_t *this_ );

/*!
 *  \brief gets the file_use_db
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_file_use_db ( gui_resources_t *this_ );

/*!
 *  \brief gets the message_error
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_message_error ( gui_resources_t *this_ );

/*!
 *  \brief gets the message_info
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_message_info ( gui_resources_t *this_ );

/*!
 *  \brief gets the message_warn
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_message_warn ( gui_resources_t *this_ );

/*!
 *  \brief gets the message_user_doc
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_message_user_doc ( gui_resources_t *this_ );

/*!
 *  \brief gets the tool_search
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_tool_search ( gui_resources_t *this_ );

/*!
 *  \brief gets the tool_navigate
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_tool_navigate ( gui_resources_t *this_ );

/*!
 *  \brief gets the tool_create
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_tool_create ( gui_resources_t *this_ );

/*!
 *  \brief gets the tool_edit
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_tool_edit ( gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_breadcrumb_folder
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_navigate_breadcrumb_folder ( gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_closed_folder
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_navigate_closed_folder ( gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_open_folder
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_navigate_open_folder ( gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_create_child
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_navigate_create_child ( gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_create_sibling
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_navigate_create_sibling ( gui_resources_t *this_ );

/*!
 *  \brief gets the background_column
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_background_column ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_undef
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_undef ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_activity
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_activity ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_block
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_block ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_box
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_box ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_class
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_class ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_communication
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_communication ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_component
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_component ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_composite
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_composite ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_deployment
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_deployment ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_internal
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_internal ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_list
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_list ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_overview
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_overview ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_package
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_package ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_parametric
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_parametric ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_profile
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_profile ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_requirement
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_requirement ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_sequence
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_sequence ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_state
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_state ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_timing
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_timing ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_usecase
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_diag_usecase ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_package
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_clas_package ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_life
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_feat_life ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_operation
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_feat_operation ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_port
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_feat_port ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_property
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_feat_property ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_provide
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_feat_provide ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_require
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_feat_require ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_depend
 *
 *  \param this_ pointer to own object attributes
 */
static inline const GdkPixbuf *gui_resources_get_type_rel_depend ( gui_resources_t *this_ );

#include "gui_resources.inl"

#endif  /* GUI_RESOURCES_H */


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
