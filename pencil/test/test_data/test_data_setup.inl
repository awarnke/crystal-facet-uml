/* File: test_data_setup.c; Copyright and License: see below */

#include "trace.h"
#include "test_assert.h"
#include "assert.h"
        
#define TEST_DATA_SETUP_DIAG_VARIANTS (6)
#define TEST_DATA_SETUP_CLASS_VARIANTS (6)
#define TEST_DATA_SETUP_FEAT_VARIANTS (6)
#define TEST_DATA_SETUP_REL_VARIANTS (6)
#define TEST_DATA_SETUP_VARIANTS (\
    TEST_DATA_SETUP_DIAG_VARIANTS * TEST_DATA_SETUP_CLASS_VARIANTS * TEST_DATA_SETUP_FEAT_VARIANTS  * TEST_DATA_SETUP_REL_VARIANTS )

static inline void test_data_setup_init( test_data_setup_t *this_, test_data_setup_mode_t mode )
{
    (*this_).variant = 0;
    (*this_).mode = mode;
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

static inline bool test_data_setup_is_valid_variant( const test_data_setup_t *this_ )
{
    return ( (*this_).variant < TEST_DATA_SETUP_VARIANTS );
}

static inline void test_data_setup_get_variant_data( const test_data_setup_t *this_, data_visible_set_t *io_data_set )
{
    data_visible_set_reinit( io_data_set );
    test_data_setup_private_set_diagram( this_, io_data_set );
    test_data_setup_private_add_classifiers( this_, io_data_set );
    test_data_setup_private_add_features( this_, io_data_set );
    test_data_setup_private_add_relationships( this_, io_data_set );
}

static const char *const EMPTY_STR = "";
static const char *const NARROW_STR = "i";
static const char *const STANDARD_STR = "Hello World";
static const char *const LONG_NAME = u8"Input-Devices::360" u8"\u00B0" u8"SurroundView::Settings";
static const char *const WIDE_NAME = "WWWWWWWWWW" "WWWWWWWWWW" "WWWWWWWWWW" "WWWWWWWWWW" "WWWWWWW";
#define TEST_DATA_SETUP_NORMAL_TEXT "This software component shall implement the following interfaces:\\n"
static const char *const NORMAL_DESCRIPTION = TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT;
static const char *const LONG_DESCRIPTION
    = TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT
      TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT
      TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT
      TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT
      TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT
      TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT
      TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT
      TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT
      TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT
      TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT
      TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT
      TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT
      TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT
      TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT TEST_DATA_SETUP_NORMAL_TEXT;

static inline void test_data_setup_private_set_diagram( const test_data_setup_t *this_, data_visible_set_t *io_data_set )
{
    /*data_diagram_type_t const mod_type = DATA_DIAGRAM_TYPE_ARRAY [ (*this_).variant %  DATA_DIAGRAM_TYPE_COUNT ];*/

    data_diagram_type_t diagram_type;
    switch ( (*this_).variant % TEST_DATA_SETUP_DIAG_VARIANTS )
    {
        default:
        case 0:
        {
            diagram_type = DATA_DIAGRAM_TYPE_LIST;
        }
        break;
        
        case 1:
        {
            diagram_type = DATA_DIAGRAM_TYPE_BOX_DIAGRAM;
        }
        break;
        
        case 2:
        {
            diagram_type = DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM;
        }
        break;
        
        case 3:
        {
            diagram_type = DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM;
        }
        break;
        
        case 4:
        {
            diagram_type = DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM;
        }
        break;
        
        case 5:
        {
            diagram_type = DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM;
        }
        break;
    }
    
    const char* diagram_name;
    const char* diagram_description;
    switch ( (*this_).mode )
    {
        default:
        case TEST_DATA_SETUP_MODE_GOOD_CASES:
        {
            diagram_name = (((*this_).variant % 2)==0) ? NARROW_STR : STANDARD_STR;
            diagram_description = NORMAL_DESCRIPTION;
        }
        break;

        case TEST_DATA_SETUP_MODE_CHALLENGING_CASES:
        {
            diagram_name = (((*this_).variant % 2)==0) ? EMPTY_STR : LONG_NAME;
            diagram_description = NORMAL_DESCRIPTION;
        }
        break;

        case TEST_DATA_SETUP_MODE_EDGE_CASES:
        {
            diagram_name = WIDE_NAME;
            diagram_description = LONG_DESCRIPTION;
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



static inline void test_data_setup_private_add_classifiers( const test_data_setup_t *this_, data_visible_set_t *io_data_set )
{
    const uint32_t count = 13;
    for ( uint32_t index = 0; index < count; index ++ )
    {
        data_visible_classifier_t vis_classfy;
        data_visible_classifier_init_empty ( &vis_classfy );
        data_classifier_t *classifier = data_visible_classifier_get_classifier_ptr ( &vis_classfy );
        data_diagramelement_t *diagramelement = data_visible_classifier_get_diagramelement_ptr ( &vis_classfy );
        
        const data_classifier_type_t class_type = DATA_CLASSIFIER_TYPE_ARRAY [ (*this_).variant % DATA_CLASSIFIER_TYPE_COUNT ];

        const char* stereotype = "";
        const char* name = "";
        const char* description = "";
        int32_t x_order = 0;
        int32_t y_order = 0;
        int32_t list_order = 0;
        
        const data_error_t d1_err = data_classifier_init( classifier,
                                                          index+1,  /* = id */
                                                          class_type,
                                                          stereotype,
                                                          name,
                                                          description,
                                                          x_order,
                                                          y_order,
                                                          list_order
                                                        );       
        TEST_ENVIRONMENT_ASSERT ( d1_err == DATA_ERROR_NONE );
    
        //    DATA_DIAGRAMELEMENT_FLAG_NONE = 0x0,  /*!< no flags set */
        //DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE = 0x01,  /*!< the classifier shall be drawn as a named instance (underline) */
        //DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE = 0x02,  /*!< the classifier shall be drawn as anonymous instance (colon-prefix and underline) */
        ////DATA_DIAGRAMELEMENT_FLAG_EMPHASIS = 0x1000000,  /*!< the classifier shall be marked by a color or other appropriate means */
        //DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT = 0x2000000,  /*!< the classifier shall be greyed out */

        
        data_row_id_t diagram_id = data_diagram_get_row_id( data_visible_set_get_diagram_const( io_data_set ) );
        
        data_diagramelement_init( diagramelement,
                                  index+1,  /* = id */
                                  diagram_id,
                                  index+1,  /* = classifier_id */
                                  DATA_DIAGRAMELEMENT_FLAG_NONE,
                                  DATA_ROW_ID_VOID  /* = focused_feature_id */
                                );
        
        const data_error_t d2_err = data_visible_set_append_classifier( io_data_set, &vis_classfy );
        TEST_ENVIRONMENT_ASSERT ( d2_err == DATA_ERROR_NONE );

        data_visible_classifier_destroy ( &vis_classfy );
    }
}

static inline void test_data_setup_private_add_features( const test_data_setup_t *this_, data_visible_set_t *io_data_set )
{
    const uint32_t count = 13;
    for ( uint32_t index = 0; index < count; index ++ )
    {
        const data_feature_type_t feat_type = DATA_FEATURE_TYPE_ARRAY [ (*this_).variant % DATA_FEATURE_TYPE_COUNT ];

        const char* feature_key = "";
        const char* feature_value = "";
        const char* feature_description = "";
        int32_t list_order = 0;
       
        data_feature_t feat;
        const data_error_t d1_err = data_feature_init( &feat,
                                                       index+1,  /* = feature_id */
                                                       feat_type,
                                                       index+1,  /* = classifier_id */
                                                       feature_key,
                                                       feature_value,
                                                       feature_description,
                                                       list_order
                                                     );
        TEST_ENVIRONMENT_ASSERT ( d1_err == DATA_ERROR_NONE );
        
        const data_error_t d2_err = data_visible_set_append_feature( io_data_set, &feat );
        TEST_ENVIRONMENT_ASSERT ( d2_err == DATA_ERROR_NONE );
        
        data_feature_destroy ( &feat );
    }
}

static inline void test_data_setup_private_add_relationships( const test_data_setup_t *this_, data_visible_set_t *io_data_set )
{
    const uint32_t count = 13;
    for ( uint32_t index = 0; index < count; index ++ )
    {
        const data_relationship_type_t rel_type = DATA_RELATIONSHIP_TYPE_ARRAY [ (*this_).variant %  DATA_RELATIONSHIP_TYPE_COUNT ]; 

        const char* relationship_name = "";
        const char* relationship_description = "";
        int32_t list_order = 0;

        data_relationship_t rel;
        const data_error_t d1_err = data_relationship_init( &rel,
                                                            index+1,  /* =  relationship_id */
                                                            rel_type,
                                                            index+1,  /* = from_classifier_id */
                                                            index+1,  /* = to_classifier_id */
                                                            relationship_name,
                                                            relationship_description,
                                                            list_order,
                                                            index+1,  /* = from_feature_id */
                                                            index+1  /* = to_feature_id */
                                                          );
        TEST_ENVIRONMENT_ASSERT ( d1_err == DATA_ERROR_NONE );
        
        const data_error_t d2_err = data_visible_set_append_relationship( io_data_set, &rel );
        TEST_ENVIRONMENT_ASSERT ( d2_err == DATA_ERROR_NONE );
        
        data_relationship_destroy ( &rel );
    }
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
