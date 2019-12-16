/* File: pencil_classifier_layouter.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline void pencil_classifier_layouter_private_sort_classifiers_by_list_order( const pencil_classifier_layouter_t *this_,
                                                                                      universal_array_index_sorter_t *out_sorted_classifiers )
{
    assert ( ((uint32_t)UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE) >= ((uint32_t)DATA_VISIBLE_SET_MAX_CLASSIFIERS) );

    universal_array_index_sorter_reinit( out_sorted_classifiers );
    uint32_t count_clasfy;
    count_clasfy = pencil_layout_data_get_classifier_count ( (*this_).layout_data );
    for ( uint32_t c_idx = 0; c_idx < count_clasfy; c_idx ++ )
    {
        layout_visible_classifier_t *visible_classifier_probe;
        visible_classifier_probe = pencil_layout_data_get_classifier_ptr ( (*this_).layout_data, c_idx );

        const data_classifier_t *classifier_probe;
        classifier_probe = layout_visible_classifier_get_classifier_const( visible_classifier_probe );

        int err;
        double weight;
        weight = data_classifier_get_list_order( classifier_probe );
        err = universal_array_index_sorter_insert( out_sorted_classifiers, c_idx, weight );
        if ( 0 != err )
        {
            TSLOG_ERROR ( "universal_array_index_sorter_t list is full." );
        }
    }
}


/*
Copyright 2018-2019 Andreas Warnke

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
