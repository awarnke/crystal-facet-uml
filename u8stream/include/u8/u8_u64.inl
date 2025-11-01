/* File: u8_u64.inl; Copyright and License: see below */

#include <assert.h>

static inline void u8_u64_get_hex ( const uint64_t value, u8_u64_hex_t *out_hex )
{
    assert( out_hex != NULL );
    /* Note: snprintf is not available on every OS */
    sprintf( &((*out_hex)[0]), "%" PRIx64, value );
}

static inline void u8_u64_get_dec ( const uint64_t value, u8_u64_dec_t *out_dec )
{
    assert( out_dec != NULL );
    /* Note: snprintf is not available on every OS */
    sprintf( &((*out_dec)[0]), "%" PRIu64, value );
}


/*
Copyright 2025-2025 Andreas Warnke

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
