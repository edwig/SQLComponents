////////////////////////////////////////////////////////////////////////
//
// File: TestClosingCursor.cpp
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
// Last Revision:   04-12-2016
// Version number:  1.3.0
//
#include "stdafx.h"
#include "TestSQL.h"
#include "SQLDatabase.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning (disable: 4477)

void TestClosingCursor()
{
  printf("Test closing the cursor:\n");
  printf("========================\n");

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
      for(unsigned ind = 1;ind <= 1500; ++ind)
      {
        SQLQuery query(&dbs);
        CString sql("SELECT COUNT(*) FROM DUAL");
        query.DoSQLStatement(sql);
        printf(".");
        if(ind % 50 == 0)
        {
          printf(" - %d\n",ind);
        }
      }
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
  long endTime = clock();
  printf("Open  test performed in: %.6f seconds\n", (double)(endTime - beginTime) / CLOCKS_PER_SEC);

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
  printf("Close test performed in: %.6f seconds\n", (double)(endTime - beginTime) / CLOCKS_PER_SEC);
}

