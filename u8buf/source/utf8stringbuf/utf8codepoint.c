/* File: utf8codepoint.c; Copyright and License: see below */

/*!
 *  \file utf8codepoint.c
 *
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author Copyright 2012-2023 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */

#include "utf8stringbuf/utf8codepoint.h"

/* UTF8CODEPOINT_INVAL_CHAR is a constant, may be stored in a read-only memory page */
const utf8codepoint_t UTF8CODEPOINT_INVAL_CHAR = { UTF8CODEPOINT_INVALID_LEN, 0x0, };


/*
 * Copyright 2012-2023 Andreas Warnke
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
