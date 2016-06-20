/* File: data_diagramelement.h; Copyright and License: see below */

#ifndef DATA_DIAGRAMELEMENT_H
#define DATA_DIAGRAMELEMENT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines in which diagram which classifier is visible
 */

#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants which define display options for a classifier in a diagram
 */
enum data_diagramelement_display_flag_enum {
    DATA_DIAGRAMELEMENT_DISPLAY_FLAG_NONE = 0x0,  /*!< no flags set */
    DATA_DIAGRAMELEMENT_DISPLAY_FLAG_RED = 0x1,  /*!< the classifier shall be highlighted by a color or other appropriate means */
    DATA_DIAGRAMELEMENT_DISPLAY_FLAG_BOLD = 0x2,  /*!< the classifier shall be drawn in bold/strong */
    DATA_DIAGRAMELEMENT_DISPLAY_FLAG_MINIMAL = 0x4,  /*!< the classifier shall be shown with minimal information, e.g. box and name only */
};

/*!
 *  \brief all data attributes needed for the diagramelement functions
 */
struct data_diagramelement_struct {
    int64_t id;  /*!< id of the diagramelement_t relation */
    int64_t diagram_id;  /*!< id of the referenced diagram_t */
    int64_t classifier_id;  /*!< id of the referenced classifier_t */
    uint64_t display_flags;  /*!< flags that influence the way how the classifier is displayed in the diagram. \see data_diagramelement_display_flag_enum */
};

typedef struct data_diagramelement_struct data_diagramelement_t;

/*!
 *  \brief initializes the data_diagramelement_t struct with id, classifier_id and diagram_id DATA_ID_VOID_ID.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_diagramelement_init_empty ( data_diagramelement_t *this_ );

/*!
 *  \brief re-initializes the data_diagramelement_t struct with id, classifier_id and diagram_id DATA_ID_VOID_ID.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_diagramelement_reinit_empty ( data_diagramelement_t *this_ );

/*!
 *  \brief initializes the data_diagramelement_t struct
 *
 *  id is initialized to DATA_ID_VOID_ID;
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the referenced diagram_t
 *  \param classifier_id id of the referenced classifier_t
 *  \param display_flags flags how to display the classifier in the context of this diagram. \see data_diagramelement_display_flag_enum
 */
static inline void data_diagramelement_init_new ( data_diagramelement_t *this_,
                                                  int64_t diagram_id,
                                                  int64_t classifier_id,
                                                  uint64_t display_flags
                                                );

/*!
 *  \brief initializes the data_diagramelement_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param id id of the data_diagramelement_t
 *  \param diagram_id id of the referenced diagram_t
 *  \param classifier_id id of the referenced classifier_t
 *  \param display_flags flags how to display the classifier in the context of this diagram. \see data_diagramelement_display_flag_enum
 */
static inline void data_diagramelement_init ( data_diagramelement_t *this_,
                                              int64_t id,
                                              int64_t diagram_id,
                                              int64_t classifier_id,
                                              uint64_t display_flags
                                            );

/*!
 *  \brief re-initializes the data_diagramelement_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param id id of the data_diagramelement_t
 *  \param diagram_id id of the referenced diagram_t
 *  \param classifier_id id of the referenced classifier_t
 *  \param display_flags flags how to display the classifier in the context of this diagram. \see data_diagramelement_display_flag_enum
 */
static inline void data_diagramelement_reinit ( data_diagramelement_t *this_,
                                                int64_t id,
                                                int64_t diagram_id,
                                                int64_t classifier_id,
                                                uint64_t display_flags
                                              );

/*!
 *  \brief destroys the data_diagramelement_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_diagramelement_destroy ( data_diagramelement_t *this_ );

/*!
 *  \brief prints the data_diagramelement_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_diagramelement_trace ( data_diagramelement_t *this_ );

#include "data_diagramelement.inl"

#endif  /* DATA_DIAGRAMELEMENT_H */


/*
Copyright 2016-2016 Andreas Warnke

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
