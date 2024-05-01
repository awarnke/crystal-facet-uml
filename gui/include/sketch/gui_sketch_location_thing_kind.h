/* File: gui_sketch_location_thing_kind.h; Copyright and License: see below */

#ifndef GUI_SKETCH_LOCATION_THING_KIND_H
#define GUI_SKETCH_LOCATION_THING_KIND_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Describes the thing-kind at a location.
 *
 *  gui_sketch_location_thing_kind_t defines visible element parts.
 */

/*!
 *  \brief list of thing-types
 */
enum gui_sketch_location_thing_kind_enum {
    GUI_SKETCH_LOCATION_THING_KIND_VOID,
    GUI_SKETCH_LOCATION_THING_KIND_OUTLINE,  /* this is the border or the connector line */
    GUI_SKETCH_LOCATION_THING_KIND_LABEL,  /* this is the name, stereotype or icon */
    GUI_SKETCH_LOCATION_THING_KIND_SPACE,  /* the inner space of a classifier or diagram */
};

typedef enum gui_sketch_location_thing_kind_enum gui_sketch_location_thing_kind_t;

#endif  /* GUI_SKETCH_LOCATION_THING_KIND_H */


/*
Copyright 2024-2024 Andreas Warnke

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
