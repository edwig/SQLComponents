////////////////////////////////////////////////////////////////////////
//
// File: BasaicDatabase.cpp
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
// Last Revision:   11-11-2016
// Version number:  1.2.0
//
#include "stdafx.h"
#include "CppUnitTest.h"
#include "SQLDatabase.h"
#include "SQLDataSet.h"
#include "SQLInfoTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DatabaseUnitTest
{
  // MS-Access
  // DSN "testing" points to the "basis.accdb" database
  // in the project rootfolder
  //
  CString g_dsn("testing");
  CString g_user("sa");
  CString g_password("word");

  void CALLBACK LogPrint(void* p_context,const char* p_text)
  {
    CString message((const char*)p_context);
    message += p_text;
    Logger::WriteMessage(message);
  }

  int CALLBACK LogLevel(void* p_context)
  {
    return 6; // = LOGLEVEL_MAX from SQLDatabase.h
  }

  TEST_CLASS(BasicDatabaseTest)
  {
  public:
    TEST_METHOD(DatabaseConnection)
    {
      Logger::WriteMessage("In unit test Database connection");

      SQLDatabase dbs;
      dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");

      long beginTime = clock();

      try
      {
        // Set options for the MS-Access database
        dbs.SetLoginTimeout(0);
        dbs.SetMARS(false);

        if(dbs.Open(g_dsn,g_user,g_password))
        {
          Logger::WriteMessage("Database opened.");
        }
        else
        {
          Logger::WriteMessage("Database ***NOT*** opened.");
        }
      }
      catch(CString& s)
      {
        Logger::WriteMessage("Database ***NOT*** opened. Reason:");
        Logger::WriteMessage(s);
      }
      catch(...)
      {
        Logger::WriteMessage("Database ***NOT*** opened for unknown reasons");
      }
      long endTime = clock();
      CString performance;
      performance.Format("Open  test performed in: %.6f seconds",(double)(endTime - beginTime) / CLOCKS_PER_SEC);
      Logger::WriteMessage(performance);
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
      performance.Format("Close test performed in: %.6f seconds",(double)(endTime - beginTime) / CLOCKS_PER_SEC);
      Logger::WriteMessage(performance);
    }

    // For DATASET tests
    double TestAppend(SQLDatabase* p_dbs,long p_master)
    {
      CString msg;
      Logger::WriteMessage("Testing the DataSet append function:");
      Logger::WriteMessage("====================================");

      SQLDataSet details("detail",p_dbs);
      details.SetPrimaryTable("detail");
      details.SetPrimaryKeyColumn("id");
      details.SetSearchableColumn("id");
      details.SetParameter("mast_id",SQLVariant(p_master));
      details.SetSelection("*"); // Select all columns

      if(details.Open())
      {
        msg.Format("Dataset details (mast_id = %d) opened. Rows: %d",p_master,details.GetNumberOfRecords());
        Logger::WriteMessage(msg);
      }
      else
      {
        msg.Format("Dataset details (mast_id = %d) ***NOT*** openend",p_master);
        Logger::WriteMessage(msg);
        Assert::Fail(L"Details table not opened");
      }

      // Prev master record
      msg.Format("Dataset append. Number of records: %d",details.GetNumberOfRecords());
      Logger::WriteMessage(msg);
      --p_master;
      details.SetParameter("mast_id",SQLVariant(p_master));
      details.Append();
      msg.Format("Dataset append. Number of records: %d",details.GetNumberOfRecords());
      Logger::WriteMessage(msg);

      // Calculate the aggregation of the amount field
      int column = details.GetFieldNumber("amount");
      AggregateInfo info;
      details.Aggregate(column,info);

      msg.Format("Aggregation of the field 'amount':\n"
                 "- sum  = %14.2f\n"
                 "- max  = %14.2f\n"
                 "- min  = %14.2f\n"
                 "- mean = %14.2f\n"
                 ,info.m_sum,info.m_max,info.m_min,info.m_mean);
      Logger::WriteMessage(msg);

      Logger::WriteMessage("Testing the object cache");
      SQLRecord* record = details.FindObjectRecord(8);
      SQLVariant* var = record->GetField(details.GetFieldNumber("id"));
      msg.Format("Found record with id: [%d]",var->GetAsSLong());
      Logger::WriteMessage(msg);

      return info.m_sum;
    }

    double ReadDetailSet(SQLDatabase* p_dbs,long p_master)
    {
      CString msg;
      SQLDataSet details("detail",p_dbs);
      details.SetPrimaryTable("detail");
      details.SetPrimaryKeyColumn("id");
      details.SetParameter("mast_id",SQLVariant(p_master));
      details.SetSelection("*"); // Select all columns

      // Alternatively to "SetSelection", we may specify a complete query
      //   CString sql("SELECT id\n"
      //               "      ,mast_id\n"
      //               "      ,line\n"
      //               "      ,description\n"
      //               "      ,amount\n"
      //               "  FROM detail\n"
      //               " WHERE mast_id = $mast_id");
      //   details.SetQuery(sql);

      if(details.Open())
      {
        msg.Format("Dataset details (mast_id = %d) openend. Rows: %d",p_master,details.GetNumberOfRecords());
        Logger::WriteMessage(msg);
      }
      else
      {
        msg.Format("Dataset details (mast_id = %d) ***NOT*** openend",p_master);
        Logger::WriteMessage(msg);
        Assert::Fail(L"Table details not opened");
      }

      // Calculate the aggregation of the amount field
      int column = details.GetFieldNumber("amount");
      AggregateInfo info;
      details.Aggregate(column,info);

      msg.Format("Aggregation of the field 'amount':\n"
                 "- sum  = %14.2f\n"
                 "- max  = %14.2f\n"
                 "- min  = %14.2f\n"
                 "- mean = %14.2f\n"
                 ,info.m_sum,info.m_max,info.m_min,info.m_mean);
      Logger::WriteMessage(msg);
      return info.m_sum;
    }

    double ReadMasterSet(SQLDatabase* p_dbs,long p_master,double p_amount)
    {
      CString msg;
      SQLDataSet master("master",p_dbs);
      master.SetPrimaryTable("master");
      master.SetPrimaryKeyColumn("id");
      master.SetParameter("key",SQLVariant(p_master));
      CString sql("SELECT id\n"
                  "      ,invoice\n"
                  "      ,description\n"
                  "      ,total\n"
                  "  FROM master\n"
                  " WHERE id = $key");
      master.SetQuery(sql);
      // Read in the dataset
      if(master.Open())
      {
        msg.Format("Dataset master (id = %d) opened",p_master);
        Logger::WriteMessage(msg);
      }
      else
      {
        msg.Format("Dataset master (id = %d) ***NOT*** opened!",p_master);
        Logger::WriteMessage(msg);
        Assert::Fail(L"Table master not opened");
      }

      int fieldnum = master.GetFieldNumber("total");
      double total = master.GetCurrentField(fieldnum)->GetAsDouble();
      msg.Format("Total amount of this record is: %.2f",total);
      Logger::WriteMessage(msg);

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


    TEST_METHOD(TestDataSet)
    {
      Logger::WriteMessage("Testing the DataSet function:");
      Logger::WriteMessage("=============================");

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
          Logger::WriteMessage("Database opened.");

          long   master = 2;
          double amount = 0.0;
          double total  = 0.0;
          amount = ReadDetailSet(&dbs,master);
          total  = ReadMasterSet(&dbs,master,amount);

          if(amount != total)
          {
            Assert::Fail(L"Amounts differ. Check that database update has been done!");
          }
          TestAppend(&dbs,master);
        }
        else
        {
          Assert::Fail(L"Database ***NOT*** opened.");
        }
        dbs.Close();
      }
      catch(CString& s)
      {
        Logger::WriteMessage("Database error. Reason:");
        Logger::WriteMessage(s);
        Assert::Fail(L"Database error");
      }
      catch(...)
      {
        Assert::Fail(L"Unknown error in database test");
      }
      long endTime = clock();
      CString msg;
      msg.Format("DATASET test performed in: %.4f seconds",(double)(endTime - beginTime) / CLOCKS_PER_SEC);
      Logger::WriteMessage(msg);
    }

    TEST_METHOD(BasicTreeControl)
    {
      Logger::WriteMessage("Testing the InfoTree function:");
      Logger::WriteMessage("==============================");
      CTreeCtrl ctrl;
      CRect rect(0,0,0,0);
      CWnd* wnd = CWnd::FromHandle(GetDesktopWindow());

      if(!ctrl.Create(WS_CHILD | WS_TABSTOP | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_HASLINES | TVS_SHOWSELALWAYS
                     ,rect,wnd,1004))
      {
        Assert::Fail(L"Cannot create tree control");
      }

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
          SQLInfoTree tree(&dbs);
          CString filename("Report.txt");
          tree.MakeTreeInfo(&ctrl);
          tree.ReportAllCapabilities(&ctrl,filename);
        }
        else
        {
          Assert::Fail(L"Database ***NOT*** opened.");
        }
        dbs.Close();
      }
      catch(...)
      {
        Assert::Fail(L"Unknown error in database test");
      }
    }
  };
}