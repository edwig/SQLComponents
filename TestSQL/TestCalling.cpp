////////////////////////////////////////////////////////////////////////
//
// File: TestCalling.cpp
//
// Copyright (c) 1998-2018 ir. W.E. Huisman
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
// Last Revision:   28-05-2018
// Version number:  1.5.0
//
#include "stdafx.h"
#include "TestSQL.h"
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include "bcd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void TestCalling()
{
  printf("Test calling function/procedure:\n");
  printf("================================\n");

  SQLDatabase dbs;
  dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");

  long beginTime = clock();

  try
  {
    // Set options for the database
    dbs.SetLoginTimeout(0);
    dbs.SetMARS(false);
    // dbs.AddConnectOption("FBS","600000");

    if(dbs.Open(g_dsn,g_user,g_password))
    {
      printf("Database opened.\n");

      beginTime = clock();

      // Call with 1 input and 1 output parameter
      SQLQuery query(dbs);
      SQLTransaction trans(&dbs,"testing");
      var* result = query.DoSQLCall(g_schema,"testmul2",12);
      printf("Result of the function call: %d\n",result->GetAsSLong());

      long endTime = clock();
      printf("Calling test 1 performed in: %.6f seconds\n",(double)(endTime - beginTime) / CLOCKS_PER_SEC);
      
      // Call to get a decimal
      query.ResetParameters();
      query.SetParameter(0,bcd(10000.01),P_SQL_PARAM_OUTPUT);
      query.SetParameter(1,"234.99");
      result = query.DoSQLCall(g_schema,"getdecimal",true);
      printf("Conversion of number 234.99 is: %s\n",result->GetAsBCD().AsString().GetString());

      // Call with 1 input parameter and NO return value
      beginTime = clock();
      SQLQuery q2(&dbs);
      q2.DoSQLCall(g_schema,"testins","TESTING");

      endTime = clock();
      printf("Calling test 2 performed in: %.6f seconds\n",(double)(endTime - beginTime) / CLOCKS_PER_SEC);

      // Call with 1 input parameter and return value AND return parameter
      beginTime = clock();
      var txt(SQL_C_CHAR,5000);
      CString line;
      for(unsigned ind = 0; ind < 202; ++ind) line += "-";

      q2.ResetParameters();
      q2.SetParameter(0,bcd("11.11"),P_SQL_PARAM_OUTPUT);
      q2.SetParameter(1,"Multi duplicate testing.");
      q2.SetParameter(2,line,P_SQL_PARAM_OUTPUT);
      var* res = q2.DoSQLCall(g_schema,"multinout",true);

      endTime = clock();
      printf("Calling test 3 performed in: %.6f seconds\n",(double)(endTime - beginTime) / CLOCKS_PER_SEC);

      int status = 0;
      bcd number = q2.GetParameter(0)->GetAsBCD();
      CString text = q2.GetParameter(2)->GetAsChar();
      printf("Result of MULTINOUT = status:%d [%s] [%s]\n",status,number.AsString().GetString(),text.GetString());

      trans.Commit();
    }
    else
    {
      printf("Database ***NOT*** opened.\n");
    }
  }
  catch(StdException* er)
  {
    printf("Database ***NOT*** opened. Reason:\n%s\n",er->GetErrorMessage().GetString());
    er->Delete();
  }
  if(dbs.IsOpen())
  {
    dbs.Close();
  }
}

