/* File: xmi_xml.h; Copyright and License: see below */

#ifndef XMI_XML_H
#define XMI_XML_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the xml namespaces, xml elements, xml attributes for xmi export
 */

/*!
 *  \brief namespace for uml elements and attributes
 */
extern const char XMI_XML_NS_UML[5];

/*!
 *  \brief namespace for sysml profile elements and attributes
 */
extern const char XMI_XML_NS_SYSML[7];

/*!
 *  \brief namespace for mofext profile elements and attributes
 */
extern const char XMI_XML_NS_MOFEXT[8];

/*!
 *  \brief namespace for standard profile elements and attributes
 */
extern const char XMI_XML_NS_STDPROF[17];

/*!
 *  \brief namespace for the model-local profile elements and attributes
 */
extern const char XMI_XML_NS_LOCALPROF[14];

/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.2 : 2e */
extern const char XMI_XML_ATTR_ID_START[9];
extern const char XMI_XML_ATTR_ID_END[3];

/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.2 : 2g */
extern const char XMI_XML_ATTR_TYPE_START[11];
extern const char XMI_XML_ATTR_TYPE_END[3];

/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.2 : 2l */
extern const char XMI_XML_ATTR_IDREF_START[12];
extern const char XMI_XML_ATTR_IDREF_END[3];

/* spec-ref: not-found */
extern const char XMI_XML_ATTR_NAME_START[7];
extern const char XMI_XML_ATTR_NAME_END[3];

/* spec-ref: not-found */
extern const char XMI_XML_ATTR_VALUE_START[8];
extern const char XMI_XML_ATTR_VALUE_END[3];

#endif  /* XMI_XML_H */


/*
Copyright 2020-2024 Andreas Warnke

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
