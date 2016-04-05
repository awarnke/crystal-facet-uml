/* File: data_classifier.h; Copyright and License: see below */

#ifndef DATA_CLASSIFIER_H
#define DATA_CLASSIFIER_H

/* public file for the doxygen documentation: */
/*! \file */

#include "data_classifier_type.h"
#include "util/string/utf8stringbuf.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>

/*!
 *  \brief constants for max string sizes
 */
enum data_classifier_max_enum {
    DATA_CLASSIFIER_MAX_NAME_SIZE = 32,
    DATA_CLASSIFIER_MAX_NAME_LENGTH = 31,
    DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE = 256,
    DATA_CLASSIFIER_MAX_STEREOTYPE_LENGTH = 255,
    DATA_CLASSIFIER_MAX_DESCRIPTION_SIZE = 1024,
    DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH = 1023,
};

/*!
 *  \brief all data attributes needed for the classifier functions
 *
 *  \see https://en.wikipedia.org/wiki/Classifier_%28UML%29
 *  \see http://www.uml-diagrams.org/classifier.html
 */
struct data_classifier_struct {
    int64_t id;
    data_classifier_type_t main_type;
    utf8stringbuf_t stereotype;
    char private_stereotype_buffer[DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE];
    utf8stringbuf_t name;
    char private_name_buffer[DATA_CLASSIFIER_MAX_NAME_SIZE];
    utf8stringbuf_t description;
    char private_description_buffer[DATA_CLASSIFIER_MAX_DESCRIPTION_SIZE];
    int32_t x_order;
    int32_t y_order;
};

typedef struct data_classifier_struct data_classifier_t;

/*!
 *  \brief initializes the data_classifier_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_classifier_init ( data_classifier_t *this_ );

/*!
 *  \brief destroys the data_classifier_t struct
 *
 *  \param this_ pointer to own object attributes
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
