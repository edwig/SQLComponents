////////////////////////////////////////////////////////////////////////
//
// File: InfoDiscoveryTest.cpp
//
// Copyright (c) 1998-2017 ir. W.E. Huisman
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
// Last Revision:   08-01-2017
// Version number:  1.4.0
//
#include "stdafx.h"
#include "SQLComponents.h"
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include "SQLInfoDB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SQLComponents;

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
      WordList list;

      if(info->MakeInfoMetaTypes(&list,p_type))
      {
        for(auto& word : list)
        {
          Logger::WriteMessage("Metatype " + p_name + " : " + word);
        }
      }
      else
      {
        Assert::Fail(L"Cannot get the meta types");
      }
    }

    void TableDiscovery(CString p_table)
    {
      Logger::WriteMessage("\nDO TABLE DISCOVERY: " + p_table);

      SQLInfoDB* info = m_database->GetSQLInfoDB();

      WordList remarks;
      WordList tables;

      if(info->MakeInfoTableTablepart(&remarks,&tables,p_table))
      {
        for(auto& remark : remarks)
        {
          Logger::WriteMessage("Remark: " + remark);
        }
        for(auto& table : tables)
        {
          Logger::WriteMessage("Table found: " + table);
          ColumnsDiscovery(info);
          PrimaryDiscovery(info);
          ForeignDiscovery(info);
          IndicesDiscovery(info);
          SpecialDiscovery(info);
          TabPrivDiscovery(info);
        }
      }
      else
      {
        Assert::Fail(L"Cannot get info for table");
      }
    }

    void ColumnsDiscovery(SQLInfoDB* p_info)
    {
      WordList columns;
      if(p_info->MakeInfoTableColumns(&columns))
      {
        for(auto& column : columns)
        {
          Logger::WriteMessage("Column: " + column);
        }
      }
      else
      {
        Assert::Fail(L"Cannot get column info for table");
      }
    }

    void PrimaryDiscovery(SQLInfoDB* p_info)
    {
      CString    primaryConstraint;
      PrimaryMap primaries;

      if(p_info->MakeInfoTablePrimary(nullptr,primaryConstraint,primaries))
      {
        Logger::WriteMessage("Primary key constraint: " + primaryConstraint);

        for(auto& primary : primaries)
        {
          CString text;
          text.Format("Primary key %d: %s"
                      ,primary.second.m_colPos
                      ,primary.second.m_colName);
          Logger::WriteMessage(text);
        }
      }
      else
      {
        Assert::Fail(L"Cannot get primary key info for table");
      }
    }
    
    void ForeignDiscovery(SQLInfoDB* p_info)
    {
      WordList references;
      if(p_info->MakeInfoTableForeign(&references))
      {
        for(auto& ref : references)
        {
          Logger::WriteMessage("Foreign: " + ref);
        }
      }
      else
      {
        Assert::Fail(L"Cannot get foreign key info for table");
      }
    }

    void IndicesDiscovery(SQLInfoDB* p_info)
    {
      WordList references;
      CString  keyname;
      PrimaryMap pmap;
      if(p_info->MakeInfoTableStatistics(&references,keyname,pmap))
      {
        for(auto& ref : references)
        {
          Logger::WriteMessage("Indices: " + ref);
        }
      }
      else
      {
        Assert::Fail(L"Cannot get statistics info for table");
      }
    }

    void SpecialDiscovery(SQLInfoDB* p_info)
    {
      WordList references;
      if(p_info->MakeInfoTableSpecials(&references))
      {
        for(auto& ref : references)
        {
          Logger::WriteMessage("Specials: " + ref);
        }
      }
      else
      {
        Assert::Fail(L"Cannot get special info for table");
      }
    }

    void TabPrivDiscovery(SQLInfoDB* p_info)
    {
      WordList references;
      if(p_info->MakeInfoTablePrivileges(&references))
      {
        for(auto& ref : references)
        {
          Logger::WriteMessage("Table privileges: " + ref);
        }
      }
      else
      {
        Assert::Fail(L"Cannot get table priviliges for table");
      }
    }

    void ProcedureDiscovery(CString p_procedure)
    {
      Logger::WriteMessage("\nDO PROCEDURE DISCOVERY: " + p_procedure);

      SQLInfoDB* info = m_database->GetSQLInfoDB();

      WordList list;
      if(info->MakeInfoProcedureProcedurepart(&list,p_procedure))
      {
        for(auto& word : list)
        {
          Logger::WriteMessage("Procedure : " + word);
        }
        ParametersDiscovery(info);
      }
      else
      {
        Logger::WriteMessage("Cannot find procedure: " + p_procedure);
      }
    }

    void ParametersDiscovery(SQLInfoDB* p_info)
    {
      WordList list;
      if(p_info->MakeInfoProcedureParameters(&list))
      {
        for(auto& word : list)
        {
          Logger::WriteMessage("Parameter : " + word);
        }
      }
      else
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
      catch(CString& s)
      {
        Logger::WriteMessage(s);
        Assert::Fail(L"Database ***NOT*** opened.");
      }
      catch(...)
      {
        Assert::Fail(L"Database ***NOT*** opened.");
      }
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
    }

  private:
    SQLDatabase* m_database { nullptr };
  };
}
