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
 *  \brief enumeration of all diagram types
 */
enum test_data_setup_mode_enum {
    TEST_DATA_SETUP_MODE_GOOD_CASES = 0,  /*!< diagram data that should be drawn by pencil without overlaps or other problems */
    TEST_DATA_SETUP_MODE_CHALLENGING_CASES = 1,  /*!< diagram data that should be drawn by pencil, possibly with overlaps or other warnings */
    TEST_DATA_SETUP_MODE_EDGE_CASES = 2,  /*!< diagram data that should be drawn by pencil, possibly only partly or with rendering errors */
};

typedef enum test_data_setup_mode_enum test_data_setup_mode_t;


/*!
 *  \brief attributes of the test_data_setup_t
 */
struct test_data_setup_struct {
    unsigned int variant;  /*!< current variant for which to produce test data */
    test_data_setup_mode_t mode;  /*!< kind of test data that shall be produced */
};

typedef struct test_data_setup_struct test_data_setup_t;

/*!
 *  \brief initializes the test_data_setup_t
 *
 *  \param this_ pointer to own object attributes
 *  \param mode the mode attribute
 */
static inline void test_data_setup_init( test_data_setup_t *this_, test_data_setup_mode_t mode );

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
static inline bool test_data_setup_is_valid_variant( const test_data_setup_t *this_ );

/*!
 *  \brief gets the current variant
 *
 *  \param this_ pointer to own object attributes
 *  \return variant attribute
 */
static inline unsigned int test_data_setup_get_variant( const test_data_setup_t *this_ );

/*!
 *  \brief gets the current mode
 *
 *  \param this_ pointer to own object attributes
 *  \return mode attribute
 */
static inline test_data_setup_mode_t test_data_setup_get_mode( const test_data_setup_t *this_ );

/*!
 *  \brief initializes the data_visible_set_t according to the mode and the set variant
 *
 *  \param this_ pointer to own object attributes
 *  \param io_data_set pointer to the data_visible_set_t which shall be partily initialized
 */
static inline void test_data_setup_get_variant_data( const test_data_setup_t *this_, data_visible_set_t *io_data_set );

/*!
 *  \brief initializes the diagram in the data_visible_set_t according to the set variant
 *
 *  \param this_ pointer to own object attributes
 *  \param io_data_set pointer to the data_visible_set_t which shall be partily initialized
 */
static inline void test_data_setup_private_set_diagram( const test_data_setup_t *this_, data_visible_set_t *io_data_set );

/*!
 *  \brief adds visibla_classifiers to the data_visible_set_t according to the set variant
 *
 *  \param this_ pointer to own object attributes
 *  \param io_data_set pointer to the data_visible_set_t which shall be partily initialized
 */
static inline void test_data_setup_private_add_classifiers( const test_data_setup_t *this_, data_visible_set_t *io_data_set );

/*!
 *  \brief  adds lifelines to the data_visible_set_t, one per visible classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param io_data_set pointer to the data_visible_set_t which contains visible classifiers and which shall be partily initialized
 */
static inline void test_data_setup_private_add_lifelines( const test_data_setup_t *this_, data_visible_set_t *io_data_set );

/*!
 *  \brief  adds features to the data_visible_set_t according to the set variant
 *
 *  \param this_ pointer to own object attributes
 *  \param io_data_set pointer to the data_visible_set_t which contains visible classifiers and which shall be partily initialized
 */
static inline void test_data_setup_private_add_features( const test_data_setup_t *this_, data_visible_set_t *io_data_set );

/*!
 *  \brief  adds relationships to the data_visible_set_t according to the set variant
 *
 *  \param this_ pointer to own object attributes
 *  \param io_data_set pointer to the data_visible_set_t which contains visible classifiers and which shall be partily initialized
 */
static inline void test_data_setup_private_add_relationships( const test_data_setup_t *this_, data_visible_set_t *io_data_set );


#include "test_data_setup.inl"

#endif  /* TEST_DATA_SETUP_H */


/*
 * Copyright 2021-2026 Andreas Warnke
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
