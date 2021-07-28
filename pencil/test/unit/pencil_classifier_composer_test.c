/* File: pencil_classifier_composer_test.c; Copyright and License: see below */

#include "pencil_classifier_composer_test.h"
#include "pencil_classifier_composer.h"
#include "set/data_visible_set.h"
#include "test_assert.h"

static void set_up(void);
static void tear_down(void);
static void test_empty_model(void);
static void test_normal_model(void);
static void test_too_big_model(void);
static void test_inconsistent_model(void);

test_suite_t pencil_classifier_composer_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "pencil_classifier_composer_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_empty_model", &test_empty_model );
    test_suite_add_test_case( &result, "test_normal_model", &test_normal_model );
    test_suite_add_test_case( &result, "test_too_big_model", &test_too_big_model );
    test_suite_add_test_case( &result, "test_inconsistent_model", &test_inconsistent_model );
    return result;
}

PangoContext* pango_ctx;
PangoLayout* pango_layout;

static void set_up(void)
{
    pango_ctx = pango_context_new();
    pango_layout = pango_layout_new( pango_ctx );
}

static void tear_down(void)
{
    g_object_unref( pango_layout );
    g_object_unref( pango_ctx );
}

static void test_empty_model(void)
{
    TEST_ASSERT(false);
}

static void test_normal_model(void)
{
}

static void test_too_big_model(void)
{
}

static void test_inconsistent_model(void)
{
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
