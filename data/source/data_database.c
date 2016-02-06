/* File: data_database.c; Copyright and License: see below */

#include "data_database.h"
#include "trace.h"
#include "log.h"

static const char *DATA_DATABASE_CREATE_METAOBJECT_TABLE = "CREATE TABLE IF NOT EXISTS metaobjects ( id INTEGER PRIMARY KEY ASC, "
                                                        "maintype INTEGER, stereotype TEXT, name TEXT, description TEXT );";
static const char *DATA_DATABASE_CREATE_METAASSOCIATION_TABLE = "CREATE TABLE IF NOT EXISTS metaassociations ( id INTEGER PRIMARY KEY ASC, "
                                                        "type INTEGER, from_metaobject_id INTEGER, to_metaobject_id INTEGER, name TEXT, "
							"FOREIGN KEY(from_metaobject_id) REFERENCES metaobjects(id), "
							"FOREIGN KEY(to_metaobject_id) REFERENCES metaobjects(id) );";
static const char *DATA_DATABASE_CREATE_METAATTRIBUTE_TABLE = "CREATE TABLE IF NOT EXISTS metaattributes ( id INTEGER PRIMARY KEY ASC, "
                                                        "metaobject_id INTEGER, key TEXT, value TEXT );";
static const char *DATA_DATABASE_CREATE_DIAGRAM_TABLE = "CREATE TABLE IF NOT EXISTS diagrams ( id INTEGER PRIMARY KEY ASC, "
                                                        "parent_id INTEGER, type INTEGER, name TEXT, "
							"FOREIGN KEY(parent_id) REFERENCES diagrams(id) );";
static const char *DATA_DATABASE_CREATE_DIAGRAM_ELEMENTS_TABLE = "CREATE TABLE IF NOT EXISTS diagram_elements ( id INTEGER PRIMARY KEY ASC, "
                                                        "diagram_id INTEGER, metaobject_id INTEGER, "
							"FOREIGN KEY(diagram_id) REFERENCES diagrams(id), "
							"FOREIGN KEY(metaobject_id) REFERENCES metaobjects(id) );";

/*!
 *  \brief initializes the tables in the database if not yet existant
 */
static void data_database_initialize_tables( sqlite3 *db );
static void data_database_initialize_tables( sqlite3 *db )
{
    TRACE_BEGIN();
    int sqlite_err;
    
    sqlite_err = sqlite3_exec( db, DATA_DATABASE_CREATE_METAOBJECT_TABLE, NULL, NULL, NULL );
    if ( SQLITE_OK != sqlite_err ) 
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_CREATE_METAOBJECT_TABLE );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
    }
    
    sqlite_err = sqlite3_exec( db, DATA_DATABASE_CREATE_METAASSOCIATION_TABLE, NULL, NULL, NULL );
    if ( SQLITE_OK != sqlite_err ) 
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_CREATE_METAASSOCIATION_TABLE );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
    }
    
    sqlite_err = sqlite3_exec( db, DATA_DATABASE_CREATE_METAATTRIBUTE_TABLE, NULL, NULL, NULL );
    if ( SQLITE_OK != sqlite_err ) 
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_CREATE_METAATTRIBUTE_TABLE );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
    }
  
    sqlite_err = sqlite3_exec( db, DATA_DATABASE_CREATE_DIAGRAM_TABLE, NULL, NULL, NULL );
    if ( SQLITE_OK != sqlite_err ) 
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_CREATE_DIAGRAM_TABLE );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
    }
    
    sqlite_err = sqlite3_exec( db, DATA_DATABASE_CREATE_DIAGRAM_ELEMENTS_TABLE, NULL, NULL, NULL );
    if ( SQLITE_OK != sqlite_err ) 
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_CREATE_DIAGRAM_ELEMENTS_TABLE );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
    }
    
    TRACE_END();
}

void data_database_init ( data_database_t *this_ )
{
    TRACE_BEGIN();
    int sqlite_err;
    
    (*this_).db_file_name = "crystal_facet_uml_default.cfu.sqlite3";
    
    sqlite_err = sqlite3_open( (*this_).db_file_name, &((*this_).db) );
    if ( SQLITE_OK != sqlite_err ) 
    {
        LOG_ERROR_INT( "sqlite3_open() failed:", sqlite_err );
        (*this_).is_open = false;
    }
    else
    {
        (*this_).is_open = true;
	data_database_initialize_tables( (*this_).db );
    }
    
    TRACE_END();
}

void data_database_destroy ( data_database_t *this_ )
{
    TRACE_BEGIN();
    int sqlite_err;

    (*this_).db_file_name = "";
    (*this_).is_open = false;

    sqlite_err = sqlite3_close( (*this_).db );
    if ( SQLITE_OK != sqlite_err ) 
    {
        LOG_ERROR_INT( "sqlite3_close() failed:", sqlite_err );
    }

    TRACE_END();
}


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
