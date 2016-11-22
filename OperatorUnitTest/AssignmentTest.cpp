////////////////////////////////////////////////////////////////////////
//
// File: AssignmentTest.cpp
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
#include "SQLVariant.h"
#include "SQLDate.h"
#include "SQLTime.h"
#include "SQLTimestamp.h"
#include "SQLInterval.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

BEGIN_TEST_MODULE_ATTRIBUTE()
  TEST_MODULE_ATTRIBUTE(L"Assignment",L"SQLVariant")
END_TEST_MODULE_ATTRIBUTE()

TEST_MODULE_INITIALIZE(ModuleInitialize)
{
  Logger::WriteMessage("BEGIN OF: SQLVariant operators");
}

TEST_MODULE_CLEANUP(ModuleCleanup)
{
  Logger::WriteMessage("END OF: SQLVariant operators");
}

namespace OperatorUnitTest
{		
	TEST_CLASS(OperatorUnitTest)
	{
	public:
		TEST_METHOD(AssignmentOperators)
		{
      Logger::WriteMessage("In unit test assignment operators");
    }

    TEST_METHOD(TestAssignChar)
    {
      Logger::WriteMessage("SQLVariant = CONST CHAR");

      SQLVariant one("23");
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("23",oneString);
      Assert::AreEqual(SQL_C_CHAR,one.GetDataType());
    }

    TEST_METHOD(TestAssignCString)
    {
      Logger::WriteMessage("SQLVariant = CString");

      CString twentythree("23");
      SQLVariant one(twentythree);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("23",oneString);
      Assert::AreEqual(SQL_C_CHAR,one.GetDataType());
    }

    TEST_METHOD(TestAssignSLong)
    {
      Logger::WriteMessage("SQLVariant = SLONG");

      SQLVariant one((long)23);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("23",oneString);
      Assert::AreEqual(SQL_C_SLONG,one.GetDataType());
    }

    TEST_METHOD(TestAssignULong)
    {
      Logger::WriteMessage("SQLVariant = ULONG");

      SQLVariant one((unsigned long)23);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("23",oneString);
      Assert::AreEqual(SQL_C_ULONG,one.GetDataType());
    }

    TEST_METHOD(TestAssignSShort)
    {
      Logger::WriteMessage("SQLVariant = SSHORT");

      SQLVariant one((short)23);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("23",oneString);
      Assert::AreEqual(SQL_C_SSHORT,one.GetDataType());
    }

    TEST_METHOD(TestAssignUShort)
    {
      Logger::WriteMessage("SQLVariant = USHORT");

      SQLVariant one((unsigned short)23);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("23",oneString);
      Assert::AreEqual(SQL_C_USHORT,one.GetDataType());
    }

    TEST_METHOD(TestAssignSTinyInt)
    {
      Logger::WriteMessage("SQLVariant = STINYINT");

      SQLVariant one((char)23);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("23",oneString);
      Assert::AreEqual(SQL_C_STINYINT,one.GetDataType());
    }

    TEST_METHOD(TestAssignUTinyInt)
    {
      Logger::WriteMessage("SQLVariant = UTINYINT");

      SQLVariant one((unsigned char)23);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("23",oneString);
      Assert::AreEqual(SQL_C_UTINYINT,one.GetDataType());
    }

    TEST_METHOD(TestAssignFloat)
    {
      Logger::WriteMessage("SQLVariant = FLOAT");

      SQLVariant one((float)23.0);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("23",oneString);
      Assert::AreEqual(SQL_C_FLOAT,one.GetDataType());
    }

    TEST_METHOD(TestAssignDouble)
    {
      Logger::WriteMessage("SQLVariant = DOUBLE");

      SQLVariant one((double)23.0);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("23",oneString);
      Assert::AreEqual(SQL_C_DOUBLE,one.GetDataType());
    }

    TEST_METHOD(TestAssignBool)
    {
      Logger::WriteMessage("SQLVariant = BOOL");

      SQLVariant one((bool)true);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("1",oneString);
      Assert::AreEqual(SQL_C_BIT,one.GetDataType());
    }

    TEST_METHOD(TestAssignSBigInt)
    {
      Logger::WriteMessage("SQLVariant = SBIGINT");

      SQLVariant one((__int64)23.0);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("23",oneString);
      Assert::AreEqual(SQL_C_SBIGINT,one.GetDataType());
    }

    TEST_METHOD(TestAssignUBigInt)
    {
      Logger::WriteMessage("SQLVariant = UBIGINT");

      SQLVariant one((unsigned __int64)23.0);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("23",oneString);
      Assert::AreEqual(SQL_C_UBIGINT,one.GetDataType());
    }

    TEST_METHOD(TestAssignDate)
    {
      Logger::WriteMessage("SQLVariant = SQLDate");

      SQLDate date("15-10-1959");
      SQLVariant one(&date);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("15-10-1959",oneString);
      Assert::AreEqual(SQL_C_DATE,one.GetDataType());
    }

    TEST_METHOD(TestAssignDateStruct)
    {
      Logger::WriteMessage("SQLVariant = &DATE_STRUCT");

      DATE_STRUCT date;
      date.day   = 15;
      date.month = 10;
      date.year  = 1959;
      SQLVariant one(&date);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("15-10-1959",oneString);
      Assert::AreEqual(SQL_C_DATE,one.GetDataType());
    }

    TEST_METHOD(TestAssignTime)
    {
      Logger::WriteMessage("SQLVariant = SQLTime");

      SQLTime time("15:55:42");
      SQLVariant one(&time);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("15:55:42",oneString);
      Assert::AreEqual(SQL_C_TIME,one.GetDataType());
    }

    TEST_METHOD(TestAssignTimeStruct)
    {
      Logger::WriteMessage("SQLVariant = &TIME_STRUCT");

      TIME_STRUCT time;
      time.hour   = 15;
      time.minute = 55;
      time.second = 42;
      SQLVariant one(&time);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("15:55:42",oneString);
      Assert::AreEqual(SQL_C_TIME,one.GetDataType());
    }

    TEST_METHOD(TestAssignTimestamp)
    {
      Logger::WriteMessage("SQLVariant = SQLTimestamp");

      SQLTimestamp stamp("1959-10-15 15:55:42");
      SQLVariant one(&stamp);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("1959-10-15 15:55:42",oneString);
      Assert::AreEqual(SQL_C_TIMESTAMP,one.GetDataType());
    }

    TEST_METHOD(TestAssignTimestampStruct)
    {
      Logger::WriteMessage("SQLVariant = &TIMESTAMP_STRUCT");

      TIMESTAMP_STRUCT stamp;
      stamp.year   = 1959;
      stamp.month  = 10;
      stamp.day    = 15;
      stamp.hour   = 15;
      stamp.minute = 55;
      stamp.second = 42;
      stamp.fraction = 0;
      SQLVariant one(&stamp);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("1959-10-15 15:55:42",oneString);
      Assert::AreEqual(SQL_C_TIMESTAMP,one.GetDataType());
    }

    TEST_METHOD(TestAssignIntervalYM)
    {
      Logger::WriteMessage("SQLVariant = SQLInterval Year-Month");

      SQLInterval intval("P57Y2M");
      SQLVariant one(&intval);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("57-2",oneString);
      Assert::AreEqual(SQL_C_INTERVAL_YEAR_TO_MONTH,one.GetDataType());
    }

    TEST_METHOD(TestAssignIntervalDS_String)
    {
      Logger::WriteMessage("SQLVariant = SQLInterval Day-Second from string");

      SQLInterval intval(SQL_IS_DAY_TO_SECOND,"20578 14:22:08");
      SQLVariant one(&intval);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("20578 14:22:8",oneString);
      Assert::AreEqual(SQL_C_INTERVAL_DAY_TO_SECOND,one.GetDataType());
    }

    TEST_METHOD(TestAssignIntervalDS_Values)
    {
      Logger::WriteMessage("SQLVariant = SQLInterval Day-Second from values");

      SQLInterval intval(SQL_IS_DAY_TO_SECOND,20578,14,22,8);
      SQLVariant one(&intval);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("20578 14:22:8",oneString);
      Assert::AreEqual(SQL_C_INTERVAL_DAY_TO_SECOND,one.GetDataType());
    }

//     TEST_METHOD(TestAssignIntervalDS_Period)
//     {
//       Logger::WriteMessage("SQLVariant = SQLInterval Day-Second from XML Period");
// 
//       SQLInterval intval("P20578D14H22M8S");
//       SQLVariant one(&intval);
//       CString oneString;
//       one.GetAsString(oneString);
// 
//       Assert::AreEqual("20578 14:22:8",oneString);
//       Assert::AreEqual(SQL_C_INTERVAL_DAY_TO_SECOND,one.GetDataType());
//     }

    TEST_METHOD(TestAssignGuid)
    {
      Logger::WriteMessage("SQLVariant = GUID");


      SQLGUID guid;
      guid.Data1 = 0xAABBCCDD;
      guid.Data2 = 0x0899;
      guid.Data3 = 0x6677;
      guid.Data4[0] = 0x11;
      guid.Data4[1] = 0x22;
      guid.Data4[2] = 0x03;
      guid.Data4[3] = 0x04;
      guid.Data4[4] = 0x55;
      guid.Data4[5] = 0x66;
      guid.Data4[6] = 0x77;
      guid.Data4[7] = 0x88;
      SQLVariant one(&guid);
      CString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual("{AABBCCDD-0899-6677-1122-030455667788}",oneString);
      Assert::AreEqual((int)SQL_C_GUID,(int)one.GetDataType());

//       SQLVariant two;
//       CString guidString("{AABBCCDD-0899-6677-1122-030455667788}");
//       two.SetData(SQL_C_GUID,guidString);
//       two.GetAsString(oneString);
// 
//       Assert::AreEqual("{AABBCCDD-0899-6677-1122-030455667788}",oneString);
//       Assert::AreEqual((int)SQL_C_GUID,(int)two.GetDataType());
    }
  };
}

