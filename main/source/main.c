/* Program: gFontCharacterViewer; Author: Andreas Warnke; License: Apache 2.0 */

#include "gui/gui_main.h"
#include "util/trace.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/*!
 *  \brief main starts the gui
 */
int main (int argc, char *argv[]) {
    TRACE_BEGIN();
    int exit_code = 0;

    TRACE_INFO("Handling GUI...");
    gui_main(argc, argv);

    TRACE_END_ERR(exit_code);
    return exit_code;
}


