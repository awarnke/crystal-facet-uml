/* File: universal_output_stream.h; Copyright and License: see below */

#ifndef UNIVERSAL_OUTPUT_STREAM_H
#define UNIVERSAL_OUTPUT_STREAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a pair of a) a pointer to a concrete instance of an interface
 *  and b) a pointer to objectdata that implements the interface.
 */

/*!
 *  \brief object (vmt+data) of a universal_output_stream_t.
 *
 */
struct universal_output_stream_struct {
    const universal_output_stream_if_t* interface;  /*!< set of interface functions to write to a stream */
    void* objectdata;  /*!< object that implements writing to a stream */
};

typedef struct universal_output_stream_struct universal_output_stream_t;


/*!
 *  \brief initializes the universal_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \param interface set of interface functions to write to a stream
 *  \param objectdata object that implements writing to a stream
 */
void universal_output_stream_init( universal_output_stream_t *this_,
                                   const universal_output_stream_if_t *interface,
                                   void* objectdata
                                 );

/*!
 *  \brief destroys the universal_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 */
void universal_output_stream_destroy( universal_output_stream_t *this_ );

/*!
 *  \brief gets the set of interface functions
 *
 *  \param this_ pointer to own object attributes
 *  \result the set of interface functions
 */
static inline const universal_output_stream_if_t* universal_output_stream_get_interface ( universal_output_stream_t *this_ );

/*!
 *  \brief gets the object that implements writing to a stream
 *
 *  \param this_ pointer to own object attributes
 *  \result the object data that implements the interface
 */
static inline void* universal_output_stream_get_objectdata ( universal_output_stream_t *this_ );

#include "universal_output_stream.inl"

#endif  /* UNIVERSAL_OUTPUT_STREAM_H */


/*
Copyright 2020-2020 Andreas Warnke

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
