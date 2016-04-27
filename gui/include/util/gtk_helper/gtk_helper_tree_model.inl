/* File: gtk_helper_tree_model.inl; Copyright and License: see below */

#include "log.h"
#include <assert.h>

static inline int gtk_helper_tree_model_get_index ( GtkTreeModel *tree_model, int id_col, int id )
{
    GtkTreeIter iter;
    gboolean success;
    int result;
    int row;

    result = -1;

    row = 0;
    success = gtk_tree_model_get_iter_first ( tree_model, &iter );
    while ( success )
    {
        GValue probe = G_VALUE_INIT;
        gtk_tree_model_get_value ( tree_model, &iter, id_col, &probe );

        assert( G_VALUE_TYPE(&probe) == G_TYPE_INT );

        int probe_int;
        probe_int = g_value_get_int ( &probe );

        if ( probe_int == id )
        {
            result = row;
            break;
        }

        success = gtk_tree_model_iter_next ( tree_model, &iter );
        row ++;
    }

    return result;
}

static inline int gtk_helper_tree_model_get_id ( GtkTreeModel *tree_model, int id_col, int index )
{
    GtkTreeIter iter;
    gboolean success;
    int result;

    result = -1;

    success = gtk_tree_model_get_iter_first ( tree_model, &iter );
    for ( int row = 0; (row < index) && success; row ++ )
    {
        success = gtk_tree_model_iter_next ( tree_model, &iter );
    }

    if ( success )
    {
        GValue probe = G_VALUE_INIT;
        gtk_tree_model_get_value ( tree_model, &iter, id_col, &probe );

        assert( G_VALUE_TYPE(&probe) == G_TYPE_INT );

        result = g_value_get_int ( &probe );
    }

    return result;
}


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
