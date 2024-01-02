////////////////////////////////////////////////////////////////////////
//
// File: AssignmentTest.cpp
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
#include "SQLVariant.h"
#include "SQLDate.h"
#include "SQLTime.h"
#include "SQLTimestamp.h"
#include "SQLInterval.h"
#include "bcd.h"
#include "UnitTest.h"

int number_of_tests = 0;

BEGIN_TEST_MODULE_ATTRIBUTE()
  TEST_MODULE_ATTRIBUTE(L"SQLComponents",L"ODBC")
END_TEST_MODULE_ATTRIBUTE()

TEST_MODULE_INITIALIZE(ModuleInitialize)
{
  Logger::WriteMessage("BEGIN OF: SQLComponents");
}

TEST_MODULE_CLEANUP(ModuleCleanup)
{
  XString message;
  message.Format(_T("Number of tests conducted: %d\n"),number_of_tests);
  Logger::WriteMessage("\n\nTOTAL TESTS:\n");
  Logger::WriteMessage(message);

  Logger::WriteMessage("END OF: SQLComponents");
}

namespace OperatorUnitTest
{		
  TEST_CLASS(OperatorAssignmentTest)
  {
  public:
    TEST_METHOD(AssignmentOperators)
    {
      Logger::WriteMessage("In unit test assignment operators");
      InitSQLComponents();

      ++number_of_tests;
    }

    TEST_METHOD(TestAssignChar)
    {
      Logger::WriteMessage("SQLVariant = CONST CHAR *");

      SQLVariant one(_T("23"));     // XTOR const char* -> to SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("23"),oneString);
      Assert::AreEqual(SQL_C_CHAR,one.GetDataType());

      SQLVariant two   = _T("34");    // Assignment const char* -> SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("34"),twoString);
      Assert::AreEqual(SQL_C_CHAR,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignCString)
    {
      Logger::WriteMessage("SQLVariant = XString");

      XString twentythree(_T("23"));
      SQLVariant one(twentythree);  // XTOR XString -> to SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("23"),oneString);
      Assert::AreEqual(SQL_C_CHAR,one.GetDataType());

      SQLVariant two = twentythree; // Assignment XString -> to SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("23"),twoString);
      Assert::AreEqual(SQL_C_CHAR,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignSLong)
    {
      Logger::WriteMessage("SQLVariant = SLONG");

      SQLVariant one(23);  // XTOR signed long -> to SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("23"),oneString);
      Assert::AreEqual(SQL_C_SLONG,one.GetDataType());

      SQLVariant two = 34;  // Assignment signed long -> to SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("34"),twoString);
      Assert::AreEqual(SQL_C_SLONG,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignULong)
    {
      Logger::WriteMessage("SQLVariant = ULONG");

      SQLVariant one((unsigned)23); // Assignment unsigned long -> to SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("23"),oneString);
      Assert::AreEqual(SQL_C_ULONG,one.GetDataType());

      SQLVariant two = (unsigned)34;  // Assignment unsigned long -> to SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("34"),twoString);
      Assert::AreEqual(SQL_C_ULONG,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignSShort)
    {
      Logger::WriteMessage("SQLVariant = SSHORT");

      SQLVariant one((short)23);    // XTOR short -> to SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("23"),oneString);
      Assert::AreEqual(SQL_C_SSHORT,one.GetDataType());

      SQLVariant two = (short)34;  // Assignment short -> to SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("34"),twoString);
      Assert::AreEqual(SQL_C_SSHORT,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignUShort)
    {
      Logger::WriteMessage("SQLVariant = USHORT");

      SQLVariant one((unsigned short)23);   // XTOR unsigned short -> to SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("23"),oneString);
      Assert::AreEqual(SQL_C_USHORT,one.GetDataType());

      SQLVariant two = (unsigned short)34;  // Assignment unsigned short -> to SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("34"),twoString);
      Assert::AreEqual(SQL_C_USHORT,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignSTinyInt)
    {
      Logger::WriteMessage("SQLVariant = STINYINT");

      SQLVariant one((char)23);   // XTOR tinyint -> to SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("23"),oneString);
      Assert::AreEqual(SQL_C_STINYINT,one.GetDataType());

      SQLVariant two = (char)34;  // Assignment tinyint -> to SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("34"),twoString);
      Assert::AreEqual(SQL_C_STINYINT,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignUTinyInt)
    {
      Logger::WriteMessage("SQLVariant = UTINYINT");

      SQLVariant one((unsigned char)23);    // XTOR unsigned tinyint -> to SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("23"),oneString);
      Assert::AreEqual(SQL_C_UTINYINT,one.GetDataType());

      SQLVariant two = (unsigned char)34;  // Assignment unsigned tinyint -> to SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("34"),twoString);
      Assert::AreEqual(SQL_C_UTINYINT,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignFloat)
    {
      Logger::WriteMessage("SQLVariant = FLOAT");

      SQLVariant one((float)23.0);  // XTOR float -> to SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("23"),oneString);
      Assert::AreEqual(SQL_C_FLOAT,one.GetDataType());

      SQLVariant two = (float)34.0;  // Assignment float -> to SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("34"),twoString);
      Assert::AreEqual(SQL_C_FLOAT,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignDouble)
    {
      Logger::WriteMessage("SQLVariant = DOUBLE");

      SQLVariant one((double)23.0); // XTOR double to SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("23"),oneString);
      Assert::AreEqual(SQL_C_DOUBLE,one.GetDataType());

      SQLVariant two = (double)34.0;  // Assignment double -> to SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("34"),twoString);
      Assert::AreEqual(SQL_C_DOUBLE,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignBool)
    {
      Logger::WriteMessage("SQLVariant = BOOL");

      SQLVariant one((bool)true); // XTOR bool -> to SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("1"),oneString);
      Assert::AreEqual(SQL_C_BIT,one.GetDataType());

      SQLVariant two = true;      // Assignment bool -> to SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("1"),twoString);
      Assert::AreEqual(SQL_C_BIT,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignSBigInt)
    {
      Logger::WriteMessage("SQLVariant = SBIGINT");

      SQLVariant one((__int64)23);    // XTOR bigint -> to SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("23"),oneString);
      Assert::AreEqual(SQL_C_SBIGINT,one.GetDataType());

      SQLVariant two = (__int64)34;  // Assignment bigint -> to SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("34"),twoString);
      Assert::AreEqual(SQL_C_SBIGINT,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignUBigInt)
    {
      Logger::WriteMessage("SQLVariant = UBIGINT");

      SQLVariant one((unsigned __int64)23);   // XTOR unsigned bigint -> to SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("23"),oneString);
      Assert::AreEqual(SQL_C_UBIGINT,one.GetDataType());

      SQLVariant two = (unsigned __int64)34;  // Assignment unsigned bigint -> to SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("34"),twoString);
      Assert::AreEqual(SQL_C_UBIGINT,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignDate)
    {
      Logger::WriteMessage("SQLVariant = SQLDate");

      InitSQLComponents();

      SQLDate date(_T("15-10-1959"));
      SQLVariant one(&date);        // XTOR SQLDate -> to SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("15-10-1959"),oneString);
      Assert::AreEqual(SQL_C_DATE,one.GetDataType());

      SQLVariant two = &date;      // Assignment SQLDate -> to SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("15-10-1959"),twoString);
      Assert::AreEqual(SQL_C_DATE,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignDateStruct)
    {
      Logger::WriteMessage("SQLVariant = &DATE_STRUCT");

      DATE_STRUCT date;
      date.day = 15;
      date.month = 10;
      date.year = 1959;
      SQLVariant one(&date);        // XTOR DATE_STRUCT -> SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("15-10-1959"),oneString);
      Assert::AreEqual(SQL_C_DATE,one.GetDataType());

      SQLVariant two = &date;       // Assignment DATE_STRUCT -> SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("15-10-1959"),twoString);
      Assert::AreEqual(SQL_C_DATE,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignTime)
    {
      Logger::WriteMessage("SQLVariant = SQLTime");

      InitSQLComponents();

      SQLTime time(_T("15:55:42"));
      SQLVariant one(&time);        // XTOR DATE_STRUCT -> SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("15:55:42"),oneString);
      Assert::AreEqual(SQL_C_TIME,one.GetDataType());

      SQLVariant two = &time;       // Assignment DATE_STRCUT -> SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("15:55:42"),twoString);
      Assert::AreEqual(SQL_C_TIME,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignTimeStruct)
    {
      Logger::WriteMessage("SQLVariant = &TIME_STRUCT");

      TIME_STRUCT time;
      time.hour = 15;
      time.minute = 55;
      time.second = 42;
      SQLVariant one(&time);        // XTOR TIME_STRUCT -> SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("15:55:42"),oneString);
      Assert::AreEqual(SQL_C_TIME,one.GetDataType());

      SQLVariant two = &time;       // Assignment TIME_STRUCT -> SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("15:55:42"),twoString);
      Assert::AreEqual(SQL_C_TIME,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignTimestamp)
    {
      Logger::WriteMessage("SQLVariant = SQLTimestamp");

      InitSQLComponents();

      SQLTimestamp stamp(_T("1959-10-15 15:55:42"));
      SQLVariant one(&stamp);       // XTOR SQLTimestamp -> SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("1959-10-15 15:55:42"),oneString);
      Assert::AreEqual(SQL_C_TIMESTAMP,one.GetDataType());

      SQLVariant two = &stamp;      // Assignment SQLTimestamp -> SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("1959-10-15 15:55:42"),twoString);
      Assert::AreEqual(SQL_C_TIMESTAMP,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignTimestampStruct)
    {
      Logger::WriteMessage("SQLVariant = &TIMESTAMP_STRUCT");

      TIMESTAMP_STRUCT stamp;
      stamp.year  = 1959;
      stamp.month = 10;
      stamp.day   = 15;
      stamp.hour  = 15;
      stamp.minute   = 55;
      stamp.second   = 42;
      stamp.fraction = 0;
      SQLVariant one(&stamp);       // XTOR TIMESTAMP_STRUCT -> SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("1959-10-15 15:55:42"),oneString);
      Assert::AreEqual(SQL_C_TIMESTAMP,one.GetDataType());

      SQLVariant two = &stamp;      // Assignment TIMESTAMP_STRUCT -> SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("1959-10-15 15:55:42"),twoString);
      Assert::AreEqual(SQL_C_TIMESTAMP,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignIntervalYM)
    {
      Logger::WriteMessage("SQLVariant = SQLInterval Year-Month");

      SQLInterval intval(_T("P57Y2M"));
      SQLVariant one(&intval);      // XTOR SQLInterval -> SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("57-2"),oneString);
      Assert::AreEqual(SQL_C_INTERVAL_YEAR_TO_MONTH,one.GetDataType());

      SQLVariant two = &intval;     // Assignment SQLInterval -> SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("57-2"),twoString);
      Assert::AreEqual(SQL_C_INTERVAL_YEAR_TO_MONTH,two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignIntervalDS_String)
    {
      Logger::WriteMessage("SQLVariant = SQLInterval Day-Second from string");

      SQLInterval intval(SQL_IS_DAY_TO_SECOND,_T("20578 14:22:08"));
      SQLVariant one(&intval);
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("20578 14:22:8"),oneString);
      Assert::AreEqual(SQL_C_INTERVAL_DAY_TO_SECOND,one.GetDataType());

      number_of_tests += 2;
    }

    TEST_METHOD(TestAssignIntervalDS_Values)
    {
      Logger::WriteMessage("SQLVariant = SQLInterval Day-Second from values");

      SQLInterval intval(SQL_IS_DAY_TO_SECOND,20578,14,22,8);
      SQLVariant one(&intval);
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("20578 14:22:8"),oneString);
      Assert::AreEqual(SQL_C_INTERVAL_DAY_TO_SECOND,one.GetDataType());

      number_of_tests += 2;
    }

    TEST_METHOD(TestAssignIntervalDS_Period)
    {
      Logger::WriteMessage("SQLVariant = SQLInterval Day-Second from XML Period");

      SQLInterval intval(_T("P20578DT14H22M8S"));
      SQLVariant one(&intval);
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("20578 14:22:8"),oneString);
      Assert::AreEqual(SQL_C_INTERVAL_DAY_TO_SECOND,one.GetDataType());

      number_of_tests += 2;
    }

    TEST_METHOD(TestAssignGUID)
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
      SQLVariant one(&guid);      // XTOR GUID -> SQLVariant
      XString oneString;
      one.GetAsString(oneString);

      Assert::AreEqual(_T("{AABBCCDD-0899-6677-1122-030455667788}"),oneString);
      Assert::AreEqual((int)SQL_C_GUID,(int)one.GetDataType());

      SQLVariant two = &guid;     // Assignment GUID -> SQLVariant
      XString twoString;
      two.GetAsString(twoString);

      Assert::AreEqual(_T("{AABBCCDD-0899-6677-1122-030455667788}"),twoString);
      Assert::AreEqual((int)SQL_C_GUID,(int)two.GetDataType());

      number_of_tests += 4;
    }

    TEST_METHOD(TestAssignGuidstring)
    {
      Logger::WriteMessage("SQLVariant = guidstring");

      SQLVariant one;
      XString oneString;
      XString guidString(_T("{AABBCCDD-0899-6677-1122-030455667788}"));
      one.SetData(SQL_C_GUID,guidString);   // Assignment SetData!

      const SQLGUID* guid = one.GetAsGUID();

      Assert::AreEqual((int)SQL_C_GUID,one.GetDataType());

      Assert::AreEqual((unsigned long) 0xAABBCCDD,guid->Data1);
      Assert::AreEqual((int)0x0899,          (int)guid->Data2);
      Assert::AreEqual((int)0x6677,          (int)guid->Data3);
      Assert::AreEqual((BYTE)0x11,guid->Data4[0]);
      Assert::AreEqual((BYTE)0x22,guid->Data4[1]);
      Assert::AreEqual((BYTE)0x03,guid->Data4[2]);
      Assert::AreEqual((BYTE)0x04,guid->Data4[3]);
      Assert::AreEqual((BYTE)0x55,guid->Data4[4]);
      Assert::AreEqual((BYTE)0x66,guid->Data4[5]);
      Assert::AreEqual((BYTE)0x77,guid->Data4[6]);
      Assert::AreEqual((BYTE)0x88,guid->Data4[7]);

      number_of_tests += 12;
    }

    TEST_METHOD(TestCastOperators)
    {
      Logger::WriteMessage("Testing the cast operators of SQLVariant");

      SQLVariant varBit   ((bool)true);
      SQLVariant varSTiny ((char)-67);
      SQLVariant varUTiny ((unsigned char)189);
      SQLVariant varSShort((short)-567);
      SQLVariant varUShort((ushort)567);
      SQLVariant varInt   (2 * 42);
      SQLVariant varUInt  ((unsigned)0x84556677);
      SQLVariant varFloat ((float)9.87654E12);
      SQLVariant varDouble((double)1.12345678901234E88);
      SQLVariant varBig   (0x12312341234i64);
      SQLVariant varUBig  ((uint64)0xCC11DD22FF11EE11ui64);

      bool     numberBit    = varBit;
      char     numberSTiny  = varSTiny;
      uchar    numberUTiny  = varUTiny;
      short    numberSShort = varSShort;
      ushort   numberUShort = varUShort;
      int      numberInt    = varInt;
      unsigned numberUInt   = varUInt;
      float    numberFloat  = varFloat;
      double   numberDouble = varDouble;
      int64    numberBig    = varBig;
      uint64   numberUBig   = varUBig;

      Assert::AreEqual((bool)true,numberBit);
      Assert::AreEqual((char)-67, numberSTiny);
      Assert::AreEqual((uchar) 189,numberUTiny);
      Assert::AreEqual((short)-567,numberSShort);
      Assert::AreEqual((int)567,(int)numberUShort);
      Assert::AreEqual((int)84,numberInt);
      Assert::AreEqual((unsigned)0x84556677,numberUInt);
      Assert::AreEqual((float)9.87654E12,numberFloat);
      Assert::AreEqual((double)1.12345678901234E88,numberDouble);

      unsigned high = (unsigned)(numberBig >> 32);
      unsigned low  = (unsigned)(numberBig & 0xFFFFFFFFi64);
      Assert::AreEqual((unsigned)0x123,high);
      Assert::AreEqual((unsigned)0x12341234,low);

      high = (unsigned)(numberUBig >> 32);
      low  = (unsigned)(numberUBig & 0xFFFFFFFFui64);
      Assert::AreEqual((unsigned)0xCC11DD22,high);
      Assert::AreEqual((unsigned)0xFF11EE11,low);

      number_of_tests += 13;
    }
  };
}

