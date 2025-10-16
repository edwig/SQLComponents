////////////////////////////////////////////////////////////////////////
//
// File: TestClosingCursor.cpp
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

void TestClosingCursor()
{
  _tprintf(_T("Test closing the cursor:\n"));
  _tprintf(_T("========================\n"));

  SQLDatabase dbs;
  dbs.AddMacro(_T("$SCHEMA$"),g_schema);
  // Do not log: we want clean output of open/close
  // dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");

  long beginTime = clock();

  try
  {
    // Set options for the database
    dbs.SetLoginTimeout(0);
    dbs.SetMARS(false);
    // dbs.AddConnectOption("FBS","600000");

    if(dbs.Open(g_dsn,g_user,g_password))
    {
      _tprintf(_T("Database opened.\n"));
      for(unsigned ind = 1;ind <= 1500; ++ind)
      {
        SQLQuery query(&dbs);
        XString sql(_T("SELECT COUNT(*) FROM $SCHEMA$.DUAL"));
        query.DoSQLStatement(sql);
        _tprintf(_T("."));
        if(ind % 50 == 0)
        {
          _tprintf(_T(" - %d\n"),ind);
        }
      }
    }
    else
    {
      _tprintf(_T("Database ***NOT*** opened.\n"));
    }
  }
  catch(StdException& er)
  {
    _tprintf(_T("Database ***NOT*** opened. Reason:\n%s\n"), er.GetErrorMessage().GetString());
  }
  long endTime = clock();
  _tprintf(_T("Open  test performed in: %.6f seconds\n"), (double)(endTime - beginTime) / CLOCKS_PER_SEC);

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
  _tprintf(_T("Close test performed in: %.6f seconds\n"), (double)(endTime - beginTime) / CLOCKS_PER_SEC);
}

