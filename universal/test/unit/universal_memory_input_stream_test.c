/* File: universal_memory_input_stream_test.c; Copyright and License: see below */

#include "universal_memory_input_stream_test.h"
#include "stream/universal_memory_input_stream.h"
#include "test_assert.h"
#include <string.h>
#include <assert.h>

static void set_up(void);
static void tear_down(void);
static void test_read(void);

static char my_out_buffer[10];
static universal_memory_input_stream_t my_mem_out_stream;

test_suite_t universal_memory_input_stream_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "universal_memory_input_stream_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_read", &test_read );
    return result;
}

static void set_up(void)
{
    memset( &my_out_buffer, '\0', sizeof(my_out_buffer) );
    universal_memory_input_stream_init( &my_mem_out_stream, &my_out_buffer, sizeof(my_out_buffer) );
}

static void tear_down(void)
{
    int err;
    err = universal_memory_input_stream_destroy( &my_mem_out_stream );
    TEST_ENVIRONMENT_ASSERT( err == 0 );
}

static void test_read(void)
{
    int err;

    /* get universal_input_stream_t */
    universal_input_stream_t *my_out_stream;
    my_out_stream = universal_memory_input_stream_get_input_stream( &my_mem_out_stream );
    TEST_ASSERT( my_out_stream != NULL );

    /* get universal_input_stream_if_t */
    const universal_input_stream_if_t *my_out_if = universal_input_stream_get_interface ( my_out_stream );
    TEST_ASSERT( my_out_if != NULL );

    /* get objectdata */
    void *my_obj_data = universal_input_stream_get_objectdata ( my_out_stream );
    TEST_ASSERT_EQUAL_PTR( &my_mem_out_stream, my_obj_data );

    /* read */
    size_t len;
    char buf[16];
    err = universal_input_stream_read ( my_out_stream, &buf, sizeof(buf), &len );
    TEST_ASSERT_EQUAL_INT( 0, err );
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
