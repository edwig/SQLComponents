////////////////////////////////////////////////////////////////////////
//
// File: TransactionTest.cpp
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
#include "CppUnitTest.h"
#include "SQLDatabase.h"
#include "SQLTransaction.h"
#include "SQLQuery.h"
#include "bcd.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SQLComponents;

namespace DatabaseUnitTest
{
  void CALLBACK LogPrint(void* p_context,const char* p_text);
  int  CALLBACK LogLevel(void* p_context);
  extern CString g_dsn;
  extern CString g_user;
  extern CString g_password;

  TEST_CLASS(TransactionTest)
  {
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

    void UpdateRecord(int p_recno,bcd p_value)
    {
      SQLQuery query(m_database);
      CString sql("UPDATE test_number\n"
                  "   SET field3 = ?\n"
                  " WHERE id     = ?");
      query.SetParameter(1,p_value);
      query.SetParameter(2,p_recno);
      int num = query.DoSQLStatementNonQuery(sql);
      Assert::AreEqual(1,num,L"Record update not counted");
    }

    void CheckRecord(int p_recno,bcd p_value)
    {
      SQLQuery query(m_database);
      CString sql("SELECT field3\n"
                  "  FROM test_number\n"
                  " WHERE id = ?");
      query.DoSQLStatement(sql,p_recno);
      if(query.GetRecord())
      {
        bcd value = query[1];
        Assert::AreEqual(true,(value == p_value));
      }
      else
      {
        Assert::Fail(L"Cannot retrieve record from testtable");
      }
    }

    TEST_METHOD(TransactionCommit)
    {
      Logger::WriteMessage("Testing standard COMMIT transaction");

      InitSQLComponents();

      if(OpenDatabase())
      {
        SQLTransaction trans(m_database,"TransCommit");
        UpdateRecord(1,100.0);
        CheckRecord (1,100.0);
        UpdateRecord(1,200.0);
        CheckRecord (1,200.0);
        trans.Commit();

        CheckRecord(1,200.0);

        CloseDatabase();
      }
    }

    TEST_METHOD(TransactionRollback)
    {
      Logger::WriteMessage("Testing standard ROLLBACK transaction");

      InitSQLComponents();

      if(OpenDatabase())
      {
        SQLTransaction trans(m_database,"TransCommit");
        UpdateRecord(1,100.0);
        CheckRecord (1,100.0);
        trans.Commit();
        CheckRecord (1,100.0);

        // Extra scope
        {
          SQLTransaction trans(m_database,"TransRollback");
          UpdateRecord(1,400.0);
          CheckRecord (1,400.0);
        }
        // Extra transaction is out of scope and should have done a rollback
        CheckRecord(1,100.0);

        CloseDatabase();
      }
    }

    TEST_METHOD(TransactionCommit_DBC)
    {
      Logger::WriteMessage("Testing isolated COMMIT transaction");

      InitSQLComponents();

      if(OpenDatabase())
      {
        SQLTransaction trans(m_database->GetDBHandle(),true);
        UpdateRecord(1,100.0);
        CheckRecord(1,100.0);
        UpdateRecord(1,200.0);
        CheckRecord(1,200.0);
        trans.Commit();

        CheckRecord(1,200.0);

        CloseDatabase();
      }
    }

    TEST_METHOD(TransactionRollback_DBC)
    {
      Logger::WriteMessage("Testing isolated ROLLBACK transaction");

      InitSQLComponents();

      if(OpenDatabase())
      {
        SQLTransaction trans(m_database->GetDBHandle(),true);
        UpdateRecord(1,100.0);
        CheckRecord(1,100.0);
        trans.Commit();
        CheckRecord(1,100.0);

        // Extra scope
        {
          SQLTransaction trans(m_database->GetDBHandle(),true);
          UpdateRecord(1,400.0);
          CheckRecord(1,400.0);
        }
        // Extra transaction is out of scope and should have done a rollback
        CheckRecord(1,100.0);

        CloseDatabase();
      }
    }

    TEST_METHOD(TransactionRollback_FAIL)
    {
      Logger::WriteMessage("Testing the FAILING of the ROLLBACK");

      InitSQLComponents();

      if(OpenDatabase())
      {
        // Extra scope
        {
          SQLTransaction trans(m_database,"Check");
          UpdateRecord(1,100.0);
          CheckRecord(1,100.0);
          trans.Commit();
          CheckRecord(1,100.0);
        }

        try
        {
          // Extra scope
          {
            SQLTransaction trans(m_database,"OpenTransaction");
            UpdateRecord(1,400.0);
            CheckRecord(1,400.0);
            CloseDatabase();
          }
        }
        catch(CString& s)
        {
          Logger::WriteMessage("Error in transaction: " + s);
        }
      }

      if(OpenDatabase())
      {
        // Extra transaction is out of scope and should have done a rollback
        CheckRecord(1,100.0);
        CloseDatabase();
      }
    }

    SQLDatabase* m_database = nullptr;
  };
}