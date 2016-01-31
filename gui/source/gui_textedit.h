/* File: gui/textedit.h; Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

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
