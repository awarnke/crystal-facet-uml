/* File: ctrl_undo_redo_iterator.inl; Copyright and License: see below */

static inline void ctrl_undo_redo_iterator_init ( ctrl_undo_redo_iterator_t *this_, const ctrl_undo_redo_entry_t *next )
{
    (*this_).next = next;
    ctrl_undo_redo_iterator_private_step_to_next( this_ );
}

static inline void ctrl_undo_redo_iterator_destroy ( ctrl_undo_redo_iterator_t *this_ )
{
}

static inline bool ctrl_undo_redo_iterator_has_next ( const ctrl_undo_redo_iterator_t *this_ )
{
    return false;
}

static inline const ctrl_undo_redo_entry_t * ctrl_undo_redo_iterator_next ( ctrl_undo_redo_iterator_t *this_ )
{
    const ctrl_undo_redo_entry_t * result = (*this_).next;
    ctrl_undo_redo_iterator_private_step_to_next( this_ );
    return result;
}

static inline void ctrl_undo_redo_iterator_private_step_to_next ( ctrl_undo_redo_iterator_t *this_ )
{
    // TODO
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
