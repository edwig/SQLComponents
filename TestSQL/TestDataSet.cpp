////////////////////////////////////////////////////////////////////////
//
// File: TestDataSet.cpp
//
// Copyright (c) 1998-2024 ir. W.E. Huisman
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

double
TestAppend(SQLDatabase* p_dbs,int p_master)
{
  _tprintf(_T("Testing the DataSet append function:\n"));
  _tprintf(_T("====================================\n"));

  SQLDataSet details(_T("detail"),p_dbs);
  details.SetPrimaryTable(_T(""),_T("detail"));
  details.SetPrimaryKeyColumn(_T("id"));
  details.SetParameter(_T("mast_id"),SQLVariant(p_master));
  details.SetSelection(_T("*")); // Select all columns

  if(details.Open())
  {
    _tprintf(_T("Dataset details (mast_id = %d) opened. Rows: %d\n"),p_master,details.GetNumberOfRecords());
  }
  else
  {
    _tprintf(_T("Dataset details (mast_id = %d) ***NOT*** openend\n"),p_master);
  }

  // Prev master record
  _tprintf(_T("Dataset append. Number of records: %d\n"),details.GetNumberOfRecords());
  --p_master;
  details.SetParameter(_T("mast_id"),SQLVariant(p_master));
  details.Append();
  _tprintf(_T("Dataset append. Number of records: %d\n"),details.GetNumberOfRecords());

  // Calculate the aggregation of the amount field
  int column = details.GetFieldNumber(_T("amount"));
  AggregateInfo info;
  details.Aggregate(column,info);

  _tprintf(_T("Aggregation of the field 'amount':\n")
         _T("- sum  = %14.2f\n")
         _T("- max  = %14.2f\n")
         _T("- min  = %14.2f\n")
         _T("- mean = %14.2f\n")
         ,info.m_sum,info.m_max,info.m_min,info.m_mean);
         
  _tprintf(_T("Testing the object cache\n"));
  SQLRecord* record = details.FindObjectRecord(8);       
  SQLVariant* var = record->GetField(details.GetFieldNumber(_T("id")));
  _tprintf(_T("Found record with id: [%d]\n"),var->GetAsSLong());        
         
  return info.m_sum;
}


double
ReadDetailSet(SQLDatabase* p_dbs,int p_master)
{
  SQLDataSet details(_T("detail"),p_dbs);
  details.SetPrimaryTable(_T(""),_T("detail"));
  details.SetPrimaryKeyColumn(_T("id"));
  details.SetParameter(_T("mast_id"),SQLVariant(p_master));
  details.SetSelection(_T("*")); // Select all columns

  // Alternatively to "SetSelection", we may specify a complete query
//   XString sql("SELECT id\n"
//               "      ,mast_id\n"
//               "      ,line\n"
//               "      ,description\n"
//               "      ,amount\n"
//               "  FROM $SCHEMA$.detail\n"
//               " WHERE mast_id = $mast_id");
//   details.SetQuery(sql);

  if(details.Open())
  {
    _tprintf(_T("Dataset details (mast_id = %d) opened. Rows: %d\n"),p_master,details.GetNumberOfRecords());
  }
  else
  {
    _tprintf(_T("Dataset details (mast_id = %d) ***NOT*** openend\n"),p_master);
  }

  // Calculate the aggregation of the amount field
  int column = details.GetFieldNumber(_T("amount"));
  AggregateInfo info;
  details.Aggregate(column,info);

  _tprintf(_T("Aggregation of the field 'amount':\n")
         _T("- sum  = %14.2f\n")
         _T("- max  = %14.2f\n")
         _T("- min  = %14.2f\n")
         _T("- mean = %14.2f\n")
        ,info.m_sum,info.m_max,info.m_min,info.m_mean);
  return info.m_sum;
}

double
ReadMasterSet(SQLDatabase* p_dbs,int p_master,double p_amount)
{
  SQLDataSet master(_T("master"),p_dbs);
  master.SetPrimaryTable(_T(""),_T("master"));
  master.SetPrimaryKeyColumn(_T("id"));
  master.SetParameter(_T("key"),SQLVariant(p_master));
  XString sql (_T("SELECT id\n")
               _T("      ,invoice\n")
               _T("      ,description\n")
               _T("      ,total\n")
               _T("  FROM master\n")
               _T(" WHERE id = $key"));
  master.SetQuery(sql);
  // Read in the dataset
  if(master.Open())
  {
    _tprintf(_T("Dataset master (id = %d) opened\n"),p_master);
  }
  else
  {
    _tprintf(_T("Dataset master (id = %d) ***NOT*** opened!\n"),p_master);
  }

  int fieldnum = master.GetFieldNumber(_T("total"));
  double total = master.GetCurrentField(fieldnum)->GetAsDouble();
  _tprintf(_T("Total amount of this record is: %.2f\n"),total);

  // Change field
  SQLRecord* record = master.GetRecord(0);
  SQLVariant var(p_amount);
  record->ModifyField(fieldnum,&var,1);

  // TEST
  // master.CancelMutation(1);

  // Synchronize with the database
  master.Synchronize();

  return total;
}

void
TestDataSet()
{
  _tprintf(_T("Testing the DataSet function:\n"));
  _tprintf(_T("=============================\n"));

  SQLDatabase dbs;
  dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)_T(""));
  long beginTime = clock();

  try
  {
    // Set options for the database
    dbs.SetLoginTimeout(0);
    dbs.SetMARS(false);

    if(dbs.Open(g_dsn,g_user,g_password))
    {
      _tprintf(_T("Database opened.\n"));

      long   master = 2;
      double amount = 0.0;
      double total  = 0.0;
      amount = ReadDetailSet(&dbs,master);
      total  = ReadMasterSet(&dbs,master,amount);

      if(amount != total)
      {
        _tprintf(_T("Amounts differ. Check that database update has been done!\n"));
      }

      TestAppend(&dbs,master);
    }
    else
    {
      _tprintf(_T("Database ***NOT*** opened.\n"));
    }
    dbs.Close();
  }
  catch(StdException& er)
  {
    _tprintf(_T("Database error. Reason:\n%s\n"),er.GetErrorMessage().GetString());
  }
  long endTime = clock();
  _tprintf(_T("DATASET test performed in: %.4f seconds\n"), (double)(endTime - beginTime) / CLOCKS_PER_SEC);
}