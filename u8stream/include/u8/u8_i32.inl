/* File: u8_i32.inl; Copyright and License: see below */

static inline int_fast32_t u8_i32_max2 ( const int_fast32_t a, const int_fast32_t b )
{
    return ( a > b ? a : b );
}

static inline int_fast32_t u8_i32_max3 ( const int_fast32_t a, const int_fast32_t b, const int_fast32_t c )
{
    return ( a > b ? ( a > c ? a : c ) : ( b > c ? b : c ) );
}

static inline int_fast32_t u8_i32_min2 ( const int_fast32_t a, const int_fast32_t b )
{
    return ( a < b ? a : b );
}

static inline int_fast32_t u8_i32_min3 ( const int_fast32_t a, const int_fast32_t b, const int_fast32_t c )
{
    return ( a < b ? ( a < c ? a : c ) : ( b < c ? b : c ) );
}


/*
Copyright 2023-2026 Andreas Warnke

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
