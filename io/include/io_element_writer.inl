/* File: io_element_writer.inl; Copyright and License: see below */

static inline void io_element_writer_init( io_element_writer_t *this_,
                                           const io_element_writer_if_t *interface,
                                           void* objectdata )
{
    (*this_).interface = interface;
    (*this_).objectdata = objectdata;
}

static inline int io_element_writer_destroy( io_element_writer_t *this_ )
{
    /*int result = (*(  (*(  (*this_).interface  )).destroy  )) ( (*this_).objectdata );*/
    (*this_).interface = NULL;
    (*this_).objectdata = NULL;
    /*return result;*/
    return 0;
}

static inline const io_element_writer_if_t* io_element_writer_get_interface ( io_element_writer_t *this_ )
{
    return (*this_).interface;
}

static inline void* io_element_writer_get_objectdata ( io_element_writer_t *this_ )
{
    return (*this_).objectdata;
}

static inline int io_element_writer_open ( io_element_writer_t* this_, const char* identifier )
{
    return (*(  (*(  (*this_).interface  )).open  )) ( (*this_).objectdata, identifier );
}

static inline int io_element_writer_write ( io_element_writer_t* this_, const void *start, size_t length )
{
    return (*(  (*(  (*this_).interface  )).write  )) ( (*this_).objectdata, start, length );
}

static inline int io_element_writer_flush ( io_element_writer_t* this_ )
{
    return (*(  (*(  (*this_).interface  )).flush  )) ( (*this_).objectdata );
}

static inline int io_element_writer_close ( io_element_writer_t* this_ )
{
    return (*(  (*(  (*this_).interface  )).close  )) ( (*this_).objectdata );
}


/*
Copyright 2021-2021 Andreas Warnke

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
