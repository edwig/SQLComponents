////////////////////////////////////////////////////////////////////////
//
// File: TestSQL.cpp
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
// Last Revision:   20-01-2019
// Version number:  1.5.4
//
#include "stdafx.h"
#include "TestSQL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Firebird Open Source RDBMS 3.0
// DSN "testing" points to the "testing.fdb" database
// in the project root folder
//
CString g_dsn("testing");
CString g_user("sysdba");
CString g_password("altijd");
CString g_schema("");

// TESTING ON INFORMIX
// CString g_dsn("magdev04");
// CString g_user("informix");
// CString g_password("inform01");
// CString g_schema("informix");

void CALLBACK LogPrint(void* p_context,const char* p_text)
{
  const char *context = (const char*) p_context;
  printf(context);
  printf(p_text);
}

int CALLBACK LogLevel(void* p_context)
{
  return 6; // = LOGLEVEL_MAX from SQLDatabase.h
}

// The one and only application object

using namespace std;

int main(int argc,char* argv[],char* envp[])
{
  int nRetCode = 0;

  InitSQLComponents(LN_ENGLISH);

  // Basic connection test
  TestConnection();
  // Test calling procedure/function
  TestCalling();
  // Test BCD to NUMERIC conversions
  TestBcd();
  // Testing Numerics
  TestNumeric();
  // Test closing cursors does not leave thrash in RDBMS    
  TestClosingCursor();
  // Test the reading of NULL
  TestNULL();
  // Test formatting of Date/Time/Timestamp
  TestFormatting();
  // Test operators
  TestOperators();
  // Test dataset
  TestDataSet();


  //////////////////////////////////////////////////////////////////////////
  // 
  // READY
  //
  //////////////////////////////////////////////////////////////////////////

  printf("Give your opinion: ");
  fflush(stdout);
  char buffer[101] = "";
  gets_s(buffer,100);

  return nRetCode;
}
