////////////////////////////////////////////////////////////////////////
//
// File: Connections.cpp
//
// Copyright (c) 1998-2024 ir. W.E. Huisman
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
       Assert::AreEqual(theConn->m_password,_T("user_password"));
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

    SQLConnection* theConn = other.GetConnection(_T("Other"));
    if(theConn)
    {
      Assert::AreEqual(theConn->m_password,_T("user_password"));
      ++number_of_tests;
    }
    else
    {
      Assert::Fail(L"Stored Written/Read-in connection not found");
    }
  }

  TEST_METHOD(PoolTesting)
  {
    Logger::WriteMessage("Testing the SQLDatabasePool");

    SQLComponents::InitSQLComponents(LN_ENGLISH);

    SQLConnections conn;
    FillConnections(conn);
    conn.SaveConnectionsFile();

    // Extra scope to test auto pointer
    {
      SQLAutoDBS dbs(m_pool,_T("MyConn"));
      Assert::IsNotNull(dbs.get());

      XString sql(_T("SELECT description FROM DETAIL where ID = 7"));
      SQLQuery query(dbs);
      query.DoSQLStatement(sql);
      if(query.GetRecord())
      {
        XString description = query[1];
        Assert::AreEqual(description, _T("Fourth line of second invoice"));
      }
    }
    Assert::AreEqual(m_pool.GetConnections(),(unsigned) 1);
    ++number_of_tests;
  }

  TEST_METHOD(CleanUpPool)
  {
    Logger::WriteMessage("Testing the cleanup of the SQLDatabasePool");

    // Open some connections to the database server
    PoolTesting();

    m_pool.Cleanup(true);

    Assert::AreEqual(m_pool.GetConnections(),(unsigned)0);
    ++number_of_tests;
  }

  TEST_METHOD(ReuseConnection)
  {
    Logger::WriteMessage("Testing the reuse of connections in the SQLDatabasePool");

    // Open some connections to the database server
    PoolTesting();
    PoolTesting();
    PoolTesting();
    PoolTesting();

    // Still just one connection live to the RDBMS
    Assert::AreEqual(m_pool.GetConnections(),(unsigned)1);
    ++number_of_tests;
  }

  void FillConnections(SQLConnections& p_conn)
  {
    p_conn.AddConnection(_T("MyConn"),_T("testing"), _T("sysdba"),_T("altijd"),       _T("DBA=W;"));
    p_conn.AddConnection(_T("Other"), _T("testing"), _T("user"),  _T("user_password"),_T("DBA=R;"));
  }

  // This is the database pool for connections
  SQLDatabasePool m_pool;
};

}
