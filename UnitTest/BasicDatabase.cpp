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
// Last Revision:   01-02-2017
// Version number:  1.3.3
//
#include "stdafx.h"
#include "SQLComponents.h"
#include "CppUnitTest.h"
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include "SQLDataSet.h"
#include "SQLInfoTree.h"
#include "SQLInfoDB.h"
#include "bcd.h"
#include <time.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SQLComponents;

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

    TEST_METHOD(TestQuery)
    {
      Logger::WriteMessage("In unit test Database query function");

      double grandTotal = 0.0;
      CString sql("SELECT id\n"
                  "      ,invoice\n"
                  "      ,description\n" 
                  "      ,total\n" 
                  "  FROM master");


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
          Logger::WriteMessage("Database ***NOT*** opened.");
        }
        // Real test 750 + 600 = 1350
        Assert::AreEqual((double)1350.0,grandTotal);
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

#ifdef SQL_COMPONENTS_MFC

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

#endif

    TEST_METHOD(BasicReadNumeric)
    {
      Logger::WriteMessage("Testing Numeric reading/writing:");
      Logger::WriteMessage("================================");

      InitSQLComponents();

      SQLDatabase dbs;
      dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)"");
      long beginTime = clock();

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

          // Reselect
          sql += " WHERE id = 1";
          query.DoSQLStatement(sql);
          if(query.GetRecord())
          {
            bcd test = query[2];
            Assert::IsTrue(test == num,L"NUMERIC incorrectly updated");
          }

          // Return to original value
          bcd one(1);
          query.SetParameter(1,one);
          trans.Start("UpdNum");
          query.DoSQLStatementNonQuery(sql2);
          trans.Commit();
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
      msg.Format("NUMERIC test performed in: %.4f seconds",(double)(endTime - beginTime) / CLOCKS_PER_SEC);
      Logger::WriteMessage(msg);
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
//       catch(CString& s)
//       {
//         Logger::WriteMessage("Database error. Reason:");
//         Logger::WriteMessage(s);
//         Assert::Fail(L"Database error");
//       }
//       catch(...)
//       {
//         Assert::Fail(L"Unknown error in database test");
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
      CString tabledef("(ID INTEGER,NAME VARCHAR(200),NUMBER DECIMAL(14,2));");
      CString precursor;
      CString cursorname("selcursor");
      CString splParam1("CHAR");
      CString splParam2("NUMERIC");
      CString quoteString("This is 'a' string");
      int     statements = 0;
      bool    notnull    = true;

      DBForeign foreign;
      foreign.m_constraintname    = "FK_detail_master";
      foreign.m_schema            = schema;
      foreign.m_tablename         = "detail";
      foreign.m_column            = "master_id";
      foreign.m_primaryTable      = table;
      foreign.m_primaryColumn     = column;
      foreign.m_enabled           = true;
      foreign.m_deferrable        = true;
      foreign.m_initiallyDeffered = false;
      foreign.m_match             = 0; // 0=Full, 1=partial, 2=simple
      foreign.m_updateRule        = 0; // 0=restrict, 1=cascade, 2=set null, 3=set default, 4=No action
      foreign.m_deleteRule        = 1; // 0=restrict, 1=cascade, 2=set null, 3=set default, 4=No action

      DBIndex index[2];

      index[0].m_indexName  = indexname;
      index[0].m_column     = "id";
      index[0].m_position   = 0;
      index[0].m_unique     = true;
      index[0].m_descending = true;
      index[1].m_indexName  = "";

      ReportString ("Database vendor name               :",p_info->GetDatabaseVendorName());
      ReportBoolean("Catalog is in upper case           :",p_info->IsCatalogUpper());
      ReportBoolean("Database understands schemas       :",p_info->GetUnderstandsSchemas());
      ReportBoolean("Driver understands comments        :",p_info->SupportsDatabaseComments());
      ReportBoolean("Support deferred constraints       :",p_info->SupportsDeferredConstraints());
      ReportBoolean("Support order-by expressions       :",p_info->SupportsOrderByExpression());
      ReportSQL    ("SQL to get default col value       :",p_info->GetSQLStringDefaultValue(table,column));
      ReportString ("Database DATETIME keyword          :",p_info->GetSystemDateTimeKeyword());
      ReportString ("Database date keyword              :",p_info->GetSystemDateString());
      ReportBoolean("Time stored as decimal             :",p_info->GetTimeIsDecimal());
      ReportBoolean("Interval stored as decimal         :",p_info->GetIntervalIsDecimal());
      ReportString ("Concatenation operator             :",p_info->GetConcatanationOperator());
      ReportString ("Quote character                    :",p_info->GetQuoteCharacter());
      ReportString ("DEFAULT NULL in tables             :",p_info->GetDefaultNULL());
      ReportString ("INOUT Parameter                    :",p_info->GetParameterINOUT());
      ReportString ("OUT Parameter                      :",p_info->GetParameterOUT());
      ReportString ("Audit user datatype                :",p_info->GetAuditUserDatatype());
      ReportString ("Audit user datatype variable       :",p_info->GetAuditUserDatatypeAsVariable());
      ReportString ("IDENTITY Primary key               :",p_info->GetPrimaryKeyType());
      ReportString ("DATETIME year-to-second type       :",p_info->GetDatetimeYearToSecondType());
      ReportString ("ALTER CONSTRAINT separator         :",p_info->GetAlterConstraintSeparator());
      ReportString ("INNER JOIN keyword                 :",p_info->GetInnerJoinKeyword());
      ReportString ("OUTER JOIN keyword                 :",p_info->GetOuterJoinKeyword());
      ReportString ("INNER JOIN keyword (in views)      :",p_info->GetViewInnerJoinKeyword());
      ReportString ("OUTER JOIN keyword (in views)      :",p_info->GetViewOuterJoinKeyword());
      ReportString ("OUTER JOIN closure                 :",p_info->GetOuterJoinClosure());
      ReportString ("OUTER JOIN sign in WHERE           :",p_info->GetOuterJoinSign());
      ReportString ("Stored procedure parameter prefix  :",p_info->GetSPParamPrefix());
      ReportString ("Identity string                    :",p_info->GetIdentityString(table));
      ReportSQL    ("Create temporary table             :",p_info->GetSQLCreateTemporaryTable(table,select));
      ReportSQL    ("Remove temporary table             :",p_info->GetSQLRemoveTemporaryTable(table,statements));
      ReportSQL    ("SELECT into temporary table        :",p_info->GetSQLSelectIntoTemp(table,select));
      ReportString ("Replace OID by sequence            :",p_info->GetReplaceColumnOIDbySequence(columns,table));
      ReportSQL    ("Remove procedure dependencies      :",p_info->GetSQLRemoveProcedureDependencies(procedure));
      ReportSQL    ("Remove field dependencies          :",p_info->GetSQLRemoveFieldDependencies(table));
      ReportSQL    ("Primary key definition (persistent):",p_info->GetPrimaryKeyDefinition(schema,table,false));
      ReportSQL    ("Primary key definition (temporary) :",p_info->GetPrimaryKeyDefinition(schema,table,true));
      ReportSQL    ("Primary key constraint             :",p_info->GetPrimaryKeyConstraint(schema,table,column));
      ReportSQL    ("Foreign key constraint             :",p_info->GetSQLForeignKeyConstraint(foreign));
      ReportSQL    ("Alter foreign key constraint       :",p_info->GetSQLAlterForeignKey(foreign,foreign));
      ReportSQL    ("Performance settings               :",p_info->GetSQLPerformanceSettings());
      ReportSQL    ("Cache mode settings                :",p_info->GetSQLCacheModeSetting(mode));
      ReportSQL    ("NLS Numeric characters             :",p_info->GetSQLNlsNumericCharacters());
      ReportSQL    ("Alter table column name            :",p_info->GetSQLModifyColumnName(table,column,"oid","INTEGER"));
      ReportLong   ("Maximum SQL length                 :",p_info->GetMaxStatementLength());
      ReportString ("Modify datatype prefix             :",p_info->GetModifyDatatypePrefix());
      ReportString ("Name of a temporary table          :",p_info->GetCodeTemporaryTable());
      ReportSQL    ("ROW LOCKING mode                   :",p_info->GetCodeLockModeRow());
      ReportString ("Temporary table with no log        :",p_info->GetCodeTempTableWithNoLog());
      ReportSQL    ("GRANT ALL on table to user         :",p_info->GetSQLGrantAllOnTable(schema,table,false));
      ReportSQL    ("GRANT ALL on table (GRANT OPTION)  :",p_info->GetSQLGrantAllOnTable(schema,table,true));
      ReportSQL    ("SELECT INTO TEMP prefix            :",p_info->GetSelectIntoTempClausePrefix(table));
      ReportSQL    ("SELECT INTO TEMP suffix            :",p_info->GetSelectIntoTempClauseSuffix(table));
      ReportBoolean("IF statement needs BEGIN/END       :",p_info->GetCodeIfStatementBeginEnd());
      ReportString ("End of the IF statement            :",p_info->GetCodeEndIfStatement());
      ReportString ("Assignment to variable             :",p_info->GetAssignmentStatement(destiny,source));
      ReportString ("Keyword for ALTER COLUMN           :",p_info->GetCodeAlterColumn());
      ReportString ("Start of a WHILE loop              :",p_info->GetStartWhileLoop(condition));
      ReportString ("End   of a WHILE loop              :",p_info->GetEndWhileLoop());
      ReportBoolean("Parenthesis for SELECT assignment  :",p_info->GetAssignmentSelectParenthesis());
      ReportString ("UPPER function                     :",p_info->GetUpperFunction(destiny));
      ReportString ("INTERVAL 1 minute ago              :",p_info->GetInterval1MinuteAgo());
      ReportSQL    ("Generate new serial                :",p_info->GetSQLGenerateSerial(table));
      ReportString ("Effective serial                   :",p_info->GetSQLEffectiveSerial("oid"));
      ReportString ("NVL statement                      :",p_info->GetNVLStatement(destiny,source));
      ReportSQL    ("Start new subtransaction           :",p_info->GetStartSubTransaction(table));
      ReportSQL    ("Commit subtransaction              :",p_info->GetCommitSubTransaction(table));
      ReportSQL    ("Rollback subtransaction            :",p_info->GetRollbackSubTransaction(table));
      ReportString ("DUAL table name                    :",p_info->GetDualTableName());
      ReportString ("FROM part for single row select    :",p_info->GetDualClause());
      ReportSQL    ("DEFERRABLE for a constraint        :",p_info->GetConstraintDeferrable());
      ReportSQL    ("SET ALL CONSTRAINTS DEFERRED       :",p_info->GetSQLDeferConstraints());
      ReportSQL    ("SET ALL CONSTRAINTS IMMEDIATE      :",p_info->GetSQLConstraintsImmediate());
      ReportSQL    ("Get all column info of a table     :",p_info->GetSQLGetColumns(schema,table));
      ReportSQL    ("Get all constraints of a table     :",p_info->GetSQLGetConstraintsForTable(table));
      ReportSQL    ("Get all indices of a table         :",p_info->GetSQLTableIndices(schema,table));
      ReportSQL    ("Create an index                    :",p_info->GetSQLCreateIndex(schema,table,index));
      ReportSQL    ("Drop an index                      :",p_info->GetSQLDropIndex(schema,indexname));
      ReportSQL    ("Get table references               :",p_info->GetSQLTableReferences(schema,table));
      ReportSQL    ("Getting the state of a sequence    :",p_info->GetSQLSequence(schema,table));
      ReportSQL    ("Create a new sequence for a table  :",p_info->GetSQLCreateSequence(schema,table));
      ReportSQL    ("Drop the sequence for a table      :",p_info->GetSQLDropSequence(schema,table));
      ReportSQL    ("Getting the rights for a sequence  :",p_info->GetSQLSequenceRights(schema,table));
      ReportSQL    ("Getting session and terminal       :",p_info->GetSQLSessionAndTerminal());
      ReportSQL    ("Get session exists                 :",p_info->GetSQLSessionExists("123"));
      ReportSQL    ("Get primary key information        :",p_info->GetSQLPrimaryKeyConstraintInformation(schema,table));
      ReportSQL    ("SQL to lock a table (exclusive)    :",p_info->GetSQLLockTable(table,true));
      ReportSQL    ("SQL to lock a table (share-read)   :",p_info->GetSQLLockTable(table,false));
      ReportSQL    ("Optimize performance of a table    :",p_info->GetSQLOptimizeTable(schema,table,statements));
//    ReportBoolean("Only one user session capability   :",p_info->GetOnlyOneUserSession());
      ReportSQL    ("Create a new table column          :",p_info->GetCreateColumn(schema,table,column,type,notnull));
      ReportSQL    ("Drop a column from a table         :",p_info->GetSQLDropColumn(schema,table,column));
      ReportSQL    ("Create one (1) foreign key         :",p_info->GetCreateForeignKey(table,CString("constraintname"),column,"master","OID"));
      ReportSQL    ("Modify table column type           :",p_info->GetModifyColumnType(schema,table,column,type));
      ReportSQL    ("Modify table column not null state :",p_info->GetModifyColumnNull(schema,table,column,notnull));
      ReportSQL    ("Drop view                          :",p_info->GetSQLDropView(schema,table,precursor));
      ReportSQL    ("Create or replace view             :",p_info->GetSQLCreateOrReplaceView(schema,table,select));
      ReportBoolean("Must create temp tables at runtime :",p_info->GetMustMakeTemptablesAtRuntime());
//    ReportSQL    ("Create temp table from content     :",p_info->DoMakeTemporaryTable(table,tabledef,column));
//    ReportSQL    ("Remove a temporary table           :",p_info->DoRemoveTemporaryTable(table));
      ReportString ("SPL assignment                     :",p_info->GetSPLAssignment(destiny));
      ReportString ("SPL Start a while loop             :",p_info->GetSPLStartWhileLoop(condition));
      ReportString ("SPL End while loop                 :",p_info->GetSPLEndWhileLoop());
      ReportString ("SPL Get procedure call             :",p_info->GetSQLSPLCall(procedure));
      ReportString ("SPL Builded parameter list (5)     :",p_info->GetBuildedParameterList(5));
      ReportString ("SPL Parameter type rename CHAR     :",p_info->GetParameterType(splParam1));
      ReportString ("SPL Parameter type rename NUMERIC  :",p_info->GetParameterType(splParam2));
      ReportString ("SQL string quotes                  :",p_info->GetSQLString(quoteString));
      ReportString ("SQL date string                    :",p_info->GetSQLDateString(1959,15,10));
      ReportString ("SQL time string                    :",p_info->GetSQLTimeString(15,50,20));
      ReportString ("SQL datetime string (timestamp)    :",p_info->GetSQLDateTimeString(1959,10,15,15,50,20));
      ReportString ("SQL datetime bound string          :",p_info->GetSQLDateTimeBoundString());
      ReportString ("SQL datetime stripped string       :",p_info->GetSQLDateTimeStrippedString(1959,10,15,15,50,20));
      ReportString ("SPL integer type                   :",p_info->GetSPLIntegerType());
      ReportString ("SPL decimal type                   :",p_info->GetSPLDecimalType());
      ReportString ("SPL cursor declaration             :",p_info->GetSPLCursorDeclaratie(destiny,select));
      ReportString ("SPL cursor name resultset          :",p_info->GetSPLCursorFound(destiny));
      ReportString ("SPL cursor row count variable      :",p_info->GetSPLCursorRowCount(destiny));
      ReportString ("SPL cursor row declaration         :",p_info->GetSPLCursorRowDeclaration(cursorname,destiny));
      ReportString ("SPL fetch cursor in row variable   :",p_info->GetSPLFetchCursorIntoRowVariable(cursorname,destiny));
      ReportString ("SPL no data found exception        :",p_info->GetSPLNoDataFoundExceptionClause());
      ReportString ("SPL raise exception                :",p_info->GetSPLRaiseException("NODATA"));
      ReportBoolean("SPL server proc with return values :",p_info->GetSPLServerFunctionsWithReturnValues());
      ReportString ("Translate error -97                :",p_info->TranslateErrortext(-1 ,"ORA--97 : Unreadable error about referenced master-detail objects"));
      ReportString ("Translate error -97                :",p_info->TranslateErrortext(-97,"Cannot remove object. Stil referenced by detail objects"));
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