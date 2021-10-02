/* File: io_element_writer.h; Copyright and License: see below */

#ifndef IO_ELEMENT_WRITER_H
#define IO_ELEMENT_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a pair of a) a pointer to a concrete instance of an interface
 *  and b) a pointer to objectdata that implements the interface.
 */

#include "io_element_writer_if.h"
#include "data_classifier.h"
#include "data_classifier_type.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "data_relationship_type.h"
#include "data_diagram.h"
#include "data_diagram_type.h"
#include "data_diagramelement.h"

/*!
 *  \brief object (vmt+data) of a io_element_writer_t.
 *
 */
struct io_element_writer_struct {
    const io_element_writer_if_t* interface;  /*!< set of interface functions to write to a stream, kind of VMT */
    void* objectdata;  /*!< object that implements writing to a stream, used in interface functions as this_ parameter */
};

typedef struct io_element_writer_struct io_element_writer_t;

/*!
 *  \brief initializes the io_element_writer_t
 *
 *  Note: While init only initializes the \c io_element_writer_t struct,
 *  destroy also calls the destroy function in \c io_element_writer_if_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param interface set of interface functions to write to a stream
 *  \param objectdata object that implements writing to a stream
 */
static inline void io_element_writer_init( io_element_writer_t *this_,
                                           const io_element_writer_if_t *interface,
                                           void* objectdata
                                         );

/*!
 *  \brief destroys the io_element_writer_t.
 *
 *  This function does NOT call \c destroy on the \c interface.
 *
 *  \param this_ pointer to own object attributes
 *  \return returns 0 if success, -1 in case of error
 */
static inline int io_element_writer_destroy( io_element_writer_t *this_ );

/*!
 *  \brief gets the set of interface functions
 *
 *  \param this_ pointer to own object attributes
 *  \return the set of interface functions
 */
static inline const io_element_writer_if_t* io_element_writer_get_interface( io_element_writer_t *this_ );

/*!
 *  \brief gets the object that implements writing to a stream
 *
 *  \param this_ pointer to own object attributes
 *  \return the object data that implements the interface
 */
static inline void* io_element_writer_get_objectdata( io_element_writer_t *this_ );

/*!
 *  \brief writes the header of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param document_title title of the document
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_write_header( io_element_writer_t *this_, const char *document_title );

/*!
 *  \brief writes the start of the main section
 *
 *  This starts a section that contains the main part of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param document_title title of the document
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_start_main( io_element_writer_t *this_, const char *document_title );

/*!
 *  \brief checks if a parent classifier may nest a child classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type data_classifier_type_t of the parent of which the nesting-ability shall be determined
 *  \param child_type data_classifier_type_t of the nested child of which the nesting-ability shall be determined
 *  \return true if nesting is allowed
 */
static inline bool io_element_writer_can_classifier_nest_classifier( io_element_writer_t *this_,
                                                                     data_classifier_type_t parent_type,
                                                                     data_classifier_type_t child_type
                                                                   );

/*!
 *  \brief checks if a parent classifier may nest relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type data_classifier_type_t of the parent of which the nesting-ability shall be determined
 *  \param child_type data_relationship_type_t of the nested child of which the nesting-ability shall be determined
 *  \return true if nesting or any relationship is allowed
 */
static inline bool io_element_writer_can_classifier_nest_relationship( io_element_writer_t *this_,
                                                                       data_classifier_type_t parent_type,
                                                                       data_relationship_type_t child_type
                                                                     );

/*!
 *  \brief writes a classifier start-element
 *
 *  This starts a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_start_classifier( io_element_writer_t *this_,
                                                      data_classifier_type_t parent_type,
                                                      const data_classifier_t *classifier_ptr
                                                    );

/*!
 *  \brief writes contents of a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_assemble_classifier( io_element_writer_t *this_,
                                                         data_classifier_type_t parent_type,
                                                         const data_classifier_t *classifier_ptr
                                                       );

/*!
 *  \brief writes a classifier end-element
 *
 *  This ends a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier, needed for xmi export
 *  \param classifier_ptr pointer to classifier that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_end_classifier( io_element_writer_t *this_,
                                                    data_classifier_type_t parent_type,
                                                    const data_classifier_t *classifier_ptr
                                                  );

/*!
 *  \brief writes a feature start-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_start_feature( io_element_writer_t *this_,
                                                   data_classifier_type_t parent_type,
                                                   const data_feature_t *feature_ptr
                                                 );

/*!
 *  \brief writes constents of a a feature
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_assemble_feature( io_element_writer_t *this_,
                                                      data_classifier_type_t parent_type,
                                                      const data_feature_t *feature_ptr
                                                    );

/*!
 *  \brief writes a feature end-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent classifier
 *  \param feature_ptr pointer to feature that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_end_feature( io_element_writer_t *this_,
                                                 data_classifier_type_t parent_type,
                                                 const data_feature_t *feature_ptr
                                               );

/*!
 *  \brief starts a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier, needed for xmi export
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_start_relationship( io_element_writer_t *this_,
                                                        data_classifier_type_t parent_type,
                                                        const data_relationship_t *relation_ptr
                                                      );

/*!
 *  \brief writes the contents of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier, needed for xmi export
 *  \param parent_is_source indicates if the parent/hosting classifier is the source end of the relationship
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \param from_c_type the type of classifier at source end
 *  \param from_f_type the type of feature at source end; DATA_FEATURE_TYPE_VOID if no feature specified
 *  \param to_c_type the type of classifier at target end
 *  \param to_f_type the type of feature at target end; DATA_FEATURE_TYPE_VOID if no feature specified
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_assemble_relationship( io_element_writer_t *this_,
                                                           data_classifier_type_t parent_type,
                                                           bool parent_is_source,
                                                           const data_relationship_t *relation_ptr,
                                                           data_classifier_type_t from_c_type,
                                                           data_feature_type_t from_f_type,
                                                           data_classifier_type_t to_c_type,
                                                           data_feature_type_t to_f_type
                                                         );

/*!
 *  \brief ends a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classifier, needed for xmi export
 *  \param relation_ptr pointer to relationship that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_end_relationship( io_element_writer_t *this_,
                                                      data_classifier_type_t parent_type,
                                                      const data_relationship_t *relation_ptr
                                                    );

/*!
 *  \brief writes a diagram start
 *
 *  This starts a section that contains a diagram and a list of diagramelements (classifier-occurrences)
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_start_diagram( io_element_writer_t *this_, const data_diagram_t *diag_ptr );

/*!
 *  \brief writes a diagram of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written
 *  \param diagram_file_base_name filename of the diagram without extension
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_assemble_diagram( io_element_writer_t *this_,
                                       const data_diagram_t *diag_ptr,
                                       const char *diagram_file_base_name
                                     );

/*!
 *  \brief ends a diagram
 *
 *  This ends a section that contains a diagram and a list of diagramelements (classifier-occurrences)
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_end_diagram( io_element_writer_t *this_, const data_diagram_t *diag_ptr );

/*!
 *  \brief writes a diagramelement start-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent diagram
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_start_diagramelement( io_element_writer_t *this_,
                                                          data_diagram_type_t parent_type,
                                                          const data_diagramelement_t *diagramelement_ptr
                                                        );

/*!
 *  \brief writes constents of a a diagramelement
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent diagram
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_assemble_diagramelement( io_element_writer_t *this_,
                                                             data_diagram_type_t parent_type,
                                                             const data_diagramelement_t *diagramelement_ptr
                                                           );

/*!
 *  \brief writes a diagramelement end-element
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the owning parent diagram
 *  \param diagramelement_ptr pointer to diagramelement that shall be written, not NULL
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_end_diagramelement( io_element_writer_t *this_,
                                                        data_diagram_type_t parent_type,
                                                        const data_diagramelement_t *diagramelement_ptr
                                                      );


/*!
 *  \brief writes the ending of the main section
 *
 *  This ends a section that contains the main part of the document
 *
 *  \param this_ pointer to own object attributes
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_end_main( io_element_writer_t *this_ );

/*!
 *  \brief writes the footer of the document
 *
 *  \param this_ pointer to own object attributes
 *  \return 0 in case of success, -1 otherwise
 */
static inline int io_element_writer_write_footer( io_element_writer_t *this_ );

#include "io_element_writer.inl"

#endif  /* IO_ELEMENT_WRITER_H */


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
