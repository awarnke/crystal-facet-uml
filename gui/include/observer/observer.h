/* File: observer.h; Copyright and License: see below */

#ifndef OBSERVER_H
#define OBSERVER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief abstracts the actual observer from the observed subject.
 */

/*!
 *  \brief attributes of an observer
 */
struct observer_struct {
    void *observer_instance;
    void (*observer_callback)(void* observer_instance, void* call_param);
    const char* callback_name;
};

typedef struct observer_struct observer_t;

/*!
 *  \brief initializes the observer_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param observer_instance a pointer to data that is passed to the callback function as first parameter
 *  \param observer_callback a callback function. Simply dereference the function name, omit parameters and add a typecast, e.g. (void (*)(void*,void*)) &my_callback_function
 *  \param callback_name name of the callback function, used for tracing and debugging
 */
static inline void observer_init ( observer_t *this_,
                                   void *observer_instance,
                                   void (*observer_callback)(void *observer_instance, void *call_param),
                                   const char* callback_name
                                 );

/*!
 *  \brief destroys the observer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void observer_destroy ( observer_t *this_ );

/*!
 *  \brief synchronously calls a callback function at the observer
 *
 *  \param this_ pointer to own object attributes
 *  \param call_param a pointer to data that is passed to the callback function as second parameter
 */
static inline void observer_notify ( observer_t *this_, void *call_param );

#include "observer/observer.inl"

#endif  /* OBSERVER_H */


/*
Copyright 2016-2025 Andreas Warnke

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
