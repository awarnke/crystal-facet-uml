/* File: test_data_setup.h; Copyright and License: see below */

#ifndef TEST_DATA_SETUP_H
#define TEST_DATA_SETUP_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides functions to setup test case input data
 */

#include "set/data_visible_set.h"

/*!
 *  \brief attributes of the test_data_setup_t
 */
struct test_data_setup_struct {
    unsigned int variant;  /*!< curerent variant for which to produce test data */
};

typedef struct test_data_setup_struct test_data_setup_t;

/*!
 *  \brief initializes the test_data_setup_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void test_data_setup_init( test_data_setup_t *this_ );

/*!
 *  \brief er-initializes the test_data_setup_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void test_data_setup_reinit( test_data_setup_t *this_ );

/*!
 *  \brief destroys the test_data_setup_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void test_data_setup_destroy( test_data_setup_t *this_ );

/*!
 *  \brief selects the next variant. In case there is no next variant, switches to invalid variant
 *
 *  \param this_ pointer to own object attributes
 */
static inline void test_data_setup_next_variant( test_data_setup_t *this_ );

/*!
 *  \brief checks if the current variant is a valid variant
 *
 *  \param this_ pointer to own object attributes
 *  \return true if there is a next variant
 */
static inline bool test_data_setup_is_valid_variant( test_data_setup_t *this_ );

/*!
 *  \brief initializes the diagram ni the data_visible_set_t according to the set variant
 *
 *  \param this_ pointer to own object attributes
 *  \param io_data_set pointer to the data_visible_set_t whic shall be partily initialized
 */
static inline void test_data_setup_diagram( const test_data_setup_t *this_, data_visible_set_t *io_data_set );


#include "test_data_setup.inl"

#endif  /* TEST_DATA_SETUP_H */


/*
 * Copyright 2021-2021 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
