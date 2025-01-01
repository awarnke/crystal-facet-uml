/* File: layout_subelement_id.h; Copyright and License: see below */

#ifndef LAYOUT_SUBELEMENT_ID_H
#define LAYOUT_SUBELEMENT_ID_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Describes the thing at a location.
 *
 *  This may be a classifier with a lifeline, a space within a container,
 *  a label or border or symbol.
 *  layout_subelement_id_t stores the type and the id of an element and if needed their parent classifier.
 */

#include "layout/layout_subelement_kind.h"
#include "gui_marked_set.h"
#include "shape/shape_int_rectangle.h"
#include "pencil_marker.h"
#include "entity/data_id.h"
#include "set/data_full_id.h"
#include <stdint.h>

#define LAYOUT_SUBELEMENT_ID_VOID ((layout_subelement_id_t){.kind=LAYOUT_SUBELEMENT_KIND_VOID,.id=DATA_FULL_ID_VOID})

/*!
 *  \brief attributes of the thing
 */
struct layout_subelement_id_struct {
    layout_subelement_kind_t kind;
    data_full_id_t id;
};

typedef struct layout_subelement_id_struct layout_subelement_id_t;

/*!
 *  \brief initializes the layout_subelement_id_t
 *
 *  \param this_ pointer to own object attributes
 *  \param kind type of the location_thing
 *  \param id pointer to element and possibly parent classifier (if applicable)
 */
static inline void layout_subelement_id_init( layout_subelement_id_t *this_,
                                              layout_subelement_kind_t kind,
                                              const data_full_id_t *id
                                            );

/*!
 *  \brief initializes the layout_subelement_id_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_subelement_id_init_void( layout_subelement_id_t *this_ );

/*!
 *  \brief re-initializes the layout_subelement_id_t
 *
 *  \param this_ pointer to own object attributes
 *  \param kind type of the location_thing
 *  \param id pointer to element and possibly parent classifier (if applicable)
 */
static inline void layout_subelement_id_reinit( layout_subelement_id_t *this_,
                                                layout_subelement_kind_t kind,
                                                const data_full_id_t *id
                                              );

/*!
 *  \brief initializes the layout_subelement_id_t by a single data_id
 *
 *  \param this_ pointer to own object attributes
 *  \param kind type of the location_thing
 *  \param id pointer to element
 */
static inline void layout_subelement_id_init_solo( layout_subelement_id_t *this_,
                                                   layout_subelement_kind_t kind,
                                                   const data_id_t *id
                                                 );

/*!
 *  \brief re-initializes the layout_subelement_id_t by a single data_id
 *
 *  \param this_ pointer to own object attributes
 *  \param kind type of the location_thing
 *  \param id pointer to element
 */
static inline void layout_subelement_id_reinit_solo( layout_subelement_id_t *this_,
                                                     layout_subelement_kind_t kind,
                                                     const data_id_t *id
                                                   );

/*!
 *  \brief copies the layout_subelement_id_t
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to other object to be copied
 */
static inline void layout_subelement_id_copy( layout_subelement_id_t *this_, const layout_subelement_id_t *that );

/*!
 *  \brief replaces the already initialized layout_subelement_id_t
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to other object to be copied
 */
static inline void layout_subelement_id_replace( layout_subelement_id_t *this_, const layout_subelement_id_t *that );

/*!
 *  \brief destroys the layout_subelement_id_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_subelement_id_destroy( layout_subelement_id_t *this_ );

/*!
 *  \brief returns if object refering to a valid location_thing
 *
 *  \param this_ pointer to own object attributes
 *  \return true if kind and id are not void
 */
static inline bool layout_subelement_id_is_valid( const layout_subelement_id_t *this_ );

/*!
 *  \brief gets the attribute of layout_subelement_id_t: kind
 *
 *  \param this_ pointer to own object attributes
 *  \return type of the location_thing
 */
static inline layout_subelement_kind_t layout_subelement_id_get_kind( const layout_subelement_id_t *this_ );

/*!
 *  \brief gets the attribute of layout_subelement_id_t: id
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the location_thing
 */
static inline const data_full_id_t *layout_subelement_id_get_id_const( const layout_subelement_id_t *this_ );

#include "layout_subelement_id.inl"

#endif  /* LAYOUT_SUBELEMENT_ID_H */


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
