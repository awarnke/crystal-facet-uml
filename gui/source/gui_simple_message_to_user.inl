/* File: gui_simple_message_to_user.inl; Copyright and License: see below */

static inline void gui_simple_message_to_user_show_message ( gui_simple_message_to_user_t *this_,
                                                             gui_simple_message_type_t type_id,
                                                             gui_simple_message_content_t content_id )
{
    gui_simple_message_to_user_show_message_with_string ( this_, type_id, content_id, NULL );
}


static inline gui_simple_message_type_t gui_simple_message_to_user_get_type_id( gui_simple_message_to_user_t *this_ )
{
    return (*this_).type_id;
}

static inline gui_simple_message_content_t gui_simple_message_to_user_get_content_id( gui_simple_message_to_user_t *this_ )
{
    return (*this_).content_id;
}


/*
Copyright 2016-2018 Andreas Warnke

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
