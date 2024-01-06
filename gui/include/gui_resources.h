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
    GdkPixbuf *file_new_db;
    GdkPixbuf *file_save;
    GdkPixbuf *file_use_db;
    GdkPixbuf *message_error;
    GdkPixbuf *message_info;
    GdkPixbuf *message_warn;
    GdkPixbuf *message_user_doc;
    GdkPixbuf *view_new_window;
    GdkPixbuf *view_search;
    GdkPixbuf *view_navigate;
    GdkPixbuf *view_create;
    GdkPixbuf *view_edit;
    GdkPixbuf *tool_sect;
    GdkPixbuf *navigate_breadcrumb_folder;
    GdkPixbuf *navigate_closed_folder;
    GdkPixbuf *navigate_create_child;
    GdkPixbuf *navigate_create_child_0;
    GdkPixbuf *navigate_create_sibling;
    GdkPixbuf *navigate_create_sibling_0;
    GdkPixbuf *navigate_open_folder;
    GdkPixbuf *search_search;
    GdkPixbuf *background_column;

    GdkPixbuf *type_undef;

    GdkPixbuf *type_diag_activity;
    GdkPixbuf *type_diag_block;
    GdkPixbuf *type_diag_box;
    GdkPixbuf *type_diag_deployment;
    GdkPixbuf *type_diag_class;
    GdkPixbuf *type_diag_communication;
    GdkPixbuf *type_diag_component;
    GdkPixbuf *type_diag_composite;
    GdkPixbuf *type_diag_internal;
    GdkPixbuf *type_diag_list;
    GdkPixbuf *type_diag_overview;
    GdkPixbuf *type_diag_package;
    GdkPixbuf *type_diag_parametric;
    GdkPixbuf *type_diag_profile;
    GdkPixbuf *type_diag_requirement;
    GdkPixbuf *type_diag_sequence;
    GdkPixbuf *type_diag_state;
    GdkPixbuf *type_diag_timing;
    GdkPixbuf *type_diag_usecase;

    GdkPixbuf *type_clas_accept;
    GdkPixbuf *type_clas_activity;
    GdkPixbuf *type_clas_actor;
    GdkPixbuf *type_clas_artifact;
    GdkPixbuf *type_clas_block;
    GdkPixbuf *type_clas_class;
    GdkPixbuf *type_clas_comment;
    GdkPixbuf *type_clas_component;
    GdkPixbuf *type_clas_constraint;
    GdkPixbuf *type_clas_decision;
    GdkPixbuf *type_clas_deephistory;
    GdkPixbuf *type_clas_diagram;
    GdkPixbuf *type_clas_final;
    GdkPixbuf *type_clas_fork;
    GdkPixbuf *type_clas_history;
    GdkPixbuf *type_clas_image;
    GdkPixbuf *type_clas_initial;
    GdkPixbuf *type_clas_interface;
    GdkPixbuf *type_clas_join;
    GdkPixbuf *type_clas_node;
    GdkPixbuf *type_clas_object;
    GdkPixbuf *type_clas_package;
    GdkPixbuf *type_clas_part;
    GdkPixbuf *type_clas_region;
    GdkPixbuf *type_clas_requirement;
    GdkPixbuf *type_clas_send;
    GdkPixbuf *type_clas_state;
    GdkPixbuf *type_clas_stereotype;
    GdkPixbuf *type_clas_subsystem;
    GdkPixbuf *type_clas_time;
    GdkPixbuf *type_clas_usecase;

    GdkPixbuf *type_feat_entry;
    GdkPixbuf *type_feat_exit;
    GdkPixbuf *type_feat_life;
    GdkPixbuf *type_feat_operation;
    GdkPixbuf *type_feat_port;
    GdkPixbuf *type_feat_property;
    GdkPixbuf *type_feat_provide;
    GdkPixbuf *type_feat_require;
    GdkPixbuf *type_feat_rx;
    GdkPixbuf *type_feat_tag;
    GdkPixbuf *type_feat_tx;

    GdkPixbuf *type_rel_aggregate;
    GdkPixbuf *type_rel_associate;
    GdkPixbuf *type_rel_async;
    GdkPixbuf *type_rel_communication_path;
    GdkPixbuf *type_rel_compose;
    GdkPixbuf *type_rel_contain;
    GdkPixbuf *type_rel_control_flow;
    GdkPixbuf *type_rel_depend;
    GdkPixbuf *type_rel_deploy;
    GdkPixbuf *type_rel_extend;
    GdkPixbuf *type_rel_generalize;
    GdkPixbuf *type_rel_include;
    GdkPixbuf *type_rel_manifest;
    GdkPixbuf *type_rel_object_flow;
    GdkPixbuf *type_rel_realize;
    GdkPixbuf *type_rel_refine;
    GdkPixbuf *type_rel_return;
    GdkPixbuf *type_rel_sync;
    GdkPixbuf *type_rel_trace;
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
 *  \brief gets the file_new_db
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_file_new_db ( gui_resources_t *this_ );

/*!
 *  \brief gets the file_save
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_file_save ( gui_resources_t *this_ );

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
 *  \brief gets the view_new_window
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_view_new_window ( gui_resources_t *this_ );

/*!
 *  \brief gets the view_search
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_view_search ( gui_resources_t *this_ );

/*!
 *  \brief gets the view_navigate
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_view_navigate ( gui_resources_t *this_ );

/*!
 *  \brief gets the view_create
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_view_create ( gui_resources_t *this_ );

/*!
 *  \brief gets the view_edit
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_view_edit ( gui_resources_t *this_ );

/*!
 *  \brief gets the tool_sect
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_tool_sect ( gui_resources_t *this_ );

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
 *  \brief gets the navigate_create_child_0
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_navigate_create_child_0 ( gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_create_sibling
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_navigate_create_sibling ( gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_create_sibling_0
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_navigate_create_sibling_0 ( gui_resources_t *this_ );

/*!
 *  \brief gets the search_search
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_search_search ( gui_resources_t *this_ );

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
static inline GdkPixbuf *gui_resources_get_type_undef ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_activity
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_activity ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_block
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_block ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_box
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_box ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_class
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_class ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_communication
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_communication ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_component
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_component ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_composite
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_composite ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_deployment
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_deployment ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_internal
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_internal ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_list
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_list ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_overview
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_overview ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_package
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_package ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_parametric
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_parametric ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_profile
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_profile ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_requirement
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_requirement ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_sequence
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_sequence ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_state
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_state ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_timing
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_timing ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_usecase
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_diag_usecase ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_accept
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_accept ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_activity
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_activity ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_actor
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_actor ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_artifact
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_artifact ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_block
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_block ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_class
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_class ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_comment
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_comment ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_component
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_component ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_constraint
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_constraint ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_decision
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_decision ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_deephistory
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_deephistory ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_diagram
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_diagram ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_final
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_final ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_fork
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_fork ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_history
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_history ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_image
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_image ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_initial
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_initial ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_interface
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_interface ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_join
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_join ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_node
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_node ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_object
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_object ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_package
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_package ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_part
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_part ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_region
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_region ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_requirement
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_requirement ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_send
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_send ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_state
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_state ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_stereotype
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_stereotype ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_subsystem
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_subsystem ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_time
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_time ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_usecase
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_clas_usecase ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_entry
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_feat_entry ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_exit
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_feat_exit ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_life
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_feat_life ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_operation
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_feat_operation ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_port
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_feat_port ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_property
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_feat_property ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_provide
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_feat_provide ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_require
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_feat_require ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_rx
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_feat_rx ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_tag
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_feat_tag ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_tx
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_feat_tx ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_aggregate
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_aggregate ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_associate
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_associate ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_async
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_async ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_compose
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_compose ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_communication_path
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_communication_path ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_contain
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_contain ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_control_flow
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_control_flow ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_depend
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_depend ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_deploy
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_deploy ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_extend
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_extend ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_generalize
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_generalize ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_include
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_include ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_manifest
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_manifest ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_object_flow
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_object_flow ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_realize
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_realize ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_refine
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_refine ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_return
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_return ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_sync
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_sync ( gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_trace
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_type_rel_trace ( gui_resources_t *this_ );

#include "gui_resources.inl"

#endif  /* GUI_RESOURCES_H */


/*
Copyright 2016-2024 Andreas Warnke

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
