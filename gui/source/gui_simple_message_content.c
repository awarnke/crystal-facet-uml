/* File: gui_simple_message_content.c; Copyright and License: see below */

#include "gui_simple_message_content.h"

static const gui_simple_message_content_names_stat_t GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED_DATA = { .list_of_names=NULL, .stat=NULL };
const gui_simple_message_content_names_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED = &GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED_DATA;

static const gui_simple_message_content_stat_t GUI_SIMPLE_MESSAGE_CONTENT_CUT_TO_CLIPBOARD_DATA = { .stat=NULL };
const gui_simple_message_content_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_CUT_TO_CLIPBOARD = &GUI_SIMPLE_MESSAGE_CONTENT_CUT_TO_CLIPBOARD_DATA;
static const gui_simple_message_content_stat_t GUI_SIMPLE_MESSAGE_CONTENT_COPY_TO_CLIPBOARD_DATA = { .stat=NULL };
const gui_simple_message_content_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_COPY_TO_CLIPBOARD = &GUI_SIMPLE_MESSAGE_CONTENT_COPY_TO_CLIPBOARD_DATA;
static const gui_simple_message_content_stat_t GUI_SIMPLE_MESSAGE_CONTENT_PASTE_FROM_CLIPBOARD_DATA = { .stat=NULL };
const gui_simple_message_content_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_PASTE_FROM_CLIPBOARD = &GUI_SIMPLE_MESSAGE_CONTENT_PASTE_FROM_CLIPBOARD_DATA;
static const gui_simple_message_content_stat_t GUI_SIMPLE_MESSAGE_CONTENT_DELETE_DATA = { .stat=NULL };
const gui_simple_message_content_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_DELETE = &GUI_SIMPLE_MESSAGE_CONTENT_DELETE_DATA;
static const gui_simple_message_content_stat_t GUI_SIMPLE_MESSAGE_CONTENT_UNDO_DATA = { .stat=NULL };
const gui_simple_message_content_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_UNDO = &GUI_SIMPLE_MESSAGE_CONTENT_UNDO_DATA;
static const gui_simple_message_content_stat_t GUI_SIMPLE_MESSAGE_CONTENT_REDO_DATA = { .stat=NULL };
const gui_simple_message_content_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_REDO = &GUI_SIMPLE_MESSAGE_CONTENT_REDO_DATA;


/*
Copyright 2020-2020 Andreas Warnke

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
