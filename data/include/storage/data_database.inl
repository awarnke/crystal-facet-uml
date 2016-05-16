/* File: data_database.inl; Copyright and License: see below */

static inline sqlite3 *data_database_get_database_ptr ( data_database_t *this_ )
{
    return (*this_).db;
}

static inline data_change_notifier_t *data_database_get_notifier_ptr ( data_database_t *this_ )
{
    return &((*this_).notifier);
}

static inline const char *data_database_get_filename_ptr ( data_database_t *this_ )
{
    return ( (*this_).is_open ? utf8stringbuf_get_string( (*this_).db_file_name ) : NULL );
}

static void data_database_clear_db_listener_list( data_database_t *this_ )
{
    for( int index = 0; index < GUI_DATABASE_MAX_LISTENERS; index ++ )
    {
        (*this_).listener_list[index] = NULL;
    }
}

static void data_database_notify_db_listeners( data_database_t *this_, data_database_listener_signal_t signal_id )
{
    for( int index = 0; index < GUI_DATABASE_MAX_LISTENERS; index ++ )
    {
        if ( NULL != (*this_).listener_list[index] )
        {
            data_database_listener_notify( (*this_).listener_list[index], signal_id );
        }
    }
}


/*
Copyright 2016-2016 Andreas Warnke

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
