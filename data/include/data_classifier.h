/* File: data_classifier.h; Copyright and License: see below */

#ifndef DATA_CLASSIFIER_H
#define DATA_CLASSIFIER_H

/* public file for the doxygen documentation: */
/*! \file */

#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the classifier functions
 */
struct data_classifier_struct {
    bool is_valid;
};

typedef struct data_classifier_struct data_classifier_t;

/*!
 *  \brief initializes the data_classifier_t struct
 */
void data_classifier_init ( data_classifier_t *this_ );

/*!
 *  \brief destroys the data_classifier_t struct
 */
void data_classifier_destroy ( data_classifier_t *this_ );


#endif  /* DATA_CLASSIFIER_H */


/*
Copyright 2016-2016 Andreas Warnke

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
