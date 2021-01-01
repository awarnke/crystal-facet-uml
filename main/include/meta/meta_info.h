/* File: meta_info.h; Copyright and License: see below */

#ifndef META_INFO_H
#define META_INFO_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the own program name
 */

/* human readable version */
#define META_INFO_PROGRAM_NAME_STR "crystal facet uml"
/* machine readable version */
#define META_INFO_PROGRAM_ID_STR "crystal-facet-uml"
#define META_INFO_COPYRIGHT_STR /*Copyright*/ "2016-2021 Andreas Warnke"
#ifdef __linux__
#define META_INFO_LICENSE_STR "Apache-2.0"  /* sqlite3 linked dynamicyally */
#else  /* __linux__ */
#define META_INFO_LICENSE_STR "Apache-2.0 (includes sqlite3 which is public domain)"  /* sqlite3 linked statically */
#endif  /* __linux__ */

#endif  /* META_INFO_H */


/*
Copyright 2016-2021 Andreas Warnke

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
