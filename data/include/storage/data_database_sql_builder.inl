/* File: data_database_sql_builder.inl; Copyright and License: see below */

static inline char* data_database_sql_builder_get_string_ptr ( data_database_sql_builder_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).sql_stringbuf );
}


/*
Copyright 2016-2023 Andreas Warnke

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
