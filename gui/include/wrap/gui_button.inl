/* File: gui_button.inl; Copyright and License: see below */

static inline void gui_button_init( gui_button_t *this_,
                                    GdkPaintable * icon_source,
                                    const char *label_text,
                                    const char *tooltip_text )
{
    (*this_).icon = GTK_IMAGE( gtk_image_new_from_paintable( icon_source ) );
    gtk_widget_set_size_request( GTK_WIDGET((*this_).icon), 32 /*=w*/ , 32 /*=h*/ );

    /*
    utf8stream_writemem_t css_out;
    utf8stream_writemem_init( &css_out, css_buf, sizeof(css_buf) );
    utf8stream_writer_t *css_in = utf8stream_writemem_get_writer( &css_out );
    u8_error_t write_err = U8_ERROR_NONE;
    write_err |= utf8stream_writer_write_str( &css_in, "<small>" );
    write_err |= utf8stream_writer_write_str( &css_in, label_text );
    write_err |= utf8stream_writer_write_str( &css_in, "</small>" );
    write_err |= utf8stream_writer_flush( &css_in );
    utf8stream_writemem_destroy( &css_out );
    */
    char css_buf[48] = "";
    utf8stringbuf_t css = UTF8STRINGBUF( css_buf );
    utf8stringbuf_append_str( &css, "<small>" );
    utf8stringbuf_append_str( &css, label_text );
    utf8stringbuf_append_str( &css, "</small>" );

    (*this_).label = GTK_LABEL( gtk_label_new( NULL ) );
    gtk_label_set_markup( (*this_).label, utf8stringbuf_get_string( &css ) );

    (*this_).box = GTK_BOX( gtk_box_new( GTK_ORIENTATION_VERTICAL, 4 /* spacing */ ) );
    gtk_box_append( (*this_).box, GTK_WIDGET((*this_).icon) );
    gtk_box_append( (*this_).box, GTK_WIDGET((*this_).label) );

    (*this_).button = GTK_BUTTON( gtk_button_new() );
    gtk_button_set_child( (*this_).button, GTK_WIDGET((*this_).box) );

    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).button), tooltip_text );

    /* increase the reference counter to keep the button and its children alive */
    /* g_object_ref( (*this_).button ); */
}

static inline void gui_button_init_toggle( gui_button_t *this_,
                                           GdkPaintable * icon_source,
                                           const char *label_text,
                                           const char *tooltip_text )
{
    (*this_).icon = GTK_IMAGE( gtk_image_new_from_paintable( icon_source ) );
    gtk_widget_set_size_request( GTK_WIDGET((*this_).icon), 32 /*=w*/ , 32 /*=h*/ );

    char css_buf[48] = "";
    utf8stringbuf_t css = UTF8STRINGBUF( css_buf );
    utf8stringbuf_append_str( &css, "<small>" );
    utf8stringbuf_append_str( &css, label_text );
    utf8stringbuf_append_str( &css, "</small>" );

    (*this_).label = GTK_LABEL( gtk_label_new( NULL ) );
    gtk_label_set_markup( (*this_).label, utf8stringbuf_get_string( &css ) );

    (*this_).box = GTK_BOX( gtk_box_new( GTK_ORIENTATION_VERTICAL, 2 /* spacing */ ) );
    gtk_box_append( (*this_).box, GTK_WIDGET((*this_).icon) );
    gtk_box_append( (*this_).box, GTK_WIDGET((*this_).label) );

    (*this_).button = GTK_BUTTON( gtk_toggle_button_new() );
    gtk_button_set_child( (*this_).button, GTK_WIDGET((*this_).box) );

    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).button), tooltip_text );

    /* increase the reference counter to keep the button and its children alive */
    /* g_object_ref( (*this_).button ); */
}

static inline void gui_button_destroy ( gui_button_t *this_ )
{
    /* decrease the reference counter to allow the button and its children to be deleted */
    /* g_object_unref( (*this_).button ); */
}

static inline GtkImage * gui_button_get_icon_ptr ( const gui_button_t *this_ )
{
    return (*this_).icon;
}

static inline GtkLabel * gui_button_get_label_ptr ( const gui_button_t *this_ )
{
    return (*this_).label;
}

static inline GtkButton * gui_button_get_button_ptr ( const gui_button_t *this_ )
{
    return GTK_BUTTON((*this_).button);
}

static inline GtkWidget * gui_button_get_widget_ptr ( const gui_button_t *this_ )
{
    return GTK_WIDGET((*this_).button);
}


/*
Copyright 2024-2025 Andreas Warnke

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
