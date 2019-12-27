/* File: gui_attributes_editor_types_types.c; Copyright and License: see below */

#include "gui_attributes_editor_types.h"
#include "util/gtk_helper/gtk_helper_tree_model.h"
#include "trace.h"
#include "data_table.h"
#include "data_id.h"
#include <gtk/gtk.h>

void gui_attributes_editor_types_init ( gui_attributes_editor_types_t *this_, gui_resources_t *resources )
{
    TRACE_BEGIN();

    assert( NULL != resources );

    (*this_).resources = resources;

    {
        GtkTreeIter iter;
        (*this_).no_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        gtk_list_store_append( (*this_).no_types, &iter);
        gtk_list_store_set ( (*this_).no_types, &iter, 0, 0x0, 1, "-- n/a --", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).diagram_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        /* order: structural from abstract to concrete, behavioral from abstract to concrete */
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_LIST, 1, "List Diagram", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_BOX_DIAGRAM, 1, "Box Diagram", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_BLOCK_DEFINITION_DIAGRAM, 1, "Block Definition Diagram (SysML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_INTERNAL_BLOCK_DIAGRAM, 1, "Internal Block Diagram (SysML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM, 1, "Parametric Diagram (SysML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM, 1, "Deployment Diagram (UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM, 1, "Component Diagram (UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM, 1, "Composite Structure Diagram (UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, 1, "Package Diagram (SysML, UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM, 1, "Class Diagram (UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_PROFILE_DIAGRAM, 1, "Profile Diagram (UML)", -1 );
        /* requirements diagram: */
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM, 1, "Requirements Diagram (SysML)", -1 );
        /* behavioral diagrams: */
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM, 1, "Use Case Diagram (SysML, UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_INTERACTION_OVERVIEW_DIAGRAM, 1, "Interaction Overview Diagram (UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM, 1, "Activity Diagram (SysML, UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM, 1, "State Machine Diagram (SysML, UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM, 1, "Communication Diagram (UML), Scenario", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM, 1, "Sequence Diagram (SysML, UML), Scenario", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM, 1, "Timing Diagram (UML), Scenario", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).classifier_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        /* order: structural from abstract to concrete, behavioral from abstract to concrete */
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_BLOCK, 1, "Block", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY, 1, "Constraint Property/Equation (parametric)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_NODE, 1, "Node (deployment)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_COMPONENT, 1, "Component", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_PART, 1, "Part (component instance)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_INTERFACE, 1, "Interface", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_PACKAGE, 1, "Package", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_CLASS, 1, "Class", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_OBJECT, 1, "Object (class instance)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ARTIFACT, 1, "Artifact", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_COMMENT, 1, "Comment", -1 );
        /* requirements classifiers: */
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_FEATURE, 1, "Feature", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_REQUIREMENT, 1, "Requirement", -1 );
        /* behavioral classifiers: */
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ACTOR, 1, "Actor (use case)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_USE_CASE, 1, "Use Case (use case)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY, 1, "System Boundary (use case)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE, 1, "Diagram Reference (interaction overview)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ACTIVITY, 1, "Activity", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION, 1, "Interruptable Region", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_FORK_NODE, 1, "Fork", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE, 1, "Join", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT, 1, "Accept Event", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT, 1, "Accept Time Event", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL, 1, "Send Signal", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE, 1, "Decision", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE, 1, "Initial Node", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE, 1, "Final Node", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_STATE, 1, "State", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY, 1, "Shallow History", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY, 1, "Deep History", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).feature_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        /* order: from close to far */
        gtk_list_store_append( (*this_).feature_types, &iter);
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_PROPERTY, 1, "Property", -1 );
        gtk_list_store_append( (*this_).feature_types, &iter);
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_OPERATION, 1, "Operation", -1 );
        gtk_list_store_append( (*this_).feature_types, &iter);
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_PORT, 1, "Port", -1 );
        gtk_list_store_append( (*this_).feature_types, &iter);
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_PROVIDED_INTERFACE, 1, "Provided Interface", -1 );
        gtk_list_store_append( (*this_).feature_types, &iter);
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_REQUIRED_INTERFACE, 1, "Required Interface", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).feature_lifeline_type = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        gtk_list_store_append( (*this_).feature_lifeline_type, &iter);
        gtk_list_store_set ( (*this_).feature_lifeline_type, &iter, 0, DATA_FEATURE_TYPE_LIFELINE, 1, "Lifeline", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).relationship_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        /* order: structural from abstract to concrete, behavioral from abstract to concrete */
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY, 1, "Dependency", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT, 1, "Containment (deployment, package), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_DEPLOY, 1, "Deploy (deployment), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_MANIFEST, 1, "Manifest (deployment), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH, 1, "Communication Path (component)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION, 1, "Association (class, use-case)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_AGGREGATION, 1, "Aggregation (class), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, 1, "Composition (class), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION, 1, "Generalization (class), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_REALIZATION, 1, "Realization (class), no cycles", -1 );
        /* requirements relationships: */
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_TRACE, 1, "Trace (requirement), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_REFINE, 1, "Refine (requirement), no cycles", -1 );
        /* behavioral relationships: */
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_EXTEND, 1, "Extend (use-case), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_INCLUDE, 1, "Include (use-case), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW, 1, "Control Flow (activity)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW, 1, "Object Flow (activity)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL, 1, "Async. Call (sequence)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL, 1, "Sync. Call (sequence)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL, 1, "Return Call (sequence)", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).shortlist_types = gtk_list_store_new( 3, G_TYPE_INT, GDK_TYPE_PIXBUF, G_TYPE_STRING );
        gtk_list_store_append( (*this_).shortlist_types, &iter);
        gtk_list_store_set ( (*this_).shortlist_types, &iter, 0, DATA_DIAGRAM_TYPE_LIST, 1, "List Diagram", 2, gui_resources_get_tool_search( (*this_).resources ), -1 );
        gtk_list_store_append( (*this_).shortlist_types, &iter);
        gtk_list_store_set ( (*this_).shortlist_types, &iter, 0, DATA_DIAGRAM_TYPE_BOX_DIAGRAM, 1, "Box Diagram", 2, gui_resources_get_tool_navigate( (*this_).resources ), -1 );
    }

    TRACE_END();
}

void gui_attributes_editor_types_destroy ( gui_attributes_editor_types_t *this_ )
{
    TRACE_BEGIN();

    g_object_unref((*this_).no_types);
    (*this_).no_types = NULL;

    g_object_unref((*this_).diagram_types);
    (*this_).diagram_types = NULL;

    g_object_unref((*this_).classifier_types);
    (*this_).classifier_types = NULL;

    g_object_unref((*this_).feature_types);
    (*this_).feature_types = NULL;

    g_object_unref((*this_).feature_lifeline_type);
    (*this_).feature_lifeline_type = NULL;

    g_object_unref((*this_).relationship_types);
    (*this_).relationship_types = NULL;

    g_object_unref((*this_).shortlist_types);
    (*this_).shortlist_types = NULL;

    (*this_).resources = NULL;

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
