////////////////////////////////////////////////////////////////////////
//
// File: TestCalling.cpp
//
// Copyright (c) 1998-2016 ir. W.E. Huisman
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
// Last Revision:   14-12-2016
// Version number:  1.3.0
//
#include "stdafx.h"
#include "TestSQL.h"
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include "bcd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning (disable: 4477)

void TestCalling()
{
  printf("Test calling function/procedure:\n");
  printf("================================\n");

  // Test environment
  g_dsn      = "twoc09";
  g_user     = "k2b";
  g_password = "k2b";

  SQLDatabase dbs;
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
      printf("Database opened.\n");

      beginTime = clock();

      // Call with 1 input and 1 output parameter
      SQLQuery query(&dbs);
      query.SetParameter(1,12);
      var result = query.DoSQLCall("testmul",true);
      printf("Result of the function call: %d\n",result.GetAsSLong());

      long endTime = clock();
      printf("Calling test 1 performed in: %.6f seconds\n",(double)(endTime - beginTime) / CLOCKS_PER_SEC);
      
      // Call with 1 input parameter and NO return value
      beginTime = clock();
      SQLQuery q2(&dbs);
      q2.DoSQLCall("testins","Testen vanuit de programmatuur");

      endTime = clock();
      printf("Calling test 2 performed in: %.6f seconds\n",(double)(endTime - beginTime) / CLOCKS_PER_SEC);

      // Call with 1 input parameter and return value AND return parameter
      beginTime = clock();
      var txt(SQL_C_CHAR,5000);
      CString line;
      for(unsigned ind = 0; ind < 200; ++ind) line += "-";

      q2.ResetParameters();
      q2.SetParameter(0,bcd(0.01),SQL_PARAM_OUTPUT);
      q2.SetParameter(1,"Multi duplicate testing.");
      q2.SetParameter(2,line,SQL_PARAM_INPUT_OUTPUT);
      var* res = q2.DoSQLCall("multinout",true);

      endTime = clock();
      printf("Calling test 3 performed in: %.6f seconds\n",(double)(endTime - beginTime) / CLOCKS_PER_SEC);

      bcd number = res->GetAsBCD();
      CString text = q2.GetParameter(2)->GetAsChar();
      printf("Result of MULTINOUT: [%s] [%s]\n",number.AsString(),text.GetString());
    }
    else
    {
      printf("Database ***NOT*** opened.\n");
    }
  }
  catch(CString& s)
  {
    printf("Database ***NOT*** opened. Reason:\n%s\n",s);
  }
  catch(...)
  {
    printf("Database ***NOT*** opened for unknown reasons\n");
  }
  if(dbs.IsOpen())
  {
    dbs.Close();
  }
}

