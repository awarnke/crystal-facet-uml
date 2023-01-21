/* File: data_change_notifier.inl; Copyright and License: see below */

#include "data_id.h"
#include "u8/u8_log.h"
#include <assert.h>

static inline void data_change_notifier_emit_signal_without_parent ( data_change_notifier_t *this_,
                                                                     data_change_event_type_t event_type,
                                                                     data_table_t table,
                                                                     data_row_id_t row_id )
{
    /* only classifier may be created without providing a parent object */
    assert( (DATA_CHANGE_EVENT_TYPE_CREATE != event_type) || (DATA_TABLE_CLASSIFIER == table) );

    data_change_notifier_emit_signal ( this_,
                                       event_type,
                                       table,
                                       row_id,
                                       DATA_TABLE_VOID,
                                       DATA_ROW_ID_VOID
                                     );
}

static inline void data_change_notifier_enable_stealth_mode ( data_change_notifier_t *this_ )
{
    (*this_).stealth_mode = true;
}

static inline void data_change_notifier_disable_stealth_mode ( data_change_notifier_t *this_ )
{
    const bool send_notification = (*this_).stealth_mode;
    (*this_).stealth_mode = false;

    if ( send_notification )
    {
        /* send notification that things have changed */
        data_change_notifier_emit_signal( this_,
                                          DATA_CHANGE_EVENT_TYPE_MULTI,
                                          DATA_TABLE_VOID,
                                          DATA_ROW_ID_VOID,
                                          DATA_TABLE_VOID,
                                          DATA_ROW_ID_VOID
                                        );
    }
}


/*
Copyright 2018-2023 Andreas Warnke

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
