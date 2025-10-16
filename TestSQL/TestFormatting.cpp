////////////////////////////////////////////////////////////////////////
//
// File: TestFormatting.cpp
//
// Created: 1998-2025 ir. W.E. Huisman
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
#include "StdAfx.h"
#include "TestSQL.h"
#include "SQLDatabase.h"
#include "SQLTimestamp.h"
#include "SQLDate.h"
#include "SQLVariant.h"

void
TestXMLStrings()
{
  _tprintf(_T("Testing parsing of XML Strings:\n"));
  _tprintf(_T("===============================\n"));

  SQLTimestamp stamp1(_T("15-10-1959 16:10:12"));
  SQLTimestamp stamp2(_T("15-10-1959T16:10:12"));
  SQLTimestamp stamp3(_T("1959-10-15T16:10:12"));

  _tprintf(_T("Normal  timestamp: %s\n"),stamp1.AsString().GetString());
  _tprintf(_T("Wrong   timestamp: %s\n"),stamp2.AsString().GetString());
  _tprintf(_T("XML     timestamp: %s\n"),stamp3.AsString().GetString());

  SQLTimestamp stamp4(_T("1959-10-15T16:10:12Z"));
  SQLTimestamp stamp5(_T("1959-10-15T16:10:12.1234Z"));
  SQLTimestamp stamp6(_T("1959-10-15T16:10:12+01:20"));
  SQLTimestamp stamp7(_T("1959-10-15T16:10:12.1234+01:20"));

  _tprintf(_T("XML UTC timestamp: %s\n"),stamp4.AsString().GetString());
  _tprintf(_T("XML UTC timestamp: %s\n"),stamp5.AsString().GetString());
  _tprintf(_T("XML AT  timestamp: %s\n"),stamp6.AsString().GetString());
  _tprintf(_T("XML AT  timestamp: %s\n"),stamp7.AsString().GetString());

  _tprintf(_T("As UTC XML string: %s\n"),stamp1.AsXMLStringUTC().GetString());
}

void
TestXMLPeriods()
{
  _tprintf(_T("Testing XML duriation period to SQL Interval\n"));
  _tprintf(_T("============================================\n"));

  XString period(_T("P20578DT14H22M8S"));

  SQLInterval intval(period);
  SQLVariant one(&intval);
  XString oneString;
  one.GetAsString(oneString);

  _tprintf(_T("XML Duration period is: %s\n"),period.GetString());
  _tprintf(_T("Converted SQLInterval : %s\n"),oneString.GetString());
  _tprintf(_T("Expected  SQLInterval : 20578 14:22:08\n"));
}

void
TestGUIDs()
{
  _tprintf(_T("Testing SQLGUID strings in SQLVariants\n"));
  _tprintf(_T("======================================\n"));

  XString guidString = _T("{AABBCCDD-0899-6677-1122-030455667788}");
  SQLGUID guid;
  guid.Data1 = 0xAABBCCDD;
  guid.Data2 = 0x0899;    // Check that this 0 comes through
  guid.Data3 = 0x6677;
  guid.Data4[0] = 0x11;
  guid.Data4[1] = 0x22;
  guid.Data4[2] = 0x03;   // Check that this 0 comes through
  guid.Data4[3] = 0x04;   // Check that this 0 comes through
  guid.Data4[4] = 0x55;
  guid.Data4[5] = 0x66;
  guid.Data4[6] = 0x77;
  guid.Data4[7] = 0x88;

  // Testing SQLGUID to SQLVariant
  SQLVariant one(&guid);
  XString oneString;
  one.GetAsString(oneString);

  _tprintf(_T("String to be converted to GUID : %s\n"),guidString.GetString());
  _tprintf(_T("SQLVariant GUID from SQLGUID   : %s\n"),oneString.GetString());

  if(guidString.CompareNoCase(oneString))
  {
    _tprintf(_T("ERROR: Conversion SQLGUID -> SQLVariant\n"));
  }

  // Testing string to SQLVariant
  SQLVariant two;
  two.SetData(SQL_C_GUID,guidString);
  two.GetAsString(oneString);

  _tprintf(_T("SQLVariant GUID from XString   : %s\n"),oneString.GetString());
  if(guidString.CompareNoCase(oneString))
  {
    _tprintf(_T("ERROR: Conversion XString -> SQLVariant\n"));
  }
}


void
TestSQLFormatting()
{
  _tprintf(_T("Testing Formatting of SQL Strings:\n"));
  _tprintf(_T("==================================\n"));

  SQLDatabase dbs;

  try
  {
    // Set options for the database
    dbs.SetLoginTimeout(0);
    // dbs.SetMARS(false);
    // dbs.AddConnectOption("FBS","600000");

    if(dbs.Open(g_dsn,g_user,g_password))
    {
      _tprintf(_T("Database opened for string testing.\n"));

      SQLTimestamp stamp1(_T("15-10-1959 16:10:12"));
      _tprintf(_T("Print stamp as     string: \"%s\"\n"),stamp1.AsString().GetString());
      _tprintf(_T("Print stamp as SQL string: \"%s\"\n"),stamp1.AsSQLString(&dbs).GetString());
      SQLDate date1(_T("151059"));
      _tprintf(_T("Print date  as     string: \"%s\"\n"),date1.AsString().GetString());
      _tprintf(_T("Print date  as SQL string: \"%s\"\n"),date1.AsSQLString(&dbs).GetString());
      SQLTime time1(_T("16:10:12"));
      _tprintf(_T("Print time  as     string: \"%s\"\n"),time1.AsString().GetString());
      _tprintf(_T("Print time  as SQL string: \"%s\"\n"),time1.AsSQLString(&dbs).GetString());
    }
    else
    {
      _tprintf(_T("Database ***NOT*** opened.\n"));
    }
    dbs.Close();
  }
  catch(StdException& er)
  {
    _tprintf(_T("Database ***NOT*** opened. Reason:\n%s\n"),er.GetErrorMessage().GetString());
  }
}

void
TestingNames()
{
  _tprintf(_T("Testing names in languages:\n"));
  _tprintf(_T("===========================\n"));

  SQLDate date(_T("15-10-1959"));

  _tprintf(_T("Weekday default language: %s\n"),date.WeekDayName().GetString());
  _tprintf(_T("Month   default language: %s\n"),date.MonthName().GetString());

  // Changing default language
  g_defaultLanguage = LN_DUTCH;

  _tprintf(_T("Weekday default language: %s\n"),date.WeekDayName().GetString());
  _tprintf(_T("Month   default language: %s\n"),date.MonthName().GetString());

  // Explicit language
  g_defaultLanguage = LN_ENGLISH;
  _tprintf(_T("Weekday default language: %s\n"),date.WeekDayName(LN_DUTCH).GetString());
  _tprintf(_T("Month   default language: %s\n"),date.MonthName  (LN_DUTCH).GetString());

  _tprintf(_T("Fulldate: %s\n"),date.FullDate(LN_DUTCH).GetString());
  _tprintf(_T("Fulldate: %s\n"),date.FullDate(LN_FRENCH).GetString());
}

void TestMJD()
{
  SQLDate date1(_T("15-10-1959"));// ("17-11-1858");

  _tprintf(_T("Test construction from string\n"));
  _tprintf(_T("The date1  is : %s\n"), date1.AsString().GetString());
  _tprintf(_T("The value1 is : %ld\n"),date1.AsNumber());

  SQLDate date2(date1.AsNumber());

  _tprintf(_T("Test construction from number\n"));
  _tprintf(_T("The date2  is : %s\n"), date2.AsString().GetString());
  _tprintf(_T("The value2 is : %ld\n"),date2.AsNumber());

  SQLDate date3(_T(""));
  _tprintf(_T("Test NULL Date\n"));
  _tprintf(_T("The date3  is : %s\n"), date3.AsString().GetString());
  _tprintf(_T("The value3 is : %ld\n"),date3.AsNumber());

  SQLTimestamp stamp1(_T(""));
  _tprintf(_T("Test NULL Timestamp\n"));
  _tprintf(_T("The stamp1 is : %s\n"), stamp1.AsString().GetString());
  _tprintf(_T("The value1 is : %ld\n"),(long)stamp1.Value());

  SQLTimestamp past = SQLTimestamp::FarInThePast();
  _tprintf(_T("Far in the past is  : %s\n"),past.AsString().GetString());

  SQLTimestamp future = SQLTimestamp::FarInTheFuture();
  _tprintf(_T("Far in the future is: %s\n"),future.AsString().GetString());

  SQLTimestamp now = SQLTimestamp::CurrentTimestamp(true);
  _tprintf(_T("Current timestmap   : %s\n"),now.AsString(9).GetString());
}

void
TestNamedDates()
{
  _tprintf(_T("Testing names in dates:\n"));
  _tprintf(_T("=======================\n"));
  
  g_defaultLanguage = LN_DUTCH;
 
  XString string1(_T("20 jan 2001"));    SQLDate date1(string1);
  XString string2(_T("jan 5 01"));       SQLDate date2(string2);
  XString string3(_T("16 februari"));    SQLDate date3(string3);
  XString string4(_T("18 maart 2017"));  SQLDate date4(string4);
  XString string5(_T("september 16"));   SQLDate date5(string5);
  XString string6(_T("5-11"));           SQLDate date6(string6);
  
  _tprintf(_T("Input [%-20s] Datum: %s\n"),string1.GetString(),date1.AsString().GetString());
  _tprintf(_T("Input [%-20s] Datum: %s\n"),string2.GetString(),date2.AsString().GetString());
  _tprintf(_T("Input [%-20s] Datum: %s\n"),string3.GetString(),date3.AsString().GetString());
  _tprintf(_T("Input [%-20s] Datum: %s\n"),string4.GetString(),date4.AsString().GetString());
  _tprintf(_T("Input [%-20s] Datum: %s\n"),string5.GetString(),date5.AsString().GetString());
  _tprintf(_T("Input [%-20s] Datum: %s\n"),string6.GetString(),date6.AsString().GetString());

  _tprintf(_T("\n"));
  _tprintf(_T("Testing names in timestamps:\n"));
  _tprintf(_T("============================\n"));
  
                                            SQLTimestamp stamp1(string1);
  XString string7(_T("22 mei 1961 11:20:33"));  SQLTimestamp stamp7(string7);
  XString string8(_T("jan 5 01 22:18:01"));     SQLTimestamp stamp8(string8);
  XString string9(_T("16 februari 15:17:45"));  SQLTimestamp stamp9(string9);
  XString strng10(_T("18 maart 2017 23:22"));   SQLTimestamp stmp10(strng10);
  XString strng11(_T("september 16 6:7:12"));   SQLTimestamp stmp11(strng11);
  XString strng12(_T("5-11 3:2:6"));            SQLTimestamp stmp12(strng12);
  
  _tprintf(_T("Input [%-30s] Timestamp: %s\n"),string1.GetString(),stamp1.AsString().GetString());
  _tprintf(_T("Input [%-30s] Timestamp: %s\n"),string7.GetString(),stamp7.AsString().GetString());
  _tprintf(_T("Input [%-30s] Timestamp: %s\n"),string8.GetString(),stamp8.AsString().GetString());
  _tprintf(_T("Input [%-30s] Timestamp: %s\n"),string9.GetString(),stamp9.AsString().GetString());
  _tprintf(_T("Input [%-30s] Timestamp: %s\n"),strng10.GetString(),stmp10.AsString().GetString());
  _tprintf(_T("Input [%-30s] Timestamp: %s\n"),strng11.GetString(),stmp11.AsString().GetString());
  _tprintf(_T("Input [%-30s] Timestamp: %s\n"),strng12.GetString(),stmp12.AsString().GetString());
}

void
TestXMLDurations()
{
  SQLInterval intval1(_T("-P3Y"));
  SQLInterval intval2(_T("-P5Y7M"));
  SQLInterval intval3(_T("P26M"));
  SQLInterval intval4(_T("P3D"));
  SQLInterval intval5(_T("-PT106H"));
  SQLInterval intval6(_T("-PT309M"));
  SQLInterval intval7(_T("PT33.12S"));
  SQLInterval intval8(_T("P3DT14H"));
  SQLInterval intval9(_T("P7DT14H12M"));
  SQLInterval intval10(_T("P8DT12H26M18S"));
  SQLInterval intval11(_T("PT17H13M08.5S"));
  SQLInterval intval12(_T("PT17H13M"));
  SQLInterval intval13(_T("PT13M08.5S"));

  _tprintf(_T("Interval year             : %s\n"),intval1.AsString().GetString());
  _tprintf(_T("Interval year to month    : %s\n"),intval2.AsString().GetString());
  _tprintf(_T("Interval month            : %s\n"),intval3.AsString().GetString());
  _tprintf(_T("Interval day              : %s\n"),intval4.AsString().GetString());
  _tprintf(_T("Interval hour             : %s\n"),intval5.AsString().GetString());
  _tprintf(_T("Interval minute           : %s\n"),intval6.AsString().GetString());
  _tprintf(_T("Interval second           : %s\n"),intval7.AsString(true).GetString());
  _tprintf(_T("Interval day to hour      : %s\n"),intval8.AsString().GetString());
  _tprintf(_T("Interval day to minute    : %s\n"),intval9.AsString().GetString());
  _tprintf(_T("Interval day to second    : %s\n"),intval10.AsString().GetString());
  _tprintf(_T("Interval hour to second   : %s\n"),intval11.AsString(true).GetString());
  _tprintf(_T("Interval hour to minute   : %s\n"),intval12.AsString().GetString());
  _tprintf(_T("Interval minute to second : %s\n"),intval13.AsString(true).GetString());
}

void
TestFormatting()
{
  TestXMLStrings();
  TestXMLPeriods();
  TestGUIDs();
  TestingNames();
  TestSQLFormatting();
  TestMJD();
  TestNamedDates();
  TestXMLDurations();
}
