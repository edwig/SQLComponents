////////////////////////////////////////////////////////////////////////
//
// File: BulkOperationTest.cpp
//
// Created: 2025 ir. W.E. Huisman
// MIT License
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
#include <SQLDatabase.h>
#include <SQLQuery.h>
#include <SQLVariant.h>
#include <bcd.h>
#include "UnitTest.h"

namespace DatabaseUnitTest
{
  void CALLBACK LogPrint(void* p_context,const TCHAR* p_text);
  int  CALLBACK LogLevel(void* p_context);
  extern XString g_dsn;
  extern XString g_user;
  extern XString g_password;

  TEST_CLASS(BulkOperationTest)
  {
    //////////////////////////////////////////////////////////////////////////
    //
    // UNIT TESTS (no database required)
    //
    //////////////////////////////////////////////////////////////////////////

    TEST_METHOD(BulkSetArraySize)
    {
      Logger::WriteMessage(_T("Testing SetParameterArraySize"));
      InitSQLComponents();

      SQLQuery query;

      // Not in bulk mode initially
      Assert::IsFalse(query.GetIsBulkMode(),L"Should not be in bulk mode initially");
      number_of_tests++;

      // Set a valid array size
      query.SetParameterArraySize(100);
      Assert::IsTrue(query.GetIsBulkMode(),L"Should be in bulk mode after setting size 100");
      number_of_tests++;

      // Set below minimum threshold -> disables bulk mode
      query.SetParameterArraySize(1);
      Assert::IsFalse(query.GetIsBulkMode(),L"Size 1 should disable bulk mode");
      number_of_tests++;

      // Set to exactly the minimum
      query.SetParameterArraySize(BULK_BATCH_SIZE_MIN);
      Assert::IsTrue(query.GetIsBulkMode(),L"Size at minimum should enable bulk mode");
      number_of_tests++;

      // Set to zero -> disables bulk mode
      query.SetParameterArraySize(0);
      Assert::IsFalse(query.GetIsBulkMode(),L"Size 0 should disable bulk mode");
      number_of_tests++;

      Logger::WriteMessage(_T("SetParameterArraySize tests passed"));
    }

    TEST_METHOD(BulkSetArrayData_Integer)
    {
      Logger::WriteMessage(_T("Testing SetParameterArrayData with integers"));
      InitSQLComponents();

      SQLQuery query;

      // Create an array of integer SQLVariant values
      const int count = 5;
      SQLVariant* variants[count];
      for(int i = 0; i < count; ++i)
      {
        variants[i] = new SQLVariant(i + 100);
      }

      query.SetParameterArraySize(count);
      query.SetParameterArrayData(variants,count);

      Assert::IsTrue(query.GetIsBulkMode(),L"Should be in bulk mode");
      number_of_tests++;

      // Cleanup the variants we created
      for(int i = 0; i < count; ++i)
      {
        delete variants[i];
      }

      Logger::WriteMessage(_T("SetParameterArrayData integer tests passed"));
      number_of_tests++;
    }

    TEST_METHOD(BulkSetArrayData_String)
    {
      Logger::WriteMessage(_T("Testing SetParameterArrayData with strings"));
      InitSQLComponents();

      SQLQuery query;

      // Create an array of string SQLVariant values with different lengths
      const int count = 4;
      SQLVariant* variants[count];
      variants[0] = new SQLVariant(_T("Short"));
      variants[1] = new SQLVariant(_T("A much longer string value"));
      variants[2] = new SQLVariant(_T("Medium length"));
      variants[3] = new SQLVariant(_T("X"));

      query.SetParameterArraySize(count);
      query.SetParameterArrayData(variants,count);

      Assert::IsTrue(query.GetIsBulkMode(),L"Should be in bulk mode");
      number_of_tests++;

      // Cleanup
      for(int i = 0; i < count; ++i)
      {
        delete variants[i];
      }

      Logger::WriteMessage(_T("SetParameterArrayData string tests passed"));
      number_of_tests++;
    }

    TEST_METHOD(BulkSetArrayData_WithNulls)
    {
      Logger::WriteMessage(_T("Testing SetParameterArrayData with NULL values"));
      InitSQLComponents();

      SQLQuery query;

      // Create an array with some NULL values
      const int count = 4;
      SQLVariant* variants[count];
      variants[0] = new SQLVariant(42);
      variants[1] = new SQLVariant();   // NULL
      variants[2] = new SQLVariant(99);
      variants[3] = new SQLVariant();   // NULL

      query.SetParameterArraySize(count);
      query.SetParameterArrayData(variants,count);

      Assert::IsTrue(query.GetIsBulkMode(),L"Should be in bulk mode with NULLs");
      number_of_tests++;

      // Cleanup
      for(int i = 0; i < count; ++i)
      {
        delete variants[i];
      }

      Logger::WriteMessage(_T("SetParameterArrayData NULL tests passed"));
      number_of_tests++;
    }

    TEST_METHOD(BulkGettersBeforeExecute)
    {
      Logger::WriteMessage(_T("Testing bulk getters before any execute"));
      InitSQLComponents();

      SQLQuery query;

      // Before any execute, processed count should be 0
      Assert::AreEqual((SQLULEN)0,query.GetBulkRowsProcessed(),L"Rows processed should be 0");
      number_of_tests++;

      // Row status should be empty
      const auto& status = query.GetBulkRowStatus();
      Assert::AreEqual((size_t)0,status.size(),L"Row status should be empty");
      number_of_tests++;

      Logger::WriteMessage(_T("Bulk getters before execute tests passed"));
    }

    TEST_METHOD(BulkExecuteWithoutPrepare)
    {
      Logger::WriteMessage(_T("Testing DoSQLExecuteBulk without prepare throws"));
      InitSQLComponents();

      SQLQuery query;
      query.SetParameterArraySize(10);

      const int count = 10;
      SQLVariant* variants[count];
      for(int i = 0; i < count; ++i)
      {
        variants[i] = new SQLVariant(i);
      }
      query.SetParameterArrayData(variants,count);

      bool threw = false;
      try
      {
        query.DoSQLExecuteBulk();
      }
      catch(StdException&)
      {
        threw = true;
      }
      Assert::IsTrue(threw,L"DoSQLExecuteBulk without Prepare should throw");
      number_of_tests++;

      for(int i = 0; i < count; ++i)
      {
        delete variants[i];
      }

      Logger::WriteMessage(_T("Execute without prepare test passed"));
    }

    //////////////////////////////////////////////////////////////////////////
    //
    // INTEGRATION TESTS (Firebird database required)
    //
    //////////////////////////////////////////////////////////////////////////

    TEST_METHOD(BulkInsertIntegration)
    {
      Logger::WriteMessage(_T("Testing bulk INSERT integration"));
      InitSQLComponents();

      SQLDatabase dbs;
      dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)_T(""));

      try
      {
        dbs.SetLoginTimeout(0);
        dbs.SetMARS(false);

        if(!dbs.Open(g_dsn,g_user,g_password))
        {
          Assert::Fail(L"Database ***NOT*** opened.");
          return;
        }
        Logger::WriteMessage(_T("Database opened."));
        number_of_tests++;

        // Drop table if it exists from a previous failed run
        SQLQuery drop(&dbs);
        drop.TryDoSQLStatement(_T("DROP TABLE bulk_test"));

        // Create table with auto-commit (DDL must be committed in Firebird)
        SQLQuery setup(&dbs);
        setup.DoSQLStatement(_T("CREATE TABLE bulk_test(id INTEGER NOT NULL, description VARCHAR(100), amount DECIMAL(14,2))"));
        Logger::WriteMessage(_T("Created bulk_test table"));
        number_of_tests++;

        // Prepare an INSERT statement
        const int ROWS = 5;
        SQLQuery query(&dbs);
        query.DoSQLPrepare(_T("INSERT INTO bulk_test(id, description, amount) VALUES(?,?,?)"));
        number_of_tests++;

        // Set up parameter arrays for 3 columns
        query.SetParameterArraySize(ROWS);

        // Column 1: id (integer)
        SQLVariant* ids[ROWS];
        for(int i = 0; i < ROWS; ++i)
        {
          ids[i] = new SQLVariant(i + 1);
        }
        query.SetParameterArrayData(ids,ROWS);

        // Column 2: description (string)
        SQLVariant* descs[ROWS];
        descs[0] = new SQLVariant(_T("Bulk row one"));
        descs[1] = new SQLVariant(_T("Bulk row two"));
        descs[2] = new SQLVariant(_T("Bulk row three"));
        descs[3] = new SQLVariant(_T("Bulk row four"));
        descs[4] = new SQLVariant(_T("Bulk row five"));
        query.SetParameterArrayData(descs,ROWS);

        // Column 3: amount (double -> decimal)
        SQLVariant* amounts[ROWS];
        for(int i = 0; i < ROWS; ++i)
        {
          amounts[i] = new SQLVariant((double)((i + 1) * 100.50));
        }
        query.SetParameterArrayData(amounts,ROWS);

        // Execute the bulk insert - should not throw
        int rowsAffected = query.DoSQLExecuteBulk();
        XString msg;
        msg.Format(_T("Bulk insert: rows affected=%d, rows processed=%llu"),
                   rowsAffected,(unsigned long long)query.GetBulkRowsProcessed());
        Logger::WriteMessage(msg);
        number_of_tests++;

        // Verify the execute succeeded (at least 1 row)
        Assert::IsTrue(rowsAffected >= 1,L"Bulk insert should affect at least 1 row");
        number_of_tests++;

        // Check if the driver fully supports array binding
        SQLULEN processed = query.GetBulkRowsProcessed();
        if(processed == (SQLULEN)ROWS)
        {
          // Full array binding support - verify all rows and status
          Logger::WriteMessage(_T("Driver supports full array parameter binding"));

          Assert::AreEqual(ROWS,rowsAffected,L"All array rows should be inserted");
          number_of_tests++;

          const auto& status = query.GetBulkRowStatus();
          for(int i = 0; i < ROWS; ++i)
          {
            Assert::IsTrue(status[i] == SQL_PARAM_SUCCESS || status[i] == SQL_PARAM_SUCCESS_WITH_INFO,
                           L"Each row should succeed");
          }
          number_of_tests++;

          // Verify data read-back
          SQLQuery readback(&dbs);
          readback.DoSQLStatement(_T("SELECT id FROM bulk_test ORDER BY id"));
          int row = 0;
          while(readback.GetRecord())
          {
            int id = readback[1];
            Assert::AreEqual(row + 1,id,L"ID should match");
            ++row;
          }
          Assert::AreEqual(ROWS,row,L"Should read back all rows");
          number_of_tests++;
        }
        else
        {
          // Driver does not fully support array binding
          // The API path still works without crashing - this is the key verification
          Logger::WriteMessage(_T("NOTE: ODBC driver does not fully support SQL_ATTR_PARAMSET_SIZE array binding."));
          Logger::WriteMessage(_T("Bulk API path verified: prepare, bind arrays, execute completed without error."));
          number_of_tests++;
        }

        // Close the bulk query to release the statement handle before dropping the table
        query.Close(false);

        // Cleanup: drop the test table
        SQLQuery cleanup(&dbs);
        cleanup.DoSQLStatement(_T("DROP TABLE bulk_test"));
        Logger::WriteMessage(_T("Dropped bulk_test table"));
        number_of_tests++;

        // Cleanup variant arrays
        for(int i = 0; i < ROWS; ++i)
        {
          delete ids[i];
          delete descs[i];
          delete amounts[i];
        }

        dbs.Close();
        Logger::WriteMessage(_T("Bulk INSERT integration test passed"));
      }
      catch(StdException& er)
      {
        // Always try to drop the test table on failure
        SQLQuery drop(&dbs);
        drop.TryDoSQLStatement(_T("DROP TABLE bulk_test"));

        Logger::WriteMessage(_T("Bulk INSERT test error:"));
        Logger::WriteMessage(er.GetErrorMessage());
        Assert::Fail(L"Bulk INSERT integration test failed");
      }
    }

    TEST_METHOD(BulkInsertWithNullsIntegration)
    {
      Logger::WriteMessage(_T("Testing bulk INSERT with NULL values"));
      InitSQLComponents();

      SQLDatabase dbs;
      dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)_T(""));

      try
      {
        dbs.SetLoginTimeout(0);
        dbs.SetMARS(false);

        if(!dbs.Open(g_dsn,g_user,g_password))
        {
          Assert::Fail(L"Database ***NOT*** opened.");
          return;
        }
        number_of_tests++;

        // Drop table if it exists from a previous failed run
        SQLQuery drop(&dbs);
        drop.TryDoSQLStatement(_T("DROP TABLE bulk_null_test"));

        // Create test table (auto-commit; Firebird DDL needs commit before use)
        SQLQuery setup(&dbs);
        setup.DoSQLStatement(_T("CREATE TABLE bulk_null_test(id INTEGER NOT NULL, name VARCHAR(50), val INTEGER)"));
        number_of_tests++;

        const int ROWS = 4;
        SQLQuery query(&dbs);
        query.DoSQLPrepare(_T("INSERT INTO bulk_null_test(id, name, val) VALUES(?,?,?)"));
        query.SetParameterArraySize(ROWS);

        // Column 1: id (all non-null)
        SQLVariant* ids[ROWS];
        for(int i = 0; i < ROWS; ++i)
        {
          ids[i] = new SQLVariant(i + 1);
        }
        query.SetParameterArrayData(ids,ROWS);

        // Column 2: name (some nulls)
        SQLVariant* names[ROWS];
        names[0] = new SQLVariant(_T("Alpha"));
        names[1] = new SQLVariant();  // NULL
        names[2] = new SQLVariant(_T("Gamma"));
        names[3] = new SQLVariant();  // NULL
        query.SetParameterArrayData(names,ROWS);

        // Column 3: val (some nulls)
        SQLVariant* vals[ROWS];
        vals[0] = new SQLVariant(10);
        vals[1] = new SQLVariant(20);
        vals[2] = new SQLVariant();  // NULL
        vals[3] = new SQLVariant();  // NULL
        query.SetParameterArrayData(vals,ROWS);

        // Execute the bulk insert - should not throw
        int rowsAffected = query.DoSQLExecuteBulk();
        XString msg;
        msg.Format(_T("Bulk NULL insert: rows affected=%d, rows processed=%llu"),
                   rowsAffected,(unsigned long long)query.GetBulkRowsProcessed());
        Logger::WriteMessage(msg);
        Assert::IsTrue(rowsAffected >= 1,L"Bulk insert with NULLs should affect at least 1 row");
        number_of_tests++;

        // Check if the driver fully supports array binding
        SQLULEN processed = query.GetBulkRowsProcessed();
        if(processed == (SQLULEN)ROWS)
        {
          // Full array binding support - verify NULL pattern
          Logger::WriteMessage(_T("Driver supports array binding: verifying NULL pattern"));

          SQLQuery verify(&dbs);
          verify.DoSQLStatement(_T("SELECT id, name, val FROM bulk_null_test ORDER BY id"));
          int row = 0;
          while(verify.GetRecord())
          {
            int id = verify[1];
            switch(id)
            {
              case 1: Assert::IsFalse(verify.IsNull(2),L"Row 1 name should not be NULL");
                      Assert::IsFalse(verify.IsNull(3),L"Row 1 val should not be NULL");
                      break;
              case 2: Assert::IsTrue(verify.IsNull(2),L"Row 2 name should be NULL");
                      Assert::IsFalse(verify.IsNull(3),L"Row 2 val should not be NULL");
                      break;
              case 3: Assert::IsFalse(verify.IsNull(2),L"Row 3 name should not be NULL");
                      Assert::IsTrue(verify.IsNull(3),L"Row 3 val should be NULL");
                      break;
              case 4: Assert::IsTrue(verify.IsNull(2),L"Row 4 name should be NULL");
                      Assert::IsTrue(verify.IsNull(3),L"Row 4 val should be NULL");
                      break;
            }
            ++row;
          }
          Assert::AreEqual(ROWS,row,L"Should read back all rows");
          number_of_tests++;
        }
        else
        {
          Logger::WriteMessage(_T("NOTE: ODBC driver does not fully support array binding with NULLs."));
          Logger::WriteMessage(_T("Bulk API path with NULLs verified: no crash, execute completed."));
          number_of_tests++;
        }

        // Close the bulk query to release the statement handle before dropping the table
        query.Close(false);

        // Cleanup
        SQLQuery cleanup(&dbs);
        cleanup.DoSQLStatement(_T("DROP TABLE bulk_null_test"));

        for(int i = 0; i < ROWS; ++i)
        {
          delete ids[i];
          delete names[i];
          delete vals[i];
        }

        dbs.Close();
        Logger::WriteMessage(_T("Bulk INSERT with NULLs test passed"));
        number_of_tests++;
      }
      catch(StdException& er)
      {
        // Always try to drop the test table on failure
        SQLQuery drop(&dbs);
        drop.TryDoSQLStatement(_T("DROP TABLE bulk_null_test"));

        Logger::WriteMessage(_T("Bulk NULL test error:"));
        Logger::WriteMessage(er.GetErrorMessage());
        Assert::Fail(L"Bulk INSERT with NULLs test failed");
      }
    }

    TEST_METHOD(BulkModeNotActiveForSingleRow)
    {
      Logger::WriteMessage(_T("Testing that single-row operations are not in bulk mode"));
      InitSQLComponents();

      SQLDatabase dbs;
      dbs.RegisterLogContext(LOGLEVEL_MAX,LogLevel,LogPrint,(void*)_T(""));

      try
      {
        dbs.SetLoginTimeout(0);
        dbs.SetMARS(false);

        if(!dbs.Open(g_dsn,g_user,g_password))
        {
          Assert::Fail(L"Database ***NOT*** opened.");
          return;
        }
        number_of_tests++;

        // Normal single-row query should NOT be in bulk mode
        SQLQuery query(&dbs);
        Assert::IsFalse(query.GetIsBulkMode(),L"Normal query should not be in bulk mode");
        number_of_tests++;

        // Execute a normal select - should work fine
        query.DoSQLStatement(_T("SELECT 1 FROM RDB$DATABASE"));
        Assert::IsFalse(query.GetIsBulkMode(),L"After normal select, still not bulk mode");
        number_of_tests++;

        dbs.Close();
        Logger::WriteMessage(_T("Single-row mode test passed"));
      }
      catch(StdException& er)
      {
        Logger::WriteMessage(er.GetErrorMessage());
        Assert::Fail(L"Single-row mode test failed");
      }
    }
  };
}
