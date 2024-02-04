/* File: gui_type_resource.c; Copyright and License: see below */

#include "gui_type_resource.h"
#include <assert.h>

G_DEFINE_TYPE (GuiTypeResource, gui_type_resource, G_TYPE_OBJECT)

static void
gui_type_resource_class_init (GuiTypeResourceClass *klass)
{
}

static void
gui_type_resource_init (GuiTypeResource *self)
{
    data_type_init_void( &((*self).type_id) );
    (*self).name = NULL;
    (*self).icon = NULL;
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
