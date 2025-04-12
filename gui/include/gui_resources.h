/* File: gui_resources.h; Copyright and License: see below */

#ifndef GUI_RESOURCES_H
#define GUI_RESOURCES_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides bitmap resources to the gui module
 */

#include "gui_gtk.h"
#include <gdk-pixbuf/gdk-pixbuf.h>

/*!
 *  \brief attributes of the gui_resources_t
 */
struct gui_resources_struct {
    GdkTexture *crystal_facet_uml;
    GdkTexture *edit_attributes_sect;

    GdkTexture *file_new;
    GdkTexture *file_open;
    GdkTexture *file_save;
    GdkTexture *file_saved;
    GdkTexture *file_unsaved;
    GdkTexture *file_export;

    GdkTexture *view_new_window;
    GdkTexture *view_search;
    GdkTexture *view_navigate;
    GdkTexture *view_create;
    GdkTexture *view_edit;

    GdkTexture *tool_sect;

    GdkTexture *edit_copy;
    GdkTexture *edit_cut;
    GdkTexture *edit_delete;
    GdkTexture *edit_paste;
    GdkTexture *edit_redo;
    GdkTexture *edit_undo;
    GdkTexture *edit_instantiate;
    GdkTexture *edit_highlight;
    GdkTexture *edit_reset;

    GdkTexture *message_error;
    GdkTexture *message_info;
    GdkTexture *message_warn;
    GdkTexture *message_user_doc;

    GdkTexture *navigate_breadcrumb_folder;
    GdkTexture *navigate_closed_folder;
    GdkTexture *navigate_create_child;
    GdkTexture *navigate_create_child_0;
    GdkTexture *navigate_create_sibling;
    GdkTexture *navigate_create_sibling_0;
    GdkTexture *navigate_open_folder;
    GdkTexture *search_search;

    GdkPixbuf *background_column;

    GdkTexture *type_undef;

    GdkTexture *type_diag_activity;
    GdkTexture *type_diag_block;
    GdkTexture *type_diag_box;
    GdkTexture *type_diag_deployment;
    GdkTexture *type_diag_class;
    GdkTexture *type_diag_communication;
    GdkTexture *type_diag_component;
    GdkTexture *type_diag_composite;
    GdkTexture *type_diag_internal;
    GdkTexture *type_diag_list;
    GdkTexture *type_diag_overview;
    GdkTexture *type_diag_package;
    GdkTexture *type_diag_parametric;
    GdkTexture *type_diag_profile;
    GdkTexture *type_diag_requirement;
    GdkTexture *type_diag_sequence;
    GdkTexture *type_diag_state;
    GdkTexture *type_diag_timing;
    GdkTexture *type_diag_usecase;

    GdkTexture *type_clas_accept;
    GdkTexture *type_clas_activity;
    GdkTexture *type_clas_actor;
    GdkTexture *type_clas_artifact;
    GdkTexture *type_clas_block;
    GdkTexture *type_clas_class;
    GdkTexture *type_clas_comment;
    GdkTexture *type_clas_component;
    GdkTexture *type_clas_constraint;
    GdkTexture *type_clas_decision;
    GdkTexture *type_clas_deephistory;
    GdkTexture *type_clas_diagram;
    GdkTexture *type_clas_final;
    GdkTexture *type_clas_fork;
    GdkTexture *type_clas_history;
    GdkTexture *type_clas_image;
    GdkTexture *type_clas_initial;
    GdkTexture *type_clas_interface;
    GdkTexture *type_clas_join;
    GdkTexture *type_clas_node;
    GdkTexture *type_clas_object;
    GdkTexture *type_clas_package;
    GdkTexture *type_clas_part;
    GdkTexture *type_clas_region;
    GdkTexture *type_clas_requirement;
    GdkTexture *type_clas_send;
    GdkTexture *type_clas_state;
    GdkTexture *type_clas_stereotype;
    GdkTexture *type_clas_subsystem;
    GdkTexture *type_clas_time;
    GdkTexture *type_clas_usecase;

    GdkTexture *type_feat_entry;
    GdkTexture *type_feat_exit;
    GdkTexture *type_feat_life;
    GdkTexture *type_feat_operation;
    GdkTexture *type_feat_port;
    GdkTexture *type_feat_property;
    GdkTexture *type_feat_provide;
    GdkTexture *type_feat_require;
    GdkTexture *type_feat_rx;
    GdkTexture *type_feat_tag;
    GdkTexture *type_feat_tx;

    GdkTexture *type_rel_aggregate;
    GdkTexture *type_rel_associate;
    GdkTexture *type_rel_async;
    GdkTexture *type_rel_communication_path;
    GdkTexture *type_rel_compose;
    GdkTexture *type_rel_contain;
    GdkTexture *type_rel_control_flow;
    GdkTexture *type_rel_depend;
    GdkTexture *type_rel_deploy;
    GdkTexture *type_rel_extend;
    GdkTexture *type_rel_generalize;
    GdkTexture *type_rel_include;
    GdkTexture *type_rel_manifest;
    GdkTexture *type_rel_object_flow;
    GdkTexture *type_rel_realize;
    GdkTexture *type_rel_refine;
    GdkTexture *type_rel_return;
    GdkTexture *type_rel_sync;
    GdkTexture *type_rel_trace;
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
static inline GdkTexture *gui_resources_get_crystal_facet_uml ( const gui_resources_t *this_ );

/*!
 *  \brief gets the file_new
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_file_new ( const gui_resources_t *this_ );

/*!
 *  \brief gets the file_open
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_file_open ( const gui_resources_t *this_ );

/*!
 *  \brief gets the file_save
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_file_save ( const gui_resources_t *this_ );

/*!
 *  \brief gets the file_saved
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_file_saved ( const gui_resources_t *this_ );

/*!
 *  \brief gets the file_unsaved
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_file_unsaved ( const gui_resources_t *this_ );

/*!
 *  \brief gets the file_export
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_file_export ( const gui_resources_t *this_ );

/*!
 *  \brief gets the edit_attributes_sect
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_edit_attributes_sect ( const gui_resources_t *this_ );

/*!
 *  \brief gets the edit_copy
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_edit_copy ( const gui_resources_t *this_ );

/*!
 *  \brief gets the edit_cut
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_edit_cut ( const gui_resources_t *this_ );

/*!
 *  \brief gets the edit_delete
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_edit_delete ( const gui_resources_t *this_ );

/*!
 *  \brief gets the edit_instantiate
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_edit_instantiate ( const gui_resources_t *this_ );

/*!
 *  \brief gets the edit_highlight
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_edit_highlight ( const gui_resources_t *this_ );

/*!
 *  \brief gets the edit_paste
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_edit_paste ( const gui_resources_t *this_ );

/*!
 *  \brief gets the edit_redo
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_edit_redo ( const gui_resources_t *this_ );

/*!
 *  \brief gets the edit_undo
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_edit_undo ( const gui_resources_t *this_ );

/*!
 *  \brief gets the edit_reset
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_edit_reset ( const gui_resources_t *this_ );

/*!
 *  \brief gets the message_error
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_message_error ( const gui_resources_t *this_ );

/*!
 *  \brief gets the message_info
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_message_info ( const gui_resources_t *this_ );

/*!
 *  \brief gets the message_warn
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_message_warn ( const gui_resources_t *this_ );

/*!
 *  \brief gets the message_user_doc
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_message_user_doc ( const gui_resources_t *this_ );

/*!
 *  \brief gets the view_new_window
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_view_new_window ( const gui_resources_t *this_ );

/*!
 *  \brief gets the view_search
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_view_search ( const gui_resources_t *this_ );

/*!
 *  \brief gets the view_navigate
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_view_navigate ( const gui_resources_t *this_ );

/*!
 *  \brief gets the view_create
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_view_create ( const gui_resources_t *this_ );

/*!
 *  \brief gets the view_edit
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_view_edit ( const gui_resources_t *this_ );

/*!
 *  \brief gets the tool_sect
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_tool_sect ( const gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_breadcrumb_folder
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_navigate_breadcrumb_folder ( const gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_closed_folder
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_navigate_closed_folder ( const gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_open_folder
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_navigate_open_folder ( const gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_create_child
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_navigate_create_child ( const gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_create_child_0
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_navigate_create_child_0 ( const gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_create_sibling
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_navigate_create_sibling ( const gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_create_sibling_0
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_navigate_create_sibling_0 ( const gui_resources_t *this_ );

/*!
 *  \brief gets the search_search
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_search_search ( const gui_resources_t *this_ );

/*!
 *  \brief gets the background_column as GdkPixbuf
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_background_column ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_undef
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_undef ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_activity
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_activity ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_block
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_block ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_box
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_box ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_class
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_class ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_communication
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_communication ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_component
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_component ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_composite
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_composite ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_deployment
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_deployment ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_internal
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_internal ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_list
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_list ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_overview
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_overview ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_package
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_package ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_parametric
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_parametric ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_profile
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_profile ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_requirement
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_requirement ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_sequence
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_sequence ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_state
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_state ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_timing
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_timing ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_diag_usecase
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_diag_usecase ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_accept
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_accept ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_activity
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_activity ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_actor
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_actor ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_artifact
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_artifact ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_block
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_block ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_class
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_class ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_comment
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_comment ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_component
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_component ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_constraint
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_constraint ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_decision
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_decision ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_deephistory
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_deephistory ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_diagram
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_diagram ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_final
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_final ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_fork
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_fork ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_history
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_history ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_image
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_image ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_initial
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_initial ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_interface
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_interface ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_join
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_join ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_node
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_node ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_object
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_object ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_package
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_package ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_part
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_part ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_region
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_region ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_requirement
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_requirement ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_send
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_send ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_state
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_state ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_stereotype
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_stereotype ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_subsystem
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_subsystem ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_time
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_time ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_clas_usecase
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_clas_usecase ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_entry
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_feat_entry ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_exit
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_feat_exit ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_life
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_feat_life ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_operation
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_feat_operation ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_port
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_feat_port ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_property
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_feat_property ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_provide
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_feat_provide ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_require
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_feat_require ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_rx
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_feat_rx ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_tag
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_feat_tag ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_feat_tx
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_feat_tx ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_aggregate
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_aggregate ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_associate
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_associate ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_async
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_async ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_compose
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_compose ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_communication_path
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_communication_path ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_contain
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_contain ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_control_flow
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_control_flow ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_depend
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_depend ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_deploy
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_deploy ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_extend
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_extend ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_generalize
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_generalize ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_include
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_include ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_manifest
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_manifest ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_object_flow
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_object_flow ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_realize
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_realize ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_refine
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_refine ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_return
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_return ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_sync
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_sync ( const gui_resources_t *this_ );

/*!
 *  \brief gets the type_rel_trace
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkTexture *gui_resources_get_type_rel_trace ( const gui_resources_t *this_ );

#include "gui_resources.inl"

#endif  /* GUI_RESOURCES_H */


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
