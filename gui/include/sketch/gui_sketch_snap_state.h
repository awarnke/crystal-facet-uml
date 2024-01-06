/* File: gui_sketch_snap_state.h; Copyright and License: see below */

#ifndef GUI_SKETCH_SNAP_STATE_H
#define GUI_SKETCH_SNAP_STATE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores an enumeration of snapping states
 */

/*!
 *  \brief enumeration of all snap states
 */
enum gui_sketch_snap_state_enum {
    GUI_SKETCH_SNAP_STATE_0 = 0,
    GUI_SKETCH_SNAP_STATE_X = 1,
    GUI_SKETCH_SNAP_STATE_Y = 2,
    GUI_SKETCH_SNAP_STATE_XY = 3,
};

typedef enum gui_sketch_snap_state_enum gui_sketch_snap_state_t;

#endif  /* GUI_SKETCH_SNAP_STATE_H */


/*
Copyright 2017-2024 Andreas Warnke

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
