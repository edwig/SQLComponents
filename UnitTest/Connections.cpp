////////////////////////////////////////////////////////////////////////
//
// File: Connections.cpp
//
// Copyright (c) 1998-2018 ir. W.E. Huisman
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
// Last Revision:  15-06-2019
// Version number: 1.5.5
//
#include "stdafx.h"
#include "SQLDatabasePool.h"
#include "SQLQuery.h"
#include "SQLAutoDBS.h"
#include "UnitTest.h"

namespace ConnectionsUnitTest
{
TEST_CLASS(SQLConnectionsTest)
{
public:
  TEST_METHOD(AddConnections)
  {
    // Method does no test, just fills the value arrays
    Logger::WriteMessage("Adding connections to the SQLConnections store");

    SQLConnections conn;
    FillConnections(conn);

    SQLConnection* theConn = conn.GetConnection("Other");
    if(theConn)
    {
       Assert::AreEqual(theConn->m_password,"user_password");
       ++number_of_tests;
    }
    else
    {
      Assert::Fail(L"Stored connection not found");
    }
  }

  TEST_METHOD(WriteConnections)
  {
    // Method does no test, just fills the value arrays
    Logger::WriteMessage("Writing connections to the XML file");

    SQLConnections conn;
    FillConnections(conn);

    bool result = conn.SaveConnectionsFile();
    Assert::IsTrue(result);
    ++number_of_tests;
  }

  TEST_METHOD(ReadConnections)
  {
    // Method does no test, just fills the value arrays
    Logger::WriteMessage("Writing connections to the XML file");

    SQLConnections conn;
    FillConnections(conn);

    bool result = conn.SaveConnectionsFile();
    Assert::IsTrue(result);

    SQLConnections other;
    result = other.LoadConnectionsFile();
    Assert::IsTrue(result);

    SQLConnection* theConn = other.GetConnection("Other");
    if(theConn)
    {
      Assert::AreEqual(theConn->m_password,"user_password");
      ++number_of_tests;
    }
    else
    {
      Assert::Fail(L"Stored Written/Read-in connection not found");
    }
  }

  TEST_METHOD(PoolTesting)
  {
    // Method does no test, just fills the value arrays
    Logger::WriteMessage("Testing the SQLDatabasePool");

    SQLComponents::InitSQLComponents(LN_ENGLISH);

    SQLConnections conn;
    FillConnections(conn);
    conn.SaveConnectionsFile();

    // Extra scope to test auto pointer
    {
      SQLAutoDBS dbs(m_pool,"MyConn");
      Assert::IsNotNull(dbs.get());

      CString sql("SELECT description FROM DETAIL where ID = 7");
      SQLQuery query(dbs);
      query.DoSQLStatement(sql);
      if(query.GetRecord())
      {
        CString description = query[1];
        Assert::AreEqual(description, "Fourth line of second invoice");
      }
    }
    Assert::AreEqual(m_pool.GetConnections(),(unsigned) 1);
    ++number_of_tests;
  }

  void FillConnections(SQLConnections& p_conn)
  {
    p_conn.AddConnection("MyConn","testing", "sysdba","altijd",       "DBA=W;");
    p_conn.AddConnection("Other", "testing", "user",  "user_password","DBA=R;");
  }

  SQLDatabasePool m_pool;
};

}
