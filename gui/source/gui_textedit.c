/* File: gui_textedit.c; Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

#include "gui_textedit.h"
#include "trace.h"
#include <gtk/gtk.h>

void btn_clear_callback( GtkButton* button, gpointer data ) {
    TRACE_BEGIN();

    gui_textedit_t (*this_) = (gui_textedit_t*) data;
    /*TRACE_DEBUG("clear\n");*/

    TRACE_END();
}
