/* File: geometry_connector_segment.h; Copyright and License: see below */

#ifndef GEOMETRY_CONNECTOR_SEGMENT_H
#define GEOMETRY_CONNECTOR_SEGMENT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Identified line segments of a connector.
 *
 *  The connector consists of a source-end,
 *  connected to a main-line, connected to a destination-end.
 */

/*!
 *  \brief constants identifying connector segments
 */
enum geometry_connector_segment_enum {
    GEOMETRY_CONNECTOR_SEGMENT_SOURCE = 0,
    GEOMETRY_CONNECTOR_SEGMENT_MAIN = 1,
    GEOMETRY_CONNECTOR_SEGMENT_DESTINATION = 2,
    GEOMETRY_CONNECTOR_SEGMENT_MAX = 3
};

typedef enum geometry_connector_segment_enum geometry_connector_segment_t;

#endif  /* GEOMETRY_CONNECTOR_SEGMENT_H */


/*
Copyright 2022-2023 Andreas Warnke

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
