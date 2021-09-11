/* File: test_data_setup.inl; Copyright and License: see below */

#include "trace.h"
#include "test_assert.h"
#include <assert.h>
#include <stdint.h>

/* number of same relationships in variant/variant-group size (the division-denominator): TEST_DATA_SETUP_REL_SAME_GROUP */
#define TEST_DATA_SETUP_REL_SAME_GROUP (1)
/* number of relationship variants (the modulo-result): TEST_DATA_SETUP_REL_VARIANTS */
#define TEST_DATA_SETUP_REL_VARIANTS (6)
/* number of same feature in variant/variant-group size (the division-denominator): TEST_DATA_SETUP_FEAT_SAME_GROUP */
#define TEST_DATA_SETUP_FEAT_SAME_GROUP ( TEST_DATA_SETUP_REL_VARIANTS )
/* number of feature variants (the modulo-result): TEST_DATA_SETUP_FEAT_VARIANTS */
#define TEST_DATA_SETUP_FEAT_VARIANTS (3)
/* number of same classifier in variant/variant-group size (the division-denominator): TEST_DATA_SETUP_CLASS_SAME_GROUP */
#define TEST_DATA_SETUP_CLASS_SAME_GROUP ( TEST_DATA_SETUP_FEAT_SAME_GROUP * TEST_DATA_SETUP_FEAT_VARIANTS )
/* number of classifier variants (the modulo-result): TEST_DATA_SETUP_CLASS_VARIANTS */
#define TEST_DATA_SETUP_CLASS_VARIANTS (4)
/* number of same diagram in variant/variant-group size (the division-denominator): TEST_DATA_SETUP_DIAG_SAME_GROUP */
#define TEST_DATA_SETUP_DIAG_SAME_GROUP ( TEST_DATA_SETUP_CLASS_SAME_GROUP * TEST_DATA_SETUP_CLASS_VARIANTS )
/* number of diagram variants (the modulo-result): TEST_DATA_SETUP_DIAG_VARIANTS */
#define TEST_DATA_SETUP_DIAG_VARIANTS (6)
/* number of total variants */
#define TEST_DATA_SETUP_VARIANTS ( TEST_DATA_SETUP_DIAG_SAME_GROUP * TEST_DATA_SETUP_DIAG_VARIANTS )

/* spread range of a pseude random byte to full integer range */
#define SPREAD_RANGE(x) (memset(&x,(char)(x),sizeof(x)))

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

static inline unsigned int test_data_setup_get_variant( const test_data_setup_t *this_ )
{
    return (*this_).variant;
}

static inline test_data_setup_mode_t test_data_setup_get_mode( const test_data_setup_t *this_ )
{
    return (*this_).mode;
}

static inline void test_data_setup_get_variant_data( const test_data_setup_t *this_, data_visible_set_t *io_data_set )
{
    data_visible_set_reinit( io_data_set );
    test_data_setup_private_set_diagram( this_, io_data_set );
    test_data_setup_private_add_classifiers( this_, io_data_set );
    const data_diagram_type_t diag_type = data_diagram_get_diagram_type( data_visible_set_get_diagram_const( io_data_set ) );
    if ( data_diagram_type_is_interaction( diag_type ) )
    {
        test_data_setup_private_add_lifelines( this_, io_data_set );
    }
    test_data_setup_private_add_features( this_, io_data_set );
    test_data_setup_private_add_relationships( this_, io_data_set );
}

static const char *const EMPTY_STR = "";
static const char *const NARROW_STR = "i";
static const char *const STANDARD_STR = "Hello World";
static const char *const LONG_NAME = u8"Input-Devices::360" u8"\u00B0" u8"SurroundView::Settings";
static const char *const WIDE_NAME = "WWWWWWWWWW" "WWWWWWWWWW" "WWWWWWWWWW" "WWWWWWWWWW" "WWWWWWW";
#define TEST_DATA_SETUP_NORMAL_TEXT "This software component shall implement the following interfaces:\n"
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

    data_diagram_type_t diagram_type;

    const uint_fast32_t pseudo_random = (*this_).variant;  /* = this shall be variable/dynamic but not really random */
    const uint_fast32_t diag_variant = ( (*this_).variant / TEST_DATA_SETUP_DIAG_SAME_GROUP ) % TEST_DATA_SETUP_DIAG_VARIANTS;
    switch ( diag_variant )
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
            /* select any normal/std diagram type */
            const data_diagram_type_t proposal_type = DATA_DIAGRAM_TYPE_ARRAY [ pseudo_random %  DATA_DIAGRAM_TYPE_COUNT ];
            const bool proposal_is_not_std
                = data_diagram_type_is_interaction( proposal_type )
                || ( proposal_type == DATA_DIAGRAM_TYPE_LIST )
                || ( proposal_type == DATA_DIAGRAM_TYPE_BOX_DIAGRAM );
            diagram_type = (proposal_is_not_std) ? DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM : proposal_type;
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
            /* select a remaining interaction diagram type */
            diagram_type = ((pseudo_random % 2)==0) ? DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM : DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM;
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
            diagram_name = ((pseudo_random % 2)==0) ? NARROW_STR : STANDARD_STR;
            diagram_description = NORMAL_DESCRIPTION;
        }
        break;

        case TEST_DATA_SETUP_MODE_CHALLENGING_CASES:
        {
            diagram_name = ((pseudo_random % 2)==0) ? EMPTY_STR : LONG_NAME;
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

    data_uuid_t diag_uuid;
    data_uuid_init_new( &diag_uuid );

    data_diagram_t *diag = data_visible_set_get_diagram_ptr( io_data_set );
    const data_error_t d_err = data_diagram_init( diag,
                                                  (*this_).variant, /* diagram_id */
                                                  DATA_ROW_ID_VOID, /* parent_diagram_id */
                                                  diagram_type,
                                                  diagram_name,
                                                  diagram_description,
                                                  (*this_).variant, /* list_order */
                                                  data_uuid_get_string( &diag_uuid )
                                                );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == d_err );

    data_uuid_destroy( &diag_uuid );
}

static inline void test_data_setup_private_add_classifiers( const test_data_setup_t *this_, data_visible_set_t *io_data_set )
{
    const uint_fast32_t class_variant = ( (*this_).variant / TEST_DATA_SETUP_CLASS_SAME_GROUP ) % TEST_DATA_SETUP_CLASS_VARIANTS;
    uint_fast32_t count = 0;
    switch ( (*this_).mode )
    {
        default:
        case TEST_DATA_SETUP_MODE_GOOD_CASES:
        {
            count = class_variant;
        }
        break;

        case TEST_DATA_SETUP_MODE_CHALLENGING_CASES:
        {
            count = (2*TEST_DATA_SETUP_CLASS_VARIANTS) + (4*class_variant);
        }
        break;

        case TEST_DATA_SETUP_MODE_EDGE_CASES:
        {
            count = (class_variant==0)
            ? (DATA_VISIBLE_SET_MAX_CLASSIFIERS)     /* variants wit MAX classifiers */
            : (DATA_VISIBLE_SET_MAX_CLASSIFIERS/4);  /* variants wit MAX/4 classifiers */
        }
        break;
    }
    for ( uint_fast32_t index = 0; index < count; index ++ )
    {
        data_visible_classifier_t vis_classfy;
        data_visible_classifier_init_empty ( &vis_classfy );
        data_classifier_t *classifier = data_visible_classifier_get_classifier_ptr ( &vis_classfy );
        data_diagramelement_t *diagramelement = data_visible_classifier_get_diagramelement_ptr ( &vis_classfy );

        const uint_fast32_t pseudo_random_1 = ((*this_).variant + index)*23;  /* = this shall be variable/dynamic but not really random */
        const uint_fast32_t pseudo_random_2 = ((*this_).variant + index + 8)*29;  /* = this shall be variable/dynamic but not really random */
        const data_classifier_type_t class_type = DATA_CLASSIFIER_TYPE_ARRAY [ pseudo_random_1 % DATA_CLASSIFIER_TYPE_COUNT ];

        const char* stereotype = "";
        const char* name = "";
        const char* description = "";
        int32_t x_order;
        int32_t y_order;
        int32_t list_order;

        switch ( class_variant )
        {
            default:
            case 0:
            {
                /* all different */
                x_order = pseudo_random_1;
                y_order = pseudo_random_2;
                list_order = pseudo_random_1;
            }
            break;

            case 1:
            {
                /* all zero */
                x_order = 0;
                y_order = 0;
                list_order = 0;
            }
            break;

            case 2:
            {
                /* all 5x5 grid */
                x_order = ((pseudo_random_1)%5)*1000;
                y_order = ((pseudo_random_2)%5)*1000;
                list_order = ((pseudo_random_1)%5)*1000;
            }
            break;

            case 3:
            {
                /* all diagnonal */
                x_order = pseudo_random_1;
                y_order = pseudo_random_1;
                list_order = pseudo_random_1;
            }
            break;
        }

        switch ( (*this_).mode )
        {
            default:
            case TEST_DATA_SETUP_MODE_GOOD_CASES:
            {
                stereotype = ((pseudo_random_1 % 2)==0) ? NARROW_STR : STANDARD_STR;
                name = ((pseudo_random_2 % 2)==0) ? NARROW_STR : STANDARD_STR;
                description = NORMAL_DESCRIPTION;
            }
            break;

            case TEST_DATA_SETUP_MODE_CHALLENGING_CASES:
            {
                stereotype = ((pseudo_random_1 % 2)==0) ? EMPTY_STR : LONG_NAME;
                name = ((pseudo_random_2 % 2)==0) ? EMPTY_STR : LONG_NAME;
                description = NORMAL_DESCRIPTION;
            }
            break;

            case TEST_DATA_SETUP_MODE_EDGE_CASES:
            {
                stereotype = ((pseudo_random_1 % 2)==0) ? EMPTY_STR : WIDE_NAME;
                name = ((pseudo_random_2 % 2)==0) ? STANDARD_STR : WIDE_NAME;
                description = ((pseudo_random_1 % 2)==0) ? NORMAL_DESCRIPTION : LONG_DESCRIPTION;
            }
            break;
        }

        data_uuid_t clsfr_uuid;
        data_uuid_init_new( &clsfr_uuid );
        data_uuid_t diagele_uuid;
        data_uuid_init_new( &diagele_uuid );

        const data_row_id_t classifier_id = ((pseudo_random_1 % 5)==0) ? (index+2) : (index+1); /* some classifiers exist twice */
        const data_error_t d1_err = data_classifier_init( classifier,
                                                          classifier_id,
                                                          class_type,
                                                          stereotype,
                                                          name,
                                                          description,
                                                          x_order,
                                                          y_order,
                                                          list_order,
                                                          data_uuid_get_string( &clsfr_uuid )
                                                        );
        TEST_ENVIRONMENT_ASSERT_EQUAL_INT ( DATA_ERROR_NONE, d1_err&(~DATA_ERROR_STRING_BUFFER_EXCEEDED) );

        const data_row_id_t diagram_id = data_diagram_get_row_id( data_visible_set_get_diagram_const( io_data_set ) );
        const data_diagramelement_flag_t flag_inst
            = ((pseudo_random_1 % 3)==0)
            ? DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE
            : ((pseudo_random_1 % 3)==1)
            ? DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE
            : DATA_DIAGRAMELEMENT_FLAG_NONE;
        const data_diagramelement_flag_t flag_emph
            = ((pseudo_random_2 % 3)==0)
            ? DATA_DIAGRAMELEMENT_FLAG_EMPHASIS
            : ((pseudo_random_2 % 3)==1)
            ? DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT
            : DATA_DIAGRAMELEMENT_FLAG_NONE;
        const data_error_t d2_err = data_diagramelement_init( diagramelement,
                                                              index+1,  /* = id */
                                                              diagram_id,
                                                              classifier_id,
                                                              flag_inst|flag_emph,
                                                              index+10001,  /* = focused_feature_id */
                                                              data_uuid_get_string( &diagele_uuid )
                                                            );
        TEST_ENVIRONMENT_ASSERT ( d2_err == DATA_ERROR_NONE );

        const data_error_t d3_err = data_visible_set_append_classifier( io_data_set, &vis_classfy );
        TEST_ENVIRONMENT_ASSERT ( d3_err == DATA_ERROR_NONE );

        data_visible_classifier_destroy ( &vis_classfy );
        data_uuid_destroy( &clsfr_uuid );
        data_uuid_destroy( &diagele_uuid );
    }
}

static inline void test_data_setup_private_add_lifelines( const test_data_setup_t *this_, data_visible_set_t *io_data_set )
{
    const uint_fast32_t count = data_visible_set_get_visible_classifier_count( io_data_set );
    for ( uint_fast32_t index = 0; index < count; index ++ )
    {
        const data_visible_classifier_t *const visclas = data_visible_set_get_visible_classifier_const ( io_data_set, index );
        const data_diagramelement_t *const diagele = data_visible_classifier_get_diagramelement_const ( visclas );

        const data_feature_type_t feat_type = DATA_FEATURE_TYPE_LIFELINE;
        const char *const feature_key = "";
        const char *const feature_value = "";
        const char *const feature_description = "";
        const int32_t list_order = 0;

        data_uuid_t feat_uuid;
        data_uuid_init_new( &feat_uuid );

        data_feature_t feat;
        const data_error_t d1_err = data_feature_init( &feat,
                                                       data_diagramelement_get_focused_feature_row_id( diagele ),  /* = feature_id */
                                                       feat_type,
                                                       data_diagramelement_get_classifier_row_id( diagele ),  /* = classifier_id */
                                                       feature_key,
                                                       feature_value,
                                                       feature_description,
                                                       list_order,
                                                       data_uuid_get_string( &feat_uuid )
                                                     );
        TEST_ENVIRONMENT_ASSERT_EQUAL_INT ( DATA_ERROR_NONE, d1_err );

        const data_error_t d2_err = data_visible_set_append_feature( io_data_set, &feat );
        TEST_ENVIRONMENT_ASSERT_EQUAL_INT ( DATA_ERROR_NONE, d2_err );

        data_feature_destroy ( &feat );
        data_uuid_destroy( &feat_uuid );
    }
}

static inline void test_data_setup_private_add_features( const test_data_setup_t *this_, data_visible_set_t *io_data_set )
{
    const uint_fast32_t feat_variant = ( (*this_).variant / TEST_DATA_SETUP_FEAT_SAME_GROUP ) % TEST_DATA_SETUP_FEAT_VARIANTS;
    uint_fast32_t count = 0;
    switch ( (*this_).mode )
    {
        default:
        case TEST_DATA_SETUP_MODE_GOOD_CASES:
        {
            count = feat_variant;
        }
        break;

        case TEST_DATA_SETUP_MODE_CHALLENGING_CASES:
        {
            count = (2*TEST_DATA_SETUP_FEAT_VARIANTS) + (4*feat_variant);
        }
        break;

        case TEST_DATA_SETUP_MODE_EDGE_CASES:
        {
            const uint_fast32_t lifeline_count = data_visible_set_get_feature_count( io_data_set );
            count = (feat_variant==0)
            ? ((DATA_VISIBLE_SET_MAX_FEATURES - lifeline_count))      /* variants wit MAX features */
            : ((DATA_VISIBLE_SET_MAX_FEATURES - lifeline_count)/4);   /* variants wit MAX/4 features */
        }
        break;
    }

    for ( uint_fast32_t index = 0; index < count; index ++ )
    {
        const uint_fast32_t pseudo_random = ((*this_).variant + index)*7;  /* = this shall be variable/dynamic but not really random */
        const data_feature_type_t feat_type = DATA_FEATURE_TYPE_ARRAY [ pseudo_random % DATA_FEATURE_TYPE_COUNT ];

        const char* feature_key = "";
        const char* feature_value = "";
        const char* feature_description = "";
        int32_t list_order;

        switch ( feat_variant )
        {
            default:
            case 0:
            {
                /* all random */
                list_order = pseudo_random;
                SPREAD_RANGE(list_order);
            }
            break;

            case 1:
            {
                /* all extreme */
                list_order = ((pseudo_random % 2)==0) ? INT32_MIN : INT32_MAX;
            }
            break;

            case 2:
            {
                /* all 5 grid */
                list_order = (pseudo_random % 5)*65536;
                SPREAD_RANGE(list_order);
            }
            break;
        }

        switch ( (*this_).mode )
        {
            default:
            case TEST_DATA_SETUP_MODE_GOOD_CASES:
            {
                feature_key = ((pseudo_random % 2)==0) ? NARROW_STR : STANDARD_STR;
                feature_description = NORMAL_DESCRIPTION;
                feature_value = ((pseudo_random % 2)==0) ? NARROW_STR : STANDARD_STR;
            }
            break;

            case TEST_DATA_SETUP_MODE_CHALLENGING_CASES:
            {
                feature_key = ((pseudo_random % 2)==0) ? EMPTY_STR : LONG_NAME;
                feature_description = NORMAL_DESCRIPTION;
                feature_value = ((pseudo_random % 2)==0) ? EMPTY_STR : LONG_NAME;
            }
            break;

            case TEST_DATA_SETUP_MODE_EDGE_CASES:
            {
                feature_key = ((pseudo_random % 2)==0) ? STANDARD_STR : WIDE_NAME;
                feature_description = LONG_DESCRIPTION;
                feature_value = ((pseudo_random % 2)==0) ? EMPTY_STR : WIDE_NAME;
            }
            break;
        }

        data_uuid_t feat_uuid;
        data_uuid_init_new( &feat_uuid );

        data_feature_t feat;
        const data_error_t d1_err = data_feature_init( &feat,
                                                       index+1,  /* = feature_id */
                                                       feat_type,
                                                       index+1,  /* = classifier_id */
                                                       feature_key,
                                                       feature_value,
                                                       feature_description,
                                                       list_order,
                                                       data_uuid_get_string( &feat_uuid )
                                                     );
        TEST_ENVIRONMENT_ASSERT_EQUAL_INT ( DATA_ERROR_NONE, d1_err&(~DATA_ERROR_STRING_BUFFER_EXCEEDED) );

        const data_error_t d2_err = data_visible_set_append_feature( io_data_set, &feat );
        TEST_ENVIRONMENT_ASSERT ( d2_err == DATA_ERROR_NONE );

        data_feature_destroy ( &feat );
        data_uuid_destroy ( &feat_uuid );
    }
}

static inline void test_data_setup_private_add_relationships( const test_data_setup_t *this_, data_visible_set_t *io_data_set )
{
    const data_diagram_type_t diag_type = data_diagram_get_diagram_type( data_visible_set_get_diagram_const( io_data_set ) );
    const uint_fast32_t rel_variant = ( (*this_).variant / TEST_DATA_SETUP_REL_SAME_GROUP ) % TEST_DATA_SETUP_REL_VARIANTS;
    uint_fast32_t count = 0;
    switch ( (*this_).mode )
    {
        default:
        case TEST_DATA_SETUP_MODE_GOOD_CASES:
        {
            count = rel_variant;
        }
        break;

        case TEST_DATA_SETUP_MODE_CHALLENGING_CASES:
        {
            count = (2*TEST_DATA_SETUP_REL_VARIANTS) + (4*rel_variant);
        }
        break;

        case TEST_DATA_SETUP_MODE_EDGE_CASES:
        {
            count = (rel_variant==0)
            ? (DATA_VISIBLE_SET_MAX_RELATIONSHIPS)      /* variants wit MAX relationships */
            : (DATA_VISIBLE_SET_MAX_RELATIONSHIPS/4);   /* variants wit MAX/4 relationships */
        }
        break;
    }

    for ( uint_fast32_t index = 0; index < count; index ++ )
    {
        const uint_fast32_t pseudo_random_1 = ((*this_).variant + index)*17;  /* = this shall be variable/dynamic but not really random */
        const uint_fast32_t pseudo_random_2 = ((*this_).variant + index + 8)*19;  /* = this shall be variable/dynamic but not really random */

        data_relationship_type_t rel_type = DATA_RELATIONSHIP_TYPE_ARRAY [ pseudo_random_1 %  DATA_RELATIONSHIP_TYPE_COUNT ];
        if ( data_diagram_type_is_interaction( diag_type ) )
        {
            /* in interaction diagrams, most relationships shall be DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL */
            if ((pseudo_random_2 % 4) != 0)
            {
                rel_type = DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL;
            }
        }
        else
        {
            /* in non-interaction diagrams, many relationships shall be DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT */
            if ((pseudo_random_2 % 4) == 0)
            {
                rel_type = DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT;
            }
        }

        const char* relationship_name = "";
        const char* relationship_description = "";
        int32_t list_order;
        data_row_id_t from_classifier_row_id = DATA_ROW_ID_VOID;
        data_row_id_t to_classifier_row_id = DATA_ROW_ID_VOID;
        data_row_id_t from_feature_row_id = DATA_ROW_ID_VOID;
        data_row_id_t to_feature_row_id = DATA_ROW_ID_VOID;

        const uint_fast32_t c_count = data_visible_set_get_visible_classifier_count( io_data_set );
        const uint_fast32_t f_count = data_visible_set_get_feature_count( io_data_set );
        if (( c_count != 0 )&&( (pseudo_random_1 % 2) == 0 ))
        {
            const data_visible_classifier_t *const from_vis_clas = data_visible_set_get_visible_classifier_const( io_data_set, pseudo_random_1 % c_count );
            const data_classifier_t *const from_classifier = data_visible_classifier_get_classifier_const( from_vis_clas );
            from_classifier_row_id = data_classifier_get_row_id( from_classifier );
        }
        else if ( f_count != 0 )
        {
            const data_feature_t *const from_feature = data_visible_set_get_feature_const( io_data_set, pseudo_random_1 % f_count );
            from_feature_row_id = data_feature_get_row_id( from_feature );
            from_classifier_row_id = data_feature_get_classifier_row_id( from_feature );
        }
        if (( c_count != 0 )&&( (pseudo_random_2 % 2) == 0 ))
        {
            const data_visible_classifier_t *const to_vis_clas = data_visible_set_get_visible_classifier_const( io_data_set, pseudo_random_2 % c_count );
            const data_classifier_t *const to_classifier = data_visible_classifier_get_classifier_const( to_vis_clas );
            to_classifier_row_id = data_classifier_get_row_id( to_classifier );
        }
        else if ( f_count != 0 )
        {
            const data_feature_t *const to_feature = data_visible_set_get_feature_const( io_data_set, pseudo_random_2 % f_count );
            to_feature_row_id = data_feature_get_row_id( to_feature );
            to_classifier_row_id = data_feature_get_classifier_row_id( to_feature );
        }

        switch ( rel_variant )
        {
            default:
            case 0:
            {
                /* all random */
                list_order = pseudo_random_1;
                SPREAD_RANGE(list_order);
            }
            break;

            case 1:
            {
                /* all extreme */
                list_order = ((pseudo_random_1 % 2)==0) ? INT32_MIN : INT32_MAX;
            }
            break;

            case 2:
            {
                /* all zero */
                list_order = 0;
            }
            break;

            case 3:
            {
                /* all 19 grid */
                list_order = (pseudo_random_1 % 19)*65536;
                SPREAD_RANGE(list_order);
            }
            break;

            case 4:
            {
                /* all 3 grid */
                list_order = (pseudo_random_1 % 3)*65536;
                SPREAD_RANGE(list_order);
            }
            break;

            case 5:
            {
                /* all 5 grid */
                list_order = (pseudo_random_1 % 5)*65536;
                SPREAD_RANGE(list_order);
            }
            break;
        }

        switch ( (*this_).mode )
        {
            default:
            case TEST_DATA_SETUP_MODE_GOOD_CASES:
            {
                relationship_name = ((pseudo_random_2 % 2)==0) ? NARROW_STR : STANDARD_STR;
                relationship_description = NORMAL_DESCRIPTION;
            }
            break;

            case TEST_DATA_SETUP_MODE_CHALLENGING_CASES:
            {
                relationship_name = ((pseudo_random_2 % 2)==0) ? EMPTY_STR : LONG_NAME;
                relationship_description = NORMAL_DESCRIPTION;
            }
            break;

            case TEST_DATA_SETUP_MODE_EDGE_CASES:
            {
                relationship_name = ((pseudo_random_2 % 2)==0) ? STANDARD_STR : WIDE_NAME;
                relationship_description = LONG_DESCRIPTION;
            }
            break;
        }

        data_uuid_t rel_uuid;
        data_uuid_init_new( &rel_uuid );

        data_relationship_t rel;
        const data_error_t d1_err = data_relationship_init( &rel,
                                                            index+1,  /* =  relationship_id */
                                                            rel_type,
                                                            from_classifier_row_id,
                                                            to_classifier_row_id,
                                                            relationship_name,
                                                            relationship_description,
                                                            list_order,
                                                            from_feature_row_id,
                                                            to_feature_row_id,
                                                            data_uuid_get_string( &rel_uuid )
                                                          );
        TEST_ENVIRONMENT_ASSERT_EQUAL_INT ( DATA_ERROR_NONE, d1_err&(~DATA_ERROR_STRING_BUFFER_EXCEEDED) );

        const data_error_t d2_err = data_visible_set_append_relationship( io_data_set, &rel );
        TEST_ENVIRONMENT_ASSERT ( d2_err == DATA_ERROR_NONE );

        data_relationship_destroy ( &rel );
        data_uuid_destroy ( &rel_uuid );
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
