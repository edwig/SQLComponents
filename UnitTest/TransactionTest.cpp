////////////////////////////////////////////////////////////////////////
//
// File: TransactionTest.cpp
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
// Version number: See SQLComponents.h
//
#include "stdafx.h"
#include "SQLDatabase.h"
#include "SQLTransaction.h"
#include "SQLQuery.h"
#include "bcd.h"
#include "UnitTest.h"

namespace DatabaseUnitTest
{
  void CALLBACK LogPrint(void* p_context,const TCHAR* p_text);
  int  CALLBACK LogLevel(void* p_context);
  extern XString g_dsn;
  extern XString g_user;
  extern XString g_password;

  TEST_CLASS(TransactionTest)
  {
    bool OpenDatabase()
    {
      Logger::WriteMessage(_T("Opening database...."));

      m_database = new SQLDatabase();
      m_database->RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)_T(""));

      try
      {
        m_database->SetMARS(false);
        m_database->SetLoginTimeout(0);
        if(m_database->Open(g_dsn,g_user,g_password))
        {
          Logger::WriteMessage(_T("Database opened."));
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
        Logger::WriteMessage(_T("Closing database...."));
        m_database->Close();
        delete m_database;
        m_database = nullptr;
        number_of_tests++;
      }
    }

    void UpdateRecord(int p_recno,bcd p_value)
    {
      SQLQuery query(m_database);
      XString sql(_T("UPDATE test_number\n")
                  _T("   SET field3 = ?\n")
                  _T(" WHERE id     = ?"));
      query.SetParameter(1,p_value);
      query.SetParameter(2,p_recno);
      int num = query.DoSQLStatementNonQuery(sql);
      Assert::AreEqual(1,num,L"Record update not counted");
      number_of_tests++;
    }

    void CheckRecord(int p_recno,bcd p_value)
    {
      SQLQuery query(m_database);
      XString sql(_T("SELECT field3\n")
                  _T("  FROM test_number\n")
                  _T(" WHERE id = ?"));
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
      number_of_tests++;
    }

    TEST_METHOD(TransactionCommit)
    {
      Logger::WriteMessage(_T("Testing standard COMMIT transaction"));

      InitSQLComponents();

      if(OpenDatabase())
      {
        SQLTransaction trans(m_database,_T("TransCommit"));
        UpdateRecord(1,bcd(100.0));
        CheckRecord (1,bcd(100.0));
        UpdateRecord(1,bcd(200.0));
        CheckRecord (1,bcd(200.0));
        trans.Commit();

        CheckRecord(1,bcd(200.0));

        CloseDatabase();
      }
    }

    TEST_METHOD(TransactionRollback)
    {
      Logger::WriteMessage(_T("Testing standard ROLLBACK transaction"));

      InitSQLComponents();

      if(OpenDatabase())
      {
        SQLTransaction trans(m_database,_T("TransCommit"));
        UpdateRecord(1,bcd(100.0));
        CheckRecord (1,bcd(100.0));
        trans.Commit();
        CheckRecord (1,bcd(100.0));

        // Extra scope
        {
          SQLTransaction trans(m_database,_T("TransRollback"));
          UpdateRecord(1,bcd(400.0));
          CheckRecord (1,bcd(400.0));
        }
        // Extra transaction is out of scope and should have done a rollback
        CheckRecord(1,bcd(100.0));

        CloseDatabase();
      }
    }

    TEST_METHOD(TransactionCommit_DBC)
    {
      Logger::WriteMessage(_T("Testing isolated COMMIT transaction"));

      InitSQLComponents();

      if(OpenDatabase())
      {
        SQLTransaction trans(m_database->GetDBHandle(),true);
        UpdateRecord(1,bcd(100.0));
        CheckRecord (1,bcd(100.0));
        UpdateRecord(1,bcd(200.0));
        CheckRecord (1,bcd(200.0));
        trans.Commit();

        CheckRecord(1,bcd(200.0));

        CloseDatabase();
      }
    }

    TEST_METHOD(TransactionRollback_DBC)
    {
      Logger::WriteMessage(_T("Testing isolated ROLLBACK transaction"));

      InitSQLComponents();

      if(OpenDatabase())
      {
        SQLTransaction trans(m_database->GetDBHandle(),true);
        UpdateRecord(1,bcd(100.0));
        CheckRecord (1,bcd(100.0));
        trans.Commit();
        CheckRecord (1,bcd(100.0));

        // Extra scope
        {
          SQLTransaction trans(m_database->GetDBHandle(),true);
          UpdateRecord(1,bcd(400.0));
          CheckRecord (1,bcd(400.0));
        }
        // Extra transaction is out of scope and should have done a rollback
        CheckRecord(1,bcd(100.0));

        CloseDatabase();
      }
    }

    TEST_METHOD(TransactionRollback_FAIL)
    {
      Logger::WriteMessage(_T("Testing the FAILING of the ROLLBACK"));

      InitSQLComponents();

      if(OpenDatabase())
      {
        // Extra scope
        {
          SQLTransaction trans(m_database,_T("Check"));
          UpdateRecord(1,bcd(100.0));
          CheckRecord (1,bcd(100.0));
          trans.Commit();
          CheckRecord (1,bcd(100.0));
        }

        try
        {
          // Extra scope
          {
            SQLTransaction trans(m_database,_T("OpenTransaction"));
            UpdateRecord(1,bcd(400.0));
            CheckRecord (1,bcd(400.0));
            CloseDatabase();
          }
        }
        catch(StdException& er)
        {
          Logger::WriteMessage(_T("Error in transaction: ") + er.GetErrorMessage());
        }
      }

      if(OpenDatabase())
      {
        // Extra transaction is out of scope and should have done a rollback
        CheckRecord(1,bcd(100.0));
        CloseDatabase();
      }
    }

    SQLDatabase* m_database = nullptr;
  };
}
