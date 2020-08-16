////////////////////////////////////////////////////////////////////////
//
// File: SQLMigrate.cpp
//
// Copyright (c) 1998-2020 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Version number: See SQLComponents.h
//
#include "StdAfx.h"
#include "SQLMigrate.h"
#include "SQLMigrateApp.h"
#include "SQLMigrateDialog.h"
#include "SQLQuery.h"
#include "SQLInfoDB.h"
#include <sql.h>
#include <sqlext.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace SQLComponents
{

SQLMigrate::SQLMigrate(MigrateParameters& p_params,Logging& m_log)
           :m_params(p_params)
           ,m_log(m_log)
           ,m_databaseSource(nullptr)
           ,m_databaseTarget(nullptr)
{
  m_directMigration = 0;
  m_totalTables     = 0;
}

SQLMigrate::~SQLMigrate()
{
  if(m_databaseSource)
  {
    delete m_databaseSource;
    m_databaseSource = nullptr;
  }
  if(m_databaseTarget)
  {
    delete m_databaseTarget;
    m_databaseTarget = nullptr;
  }
}

bool 
SQLMigrate::Migrate()
{
  bool result = false;

  m_totalTables     = 0;
  m_directMigration = m_params.v_direct;

  m_log.SetScript    (m_params.v_createscript);
  m_log.SetDropScript(m_params.v_dropscript);

  m_databaseSource = new SQLDatabase();
  m_databaseTarget = new SQLDatabase();

  CString error;
  CString connectSource;
  CString connectTarget;
  connectSource.Format("DSN=%s;UID=%s;PWD=%s;", m_params.v_source_dsn, m_params.v_source_user, m_params.v_source_password);
  connectTarget.Format("DSN=%s;UID=%s;PWD=%s;", m_params.v_target_dsn, m_params.v_target_user, m_params.v_target_password);

  try
  {
    // Always connect to the source as a minimum
    //             "--------------------: "
    m_log.WriteLog("Make connection with: " + m_params.v_source_dsn);
    m_log.WriteLog("Logged in user      : " + m_params.v_source_user);
    if(m_databaseSource->Open(connectSource,true) == false)  // Read-only connect
    {
      error.Format("Cannot make connection with: " + m_params.v_source_dsn);
      m_log.WriteLog(error);
      AfxMessageBox (error,MB_OK|MB_ICONERROR);
      return false;
    }
    m_log.SetDBType(true,m_databaseSource->GetDatabaseTypeName());
    m_params.v_sourceType = m_databaseSource->GetDatabaseType();

    // And always make connection to the target database (also when not directly converting)
    // This is necessary to read in the target database capabilities
    m_log.WriteLog("Make connection with: " + m_params.v_target_dsn);
    m_log.WriteLog("Logged in user      : " + m_params.v_target_user);
    if(m_databaseTarget->Open(connectTarget) == false)
    {
      error.Format("Cannot make connection with: " + m_params.v_target_dsn);
      m_log.WriteLog(error);
      AfxMessageBox (error,MB_OK|MB_ICONERROR);
      return false;
    }

    // Try to use the target schema
    bool defschema = m_databaseTarget->SetDefaultSchema(m_params.v_target_schema);
    m_log.WriteLog("Connect to schema   : " + (defschema ? m_params.v_target_schema : "NO SCHEMA!!"));

    m_log.SetDBType(false,m_databaseTarget->GetDatabaseTypeName());
    m_params.v_targetType  = m_databaseTarget->GetDatabaseType();
    m_log.WriteLog("");

    // Find the number of tables to migrate (and which tables)
    m_log.WriteLog("Tables to migrate   : " + m_params.v_table);
    m_totalTables = ReadTableStructures(m_params.v_source_user,m_params.v_table,m_databaseSource);
    CString num;
    num.Format("%d",m_totalTables);
    m_log.WriteLog("Tables to migrate   : " + num);

    // Register the number of tables for the gauges
    m_log.SetTables(m_totalTables);

    if(m_totalTables == 0)
    {
      // Check if nothing to do
      error = "No tables to migrate";
      m_log.WriteLog(error);
      AfxMessageBox(error,MB_OK|MB_ICONERROR);
    }
    else
    {
      if(m_params.v_do_tables)
      {
        DropTables();
      }

      if(m_params.v_do_tables || m_params.v_primarys  || m_params.v_indices   ||
         m_params.v_foreigns  || m_params.v_sequences || m_params.v_triggers  ||
         m_params.v_access    )
      {
        CreateTables();
      }
      
      if(m_params.v_do_data)
      {
        // If minimum object-ID requested: do not truncate the contents
        // CXHibernate and network databases uses a "OID" primary key
        if(m_params.v_deletes && m_params.v_minOid == 0)
        {
          TruncateTables();
        }

        switch(m_directMigration)
        {
          case 0: // HIGH PERFORMANCE DATAPUMP 
                  FillTablesViaPump(); 
                  break;
          case 1: // Fill database via SELECT/INSERT (Works always, but slower)
                  FillTablesViaData(true);
                  break;
          case 2: // Fill Script files. Migrate later by running these scripts
                  FillTablesViaData(false);
                  break;
        }
      }
    }
    // Closing the databases again
    m_databaseSource->Close();
    m_databaseTarget->Close();

    // Clear tables list
    m_tables.clear();

    // Reached the end
    result = true;
  }
  catch (StdException& ex)
  {
    m_log.WriteLog("ERROR in migration: " + ex.GetErrorMessage());
    return false;
  }
  catch(...)
  {
    m_log.WriteLog("Unknown error in migration");
    return false;
  }

  if(m_directMigration == 2)
  {
    m_log.WriteDrop("-- End of script");
    m_log.WriteOut ("-- End of script");
  }

  CString resultline;
  resultline.Format("Result of the migration: %d Errors.",m_params.v_errors);
  m_log.WriteLog("");
  m_log.WriteLog(resultline);
  m_log.WriteLog("*** End of migration. ***");

  return result;
}

//////////////////////////////////////////////////////////////////////////
//
// READING THE TABLES TO CONVERT
//
//////////////////////////////////////////////////////////////////////////

int
SQLMigrate::ReadTableStructures(CString p_owner,CString p_pattern,SQLDatabase* p_database)
{
  CString errors;

  // Set status line of current step
  m_log.SetStatus("Read table structures");

  if(p_pattern.IsEmpty())
  {
    m_log.WriteLog("To migrate          : ALL TABLES");
    p_database->GetSQLInfoDB()->MakeInfoTableTable(m_tables,errors,p_owner,p_pattern);
  }
  else if(p_pattern.Right(1) == "%")
  {
    m_log.WriteLog("To migrate          : Selection from source database");
    p_database->GetSQLInfoDB()->MakeInfoTableTable(m_tables,errors,p_owner,p_pattern);
  }
  else if(p_pattern.Right(4).CompareNoCase(".txt") == 0)
  {
    m_log.WriteLog("To migrate          : Selection from text file");
    ReadTablesFromFile(p_pattern);
  }
  else
  {
    // If all else fails: It MUST be a table name
    m_log.WriteLog("One table with name : " + p_pattern);
    MetaTable table;
    table.m_schema = p_owner;
    table.m_table  = p_pattern;
    m_tables.push_back(table);
  }
  return (int) m_tables.size();
}

void
SQLMigrate::ReadTablesFromFile(CString& p_file)
{
  char buffer[101];
  FILE* file = nullptr;
  fopen_s(&file,p_file,"r");
  if(file)
  {
    while(fgets(buffer,100,file))
    {
      int len = (int) strlen(buffer);
      if(len && buffer[len-1] == '\n')
      {
        buffer[len-1] = 0;
      }
      MetaTable table;
      table.m_table = buffer;
      m_tables.push_back(table);
    }
    fclose(file);
  }
  else
  {
    CString msg;
    msg.Format("Cannot open the text file with table names: %s",p_file);
    AfxMessageBox(msg,MB_OK|MB_ICONERROR);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// DROP   THE TARGET TABLES in the TARGET DATABASE
// CREATE THE TARGET TABLES in the TARGET DATABASE
//
//////////////////////////////////////////////////////////////////////////

void
SQLMigrate::DropTables()
{
  CString name("drop tabel");
  int numTables = 0;
  CString vendor = m_databaseTarget->GetDatabaseTypeName();

  CString comment("-- ");
  CString header1("DROPPING THE TABLES IN THE TARGET DATABASE");
  CString header2("==========================================");

  m_log.WriteLog("");
  m_log.WriteLog(header1);
  m_log.WriteLog(header2);

  if(m_directMigration == 2)
  {
    m_log.WriteDrop(comment + header1);
    m_log.WriteDrop(comment + header2);
    m_log.WriteDrop("");
  }

  // Set status for current step
  m_log.SetStatus("Dropping tables");

  for(unsigned int ind = 0; ind < m_tables.size(); ++ind)
  {
    CString table     = m_tables[ind].m_table;
    CString statement = m_databaseTarget->GetSQLInfoDB()->GetCATALOGTableDrop(m_params.v_target_schema,table,true,true,false);

    // Process the statement
    m_log.WriteLog(statement);
    if(m_directMigration < 2)
    {
      try
      {
        SQLQuery query(m_databaseTarget);
        SQLTransaction trans(m_databaseTarget,"Drop");
        query.DoSQLStatement(statement);
        trans.Commit();
        CommitDDL(query,m_databaseTarget->GetSQLInfoDB());
      }
      catch(StdException& ex)
      {
        m_log.WriteLog("ERROR : Dropping of table failed.");
        m_log.WriteLog(ex.GetErrorMessage());
        ++m_params.v_errors;
      }
    }
    else
    {
      m_log.WriteDrop(statement,true);
    }
    // Record number of tables in gouge
    m_log.SetTablesGauge(++numTables);
  }
}

void
SQLMigrate::CreateTables()
{
  CString name("Create table");
  int  numTables = 0;
  CString vendor = m_databaseTarget->GetDatabaseTypeName();

  CString comment("-- ");
  CString header1("CREATING TABLES IN THE TARGET DATABASE");
  CString header2("======================================");

  m_log.WriteLog("");
  m_log.WriteLog(header1);
  m_log.WriteLog(header2);

  if(m_directMigration == 2)
  {
    m_log.WriteOut(comment + header1);
    m_log.WriteOut(comment + header2);
    m_log.WriteOut("");
  }

  // Set status of current step
  m_log.SetStatus("Creating tables");

  // Try to get the optimal result
  SQLInfoDB* source = m_databaseSource->GetSQLInfoDB();
  SQLInfoDB* target = m_databaseTarget->GetSQLInfoDB();
  source->SetPreferODBC(false);
  target->SetPreferODBC(false);

  for(unsigned int ind = 0; ind < m_tables.size(); ++ind)
  {
    DDLCreateTable create(source);

    // Getting the table to migrate
    CString tablename = m_tables[ind].m_table;
    if (!m_params.v_source_schema.IsEmpty() && source->GetRDBMSUnderstandsSchemas())
    {
      tablename = m_params.v_source_schema + "." + tablename;
    }

    // Getting the table info (and throw away the results!)
    create.GetTableStatements(tablename);

    // If we migrate to another type, PSM cannot be converted
    MTriggerMap emptyTriggers;
    if(m_databaseSource->GetDatabaseType() != m_databaseTarget->GetDatabaseType())
    {
      create.SetTableInfoTrigger(emptyTriggers);
    }

    // Optional parts: If not set. empty out the info again
    MPrimaryMap emptyPrimary;
    if (!m_params.v_primarys)
    {
      create.SetTableInfoPrimary(emptyPrimary);
    }
    MIndicesMap emptyIndices;
    if (!m_params.v_indices)
    {
      create.SetTableInfoIndices(emptyIndices);
    }
    MForeignMap emptyForeigns;
    if (!m_params.v_foreigns)
    {
      create.SetTableInfoForeign(emptyForeigns);
    }
    MSequenceMap emptySequences;
    if (!m_params.v_sequences)
    {
      create.SetTableInfoSequence(emptySequences);
    }
    if (!m_params.v_triggers)
    {
      create.SetTableInfoTrigger(emptyTriggers);
    }
    MPrivilegeMap emptyPrivileges;
    if (!m_params.v_access)
    {
      create.SetTableInfoPrivilege(emptyPrivileges);
    }

    // Set the target database
    create.SetInfoDB(target);

    // Create target database table name
    CString targettable = m_tables[ind].m_table;
    if(!m_params.v_target_schema.IsEmpty() && target->GetRDBMSUnderstandsSchemas())
    {
      targettable = m_params.v_target_schema + "." + targettable;
    }

    // Tweak the columns from source to target database !!
    if(source->GetRDBMSDatabaseType() != target->GetRDBMSDatabaseType())
    {
      FixupTableColumns(create);
      FixupTableIndices(create);
    }

    // Set the schema/tablespace
    create.SetTablesSchema   (m_params.v_target_schema);
    create.SetTableTablespace(m_params.v_tablespace);
    create.SetIndexTablespace(m_params.v_tablespace);

    // Re-Create the table in the target database type
    target->SetPreferODBC(true);
    DDLS statements = create.GetTableStatements(targettable);

    // Now hold on the first statement, if no re-create tables
    if(m_params.v_do_tables == false)
    {
      statements.pop_front();
    }

    // Do something with the statement
    for(auto& statement : statements)
    {
      m_log.WriteLog(statement);
      if(m_directMigration < 2)
      {
        try
        {
          SQLQuery query(m_databaseTarget);
          SQLTransaction trans(m_databaseTarget,"CreateTable");
          query.DoSQLStatement(statement);
          trans.Commit();
        }
        catch(StdException& ex)
        {
          m_log.WriteLog("ERROR: Table creation failed");
          m_log.WriteLog(ex.GetErrorMessage());
          m_params.v_errors++;
        }
      }
      else
      {
        statement.Replace("\r","");
        m_log.WriteOut(statement,true);
      }
    }
    // Show number of tables
    m_log.SetTablesGauge(++numTables);
  }
}

void
SQLMigrate::FixupTableColumns(DDLCreateTable& p_create)
{
  SQLInfoDB* source = m_databaseSource->GetSQLInfoDB();
  SQLInfoDB* target = m_databaseTarget->GetSQLInfoDB();

  for(auto& column : p_create.m_columns)
  {
    // Fix the datatype for the target database
    target->GetKEYWORDDataType(&column);

    // Fix default values
    if(!column.m_default.IsEmpty())
    {
      if(column.m_default.CompareNoCase(source->GetKEYWORDCurrentDate()) == 0)
      {
        column.m_default = target->GetKEYWORDCurrentDate();
      }
      else if(column.m_default.CompareNoCase(source->GetKEYWORDCurrentTimestamp()) == 0)
      {
        column.m_default = target->GetKEYWORDCurrentTimestamp();
      }
      else if(column.m_default.Find('(') > 0)
      {
        // HACK: Functions cannot be converted
        // Incompatible: Make empty, but WARN the user
        CString warning;
        warning.Format("-- WARNING: Default on column [%s] removed: %s",column.m_column.GetString(),column.m_default.GetString());
        m_log.WriteLog(warning);

        // Must remove the default
        column.m_default.Empty();
      }
    }
  }
}

void
SQLMigrate::FixupTableIndices(DDLCreateTable& p_create)
{
  SQLInfoDB* source = m_databaseSource->GetSQLInfoDB();
  SQLInfoDB* target = m_databaseTarget->GetSQLInfoDB();

  for(auto& index : p_create.m_indices)
  {
    if(target->GetRDBMSSupportsFunctionalIndexes() == false)
    {
      if(!index.m_filter.IsEmpty())
      {
        // AUCH: Source table has a function index, but we do not support it
        //       Try to get the first column name from the index
        //       It's not the same, but for UPPER/LOWER functionals it works!!
        int pos1 = index.m_filter.Find('(');
        int pos2 = index.m_filter.Find(')');
        if(pos1 >= 0 && pos2 >= 0 && pos2 > pos1)
        {
          CString naam = index.m_filter.Mid(pos1+1,(pos2 - pos1 - 1));
          naam = naam.Trim(source->GetIdentifierQuoteCharacter());
          index.m_columnName = naam;

          CString warning;
          warning.Format("-- WARNING: Functional index removed from index [%s] : %s", index.m_indexName.GetString(),index.m_filter.GetString());
          m_log.WriteLog(warning);

          // Must remove the filter
          index.m_filter.Empty();
        }
      }
    }
  }
}


//////////////////////////////////////////////////////////////////////////\
//
// MAKE TABLES INITIALLY EMPTY
//
//////////////////////////////////////////////////////////////////////////

void
SQLMigrate::TruncateTables()
{
  CString name("delete from table");
  int numTables = 0;

  CString text;
  CString comment("-- ");
  CString header1("EMPTY THE TABLES IN THE TARGET DATABASE");
  CString header2("=======================================");

  m_log.WriteLog("");
  m_log.WriteLog(header1);
  m_log.WriteLog(header2);

  if(m_directMigration == 2)
  {
    m_log.WriteOut(comment + header1);
    m_log.WriteOut(comment + header2);
    m_log.WriteOut("");
  }

  // Current status
  m_log.SetStatus("Empty the tables");

  for(unsigned int ind = 0; ind < m_tables.size(); ++ind)
  {
    CString table = m_tables[ind].m_table;
    if(!m_params.v_target_schema.IsEmpty() && m_databaseTarget->GetSQLInfoDB()->GetRDBMSUnderstandsSchemas())
    {
      table = m_params.v_target_schema + "." + table;
    }

    CString statement = "DELETE FROM " + table;
    // Process result
    m_log.WriteLog(statement);
    if(m_directMigration < 2)
    {
      try
      {
        SQLQuery query(m_databaseTarget);
        SQLTransaction trans(m_databaseTarget,name);
        query.DoSQLStatement(statement);
        int rows = query.GetNumberOfRows();
        trans.Commit();

        text.Format("Number of rows removed: [%d]",rows);
        m_log.WriteLog(text);
      }
      catch(StdException& ex)
      {
        m_log.WriteLog("ERROR: Failed to empty the table");
        m_log.WriteLog(ex.GetErrorMessage());
        m_params.v_errors++;
      }
    }
    else
    {
      m_log.WriteOut(statement,true);
    }
    // Show number of tables
    m_log.SetTablesGauge(++numTables);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// FILLING THE TABLES
//
//////////////////////////////////////////////////////////////////////////

void
SQLMigrate::FillTablesViaPump()
{
  int  numTables = 0;

  m_log.WriteLog("");
  m_log.WriteLog("MIGRATING TABLE CONTENT TO TARGET DATABASE");
  m_log.WriteLog("==========================================");

  // Current step
  m_log.SetStatus("Migrating contents");

  // Create rebind mapping with known exceptions
  RebindMap rebinds;
  DatatypeExceptions(rebinds);

  for(unsigned int ind = 0; ind < m_tables.size(); ++ind)
  {
    CString table  = m_tables[ind].m_table;
    CString schema = m_params.v_target_schema;

    long rows    = 0;
    long totrows = 0;
    int  missing = 0;

    // Start processing
    clock_t start = clock();
    clock_t einde = 0;

    try
    {
      CString text;
      text.Format("Migrating table     : %s", table);
      m_log.WriteLog(text);

      totrows = CountTableContents(schema,table);
      text.Format("Total number of rows: %ld", totrows);
      m_log.WriteLog(text);

      if(totrows)
      {
        SQLQuery query1(m_databaseSource);
        SQLQuery query2(m_databaseTarget);

        // Source->Target database datatype migration
        CString select = MakeSelectStatement(table,m_params.v_source_schema);
        CString insert = MakeInsertStatement(table,m_params.v_source_schema,m_params.v_target_schema);

        SQLTransaction trans(m_databaseTarget,"Migration");

        // Prepare the query objects with the correct rebind mappings
        query1.SetRebindMap(&rebinds);
        query2.SetRebindMap(&rebinds);

        // Prepare the queries, including the rebinds
        query1.DoSQLPrepare(select);
        query2.DoSQLPrepare(insert);
   
        // There we go!
        query1.DoSQLExecute();

        // Bind output from query1 to the input of query2
        query2.SetParameters((VarMap*)query1.GetBoundedColumns());
        query2.BindParameters();

        bool ready = false;
        while(!ready)
        {
          try
          {
            if(query1.GetRecord())
            {
              if(m_params.v_truncate)
              {
                // Informix data field truncate
                query1.TruncateCharFields();
                // Most databases do not support TIMSTAMP fractions
                query1.TruncateTimestamps();
              }
              query2.DoSQLExecute();

              // Show progress
              m_log.SetTableGauge(++rows,totrows);
              if(rows % m_params.v_logLines == 0)
              {
                CString meld;
                meld.Format("Table: %s Rows: %ld [%5.2f %%]",table,rows,((double) rows / (double)totrows * 100.0));
                m_log.WriteLog(meld);
              }
            }
            else
            {
              // Read past last record in the select
              ready = true;
            }
          }
          catch(StdException& ex)
          {
            // Show missing records for first 100 rows
            if(++missing < 100)
            {
              LogMissingRecord(query1,ex.GetErrorMessage());
            }
            else if(missing == 100)
            {
              m_log.WriteLog("");
              m_log.WriteLog("More than 100 missing records. Other records are not individually shown in the logfile!!");
              m_log.WriteLog("");
            }
            ++m_params.v_errors;
          }
        }
        query2.SetParameters(nullptr);
        trans.Commit();
      }
    }
    catch(StdException& ex)
    {
      m_log.WriteLog("ERROR: Migration of table has failed");
      m_log.WriteLog(ex.GetErrorMessage());
    }
    catch(...)
    {
      m_log.WriteLog("ERROR: Migration of table has failed");
      m_log.WriteLog("Unknown error while migrating");
    }


    // Show tables
    m_log.SetTablesGauge(++numTables);
    CString overgezet;
    overgezet.Format("Table [%s.%s] migrated with [%ld of %ld] rows",m_params.v_source_user,table,rows,totrows);
    m_log.WriteLog(overgezet);
    // Extra missing records log
    if(rows < totrows)
    {
      overgezet.Format("MISSING: Table [%s.%s] missing rows: %d",m_params.v_source_user,table,(totrows - rows));
      m_log.WriteLog(overgezet);
    }
    // End in the logfile
    einde = clock();
    overgezet.Format("Table [%s.%s] Total processing time: %.2f seconds",m_params.v_source_user,table,(double)(einde - start) / CLOCKS_PER_SEC);
    m_log.WriteLog(overgezet);
  }
}

CString
SQLMigrate::MakeSelectStatement(CString& p_tabel,CString& p_user)
{
  CString    statement("SELECT ");
  MColumnMap columns;
  CString    schema;
  CString    errors;

  m_databaseSource->GetSQLInfoDB()->MakeInfoTableColumns(columns,errors,p_user,p_tabel);
  for(unsigned int regel = 0; regel < columns.size(); ++regel)
  {
    MetaColumn* info = &(columns[regel]);

    if(regel > 0) 
    {
      statement += ",";
    }
    //     // DIRTY HACK!!
    //     // NASTY CONVERSION OF INFORMIX INTERVAL columns
    //     bool interval = _strnicmp(info->m_typename,"INTERVAL",8) == 0;
    //     if(interval)
    //     {
    //       statement += CString("TRIM(") + info->m_column + "||\'\') as ";
    //     }
    statement += info->m_column;
  }
  statement += " FROM ";
  if(!p_user.IsEmpty())
  {
    statement += p_user + ".";
  }
  statement += p_tabel;

  // Optional begin on the object-id primary key of the table
  // Used in CXHibernate and network databases
  if(m_params.v_minOid)
  {
    statement.AppendFormat("\n WHERE oid >= %d",m_params.v_minOid);
  }
  return statement;
}

CString
SQLMigrate::MakeInsertStatement(CString& p_tabel,CString& p_user,CString& p_doel_user)
{
  CString    data;
  CString    statement("INSERT INTO ");
  MColumnMap columns;
  CString    schema;
  CString    errors;

  statement += p_doel_user + ".";
  statement += p_tabel + " (";

  m_databaseSource->GetSQLInfoDB()->MakeInfoTableColumns(columns,errors,p_user,p_tabel);
  for(unsigned int regel = 0; regel < columns.size(); ++regel)
  {
    MetaColumn* info = &(columns[regel]);
    if(regel > 0) 
    {
      statement += ",";
      data      += ",";
    }
    statement += info->m_column;
    data      += "?";
  }
  statement += ") VALUES (" + data + ")";
  return statement;
}

long
SQLMigrate::CountTableContents(CString p_owner,CString& tabel)
{
  CString  name("count");
  SQLQuery query(m_databaseSource);
  CString  statement = "SELECT COUNT(*) FROM ";

  if(m_databaseTarget->GetSQLInfoDB()->GetRDBMSUnderstandsSchemas())
  {
    statement += p_owner + ".";
  }
  statement += tabel;

  // Optional begin on the primary key of the table
  if(m_params.v_minOid)
  {
    statement.AppendFormat("\n WHERE oid >= %d", m_params.v_minOid);
  }

  query.DoSQLStatement(statement);
  if(query.GetRecord())
  {
    return (long) query[1];
  }
  return 0;
}

void
SQLMigrate::LogMissingRecord(SQLQuery& p_query,CString& p_error)
{
  CString text;
  m_log.WriteLog("MISSING RECORD");
  // Reason for the error
  text.Format("ERROR: %s",p_error);
  m_log.WriteLog(text);

  int aantal = p_query.GetNumberOfColumns();
  for(int ind = 1; ind <= aantal; ++ind)
  {
    CString naam;
    CString waarde;
    SQLVariant* var = p_query.GetColumn(ind);
    var->GetAsString(waarde);
    p_query.GetColumnName(ind,naam);

    text.Format("%d: %s [%s]",ind,naam,waarde);
    m_log.WriteLog(text);
  }
}

void
SQLMigrate::CommitDDL(SQLQuery& p_query,SQLInfoDB* p_info)
{
  if(p_info->GetRDBMSMustCommitDDL())
  {
    CString statement("COMMIT");
    CString name("Commit DDL");
    p_query.DoSQLStatement(statement);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// ALTERNATIVE (OLD) WAY OF INSERTING DATA
//
//////////////////////////////////////////////////////////////////////////

void
SQLMigrate::FillTablesViaData(bool p_process)
{
  int numTables = 0;
  CString name("Select");
  CString comment("-- ");
  CString header1("TABLE CONTENTS MIGRATION TO INSERT SCRIPTS");
  CString header2("==========================================");

  m_log.WriteLog("");
  m_log.WriteLog(header1);
  m_log.WriteLog(header2);

  m_log.WriteOut(comment + header1);
  m_log.WriteOut(comment + header2);
  m_log.WriteOut("");

  // Current step to status line
  m_log.SetStatus("Migrate table content");

  for(unsigned int ind = 0; ind < m_tables.size(); ++ind)
  {
    long rows      = 0;
    long totalrows = 0;

    // Get pattern from source database
    // TABLE: catalog:schema.table 
    CString table = m_tables[ind].m_table;

    // Process
    clock_t start = clock(), einde;
    try
    {
      m_log.WriteOut("");
      m_log.WriteOut("-- TABLE: " + table);
      m_log.WriteOut("");

      // Get the columns
      CString    errors;
      CString    schema;
      MColumnMap columns;

      m_databaseSource->GetSQLInfoDB()->MakeInfoTableColumns(columns,errors,m_params.v_source_schema,table);

      totalrows = CountTableContents(m_params.v_source_user,table);
      if(totalrows)
      {
        CString meld;
        meld.Format("Migrating table    : %s",table);
        m_log.WriteLog(meld);
        meld.Format("Number of records  : %ld",totalrows);
        m_log.WriteLog(meld);

        SQLQuery query1(m_databaseSource);
        SQLQuery query2(m_databaseTarget);
        SQLTransaction trans(m_databaseSource,"Migration");

        CString select = MakeSelectStatement(table,m_params.v_source_schema);
        query1.DoSQLStatement(select);

        while(query1.GetRecord())
        {
          CString insert;
          int extra = 0;

          try
          {
            insert = MakeInsertDataStatement(table,m_params.v_target_schema,query1,columns);
            if (p_process)
            {
              query2.DoSQLStatement(insert);
            }
            else
            {
              m_log.WriteOut(insert, true);
            }
            extra = 1;
          }
          catch(StdException& ex)
          {
            LogMissingRecord(query1,ex.GetErrorMessage());
            m_log.WriteLog(insert);
            ++m_params.v_errors;
          }

          // Next row
          rows += extra;

          // Mark progress in gauge
          m_log.SetTableGauge(rows,totalrows);
          if(rows % m_params.v_logLines == 0)
          {
            CString text;
            text.Format("Table: %s Rows: %ld [%5.2f %%]",table,rows,((double) rows / (double)totalrows * 100.0));
            m_log.WriteLog(text);
          }
        }
        trans.Commit();
      }
    }
    catch(StdException& ex)
    {
      m_log.WriteLog("ERROR: Failed to list table content");
      m_log.WriteLog(ex.GetErrorMessage());
    }
    catch(...)
    {
      m_log.WriteLog("ERROR: Failed to list table content");
      m_log.WriteLog("Unknown error at migration");
    }
    // Show tables
    m_log.SetTablesGauge(++numTables);
    CString processed;
    processed.Format("Table [%s.%s] processed with [%ld of %ld] rows",m_params.v_source_user,table,rows,totalrows);
    m_log.WriteLog(processed);
    // Extra missing rows in logfile
    if(rows < totalrows)
    {
      processed.Format("MISSING ROWS: Table [%s.%s] total missing rows: %d",m_params.v_source_user,table,(totalrows - rows));
      m_log.WriteLog(processed);
    }
    // End in logfile
    einde = clock();
    processed.Format("Table [%s.%s] Total running time: %.2f seconds",m_params.v_source_user,table,(double)(einde - start) / CLOCKS_PER_SEC);
    m_log.WriteLog(processed);
  }
}

CString
SQLMigrate::MakeInsertDataStatement(CString& p_table,CString& p_target_schema,SQLQuery& p_input,MColumnMap& columns)
{
  CString data;
  CString statement("INSERT INTO ");
  bool seperator = false;

  if(!p_target_schema.IsEmpty())
  {
    statement += p_target_schema + ".";
  }
  statement += p_table + " (";

  for(unsigned int row = 0; row < columns.size(); ++row)
  {
    MetaColumn* info = &(columns[row]);
    SQLVariant* var  = p_input.GetColumn(row + 1);
    CString datum    = VariantToInsertString(var,info->m_datatype);
  
    if(!datum.IsEmpty() && datum != "\'\'")
    {
      if(seperator) 
      {
        data      += ",";
        statement += ",";
      }
      data      += datum;
      statement += info->m_column;
      seperator  = true;
    }
  }

  statement += ") VALUES (" + data + ")";
  return statement;
}

CString
SQLMigrate::VariantToInsertString(SQLVariant* p_var,int p_datatype)
{
  CString result;
  TIMESTAMP_STRUCT* stamp;
  DATE_STRUCT*      date;
  TIME_STRUCT*      time;
  SQLGUID*          guid;

  switch(p_datatype)
  {
    case SQL_CHAR:        // Fall through
    case SQL_VARCHAR:
    case SQL_LONGVARCHAR: p_var->GetAsString(result);
                          result.Replace("\'","\'\'");
                          result = "\'" + result + "\'";
                          break;
    case SQL_TIME:
    case SQL_TYPE_TIME:   time = p_var->GetAsTime();
                          result.Format("{t '%2.2d:%2.2d:%2.2d'}"
                                       ,time->hour
                                       ,time->minute
                                       ,time->second);
                          break;
    case SQL_DATE:        
    case SQL_TYPE_DATE:   date = p_var->GetAsDate();
                          if(date->day && date->month && date->year)
                          {
                            result.Format("{d '%4.4d-%2.2d-%2.2d'}"
                                         ,date->year
                                         ,date->month
                                         ,date->day);
                          }
                          break;
    case SQL_TIMESTAMP:
    case SQL_TYPE_TIMESTAMP:
                          stamp = p_var->GetAsTimestamp();
                          if(stamp->day && stamp->month && stamp->year)
                          {
                            result.Format("{ts '%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d'}"
                                         ,stamp->year
                                         ,stamp->month
                                         ,stamp->day
                                         ,stamp->hour
                                         ,stamp->minute
                                         ,stamp->second);
                          }
                          break;
    case SQL_GUID:        guid = p_var->GetAsGUID();
                          result.Format("{guid '%8.8X-%4.4X-%X%X%X%X%X%X%X%X'}"
                                        ,guid->Data1
                                        ,guid->Data2
                                        ,guid->Data3
                                        ,guid->Data4[0]
                                        ,guid->Data4[1]
                                        ,guid->Data4[2]
                                        ,guid->Data4[3]
                                        ,guid->Data4[4]
                                        ,guid->Data4[5]
                                        ,guid->Data4[6]
                                        ,guid->Data4[7]);
    // Most datatypes (numeric)
    // BEWARE: Intervals not implemented
    default:              p_var->GetAsString(result);
                          break;
  }
  return result;
}

CString
SQLMigrate::MakeIdentityStatement(CString p_vendor,CString p_user,CString p_tabel)
{
  CString identity;
  if(m_databaseTarget->GetDatabaseType() == DatabaseType::RDBMS_SQLSERVER)
  {
    identity = "SET IDENTITY_INSERT " + p_user + "." + p_tabel + " ON;";
  }
  return identity;
}

// These are the exceptions found throughout the years
// Conversions that cannot be done in the 'regular' way
void
SQLMigrate::DatatypeExceptions(RebindMap& p_map)
{
  // ORACLE -> Firebird
  if(m_params.v_sourceType == DatabaseType::RDBMS_ORACLE &&
     m_params.v_targetType == DatabaseType::RDBMS_FIREBIRD)
  {
    // CLOB -> BLOB SUB_TYPE TEXT
    p_map.insert(std::make_pair(SQL_LONGVARCHAR,SQL_LONGVARBINARY));
  }

  // ORACLE -> MS-SQLServer. 
  // Only way to convert the timestamps
  // Otherwise we get a 'timestamp field overflow' error
  if(m_params.v_sourceType == DatabaseType::RDBMS_ORACLE &&
     m_params.v_targetType == DatabaseType::RDBMS_SQLSERVER)
  {
    p_map.insert(std::make_pair(SQL_TYPE_TIMESTAMP, SQL_VARCHAR));
  }
}

}