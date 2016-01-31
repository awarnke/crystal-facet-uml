/* File: gui_main.c;.c Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

#include "gui_main.h"
#include "gui_init.h"
#include "trace.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

void gui_main (int argc, char *argv[]) {
    TRACE_BEGIN();

    TRACE_INFO("initializing gui thread...");

    gtk_init(&argc, &argv);
    gui_init();

    gtk_main();

    TRACE_END();
}
