////////////////////////////////////////////////////////////////////////
//
// File: TestCalling.cpp
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
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include "SQLTransaction.h"
#include "bcd.h"
#include "UnitTest.h"

namespace DatabaseUnitTest
{
  void CALLBACK LogPrint(void* p_context,const char* p_text);
  int  CALLBACK LogLevel(void* p_context);
  extern CString g_dsn;
  extern CString g_user;
  extern CString g_password;
  extern CString g_schema;

  TEST_CLASS(TestCallingFunctions)
  {
    TEST_METHOD(TestCallFunctions)
    {
      InitSQLComponents();

      OpenDatabase();
      ResetResultSet();
      TestCalling();
      CheckResultSet();
      CloseDatabase();
    }

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
        number_of_tests++;
      }
    }

    void ResetResultSet()
    {
      SQLQuery query(m_database);
      SQLTransaction trans(m_database,"DelSet");
      CString sql = "DELETE FROM test_record";
      query.DoSQLStatementNonQuery(sql);
      trans.Commit();
      number_of_tests++;
    }

    void CheckResultSet()
    {
      int programCount  = 0;
      int duplicatCount = 0;

      CString dup = m_duplicate + m_duplicate;

      SQLQuery query(m_database);
      SQLTransaction trans(m_database,"DelSet");
      CString sql = "SELECT * FROM test_record";
      query.DoSQLStatement(sql);
      while(query.GetRecord())
      {
        CString name = query[1];

        if(name.Compare(m_program) == 0) ++programCount;
        if(name.Compare(dup)       == 0) ++duplicatCount;
      }
      trans.Commit();

      // Both records need to show up ONCE!
      Assert::AreEqual(1,programCount);
      Assert::AreEqual(1,duplicatCount);

      number_of_tests += 2;
    }

    void TestCalling()
    {
      CString text;
      Logger::WriteMessage("Testing FUNCTION CALL interface...");

      // Call with 1 input and 1 output parameter
      SQLQuery query(m_database);
      SQLTransaction trans(m_database,"testing");

      var* result = query.DoSQLCall(g_schema,"testmul2",12);

      text.Format("Result of the function call (1  input parameter): %d",result->GetAsSLong());
      Logger::WriteMessage(text);
      Assert::AreEqual(24,(int)result->GetAsSLong());
      number_of_tests++;


      // Call with 1 input parameter with SQL input
      SQLQuery q2(m_database);
      result = q2.DoSQLCall(g_schema,"testins",m_program.GetString());
      text.Format("Result of the function call (0 output parameter): %d",result->GetAsSLong());
      Logger::WriteMessage(text);

      // Call with 1 input parameter and a BCD return parameter
      q2.ResetParameters();
      q2.SetParameter(0,bcd(),P_SQL_PARAM_OUTPUT);
      q2.GetParameter(0)->SetNumericPrecisionScale(18,2);
      var* res = q2.DoSQLCall(g_schema,"getdecimal","345.99");
      text.Format("Result of GETDECIMAL '345.99' = [%s]",res->GetAsBCD().AsString());
      Logger::WriteMessage(text);
      Assert::AreEqual("345.99",res->GetAsBCD().AsString());
      number_of_tests++;

      // Call with 1 input parameter and return value AND return parameter
      var txt(SQL_C_CHAR,200);
      q2.ResetParameters();
      q2.SetParameter(0,bcd(),P_SQL_PARAM_OUTPUT);
      q2.GetParameter(0)->SetNumericPrecisionScale(18,2);
      q2.SetParameter(1,m_duplicate);
      q2.SetParameter(2,&txt,P_SQL_PARAM_OUTPUT);

      res = q2.DoSQLCall(g_schema,"multinout",true);

      bcd number = res->GetAsBCD();
      CString restext = q2.GetParameter(2)->GetAsChar();
      text.Format("Result of MULTINOUT: [%s] [%s]\n",number.AsString(),restext.GetString());
      Logger::WriteMessage(text);

      Assert::AreEqual(77.88,number.AsDouble());
      CString dub = m_duplicate + m_duplicate;
      Assert::AreEqual(dub.GetString(),restext.GetString());
      number_of_tests++;

      trans.Commit();
      number_of_tests++;
    }

  private:
    SQLDatabase* m_database  = nullptr;
    CString      m_program   = "Testing from within a program." ;
    CString      m_duplicate = "Multiple duplicate testing.";
  };
}

