////////////////////////////////////////////////////////////////////////
//
// File: TestSelections.cpp
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
#include "TestSQL.h"
#include "SQLDatabase.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bcd SelectionsMaster(SQLDatabase& p_dbs)
{
  _tprintf(_T("MASTER\n"));
  _tprintf(_T("ID   INVC Description                         Total\n"));
  _tprintf(_T("---- ---- ------------------------------ ----------\n"));

  bcd grandTotal;
  SQLQuery query(p_dbs);
  XString sql = _T("SELECT * FROM master");
  query.DoSQLStatement(sql);
  while(query.GetRecord())
  {
    // Getting the data from the record
    int     id          = query[1];
    int     invoice     = query[2];
    XString description = query[3];
    bcd     total       = query[4];

    // Printing
    _tprintf(_T("%4d %4d %-30.30s %10.10s\n"),id,invoice,description.GetString(),total.AsString(bcd::Format::Bookkeeping,false,2).GetString());

    grandTotal += total;
  }
  _tprintf(_T("---- ---- ------------------------------ ----------\n"));
  _tprintf(_T("Total amount: %s\n\n"),grandTotal.AsString(bcd::Format::Bookkeeping,false,2).GetString());
  return grandTotal;
}

bcd SelectionsDetail(SQLDatabase& p_dbs)
{
  _tprintf(_T("DETAIL\n"));
  _tprintf(_T("ID   Mast Line Description                         Total\n"));
  _tprintf(_T("---- ---- ---- ------------------------------ ----------\n"));

  bcd grandTotal;
  SQLQuery query(p_dbs);
  XString sql = _T("SELECT * FROM detail");
  query.DoSQLStatement(sql);
  while(query.GetRecord())
  {
    int id     = query[1];
    int mastId = query[2];
    int line   = query[3];
    XString description = query[4];
    bcd amount = query[5];

    // Printing
    _tprintf(_T("%4d %4d %4d %-30.30s %10.10s\n"),id,mastId,line,description.GetString(),amount.AsString(bcd::Format::Bookkeeping,false,2).GetString());

    grandTotal += amount;
  }
  _tprintf(_T("---- ---- ---- ------------------------------ ----------\n"));
  _tprintf(_T("Total amount: %s\n\n"),grandTotal.AsString(bcd::Format::Bookkeeping,false,2).GetString());
  return grandTotal;
}

void TestSelections()
{
  _tprintf(_T("Testing selecting:\n"));
  _tprintf(_T("==================\n"));

  SQLDatabase dbs;
  dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*) _T(""));

  long beginTime = clock();

  try
  {
    // Set options for the MS-Access database
    dbs.SetLoginTimeout(0);
    dbs.SetMARS(false);

    // ORACLE OPTIONS
    // dbs.AddConnectOption("FBS","600000");

    if(dbs.Open(g_dsn,g_user,g_password))
    {
      _tprintf(_T("Database opened.\n"));
      bcd mast = SelectionsMaster(dbs);
      bcd dets = SelectionsDetail(dbs);

      if(mast == dets)
      {
        _tprintf(_T("Master/detail sums up nicely!\n"));
      }
      else
      {
        _tprintf(_T("ALARM: Master and Detail total amounts differ!!\n"));
      }
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

