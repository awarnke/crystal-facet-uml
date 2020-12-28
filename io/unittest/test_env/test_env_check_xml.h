/* File: test_env_check_xml.h; Copyright and License: see below */

#ifndef TEST_ENV_CHECK_XML_H
#define TEST_ENV_CHECK_XML_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides functions to check if test results are xml compliant
 */

#include "set/data_visible_set.h"
#include "data_classifier_type.h"
#include "data_feature_type.h"
#include "data_relationship_type.h"
#include "data_diagram_type.h"
#include "data_row_id.h"
#include <stdbool.h>

static int test_env_check_xml_is_valid(const char* xml_string);  /* helper function to validate xml */

#include "test_env_check_xml.inl"

#endif  /* TEST_ENV_CHECK_XML_H */


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
