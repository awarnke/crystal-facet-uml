/* File: gui_simple_message_content.c; Copyright and License: see below */

#include "gui_simple_message_content.h"

static const gui_simple_message_content_quantity_t GUI_SIMPLE_MESSAGE_CONTENT_DB_INCONSISTENT_DATA = { .quantity=0 };
const gui_simple_message_content_quantity_t *GUI_SIMPLE_MESSAGE_CONTENT_DB_INCONSISTENT = &GUI_SIMPLE_MESSAGE_CONTENT_DB_INCONSISTENT_DATA;
static const gui_simple_message_content_quantity_t GUI_SIMPLE_MESSAGE_CONTENT_MAX_WINDOWS_ALREADY_OPEN_DATA = { .quantity=0 };
const gui_simple_message_content_quantity_t *GUI_SIMPLE_MESSAGE_CONTENT_MAX_WINDOWS_ALREADY_OPEN = &GUI_SIMPLE_MESSAGE_CONTENT_MAX_WINDOWS_ALREADY_OPEN_DATA;

#if 0
static const gui_simple_message_content_position_t GUI_SIMPLE_MESSAGE_CONTENT_INVALID_INPUT_DATA_DATA = { .stream_position=0 };
const gui_simple_message_content_position_t *GUI_SIMPLE_MESSAGE_CONTENT_INVALID_INPUT_DATA = &GUI_SIMPLE_MESSAGE_CONTENT_INVALID_INPUT_DATA_DATA;
#endif

static const gui_simple_message_content_name_t GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_NOT_OPENED_DATA = { .name=NULL };
const gui_simple_message_content_name_t *GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_NOT_OPENED = &GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_NOT_OPENED_DATA;
static const gui_simple_message_content_name_t GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_NOT_CREATEABLE_DATA = { .name=NULL };
const gui_simple_message_content_name_t *GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_NOT_CREATEABLE = &GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_NOT_CREATEABLE_DATA;
static const gui_simple_message_content_name_t GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_OPENED_WITH_ERROR_DATA = { .name=NULL };
const gui_simple_message_content_name_t *GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_OPENED_WITH_ERROR = &GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_OPENED_WITH_ERROR_DATA;
static const gui_simple_message_content_name_t GUI_SIMPLE_MESSAGE_CONTENT_FILE_EXPORT_FAILED_DATA = { .name=NULL };
const gui_simple_message_content_name_t *GUI_SIMPLE_MESSAGE_CONTENT_FILE_EXPORT_FAILED = &GUI_SIMPLE_MESSAGE_CONTENT_FILE_EXPORT_FAILED_DATA;
static const gui_simple_message_content_name_t GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE_DATA = { .name=NULL };
const gui_simple_message_content_name_t *GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE = &GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE_DATA;
static const gui_simple_message_content_name_t GUI_SIMPLE_MESSAGE_CONTENT_LOADING_WAIT_DATA = { .name=NULL };
const gui_simple_message_content_name_t *GUI_SIMPLE_MESSAGE_CONTENT_LOADING_WAIT = &GUI_SIMPLE_MESSAGE_CONTENT_LOADING_WAIT_DATA;
static const gui_simple_message_content_name_t GUI_SIMPLE_MESSAGE_CONTENT_EXPORTING_WAIT_DATA = { .name=NULL };
const gui_simple_message_content_name_t *GUI_SIMPLE_MESSAGE_CONTENT_EXPORTING_WAIT = &GUI_SIMPLE_MESSAGE_CONTENT_EXPORTING_WAIT_DATA;

static const gui_simple_message_content_error_t GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED_DATA = { .error_message=NULL };
const gui_simple_message_content_error_t *GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED = &GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED_DATA;

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

static const gui_simple_message_content_names_stat_t GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED_DATA = { .list_of_names=NULL, .stat=NULL };
const gui_simple_message_content_names_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED = &GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED_DATA;


/*
Copyright 2020-2022 Andreas Warnke

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
