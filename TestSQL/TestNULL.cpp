////////////////////////////////////////////////////////////////////////
//
// File: TestNULL.cpp
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
#include "SQLTransaction.h"
#include "SQLQuery.h"

void InsertWithNULL(SQLDatabase* p_dbs)
{
  CString sql = "INSERT INTO k2b_def_sleutel\n"
                "(organisatie_id,pakket_id,sl_id,sl_naam,sl_oms)\n"
                "VALUES (? ,? ,? ,? ,? )";

  try
  {
    SQLTransaction trans(p_dbs,"InsertNULL");
    SQLQuery query(p_dbs);

    SQLVariant var("");

    query.SetParameter(1,(long) 0);
    query.SetParameter(2,(long) 2);
    query.SetParameter(3,(long) 3000);
    query.SetParameter(4,"Dit is een naam");
    query.SetParameter(5,&var);

    query.DoSQLStatement(sql);
    trans.Commit();
  }
  catch(CString& error)
  {
    printf("Insert ging fout: %s\n",error.GetString());
  }
}

int
SelectNULL(SQLDatabase* p_dbs)
{
  CString sql = "SELECT organisatie_id\n"
                "      ,sl_id\n"
                "      ,sl_oms\n"
                "  FROM k2b_def_sleutel\n"
                " WHERE sl_id = ?";

  try
  {
    SQLQuery query(p_dbs);
    query.SetParameter(1,(long) 3000);
    query.DoSQLStatement(sql);
    if(query.GetRecord())
    {
      int orgID = query.GetColumn(1)->GetAsSLong();
      int slID  = query.GetColumn(2)->GetAsSLong();
      CString oms = query.GetColumn(3)->GetAsChar();
      SQLVariant* var = query.GetColumn(3);

      printf("Omschrijving: %s\n",oms.GetString());
      printf("Lengte string: %d\n",oms.GetLength());
    }
  }
  catch(CString& er)
  {
    printf("Fout: %s\n",er.GetString());
  }
  return 0;
}


void
TestNULL()
{
  printf("Testing the NULL functionality:\n");
  printf("===============================\n");

  SQLDatabase dbs;
  dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");
  long beginTime = clock();

  try
  {
    // Set options for the database
    dbs.SetLoginTimeout(0);
    dbs.SetMARS(false);

    if(dbs.Open(g_dsn,g_user,g_password))
    {
      printf("Database opened.\n");

//       InsertWithNULL(&dbs);
//       SelectNULL(&dbs);
    }
    else
    {
      printf("Database ***NOT*** opened.\n");
    }
    dbs.Close();
  }
  catch(CString& s)
  {
    printf("Database error. Reason:\n%s\n",s.GetString());
  }
  catch(...)
  {
    printf("Unknown error in database test\n");
  }
  long endTime = clock();
  printf("NULL test performed in: %.4f seconds\n",(double)(endTime - beginTime) / CLOCKS_PER_SEC);
}