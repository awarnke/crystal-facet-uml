/* File: universal_input_stream.h; Copyright and License: see below */

#ifndef UNIVERSAL_INPUT_STREAM_H
#define UNIVERSAL_INPUT_STREAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a pair of a) a pointer to a concrete instance of an interface
 *  and b) a pointer to objectdata that implements the interface.
 */

#include "u8stream/universal_input_stream_if.h"
#include "u8/u8_error.h"

/*!
 *  \brief object (vmt+data) of a universal_input_stream_t.
 *
 */
struct universal_input_stream_struct {
    const universal_input_stream_if_t* interface;  /*!< set of interface functions to write to a stream, kind of VMT */
    universal_input_stream_impl_t* objectdata;  /*!< object that implements writing to a stream, used in interface functions as this_ parameter */
};

typedef struct universal_input_stream_struct universal_input_stream_t;

/*!
 *  \brief initializes the universal_input_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \param interface set of interface functions to write to a stream
 *  \param objectdata object that implements writing to a stream
 */
static inline void universal_input_stream_private_init( universal_input_stream_t *this_,
                                                        const universal_input_stream_if_t *interface,
                                                        universal_input_stream_impl_t* objectdata
                                                      );

/*!
 *  \brief destroys the universal_input_stream_t.
 *
 *  This function does NOT call \c destroy on the \c interface.
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t universal_input_stream_private_destroy( universal_input_stream_t *this_ );

/*!
 *  \brief gets the set of interface functions
 *
 *  \param this_ pointer to own object attributes
 *  \return the set of interface functions
 */
static inline const universal_input_stream_if_t* universal_input_stream_get_interface ( universal_input_stream_t *this_ );

/*!
 *  \brief gets the object that implements writing to a stream
 *
 *  \param this_ pointer to own object attributes
 *  \return the object data that implements the interface
 */
static inline universal_input_stream_impl_t* universal_input_stream_get_objectdata ( universal_input_stream_t *this_ );

/*!
 *  \brief calls \c read on the \c interface
 *
 *  \param this_ pointer to own object attributes
 *  \param out_buffer buffer to write read bytes
 *  \param max_size length of the buffer to write
 *  \param out_length number of bytes read
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_END_OF_STREAM when at end, U8_ERROR_AT_FILE_READ otherwise
 */
static inline u8_error_t universal_input_stream_read ( universal_input_stream_t *this_, void *out_buffer, size_t max_size, size_t *out_length );

/*!
 *  \brief resets the read position to a starting point
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_READ otherwise
 */
static inline u8_error_t universal_input_stream_reset ( universal_input_stream_t *this_ );

#include "universal_input_stream.inl"

#endif  /* UNIVERSAL_INPUT_STREAM_H */


/*
Copyright 2021-2022 Andreas Warnke

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
