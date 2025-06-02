/* File: json_type_name_map.inl; Copyright and License: see below */

#include "xmi/xmi_element_info_map.h"
#include "xmi/xmi_element_info.h"
#include "xmi/xmi_diagram_info_map.h"
#include "xmi/xmi_diagram_info.h"
#include <stdint.h>
#include <assert.h>

static inline void json_type_name_map_init( json_type_name_map_t *this_ )
{
    for ( unsigned int c_idx = 0; c_idx < DATA_CLASSIFIER_TYPE_COUNT; c_idx ++ )
    {
        (*this_).classifier_types[c_idx]
            = json_type_name_map_get_classifier_type( this_,
                                                      DATA_CLASSIFIER_TYPE_VOID,
                                                      DATA_CLASSIFIER_TYPE_ARRAY[c_idx]
                                                    );
    }
    (*this_).classifier_types[ DATA_CLASSIFIER_TYPE_COUNT ] = NULL;

    for ( unsigned int f_idx = 0; f_idx < DATA_FEATURE_TYPE_COUNT; f_idx ++ )
    {
        (*this_).feature_types[f_idx]
            = json_type_name_map_get_feature_type( this_,
                                                   DATA_FEATURE_TYPE_VOID,
                                                   DATA_FEATURE_TYPE_ARRAY[f_idx]
                                                 );
    }
    (*this_).feature_types[ DATA_FEATURE_TYPE_COUNT ] = NULL;

    for ( unsigned int r_idx = 0; r_idx < DATA_RELATIONSHIP_TYPE_COUNT; r_idx ++ )
    {
        (*this_).relationship_types[r_idx]
            = json_type_name_map_get_relationship_type( this_,
                                                        false,
                                                        DATA_RELATIONSHIP_TYPE_ARRAY[r_idx]
                                                      );
    }
    (*this_).relationship_types[ DATA_RELATIONSHIP_TYPE_COUNT ] = NULL;

    for ( unsigned int d_idx = 0; d_idx < DATA_DIAGRAM_TYPE_COUNT; d_idx ++ )
    {
        (*this_).diagram_types[d_idx]
            = json_type_name_map_get_diagram_type( this_,
                                                   DATA_DIAGRAM_TYPE_ARRAY[d_idx]
                                                 );
    }
    (*this_).diagram_types[ DATA_DIAGRAM_TYPE_COUNT ] = NULL;

    enum { JSON_TYPE_NAME_MAP_DF_CNT = 3 };
    data_diagram_flag_t diag_flags[ JSON_TYPE_NAME_MAP_DF_CNT ] = {
        DATA_DIAGRAM_FLAG_NONE,
        DATA_DIAGRAM_FLAG_EMPHASIS,
        DATA_DIAGRAM_FLAG_GRAY_OUT
    };
    for ( unsigned int df_idx = 0; df_idx < JSON_TYPE_NAME_MAP_DF_CNT; df_idx ++ )
    {
        (*this_).diagram_tags[df_idx]
            = json_type_name_map_get_diagram_tags( this_, diag_flags[df_idx] );
    }
    (*this_).diagram_tags[ JSON_TYPE_NAME_MAP_DF_CNT ] = NULL;

    enum { JSON_TYPE_NAME_MAP_DEF_CNT = 9 };
    data_diagramelement_flag_t diagele_flags[ JSON_TYPE_NAME_MAP_DEF_CNT ] = {
        DATA_DIAGRAMELEMENT_FLAG_NONE,
        DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
        DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT,
        DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE,
        DATA_DIAGRAMELEMENT_FLAG_EMPHASIS | DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE,
        DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT | DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE,
        DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE,
        DATA_DIAGRAMELEMENT_FLAG_EMPHASIS | DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE,
        DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT | DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE
    };
    for ( unsigned int def_idx = 0; def_idx < JSON_TYPE_NAME_MAP_DEF_CNT; def_idx ++ )
    {
        (*this_).diagramelement_tags[def_idx]
            = json_type_name_map_get_diagramelement_tags( this_, diagele_flags[def_idx] );
    }
    (*this_).diagramelement_tags[ JSON_TYPE_NAME_MAP_DEF_CNT ] = NULL;
}

static inline void json_type_name_map_destroy( json_type_name_map_t *this_ )
{
}

static inline const char * json_type_name_map_get_classifier_type( const json_type_name_map_t *this_,
                                                                   data_classifier_type_t parent_type,
                                                                   data_classifier_type_t classifier_type )
{
    const xmi_element_info_t *element_info;
    const u8_error_t err
        = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                               parent_type,
                                               classifier_type,
                                               &element_info
                                             );
    const char *const type_name = ( err != U8_ERROR_NONE ) ? "" : xmi_element_info_get_name( element_info );
    return type_name;
}

static inline const char *const * json_type_name_map_get_classifier_types_list ( const json_type_name_map_t *this_ )
{
    return (const char *const *) &((*this_).classifier_types);
}

static inline const char * json_type_name_map_get_feature_type( const json_type_name_map_t *this_,
                                                                data_classifier_type_t parent_type,
                                                                data_feature_type_t feature_type )
{
    const xmi_element_info_t *element_info;
    const u8_error_t err
        = xmi_element_info_map_get_feature( &xmi_element_info_map_standard,
                                            parent_type,
                                            feature_type,
                                            &element_info
                                          );
    const char *const type_name = ( err != U8_ERROR_NONE ) ? "" : xmi_element_info_get_name( element_info );
    return type_name;
}

static inline const char *const * json_type_name_map_get_feature_types_list ( const json_type_name_map_t *this_ )
{
    return (const char *const *) &((*this_).feature_types);
}

static inline const char * json_type_name_map_get_relationship_type( const json_type_name_map_t *this_,
                                                                     bool statemachine_context,
                                                                     data_relationship_type_t rel_type )
{
    const xmi_element_info_t *element_info;
    const u8_error_t err
        = xmi_element_info_map_get_relationship( &xmi_element_info_map_standard,
                                                 statemachine_context,
                                                 rel_type,
                                                 &element_info
                                               );
    const char *const type_name = ( err != U8_ERROR_NONE ) ? "" : xmi_element_info_get_name( element_info );
    return type_name;
}

static inline const char *const * json_type_name_map_get_relationship_types_list ( const json_type_name_map_t *this_ )
{
    return (const char *const *) &((*this_).relationship_types);
}

static inline const char * json_type_name_map_get_diagram_type( const json_type_name_map_t *this_,
                                                                data_diagram_type_t diagram_type )
{
    const xmi_diagram_info_t *diagram_info;
    const u8_error_t err
        = xmi_diagram_info_map_get_diagram( &xmi_diagram_info_map_standard,
                                            diagram_type,
                                            &diagram_info
                                          );
    const char *const type_name = ( err != U8_ERROR_NONE ) ? "" : xmi_diagram_info_get_name( diagram_info );
    return type_name;
}

static inline const char *const * json_type_name_map_get_diagram_types_list ( const json_type_name_map_t *this_ )
{
    return (const char *const *) &((*this_).diagram_types);
}

static inline const char * json_type_name_map_get_diagram_tags( const json_type_name_map_t *this_,
                                                                data_diagram_flag_t diagram_flag )
{
    const char * tag_names = "";

    switch( diagram_flag )
    {
        case DATA_DIAGRAM_FLAG_EMPHASIS:
        {
            tag_names = "Emphasis";
        }
        break;

        case DATA_DIAGRAM_FLAG_GRAY_OUT:
        {
            tag_names = "GrayOut";
        }
        break;

        default:
        {
            tag_names = "";
        }
        break;
    }

    return tag_names;
}

static inline const char *const * json_type_name_map_get_diagram_tags_list ( const json_type_name_map_t *this_ )
{
    return (const char *const *) &((*this_).diagram_tags);
}

static inline const char * json_type_name_map_get_diagramelement_tags( const json_type_name_map_t *this_,
                                                                       data_diagramelement_flag_t diagramelement_flag )
{
    const char * tag_names = "";

    switch( (int_fast32_t) diagramelement_flag )
    {
        case DATA_DIAGRAMELEMENT_FLAG_EMPHASIS:
        {
            tag_names = "Emphasis";
        }
        break;

        case DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT:
        {
            tag_names = "GrayOut";
        }
        break;

        case DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE:
        {
            tag_names = "NamedInstance";
        }
        break;

        case DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_EMPHASIS:
        {
            tag_names = "NamedInstance, Emphasis";
        }
        break;

        case DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT:
        {
            tag_names = "NamedInstance, GrayOut";
        }
        break;

        case DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE:
        {
            tag_names = "AnonymousInstance";
        }
        break;

        case DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_EMPHASIS:
        {
            tag_names = "AnonymousInstance, Emphasis";
        }
        break;

        case DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT:
        {
            tag_names = "AnonymousInstance, GrayOut";
        }
        break;

        default:
        {
            tag_names = "";
        }
        break;
    }

    return tag_names;
}

static inline const char *const * json_type_name_map_get_diagramelement_tags_list ( const json_type_name_map_t *this_ )
{
    return (const char *const *) &((*this_).diagramelement_tags);
}


/*
Copyright 2022-2025 Andreas Warnke

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
