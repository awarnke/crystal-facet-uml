/* File: io_export_head.h; Copyright and License: see below */

#include "io_element_writer.h"
#include "storage/data_database_reader.h"
#include "entity/data_head_key.h"
#include <assert.h>

static inline void io_export_head_init ( io_export_head_t *this_,
                                         data_database_reader_t *db_reader,
                                         io_element_writer_t *out_element_writer )
{
    assert( NULL != db_reader );
    assert( NULL != out_element_writer );
    (*this_).db_reader = db_reader;
    (*this_).element_writer = out_element_writer;
}

static inline void io_export_head_destroy ( io_export_head_t *this_ )
{
    assert( NULL != (*this_).element_writer );
    assert( NULL != (*this_).db_reader );
    (*this_).element_writer = NULL;
    (*this_).db_reader = NULL;
}

static inline u8_error_t io_export_head_iterate_head_values( io_export_head_t *this_ )
{
    assert( NULL != (*this_).element_writer );
    assert( NULL != (*this_).db_reader );
    u8_error_t result = U8_ERROR_NONE;

    result |= io_element_writer_start_head( (*this_).element_writer );

    /* DATA_HEAD_KEY_MODEL_ID_PREFIX */
    {
        data_head_init_new( &((*this_).temp_head), "", "" );
        const u8_error_t id_prefix_err
            = data_database_reader_get_head_by_key( (*this_).db_reader,
                                                    DATA_HEAD_KEY_MODEL_ID_PREFIX,
                                                    &((*this_).temp_head)
                                                  );
        if ( id_prefix_err == U8_ERROR_NONE )
        {
            result |= io_element_writer_add_head( (*this_).element_writer, &((*this_).temp_head) );
        }
        data_head_destroy( &((*this_).temp_head) );
    }

    /* DATA_HEAD_KEY_MODEL_UUID */
    {
        data_head_init_new( &((*this_).temp_head), "", "" );
        const u8_error_t id_prefix_err
            = data_database_reader_get_head_by_key( (*this_).db_reader,
                                                    DATA_HEAD_KEY_MODEL_ID_PREFIX,
                                                    &((*this_).temp_head)
                                                  );
        if ( id_prefix_err == U8_ERROR_NONE )
        {
            result |= io_element_writer_add_head( (*this_).element_writer, &((*this_).temp_head) );
        }
        data_head_destroy( &((*this_).temp_head) );
    }

    result |= io_element_writer_end_head( (*this_).element_writer );

    return result;
}


/*
Copyright 2026-2026 Andreas Warnke

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
