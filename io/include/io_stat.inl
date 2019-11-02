/* File: io_stat.inl; Copyright and License: see below */

static inline void io_stat_init( io_stat_t *this_ )
{
    (*this_).diagrams = 0;
    (*this_).diagramelements = 0;
    (*this_).classifiers = 0;
    (*this_).uncond_features = 0;
    (*this_).scenario_features = 0;
    (*this_).uncond_relationships = 0;
    (*this_).scenario_relationships = 0;
}

static inline void io_stat_destroy( io_stat_t *this_ )
{
}

static inline void io_stat_set_diagrams( io_stat_t *this_, uint32_t count )
{
    (*this_).diagrams = count;
}

static inline uint32_t io_stat_get_diagrams( const io_stat_t *this_ )
{
    return (*this_).diagrams;
}

static inline void io_stat_inc_diagrams( io_stat_t *this_, uint32_t count )
{
    (*this_).diagrams += count;
}

static inline void io_stat_set_diagramelements( io_stat_t *this_, uint32_t count )
{
    (*this_).diagramelements = count;
}

static inline uint32_t io_stat_get_diagramelements( const io_stat_t *this_ )
{
    return (*this_).diagramelements;
}

static inline void io_stat_inc_diagramelements( io_stat_t *this_, uint32_t count )
{
    (*this_).diagramelements += count;
}

static inline void io_stat_set_classifiers( io_stat_t *this_, uint32_t count )
{
    (*this_).classifiers = count;
}

static inline uint32_t io_stat_get_classifiers( const io_stat_t *this_ )
{
    return (*this_).classifiers;
}

static inline void io_stat_inc_classifiers( io_stat_t *this_, uint32_t count )
{
    (*this_).classifiers += count;
}

static inline void io_stat_set_uncond_features( io_stat_t *this_, uint32_t count )
{
    (*this_).uncond_features = count;
}

static inline uint32_t io_stat_get_uncond_features( const io_stat_t *this_ )
{
    return (*this_).uncond_features;
}

static inline void io_stat_inc_uncond_features( io_stat_t *this_, uint32_t count )
{
    (*this_).uncond_features += count;
}

static inline void io_stat_set_scenario_features( io_stat_t *this_, uint32_t count )
{
    (*this_).scenario_features = count;
}

static inline uint32_t io_stat_get_scenario_features( const io_stat_t *this_ )
{
    return (*this_).scenario_features;
}

static inline void io_stat_inc_scenario_features( io_stat_t *this_, uint32_t count )
{
    (*this_).scenario_features += count;
}

static inline void io_stat_set_uncond_relationships( io_stat_t *this_, uint32_t count )
{
    (*this_).uncond_relationships = count;
}

static inline uint32_t io_stat_get_uncond_relationships( const io_stat_t *this_ )
{
    return (*this_).uncond_relationships;
}

static inline void io_stat_inc_uncond_relationships( io_stat_t *this_, uint32_t count )
{
    (*this_).uncond_relationships += count;
}

static inline void io_stat_set_scenario_relationships( io_stat_t *this_, uint32_t count )
{
    (*this_).scenario_relationships = count;
}

static inline uint32_t io_stat_get_scenario_relationships( const io_stat_t *this_ )
{
    return (*this_).scenario_relationships;
}

static inline void io_stat_inc_scenario_relationships( io_stat_t *this_, uint32_t count )
{
    (*this_).scenario_relationships += count;
}


/*
Copyright 2019-2019 Andreas Warnke

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
