////////////////////////////////////////////////////////////////////////
//
// File: TestNULL.cpp
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
#include "SQLTransaction.h"
#include "SQLQuery.h"

void InsertWithNULL(SQLDatabase* p_dbs)
{
  XString sql = _T("INSERT INTO k2b_def_sleutel\n")
                _T("(organisatie_id,pakket_id,sl_id,sl_naam,sl_oms)\n")
                _T("VALUES (? ,? ,? ,? ,? )");

  try
  {
    SQLTransaction trans(p_dbs,_T("InsertNULL"));
    SQLQuery query(p_dbs);

    SQLVariant var(_T(""));

    query.SetParameter(1,(long) 0);
    query.SetParameter(2,(long) 2);
    query.SetParameter(3,(long) 3000);
    query.SetParameter(4,_T("Dit is een naam"));
    query.SetParameter(5,&var);

    query.DoSQLStatement(sql);
    trans.Commit();
  }
  catch(StdException& er)
  {
    _tprintf(_T("Insert went wrong: %s\n"),er.GetErrorMessage().GetString());
  }
}

int
SelectNULL(SQLDatabase* p_dbs)
{
  XString sql = _T("SELECT organisatie_id\n")
                _T("      ,sl_id\n")
                _T("      ,sl_oms\n")
                _T("  FROM k2b_def_sleutel\n")
                _T(" WHERE sl_id = ?");

  try
  {
    SQLQuery query(p_dbs);
    query.SetParameter(1,(long) 3000);
    query.DoSQLStatement(sql);
    if(query.GetRecord())
    {
      int orgID = query.GetColumn(1)->GetAsSLong();
      int slID  = query.GetColumn(2)->GetAsSLong();
      XString oms = query.GetColumn(3)->GetAsChar();
      SQLVariant* var = query.GetColumn(3);

      _tprintf(_T("Description: %s\n"),oms.GetString());
      _tprintf(_T("Length string: %d\n"),oms.GetLength());
    }
  }
  catch(StdException& er)
  {
    _tprintf(_T("Error: %s\n"),er.GetErrorMessage().GetString());
  }
  return 0;
}


void
TestNULL()
{
  _tprintf(_T("Testing the NULL functionality:\n"));
  _tprintf(_T("===============================\n"));

  SQLDatabase dbs;
  dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)_T(""));
  long beginTime = clock();

  try
  {
    // Set options for the database
    dbs.SetLoginTimeout(0);
    dbs.SetMARS(false);

    if(dbs.Open(g_dsn,g_user,g_password))
    {
      _tprintf(_T("Database opened.\n"));

//       InsertWithNULL(&dbs);
//       SelectNULL(&dbs);
    }
    else
    {
      _tprintf(_T("Database ***NOT*** opened.\n"));
    }
    dbs.Close();
  }
  catch(StdException& er)
  {
    _tprintf(_T("Database error. Reason:\n%s\n"),er.GetErrorMessage().GetString());
  }
  long endTime = clock();
  _tprintf(_T("NULL test performed in: %.4f seconds\n"),(double)(endTime - beginTime) / CLOCKS_PER_SEC);
}