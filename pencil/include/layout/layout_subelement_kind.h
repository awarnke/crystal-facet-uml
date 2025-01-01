/* File: layout_subelement_kind.h; Copyright and License: see below */

#ifndef LAYOUT_SUBELEMENT_KIND_H
#define LAYOUT_SUBELEMENT_KIND_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Describes the thing-kind at a location.
 *
 *  layout_subelement_kind_t defines visible element parts.
 */

/*!
 *  \brief list of thing-types
 */
enum layout_subelement_kind_enum {
    LAYOUT_SUBELEMENT_KIND_VOID,
    LAYOUT_SUBELEMENT_KIND_OUTLINE,  /* this is the border or the connector line */
    LAYOUT_SUBELEMENT_KIND_LABEL,  /* this is the name, stereotype or icon */
    LAYOUT_SUBELEMENT_KIND_SPACE,  /* the inner space of a classifier or diagram */
};

typedef enum layout_subelement_kind_enum layout_subelement_kind_t;

#endif  /* LAYOUT_SUBELEMENT_KIND_H */


/*
Copyright 2024-2025 Andreas Warnke

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
