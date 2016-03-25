/* File: gtk_helper_tree_model.h; Copyright and License: see below */

#ifndef GTK_HELPER_TREE_MODEL_H
#define GTK_HELPER_TREE_MODEL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief converts between ids and indexes in a GTK Tree Model
 */

#include <gtk/gtk.h>

/*!
 *  \brief gets the index in the model of the given id
 *
 *  \param id_col column in the tree model that contains the id
 *  \param id the id to find
 *  \return index of the row or -1 if not found
 *
 */
static inline int gtk_helper_tree_model_get_index ( GtkTreeModel *tree_model, int id_col, int id );

/*!
 *  \brief gets the id in the model of the given index
 *
 *  \param id_col column in the tree model that contains the id
 *  \param index the row to look up
 *  \return id of the row or -1 if index out of range
 *
 */
static inline int gtk_helper_tree_model_get_id ( GtkTreeModel *tree_model, int id_col, int index );


#include "util/gtk_helper/gtk_helper_tree_model.inl"

#endif  /* GTK_HELPER_TREE_MODEL_H */


/*
Copyright 2016-2016 Andreas Warnke

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
