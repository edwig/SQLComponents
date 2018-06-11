////////////////////////////////////////////////////////////////////////
//
// File: TestConnection.cpp
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void TestConnection()
{
  printf("Testing the connection:\n");
  printf("=======================\n");

  SQLDatabase dbs;
  dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");

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
      printf("Database opened.\n");
    }
    else
    {
      printf("Database ***NOT*** opened.\n");
    }
  }
  catch(StdException& er)
  {
    printf("Database ***NOT*** opened. Reason:\n%s\n",er.GetErrorMessage().GetString());
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

