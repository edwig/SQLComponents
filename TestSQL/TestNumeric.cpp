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
  printf("Testing Numeric reading/writing:\n");
  printf("================================\n");

  SQLDatabase dbs;
  dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");
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
      printf("Database opened.\n");

      XString sql = "SELECT field2,field3 FROM test_number";
      SQLQuery query(&dbs);

      query.DoSQLStatement(sql);
      while(query.GetRecord())
      {
        double field1 = query[1];
        bcd    field2 = query[2];

        printf("Field 1 [%.4f] Field 2 [%s]\n",field1,field2.AsString().GetString());

        SQL_NUMERIC_STRUCT num;
        int size = sizeof(SQL_NUMERIC_STRUCT);
        memset(&num,0,size);
        memcpy_s(&num,size,query.GetColumn(1)->GetAsNumeric(),size);

        icd testing(&num);
        printf("ICD %s\n",testing.AsString().GetString());

        SQL_NUMERIC_STRUCT test;
        testing.AsNumeric(&test);

        if(memcmp(&num,&test,size) != 0)
        {
          printf("Conversion back is invalid!\n");
        }
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

//   printf("Precision: %d\n",res.precision);
//   printf("Scale    : %d\n",res.scale);
//   printf("Sign     : %d\n",res.sign);

  for(char ind = 0;ind <= res.scale; ++ind)
  {
    printf("Numeric mantissa [%d:%02.2X]\n",ind,res.val[ind]);
  }

  bcd check(&res);
  printf("bcd -> SQL_NUMERIC_STRUCT: %s\n",check.AsString().GetString());
}

// Test BCD to NUMERIC conversions
void TestIcd()
{
  // Header
  printf("Testing icd to SQL_NUMERIC_STRUCT\n");
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

  icd ten(&num);

  printf("SQL_NUMERIC_STRUCT -> icd: %s\n",ten.AsString().GetString());


  // Now back again to a SQL_NUMERIC_STRUCT
  ten.AsNumeric(&res);

  printf("Precision: %d\n",res.precision);
  printf("Scale    : %d\n",res.scale);
  printf("Sign     : %d\n",res.sign);

  for(char ind = 0;ind <= res.scale; ++ind)
  {
    printf("Numeric mantissa [%d:%02.2X]\n",ind,res.val[ind]);
  }

  icd check(&res);
  printf("icd -> SQL_NUMERIC_STRUCT: %s\n",check.AsString().GetString());
}


void TestBulkICD()
{
  // Header
  printf("Testing numeric(14,2) in bulk\n");
  printf("-----------------------------\n");

  XString number;
  SQL_NUMERIC_STRUCT numst1;
  SQL_NUMERIC_STRUCT numst2;
  memset(&numst1,0,sizeof(SQL_NUMERIC_STRUCT));
  memset(&numst2,0,sizeof(SQL_NUMERIC_STRUCT));
  int errors = 0;

  for(int64 x = 0; x < ((int64)LONG_MAX * (int64)100); ++x)
  {
    number.Format("%lld",x);
    icd num1(number);
    bcd num2(number);

    num1.SetLengthAndPrecision(14,2);
    num2.SetLengthAndPrecision(14,2);

    num1.AsNumeric(&numst1);
    num2.AsNumeric(&numst2);

    if(memcmp(&numst1,&numst2,sizeof(SQL_NUMERIC_STRUCT)) != 0)
    {
      printf("Numbers do no compare: %lld\n",x);
      ++errors;
    }
  }
  printf("Total number of errors: %d\n",errors);
}

void TestBulkDecimals(bool p_trunc)
{
  // Header
  printf("Testing decimals 0.9999 until 0.0001\n");
  printf("------------------------------------\n");

  XString number;
  SQL_NUMERIC_STRUCT numst1;
  SQL_NUMERIC_STRUCT numst2;
  memset(&numst1,0,sizeof(SQL_NUMERIC_STRUCT));
  memset(&numst2,0,sizeof(SQL_NUMERIC_STRUCT));
  int errors = 0;

  for(long x = 99990000; x >= 9999; x -= 10000)
  {
    icd num1(0,x);
    bcd num2(0,x);

    if(p_trunc)
    {
      num1.SetLengthAndPrecision(8,2);
      num2.SetLengthAndPrecision(8,2);
    }

//     printf("ICD %d : %s\n",x,num1.AsDisplayString().GetString());
//     printf("BCD %d : %s\n",x,num2.AsString(bcd::Format::Bookkeeping,false,4).GetString());
//     printf("\n");

    num1.AsNumeric(&numst1);
    num2.AsNumeric(&numst2);

    if(memcmp(&numst1,&numst2,sizeof(SQL_NUMERIC_STRUCT)) != 0)
    {
      printf("Numbers do no compare: %ld\n",x);
      ++errors;
    }

    icd num11(&numst1);
    bcd num21(&numst2);
    num11  = num11.Mul(icd(10000L,0));
    num21 *= 10000;

    int x11 = num11.AsLong() * 10000;
    int x21 = num21.AsLong() * 10000;

    if(p_trunc)
    {
      if(x11 != x21)
      {
        printf("Numbers do no compare: %ld\n",x);
        ++errors;
      }
    }
    else
    {
      if(x11 != x || x21 != x)
      {
        printf("Numbers do no compare: %ld\n",x);
        ++errors;
      }
    }
  }
  printf("Total number of errors: %d\n",errors);
}

void TestBCDIndividual()
{
  long x = 8000000;
  bcd num2(0,x);

  printf("BCD %d : %s\n",x,num2.AsString(bcd::Format::Bookkeeping,false,4).GetString());
  num2.SetLengthAndPrecision(8,2);
  printf("BCD %d : %s\n",x,num2.AsString(bcd::Format::Bookkeeping,false,4).GetString());

}