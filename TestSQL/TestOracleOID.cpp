////////////////////////////////////////////////////////////////////////
//
// File: TestOracleOID.cpp
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
#include "SQLDataSet.h"

void PerformUpdateTest1(SQLDatabase* p_dbs)
{
  SQLDataSet set;

  set.SetDatabase(p_dbs);
  set.SetPrimaryTable(_T("kdba"),_T("kiw_gem"));
  set.SetPrimaryKeyColumn(_T("oid"));
  set.SetSelection(_T("*"));
//  set.SetWhereCondition("oid = 1323");
  set.SetWhereCondition(_T("gemhoortwrg = 23"));

  if(set.Open())
  {
    if(set.GetNumberOfRecords() == 1)
    {
      set.GetRecord(0)->ModifyField(_T("gemeentenaam"), _T("Capelle aan den IJssel"));
      set.Synchronize();
    }
    else
    {
      _tprintf(_T("Test record not found!\n"));
    }
    set.Close();
  }
}

void PerformUpdateTest2(SQLDatabase* p_dbs)
{
  SQLDataSet set;

  set.SetDatabase(p_dbs);
  set.SetPrimaryTable(_T("kdba"), _T("kiw_gem"));
  set.SetPrimaryKeyColumn(_T("oid"));
  set.SetSelection(_T("*"));
  set.SetWhereCondition(_T("oid = 1323"));

  if (set.Open())
  {
    if (set.GetNumberOfRecords() == 1)
    {
      set.GetRecord(0)->ModifyField(_T("gemeentenaam"),_T("Meppel"));
      set.Synchronize();
    }
    else
    {
      _tprintf(_T("Test record not found!\n"));
    }
    set.Close();
  }
}

void TestOracleOID()
{
  _tprintf(_T("Testing the connection:\n"));
  _tprintf(_T("=======================\n"));

  SQLDatabase dbs;
  dbs.RegisterLogContext(LOGLEVEL_MAX, LogLevel, LogPrint, (void*)_T(""));

  long beginTime = clock();

  try
  {
    dbs.SetLoginTimeout(10);

    // ORACLE OPTIONS

    // Fill in your password!
    if(dbs.Open(_T("owoc1201"),_T("kdba"),_T("******")))
    {
      _tprintf(_T("Database opened.\n"));
      PerformUpdateTest1(&dbs);
      PerformUpdateTest2(&dbs);
    }
    else
    {
      _tprintf(_T("Database ***NOT*** opened.\n"));
    }
  }
  catch (StdException& er)
  {
    _tprintf(_T("Database ***NOT*** opened. Reason:\n%s\n"), er.GetErrorMessage().GetString());
  }
  long endTime = clock();
  _tprintf(_T("Open  test performed in: %.6f seconds\n"), (double)(endTime - beginTime) / CLOCKS_PER_SEC);
}
