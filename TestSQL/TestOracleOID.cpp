////////////////////////////////////////////////////////////////////////
//
// File: TestOracleOID.cpp
//
// Copyright (c) 1998-2021 ir. W.E. Huisman
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
#include "SQLDataSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void PerformUpdateTest1(SQLDatabase* p_dbs)
{
  SQLDataSet set;

  set.SetDatabase(p_dbs);
  set.SetPrimaryTable("kdba","kiw_gem");
  set.SetPrimaryKeyColumn("oid");
  set.SetSelection("*");
//  set.SetWhereCondition("oid = 1323");
  set.SetWhereCondition("gemhoortwrg = 23");

  if(set.Open())
  {
    if(set.GetNumberOfRecords() == 1)
    {
      set.GetRecord(0)->ModifyField("gemeentenaam", "Capelle aan den IJssel");
      set.Synchronize();
    }
    else
    {
      printf("Test record not found!\n");
    }
    set.Close();
  }
}

void PerformUpdateTest2(SQLDatabase* p_dbs)
{
  SQLDataSet set;

  set.SetDatabase(p_dbs);
  set.SetPrimaryTable("kdba", "kiw_gem");
  set.SetPrimaryKeyColumn("oid");
  set.SetSelection("*");
  set.SetWhereCondition("oid = 1323");

  if (set.Open())
  {
    if (set.GetNumberOfRecords() == 1)
    {
      set.GetRecord(0)->ModifyField("gemeentenaam","Meppel");
      set.Synchronize();
    }
    else
    {
      printf("Test record not found!\n");
    }
    set.Close();
  }
}

void TestOracleOID()
{
  printf("Testing the connection:\n");
  printf("=======================\n");

  SQLDatabase dbs;
  dbs.RegisterLogContext(LOGLEVEL_MAX, LogLevel, LogPrint, (void*)"");

  long beginTime = clock();

  try
  {
    dbs.SetLoginTimeout(10);

    // ORACLE OPTIONS

    // Fill in your password!
    if(dbs.Open("owoc1201","kdba","******"))
    {
      printf("Database opened.\n");
      PerformUpdateTest1(&dbs);
      PerformUpdateTest2(&dbs);
    }
    else
    {
      printf("Database ***NOT*** opened.\n");
    }
  }
  catch (StdException& er)
  {
    printf("Database ***NOT*** opened. Reason:\n%s\n", er.GetErrorMessage().GetString());
  }
  long endTime = clock();
  printf("Open  test performed in: %.6f seconds\n", (double)(endTime - beginTime) / CLOCKS_PER_SEC);
}
