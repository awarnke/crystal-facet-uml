/* File: data_classifier.inl; Copyright and License: see below */

static inline void data_classifier_init_new ( data_classifier_t *this_, data_classifier_type_t classifier_type, const char* classifier_name, const char* classifier_description )
{
    utf8error_t strerr;

    (*this_).id = DATA_ID_CONST_VOID_ID;
    (*this_).main_type = classifier_type;

    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    utf8stringbuf_clear( (*this_).stereotype );

    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).name, classifier_name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
    }

    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).description, classifier_description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
    }

    (*this_).x_order = 0;
    (*this_).y_order = 0;
}

static inline void data_classifier_init_empty ( data_classifier_t *this_ )
{
    (*this_).id = DATA_ID_CONST_VOID_ID;
    (*this_).main_type = DATA_CLASSIFIER_TYPE_BLOCK;

    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    utf8stringbuf_clear( (*this_).stereotype );
    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    utf8stringbuf_clear( (*this_).name );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    utf8stringbuf_clear( (*this_).description );

    (*this_).x_order = 0;
    (*this_).y_order = 0;
}

static inline void data_classifier_destroy ( data_classifier_t *this_ )
{
    (*this_).id = DATA_ID_CONST_VOID_ID;
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
