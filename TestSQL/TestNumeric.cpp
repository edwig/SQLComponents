////////////////////////////////////////////////////////////////////////
//
// File: TestNumeric.cpp
//
// Copyright (c) 1998-2022 ir. W.E. Huisman
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
#include "SQLQuery.h"
#include "SQLDatabase.h"
#include "SQLVariant.h"
#include "bcd.h"
#include "icd.h"

void TestNumeric()
{
  _tprintf(_T("Testing Numeric reading/writing:\n"));
  _tprintf(_T("================================\n"));

  SQLDatabase dbs;
  dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)_T(""));
  long beginTime = clock();

  try
  {
//     g_dsn      = "ok2b01";  // twoc09 = oracle / ok2b01 = MS-SQLServer / testing = MS-Access
//     g_user     = "k2b";
//     g_password = "k2b";

    // Set options for the database
    dbs.SetLoginTimeout(0);
    dbs.SetMARS(false);

    if(dbs.Open(g_dsn,g_user,g_password))
    {
      _tprintf(_T("Database opened.\n"));

      XString sql = _T("SELECT field2,field3 FROM test_number");
      SQLQuery query(&dbs);

      query.DoSQLStatement(sql);
      while(query.GetRecord())
      {
        double field1 = query[1];
        bcd    field2 = query[2];

        _tprintf(_T("Field 1 [%.4f] Field 2 [%s]\n"),field1,field2.AsString().GetString());

        SQL_NUMERIC_STRUCT num;
        int size = sizeof(SQL_NUMERIC_STRUCT);
        memset(&num,0,size);
        memcpy_s(&num,size,query.GetColumn(1)->GetAsNumeric(),size);

        icd testing(&num);
        _tprintf(_T("ICD %s\n"),testing.AsString().GetString());

        SQL_NUMERIC_STRUCT test;
        testing.AsNumeric(&test);

        if(memcmp(&num,&test,size) != 0)
        {
          _tprintf(_T("Conversion back is invalid!\n"));
        }
      }


      // Do an update
      SQLTransaction trans(&dbs,_T("UpdNum"));
      sql = _T("UPDATE test_number\n")
            _T("   SET field3 = ?\n")
            _T(" WHERE id     = 1");
      bcd num(_T("33445.2"));

      // Parameter must match prec/scale of the database field
      query.SetParameter(1,num);

      query.DoSQLStatementNonQuery(sql);
      trans.Commit();
    }
    else
    {
      _tprintf(_T("Database ***NOT*** opened.\n"));
    }
    dbs.Close();
  }
  catch (StdException& er)
  {
    _tprintf(_T("Database error. Reason: %s\n"),er.GetErrorMessage().GetString());
  }
  long endTime = clock();
  _tprintf(_T("NUMERIC test performed in: %.4f seconds\n"),(double)(endTime - beginTime) / CLOCKS_PER_SEC);
}

// Test BCD to NUMERIC conversions
void TestBcd()
{
  // Header
  _tprintf(_T("Testing bcd to SQL_NUMERIC_STRUCT\n"));
  _tprintf(_T("---------------------------------\n"));

  // num = 10.001 (ten and 1 thousandth)
  SQL_NUMERIC_STRUCT num;
  SQL_NUMERIC_STRUCT res;
  memset(&num,0,sizeof(SQL_NUMERIC_STRUCT));
  memset(&res,0,sizeof(SQL_NUMERIC_STRUCT));
  num.sign = 1; // Positive
  num.precision = 6;
  num.scale = 4;

  num.val[0] = 0xAA;
  num.val[1] = 0x86;
  num.val[2] = 0x01;

  bcd ten(&num);

  _tprintf(_T("SQL_NUMERIC_STRUCT -> bcd: %s\n"),ten.AsString().GetString());


  // Now back again to a SQL_NUMERIC_STRUCT
  ten.AsNumeric(&res);

//   printf("Precision: %d\n",res.precision);
//   printf("Scale    : %d\n",res.scale);
//   printf("Sign     : %d\n",res.sign);

  for(TCHAR ind = 0;ind <= res.scale; ++ind)
  {
    _tprintf(_T("Numeric mantissa [%d:%02.2X]\n"),ind,res.val[ind]);
  }

  bcd check(&res);
  _tprintf(_T("bcd -> SQL_NUMERIC_STRUCT: %s\n"),check.AsString().GetString());
}

// Test BCD to NUMERIC conversions
void TestIcd()
{
  // Header
  _tprintf(_T("Testing icd to SQL_NUMERIC_STRUCT\n"));
  _tprintf(_T("---------------------------------\n"));

  // num = 10.001 (ten and 1 thousandth)
  SQL_NUMERIC_STRUCT num;
  SQL_NUMERIC_STRUCT res;
  memset(&num,0,sizeof(SQL_NUMERIC_STRUCT));
  memset(&res,0,sizeof(SQL_NUMERIC_STRUCT));
  num.sign = 1; // Positive
  num.precision = 6;
  num.scale = 4;

  num.val[0] = 0xAA;
  num.val[1] = 0x86;
  num.val[2] = 0x01;

  icd ten(&num);

  _tprintf(_T("SQL_NUMERIC_STRUCT -> icd: %s\n"),ten.AsString().GetString());


  // Now back again to a SQL_NUMERIC_STRUCT
  ten.AsNumeric(&res);

  _tprintf(_T("Precision: %d\n"),res.precision);
  _tprintf(_T("Scale    : %d\n"),res.scale);
  _tprintf(_T("Sign     : %d\n"),res.sign);

  for(TCHAR ind = 0;ind <= res.scale; ++ind)
  {
    _tprintf(_T("Numeric mantissa [%d:%02.2X]\n"),ind,res.val[ind]);
  }

  icd check(&res);
  _tprintf(_T("icd -> SQL_NUMERIC_STRUCT: %s\n"),check.AsString().GetString());
}

void TestBCDIndividual()
{
  long x = 8000000;
  bcd num2(0,x);

  _tprintf(_T("BCD %d : %s\n"),x,num2.AsString(bcd::Format::Bookkeeping,false,4).GetString());
  num2.SetLengthAndPrecision(8,2);
  _tprintf(_T("BCD %d : %s\n"),x,num2.AsString(bcd::Format::Bookkeeping,false,4).GetString());

}