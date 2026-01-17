////////////////////////////////////////////////////////////////////////
//
// File: XPort.h
//
// Created: 1998-2025 ir. W.E. Huisman
// MIT License
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
// Version number: See SQLXport.h
//
#include "stdafx.h"
#include "XPort.h"
#include "SQLQuery.h"
#include "SQLXport.h"
#include <SQLInfoDB.h>

XPort::XPort(Parameters& p_parameters)
      :m_parameters(p_parameters)
      ,m_xfile(p_parameters)
{
  m_schema = m_parameters.m_schema;
  m_object = m_parameters.m_object;
}

XPort::~XPort()
{
  if(m_database.IsOpen())
  {
    Disconnect();
  }
}

void
XPort::Export()
{
  // STEP 1: Connect to database
  if(Connect() == false)
  {
    return;
  }

  // STEP 2: Connect to a dump file
  if(OpenDumpFileWrite() == false)
  {
    return;
  }

  // STEP 3: Lock all tables
  if(m_parameters.m_consistent)
  {
    xprintf(false,_T("CONSISTENT Export mode: locking all tables in share mode\n"));
    if(ConsistentTables(m_parameters.m_schema) == false)
    {
      return;
    }
  }
  ExportUserTypes();

  // STEP 4: Export of all tables/columns and data of all tables
  if(m_parameters.m_tables)
  {
    // STEP 5: Retrieve all tables to work on
    //         and export the table definitions and data
    GetAllTables();
    ExportTables();

    if(m_parameters.m_constraints)
    {
      // STEP 6: Export all indices
      ExportIndices();

      // STEP 7: Export all primary keys
      ExportPrimaryKeys();

      // STEP 8: Export all foreign keys
      ExportForeignKeys();

      // STEP 9: Export all default constraints
      ExportDefaultConstraints();

      // STEP 10: Export all check constraints
      ExportCheckConstraints();
    }
  }

  // STEP 11: For all sequences
  if(m_parameters.m_sequences)
  {
    ExportSequences();
  }

  // STEP 12: for all procedures/functions/triggers/package/type/type body
  if(m_parameters.m_source)
  {
    ExportProcedures();
    ExportTriggers();
  }

  // STEP 13: for all views
  if(m_parameters.m_views)
  {
    ExportViews();
  }

  // STEP 14: Export of the synonyms
  if(m_parameters.m_synonyms)
  {
    ExportSynonyms();
  }

  // STEP 15: All object rights
  if(m_parameters.m_grants)
  {
    ExportPrivileges();
  }
  // STEP 16: Export all gathered comments
  ExportComments();

  // STEP 17: End-of file marker
  CloseDumpFile();

  xprintf(false,_T("\n*** Export ready ***\n\n"));
}

void
XPort::Import()
{
  // STEP 1: Connect to database
  if(m_parameters.m_listOnly == false)
  {
    if(Connect() == false)
    {
      return;
    }
  }

  // STEP 2: Open the export file, skipping header
  if(!OpenDumpFileRead())
  {
    return;
  }

  // STEP 3: Optional drop the schema content
  if(m_parameters.m_drop)
  {
    DropSchema();
  }

  // STEP 4: Read and process the dump list
  ImportDump();

  // STEP 5: RETRY QUEUE
  // All procedure text is now imported
  // Perform the retry queue, so failed views/source can get imported
  PerformRetries();

  // STEP 6: Statistics
  // After everything is in database, extra processing
  if(m_parameters.m_statistics)
  {
    xprintf(false,_T("Starting 'compute statistics' upon completion of the import\n"));
    PerformStatistics();
  }

  // STEP 7: Recompile
  if(m_parameters.m_recompile)
  {
    xprintf(false,_T("Starting 'full recompile' upon completion of the import\n"));
    PerformRecompile();
  }
  xprintf(false,_T("\n*** Import ready ***\n\n"));
}

bool
XPort::Connect()
{
  XString database(m_parameters.m_database);
  XString user    (m_parameters.m_user);
  XString schema  (m_parameters.m_schema);

  try
  {
    m_database.Open(database,user,m_parameters.m_password);
  }
  catch(StdException& er)
  {
    xerror(_T("Cannot open database [%s]. Error: %s\n"),database.GetString(),er.GetErrorMessage().GetString());
    return false;
  }
  xprintf(false,_T("Database [%s] opened by user [%s]\n"),database.GetString(),user.GetString());
  if(!schema.IsEmpty())
  {
    m_database.AddMacro(SCHEMA_MACRO,schema);
    xprintf(false,_T("Default schema for import set to [%s]\n"),schema.GetString());
  }
  // Use our SQL if possible and present
  m_database.GetSQLInfoDB()->GetInfo();
  m_database.GetSQLInfoDB()->SetPreferODBC(false);
  // Generate not too much indexes for export
  m_database.GetSQLInfoDB()->SetFilterPKFK(true);

  // Explicitly set the default schema, as givven from the parameters
  // This will set the schema search-path or the default (database+schema)
  if(m_database.SetDefaultSchema(user,schema))
  {
    xprintf(false,_T("Default schema search path set to [%s->%s]\n"),user.GetString(),schema.GetString());
    return true;
  }
  xerror(_T("Cannot determine the schema search path for [%s->%s]\n"),user.GetString(),schema.GetString());
  return false;
}

bool
XPort::Disconnect()
{
  if(m_database.IsOpen())
  {
    // Now closing the database
    XString name = m_database.GetDatabaseName();
    m_database.Close();
    xprintf(false,_T("Database connection with [%s] closed\n"),name.GetString());
  }
  else
  {
    xerror(_T("Database connection was not open\n"));
  }
  return true;
}

bool
XPort::OpenDumpFileWrite()
{
  if(!m_xfile.OpenCreate(m_parameters.m_file,m_database.GetDatabaseTypeName()))
  {
    xprintf(false,_T("Cannot open the export file [%s]\n"),m_parameters.m_file.GetString());
    return false;
  }
  xprintf(false,_T("Opened the export dump file for writing [%s]\n"),m_parameters.m_file.GetString());
  return true;
}

bool
XPort::OpenDumpFileRead()
{
  bool result = m_xfile.OpenRead(m_parameters.m_file,m_database.GetDatabaseTypeName(),m_parameters.m_listOnly);
  if(!result)
  {
    xprintf(false,_T("Cannot open the export file [%s]\n"),m_parameters.m_file.GetString());
    return false;
  }
  xprintf(false,_T("Opened the export dump file for reading [%s]\n"),m_parameters.m_file.GetString());
  return true;
}

bool
XPort::CloseDumpFile()
{
  m_xfile.WriteEndExport();
  return m_xfile.Close();
}

void    
XPort::DropSchema()
{
  OList statements;

  GatherDropSchema (statements);
  ExecuteDropSchema(statements);
}

void
XPort::GatherDropSchema(OList& p_statements)
{ 
  SQLInfoDB* info = m_database.GetSQLInfoDB();
  XString schema(m_schema);
  XString all;

  XString allsynonyms    = info->GetCATALOGTableSynonyms(schema,all);
  XString allsequences   = info->GetCATALOGSequenceList (schema,all);
  XString allprocedures  = info->GetPSMProcedureList    (schema,all);
  XString alltriggers    = info->GetCATALOGTriggerList  (schema,all);
  XString allviews       = info->GetCATALOGViewList     (schema,all);
  XString allforeigns    = info->GetCATALOGForeignList  (schema,all);
  XString alltables      = info->GetCATALOGTablesList   (schema,all);
  XString alltypes       = info->GetCATALOGTypeList     (schema,all);
  
  xprintf(false,_T("Gathering contents of schema: %s\n"),m_schema.GetString());
  try
  {
    SQLQuery query(&m_database);

    // SYNONYMS
    query.DoSQLStatement(allsynonyms,schema);
    while(query.GetRecord())
    {
      XString synonym  = query[MetaSynonym_synonymname];
      XString dropline = info->GetCATALOGSynonymDrop(schema,synonym);
      p_statements.push_back(dropline);
    }
    // SEQUENCES
    query.DoSQLStatement(allsequences,schema);
    while(query.GetRecord())
    {
      XString dropline = info->GetCATALOGSequenceDrop(schema,query[MetaSequence_sequencename]);
      p_statements.push_back(dropline);
    }
    // PROCEDURES
    query.DoSQLStatement(allprocedures,schema);
    while(query.GetRecord())
    {
      XString procedure = query[MetaProcedure_procedurename];
      int     typeproc  = query[4];
      XString dropline = info->GetPSMProcedureDrop(schema,procedure,typeproc == 2);
      p_statements.push_back(dropline);
    }
    // TRIGGERS
    query.DoSQLStatement(alltriggers,schema);
    while(query.GetRecord())
    {
      XString tablename = query[MetaTrigger_tablename];
      XString trigger   = query[MetaTrigger_triggername];
      XString dropline = info->GetCATALOGTriggerDrop(schema,tablename,trigger);
      p_statements.push_back(dropline);
    }
    // VIEWS
    query.DoSQLStatement(allviews,schema);
    while(query.GetRecord())
    {
      XString precursor;
      XString dropline = info->GetCATALOGViewDrop(schema,query[MetaTable_tablename],precursor);
      if(!precursor.IsEmpty())
      {
        p_statements.push_back(precursor);
      }
      p_statements.push_back(dropline);
    }
    // FOREIGN KEY CONSTRAINTS
    query.DoSQLStatement(allforeigns,schema);
    while(query.GetRecord())
    {
      if(query[MetaForeign_keysequence].GetAsSLong() == 1)
      {
        XString dropline = info->GetCATALOGForeignDrop(schema,query[MetaForeign_fk_tablename],query[MetaForeign_fk_constraint]);
        p_statements.push_back(dropline);
      }
    }
    // TABLES
    query.DoSQLStatement(alltables,schema);
    while(query.GetRecord())
    {
      XString dropline = info->GetCATALOGTableDrop(schema,query[3],false,true);
      p_statements.push_back(dropline);
    }
    // USER TYPES
    if(!alltypes.IsEmpty())
    {
      query.DoSQLStatement(alltypes,schema);
      while(query.GetRecord())
      {
        if(query.GetColumn(MetaType_ordinal)->GetAsSLong() == 1)
        {
          XString dropline = info->GetCATALOGTypeDrop(schema,query[MetaType_typename]);
          p_statements.push_back(dropline);
        }
      }
    }
  }
  catch(StdException& ex)
  {
  	xerror(_T("Cannot gather information for schema:%s Error: %s\n"),m_schema.GetString(),ex.GetErrorMessage().GetString());
    return;
  }
}

void
XPort::ExecuteDropSchema(OList& p_statements)
{
  XString transname(_T("DropSchema"));
  SQLTransaction trans(GetDatabase(),transname);
  int count  = (int)p_statements.size();
  int errors = 0;
  XString drop;

  xprintf(false, _T("Dropping the contents of schema: %s\n"), m_schema.GetString());

  try
  {
    for(auto& statement : p_statements)
    {
      drop = statement;
      if(ImportSQL(drop) != 0)
      {
        ++errors;
      }
    }
    trans.Commit();
    // Tell what we've done
    xprintf(false,_T("Schema dropped: %d SQL statements done"),count);
    if(errors)
    {
      xprintf(false,_T(". Errors: %d"),errors);
    }
    xprintf(false,_T("\n"));
  }
  catch(StdException& ex)
  {
    xerror(_T("Cannot drop schema:%s\nSQL: %s\nError: %s\n"),m_schema.GetString(),drop.GetString(),ex.GetErrorMessage().GetString());
    return;
  }
}

int
XPort::GetAllUserTypes()
{
  int count = 0;
  XString sql = m_database.GetSQLInfoDB()->GetCATALOGTypeList(m_schema,m_object,true);

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(&m_database);
      int param = 1;
      if(!m_schema.IsEmpty())
      {
        qry.SetParameter(param++,m_schema);
      }
      if(!m_object.IsEmpty())
      {
        qry.SetParameter(param++,m_object);
      }
      qry.DoSQLStatement(sql);
      while(qry.GetRecord())
      {
        XString usertype = qry[MetaType_typename];
        int     ordinal  = qry[MetaType_ordinal];
        if(ordinal == 1)
        {
          ++count;
          m_usertypes.push_back(usertype);
        }
      }
      if(!m_usertypes.empty())
      {
        break;
      }
      // Standard naming conventions
      sql = m_database.GetSQLInfoDB()->GetCATALOGTypeList(m_schema,m_object);
    }
  }
  catch(StdException& ex)
  {
    xerror(_T("Error counting user defined types: %s\n"),ex.GetErrorMessage().GetString());
    m_usertypes.clear();
    count = 0;
  }
  xprintf(false,_T("Exporting a total of [%d] user defined types\n"),count);
  return count;
}

int  
XPort::GetAllTables()
{
  int count = 0;
  XString sql = m_database.GetSQLInfoDB()->GetCATALOGTablesList(m_schema,m_object,true);

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(&m_database);

      int param = 1;
      if(!m_schema.IsEmpty())
      {
        qry.SetParameter(param++,m_schema);
      }
      if(!m_object.IsEmpty())
      {
        qry.SetParameter(param++,m_object);
      }
      qry.DoSQLStatement(sql);
      while(qry.GetRecord())
      {
        ++count;
        XString table = qry[3];
        XString type  = qry[4];
        table = table.Trim();
        type  = type.Trim();
        // Skip local and global temporary tables
        // Skip system tables and views
        if(type.Compare(_T("TABLE")) == 0)
        {
          m_tables.push_back(table);
        }
      }
      if(!m_tables.empty())
      {
        break;
      }
      // Standard unquoted names
      sql = m_database.GetSQLInfoDB()->GetCATALOGTablesList(m_schema,m_object);
    }
  }
  catch(StdException& ex)
  {
    xerror(_T("Error counting tables: %s\n"),ex.GetErrorMessage().GetString());
    m_tables.clear();
    count = 0;
  }
  xprintf(false,_T("Exporting a total of [%d] tables\n"),count);
  return count;
}

int
XPort::GetAllCheckConstraints()
{
  long    count = 0;
  XString sql = m_database.GetSQLInfoDB()->GetCATALOGCheckList(m_schema,m_object);

  try
  {
    SQLQuery qry(&m_database);

    int param = 1;
    if(!m_schema.IsEmpty())
    {
      qry.SetParameter(param++,m_schema);
    }
    if(!m_object.IsEmpty())
    {
      qry.SetParameter(param++,m_object);
    }
    qry.DoSQLStatement(sql);
    while(qry.GetRecord())
    {
      ++count;
      XString naam = qry[4];
      m_constraints.push_back(naam);
    }
  }
  catch(StdException& ex)
  {
    xerror(_T("Internal error: Getting a check constraint count : %s\n"),ex.GetErrorMessage().GetString());
    count = 0;
  }
  return count;
}

int
XPort::GetAllViews()
{
  int count = 0;
  // try quoted identifiers first
  XString sql = m_database.GetSQLInfoDB()->GetCATALOGViewList(m_schema,m_object,true);

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(&m_database);

      int param = 1;
      if(!m_schema.IsEmpty())
      {
        qry.SetParameter(param++,m_schema);
      }
      if(!m_object.IsEmpty())
      {
        qry.SetParameter(param++,m_object);
      }
      qry.DoSQLStatement(sql);
      while(qry.GetRecord())
      {
        ++count;
        XString viewName = qry[3];
        m_views.push_back(viewName);
      }
      if(count)
      {
        return count;
      }
      // Standard identifiers
      sql = m_database.GetSQLInfoDB()->GetCATALOGViewList(m_schema,m_object);
    }
  }
  catch(StdException& ex)
  {
    xerror(ex.GetErrorMessage());
    xerror(_T("Internal error: Reading count of views\n"));
    count = 0;
  }
  return count;
}

int
XPort::GetAllSequences()
{
  int count = 0;
  XString object(m_object);
  XString sql = m_database.GetSQLInfoDB()->GetCATALOGSequenceList(m_schema,object,true);

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(&m_database);
      int param = 1;
      if(!m_schema.IsEmpty())
      {
        qry.SetParameter(param++,m_schema);
       }
      if(!object.IsEmpty())
      {
        qry.SetParameter(param++,object);
      }
      qry.DoSQLStatement(sql);
      while(qry.GetRecord())
      {
        ++count;
        XString seqName = qry[3];
        m_sequences.push_back(seqName);
      }
      if(count)
      {
        return count;
      }
      sql = m_database.GetSQLInfoDB()->GetCATALOGSequenceList(m_schema,object);
    }
  }
  catch(StdException& ex)
  {
    xerror(ex.GetErrorMessage());
    xerror(_T("Internal error reading count of sequences.\n"));
    count = 0;
  }
  return count;
}

int
XPort::GetAllProcedures()
{
  int count = 0;
  XString object(m_object);
  if(!object.IsEmpty() && (object.Find('%') < 0))
  {
    object = _T("%") + object + _T("%");
  }
  XString sql = m_database.GetSQLInfoDB()->GetPSMProcedureAttributes(m_schema,object,true);

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(&m_database);
      int param =1;
      if(!m_schema.IsEmpty())
      {
        qry.SetParameter(param++,m_schema);
      }
      if(!m_object.IsEmpty())
      {
        qry.SetParameter(param++,object);
      }
      qry.DoSQLStatement(sql);
      while(qry.GetRecord())
      {
        ++count;
        XString procType = qry[8].GetAsSLong() == 1 ? _T("procedure") : _T("function");
        XString procName = qry[3];
        XString procedure = procType + _T(":") + procName;
        m_procedures.push_back(procedure);
      }
      if(count)
      {
        return count;
      }
      sql = m_database.GetSQLInfoDB()->GetPSMProcedureAttributes(m_schema,object);
    }
  }
  catch(StdException& ex)
  {
    xerror(ex.GetErrorMessage());
    xerror(_T("Internal error reading count of procedures.\n"));
    return 0;
  }
  return count;
}

int
XPort::GetAllTriggers()
{
  int count = 0;
  XString object(m_object);
  if(!object.IsEmpty() && (object.Find('%') < 0))
  {
    object = _T("%") + object + _T("%");
  }
  XString sql = m_database.GetSQLInfoDB()->GetCATALOGTriggerList(m_schema,object,true);

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(&m_database);
      int param = 1;
      if(!m_schema.IsEmpty())
      {
        qry.SetParameter(param++,m_schema);
      }
      if(!m_object.IsEmpty())
      {
        qry.SetParameter(param++,object);
      }
      qry.DoSQLStatement(sql);
      while(qry.GetRecord())
      {
        ++count;
        XString trigName = qry[4];
        XString trigger = _T("trigger:") + trigName;
        m_triggers.push_back(trigger);
      }
      if(count)
      {
        return count;
      }
      sql = m_database.GetSQLInfoDB()->GetCATALOGTriggerList(m_schema,object);
    }
  }
  catch(StdException& ex)
  {
    xerror(ex.GetErrorMessage());
    xerror(_T("Internal error reading count of triggers.\n"));
    return 0;
  }
  return count;
}

int
XPort::GetAllSynonyms()
{
  int count = 0;
  XString object(m_object);
  XString sql = m_database.GetSQLInfoDB()->GetCATALOGSynonymList(m_schema,object);

  // No synonyms to be found in this database
  if(sql.IsEmpty())
  {
    return 0;
  }

  try
  {
    SQLQuery qry(&m_database);
    int param = 1;
    if(!m_schema.IsEmpty())
    {
      qry.SetParameter(param++,m_schema);
    }
    if(!object.IsEmpty())
    {
      qry.SetParameter(param++,object);
    }
    qry.DoSQLStatement(sql);
    while(qry.GetRecord())
    {
      ++count;
      XString synonym = qry[3];
      m_synonyms.push_back(synonym);
    }
  }
  catch(StdException& ex)
  {
    xerror(ex.GetErrorMessage());
    xerror(_T("Internal error reading count of synonyms.\n"));
    return 0;
  }
  return count;
}

// Does a SQL statement
// Returns 0 (success) or -1 (error)
int
XPort::DoSQLStatement(XString& p_sql,bool p_can_retry)
{
  try
  {
    SQLQuery qry(&m_database);
    qry.DoSQLStatement(p_sql);
  }
  catch(StdException& ex)
  {
    if(p_can_retry)
    {
      m_retries.push_back(p_sql);
      return 0;
    }
    p_sql.Replace(SCHEMA_MACRO,m_parameters.m_schema);
    xerror(_T("Error in sql: %s\n%s\n"),ex.GetErrorMessage().GetString(),p_sql.GetString());
    return -1;
  }
  return 0;
}

bool
XPort::ConsistentTables(const XString& p_schema)
{
  XString lock;
  SQLInfoDB* info = m_database.GetSQLInfoDB();

  for(unsigned ind = 0;ind < m_tables.size(); ++ind)
  {
    XString table = info->QueryIdentifierQuotation(m_tables[ind]);
    xprintf(false,_T("Lock table [%s.%-32s] for export [%u/%u]\n"),p_schema.GetString(),table.GetString(),ind + 1,m_tables.size());

    lock = info->GetSQLLockTable(p_schema,table,false,0); // gWaitTime);
    if(DoSQLStatement(lock,false))
    {
      xprintf(false,_T("Cannot lock table [%s.%s] for export\n"),p_schema.GetString(),table.GetString());
      return false;
    }
  }
  _tprintf(_T("\n"));
  return true;
}

void
XPort::PerformStatistics()
{
  XString calc;
  int ind = 1;
  SQLInfoDB* info = m_database.GetSQLInfoDB();
  XString schema = info->QueryIdentifierQuotation(m_schema);

  for(auto& table : m_tables)
  {
    xprintf(false,_T("Gather statistics for table: %s.%s [%u/%u]\n"),m_schema.GetString(),table.GetString(),ind++,m_tables.size());
    table = info->QueryIdentifierQuotation(table);
    calc  = info->GetSQLOptimizeTable(schema,table);
    DoSQLStatement(calc);
  }
  xprintf(false,_T("\n"));
}

void
XPort::PerformRecompile()
{
  if(m_database.GetDatabaseType() == DatabaseType::RDBMS_ORACLE)
  {
    XString compile;
    xprintf(false,_T("Do a full recompile for all objects in schema: %s\n"),m_schema.GetString());
    compile.Format(_T("execute dbms_utility.compile_schema(schema => '%s')"),m_schema.GetString());
    DoSQLStatement(compile);
  }
}


void
XPort::ShowRetryName(const XString& p_sql)
{
  XString name;
  int pos = p_sql.Find(_T("\n"));
  if(pos > 0)
  {
    name = p_sql.Left(pos);
    name.Replace(SCHEMA_MACRO,m_parameters.m_schema);
    xprintf(false,_T("Retry: %s\n"),name.GetString());
  }
}

void
XPort::PerformRetries()
{
  // If nothing to do, bugger off
  if(m_retries.size() == 0)
  {
    return;
  }
  OList second;
  int   pass    = 0;
  int   retries = (int) m_retries.size() * 10;

  // We are going to do the retry queue from here
  xprintf(false,_T("\nRETRY QUEUE of previously rejected DDL statements\n"));
  xprintf(false,  _T("=================================================\n"));
  xprintf(false,  _T("Maximum number of retry passes: %d\n"),retries);

  // Do while objects in the retry queue, and we have a calculated retry count
  // Retries protect against loops from endless queues
  while(retries > 0 && !m_retries.empty())
  {
    xprintf(false,_T("Pass: %d. Number of objects in the retry queue: %d\n"),++pass,m_retries.size());
    for(size_t ind = 0;ind < m_retries.size(); ++ind)
    {
      XString sql = m_retries[ind];
      ShowRetryName(sql);
      if(DoSQLStatement(sql,false) < 0)
      {
        second.push_back(sql);
      }
    }
    // Number of objects has not dwindled
    // so, no reason to go on trying!
    if(m_retries.size() == second.size())
    {
      break;
    }
    // Put the second queue in the retry queue
    m_retries.clear();
    m_retries = second;
    second.clear();
    // Next retry round
    --retries;
  }

  // Print the contents, if something left in the queue
  if(!second.empty())
  {
    xprintf(false,_T("\nRETRY QUEUE IS NOT EMPTY:\n"));
    // At the end of the retry 
    // If any errors left, show them
    xprintf(false,_T("The following %d objects where unresolvable:\n"),second.size());
    for(size_t ind = 0;ind < second.size();++ind)
    {
      XString sql = second[ind];
      ShowRetryName(sql);
    }
    xprintf(false,_T("After %d passes\n\n"),pass);
  }

  // Clear the queue
  m_retries.clear();
}

XString 
XPort::GetDefineSQLTable(const XString& p_table)
{
  DDLCreateTable create(m_database.GetSQLInfoDB());

  try
  {
    // Getting columns and options for the first statement
    XString table;
    if(!m_schema.IsEmpty())
    {
      table = m_schema + _T(".");
    }
    table += p_table;
    DDLS statements = create.GetTableStatements(table,true,true,false,false,false,false,false,false);
    if(statements.size() == 1)
    {
      RecordAllColumns(create);
      RecordAllComments(create);
      return statements[0];
    }
  }
  catch(StdException& ex)
  {
    xerror(ex.GetErrorMessage());
  }
  xerror(_T("Error retrieving table columns for table [%s]\n"),p_table.GetString());
  return _T("");
}

DDLS
XPort::GetDefineSQLIndex(const XString& p_table)
{
  DDLCreateTable create(m_database.GetSQLInfoDB());

  try
  {
    // Getting columns, indices, and primary key (Possibly overlapping indices)
    DDLS statements = create.GetTableStatements(p_table,false,false,true,false,false,false,false,false);
    if(statements.size() >= 1)
    {
      RecordAllIndices(create,statements);
      return statements;
    }
    return statements;
  }
  catch(StdException& ex)
  {
    xerror(ex.GetErrorMessage());
  }
  xerror(_T("Error retrieving index information for table [%s]\n"),p_table.GetString());
  DDLS empty;
  return empty;
}

DDLS
XPort::GetDefineSQLPrimary(const XString& p_table)
{
  DDLCreateTable create(m_database.GetSQLInfoDB());

  try
  {
    // Getting columns, indices, and primary key (Possibly overlapping indices)
    DDLS statements = create.GetTableStatements(p_table,false,false,false,true,false,false,false,false);
    if(statements.size() >= 1)
    {
      RecordAllPrimaries(create,p_table);
    }
    return statements;
  }
  catch(StdException& ex)
  {
    xerror(ex.GetErrorMessage());
  }
  xerror(_T("Error retrieving primary key information for table [%s]\n"),p_table.GetString());
  DDLS empty;
  return empty;
}

DDLS
XPort::GetDefineSQLForeigns(const XString& p_table)
{
  DDLCreateTable create(m_database.GetSQLInfoDB());

  try
  {
    // Getting columns and foreign keys and possibly overlapping indices
    DDLS statements = create.GetTableStatements(p_table,true,false,false,false,true,false,false,false);
    RecordAllForeigns(create);
    return statements;
  }
  catch(StdException& ex)
  {
    xerror(ex.GetErrorMessage());
  }
  xerror(_T("Error retrieving foreign key information for table [%s]\n"),p_table.GetString());
  DDLS empty;
  return empty;
}

void
XPort::RecordAllColumns(DDLCreateTable& p_create)
{
  m_columns.clear();
  m_trimlist.clear();

  for(auto& column : p_create.m_columns)
  {
    m_columns.push_back(column.m_column);
    bool trim = (column.m_datatype == SQL_CHAR || column.m_datatype == SQL_WCHAR);
    m_trimlist.push_back(trim);
  }
}

void
XPort::RecordAllIndices(DDLCreateTable& p_create,DDLS& p_ddls)
{
  m_indices.clear();
  DDLS::iterator it = p_ddls.begin();
  for(auto& index : p_create.m_indices)
  {
    XString name = index.m_indexName;
    if(name.IsEmpty() || index.m_position > 1)
    {
      continue;
    }
    XString sql = *it;
    sql.MakeLower();
    if(sql.Find(_T("primary key")) < 0)
    {
      m_indices.push_back(name);
      ++it;
    }
    else
    {
      it = p_ddls.erase(it);
    }
  }
}

void
XPort::RecordAllPrimaries(DDLCreateTable& p_create,const XString& p_table)
{
  m_constraints.clear();

  // Check that we have a primary key
  if(p_create.m_primaries.empty())
  {
    return;
  }
  // Check that it has a name
  if(p_create.m_primaries[0].m_constraintName.IsEmpty())
  {
    m_constraints.push_back(XString(_T("PK_")) + p_table);
    return;
  }
  // Use the primary key constraint name
  m_constraints.push_back(p_create.m_primaries[0].m_constraintName);
}

void
XPort::RecordAllForeigns(DDLCreateTable& p_create)
{
  m_constraints.clear();
  for(auto& foreign : p_create.m_foreigns)
  {
    m_constraints.push_back(foreign.m_foreignConstraint);
  }
}

XString 
XPort::GetDefineSQLUserType(const XString& p_type)
{
  XString create;
  XString usertype(p_type);
  int pos = p_type.Find(':');
  if(pos > 0)
  {
    usertype = p_type.Mid(pos + 1);
  }
  XString object;
  XString errors;
  XString catalog;
  MUserTypeMap usertypes;
  if(m_database.GetSQLInfoDB()->MakeInfoUserTypes(usertypes,errors,catalog,m_schema,usertype))
  {
    create = m_database.GetSQLInfoDB()->GetCATALOGTypeCreate(usertypes);
    XString comment = m_database.GetSQLInfoDB()->GetCATALOGCommentCreate(m_schema,_T("TYPE"),usertype,_T(""),usertypes[0].m_remarks);
    if(!comment.IsEmpty())
    {
      m_comments.push_back(comment);
    }
  }
  else
  {
    xerror(_T("Internal error reading user type %s.%s\n"),m_schema.GetString(),p_type.GetString());
  }
  create = create.TrimLeft(_T("\r\n"));
  return create;
}

XString
XPort::GetDefineSQLView(const XString& p_view)
{
  XString view(p_view);
  SQLInfoDB* info = m_database.GetSQLInfoDB();
  XString sql = info->GetCATALOGViewText(m_schema,view,true);
  XString create;

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(&m_database);
      qry.DoSQLStatement(sql);
      if(qry.GetRecord())
      {
        qry[1].GetAsString(create);
        break;
      }
      sql = info->GetCATALOGViewText(m_schema,view);
    }
  }
  catch(StdException& ex)
  {
    xerror(ex.GetErrorMessage());
    xerror(_T("Internal error: Getting information for view: %s\n"),view.GetString());
    return _T("");
  }

  // Some RDBMS only store the creation text partially
  if(create.Left(6).CompareNoCase(_T("CREATE")))
  {
    // Getting the column information for the view
    XString errors;
    XString catalog;
    MColumnMap columns;
    info->MakeInfoTableColumns(columns,errors,catalog,m_schema,p_view);

    XString extra = info->GetCATALOGViewCreate(m_schema,p_view,columns,create);
    if(extra.GetLength() > create.GetLength())
    {
      return extra;
    }
  }
  return create;
}

XString
XPort::GetDefineSQLSequence(const XString& p_sequence)
{
  XString sequence(p_sequence);

  SQLInfoDB* info = m_database.GetSQLInfoDB();
  XString sql = info->GetCATALOGSequenceAttributes(m_schema,sequence,true);
  XString define;

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery query(m_database);
      int param = 1;
      if(!m_schema.IsEmpty()) 
      {
        query.SetParameter(param++,m_schema);
      }
      query.SetParameter(param++,sequence);
      query.DoSQLStatement(sql);
      if(query.GetRecord())
      {
        MetaSequence seq;
        seq.m_schemaName   = m_schema;
        seq.m_sequenceName = p_sequence;
        seq.m_currentValue = query[MetaSequence_currentvalue];
        seq.m_minimalValue = query[MetaSequence_minimalvalue];
        seq.m_increment    = query[MetaSequence_increment];
        seq.m_cache        = query[MetaSequence_cache];
        seq.m_cycle        = query[MetaSequence_cycle];
        seq.m_order        = query[MetaSequence_order];
        seq.m_remarks      = (XString) query[MetaSequence_remarks];

        define = info->GetCATALOGSequenceCreate(seq);
        XString comment = info->GetCATALOGCommentCreate(m_schema,_T("SEQUENCE"),sequence,_T(""),seq.m_remarks);
        if(!comment.IsEmpty())
        {
          m_comments.push_back(comment);
        }
        break;
      }
      sql = info->GetCATALOGSequenceAttributes(m_schema,sequence);
    }
  }
  catch (StdException& ex)
  {
    xerror(ex.GetErrorMessage());
    xerror(_T("Internal error reading sequence: %s\n"),p_sequence.GetString());
  }
  return define;
}

XString
XPort::GetDefineSQLProcedure(const XString& p_procedure)
{
  XString create;

  // Find procedure/function name
  XString procedure(p_procedure);
  int pos = p_procedure.Find(':');
  if(pos > 0)
  {
    procedure = p_procedure.Mid(pos + 1);
  }

  // Find all attributes
  XString errors;
  XString catalog;
  MProcedureMap procedures;
  if(m_database.GetSQLInfoDB()->MakeInfoPSMProcedures(procedures,errors,catalog,m_schema,procedure))
  {
    // Find the true definition
    create = m_database.GetSQLInfoDB()->GetPSMProcedureCreate(procedures[0]);
  }
  return create;
}

XString
XPort::GetDefineSQLTrigger(const XString& p_trigger)
{
  XString create;
  XString trigger(p_trigger);
  int pos = p_trigger.Find(':');
  if(pos > 0)
  {
    trigger = p_trigger.Mid(pos + 1);
  }
  XString object;
  XString errors;
  MTriggerMap triggers;
  if(m_database.GetSQLInfoDB()->MakeInfoTableTriggers(triggers,errors,m_schema,object,trigger))
  {
    create = m_database.GetSQLInfoDB()->GetCATALOGTriggerCreate(triggers[0]);
    XString comment = m_database.GetSQLInfoDB()->GetCATALOGCommentCreate(m_schema,_T("TRIGGER"),trigger,_T(""),triggers[0].m_remarks);
    if(!comment.IsEmpty())
    {
      m_comments.push_back(comment);
    }
  }
  else
  {
    xerror(_T("Internal error reading PSM source of %s.%s\n"),m_schema.GetString(),p_trigger.GetString());
  }
  create = create.TrimLeft(_T("\r\n"));
  return create;
}

XString 
XPort::GetDefineSQLSynonym(const XString& p_synonym)
{
  XString create;
  XString synonym(p_synonym);

  XString sql = m_database.GetSQLInfoDB()->GetCATALOGSynonymAttributes(m_schema,synonym);

  try
  {
    SQLQuery query(&m_database);
    int param = 1;
    if(!m_schema.IsEmpty())
    {
      query.SetParameter(param++,m_schema);
    }
    if(!p_synonym.IsEmpty())
    {
      query.SetParameter(param++,synonym);
    }
    query.DoSQLStatement(sql);
    if(query.GetRecord())
    {
      XString definition = query.GetColumn(4)->GetAsString();
      create = m_database.GetSQLInfoDB()->GetCATALOGSynonymCreate(m_schema,synonym,definition);
    }
  }
  catch (StdException& ex)
  {
    xerror(ex.GetErrorMessage());
    xerror(_T("Internal error reading synonym of %s.%s\n"),m_schema.GetString(),synonym.GetString());
  }
  return create;
}

// Getters

XString
XPort::GetDefineRowSelect(const XString& p_table,SQLInfoDB* p_info)
{
  XString select(_T("SELECT "));
  for(unsigned ind = 0;ind < m_columns.size(); ++ind)
  {
    if(ind > 0) select += _T("      ,");
    if(m_trimlist[ind])
    {
      select += _T("TRIM(");
    }
    select += p_info->QueryIdentifierQuotation(m_columns[ind]);
    select += m_trimlist[ind] ? _T(")\n") : _T("\n");
  }
  select += _T(" FROM ");
  if(!m_schema.IsEmpty() && p_info->GetRDBMSUnderstandsSchemas())
  {
    select += p_info->QueryIdentifierQuotation(m_schema);
    select += _T(".");
  }
  select += p_info->QueryIdentifierQuotation(p_table);

  if(!m_parameters.m_filter.IsEmpty())
  {
    select += _T("\n WHERE ");
    select += m_parameters.m_filter;
  }

  // Getting columns and primary key 
  DDLCreateTable create(p_info);
  create.GetTableStatements(p_table,true,false,false,true,false,false,false,false);

  if(!create.m_primaries.empty())
  {
    int colno = 0;
    select += _T("\n ORDER BY ");
    for(auto& primary : create.m_primaries)
    {
      if(colno) select += _T(",");
      select += p_info->QueryIdentifierQuotation(primary.m_columnName);
      ++colno;
    }
  }
  return select;
}

XString
XPort::GetDefineCountSelect(const XString& p_table,SQLInfoDB* p_info)
{
  XString select(_T("SELECT COUNT(*) FROM "));
  if(!m_schema.IsEmpty() && p_info->GetRDBMSUnderstandsSchemas())
  {
    select += p_info->QueryIdentifierQuotation(m_schema);
    select += _T(".");
  }
  select += p_info->QueryIdentifierQuotation(p_table);
  if(!m_parameters.m_filter.IsEmpty())
  {
    select += _T("\n WHERE ");
    select += m_parameters.m_filter;
  }
  return select;
}

XString
XPort::GetDefineRowInsert(const XString& p_table,SQLInfoDB* p_info)
{
  bool inf = (m_parameters.m_listOnly == false);

  XString insert(_T("INSERT INTO "));

  if(!m_schema.IsEmpty() && inf && (p_info->GetRDBMSUnderstandsSchemas()))
  {
    insert += p_info->QueryIdentifierQuotation(m_schema);
    insert += _T(".");
  }
  insert += inf ? p_info->QueryIdentifierQuotation(p_table) : p_table;
  insert += _T("\n(");

  for(unsigned ind = 0;ind < m_columns.size(); ++ind)
  {
    insert += (ind > 0) ? _T(" ,") : _T(" ");
    insert += inf ? p_info->QueryIdentifierQuotation(m_columns[ind]) : m_columns[ind];
    insert += _T("\n");
  }
  insert += _T(")\nVALUES(");

  for(unsigned ind = 0;ind < m_columns.size(); ++ind)
  {
    if(ind > 0) 
    {
      insert += _T(",");
    }
    insert += _T("?");
  }
  insert += _T(")");

  return insert;
}

void
XPort::WriteTableAccessRights(const XString& p_object,int& p_count)
{
  try
  {
    XString errors;
    XString catalog;
    MPrivilegeMap privileges;
    SQLInfoDB* info = m_database.GetSQLInfoDB();
    if(info->MakeInfoTablePrivileges(privileges,errors,catalog,m_schema,p_object))
    {
      XString column;
      for(auto& priv : privileges)
      {
        XString grant = info->GetCATALOGGrantPrivilege(m_schema,p_object,column,priv.m_privilege,priv.m_grantee,priv.m_grantable);
        m_xfile.WriteSQL(grant);

        if((++p_count % COMMIT_SIZE) == 0)
        {
          _tprintf(_T("%d records processed\r"),p_count);
          _tprintf(_T("Exporting access rights: "));
        }
      }
    }
  }
  catch(StdException& ex)
  {
    xerror(ex.GetErrorMessage());
    xerror(_T("\nInternal error reading table access rights for [%s.%s]\n"),m_schema.GetString(),m_object.GetString());
  }
}

void
XPort::WriteColumnAccessRights(const XString& p_object,int& p_count)
{
  try
  {
    XString errors;
    XString catalog;
    MPrivilegeMap privileges;
    SQLInfoDB* info = m_database.GetSQLInfoDB();
    if(info->MakeInfoColumnPrivileges(privileges,errors,catalog,m_schema,p_object))
    {
      for(auto& priv : privileges)
      {
        XString grant = info->GetCATALOGGrantPrivilege(m_schema,p_object,priv.m_columnName,priv.m_privilege,priv.m_grantee,priv.m_grantable);
        m_xfile.WriteSQL(grant);

        if((++p_count % COMMIT_SIZE) == 0)
        {
          _tprintf(_T("%d records processed\r"),p_count);
          _tprintf(_T("Exporting access rights: "));
        }
      }
    }
  }
  catch(StdException& ex)
  {
    xerror(ex.GetErrorMessage());
    xerror(_T("\nInternal error reading column access rights for [%s.%s]\n"),m_schema.GetString(),m_object.GetString());
  }
}

void
XPort::WriteProcedureAccessRights(const XString& p_object,int& p_count)
{
  XString object(p_object);

  SQLInfoDB* info = m_database.GetSQLInfoDB();
  XString sql = info->GetPSMProcedurePrivilege(m_schema,object);

  if(sql.IsEmpty())
  {
    return;
  }

  try
  {
    SQLQuery query(m_database);
    query.DoSQLStatement(sql);
    while(query.GetRecord())
    {
      XString empty;
      XString grantee   = query[5];
      XString privilege = query[6];
      bool    grantable = _tcsicmp(query[7].GetAsString(),_T("YES")) == 0;

      XString grant = info->GetCATALOGGrantPrivilege(m_schema,object,empty,privilege,grantee,grantable);
      m_xfile.WriteSQL(grant);

      if((++p_count % COMMIT_SIZE) == 0)
      {
        _tprintf(_T("%d records processed\r"),p_count);
        _tprintf(_T("Exporting access rights: "));
      }
    }
  }
  catch(StdException& ex)
  {
    xerror(ex.GetErrorMessage());
    xerror(_T("\nInternal error reading procedure access rights for [%s.%s]\n"),m_schema.GetString(),m_object.GetString());
  }
}

void
XPort::WriteSequenceAccessRights(const XString& p_sequence,int& p_count)
{
  XString sequence(p_sequence);
  SQLInfoDB* info = m_database.GetSQLInfoDB();
  XString sql = info->GetCATALOGSequencePrivilege(m_schema,sequence);

  if(sql.IsEmpty())
  {
    return;
  }

  try
  {
    SQLQuery query(m_database);
    int param = 1;
    if(!m_schema.IsEmpty())
    {
      query.SetParameter(param++,m_schema);
    }
    if(!p_sequence.IsEmpty())
    {
      query.SetParameter(param++,sequence);
    }
    query.DoSQLStatement(sql);
    while(query.GetRecord())
    {
      XString empty;
      XString privilege = query[4];
      XString grantee   = query[5];
      bool    grantable = _tcsicmp(query[6].GetAsString(),_T("YES")) == 0;

      XString grant = info->GetCATALOGGrantPrivilege(m_schema,sequence,empty,privilege,grantee,grantable);
      m_xfile.WriteSQL(grant);

      if((++p_count % COMMIT_SIZE) == 0)
      {
        _tprintf(_T("%d records processed\r"),p_count);
        _tprintf(_T("Exporting access rights: "));
      }
    }
  }
  catch(StdException& ex)
  {
    xerror(ex.GetErrorMessage());
    xerror(_T("\nInternal error reading sequence access rights for [%s.%s]\n"),m_schema.GetString(),m_object.GetString());
  }
}

//////////////////////////////////////////////////////////////////////////
//
// HIGH LEVEL EXPORT
//
//////////////////////////////////////////////////////////////////////////

void
XPort::ExportUserTypes()
{
  m_xfile.WriteSection(_T("USERTYPES"));
  GetAllUserTypes();
  for(auto& type : m_usertypes)
  {
    m_xfile.WriteUserType(type);
    XString sql = GetDefineSQLUserType(type);
    PostProcessSQL(sql);
    m_xfile.WriteSQL(sql);
  }
  m_xfile.WriteSectionEnd();
  m_xfile.Flush();
}

void
XPort::ExportTables()
{
  m_xfile.WriteSection(_T("TABLES"));
  for(auto& table : m_tables)
  {
    // STEP 5: Export of all columns in one (1) table
    m_xfile.WriteTable(table);
    XString sql = GetDefineSQLTable(table);
    m_xfile.WriteSQL(sql);
    m_xfile.WriteColumns(GetColumns());
    // STEP 6: Export of all data rows in one (1) table
    if(m_parameters.m_createSql)
    {
      xprintf(false,_T("\n"));
      m_xfile.WriteSectionEnd();
    }
    else
    {
      m_xfile.WriteRows(*this,table,m_parameters.m_rows);
    }
    m_xfile.Flush();
  }
  m_xfile.WriteSectionEnd();
}

void
XPort::ExportIndices()
{
  m_xfile.WriteSection(_T("INDICES"));

  for(auto& table : m_tables)
  {
    m_indices.clear();
    DDLS statements = GetDefineSQLIndex(table);

    for(auto& index : m_indices)
    {
      XString sql = statements.front();
      statements.pop_front();
      m_xfile.WriteIndex(table,index);
      m_xfile.WriteSQL(sql);
    }
  }
  m_xfile.WriteSectionEnd();
  m_xfile.Flush();
}

void
XPort::ExportPrimaryKeys()
{
  int ind = 1;
  m_xfile.WriteSection(_T("PRIMARY KEYS"));

  for(auto& table : m_tables)
  {
    DDLS statements = GetDefineSQLPrimary(table);
    if(statements.size() >= 1)
    {
      m_xfile.WriteConstraint(ind++,table,m_constraints.front());
      m_xfile.WriteSQL(statements.front());
    }
  }
  m_xfile.WriteSectionEnd();
  m_xfile.Flush();
}

void
XPort::ExportForeignKeys()
{
  int ind = 1;
  m_xfile.WriteSection(_T("FOREIGN KEYS"));

  for(auto& table : m_tables)
  {
    DDLS statements = GetDefineSQLForeigns(table);
    DDLS::iterator it = statements.begin();
    for(auto& foreign : m_constraints)
    {
      m_xfile.WriteConstraint(ind++,table,foreign);
      m_xfile.WriteSQL(*it++);
    }
  }
  m_xfile.WriteSectionEnd();
  m_xfile.Flush();
}

void
XPort::ExportDefaultConstraints()
{
  int ind = 0;
  m_xfile.WriteSection(_T("DEFAULTS"));
  SQLInfoDB* info = m_database.GetSQLInfoDB();

  for(auto& table : m_tables)
  {
    XString all;
    XString sql = info->GetCATALOGDefaultAttributes(m_schema,table,all);

    // No constraints to be found on this database
    if(sql.IsEmpty())
    {
      continue;
    }

    SQLQuery query(m_database);
    int param = 1;
    if(!m_schema.IsEmpty())
    {
      query.SetParameter(param++,m_schema);
    }
    query.SetParameter(param++,table);
    query.DoSQLStatement(sql);
    while(query.GetRecord())
    {
      XString constraint = query[4];
      XString column     = query[5];
      XString definition = query[6];

      m_xfile.WriteConstraint(ind++,table,constraint);
      XString def = info->GetCATALOGDefaultCreate(m_schema,table,constraint,column,definition);
      m_xfile.WriteSQL(def);
    }
  }
  m_xfile.WriteSectionEnd();
  m_xfile.Flush();

  // All check constraints
  xprintf(false,_T("Total default constraints: %d\n"),ind);
}

void
XPort::ExportCheckConstraints()
{
  int cons = 0;
  m_xfile.WriteSection(_T("CHECKS"));
  SQLInfoDB* info = m_database.GetSQLInfoDB();

  for(auto& table : m_tables)
  {
    XString all;
    XString sql = info->GetCATALOGCheckAttributes(m_schema,table,all,true);

    // No constraints to be found on this database
    if(sql.IsEmpty())
    {
      continue;
    }

    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery query(m_database);
      int param = 1;
      int rows  = 0;
      if(!m_schema.IsEmpty())
      {
        query.SetParameter(param++,m_schema);
      }
      query.SetParameter(param++,table);
      query.DoSQLStatement(sql);
      while(query.GetRecord())
      {
        XString constraint = query[MetaCheck_constraintname];
        XString definition = query[MetaCheck_condition];

        m_xfile.WriteConstraint(cons++,table,constraint);
        XString def = info->GetCATALOGCheckCreate(m_schema,table,constraint,definition);
        m_xfile.WriteSQL(def);
        ++rows;
      }
      if(rows)
      {
        break;
      }
      sql = info->GetCATALOGCheckAttributes(m_schema,table,all);
    }
  }
  m_xfile.WriteSectionEnd();
  m_xfile.Flush();

  // All check constraints
  xprintf(false,_T("Total check constraints: %d\n"),cons);
}

void
XPort::ExportViews()
{
  m_xfile.WriteSection(_T("VIEWS"));
  GetAllViews();
  for(auto& view : m_views)
  {
    m_xfile.WriteView(view);
    XString sql = GetDefineSQLView(view);
    PostProcessSQL(sql);
    m_xfile.WriteSQL(sql);
  }
  m_xfile.WriteSectionEnd();
  m_xfile.Flush();
}

void
XPort::ExportSequences()
{
  m_xfile.WriteSection(_T("SEQUENCES"));
  GetAllSequences();
  for(auto& sequence : m_sequences)
  {
    m_xfile.WriteSequence(sequence);
    XString sql = GetDefineSQLSequence(sequence);
    m_xfile.WriteSQL(sql);
  }
  m_xfile.WriteSectionEnd();
  m_xfile.Flush();
}

void
XPort::ExportProcedures()
{
  m_xfile.WriteSection(_T("PROCEDURES"));
  GetAllProcedures();
  for(auto& procedure : m_procedures)
  {
    m_xfile.WriteProcedure(procedure);
    XString sql = GetDefineSQLProcedure(procedure);
    PostProcessSQL(sql);
    m_xfile.WriteSQL(sql);
  }
  m_xfile.WriteSectionEnd();
  m_xfile.Flush();
}

void
XPort::ExportTriggers()
{
  // On import it does the same as procedures/functions
  m_xfile.WriteSection(_T("TRIGGERS"));
  GetAllTriggers();
  for(auto& trigger : m_triggers)
  {
    m_xfile.WriteProcedure(trigger);
    XString sql = GetDefineSQLTrigger(trigger);
    PostProcessSQL(sql);
    m_xfile.WriteSQL(sql);
  }
  m_xfile.WriteSectionEnd();
  m_xfile.Flush();
}

void
XPort::ExportSynonyms()
{
  m_xfile.WriteSection(_T("SYNONYMS"));

  GetAllSynonyms();
  for(auto& synonym : m_synonyms)
  {
    m_xfile.WriteSynonym(m_schema,synonym);
    XString sql = GetDefineSQLSynonym(synonym);
    m_xfile.WriteSQL(sql);
  }
  // End of all synonyms
  m_xfile.WriteSectionEnd();
  m_xfile.Flush();
}

void
XPort::ExportPrivileges()
{
  m_xfile.WriteSection(_T("ACCESS RIGHTS"));

  int count = 0;

  xprintf(false,_T("Exporting access rights: "));
  for(auto& table : m_tables)
  {
    WriteTableAccessRights (table,count);
    WriteColumnAccessRights(table,count);
  }
  for(auto& view : m_views)
  {
    WriteTableAccessRights (view,count);
    WriteColumnAccessRights(view,count);
  }
  for(auto& procedure : m_procedures)
  {
    WriteProcedureAccessRights(procedure,count);
  }
  for(auto& sequence : m_sequences)
  {
    WriteSequenceAccessRights(sequence,count);
  }
  xprintf(false,_T("%d records exported.\n"),count);

  m_xfile.WriteSectionEnd();
  m_xfile.Flush();
}

void
XPort::ExportComments()
{
  m_xfile.WriteSection(_T("COMMENTS"));

  xprintf(false,_T("Exporting comments: "));
  for(auto& comment : m_comments)
  {
    m_xfile.WriteSQL(comment);
  }
  // End of all synonyms
  m_xfile.WriteSectionEnd();
  m_xfile.Flush();
  xprintf(false,_T("%d records\n"),(int)m_comments.size());
}

void
XPort::RecordAllComments(DDLCreateTable& p_create)
{
  DDLS comments = p_create.GetCommentStatements();
  for(auto& comment : comments)
  {
    m_comments.push_back(comment);
  }
}

void
XPort::PostProcessSQL(XString& p_sql)
{
  if(m_database.GetDatabaseType() == DatabaseType::RDBMS_MARIADB ||
     m_database.GetDatabaseType() == DatabaseType::RDBMS_MYSQL)
  {
    bool quotes = true;
    XString search(m_schema);
    XString quote = m_database.GetSQLInfoDB()->GetIdentifierQuoteCharacter();
    if(quote.GetLength() == 1)
    {
      // These quotes: " ' ` 
      search = quote + search + quote;
    }
    else if(quote.GetLength() == 2)
    {
      // These quotes: [] or {}
      search = XString(quote[0]) + search + quote[1];
    }
    else
    {
      // No quotes
      quotes = false;
    }
    // Search for the longest strings first
    if(quotes)
    {
      p_sql.Replace(search,SCHEMA_MACRO);
    }
    p_sql.Replace(search,m_schema);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// HIGH LEVEL IMPORT
//
//////////////////////////////////////////////////////////////////////////

void
XPort::ImportDump()
{
  // STEP 3: As long as the end-marker is not reached
  TCHAR type = m_xfile.NextType();
  while(type != _T('X') && type != _TEOF)
  {
    // Expecting a section to read
    if(type != 'S')
    {
      xerror(_T("Broking import. Expecting a section header.\n"));
      break;
    }
    // Read and show the section name
    XString section_name = m_xfile.ReadSection();

    if(section_name.CompareNoCase(_T("USERTYPES")) == 0)
    {
      // STEP 4: Importing all user defined types
      ImportUserTypes(type);
    }
    else if(section_name.CompareNoCase(_T("TABLES")) == 0)
    {
      // STEP 5: IMporting tables
      ImportTables(m_parameters.m_listOnly,type);
    }
    else if(section_name.CompareNoCase(_T("INDICI")) == 0 ||
            section_name.CompareNoCase(_T("INDICES")) == 0)
    {
      // STEP 6: Import all indices
      ImportIndices(type);
    }
    else if(section_name.CompareNoCase(_T("PRIMARY KEYS")) == 0)
    {
      // STEP 7: Import all primary keys
      ImportPrimaryKeys(type);
    }
    else if(section_name.CompareNoCase(_T("FOREIGN KEYS")) == 0)
    {
      // STEP 8: Import all foreign keys
      ImportForeignKeys(type);
    }
    else if(section_name.CompareNoCase(_T("DEFAULTS")) == 0)
    {
      // STEP 9: Import all default constraints
      ImportDefaults(type);
    }
    else if(section_name.CompareNoCase(_T("CHECKS")) == 0)
    {
      // STEP 10: Import all check constraints/not-nulls
      ImportChecks(type);
    }
    else if(section_name.CompareNoCase(_T("VIEWS")) == 0)
    {
      // STEP 11: Import all views
      ImportViews(type);
    }
    else if(section_name.CompareNoCase(_T("SEQUENCES")) == 0)
    {
      // STEP 12: Importing all sequences
      ImportSequences(type);
    }
    else if(section_name.CompareNoCase(_T("PROCEDURES")) == 0)
    {
      // STEP 13: Importing all PL/SQL source
      ImportProcedures(type);
    }
    else if(section_name.CompareNoCase(_T("TRIGGERS")) == 0)
    {
      // STEP 14: Importing all PL/SQL source
      ImportTriggers(type);
    }
    else if(section_name.CompareNoCase(_T("SYNONYMS")) == 0)
    {
      // STEP 15: Importing synonyms
      ImportSynonyms(type);
    }
    else if(section_name.CompareNoCase(_T("ACCESS RIGHTS")) == 0)
    {
      // STEP 16: All rights on all objects (missing column rights)
      ImportRights(m_parameters.m_listOnly,type);
    }
    else if(section_name.CompareNoCase(_T("COMMENTS")) == 0)
    {
      // STEP 17: Import all comments
      ImportComments(type);
    }
    else
    {
      xerror(_T("ALARM: Unknown section named [%s]\n"),section_name.GetString());
      break;
    }
  }
  // To be sure: Read the end-of-export marker
  m_xfile.ReadEndExport();
  // Close import file
  m_xfile.Close();
}

//////////////////////////////////////////////////////////////////////////
//
// DETAILS OF IMPORT
//
//////////////////////////////////////////////////////////////////////////

void
XPort::ImportUserTypes(TCHAR& p_type)
{
  xprintf(false,_T("Importing user types\n"));
  SQLTransaction trans(GetDatabase(),_T("usertypes"));
  p_type = m_xfile.NextType();
  while(p_type != 'E')
  {
    XString sequence = m_xfile.ReadUserType();
    XString sql = m_xfile.ReadSQL();
    if(ImportObject(m_object,sequence))
    {
      ImportSQL(sql);
    }
    p_type = m_xfile.NextType();
  }
  m_xfile.ReadEnd();
  p_type = m_xfile.NextType();
  trans.Commit();
}

void
XPort::ImportTables(bool p_listOnly,TCHAR& p_type)
{
  xprintf(false,_T("Importing tables definitions and table data.\n"));
  // Do the tables section
  p_type = m_xfile.NextType();
  // STEP 4: while not at end of all tables
  while(p_type != 'E')
  {
    do
    {
      while(p_type != 'E')
      {
        // STEP 5a: create table from column definition
        bool haveTable = false;
        XString table = m_xfile.ReadTable(p_listOnly);
        XString sql = m_xfile.ReadSQL();
        m_xfile.ReadColumns(GetColumns());
        FindIdentity(sql);

        SQLTransaction trans(GetDatabase(),_T("table_") + table);
        if(ImportObject(m_object,table))
        {
          int code = ImportSQL(sql);
          if(code == 0 || code == -955)
          {
            // If succeeded or name already exists
            haveTable = true;
          }
        }
        trans.Commit();

        // STEP 5b: Import data rows in this table

        BulkImportPrepare(table);
        m_xfile.ReadRows(*this,table,haveTable,p_listOnly);
        BulkImportComplete(table);

        p_type = m_xfile.NextType();
      }
      m_xfile.ReadEnd();
      p_type = m_xfile.NextType();
    }
    while(p_type == 'T');
  }
  m_xfile.ReadEnd();
  p_type = m_xfile.NextType();
}

void
XPort::ImportIndices(TCHAR& p_type)
{
  xprintf(false,_T("Importing indices and unique constraints.\n"));

  p_type = m_xfile.NextType();
  while(p_type != 'E')
  {
    XString index = m_xfile.ReadIndex();
    XString sql = m_xfile.ReadSQL();
    if(ImportObject(m_object,index))
    {
      SQLTransaction trans(GetDatabase(),_T("index_") + index);
      ImportSQL(sql);
      trans.Commit();
    }
    p_type = m_xfile.NextType();
  }
  m_xfile.ReadEnd();
  p_type = m_xfile.NextType();
}

void
XPort::ImportPrimaryKeys(TCHAR& p_type)
{
  xprintf(false,_T("Enforcing primary key constraints.\n"));
  p_type = m_xfile.NextType();
  while(p_type != 'E')
  {
    XString constraint = m_xfile.ReadConstraint();
    XString sql = m_xfile.ReadSQL();
    if(ImportObject(m_object,constraint))
    {
      SQLTransaction trans(GetDatabase(),_T("primary_") + constraint);
      ImportSQL(sql);
      trans.Commit();
    }
    p_type = m_xfile.NextType();
  }
  m_xfile.ReadEnd();
  p_type = m_xfile.NextType();
}

void
XPort::ImportForeignKeys(TCHAR& p_type)
{
  xprintf(false,_T("Enforcing foreign key constraints.\n"));
  p_type = m_xfile.NextType();
  while(p_type != 'E')
  {
    XString constraint = m_xfile.ReadConstraint();
    XString sql = m_xfile.ReadSQL();
    if(ImportObject(m_object,constraint))
    {
      SQLTransaction trans(GetDatabase(),_T("foreign_") + constraint);
      ImportSQL(sql);
      trans.Commit();
    }
    p_type = m_xfile.NextType();
  }
  m_xfile.ReadEnd();
  p_type = m_xfile.NextType();
}

void
XPort::ImportDefaults(TCHAR& p_type)
{
  xprintf(false,_T("Enforcing default constraints for table columns.\n"));
  p_type = m_xfile.NextType();
  while(p_type != 'E')
  {
    XString constraint = m_xfile.ReadConstraint();
    XString sql = m_xfile.ReadSQL();
    if(ImportObject(m_object,constraint))
    {
//       SQLTransaction trans(GetDatabase(),"default_" + constraint);
//       ImportSQL(sql);
//       trans.Commit();
    }
    p_type = m_xfile.NextType();
  }
  m_xfile.ReadEnd();
  p_type = m_xfile.NextType();
}

void
XPort::ImportChecks(TCHAR& p_type)
{
  xprintf(false,_T("Enforcing check constraints and not-null constraints.\n"));
  p_type = m_xfile.NextType();
  while(p_type != 'E')
  {
    XString constraint = m_xfile.ReadConstraint();
    XString sql = m_xfile.ReadSQL();
    if(ImportObject(m_object,constraint))
    {
      SQLTransaction trans(GetDatabase(),_T("cons_") + constraint);
      ImportSQL(sql);
      trans.Commit();
    }
    p_type = m_xfile.NextType();
  }
  m_xfile.ReadEnd();
  p_type = m_xfile.NextType();
}

void
XPort::ImportViews(TCHAR& p_type)
{
  xprintf(false,_T("Importing views.\n"));
  p_type = m_xfile.NextType();
  while(p_type != 'E')
  {
    XString view = m_xfile.ReadView();
    XString sql = m_xfile.ReadSQL();
    if(ImportObject(m_object,view))
    {
      SQLTransaction trans(GetDatabase(),_T("view_") + view);
      ImportSQL(sql,true);
      trans.Commit();
    }
    p_type = m_xfile.NextType();
  }
  m_xfile.ReadEnd();
  p_type = m_xfile.NextType();
}

void
XPort::ImportSequences(TCHAR& p_type)
{
  xprintf(false,_T("Importing sequences\n"));
  SQLTransaction trans(GetDatabase(),_T("sequences"));
  p_type = m_xfile.NextType();
  while(p_type != 'E')
  {
    XString sequence = m_xfile.ReadSequence();
    XString sql = m_xfile.ReadSQL();
    if(ImportObject(m_object,sequence))
    {
      ImportSQL(sql);
    }
    p_type = m_xfile.NextType();
  }
  m_xfile.ReadEnd();
  p_type = m_xfile.NextType();
  trans.Commit();
}

void
XPort::ImportProcedures(TCHAR& p_type)
{
  xprintf(false,_T("Importing all PSM source for procedures and functions.\n"));
  p_type = m_xfile.NextType();
  while(p_type != 'E')
  {
    XString procedure = m_xfile.ReadProcedure();
    XString sql = m_xfile.ReadSQL();
    if(ImportObject(m_object,procedure))
    {
      SQLTransaction trans(GetDatabase(),_T("psm_") + procedure);
      ImportSQL(sql,true,_T("\n/"));
      trans.Commit();
    }
    p_type = m_xfile.NextType();
  }
  m_xfile.ReadEnd();
  p_type = m_xfile.NextType();
}

void
XPort::ImportTriggers(TCHAR& p_type)
{
  xprintf(false,_T("Importing all PSM source for triggers.\n"));
  p_type = m_xfile.NextType();
  while(p_type != 'E')
  {
    XString procedure = m_xfile.ReadProcedure();
    XString sql = m_xfile.ReadSQL();
    if(ImportObject(m_object,procedure))
    {
      SQLTransaction trans(GetDatabase(),_T("psm_") + procedure);
      ImportSQL(sql,true,_T("\n/"));
      trans.Commit();
    }
    p_type = m_xfile.NextType();
  }
  m_xfile.ReadEnd();
  p_type = m_xfile.NextType();
}

void
XPort::ImportSynonyms(TCHAR& p_type)
{
  xprintf(false,_T("Importing public synonyms.\n"));
  SQLTransaction trans(GetDatabase(),_T("synonyms"));

  p_type = m_xfile.NextType();
  while(p_type != 'E')
  {
    XString synonymType;
    XString synonymName;
    m_xfile.ReadSynonym(synonymType,synonymName);
    XString sql = m_xfile.ReadSQL();
    if(ImportObject(m_object,synonymName))
    {
      ImportSQL(sql);
    }
    p_type = m_xfile.NextType();
  }
  m_xfile.ReadEnd();
  p_type = m_xfile.NextType();
  trans.Commit();
}

void
XPort::ImportRights(bool p_listOnly,TCHAR& p_type)
{
  int count = 0;
  int errors = 0;
  
  p_type = m_xfile.NextType();
  // What we will do
  xprintf(false,_T("Importing access rights: "));
  if(p_listOnly)
  {
    xprintf(false,_T("\n"));
  }

  // Loop until end
  SQLTransaction trans(GetDatabase(),_T("privileges"));
  while(p_type != 'E')
  {
    ++count;
    XString sql = m_xfile.ReadSQL();

    ImportSQL(sql,true);

    p_type = m_xfile.NextType();
    if((count % COMMIT_SIZE) == 0)
    {
      _tprintf(_T("%d records processed\r"),count);
      _tprintf(_T("Importing access rights: "));
    }
  }
  trans.Commit();

  xprintf(false,_T("%d records imported."),count);
  if(errors)
  {
    xprintf(false,_T(" %d records failed."),errors);
  }
  xprintf(false,_T("\n"));
  m_xfile.ReadEnd();
  p_type = m_xfile.NextType();
}

void
XPort::ImportComments(TCHAR& p_type)
{
  int count = 0;
  int errors = 0;

  p_type = m_xfile.NextType();
  xprintf(false,_T("Importing comments: "));

  SQLTransaction trans(GetDatabase(),_T("comments"));
  while(p_type != 'E')
  {
    ++count;
    XString sql = m_xfile.ReadSQL();
    ImportSQL(sql,true);
    p_type = m_xfile.NextType();
    if((count % COMMIT_SIZE) == 0)
    {
      _tprintf(_T("%d records processed\r"),count);
      _tprintf(_T("Importing comments: "));
    }
  }
  trans.Commit();

  xprintf(false,_T("%d records imported."),count);
  if(errors)
  {
    xprintf(false,_T(" %d records failed."),errors);
  }
  xprintf(false,_T("\n"));
  m_xfile.ReadEnd();
  p_type = m_xfile.NextType();
}

// Import everything (object = empty) or 1 object only
bool
XPort::ImportObject(const XString& p_object,const XString& p_importing)
{
  if(p_object.IsEmpty())
  {
    return true;
  }
  if(p_importing.Find(p_object) >= 0)
  {
    return true;
  }
  return false;
}

// Service function: List or run SQL
int
XPort::ImportSQL(XString& p_sql,bool p_retries/*= false*/,const XString& p_delim /*=";"*/)
{
  if(m_parameters.m_listOnly)
  {
    p_sql.Replace(SCHEMA_MACRO,m_parameters.m_schema);
    xprint(true,p_sql.GetString());
    if(p_sql.Right(p_delim.GetLength()) != p_delim)
    {
      xprintf(true,_T("%s"),p_delim.GetString());
    }
    if(p_sql.Right(1) != _T("\n"))
    {
      xprintf(true,_T("\n"));
    }
    return 0;
  }
  else
  {
    return DoSQLStatement(p_sql,p_retries);
  }
}

// Find the identity of a column before bulk import
void
XPort::FindIdentity(XString p_sql)
{
  m_identity = false;
  if(p_sql.Find(_T("identity")) >= 0)
  {
    m_identity = true;
  }
}


// Prepare a table for bulk import and complete the bulk import
void
XPort::BulkImportPrepare(const XString& p_table)
{
  if(m_parameters.m_listOnly)
  {
    return;
  }

  XString batch = m_database.GetSQLInfoDB()->GetBulkImportPrefix(m_schema,p_table,m_identity);
  if(!batch.IsEmpty())
  {
    try
    {
       SQLQuery query(&m_database);
       SQLTransaction trans(&m_database,_T("prepare"));

       query.DoSQLStatementBatch(batch);
       trans.Commit();
    }
    catch(StdException& ex)
    {
      xerror(_T("Error while preparing table [%s] for bulk import: %s"),p_table.GetString(),ex.GetErrorMessage().GetString());
    }
  }
}

void
XPort::BulkImportComplete(const XString& p_table)
{
  if(m_parameters.m_listOnly)
  {
    return;
  }

  XString batch = m_database.GetSQLInfoDB()->GetBulkImportPostfix(m_schema,p_table,m_identity);
  if(!batch.IsEmpty())
  {
    try
    {
      SQLQuery query(&m_database);
      SQLTransaction trans(&m_database,_T("complete"));

      query.DoSQLStatementBatch(batch);
      trans.Commit();
    }
    catch(StdException& ex)
    {
      xerror(_T("Error while completing table [%s] bulk import: %s"),p_table.GetString(),ex.GetErrorMessage().GetString());
    }
  }
}

