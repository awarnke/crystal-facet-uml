/* File: gui_sketch_location_thing.h; Copyright and License: see below */

#ifndef GUI_SKETCH_LOCATION_THING_H
#define GUI_SKETCH_LOCATION_THING_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Describes the thing at a location.
 *
 *  This may be a classifier with a lifeline, a space within a container,
 *  a label or border or symbol.
 *  gui_sketch_location_thing_t stores the type and the id of an element and if needed their parent classifier.
 */

#include "sketch/gui_sketch_location_thing_kind.h"
#include "gui_marked_set.h"
#include "shape/shape_int_rectangle.h"
#include "pencil_marker.h"
#include "data_id.h"
#include "set/data_full_id.h"
#include <stdint.h>

/*!
 *  \brief attributes of the thing
 */
struct gui_sketch_location_thing_struct {
    gui_sketch_location_thing_kind_t kind;
    data_full_id_t id;
};

typedef struct gui_sketch_location_thing_struct gui_sketch_location_thing_t;

/*!
 *  \brief initializes the gui_sketch_location_thing_t
 *
 *  \param this_ pointer to own object attributes
 *  \param kind type of the location_thing
 *  \param id pointer to element and possibly parent classifier (if applicable)
 */
static inline void gui_sketch_location_thing_init( gui_sketch_location_thing_t *this_,
                                                   gui_sketch_location_thing_kind_t kind,
                                                   const data_full_id_t *id
                                                 );

/*!
 *  \brief initializes the gui_sketch_location_thing_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_location_thing_init_void( gui_sketch_location_thing_t *this_ );

/*!
 *  \brief re-initializes the gui_sketch_location_thing_t
 *
 *  \param this_ pointer to own object attributes
 *  \param kind type of the location_thing
 *  \param id pointer to element and possibly parent classifier (if applicable)
 */
static inline void gui_sketch_location_thing_reinit( gui_sketch_location_thing_t *this_,
                                                     gui_sketch_location_thing_kind_t kind,
                                                     const data_full_id_t *id
                                                   );

/*!
 *  \brief initializes the gui_sketch_location_thing_t by a single data_id
 *
 *  \param this_ pointer to own object attributes
 *  \param kind type of the location_thing
 *  \param id pointer to element
 */
static inline void gui_sketch_location_thing_init_solo( gui_sketch_location_thing_t *this_,
                                                        gui_sketch_location_thing_kind_t kind,
                                                        const data_id_t *id
                                                      );

/*!
 *  \brief re-initializes the gui_sketch_location_thing_t by a single data_id
 *
 *  \param this_ pointer to own object attributes
 *  \param kind type of the location_thing
 *  \param id pointer to element
 */
static inline void gui_sketch_location_thing_reinit_solo( gui_sketch_location_thing_t *this_,
                                                          gui_sketch_location_thing_kind_t kind,
                                                          const data_id_t *id
                                                        );

/*!
 *  \brief copies the gui_sketch_location_thing_t
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to other object to be copied
 */
static inline void gui_sketch_location_thing_copy( gui_sketch_location_thing_t *this_, const gui_sketch_location_thing_t *that );

/*!
 *  \brief replaces the already initialized gui_sketch_location_thing_t
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to other object to be copied
 */
static inline void gui_sketch_location_thing_replace( gui_sketch_location_thing_t *this_, const gui_sketch_location_thing_t *that );

/*!
 *  \brief destroys the gui_sketch_location_thing_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_location_thing_destroy( gui_sketch_location_thing_t *this_ );

/*!
 *  \brief returns if object refering to a valid location_thing
 *
 *  \param this_ pointer to own object attributes
 *  \return true if kind and id are not void
 */
static inline bool gui_sketch_location_thing_is_valid( const gui_sketch_location_thing_t *this_ );

/*!
 *  \brief gets the attribute of gui_sketch_location_thing_t: kind
 *
 *  \param this_ pointer to own object attributes
 *  \return type of the location_thing
 */
static inline gui_sketch_location_thing_kind_t gui_sketch_location_thing_get_kind( const gui_sketch_location_thing_t *this_ );

/*!
 *  \brief gets the attribute of gui_sketch_location_thing_t: id
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the location_thing
 */
static inline const data_full_id_t *gui_sketch_location_thing_get_id_const( const gui_sketch_location_thing_t *this_ );

#include "gui_sketch_location_thing.inl"

#endif  /* GUI_SKETCH_LOCATION_THING_H */


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
