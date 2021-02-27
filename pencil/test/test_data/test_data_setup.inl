/* File: test_data_setup.c; Copyright and License: see below */

#include "trace.h"
#include "test_assert.h"
#include "assert.h"
        
#define TEST_DATA_SETUP_VARIANTS (6)

static inline void test_data_setup_init( test_data_setup_t *this_ )
{
    (*this_).variant = 0;
}

static inline void test_data_setup_reinit( test_data_setup_t *this_ )
{
    (*this_).variant = 0;
}

static inline void test_data_setup_destroy( test_data_setup_t *this_ )
{
}

static inline void test_data_setup_next_variant( test_data_setup_t *this_ )
{
    assert( (*this_).variant <= TEST_DATA_SETUP_VARIANTS );
    (*this_).variant++;
}

static inline bool test_data_setup_is_valid_variant( test_data_setup_t *this_ )
{
    return ( (*this_).variant < TEST_DATA_SETUP_VARIANTS );
}

static inline void test_data_setup_diagram( const test_data_setup_t *this_, data_visible_set_t *io_data_set )
{
    data_diagram_type_t diagram_type;
    const char* diagram_name;
    const char* diagram_description;
    switch ( (*this_).variant )
    {
        default:
        case 0:
        {
            diagram_type = DATA_DIAGRAM_TYPE_LIST;
            diagram_name = "";
            diagram_description = "";
        }
        break;
        
        case 1:
        {
            diagram_type = DATA_DIAGRAM_TYPE_BOX_DIAGRAM;
            diagram_name = "WWWWWWW WWWWWWW WWWWWWW WWWWWWW WWWWWWW WWWWWWW";
            diagram_description = "WWWWWWW WWWWWWW WWWWWWW WWWWWWW WWWWWWW WWWWWWW";
        }
        break;
        
        case 2:
        {
            diagram_type = DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM;
            diagram_name = u8"100\u00B0Celsius";
            diagram_description = u8"100\u00B5s equals 0.1ms";
        }
        break;
        
        case 3:
        {
            diagram_type = DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM;
            diagram_name = "";
            diagram_description = "";
        }
        break;
        
        case 4:
        {
            diagram_type = DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM;
            diagram_name = "";
            diagram_description = "";
        }
        break;
        
        case 5:
        {
            diagram_type = DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM;
            diagram_name = "";
            diagram_description = "";
        }
        break;
    }
    data_diagram_t *diag = data_visible_set_get_diagram_ptr( io_data_set );
    const data_error_t d_err = data_diagram_init( diag, 
                                                  (*this_).variant, /* diagram_id */
                                                  DATA_ROW_ID_VOID, /* parent_diagram_id */
                                                  diagram_type,
                                                  diagram_name,
                                                  diagram_description,
                                                  (*this_).variant /* list_order */
                                                );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == d_err );
}


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
