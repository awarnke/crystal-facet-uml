/* File: gui_button.inl; Copyright and License: see below */

static inline void gui_button_init( gui_button_t *this_,
                                    GdkPaintable * icon_source,
                                    const char *label_text,
                                    const char *tooltip_text )
{
    (*this_).icon = GTK_IMAGE( gtk_image_new_from_paintable( icon_source ) );
    gtk_widget_set_size_request( GTK_WIDGET((*this_).icon), 32 /*=w*/ , 32 /*=h*/ );

    (*this_).label = GTK_LABEL( gtk_label_new( label_text ) );

    (*this_).box = GTK_BOX( gtk_box_new(  GTK_ORIENTATION_VERTICAL, 0 /* spacing */ ) );
    gtk_box_append( (*this_).box, GTK_WIDGET((*this_).icon) );
    gtk_box_append( (*this_).box, GTK_WIDGET((*this_).label) );

    (*this_).button = GTK_BUTTON( gtk_button_new() );
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
    return (*this_).button;
}


/*
Copyright 2024-2024 Andreas Warnke

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
