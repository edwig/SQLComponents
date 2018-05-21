////////////////////////////////////////////////////////////////////////
//
// File: BasaicDatabase.cpp
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
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include "SQLDataSet.h"
#include "SQLInfoDB.h"
#include "SQLAssociation.h"
#include "bcd.h"
#include <time.h>

#include "UnitTest.h"

namespace DatabaseUnitTest
{
  // Firebird Open Source RDBMS 3.0
  // DSN "testing" points to the "testing.fdb" database
  // in the project root folder
  //
  CString g_dsn("testing");
  CString g_user("sysdba");
  CString g_password("altijd");
  CString g_schema("");

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
      InitSQLComponents();

      SQLDatabase dbs;
      dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");
      number_of_tests++;

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
          Assert::Fail(L"Database ***NOT*** opened.");
        }
        number_of_tests++;
      }
      catch(StdException* er)
      {
        Logger::WriteMessage(MessageFromException(er));
        Assert::Fail(L"Database ***NOT*** opened. Reason:");
        er->Delete();
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

    TEST_METHOD(TestQuery)
    {
      Logger::WriteMessage("In unit test Database query function");

      double grandTotal = 0.0;
      CString sql("SELECT id\n"
                  "      ,invoice\n"
                  "      ,description\n" 
                  "      ,total\n" 
                  "  FROM master");

      InitSQLComponents();

      SQLDatabase dbs;
      dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");
      try
      {
        // Set options for the MS-Access database
        dbs.SetLoginTimeout(0);
        dbs.SetMARS(false);

        if(dbs.Open(g_dsn,g_user,g_password))
        {
          Logger::WriteMessage("Database opened.");

          SQLQuery query(&dbs);
          query.DoSQLStatement(sql);
          while(query.GetRecord())
          {
            long    id       = query[1];
            long    invoice  = query[2];
            CString descript = query[3];
            double  total    = query[4];

            grandTotal += total;

            Logger::WriteMessage(descript);
          }
        }
        else
        {
          Assert::Fail(L"Database ***NOT*** opened.");
        }
        number_of_tests++;

        // Real test 750 + 600 = 1350
        Assert::AreEqual((double)1350.0,grandTotal);
        number_of_tests++;
      }
      catch(StdException* er)
      {
        Logger::WriteMessage(MessageFromException(er));
        Assert::Fail(L"Database ***NOT*** opened. Reason:");
        er->Delete();
      }
    }

    // For DATASET tests
    double TestAppend(SQLDatabase* p_dbs,long p_master)
    {
      CString msg;
      Logger::WriteMessage("Testing the DataSet append function:");
      Logger::WriteMessage("====================================");

      SQLDataSet details("detail",p_dbs);
      details.SetPrimaryTable(g_schema,"detail");
      details.SetPrimaryKeyColumn("id");
      details.SetParameter("mast_id",SQLVariant(p_master));
      details.SetSelection("*"); // Select all columns
      number_of_tests += 5;

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
      number_of_tests++;

      // Prev master record
      msg.Format("Dataset append. Number of records: %d",details.GetNumberOfRecords());
      Logger::WriteMessage(msg);
      --p_master;
      details.SetParameter("mast_id",SQLVariant(p_master));
      details.Append();
      msg.Format("Dataset append. Number of records: %d",details.GetNumberOfRecords());
      Logger::WriteMessage(msg);
      number_of_tests++;

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
      number_of_tests++;

      Logger::WriteMessage("Testing the object cache");
      SQLRecord* record = details.FindObjectRecord(8);
      SQLVariant* var = record->GetField(details.GetFieldNumber("id"));
      msg.Format("Found record with id: [%d]",var->GetAsSLong());
      Logger::WriteMessage(msg);
      number_of_tests++;

      // Test saving as an XML file
      SetDefaultSQLLanguage(LN_DUTCH);
      bool saved = details.XMLSave("SQLDataSet_Details.xml","TestDetails");
      Assert::AreEqual(true,saved);
      number_of_tests++;

      return info.m_sum;
    }

    double ReadDetailSet(SQLDatabase* p_dbs,long p_master)
    {
      CString msg;
      SQLDataSet details("detail",p_dbs);
      details.SetPrimaryTable(g_schema,"detail");
      details.SetPrimaryKeyColumn("id");
      details.SetParameter("mast_id",SQLVariant(p_master));
      details.SetSelection("*"); // Select all columns
      number_of_tests += 4;

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
      number_of_tests++;

      // Calculate the aggregation of the amount field
      int column = details.GetFieldNumber("amount");
      AggregateInfo info;
      details.Aggregate(column,info);
      number_of_tests++;

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
      master.SetPrimaryTable(g_schema,"master");
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
      number_of_tests++;

      int fieldnum = master.GetFieldNumber("total");
      double total = master.GetCurrentField(fieldnum)->GetAsDouble();
      msg.Format("Total amount of this record is: %.2f",total);
      Logger::WriteMessage(msg);
      number_of_tests++;

      // Change field
      SQLRecord* record = master.GetRecord(0);
      SQLVariant var(p_amount);
      record->ModifyField(fieldnum,&var,1);
      number_of_tests++;

      // TEST
      // master.CancelMutation(1);

      // Synchronize with the database
      master.Synchronize();
      number_of_tests++;

      return total;
    }


    TEST_METHOD(TestDataSet)
    {
      Logger::WriteMessage("Testing the DataSet function:");
      Logger::WriteMessage("=============================");

      InitSQLComponents();

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
          number_of_tests++;
        }
        else
        {
          Assert::Fail(L"Database ***NOT*** opened.");
        }
        dbs.Close();
      }
      catch(StdException* er)
      {
        Logger::WriteMessage("Database error. Reason:");
        Logger::WriteMessage(MessageFromException(er));
        Assert::Fail(L"Database error");
        er->Delete();
      }
      long endTime = clock();
      number_of_tests++;

      CString msg;
      msg.Format("DATASET test performed in: %.4f seconds",(double)(endTime - beginTime) / CLOCKS_PER_SEC);
      Logger::WriteMessage(msg);
    }

    TEST_METHOD(BasicReadNumeric)
    {
      Logger::WriteMessage("Testing Numeric reading/writing:");
      Logger::WriteMessage("================================");

      InitSQLComponents();

      SQLDatabase dbs;
      dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");
      long beginTime = clock();
      number_of_tests++;

//       g_dsn = "ok2b01";
//       g_user = "k2b";
//       g_password = "k2b";

      try
      {
        // Set options for the database
        dbs.SetLoginTimeout(0);
        dbs.SetMARS(false);

        if(dbs.Open(g_dsn,g_user,g_password))
        {
          Logger::WriteMessage("Database opened.");

          CString sql = "SELECT field2,field3 FROM test_number";
          SQLQuery query(&dbs);

          query.DoSQLStatement(sql);
          while(query.GetRecord())
          {
            double field1 = query[1];
            bcd    field2 = query[2];

            CString msg;
            msg.Format("Field 1 [%.4f] Field 2 [%s]",field1,field2.AsString());
            Logger::WriteMessage(msg);
          }
          number_of_tests++;

          // Change value
          SQLTransaction trans(&dbs,"UpdNum");
          CString sql2 = "UPDATE test_number\n"
                         "   SET field3 = ?\n"
                         " WHERE id     = 1";
          bcd num(3034.6);
          query.SetConcurrency(SQL_CONCUR_LOCK);
          query.SetParameter(1,num);
          query.DoSQLStatementNonQuery(sql2);
          trans.Commit();
          number_of_tests++;

          // Reselect
          sql += " WHERE id = 1";
          query.DoSQLStatement(sql);
          if(query.GetRecord())
          {
            bcd test = query[2];
            Assert::IsTrue(test == num,L"NUMERIC incorrectly updated");
          }
          number_of_tests++;

          // Return to original value
          bcd one(1);
          query.SetParameter(1,one);
          trans.Start("UpdNum");
          query.DoSQLStatementNonQuery(sql2);
          trans.Commit();

          number_of_tests++;
        }
        else
        {
          Assert::Fail(L"Database ***NOT*** opened.");
        }
       dbs.Close();
      }
      catch(StdException* er)
      {
        Logger::WriteMessage("Database error. Reason:");
        Logger::WriteMessage(MessageFromException(er));
        Assert::Fail(L"Database error");
        er->Delete();
      }
      long endTime = clock();
      number_of_tests++;

      CString msg;
      msg.Format("NUMERIC test performed in: %.4f seconds",(double)(endTime - beginTime) / CLOCKS_PER_SEC);
      Logger::WriteMessage(msg);
    }

    bcd  FindAssociationSet(SQLDataSet&     master
                           ,SQLDataSet&     detail
                           ,SQLAssociation& assoc
                           ,long            p_set)
    {
      // Set up a value
      SQLVariant val(p_set);
      assoc.SetAssociation(0,&val);
      number_of_tests++;

      // Finding the master record
      SQLRecord* masterRecord = assoc.FollowToMaster();
      if(masterRecord)
      {
        SQLVariant* desc = masterRecord->GetField("description");
        CString name;
        desc->GetAsString(name);
        Logger::WriteMessage("Master record: " + name);
      }
      else
      {
        Assert::Fail(L"Master record 2 not found");
      }
      number_of_tests++;

      // Finding the detail records
      RecordSet* recSet = assoc.FollowToDetails();
      bcd total;
      for(auto& rec : *recSet)
      {
        bcd num = rec->GetField("amount")->GetAsBCD();
        Logger::WriteMessage("Detail record: " + num.AsString());
        total += num;
        number_of_tests++;
      }
      Logger::WriteMessage("Total of details: " + total.AsString());
      number_of_tests++;

      // Delete details resultset
      delete recSet;

      return total;
    }


    TEST_METHOD(AssociationTest)
    {
      Logger::WriteMessage("Testing association following master/detail:");
      Logger::WriteMessage("============================================");

      InitSQLComponents();

      SQLDatabase dbs;
      dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");

      try
      {
        // Set options for the database
        dbs.SetLoginTimeout(0);
        dbs.SetMARS(false);

        if(dbs.Open(g_dsn,g_user,g_password))
        {
          Logger::WriteMessage("Database opened.");
          number_of_tests++;

          // Set up the datasets
          SQLDataSet master("master",&dbs);
          SQLDataSet detail("detail",&dbs);
          CString    masterQuery = "SELECT * FROM master";
          CString    detailQuery = "SELECT * FROM detail";

          master.SetQuery(masterQuery);
          detail.SetQuery(detailQuery);
          master.SetPrimaryTable(g_schema,"master");
          detail.SetPrimaryTable(g_schema,"detail");
          master.SetPrimaryKeyColumn("id");
          detail.SetPrimaryKeyColumn("id");
          number_of_tests += 8;

          // Set up an association
          SQLAssociation assoc(&master,&detail);
          assoc.SetAssociation("id","mast_id");
          number_of_tests++;

          // Print the set
          bcd set2 = FindAssociationSet(master,detail,assoc,2L);
          bcd set1 = FindAssociationSet(master,detail,assoc,1L);

          Assert::AreEqual(750L,set1.AsLong());
          Assert::AreEqual(600L,set2.AsLong());
          number_of_tests += 2;
        }
        else
        {
          Assert::Fail(L"Database ***NOT*** opened.");
        }
        dbs.Close();
        number_of_tests++;
      }
      catch(StdException* er)
      {
        Logger::WriteMessage("Database error. Reason:");
        Logger::WriteMessage(MessageFromException(er));
        Assert::Fail(L"Database error");
        er->Delete();
      }
    }

//     TEST_METHOD(BasicUpdateCursor)
//     {
//       Logger::WriteMessage("Testing UPDATE WHERE CURRENT OF:");
//       Logger::WriteMessage("================================");
// 
//       SQLDatabase dbs;
//       dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");
//       long beginTime = clock();
// 
//       try
//       {
//         // Set options for the database
//         dbs.SetLoginTimeout(0);
//         dbs.SetMARS(false);
// 
//         if(dbs.Open(g_dsn,g_user,g_password))
//         {
//           Logger::WriteMessage("Database opened.");
// 
//           bcd theValue;
//           CString sql1 = "SELECT field3\n"
//                          "  FROM test_number\n"
//                          " WHERE id = 1";
// 
//           CString sql2 = "UPDATE test_number\n"
//                         "   SET field3 = ?\n"
//                         " WHERE CURRENT OF ";
//           SQLQuery query(&dbs);
//           SQLTransaction trans(&dbs,"UpdCursor");
//           query.SetConcurrency(SQL_CONCUR_LOCK);
// 
//           query.DoSQLStatement(sql1);
//           if(query.GetRecord())
//           {
//             bcd value = query[1];
//             theValue  = value * 2;
// 
//             query.SetParameter(1,theValue);
//             sql2 += query.GetCursorName();
//             query.DoSQLStatementNonQuery(sql2);
//           }
//           trans.Commit();
// 
//           // Checking the value
//           
//           query.ResetParameters();
//           SQLVariant* result = query.DoSQLStatementScalar(sql1);
//           bcd check = result->GetAsBCD();
// 
//           Assert::AreEqual(theValue.AsString().GetString(),check.AsString().GetString());
//         }
//         else
//         {
//           Assert::Fail(L"Database ***NOT*** opened.");
//         }
//         dbs.Close();
//       }
//       catch(StdException* er)
//       {
//         Logger::WriteMessage("Database error. Reason:");
//         Logger::WriteMessage(MessageFromException(er));
//         Assert::Fail(L"Database error");
//         er->Delete();
//       }
//       long endTime = clock();
//       CString msg;
//       msg.Format("NUMERIC test performed in: %.4f seconds",(double)(endTime - beginTime) / CLOCKS_PER_SEC);
//       Logger::WriteMessage(msg);
//     }

    //////////////////////////////////////////////////////////////////////////
    //
    // TESTING DBInfo
    //
    //////////////////////////////////////////////////////////////////////////

    void ReportString(CString p_prompt,CString p_info)
    {
      CString text;
      text.Format("%s %s",p_prompt,p_info);
      Logger::WriteMessage(text);
    }

    void ReportBoolean(CString p_prompt,bool p_info)
    {
      CString text;
      text.Format("%s %s",p_prompt,p_info ? "true" : "false");
      Logger::WriteMessage(text);
    }

    void ReportLong(CString p_prompt,long p_info)
    {
      CString text;
      text.Format("%s %d",p_prompt,p_info);
      Logger::WriteMessage(text);
    }

    void ReportSQL(CString p_prompt,CString p_sql)
    {
      Logger::WriteMessage(p_prompt);
      if(p_sql.IsEmpty())
      {
        Logger::WriteMessage("<NO SQL GIVEN BY DBINFO>");
      }
      else
      {
        Logger::WriteMessage(p_sql);
      }
    }

    void ReportDBInfo(SQLInfoDB* p_info)
    {
      CString schema ("dbo");
      CString table  ("master");
      CString column ("id");
      CString type   ("integer");
      CString select ("SELECT id FROM master\n");
      CString columns("oid,description,amount");
      CString procedure("procedure");
      CString mode("auto");
      CString destiny("variable");
      CString source("\'Test'\"");
      CString indexname("01_master_id");
      CString condition("variable IS NOT NULL");
      CString foreign("fk_master_one");
      CString triggername("master_check");
      CString viewname("myview");
      CString procedurename("calculate");
      CString tabledef("(ID INTEGER,NAME VARCHAR(200),NUMBER DECIMAL(14,2));");
      CString precursor;
      CString cursorname("selcursor");
      CString splParam1("CHAR");
      CString splParam2("NUMERIC");
      CString quoteString("This is 'a' string");
      int     statements = 0;
      bool    notnull    = true;

      MetaIndex index;
      index.m_indexName  = indexname;
      index.m_columnName = "id";
      index.m_position   = 0;
      index.m_unique     = true;
      index.m_ascending  = "A";

      MetaTable    tables;
      MetaColumn   metacolumns;
      MetaIndex    metaIndex;
      MIndicesMap  indices;
      MPrimaryMap  primaries;
      MForeignMap  foreigns;
      MetaTrigger  trigger;
      MetaSequence sequence;
      MetaProcedure metaproc;
      MParameterMap parameters;
      std::vector<CString> cursorcolumns;    // Cursor fetch
      std::vector<CString> cursorparameters; // Fetch into

      // RDBMS
      ReportLong   ("Database type                      :",p_info->GetRDBMSDatabaseType());
      ReportString ("Database vendor name               :",p_info->GetRDBMSVendorName());
      number_of_tests += 2;
    //ReportString ("Database physical name             :",p_info->GetRDBMSPhysicalDatabaseName());
      ReportBoolean("Catalog is in upper case           :",p_info->GetRDBMSIsCatalogUpper());
      ReportBoolean("Database understands schemas       :",p_info->GetRDBMSUnderstandsSchemas());
      ReportBoolean("Driver understands comments        :",p_info->GetRDBMSSupportsComments());
      ReportBoolean("Support deferred constraints       :",p_info->GetRDBMSSupportsDeferredConstraints());
      ReportBoolean("Support order-by expressions       :",p_info->GetRDBMSSupportsOrderByExpression());
      ReportBoolean("Supports {[?=] CALL escapes}       :",p_info->GetRDBMSSupportsComments());
      ReportBoolean("Time stored as decimal             :",p_info->GetRDBMSSupportsDatatypeTime());
      ReportBoolean("Interval type supported            :",p_info->GetRDBMSSupportsDatatypeInterval());
      ReportLong   ("Maximum SQL length                 :",p_info->GetRDBMSMaxStatementLength());
      ReportBoolean("Must Commit DDL                    :",p_info->GetRDBMSMustCommitDDL());
      number_of_tests += 10;
      // KEYWORDS
      ReportString ("Database DATETIME keyword          :",p_info->GetKEYWORDCurrentTimestamp());
      ReportString ("Database date keyword              :",p_info->GetKEYWORDCurrentDate());
      ReportString ("Concatenation operator             :",p_info->GetKEYWORDConcatanationOperator());
      ReportString ("Quote character                    :",p_info->GetKEYWORDQuoteCharacter());
      ReportString ("Default NULL for parameter         :",p_info->GetKEYWORDParameterDefaultNULL());
      ReportString ("Parameter is for INPUT and OUTPUT  :",p_info->GetKEYWORDParameterINOUT());
      ReportString ("Parameter is for OUTPUT only       :",p_info->GetKEYWORDParameterOUT());
      ReportString ("Network dbs primary key type       :",p_info->GetKEYWORDNetworkPrimaryKeyType());
      ReportString ("Datatype for TIMESTAMP / DATETIME  :",p_info->GetKEYWORDTypeTimestamp());
      ReportString ("Parameter prefix in PSM module     :",p_info->GetKEYWORDParameterPrefix());
      ReportString ("Identity string                    :",p_info->GetKEYWORDIdentityString(table));
      ReportString ("Function UPPER()                   :",p_info->GetKEYWORDUpper(source));
      ReportString ("INTERVAL 1 minute ago              :",p_info->GetKEYWORDInterval1MinuteAgo());
      ReportString("Statement Not Null Value (NVL)     :", p_info->GetKEYWORDStatementNVL(column,source));
      number_of_tests += 14;
      // SQL's
      ReportSQL    ("Generate new serial                :",p_info->GetSQLGenerateSerial(table));
      ReportString ("Effective serial                   :",p_info->GetSQLEffectiveSerial("oid"));
      ReportSQL    ("Start new subtransaction           :",p_info->GetSQLStartSubTransaction(table));
      ReportSQL    ("Commit subtransaction              :",p_info->GetSQLCommitSubTransaction(table));
      ReportSQL    ("Rollback subtransaction            :",p_info->GetSQLRollbackSubTransaction(table));
      ReportString ("FROM part for single row select    :",p_info->GetSQLFromDualClause());
      ReportSQL    ("SQL lock table                     :",p_info->GetSQLLockTable(schema,table,true));
      ReportSQL    ("SQL Optimize table                 :",p_info->GetSQLOptimizeTable(schema,table));
      ReportString ("SQL string quotes                  :",p_info->GetSQLString(quoteString));
      ReportString ("SQL date string                    :",p_info->GetSQLDateString(1959,15,10));
      ReportString ("SQL time string                    :",p_info->GetSQLTimeString(15,50,20));
      ReportString ("SQL datetime string (timestamp)    :",p_info->GetSQLDateTimeString(1959,10,15,15,50,20));
      ReportString ("SQL datetime bound string          :",p_info->GetSQLDateTimeBoundString());
      ReportString ("SQL datetime stripped string       :",p_info->GetSQLDateTimeStrippedString(1959,10,15,15,50,20));
      number_of_tests += 14;
      // CATALOG FUNCTION SQL
      // CATALOG TABLE
      ReportSQL    ("TABLE exists                       :",p_info->GetCATALOGTableExists    (schema,table));
      ReportSQL    ("TABLE list                         :",p_info->GetCATALOGTablesList     (schema,table));
      ReportSQL    ("TABLE attributes                   :",p_info->GetCATALOGTableAttributes(schema,table));
      ReportSQL    ("TABLE Synonyms                     :",p_info->GetCATALOGTableSynonyms  (schema,table));
      ReportSQL    ("TABLE Catalogs                     :",p_info->GetCATALOGTableCatalog   (schema,table));
      ReportSQL    ("TABLE exists                       :",p_info->GetCATALOGTableExists    (schema,table));
      ReportSQL    ("TABLE create                       :",p_info->GetCATALOGTableCreate    (tables,metacolumns));
      ReportSQL    ("TABLE Rename                       :",p_info->GetCATALOGTableRename    (schema,table,table + "_ext"));
      ReportSQL    ("TABLE Drop                         :",p_info->GetCATALOGTableDrop      (schema,table));
      number_of_tests += 9;
      // CATALOG Temp table
      ReportSQL    ("TEMP TABLE create                  :",p_info->GetCATALOGTemptableCreate  (schema,table,select));
      ReportSQL    ("TEMP TABLE select into temp        :",p_info->GetCATALOGTemptableIntoTemp(schema,table,select));
      ReportSQL    ("TEMP TABLE drop                    :",p_info->GetCATALOGTemptableDrop    (schema,table));
      number_of_tests += 3;
      // CATALOG Columns
      ReportSQL    ("COLUMN exists                      :",p_info->GetCATALOGColumnExists    (schema,table,column));
      ReportSQL    ("COLUMN all column info of a table  :",p_info->GetCATALOGColumnList      (schema,table));
      ReportSQL    ("COLUMN all attributes of the column:",p_info->GetCATALOGColumnAttributes(schema,table,column));
      ReportSQL    ("COLUMN Create                      :",p_info->GetCATALOGColumnCreate    (metacolumns));
      ReportSQL    ("COLUMN Alter                       :", p_info->GetCATALOGColumnAlter    (metacolumns));
      ReportSQL    ("COLUMN Rename                      :",p_info->GetCATALOGColumnRename    (schema,table,column,column + "_ext",type));
      ReportSQL    ("COLUMN Drop                        :",p_info->GetCATALOGColumnDrop      (schema,table,column));
      number_of_tests += 7;
      // CATALOG Index
      ReportSQL    ("INDEX exists                       :",p_info->GetCATALOGIndexExists    (schema,table,indexname));
      ReportSQL    ("INDEX list all indices of a table  :",p_info->GetCATALOGIndexList      (schema,table));
      ReportSQL    ("INDEX attributes of an index       :",p_info->GetCATALOGIndexAttributes(schema,table,indexname));
      ReportSQL    ("INDEX Create                       :",p_info->GetCATALOGIndexCreate    (indices));
      ReportSQL    ("INDEX Drop                         :",p_info->GetCATALOGIndexDrop      (schema,table,indexname));
      number_of_tests += 5;
      //ReportSQL    ("INDEX Filter                       :",p_info->GetCATALOGIndexFilter    (metaIndex));
      // CATALOG Primary key
      ReportSQL    ("PRIMARY KEY exists                 :",p_info->GetCATALOGPrimaryExists    (schema,table));
      ReportSQL    ("PRIMARY KEY attributes             :",p_info->GetCATALOGPrimaryAttributes(schema,table));
      ReportSQL    ("PRIMARY KEY create                 :",p_info->GetCATALOGPrimaryCreate    (primaries));
      ReportSQL    ("PRIMARY KEY drop                   :",p_info->GetCATALOGPrimaryDrop      (schema,table,"pk_" + table));
      number_of_tests += 4;
      // CATALOG Foreign key
      ReportSQL    ("FOREIGN KEY exists                 :",p_info->GetCATALOGForeignExists    (schema,table,foreign));
      ReportSQL    ("FOREIGN KEY list                   :",p_info->GetCATALOGForeignList      (schema,table));
      ReportSQL    ("FOREIGN KEY attributes             :",p_info->GetCATALOGForeignAttributes(schema,table,foreign));
    //ReportSQL    ("FOREIGN KEY create                 :",p_info->GetCATALOGForeignCreate    (foreigns));
      ReportSQL    ("FOREIGN KEY alter                  :",p_info->GetCATALOGForeignAlter     (foreigns,foreigns));
      ReportSQL    ("FOREIGN KEY drop                   :",p_info->GetCATALOGForeignDrop      (schema,table,foreign));
      number_of_tests += 5;
      // CATALOG Triggers
      ReportSQL    ("TRIGGER exists                     :",p_info->GetCATALOGTriggerExists    (schema,table,triggername));
      ReportSQL    ("TRIGGER lists                      :",p_info->GetCATALOGTriggerList      (schema,table));
      ReportSQL    ("TRIGGER attributes                 :",p_info->GetCATALOGTriggerAttributes(schema,table,triggername));
      ReportSQL    ("TRIGGER create                     :",p_info->GetCATALOGTriggerCreate    (trigger));
      ReportSQL    ("TRIGGER drop                       :",p_info->GetCATALOGTriggerDrop      (schema,table,triggername));
      number_of_tests += 5;
      // CATALOG Sequence
      ReportSQL    ("SEQUENCE exists                    :",p_info->GetCATALOGSequenceExists    (schema,table));
      ReportSQL    ("SEQUENCE lists                     :",p_info->GetCATALOGSequenceList      (schema,table));
      ReportSQL    ("SEQUENCE attributes                :",p_info->GetCATALOGSequenceAttributes(schema,table));
      ReportSQL    ("SEQUENCE create                    :",p_info->GetCATALOGSequenceCreate    (sequence));
      ReportSQL    ("SEQUENCE drop                      :",p_info->GetCATALOGSequenceDrop      (schema,table + "_seq"));
      number_of_tests += 5;
      // CATALOG View
      ReportSQL    ("VIEW exists                        :",p_info->GetCATALOGViewExists    (schema,viewname));
      ReportSQL    ("VIEW list                          :",p_info->GetCATALOGViewList      (schema,viewname));
      ReportSQL    ("VIEW attributes                    :",p_info->GetCATALOGViewAttributes(schema,viewname));
      ReportSQL    ("VIEW Create                        :",p_info->GetCATALOGViewCreate    (schema,table,select));
      ReportSQL    ("VIEW Drop                          :",p_info->GetCATALOGViewDrop      (schema,table,precursor));
      number_of_tests += 5;
      // PERSISTENT STORED MODULES
      ReportSQL    ("PSM PROCEDURE exists               :",p_info->GetPSMProcedureExists    (schema,procedurename));
      ReportSQL    ("PSM PROCEDURE list                 :",p_info->GetPSMProcedureList      (schema));
      ReportSQL    ("PSM PROCEDURE attributes           :",p_info->GetPSMProcedureAttributes(schema,procedurename));
      ReportSQL    ("PSM PROCEDURE sourcecode           :",p_info->GetPSMProcedureSourcecode(schema,procedurename));
      ReportSQL    ("PSM PROCEDURE create               :",p_info->GetPSMProcedureCreate    (metaproc));
      ReportSQL    ("PSM PROCEDURE drop                 :",p_info->GetPSMProcedureDrop      (schema,procedurename));
    //ReportSQL    ("PSM PROCEDURE errors               :",p_info->GetPSMProcedureErrors    (schema,procedurename));
      ReportSQL    ("PSM PROCEDURE parameters           :",p_info->GetPSMProcedureParameters(schema,procedurename));
      number_of_tests += 7;
      // PSM elements
    //ReportString ("PSM Declaration                    :",p_info->GetPSMDeclaration(true,destiny,SQL_INTEGER,10,0,"","",""));
      ReportString ("Assignment to variable             :",p_info->GetPSMAssignment(destiny,source));
      ReportString ("PSM IF statement                   :",p_info->GetPSMIF(condition));
      ReportString ("PSM IF else                        :",p_info->GetPSMIFElse());
      ReportString ("PSM IF End of the IF               :",p_info->GetPSMIFEnd());
      ReportString ("PSM WHILE loop                     :",p_info->GetPSMWhile(condition));
      ReportString ("PSM WHILE end                      :",p_info->GetPSMWhileEnd());
      ReportString ("PSM LOOP                           :",p_info->GetPSMLOOP());
      ReportString ("PSM LOOP end                       :",p_info->GetPSMLOOPEnd());
      ReportString ("PSM BREAK                          :",p_info->GetPSMBREAK());
      ReportString ("PSM RETURN                         :",p_info->GetPSMRETURN(source));
      ReportString ("PSM EXECUTE                        :",p_info->GetPSMExecute(procedurename,parameters));
      ReportString ("PSM CURSOR declaration             :",p_info->GetPSMCursorDeclaration(destiny,select));
      ReportString ("PSM CURSOR Fetch                   :",p_info->GetPSMCursorFetch(cursorname,cursorcolumns,cursorparameters));
      ReportString ("PSM exception catch NODATA         :",p_info->GetPSMExceptionCatchNoData());
      ReportString ("PSM exception catch SQLSTATE       :",p_info->GetPSMExceptionCatch("HY001"));
      ReportString ("PSM exception raise                :",p_info->GetPSMExceptionRaise("NODATA"));
      number_of_tests += 16;
      // SESSION CONTROL
      ReportSQL    ("SESSION Getting session/terminal   :",p_info->GetSESSIONMyself());
      ReportSQL    ("SESSION exists                     :",p_info->GetSESSIONExists("123"));
      ReportSQL    ("SESSION list                       :",p_info->GetSESSIONList());
      ReportSQL    ("SESSION attributes                 :",p_info->GetSESSIONExists("123"));
      ReportSQL    ("SET ALL CONSTRAINTS DEFERRED       :",p_info->GetSESSIONConstraintsDeferred());
      ReportSQL    ("SET ALL CONSTRAINTS IMMEDIATE      :",p_info->GetSESSIONConstraintsImmediate());
      number_of_tests += 6;
    }
    
    void ReportOracle()
    {
      Logger::WriteMessage("Reporting on DBInfo for Oracle database");
      Logger::WriteMessage("---------------------------------------");

      SQLDatabase dbs;
      dbs.SetDatabaseType(RDBMS_ORACLE);
      SQLInfoDB* info = dbs.GetSQLInfoDB();
      ReportDBInfo(info);
      dbs.Close();
      number_of_tests++;
    }

    void ReportInformix()
    {
      Logger::WriteMessage("Reporting on DBInfo for Informix database");
      Logger::WriteMessage("-----------------------------------------");

      SQLDatabase dbs;
      dbs.SetDatabaseType(RDBMS_INFORMIX);
      SQLInfoDB* info = dbs.GetSQLInfoDB();
      ReportDBInfo(info);
      dbs.Close();
      number_of_tests++;
    }

    void ReportSQLServer()
    {
      Logger::WriteMessage("Reporting on DBInfo for SQL Server database");
      Logger::WriteMessage("-------------------------------------------");

      SQLDatabase dbs;
      dbs.SetDatabaseType(RDBMS_SQLSERVER);
      SQLInfoDB* info = dbs.GetSQLInfoDB();
      ReportDBInfo(info);
      dbs.Close();
      number_of_tests++;
    }

    void ReportFirebird()
    {
      Logger::WriteMessage("Reporting on DBInfo for Firebird database");
      Logger::WriteMessage("-----------------------------------------");

      SQLDatabase dbs;
      dbs.SetDatabaseType(RDBMS_FIREBIRD);
      SQLInfoDB* info = dbs.GetSQLInfoDB();
      ReportDBInfo(info);
      dbs.Close();
      number_of_tests++;
    }

    void ReportPostgreSQL()
    {
      Logger::WriteMessage("Reporting on DBInfo for PostgreSQL database");
      Logger::WriteMessage("-------------------------------------------");

      SQLDatabase dbs;
      dbs.SetDatabaseType(RDBMS_POSTGRESQL);
      SQLInfoDB* info = dbs.GetSQLInfoDB();
      ReportDBInfo(info);
      dbs.Close();
      number_of_tests++;
    }

    void ReportMSAccess()
    {
      Logger::WriteMessage("Reporting on DBInfo for MS-Access database");
      Logger::WriteMessage("------------------------------------------");

      SQLDatabase dbs;
      dbs.SetDatabaseType(RDBMS_ACCESS);
      SQLInfoDB* info = dbs.GetSQLInfoDB();
      ReportDBInfo(info);
      dbs.Close();
      number_of_tests++;
    }

    void ReportGenericODBC()
    {
      Logger::WriteMessage("Reporting on DBInfo for Generic ODBC database");
      Logger::WriteMessage("---------------------------------------------");

      SQLDatabase dbs;
      dbs.SetDatabaseType(RDBMS_ODBC_STANDARD);
      SQLInfoDB* info = dbs.GetSQLInfoDB();
      ReportDBInfo(info);
      dbs.Close();
      number_of_tests++;
    }

    TEST_METHOD(TestDBInfo)
    {
      Logger::WriteMessage("Reporting on DBInfo");
      Logger::WriteMessage("===================");

      ReportOracle();
      ReportInformix();
      ReportSQLServer();
      ReportFirebird();
      ReportPostgreSQL();
      ReportMSAccess();
      ReportGenericODBC();
    }
  };
}