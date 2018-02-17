/* File: ctrl_policy_enforcer_test.c; Copyright and License: see below */

#include "ctrl_policy_enforcer_test.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "trace.h"
#include <stdio.h>

static void set_up(void);
static void tear_down(void);
static void diagram_to_lifeline_consistency(void);
static void diagramelement_to_lifeline_consistency(void);

/*!
 *  \brief database instance on which the tests are performed
 */
static data_database_t database;

/*!
 *  \brief database filename on which the tests are performed and which is automatically deleted when finished
 */
static const char* DATABASE_FILENAME = "unittest_crystal_facet_uml_default.cfu.sqlite3";

/*!
 *  \brief database reader to access the database
 */
static data_database_reader_t db_reader;

/*!
 *  \brief database writer to access the database
 */
static data_database_writer_t db_writer;

/*!
 *  \brief controller instance on which the tests are performed
 */
static ctrl_controller_t controller;

TestRef ctrl_policy_enforcer_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("diagram_to_lifeline_consistency",diagram_to_lifeline_consistency),
        new_TestFixture("diagramelement_to_lifeline_consistency",diagramelement_to_lifeline_consistency),
    };
    EMB_UNIT_TESTCALLER(result,"ctrl_policy_enforcer_test",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static void set_up(void)
{
    data_database_init( &database );
    data_database_open( &database, DATABASE_FILENAME );

    data_database_reader_init( &db_reader, &database );
    data_database_writer_init( &db_writer, &db_reader, &database );

    ctrl_controller_init( &controller, &database );
}

static void tear_down(void)
{
    int err;
    ctrl_controller_destroy( &controller );

    data_database_writer_destroy( &db_writer );
    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
    err = remove( DATABASE_FILENAME );
}

static void diagram_to_lifeline_consistency(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;

    /* create two diagrams of type DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
    {
        data_diagram_t new_diagram;

        /*
        ctrl_error_t ctrl_diagram_controller_create_diagram ( ctrl_diagram_controller_t *this_,
                                                              const data_diagram_t *new_diagram,
                                                              bool add_to_latest_undo_set,
                                                              int64_t* out_new_id
        );
        */

    }

    /* create a classifier of type DATA_CLASSIFIER_TYPE_UML_CLASS */
    {

    }

    /* create two diagramelements, one for each diagram */
    {

    }

    /* change the type of first diagram to DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    {

    }

    /* check that the classifier now has a feature of type DATA_FEATURE_TYPE_LIFELINE */
    {

    }

    /* delete the first diagram (but not the classifier) */
    {

    }

    /* check that the feature of type DATA_FEATURE_TYPE_LIFELINE is deleted */
    {

    }
}

static void diagramelement_to_lifeline_consistency(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;

    /* create a diagram of type DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    {

    }

    /* create a classifier of type DATA_CLASSIFIER_TYPE_UML_CLASS */
    {

    }

    /* create two diagramelements for the classifier */
    {

    }

    /* check that the classifier now has two features of type DATA_FEATURE_TYPE_LIFELINE */
    {

    }

    /* delete the first diagramelement (but not the classifier) */
    {

    }

    /* check that one feature of type DATA_FEATURE_TYPE_LIFELINE is deleted */
    {

    }
}


/*
 * Copyright 2018-2018 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
