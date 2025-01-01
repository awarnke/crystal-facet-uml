/* File: ctrl_consistency_checker.c; Copyright and License: see below */

#include "ctrl_consistency_checker.h"
#include "storage/data_database_consistency_checker.h"
#include "entity/data_id.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

void ctrl_consistency_checker_init ( ctrl_consistency_checker_t *this_,
                                     data_database_t *database,
                                     data_database_reader_t *db_reader,
                                     data_database_writer_t *db_writer )
{
    U8_TRACE_BEGIN();
    assert( NULL != database );
    assert( NULL != db_reader );
    assert( NULL != db_writer );

    (*this_).database = database;
    (*this_).db_reader = db_reader;
    (*this_).db_writer = db_writer;

    data_database_consistency_checker_init( &((*this_).db_checker), database );

    U8_TRACE_END();
}

void ctrl_consistency_checker_destroy ( ctrl_consistency_checker_t *this_ )
{
    U8_TRACE_BEGIN();

    data_database_consistency_checker_destroy( &((*this_).db_checker) );

    (*this_).database = NULL;
    (*this_).db_reader = NULL;
    (*this_).db_writer = NULL;

    U8_TRACE_END();
}

u8_error_t ctrl_consistency_checker_repair_database ( ctrl_consistency_checker_t *this_,
                                                      bool modify_db,
                                                      uint32_t *out_err,
                                                      uint32_t *out_fix,
                                                      utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert ( NULL != out_english_report );
    u8_error_t err_result = U8_ERROR_NONE;
    uint32_t error_count = 0;
    uint32_t fix_count = 0;

    /* write report title */
    if ( modify_db )
    {
        utf8stream_writer_write_str( out_english_report, "TITLE: Repair database\n" );
    }
    else
    {
        utf8stream_writer_write_str( out_english_report, "TITLE: Check database\n" );
    }

    /* == find inconsistencies in drawings == */

    /* get all root diagrams */
    err_result |= ctrl_consistency_checker_private_ensure_single_root_diagram( this_, modify_db, &error_count, &fix_count, out_english_report );

    /* find invalid and circular diagram parent links */
    err_result |= ctrl_consistency_checker_private_ensure_valid_diagram_parents( this_, modify_db, &error_count, &fix_count, out_english_report );

    /* find nonreferencing diagramelements */
    err_result |= ctrl_consistency_checker_private_ensure_valid_diagramelements( this_, modify_db, &error_count, &fix_count, out_english_report );

    /* find illreferencing diagramelements */
    err_result |= ctrl_consistency_checker_private_ensure_valid_diagele_features( this_, modify_db, &error_count, &fix_count, out_english_report );

    /* find unreferenced, invisible classifiers */
    err_result |= ctrl_consistency_checker_private_ensure_referenced_classifiers( this_, modify_db, &error_count, &fix_count, out_english_report );

    /* == find inconsistencies in meta model == */

    /* find nonreferencing features */
    err_result |= ctrl_consistency_checker_private_ensure_valid_feature_parents( this_, modify_db, &error_count, &fix_count, out_english_report );

    /* find nonreferencing relationships */
    err_result |= ctrl_consistency_checker_private_ensure_valid_relationship_classifiers( this_, modify_db, &error_count, &fix_count, out_english_report );

    /* find illreferencing relationships */
    err_result |= ctrl_consistency_checker_private_ensure_valid_relationship_features( this_, modify_db, &error_count, &fix_count, out_english_report );

    /* prepare results and return */
    if ( NULL != out_err )
    {
        (*out_err) = error_count;
    }
    if ( NULL != out_fix )
    {
        (*out_fix) = fix_count;
    }

    /* write report summary */
    const char *db_filename = data_database_get_filename_ptr ( (*this_).database );
    utf8stream_writer_write_str( out_english_report, "SUMMARY: " );
    if ( db_filename != NULL )
    {
        utf8stream_writer_write_str( out_english_report, "\n    File: " );
        utf8stream_writer_write_str( out_english_report, db_filename );
    }
    utf8stream_writer_write_str( out_english_report, "\n    Errors found: " );
    utf8stream_writer_write_int( out_english_report, error_count );
    utf8stream_writer_write_str( out_english_report, "\n    Errors fixed: " );
    utf8stream_writer_write_int( out_english_report, fix_count );
    utf8stream_writer_write_str( out_english_report, "\n" );

    /* if not all errors fixed, set result to U8_ERROR_DB_STRUCTURE */
    if ( error_count > fix_count )
    {
        err_result |= U8_ERROR_DB_STRUCTURE;
    }

    U8_TRACE_END_ERR( err_result );
    return err_result;
}

u8_error_t ctrl_consistency_checker_private_ensure_single_root_diagram ( ctrl_consistency_checker_t *this_,
                                                                         bool modify_db,
                                                                         uint32_t *io_err,
                                                                         uint32_t *io_fix,
                                                                         utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert ( NULL != io_err );
    assert ( NULL != io_fix );
    assert ( NULL != out_english_report );
    u8_error_t result = U8_ERROR_NONE;

    /* write report title */
    utf8stream_writer_write_str( out_english_report, "STEP: Ensure a single root diagram\n" );

    /* get all root diagrams */
    data_small_set_t all_roots;
    data_small_set_init( &all_roots );
    data_diagram_iterator_t diagram_iterator;
    result |= data_diagram_iterator_init_empty( &diagram_iterator );

    result |= data_database_reader_get_diagrams_by_parent_id( (*this_).db_reader,
                                                              DATA_ROW_VOID,
                                                              &diagram_iterator
                                                            );

    if ( U8_ERROR_NONE != result )
    {
        utf8stream_writer_write_str( out_english_report, "ERROR READING DATABASE.\n" );
    }

    while ( data_diagram_iterator_has_next( &diagram_iterator ) )
    {
        result |= data_diagram_iterator_next( &diagram_iterator, &((*this_).temp_diagram) );
        result |= data_small_set_add_obj( &all_roots, data_diagram_get_data_id( &((*this_).temp_diagram) ) );

        utf8stream_writer_write_str( out_english_report, "    INFO: Root diagram: " );
        utf8stream_writer_write_int( out_english_report, data_diagram_get_row_id( &((*this_).temp_diagram) ) );
        utf8stream_writer_write_str( out_english_report, ": " );
        utf8stream_writer_write_str( out_english_report, data_diagram_get_name_const( &((*this_).temp_diagram) ) );
        utf8stream_writer_write_str( out_english_report, "\n" );
    }
    const uint_fast32_t root_diagram_count = data_small_set_get_count( &all_roots );
    utf8stream_writer_write_str( out_english_report, "    ROOT DIAGRAM COUNT: " );
    utf8stream_writer_write_int( out_english_report, root_diagram_count );
    utf8stream_writer_write_str( out_english_report, "\n" );
    result |= data_diagram_iterator_destroy( &diagram_iterator );

    if ( root_diagram_count == 0 )
    {
        (*io_err) ++;
        utf8stream_writer_write_str( out_english_report, "    PROPOSED FIX: Create a diagram via the GUI.\n" );
    }
    else if ( root_diagram_count > 1 )
    {
        (*io_err) += (root_diagram_count-1) ;
        const data_id_t proposed_root_diagram = data_small_set_get_id( &all_roots, 0 );
        const data_row_t proposed_root_diagram_id = data_id_get_row_id( &proposed_root_diagram );
        if ( ! modify_db )
        {
            utf8stream_writer_write_str( out_english_report, "    PROPOSED FIX: Attach additional root diagrams below the first: " );
            utf8stream_writer_write_int( out_english_report, proposed_root_diagram_id );
            utf8stream_writer_write_str( out_english_report, "\n" );
        }
        else
        {
            for ( int list_pos = 1; list_pos < root_diagram_count; list_pos ++ )
            {
                const data_id_t proposed_child_diagram = data_small_set_get_id( &all_roots, list_pos );
                const data_row_t proposed_child_diagram_id = data_id_get_row_id( &proposed_child_diagram );
                const u8_error_t data_err
                    = data_database_writer_update_diagram_parent_id( (*this_).db_writer,
                                                                     proposed_child_diagram_id,
                                                                     proposed_root_diagram_id,
                                                                     NULL
                                                                   );
                if ( U8_ERROR_NONE == data_err )
                {
                    utf8stream_writer_write_str( out_english_report, "    FIX: Diagram " );
                    utf8stream_writer_write_int( out_english_report, proposed_child_diagram_id );
                    utf8stream_writer_write_str( out_english_report, " attached to " );
                    utf8stream_writer_write_int( out_english_report, proposed_root_diagram_id);
                    utf8stream_writer_write_str( out_english_report, "\n" );
                    (*io_fix) ++;
                }
                else
                {
                    utf8stream_writer_write_str( out_english_report, "ERROR WRITING DATABASE.\n" );
                    result |= data_err;
                }
            }
        }
    }
    data_small_set_destroy( &all_roots );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_consistency_checker_private_ensure_valid_diagram_parents ( ctrl_consistency_checker_t *this_,
                                                                           bool modify_db,
                                                                           uint32_t *io_err,
                                                                           uint32_t *io_fix,
                                                                           utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert ( NULL != io_err );
    assert ( NULL != out_english_report );
    u8_error_t err_result = U8_ERROR_NONE;
    u8_error_t data_err;

    /* write report title */
    utf8stream_writer_write_str( out_english_report, "STEP: Ensure that no circular/invalid references of diagram parents exist\n" );

    data_small_set_t circ_ref;
    data_small_set_init( &circ_ref );
    data_err = data_database_consistency_checker_find_circular_diagram_parents ( &((*this_).db_checker), &circ_ref );
    if ( U8_ERROR_NONE == data_err )
    {
        uint32_t circ_ref_count = data_small_set_get_count( &circ_ref );

        utf8stream_writer_write_str( out_english_report, "    DIAGRAMS WITH CIRCLUAR OR INVALID PARENT REFERENCES: " );
        utf8stream_writer_write_int( out_english_report, circ_ref_count );
        utf8stream_writer_write_str( out_english_report, "\n" );

        if ( circ_ref_count != 0 )
        {
            (*io_err) += circ_ref_count;

            /* get the root diagram */
            data_row_t root_diag_id = DATA_ROW_VOID;
            {
                data_diagram_iterator_t diagram_iterator;
                err_result |= data_diagram_iterator_init_empty( &diagram_iterator );
                err_result |= data_database_reader_get_diagrams_by_parent_id( (*this_).db_reader,
                                                                              DATA_ROW_VOID,
                                                                              &diagram_iterator
                                                                            );
                if (( U8_ERROR_NONE == err_result )&& data_diagram_iterator_has_next( &diagram_iterator ) )
                {
                    err_result |= data_diagram_iterator_next( &diagram_iterator, &((*this_).temp_diagram) );
                    root_diag_id = data_diagram_get_row_id( &((*this_).temp_diagram) );
                }
                err_result |= data_diagram_iterator_destroy( &diagram_iterator );
            }

            if ( ! modify_db )
            {
                utf8stream_writer_write_str( out_english_report, "    PROPOSED FIX: Move " );
                utf8stream_writer_write_int( out_english_report, circ_ref_count );
                utf8stream_writer_write_str( out_english_report, " diagrams below root diagram " );
                utf8stream_writer_write_int( out_english_report, root_diag_id );
                utf8stream_writer_write_str( out_english_report, ".\n" );
            }
            else
            {
                for ( int list_pos = 0; list_pos < circ_ref_count; list_pos ++ )
                {
                    data_id_t diagram_id = data_small_set_get_id( &circ_ref, list_pos );
                    data_row_t diagram_row_id = data_id_get_row_id( &diagram_id );

                    data_err = data_database_writer_update_diagram_parent_id ( (*this_).db_writer,
                                                                               diagram_row_id,
                                                                               root_diag_id,
                                                                               NULL /*out_old_diagram*/
                                                                             );
                    if ( U8_ERROR_NONE == data_err )
                    {
                        utf8stream_writer_write_str( out_english_report, "    FIX: Diagram " );
                        utf8stream_writer_write_int( out_english_report, diagram_row_id );
                        utf8stream_writer_write_str( out_english_report, " moved below root diagram " );
                        utf8stream_writer_write_int( out_english_report, root_diag_id );
                        utf8stream_writer_write_str( out_english_report, ".\n" );
                        (*io_fix) ++;
                    }
                    else
                    {
                        utf8stream_writer_write_str( out_english_report, "ERROR WRITING DATABASE.\n" );
                        err_result |= data_err;
                    }
                }
            }
        }
    }
    else
    {
        utf8stream_writer_write_str( out_english_report, "ERROR READING DATABASE.\n" );
        err_result |= data_err;
    }
    data_small_set_destroy( &circ_ref );

    U8_TRACE_END_ERR( err_result );
    return err_result;
}

u8_error_t ctrl_consistency_checker_private_ensure_valid_diagramelements ( ctrl_consistency_checker_t *this_,
                                                                           bool modify_db,
                                                                           uint32_t *io_err,
                                                                           uint32_t *io_fix,
                                                                           utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert ( NULL != io_err );
    assert ( NULL != io_fix );
    assert ( NULL != out_english_report );
    u8_error_t err_result = U8_ERROR_NONE;
    u8_error_t data_err;

    /* write report title */
    utf8stream_writer_write_str( out_english_report, "STEP: Ensure that diagramelements reference valid diagrams and classifiers\n" );

    data_small_set_t unref;
    data_small_set_init( &unref );
    data_err = data_database_consistency_checker_find_nonreferencing_diagramelements ( &((*this_).db_checker), &unref );
    if ( U8_ERROR_NONE == data_err )
    {
        uint32_t unref_count = data_small_set_get_count( &unref );

        utf8stream_writer_write_str( out_english_report, "    NONREFERENCING DIAGRAMELEMENTS COUNT: " );
        utf8stream_writer_write_int( out_english_report, unref_count );
        utf8stream_writer_write_str( out_english_report, "\n" );

        if ( unref_count != 0 )
        {
            (*io_err) += unref_count;

            if ( ! modify_db )
            {
                utf8stream_writer_write_str( out_english_report, "    PROPOSED FIX: Delete " );
                utf8stream_writer_write_int( out_english_report, unref_count );
                utf8stream_writer_write_str( out_english_report, " diagramelements.\n" );
            }
            else
            {
                for ( int list_pos = 0; list_pos < unref_count; list_pos ++ )
                {
                    data_id_t diagramelement_id = data_small_set_get_id( &unref, list_pos );
                    data_row_t diagramelement_row_id = data_id_get_row_id( &diagramelement_id );
                    data_err = data_database_writer_delete_diagramelement ( (*this_).db_writer, diagramelement_row_id, NULL );
                    if ( U8_ERROR_NONE == data_err )
                    {
                        utf8stream_writer_write_str( out_english_report, "    FIX: Diagramelement " );
                        utf8stream_writer_write_int( out_english_report, diagramelement_row_id );
                        utf8stream_writer_write_str( out_english_report, " deleted.\n" );
                        (*io_fix) ++;
                    }
                    else
                    {
                        utf8stream_writer_write_str( out_english_report, "ERROR WRITING DATABASE.\n" );
                        err_result |= data_err;
                    }
                }
            }
        }
    }
    else
    {
        utf8stream_writer_write_str( out_english_report, "ERROR READING DATABASE.\n" );
        err_result |= data_err;
    }

    U8_TRACE_END_ERR( err_result );
    return err_result;
}

u8_error_t ctrl_consistency_checker_private_ensure_valid_diagele_features ( ctrl_consistency_checker_t *this_,
                                                                            bool modify_db,
                                                                            uint32_t *io_err,
                                                                            uint32_t *io_fix,
                                                                            utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert ( NULL != io_err );
    assert ( NULL != io_fix );
    assert ( NULL != out_english_report );
    u8_error_t err_result = U8_ERROR_NONE;
    u8_error_t data_err;

    /* write report title */
    utf8stream_writer_write_str( out_english_report, "STEP: Ensure that diagramelements reference NULL or valid features\n" );

    data_small_set_t unref;
    data_small_set_init( &unref );
    data_err = data_database_consistency_checker_find_invalid_focused_features ( &((*this_).db_checker), &unref );
    if ( U8_ERROR_NONE == data_err )
    {
        uint32_t unref_count = data_small_set_get_count( &unref );

        utf8stream_writer_write_str( out_english_report, "    ILLREFERENCING DIAGRAMELEMENTS COUNT: " );
        utf8stream_writer_write_int( out_english_report, unref_count );
        utf8stream_writer_write_str( out_english_report, "\n" );

        if ( unref_count != 0 )
        {
            (*io_err) += unref_count;

            if ( ! modify_db )
            {
                utf8stream_writer_write_str( out_english_report, "    PROPOSED FIX: Unlink focused features from " );
                utf8stream_writer_write_int( out_english_report, unref_count );
                utf8stream_writer_write_str( out_english_report, " diagramelements.\n" );
            }
            else
            {
                for ( int list_pos = 0; list_pos < unref_count; list_pos ++ )
                {
                    data_id_t diagramelement_id = data_small_set_get_id( &unref, list_pos );
                    data_row_t diagramelement_row_id = data_id_get_row_id( &diagramelement_id );
                    data_err = data_database_writer_update_diagramelement_focused_feature_id ( (*this_).db_writer, diagramelement_row_id, DATA_ROW_VOID, NULL );
                    if ( U8_ERROR_NONE == data_err )
                    {
                        utf8stream_writer_write_str( out_english_report, "    FIX: Focused features unlinked from " );
                        utf8stream_writer_write_int( out_english_report, diagramelement_row_id );
                        utf8stream_writer_write_str( out_english_report, " diagramelements.\n" );
                        (*io_fix) ++;
                    }
                    else
                    {
                        utf8stream_writer_write_str( out_english_report, "ERROR WRITING DATABASE.\n" );
                        err_result |= data_err;
                    }
                }
            }
        }
    }
    else
    {
        utf8stream_writer_write_str( out_english_report, "ERROR READING DATABASE.\n" );
        err_result |= data_err;
    }

    U8_TRACE_END_ERR( err_result );
    return err_result;
}

u8_error_t ctrl_consistency_checker_private_ensure_referenced_classifiers ( ctrl_consistency_checker_t *this_,
                                                                            bool modify_db,
                                                                            uint32_t *io_err,
                                                                            uint32_t *io_fix,
                                                                            utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert ( NULL != io_err );
    assert ( NULL != io_fix );
    assert ( NULL != out_english_report );
    u8_error_t err_result = U8_ERROR_NONE;
    u8_error_t data_err;

    /* write report title */
    utf8stream_writer_write_str( out_english_report, "STEP: Ensure that classifiers are referenced\n" );

    data_small_set_t unref;
    data_small_set_init( &unref );
    data_err = data_database_consistency_checker_find_unreferenced_classifiers ( &((*this_).db_checker), &unref );
    if ( U8_ERROR_NONE == data_err )
    {
        uint32_t unref_count = data_small_set_get_count( &unref );

        utf8stream_writer_write_str( out_english_report, "    UNREFERENCED CLASSIFIER COUNT: " );
        utf8stream_writer_write_int( out_english_report, unref_count );
        utf8stream_writer_write_str( out_english_report, "\n" );

        if ( unref_count != 0 )
        {
            (*io_err) += unref_count;

            if ( ! modify_db )
            {
                utf8stream_writer_write_str( out_english_report, "    PROPOSED FIX: Delete " );
                utf8stream_writer_write_int( out_english_report, unref_count );
                utf8stream_writer_write_str( out_english_report, " classifiers.\n" );
            }
            else
            {
                for ( int list_pos = 0; list_pos < unref_count; list_pos ++ )
                {
                    data_id_t classifier_id = data_small_set_get_id( &unref, list_pos );
                    data_row_t classifier_row_id = data_id_get_row_id( &classifier_id );
                    err_result |= data_database_consistency_checker_kill_classifier ( &((*this_).db_checker), classifier_row_id );
                    if ( U8_ERROR_NONE == err_result )
                    {
                        utf8stream_writer_write_str( out_english_report, "    FIX: Classifier " );
                        utf8stream_writer_write_int( out_english_report, classifier_row_id );
                        utf8stream_writer_write_str( out_english_report, " deleted.\n" );
                        (*io_fix) ++;
                    }
                    else
                    {
                        utf8stream_writer_write_str( out_english_report, "ERROR WRITING DATABASE.\n" );
                    }
                }
            }
        }
    }
    else
    {
        utf8stream_writer_write_str( out_english_report, "ERROR READING DATABASE.\n" );
        err_result |= data_err;
    }

    U8_TRACE_END_ERR( err_result );
    return err_result;
}

u8_error_t ctrl_consistency_checker_private_ensure_valid_feature_parents ( ctrl_consistency_checker_t *this_,
                                                                           bool modify_db,
                                                                           uint32_t *io_err,
                                                                           uint32_t *io_fix,
                                                                           utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert ( NULL != io_err );
    assert ( NULL != io_fix );
    assert ( NULL != out_english_report );
    u8_error_t err_result = U8_ERROR_NONE;
    u8_error_t data_err;

    /* write report title */
    utf8stream_writer_write_str( out_english_report, "STEP: Ensure that features have valid classifiers\n" );

    data_small_set_t unref;
    data_small_set_init( &unref );
    data_err = data_database_consistency_checker_find_unreferenced_features ( &((*this_).db_checker), &unref );
    if ( U8_ERROR_NONE == data_err )
    {
        uint32_t unref_count = data_small_set_get_count( &unref );

        utf8stream_writer_write_str( out_english_report, "    NONREFERENCING FEATURES COUNT: " );
        utf8stream_writer_write_int( out_english_report, unref_count );
        utf8stream_writer_write_str( out_english_report, "\n" );

        if ( unref_count != 0 )
        {
            (*io_err) += unref_count;

            if ( ! modify_db )
            {
                utf8stream_writer_write_str( out_english_report, "    PROPOSED FIX: Delete " );
                utf8stream_writer_write_int( out_english_report, unref_count );
                utf8stream_writer_write_str( out_english_report, " features.\n" );
            }
            else
            {
                for ( int list_pos = 0; list_pos < unref_count; list_pos ++ )
                {
                    data_id_t feature_id = data_small_set_get_id( &unref, list_pos );
                    data_row_t feature_row_id = data_id_get_row_id( &feature_id );
                    data_err = data_database_writer_delete_feature ( (*this_).db_writer, feature_row_id, NULL );
                    if ( U8_ERROR_NONE == data_err )
                    {
                        utf8stream_writer_write_str( out_english_report, "    FIX: Feature " );
                        utf8stream_writer_write_int( out_english_report, feature_row_id );
                        utf8stream_writer_write_str( out_english_report, " deleted.\n" );
                        (*io_fix) ++;
                    }
                    else
                    {
                        utf8stream_writer_write_str( out_english_report, "ERROR WRITING DATABASE.\n" );
                        err_result |= data_err;
                    }
                }
            }
        }
    }
    else
    {
        utf8stream_writer_write_str( out_english_report, "ERROR READING DATABASE.\n" );
        err_result |= data_err;
    }

    U8_TRACE_END_ERR( err_result );
    return err_result;
}

u8_error_t ctrl_consistency_checker_private_ensure_valid_relationship_classifiers ( ctrl_consistency_checker_t *this_,
                                                                                    bool modify_db,
                                                                                    uint32_t *io_err,
                                                                                    uint32_t *io_fix,
                                                                                    utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert ( NULL != io_err );
    assert ( NULL != io_fix );
    assert ( NULL != out_english_report );
    u8_error_t err_result = U8_ERROR_NONE;
    u8_error_t data_err;

    /* write report title */
    utf8stream_writer_write_str( out_english_report, "STEP: Ensure that relationships link valid classifiers\n" );

    data_small_set_t unref;
    data_small_set_init( &unref );
    data_err = data_database_consistency_checker_find_unreferenced_relationships ( &((*this_).db_checker), &unref );
    if ( U8_ERROR_NONE == data_err )
    {
        uint32_t unref_count = data_small_set_get_count( &unref );

        utf8stream_writer_write_str( out_english_report, "    NONREFERENCING RELATIONSHIPS COUNT: " );
        utf8stream_writer_write_int( out_english_report, unref_count );
        utf8stream_writer_write_str( out_english_report, "\n" );

        if ( unref_count != 0 )
        {
            (*io_err) += unref_count;

            if ( ! modify_db )
            {
                utf8stream_writer_write_str( out_english_report, "    PROPOSED FIX: Delete " );
                utf8stream_writer_write_int( out_english_report, unref_count );
                utf8stream_writer_write_str( out_english_report, " relationships.\n" );
            }
            else
            {
                for ( int list_pos = 0; list_pos < unref_count; list_pos ++ )
                {
                    data_id_t relationship_id = data_small_set_get_id( &unref, list_pos );
                    data_row_t relation_row_id = data_id_get_row_id( &relationship_id );
                    data_err = data_database_writer_delete_relationship ( (*this_).db_writer, relation_row_id, NULL );
                    if ( U8_ERROR_NONE == data_err )
                    {
                        utf8stream_writer_write_str( out_english_report, "    FIX: Relationship " );
                        utf8stream_writer_write_int( out_english_report, relation_row_id );
                        utf8stream_writer_write_str( out_english_report, " deleted.\n" );
                        (*io_fix) ++;
                    }
                    else
                    {
                        utf8stream_writer_write_str( out_english_report, "ERROR WRITING DATABASE.\n" );
                        err_result |= data_err;
                    }
                }
            }
        }
    }
    else
    {
        utf8stream_writer_write_str( out_english_report, "ERROR READING DATABASE.\n" );
        err_result |= data_err;
    }

    U8_TRACE_END_ERR( err_result );
    return err_result;
}

u8_error_t ctrl_consistency_checker_private_ensure_valid_relationship_features ( ctrl_consistency_checker_t *this_,
                                                                                 bool modify_db,
                                                                                 uint32_t *io_err,
                                                                                 uint32_t *io_fix,
                                                                                 utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert ( NULL != io_err );
    assert ( NULL != io_fix );
    assert ( NULL != out_english_report );
    u8_error_t err_result = U8_ERROR_NONE;
    u8_error_t data_err;

    /* write report title */
    utf8stream_writer_write_str( out_english_report, "STEP: Ensure that relationships link NULL or valid features\n" );

    data_small_set_t unref;
    data_small_set_init( &unref );
    data_err = data_database_consistency_checker_find_invalid_relationship_features ( &((*this_).db_checker), &unref );
    if ( U8_ERROR_NONE == data_err )
    {
        uint32_t unref_count = data_small_set_get_count( &unref );

        utf8stream_writer_write_str( out_english_report, "    ILLREFERENCING RELATIONSHIPS COUNT: " );
        utf8stream_writer_write_int( out_english_report, unref_count );
        utf8stream_writer_write_str( out_english_report, "\n" );

        if ( unref_count != 0 )
        {
            (*io_err) += unref_count;

            if ( ! modify_db )
            {
                utf8stream_writer_write_str( out_english_report, "    PROPOSED FIX: Delete " );
                utf8stream_writer_write_int( out_english_report, unref_count );
                utf8stream_writer_write_str( out_english_report, " relationships.\n" );
            }
            else
            {
                for ( int list_pos = 0; list_pos < unref_count; list_pos ++ )
                {
                    data_id_t relationship_id = data_small_set_get_id( &unref, list_pos );
                    data_row_t relation_row_id = data_id_get_row_id( &relationship_id );
                    data_err = data_database_writer_delete_relationship ( (*this_).db_writer, relation_row_id, NULL );
                    if ( U8_ERROR_NONE == data_err )
                    {
                        utf8stream_writer_write_str( out_english_report, "    FIX: Relationship " );
                        utf8stream_writer_write_int( out_english_report, relation_row_id );
                        utf8stream_writer_write_str( out_english_report, " deleted.\n" );
                        (*io_fix) ++;
                    }
                    else
                    {
                        utf8stream_writer_write_str( out_english_report, "ERROR WRITING DATABASE.\n" );
                        err_result |= data_err;
                    }
                }
            }
        }
    }
    else
    {
        utf8stream_writer_write_str( out_english_report, "ERROR READING DATABASE.\n" );
        err_result |= data_err;
    }

    U8_TRACE_END_ERR( err_result );
    return err_result;
}


/*
Copyright 2016-2025 Andreas Warnke

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
