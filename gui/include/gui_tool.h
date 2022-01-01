/* File: gui_tool.h; Copyright and License: see below */

#ifndef GUI_TOOL_H
#define GUI_TOOL_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Enumeration of gui tools
 */

#include <gtk/gtk.h>

/*!
 *  \brief enumeration on tools
 */
enum gui_tool_enum {
    GUI_TOOL_NAVIGATE,
    GUI_TOOL_EDIT,
    GUI_TOOL_CREATE,
    GUI_TOOL_SEARCH,
};

typedef enum gui_tool_enum gui_tool_t;

#endif  /* GUI_TOOL_H */


/*
Copyright 2016-2022 Andreas Warnke

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
