/* File: io_element_writer_if.h; Copyright and License: see below */

#ifndef IO_ELEMENT_WRITER_IF_H
#define IO_ELEMENT_WRITER_IF_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a set of function pointers that allow to implement an output stream.
 *
 *  This is an interface only - to be used in a pipes-and-filters architecture
 *  where pipes and filters do not know their predecessor or successor.
 *
 *  A user of this interface needs a) a pointer to a concrete instance of this interface
 *  and b) a pointer to an object that implements the interface (this_).
 */

#include "data_classifier.h"
#include "data_classifier_type.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "data_relationship_type.h"
#include "data_diagram.h"
#include "data_diagram_type.h"
#include "data_diagramelement.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* io_element_writer_impl_t allows to distinguisth general void pointers from pointers to implementation objects */
typedef void io_element_writer_impl_t;

/*!
 *  \brief function pointers of a io_element_writer_if_t.
 *
 *  This is similar to a vmt and used here to access an implementation of this interface
 *
 *  Lifecycle functions like init and destroy are not part of the interface.
 */
struct io_element_writer_if_struct {
#include "io_element_writer_if.inl"
};

typedef struct io_element_writer_if_struct io_element_writer_if_t;

#endif  /* IO_ELEMENT_WRITER_IF_H */


/*
Copyright 2021-2023 Andreas Warnke

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
