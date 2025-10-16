////////////////////////////////////////////////////////////////////////
//
// File: TestNVarchar.cpp
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
// Version number: See SQLComponents.h
//
#include "stdafx.h"
#include "TestSQL.h"
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include <conio.h>

void SelectionsDBOMaster(SQLDatabase& p_dbs)
{
  SQLQuery query(p_dbs);
  XString sql = _T("SELECT id,oms_char,oms_wchar FROM dbo.master");
  query.DoSQLStatement(sql);

  _tprintf(_T("MASTER with NVARCHAR\n\n"));
  _tprintf(_T("ID   Description oms_char           Description oms_wchar\n"));
  _tprintf(_T("---- ------------------------------ ------------------------------\n"));
  while(query.GetRecord())
  {
    // Getting the data from the record
    int     id           = query[1];
    XString description1 = query[2];
    XString description2 = query[3];

    // Printing
    _tprintf(_T("%-4.4d %-30.30s %-30.30s\n"),id,description1.GetString(),description2.GetString());
  }
}

void UpdateDBOMaster(SQLDatabase& p_dbs)
{
  SQLQuery query(p_dbs);
  XString sql = _T("UPDATE dbo.master\n"
                   "   SET oms_char  = ?\n"
                   "      ,oms_wchar = ?\n"
                   " WHERE id        = ?");
  XString description1(_T("SECOND ROW"));
  XString description2(_T("The name is: Génèpëllêtje"));
  int     id = 2;

  query.SetParameter(1,description1);      // Normal CHAR
  query.SetParameter(2,description2,true); // Wide  WCHAR
  query.SetParameter(3,id);

  SQLTransaction trans(&p_dbs,_T("update"));
  query.DoSQLStatement(sql);
  trans.Commit();

  SelectionsDBOMaster(p_dbs);

  description1 = _T("second row");
  description2 = _T("Amount is € 24.00");

  query.ResetParameters();
  query.SetParameter(1,description1);       // Normal CHAR
  query.SetParameter(2,description2,true);  // Wide  WCHAR
  query.SetParameter(3,id);

  trans.Start(_T("update"));
  query.DoSQLStatement(sql);
  trans.Commit();

  SelectionsDBOMaster(p_dbs);
}

void TestNVarchar()
{
  _tprintf(_T("Testing selecting:\n"));
  _tprintf(_T("==================\n"));

  SQLDatabase dbs;
  dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*) _T(""));

  long beginTime = clock();

  try
  {
    // Settings of a database that holds a table with NVARCHAR column in UTF-16 in SQL-Server
    g_dsn    = _T("srnd01");
    g_user   = _T("sa");
    g_schema = _T("dbo");

    // Set options for the MS-Access database
    dbs.SetLoginTimeout(0);
    dbs.SetMARS(true);

    if(dbs.Open(g_dsn,g_user,g_password))
    {
      _tprintf(_T("Database opened.\n"));
      SelectionsDBOMaster(dbs);
      UpdateDBOMaster(dbs);
    }
    else
    {
      _tprintf(_T("Database ***NOT*** opened.\n"));
    }
  }
  catch(StdException& er)
  {
    _tprintf(_T("Database ***NOT*** opened. Reason:\n%s\n"),er.GetErrorMessage().GetString());
  }
  long endTime = clock();
  _tprintf(_T("Open  test performed in: %.6f seconds\n"),(double) (endTime - beginTime) / CLOCKS_PER_SEC);

  //////////////////////////////////////////////////////////////////////////
  //
  // CLOSING NOW
  //
  //////////////////////////////////////////////////////////////////////////

  beginTime = clock();
  if(dbs.IsOpen())
  {
    dbs.Close();
  }
  endTime = clock();
  _tprintf(_T("Close test performed in: %.6f seconds\n"),(double) (endTime - beginTime) / CLOCKS_PER_SEC);
}

