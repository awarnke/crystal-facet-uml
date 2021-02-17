/* File: pencil_layouter_test.c; Copyright and License: see below */

#include "pencil_layouter_test.h"
#include "pencil_layouter.h"
#include "trace.h"
#include "test_assert.h"
        
static void set_up(void);
static void tear_down(void);
static void iterate_mini_models(void);

test_suite_t pencil_layouter_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "pencil_layouter_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "iterate_mini_models", &iterate_mini_models );
    return result;
}

static void set_up(void)
{
}

static void tear_down(void)
{
}
    
static void iterate_mini_models(void)
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
