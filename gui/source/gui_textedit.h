/* File: gui_textedit.h; Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

#ifndef GUI_TEXTEDIT_H
#define GUI_TEXTEDIT_H

#include <gtk/gtk.h>

struct gui_textedit_struct {
    int32_t lastUnicode;
    int32_t currentUnicode;
    char textBuf[256];
    char outputBuf[1536];
    GtkWidget *characterEntry;
    GtkAdjustment *scrollRange;
};

typedef struct gui_textedit_struct gui_textedit_t;

#define GUI_TEXTEDIT(ME,OUTWIDGET,SCROLLRANGE) {-1,-1,"","",OUTWIDGET,SCROLLRANGE,}

void btn_clear_callback( GtkButton* button, gpointer data );

#endif  /* GUI_TEXTEDIT_H */


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
