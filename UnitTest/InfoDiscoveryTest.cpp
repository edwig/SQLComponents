////////////////////////////////////////////////////////////////////////
//
// File: InfoDiscoveryTest.cpp
//
// Copyright (c) 1998-2019 ir. W.E. Huisman
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
#include "stdafx.h"
#include "SQLComponents.h"
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include "SQLInfoDB.h"
#include "UnitTest.h"

namespace DatabaseUnitTest
{
  void CALLBACK LogPrint(void* p_context,const char* p_text);
  int  CALLBACK LogLevel(void* p_context);
  extern CString g_dsn;
  extern CString g_user;
  extern CString g_password;
  extern CString g_schema;

  TEST_CLASS(DiscoveryInfo)
  {

    TEST_METHOD(Discovery)
    {
      Logger::WriteMessage("SQLInfo Discovery test for tables and procedures");
      Logger::WriteMessage("================================================");

      InitSQLComponents();

      OpenDatabase();

      // Types of objects we recognize
      MetaTypes();
      // Discover a few tables
      TableDiscovery("master");
      TableDiscovery("detail");
      // Discover a few procedures
      ProcedureDiscovery("multinout");
      ProcedureDiscovery("getdecimal");
      // Try out Native translation of the driver
      TranslateSQLtoNative("SELECT {fn UCASE(description)} FROM detail");

      CloseDatabase();
    }

    void MetaTypes()
    {
      Logger::WriteMessage("Meta types that can be discovered");
      MetaType(META_CATALOGS,"catalogs");
      MetaType(META_SCHEMAS, "schemata");
      MetaType(META_TABLES,  "tables");
    }

    void MetaType(int p_type,CString p_name)
    {
      SQLInfoDB* info = m_database->GetSQLInfoDB();
      MMetaMap objects;
      CString  errors;

      if(info->MakeInfoMetaTypes(objects,errors,p_type))
      {
        number_of_tests++;

        for(auto& obj : objects)
        {
          Logger::WriteMessage("Metatype " + p_name + " : " + obj.m_objectName);
          number_of_tests++;
        }
      }
      else
      {
        // Essentially not an error. It's an optional feature of ODBC
        Logger::WriteMessage("Cannot get the meta types for: " + p_name);
        number_of_tests++;
      }
    }

    void TableDiscovery(CString p_table)
    {
      Logger::WriteMessage("\nDO TABLE DISCOVERY: " + p_table);

      SQLInfoDB* info = m_database->GetSQLInfoDB();
      MTableMap tables;
      CString   errors;

      if(info->MakeInfoTableTable(tables,errors,"",p_table))
      {
        number_of_tests++;

        for(auto& table : tables)
        {
          CString schema  = table.m_schema;
          CString tabname = table.m_table;

          Logger::WriteMessage("Table found: " + table.m_fullName);

          ColumnsDiscovery(info,schema,tabname);
          PrimaryDiscovery(info,schema,tabname);
          ForeignDiscovery(info,schema,tabname);
          IndicesDiscovery(info,schema,tabname);
          SpecialDiscovery(info,schema,tabname);
          TriggerDiscovery(info,schema,tabname);
          TabPrivDiscovery(info,schema,tabname);
        }
      }
      else if(!errors.IsEmpty())
      {
        Assert::Fail(L"Cannot get info for table");
        number_of_tests++;
      }
    }

    void ColumnsDiscovery(SQLInfoDB* p_info,CString p_schema,CString p_table)
    {
      CString    errors;
      MColumnMap columns;

      if(p_info->MakeInfoTableColumns(columns,errors,p_schema,p_table))
      {
        number_of_tests++;

        for(auto& column : columns)
        {
          Logger::WriteMessage("Column: " + column.m_column);
          number_of_tests++;
        }
      }
      else if(!errors.IsEmpty())
      {
        Assert::Fail(L"Cannot get column info for table");
        number_of_tests++;
      }
    }

    void PrimaryDiscovery(SQLInfoDB* p_info,CString p_schema,CString p_table)
    {
      MPrimaryMap primaries;
      CString     errors;

      if(p_info->MakeInfoTablePrimary(primaries,errors,p_schema,p_table))
      {
        number_of_tests++;

        for(auto& primary : primaries)
        {
          Logger::WriteMessage("Primary key constraint: " + primary.m_constraintName);

          CString text;
          text.Format("Primary key %d: %s"
                      ,primary.m_columnPosition
                      ,primary.m_columnName);
          Logger::WriteMessage(text);
          number_of_tests++;
        }
      }
      else if(!errors.IsEmpty())
      {
        Assert::Fail(L"Cannot get primary key info for table");
        number_of_tests++;
      }
    }
    
    void ForeignDiscovery(SQLInfoDB* p_info,CString p_schema,CString p_table)
    {
      MForeignMap references;
      CString     errors;

      if(p_info->MakeInfoTableForeign(references,errors,p_schema,p_table))
      {
        number_of_tests++;
        for(auto& ref : references)
        {
          Logger::WriteMessage("Foreign: " + ref.m_foreignConstraint);
          number_of_tests++;
        }
      }
      else if (!errors.IsEmpty())
      {
        Assert::Fail(L"Cannot get foreign key info for table");
      }
    }

    void IndicesDiscovery(SQLInfoDB* p_info,CString p_schema,CString p_table)
    {
      MIndicesMap statistics;
      CString        errors;

      if(p_info->MakeInfoTableStatistics(statistics,errors,p_schema,p_table,nullptr))
      {
        number_of_tests++;

        for(auto& ind : statistics)
        {
          Logger::WriteMessage("Indices: " + ind.m_indexName);
          number_of_tests++;
        }
      }
      else if(!errors.IsEmpty())
      {
        Assert::Fail(L"Cannot get statistics info for table");
      }
    }

    void SpecialDiscovery(SQLInfoDB* p_info,CString p_schema,CString p_table)
    {
      MSpecialsMap specials;
      CString errors;
      if(p_info->MakeInfoTableSpecials(specials,errors,p_schema,p_table))
      {
        number_of_tests++;

        for(auto& spec : specials)
        {
          Logger::WriteMessage("Specials: " + spec.m_columnName);
          number_of_tests++;
        }
      }
      else if(!errors.IsEmpty())
      {
        Assert::Fail(L"Cannot get special info for table");
      }
    }

    void TriggerDiscovery(SQLInfoDB* p_info,CString p_schema,CString p_table)
    {
      MTriggerMap triggers;
      CString errors;

      if(p_info->MakeInfoTableTriggers(triggers,errors,p_schema,p_table))
      {
        CString line;

        number_of_tests++;
        for(auto& trigger : triggers)
        {
          line.Format("Trigger: [%d] %s",trigger.m_position,trigger.m_triggerName);
          Logger::WriteMessage(line);

          line.Format("Trigger fires: %s",trigger.m_before ? "before" : "after");
          Logger::WriteMessage(line);

          // Triggers for which statement
          line.Format("Trigger DML INSERT: %s",trigger.m_insert ? "yes" : "no");
          Logger::WriteMessage(line);
          line.Format("Trigger DML UPDATE: %s",trigger.m_update ? "yes" : "no");
          Logger::WriteMessage(line);
          line.Format("Trigger DML DELETE: %s",trigger.m_delete ? "yes" : "no");
          Logger::WriteMessage(line);
          line.Format("Trigger DML SELECT: %s",trigger.m_select ? "yes" : "no");
          Logger::WriteMessage(line);

          line.Format("Trigger source: %s",trigger.m_source);
          Logger::WriteMessage(line);
          number_of_tests++;
        }
      }
      else if(!errors.IsEmpty())
      {
        Assert::Fail(L"Cannot get the trigger info for table");
      }
    }

    void TabPrivDiscovery(SQLInfoDB* p_info,CString p_schema,CString p_table)
    {
      MPrivilegeMap privileges;
      CString errors;

      if(p_info->MakeInfoTablePrivileges(privileges,errors,p_schema,p_table))
      {

        number_of_tests++;
        for(auto& priv : privileges)
        {
          CString line;
          line.Format("Table privilege: %s was granted %s by %s"
                     ,priv.m_grantee
                     ,priv.m_privilege
                     ,priv.m_grantor);
          Logger::WriteMessage(line);
          number_of_tests++;
        }
      }
      else if(!errors.IsEmpty())
      {
        Assert::Fail(L"Cannot get table privileges for table");
      }
    }

    void ProcedureDiscovery(CString p_procedure)
    {
      Logger::WriteMessage("\nDO PROCEDURE DISCOVERY: " + p_procedure);

      SQLInfoDB* info = m_database->GetSQLInfoDB();

      MProcedureMap procedures;
      CString errors;
      if(info->MakeInfoPSMProcedures(procedures,errors,"",p_procedure))
      {
        number_of_tests++;
        for(auto& proc : procedures)
        {
          Logger::WriteMessage("Procedure : " + proc.m_procedureName);
          ParametersDiscovery(info,proc.m_schemaName,proc.m_procedureName);
          number_of_tests++;
        }
      }
      else if(!errors.IsEmpty())
      {
        Logger::WriteMessage("Cannot find procedure: " + p_procedure);
      }
    }

    void ParametersDiscovery(SQLInfoDB* p_info,CString p_schema,CString p_procedure)
    {
      MParameterMap params;
      CString errors;

      if(p_info->MakeInfoPSMParameters(params,errors,p_schema,p_procedure))
      {
        number_of_tests++;
        for(auto& parm : params)
        {
          Logger::WriteMessage("Parameter : " + parm.m_parameter);
          number_of_tests++;
        }
      }
      else if(!errors.IsEmpty())
      {
        Assert::Fail(L"Cannot get parameters for procedure");
      }
    }

    void TranslateSQLtoNative(CString p_sql)
    {
      SQLInfoDB* info = m_database->GetSQLInfoDB();
      CString translated = info->NativeSQL(NULL,p_sql);

      Logger::WriteMessage("TRANSLATE : " + p_sql);
      Logger::WriteMessage("Translated: " + translated);
      number_of_tests++;
    }

//////////////////////////////////////////////////////////////////////////

    bool OpenDatabase()
    {
      Logger::WriteMessage("Opening database....");

      m_database = new SQLDatabase();
      m_database->RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");

      try
      {
        m_database->SetMARS(false);
        m_database->SetLoginTimeout(0);
        if(m_database->Open(g_dsn,g_user,g_password))
        {
          Logger::WriteMessage("Database opened.");
        }
        else
        {
          Assert::Fail(L"Database ***NOT*** opened.");
        }
      }
      catch(StdException& er)
      {
        Logger::WriteMessage(er.GetErrorMessage());
        Assert::Fail(L"Database ***NOT*** opened.");
      }
      number_of_tests++;
      return true;
    }

    void CloseDatabase()
    {
      if(m_database)
      {
        Logger::WriteMessage("Closing database....");
        m_database->Close();
        delete m_database;
        m_database = nullptr;
      }
      number_of_tests++;
    }

  private:
    SQLDatabase* m_database { nullptr };
  };
}
