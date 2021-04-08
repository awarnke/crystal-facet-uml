/* File: utf8stringview.inl; Copyright and License: see below */

/*!
 *  \file utf8stringview.inl
 *
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author Copyright 2021-2021 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */

#ifdef __cplusplus
extern "C" {
#endif

static inline const char* utf8stringview_get_start( utf8stringview_t this_ ) {
    return this_.start;
}

static inline size_t utf8stringview_get_length( utf8stringview_t this_ ) {
    return this_.length;
}

#ifdef __cplusplus
}
#endif


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
