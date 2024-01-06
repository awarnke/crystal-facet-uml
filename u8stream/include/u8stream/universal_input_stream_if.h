/* File: universal_input_stream_if.h; Copyright and License: see below */

#ifndef UNIVERSAL_INPUT_STREAM_IF_H
#define UNIVERSAL_INPUT_STREAM_IF_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a set of function pointers that allow to implement a byte (u8) input stream.
 *
 *  This is an interface only - to be used in a pipes-and-filters architecture
 *  where pipes and filters do not know their predecessor or successor.
 *
 *  A user of this interface needs a) a pointer to a concrete instance of this interface
 *  and b) a pointer to an object that implements the interface (this_).
 */

#include "u8/u8_error.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* universal_input_stream_impl_t allows to distinguisth general void pointers from pointers to implementation objects */
typedef void universal_input_stream_impl_t;

/*!
 *  \brief function pointers of a universal_input_stream_if_t.
 *
 *  This is similar to a vmt and used here to access an implementation of this interface
 *
 *  Lifecycle functions like init and destroy are not part of the interface.
 */
struct universal_input_stream_if_struct {
    /*! a function to read bytes from an input stream into a buffer */
    u8_error_t (*read)(universal_input_stream_impl_t *this_, void *out_buffer, size_t max_size, size_t *out_length);
    /*! a function to reset the read position to a starting point */
    u8_error_t (*reset)(universal_input_stream_impl_t *this_ );
};

typedef struct universal_input_stream_if_struct universal_input_stream_if_t;

#endif  /* UNIVERSAL_INPUT_STREAM_IF_H */


/*
Copyright 2021-2024 Andreas Warnke

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
