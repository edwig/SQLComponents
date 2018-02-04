////////////////////////////////////////////////////////////////////////
//
// File: TestDataSet.cpp
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
// Last Revision:   04-02-2018
// Version number:  1.4.5
//
#include "stdafx.h"
#include "TestSQL.h"
#include "SQLDatabase.h"
#include "SQLDataSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

double
TestAppend(SQLDatabase* p_dbs,long p_master)
{
  printf("Testing the DataSet append function:\n");
  printf("====================================\n");

  SQLDataSet details("detail",p_dbs);
  details.SetPrimaryTable(g_schema,"detail");
  details.SetPrimaryKeyColumn("id");
  details.SetSearchableColumn("id");
  details.SetParameter("mast_id",SQLVariant(p_master));
  details.SetSelection("*"); // Select all columns

  if(details.Open())
  {
    printf("Dataset details (mast_id = %d) opened. Rows: %d\n",p_master,details.GetNumberOfRecords());
  }
  else
  {
    printf("Dataset details (mast_id = %d) ***NOT*** openend\n",p_master);
  }

  // Prev master record
  printf("Dataset append. Number of records: %d\n",details.GetNumberOfRecords());
  --p_master;
  details.SetParameter("mast_id",SQLVariant(p_master));
  details.Append();
  printf("Dataset append. Number of records: %d\n",details.GetNumberOfRecords());

  // Calculate the aggregation of the amount field
  int column = details.GetFieldNumber("amount");
  AggregateInfo info;
  details.Aggregate(column,info);

  printf("Aggregation of the field 'amount':\n"
         "- sum  = %14.2f\n"
         "- max  = %14.2f\n"
         "- min  = %14.2f\n"
         "- mean = %14.2f\n"
         ,info.m_sum,info.m_max,info.m_min,info.m_mean);
         
  printf("Testing the object cache\n");
  SQLRecord* record = details.FindObjectRecord(8);       
  SQLVariant* var = record->GetField(details.GetFieldNumber("id"));
  printf("Found record with id: [%d]\n",var->GetAsSLong());        
         
  return info.m_sum;
}


double
ReadDetailSet(SQLDatabase* p_dbs,long p_master)
{
  SQLDataSet details("detail",p_dbs);
  details.SetPrimaryTable(g_schema,"detail");
  details.SetPrimaryKeyColumn("id");
  details.SetParameter("mast_id",SQLVariant(p_master));
  details.SetSelection("*"); // Select all columns

  // Alternatively to "SetSelection", we may specify a complete query
//   CString sql("SELECT id\n"
//               "      ,mast_id\n"
//               "      ,line\n"
//               "      ,description\n"
//               "      ,amount\n"
//               "  FROM $SCHEMA$.detail\n"
//               " WHERE mast_id = $mast_id");
//   details.SetQuery(sql);

  if(details.Open())
  {
    printf("Dataset details (mast_id = %d) opened. Rows: %d\n",p_master,details.GetNumberOfRecords());
  }
  else
  {
    printf("Dataset details (mast_id = %d) ***NOT*** openend\n",p_master);
  }

  // Calculate the aggregation of the amount field
  int column = details.GetFieldNumber("amount");
  AggregateInfo info;
  details.Aggregate(column,info);

  printf("Aggregation of the field 'amount':\n"
         "- sum  = %14.2f\n"
         "- max  = %14.2f\n"
         "- min  = %14.2f\n"
         "- mean = %14.2f\n"
        ,info.m_sum,info.m_max,info.m_min,info.m_mean);
  return info.m_sum;
}

double
ReadMasterSet(SQLDatabase* p_dbs,long p_master,double p_amount)
{
  SQLDataSet master("master",p_dbs);
  master.SetPrimaryTable(g_schema,"master");
  master.SetPrimaryKeyColumn("id");
  master.SetParameter("key",SQLVariant(p_master));
  CString sql ("SELECT id\n"
               "      ,invoice\n"
               "      ,description\n"
               "      ,total\n"
               "  FROM %SCHEMA%.master\n"
               " WHERE id = $key");
  master.SetQuery(sql);
  // Read in the dataset
  if(master.Open())
  {
    printf("Dataset master (id = %d) opened\n",p_master);
  }
  else
  {
    printf("Dataset master (id = %d) ***NOT*** opened!\n",p_master);
  }

  int fieldnum = master.GetFieldNumber("total");
  double total = master.GetCurrentField(fieldnum)->GetAsDouble();
  printf("Total amount of this record is: %.2f\n",total);

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
  printf("Testing the DataSet function:\n");
  printf("=============================\n");

  SQLDatabase dbs;
  dbs.AddMacro("%SCHEMA%.",g_schema);
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

      long   master = 2;
      double amount = 0.0;
      double total  = 0.0;
      amount = ReadDetailSet(&dbs,master);
      total  = ReadMasterSet(&dbs,master,amount);

      if(amount != total)
      {
        printf("Amounts differ. Check that database update has been done!\n");
      }

      TestAppend(&dbs,master);
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
  printf("DATASET test performed in: %.4f seconds\n", (double)(endTime - beginTime) / CLOCKS_PER_SEC);
}