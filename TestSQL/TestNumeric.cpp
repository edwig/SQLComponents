////////////////////////////////////////////////////////////////////////
//
// File: TestNumeric.cpp
//
// Copyright (c) 1998-2019 ir. W.E. Huisman
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

void TestNumeric()
{
  printf("Testing Numeric reading/writing:\n");
  printf("================================\n");

  SQLDatabase dbs;
  dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");
  long beginTime = clock();

  try
  {
//     g_dsn = "ok2b01";  // twoc09 = oracle / ok2b01 = MS-SQLServer / testing = MS-Access
//     g_user = "k2b";
//     g_password = "k2b";

    // Set options for the database
    dbs.SetLoginTimeout(0);
    dbs.SetMARS(false);

    if(dbs.Open(g_dsn,g_user,g_password))
    {
      printf("Database opened.\n");

      CString sql = "SELECT field2,field3 FROM test_number";
      SQLQuery query(&dbs);

      query.DoSQLStatement(sql);
      while(query.GetRecord())
      {
        double field1 = query[1];
        bcd    field2 = query[2];

        printf("Field 1 [%.4f] Field 2 [%s]\n",field1,field2.AsString().GetString());
      }


      // Do an update
      SQLTransaction trans(&dbs,"UpdNum");
      sql = "UPDATE test_number\n"
            "   SET field3 = ?\n"
            " WHERE id     = 1";
      bcd num("33445.2");

      // Parameter must match prec/scale of the database field
      query.SetParameter(1,num);

      query.DoSQLStatementNonQuery(sql);
      trans.Commit();
    }
    else
    {
      printf("Database ***NOT*** opened.\n");
    }
    dbs.Close();
  }
  catch (StdException& er)
  {
    printf("Database error. Reason: %s\n",er.GetErrorMessage().GetString());
  }
  long endTime = clock();
  printf("NUMERIC test performed in: %.4f seconds\n",(double)(endTime - beginTime) / CLOCKS_PER_SEC);
}

// Test BCD to NUMERIC conversions
void TestBcd()
{
  // Header
  printf("Testing bcd to SQL_NUMERIC_STRUCT\n");
  printf("---------------------------------\n");

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

  printf("SQL_NUMERIC_STRUCT -> bcd: %s\n",ten.AsString().GetString());


  // Now back again to a SQL_NUMERIC_STRUCT
  ten.AsNumeric(&res);

  printf("Precision: %d\n",res.precision);
  printf("Scale    : %d\n",res.scale);
  printf("Sign     : %d\n",res.sign);

  for(char ind = 0;ind <= res.scale; ++ind)
  {
    printf("Numeric mantissa [%d:%02.2X]\n",ind,res.val[ind]);
  }

  bcd check(&res);
  printf("bcd -> SQL_NUMERIC_STRUCT: %s\n",check.AsString().GetString());
}
