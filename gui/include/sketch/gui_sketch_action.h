/* File: gui_sketch_action.h; Copyright and License: see below */

#ifndef GUI_SKETCH_ACTION_H
#define GUI_SKETCH_ACTION_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines action ids for buttons shown within the sketch area
 */

/*!
 *  \brief enumeration of all button actions
 */
enum gui_sketch_action_enum {
    GUI_SKETCH_ACTION_NONE = 0,
    GUI_SKETCH_ACTION_NEW_SIBLING_DIAGRAM = 1,
    GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM = 2,
    GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM = 3,
};

typedef enum gui_sketch_action_enum gui_sketch_action_t;

#endif  /* GUI_SKETCH_ACTION_H */


/*
Copyright 2018-2022 Andreas Warnke

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
