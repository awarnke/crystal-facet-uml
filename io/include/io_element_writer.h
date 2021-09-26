/* File: io_element_writer.h; Copyright and License: see below */

#ifndef IO_ELEMENT_WRITER_H
#define IO_ELEMENT_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a pair of a) a pointer to a concrete instance of an interface
 *  and b) a pointer to objectdata that implements the interface.
 */

#include "stream/io_element_writer_if.h"

/*!
 *  \brief object (vmt+data) of a io_element_writer_t.
 *
 */
struct io_element_writer_struct {
    const io_element_writer_if_t* interface;  /*!< set of interface functions to write to a stream, kind of VMT */
    void* objectdata;  /*!< object that implements writing to a stream, used in interface functions as this_ parameter */
};

typedef struct io_element_writer_struct io_element_writer_t;

/*!
 *  \brief initializes the io_element_writer_t
 *
 *  Note: While init only initializes the \c io_element_writer_t struct,
 *  destroy also calls the destroy function in \c io_element_writer_if_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param interface set of interface functions to write to a stream
 *  \param objectdata object that implements writing to a stream
 */
static inline void io_element_writer_init( io_element_writer_t *this_,
                                           const io_element_writer_if_t *interface,
                                           void* objectdata
                                         );

/*!
 *  \brief destroys the io_element_writer_t.
 *
 *  This function does NOT call \c destroy on the \c interface.
 *
 *  \param this_ pointer to own object attributes
 *  \result returns 0 if success, -1 in case of error
 */
static inline int io_element_writer_destroy( io_element_writer_t *this_ );

/*!
 *  \brief gets the set of interface functions
 *
 *  \param this_ pointer to own object attributes
 *  \result the set of interface functions
 */
static inline const io_element_writer_if_t* io_element_writer_get_interface ( io_element_writer_t *this_ );

/*!
 *  \brief gets the object that implements writing to a stream
 *
 *  \param this_ pointer to own object attributes
 *  \result the object data that implements the interface
 */
static inline void* io_element_writer_get_objectdata ( io_element_writer_t *this_ );

/*!
 *  \brief calls \c open on the \c interface
 *
 *  \param this_ pointer to own object attributes
 *  \param identifier identifier of the stream, e.g. a path in case of a file stream.
 *  \result returns 0 if success, -1 in case of error
 */
static inline int io_element_writer_open (io_element_writer_t* this_, const char* identifier);

/*!
 *  \brief calls \c write on the \c interface
 *
 *  \param this_ pointer to own object attributes
 *  \param start start address from where to write
 *  \param length length in bytes to write
 *  \result returns 0 if success, -1 in case of error
 */
static inline int io_element_writer_write (io_element_writer_t* this_, const void *start, size_t length);

/*!
 *  \brief calls \c flush on the \c interface
 *
 *  \param this_ pointer to own object attributes
 *  \result returns 0 if success, -1 in case of error
 */
static inline int io_element_writer_flush (io_element_writer_t* this_);

/*!
 *  \brief calls \c close on the \c interface
 *
 *  \param this_ pointer to own object attributes
 *  \result returns 0 if success, -1 in case of error
 */
static inline int io_element_writer_close (io_element_writer_t* this_);

#include "io_element_writer.inl"

#endif  /* IO_ELEMENT_WRITER_H */


/*
Copyright 2021-2021 Andreas Warnke

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
