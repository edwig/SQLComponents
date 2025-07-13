////////////////////////////////////////////////////////////////////////
//
// File: XPort.h
//
// Copyright (c) 1998-2025 ir. W.E. Huisman
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
// Version number: See SQLXport.h
//
#pragma once
#include "XFile.h"
#include "Parameters.h"
#include <SQLDatabase.h>
#include <DDLCreateTable.h>
#include <vector>

using namespace SQLComponents;

class Parameters;

// Constraint types
typedef enum _constraints
{
  CONS_PRIMARY = 1
 ,CONS_FOREIGN
 ,CONS_CHECK
}
ConsType;

const int COMMIT_SIZE = 100;

class XPort
{
public:
  XPort(Parameters& p_parameters);
 ~XPort();

  void    Export();
  void    Import();

  // Starting and stopping
  bool    Connect();
  bool    Disconnect();
  bool    OpenDumpFileWrite();
  bool    OpenDumpFileRead();
  bool    CloseDumpFile();

  // Commands
  int     GetAllTables();
  int     GetAllCheckConstraints();
  int     GetAllViews();
  int     GetAllSequences();
  int     GetAllProcedures();
  int     GetAllTriggers();
  int     GetAllSynonyms();
  int     DoSQLStatement(XString& p_sql,bool p_can_retry = false);
  void    DropSchema();
  bool    ConsistentTables (XString p_schema);
  void    PerformStatistics();
  void    PerformRecompile();
  void    PerformRetries();

  // EXPORT
  void    ExportTables();
  void    ExportIndices();
  void    ExportPrimaryKeys();
  void    ExportForeignKeys();
  void    ExportDefaultConstraints();
  void    ExportCheckConstraints();
  void    ExportViews();
  void    ExportSequences();
  void    ExportProcedures();
  void    ExportTriggers();
  void    ExportSynonyms();
  void    ExportRights();

  // IMPORT
  void    ImportDump();

  // Schema SQL
  XString GetDefineSQLTable   (XString p_table);
  DDLS    GetDefineSQLIndex   (XString p_table);
  DDLS    GetDefineSQLPrimary (XString p_table);
  DDLS    GetDefineSQLForeigns(XString p_table);

  XString GetDefineSQLView     (XString p_view);
  XString GetDefineSQLSequence (XString p_sequence);
  XString GetDefineSQLProcedure(XString p_procedure);
  XString GetDefineSQLTrigger  (XString p_trigger);
  XString GetDefineSQLSynonym  (XString p_synonym);
  // Data SQL
  XString GetDefineCountSelect(XString p_table,SQLInfoDB* p_info);
  XString GetDefineRowSelect  (XString p_table,SQLInfoDB* p_info);
  XString GetDefineRowInsert  (XString p_table,SQLInfoDB* p_info);

  // Getters
  OList*        GetColumns();
  SQLDatabase*  GetDatabase();

private:
  // Exporting
  void          WriteTableAccessRights (XString p_object,int& p_count);
  void          WriteColumnAccessRights(XString p_object,int& p_count);
  void          WriteProcedureAccessRights(XString p_object,int& p_count);
  void          WriteSequenceAccessRights(XString p_sequence,int& p_count);
  void          ShowRetryName(XString& p_sql);
  bool          ImportObject(XString& p_object,XString& p_importing);
  int           ImportSQL(XString& p_sql,bool p_retries = false,XString p_delim = _T(";"));
  void          RecordAllColumns  (DDLCreateTable& p_create);
  void          RecordAllIndices  (DDLCreateTable& p_create,DDLS& p_ddls);
  void          RecordAllPrimaries(DDLCreateTable& p_create,CString p_table);
  void          RecordAllForeigns (DDLCreateTable& p_create);
  // Drop old contents
  void          GatherDropSchema (OList& p_statements);
  void          ExecuteDropSchema(OList& p_statements);
  // Importing
  void          ImportTables(bool p_listOnly,TCHAR& p_type);
  void          ImportIndices(TCHAR& p_type);
  void          ImportPrimaryKeys(TCHAR& p_type);
  void          ImportForeignKeys(TCHAR& p_type);
  void          ImportDefaults(TCHAR& p_type);
  void          ImportChecks(TCHAR& p_type);
  void          ImportViews(TCHAR& p_type);
  void          ImportSequences(TCHAR& p_type);
  void          ImportProcedures(TCHAR& p_type);
  void          ImportSynonyms(TCHAR& p_type);
  void          ImportRights(bool p_listOnly,TCHAR& p_type);

  // All the parameters
  Parameters& m_parameters;
  // File to read/write the dump to/from
  XFile       m_xfile;
  // Database to get the export from or do the import in
  SQLDatabase m_database;
  // What to export/import
  XString     m_schema;
  XString     m_object;
  // Object lists with objects
  OList       m_tables;
  OList       m_columns;
  OList       m_indices;
  OList       m_constraints;
  OList       m_views;
  OList       m_sequences;
  OList       m_procedures;
  OList       m_triggers;
  OList       m_synonyms;
  // Retry queue for views
  OList       m_retries;
};

inline OList*
XPort::GetColumns()
{
  return &m_columns;
}

inline SQLDatabase*
XPort::GetDatabase()
{
  return &m_database;
}

