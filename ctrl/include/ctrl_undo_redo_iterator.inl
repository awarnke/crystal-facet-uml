/* File: ctrl_undo_redo_iterator.inl; Copyright and License: see below */

static inline void ctrl_undo_redo_iterator_init ( ctrl_undo_redo_iterator_t *this_,
                                                  const ctrl_undo_redo_entry_t (*ring_buf)[],
                                                  uint32_t ring_buf_size,
                                                  bool iterate_upwards,
                                                  uint32_t current,
                                                  uint32_t length )
{
    assert( ring_buf != NULL );
    assert( ring_buf_size > 0 );
    assert( current < ring_buf_size );
    assert( length <= ring_buf_size );
    (*this_).ring_buf = ring_buf;
    (*this_).ring_buf_size = ring_buf_size;
    (*this_).iterate_upwards = iterate_upwards;
    (*this_).current = current;
    (*this_).length = length;
}

static inline void ctrl_undo_redo_iterator_reinit ( ctrl_undo_redo_iterator_t *this_,
                                                    const ctrl_undo_redo_entry_t (*ring_buf)[],
                                                    uint32_t ring_buf_size,
                                                    bool iterate_upwards,
                                                    uint32_t current,
                                                    uint32_t length )
{
    ctrl_undo_redo_iterator_init( this_, ring_buf, ring_buf_size, iterate_upwards, current, length );
}

static inline void ctrl_undo_redo_iterator_init_empty ( ctrl_undo_redo_iterator_t *this_ )
{
    (*this_).ring_buf = NULL;
    (*this_).ring_buf_size = 1;
    (*this_).iterate_upwards = true;
    (*this_).current = 0;
    (*this_).length = 0;
}

static inline void ctrl_undo_redo_iterator_destroy ( ctrl_undo_redo_iterator_t *this_ )
{
    (*this_).ring_buf = NULL;
}

static inline bool ctrl_undo_redo_iterator_has_next ( const ctrl_undo_redo_iterator_t *this_ )
{
    return ( (*this_).length != 0 );
}

static inline const ctrl_undo_redo_entry_t * ctrl_undo_redo_iterator_next ( ctrl_undo_redo_iterator_t *this_ )
{
    assert( (*this_).length > 0 );

    const ctrl_undo_redo_entry_t * result = NULL;
    if ( (*this_).length > 0 )
    {
        result = &((*(*this_).ring_buf)[(*this_).current]);
        if ( (*this_).iterate_upwards )
        {
            (*this_).current = ( (*this_).current + 1 ) % (*this_).ring_buf_size;
        }
        else
        {
            (*this_).current = ( (*this_).current + (*this_).ring_buf_size - 1 ) % (*this_).ring_buf_size;
        }
        (*this_).length --;
    }

    return result;
}

static inline void ctrl_undo_redo_iterator_collect_statistics ( ctrl_undo_redo_iterator_t *this_,
                                                                bool undo,
                                                                data_stat_t *io_stat )
{
    assert( io_stat != NULL );
    while ( ctrl_undo_redo_iterator_has_next( this_ ) )
    {
        const ctrl_undo_redo_entry_t *const current_entry = ctrl_undo_redo_iterator_next( this_ );
        ctrl_undo_redo_entry_to_statistics ( current_entry, undo /*=undo*/, false /*=err*/, io_stat );
    }
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
