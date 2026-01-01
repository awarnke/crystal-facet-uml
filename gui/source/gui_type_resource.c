/* File: gui_type_resource.c; Copyright and License: see below */

#include "gui_type_resource.h"
#include "u8/u8_trace.h"
#include <assert.h>

G_DEFINE_TYPE (GuiTypeResource, gui_type_resource, G_TYPE_OBJECT)

typedef enum
{
  GUI_TYPE_RESOURCE_PROP_NAME = 1,
  GUI_TYPE_RESOURCE_PROP_ICON,
  GUI_TYPE_RESOURCE_PROP_MAX
} GuiTypeResourceProperty;

static GParamSpec *gui_type_resource_properties[GUI_TYPE_RESOURCE_PROP_MAX] = { NULL, };

static void
gui_type_resource_set_property( GObject      *object,
                                guint         property_id,
                                const GValue *value,
                                GParamSpec   *pspec )
{
    U8_TRACE_BEGIN();
    GuiTypeResource *self = GUI_TYPE_RESOURCE (object);

    switch ((GuiTypeResourceProperty) property_id)
    {
        case GUI_TYPE_RESOURCE_PROP_NAME:
        {
            /* this object is read only */
            self->name = g_value_get_string( value );
        }
        break;

        case GUI_TYPE_RESOURCE_PROP_ICON:
        {
            /* this object is read only */
            if ( self->icon != NULL )
            {
                g_object_unref( self->icon );
            }
            self->icon = g_value_get_object( value );
        }
        break;

        default:
        {
            /* We don't have any other property... */
            G_OBJECT_WARN_INVALID_PROPERTY_ID( object, property_id, pspec );
        }
        break;
    }

    U8_TRACE_END();
}

static void
gui_type_resource_get_property( GObject    *object,
                                guint       property_id,
                                GValue     *value,
                                GParamSpec *pspec )
{
    U8_TRACE_BEGIN();
    GuiTypeResource *self = GUI_TYPE_RESOURCE (object);

    switch ((GuiTypeResourceProperty) property_id)
    {
        case GUI_TYPE_RESOURCE_PROP_NAME:
        {
            g_value_set_static_string( value, self->name );
        }
        break;

        case GUI_TYPE_RESOURCE_PROP_ICON:
        {
            g_value_set_object( value, self->icon );
        }
        break;

        default:
        {
            /* We don't have any other property... */
            G_OBJECT_WARN_INVALID_PROPERTY_ID( object, property_id, pspec );
        }
        break;
    }

    U8_TRACE_END();
}

static void
gui_type_resource_class_init( GuiTypeResourceClass *klass )
{
    U8_TRACE_BEGIN();
    GObjectClass *object_class = G_OBJECT_CLASS ( klass );

    object_class->set_property = gui_type_resource_set_property;
    object_class->get_property = gui_type_resource_get_property;

    gui_type_resource_properties[GUI_TYPE_RESOURCE_PROP_NAME] =
    g_param_spec_string( "name",
                         "Name",
                         "Name of the represented element type.",
                         NULL  /* default value */,
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE
                       );

    gui_type_resource_properties[GUI_TYPE_RESOURCE_PROP_ICON] =
    g_param_spec_object( "icon",
                         "Icon",
                         "Icon of the represented element type.",
                         gdk_texture_get_type(),
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE
                       );

    g_object_class_install_properties( object_class,
                                       GUI_TYPE_RESOURCE_PROP_MAX,
                                       gui_type_resource_properties
                                     );
    U8_TRACE_END();
}

static void
gui_type_resource_init (GuiTypeResource *self)
{
    data_type_init_void( &((*self).type_id) );
    (*self).name = NULL;
    (*self).icon = NULL;
}

gboolean gui_type_resource_equal( const gui_type_resource_t *this_, const gui_type_resource_t *that )
{
    U8_TRACE_BEGIN();
    const gboolean result = ( data_type_equals( &((*this_).type_id), &((*that).type_id) ) );
    U8_TRACE_END();
    return result;
}


/*
Copyright 2024-2026 Andreas Warnke

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
