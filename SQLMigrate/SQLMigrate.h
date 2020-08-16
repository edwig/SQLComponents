////////////////////////////////////////////////////////////////////////
//
// File: SQLMigrate.h
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
#pragma once
#include "SQLComponents.h"
#include "Logging.h"
#include "MigrateParameters.h"
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include "SQLMetaInfo.h"
#include "DDLCreateTable.h"
#include <vector>

namespace SQLComponents
{

class SQLMigrate  
{
public:
  SQLMigrate(MigrateParameters& p_params,Logging& p_log);
 ~SQLMigrate();

  bool     Migrate();
private:
  int      ReadTableStructures(CString p_owner,CString p_patroon,SQLDatabase* p_database);
  void     ReadTablesFromFile (CString& p_bestand);
  // Processing
  void     DropTables();
  void     CreateTables();
  void     FillTablesViaPump();
  void     FillTablesViaData(bool p_process);
  void     TruncateTables();

  void     FixupTableColumns(DDLCreateTable& p_create);
  void     FixupTableIndices(DDLCreateTable& p_create);

  // Statements
  CString  MakeSelectStatement    (CString& p_tabel,CString& p_user);
  CString  MakeInsertStatement    (CString& p_tabel,CString& p_user,CString& p_doel_user);
  CString  MakeInsertDataStatement(CString& p_tabel,CString& p_target_schema,SQLQuery& p_input,MColumnMap& kolommen);
  CString  MakeIdentityStatement  (CString p_vendor,CString p_user,CString p_tabel);

  CString  VariantToInsertString  (SQLVariant* p_var,int p_datatype);
  long     CountTableContents     (CString  p_owner,CString& tabel);
  void     CommitDDL(SQLQuery& p_query,SQLInfoDB* p_info);
  void     DatatypeExceptions(RebindMap& p_map);

  // Record missing records
  void     LogMissingRecord(SQLQuery& p_query,CString& p_error);

  typedef std::vector<CString> TablesMap;

  // DATA
  MigrateParameters m_params;
  Logging           m_log;

  int          m_directMigration;
  int          m_totalTables;
  SQLDatabase* m_databaseSource;
  SQLDatabase* m_databaseTarget;
  MTableMap    m_tables;
  RebindMap    m_rebinds;
};

}