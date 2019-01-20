////////////////////////////////////////////////////////////////////////
//
// File: TestFormatting.cpp
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
// Last Revision:   20-01-2019
// Version number:  1.5.4
//
#include "StdAfx.h"
#include "TestSQL.h"
#include "SQLDatabase.h"
#include "SQLTimestamp.h"
#include "SQLDate.h"
#include "SQLVariant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void
TestXMLStrings()
{
  printf("Testing parsing of XML Strings:\n");
  printf("===============================\n");

  SQLTimestamp stamp1("15-10-1959 16:10:12");
  SQLTimestamp stamp2("15-10-1959T16:10:12");
  SQLTimestamp stamp3("1959-10-15T16:10:12");

  printf("Normal  timestamp: %s\n",stamp1.AsString().GetString());
  printf("Wrong   timestamp: %s\n",stamp2.AsString().GetString());
  printf("XML     timestamp: %s\n",stamp3.AsString().GetString());

  SQLTimestamp stamp4("1959-10-15T16:10:12Z");
  SQLTimestamp stamp5("1959-10-15T16:10:12.1234Z");
  SQLTimestamp stamp6("1959-10-15T16:10:12+01:20");
  SQLTimestamp stamp7("1959-10-15T16:10:12.1234+01:20");

  printf("XML UTC timestamp: %s\n",stamp4.AsString().GetString());
  printf("XML UTC timestamp: %s\n",stamp5.AsString().GetString());
  printf("XML AT  timestamp: %s\n",stamp6.AsString().GetString());
  printf("XML AT  timestamp: %s\n",stamp7.AsString().GetString());

  printf("As UTC XML string: %s\n",stamp1.AsXMLStringUTC().GetString());
}

void
TestXMLPeriods()
{
  printf("Testing XML duriation period to SQL Interval\n");
  printf("============================================\n");

  CString period("P20578DT14H22M8S");

  SQLInterval intval(period);
  SQLVariant one(&intval);
  CString oneString;
  one.GetAsString(oneString);

  printf("XML Duration period is: %s\n",period.GetString());
  printf("Converted SQLInterval : %s\n",oneString.GetString());
  printf("Expected  SQLInterval : 20578 14:22:08\n");
}

void
TestGUIDs()
{
  printf("Testing SQLGUID strings in SQLVariants\n");
  printf("======================================\n");

  CString guidString = "{AABBCCDD-0899-6677-1122-030455667788}";
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
  CString oneString;
  one.GetAsString(oneString);

  printf("String to be converted to GUID : %s\n",guidString.GetString());
  printf("SQLVariant GUID from SQLGUID   : %s\n",oneString.GetString());

  if(guidString.CompareNoCase(oneString))
  {
    printf("ERROR: Conversion SQLGUID -> SQLVariant\n");
  }

  // Testing string to SQLVariant
  SQLVariant two;
  two.SetData(SQL_C_GUID,guidString);
  two.GetAsString(oneString);

  printf("SQLVariant GUID from CString   : %s\n",oneString.GetString());
  if(guidString.CompareNoCase(oneString))
  {
    printf("ERROR: Conversion CString -> SQLVariant\n");
  }
}


void
TestSQLFormatting()
{
  printf("Testing Formatting of SQL Strings:\n");
  printf("==================================\n");

  SQLDatabase dbs;

  try
  {
    // Set options for the database
    dbs.SetLoginTimeout(0);
    // dbs.SetMARS(false);
    // dbs.AddConnectOption("FBS","600000");

    if(dbs.Open(g_dsn,g_user,g_password))
    {
      printf("Database opened for string testing.\n");

      SQLTimestamp stamp1("15-10-1959 16:10:12");
      printf("Print stamp as     string: \"%s\"\n",stamp1.AsString().GetString());
      printf("Print stamp as SQL string: \"%s\"\n",stamp1.AsSQLString(&dbs).GetString());
      SQLDate date1("151059");
      printf("Print date  as     string: \"%s\"\n",date1.AsString().GetString());
      printf("Print date  as SQL string: \"%s\"\n",date1.AsSQLString(&dbs).GetString());
      SQLTime time1("16:10:12");
      printf("Print time  as     string: \"%s\"\n",time1.AsString().GetString());
      printf("Print time  as SQL string: \"%s\"\n",time1.AsSQLString(&dbs).GetString());
    }
    else
    {
      printf("Database ***NOT*** opened.\n");
    }
    dbs.Close();
  }
  catch(StdException& er)
  {
    printf("Database ***NOT*** opened. Reason:\n%s\n",er.GetErrorMessage().GetString());
  }
}

void
TestingNames()
{
  printf("Testing names in languages:\n");
  printf("===========================\n");

  SQLDate date("15-10-1959");

  printf("Weekday default language: %s\n",date.WeekDayName().GetString());
  printf("Month   default language: %s\n",date.MonthName().GetString());

  // Changing default language
  g_defaultLanguage = LN_DUTCH;

  printf("Weekday default language: %s\n",date.WeekDayName().GetString());
  printf("Month   default language: %s\n",date.MonthName().GetString());

  // Explicit language
  g_defaultLanguage = LN_ENGLISH;
  printf("Weekday default language: %s\n",date.WeekDayName(LN_DUTCH).GetString());
  printf("Month   default language: %s\n",date.MonthName  (LN_DUTCH).GetString());

  printf("Fulldate: %s\n",date.FullDate(LN_DUTCH).GetString());
  printf("Fulldate: %s\n",date.FullDate(LN_FRENCH).GetString());
}

void TestMJD()
{
  SQLDate date1("15-10-1959");// ("17-11-1858");

  printf("Test construction from string\n");
  printf("The date1  is : %s\n", date1.AsString().GetString());
  printf("The value1 is : %ld\n",date1.AsNumber());

  SQLDate date2(date1.AsNumber());

  printf("Test construction from number\n");
  printf("The date2  is : %s\n", date2.AsString().GetString());
  printf("The value2 is : %ld\n",date2.AsNumber());

  SQLDate date3("");
  printf("Test NULL Date\n");
  printf("The date3  is : %s\n", date3.AsString().GetString());
  printf("The value3 is : %ld\n",date3.AsNumber());

  SQLTimestamp stamp1("");
  printf("Test NULL Timestamp\n");
  printf("The stamp1 is : %s\n", stamp1.AsString().GetString());
  printf("The value1 is : %ld\n",(long)stamp1.Value());

  SQLTimestamp past = SQLTimestamp::FarInThePast();
  printf("Far in the past is  : %s\n",past.AsString().GetString());

  SQLTimestamp future = SQLTimestamp::FarInTheFuture();
  printf("Far in the future is: %s\n",future.AsString().GetString());

  SQLTimestamp now = SQLTimestamp::CurrentTimestamp(true);
  printf("Current timestmap   : %s\n",now.AsString(9).GetString());
}

void
TestNamedDates()
{
  printf("Testing names in dates:\n");
  printf("=======================\n");
  
  g_defaultLanguage = LN_DUTCH;
 
  CString string1("20 jan 2001");    SQLDate date1(string1);
  CString string2("jan 5 01");       SQLDate date2(string2);
  CString string3("16 februari");    SQLDate date3(string3);
  CString string4("18 maart 2017");  SQLDate date4(string4);
  CString string5("september 16");   SQLDate date5(string5);
  CString string6("5-11");           SQLDate date6(string6);
  
  printf("Input [%-20s] Datum: %s\n",string1.GetString(),date1.AsString().GetString());
  printf("Input [%-20s] Datum: %s\n",string2.GetString(),date2.AsString().GetString());
  printf("Input [%-20s] Datum: %s\n",string3.GetString(),date3.AsString().GetString());
  printf("Input [%-20s] Datum: %s\n",string4.GetString(),date4.AsString().GetString());
  printf("Input [%-20s] Datum: %s\n",string5.GetString(),date5.AsString().GetString());
  printf("Input [%-20s] Datum: %s\n",string6.GetString(),date6.AsString().GetString());

  printf("\n");
  printf("Testing names in timestamps:\n");
  printf("============================\n");
  
                                            SQLTimestamp stamp1(string1);
  CString string7("22 mei 1961 11:20:33");  SQLTimestamp stamp7(string7);
  CString string8("jan 5 01 22:18:01");     SQLTimestamp stamp8(string8);
  CString string9("16 februari 15:17:45");  SQLTimestamp stamp9(string9);
  CString strng10("18 maart 2017 23:22");   SQLTimestamp stmp10(strng10);
  CString strng11("september 16 6:7:12");   SQLTimestamp stmp11(strng11);
  CString strng12("5-11 3:2:6");            SQLTimestamp stmp12(strng12);
  
  printf("Input [%-30s] Timestamp: %s\n",string1.GetString(),stamp1.AsString().GetString());
  printf("Input [%-30s] Timestamp: %s\n",string7.GetString(),stamp7.AsString().GetString());
  printf("Input [%-30s] Timestamp: %s\n",string8.GetString(),stamp8.AsString().GetString());
  printf("Input [%-30s] Timestamp: %s\n",string9.GetString(),stamp9.AsString().GetString());
  printf("Input [%-30s] Timestamp: %s\n",strng10.GetString(),stmp10.AsString().GetString());
  printf("Input [%-30s] Timestamp: %s\n",strng11.GetString(),stmp11.AsString().GetString());
  printf("Input [%-30s] Timestamp: %s\n",strng12.GetString(),stmp12.AsString().GetString());
}

void
TestXMLDurations()
{
  SQLInterval intval1("-P3Y");
  SQLInterval intval2("-P5Y7M");
  SQLInterval intval3("P26M");
  SQLInterval intval4("P3D");
  SQLInterval intval5("-PT106H");
  SQLInterval intval6("-PT309M");
  SQLInterval intval7("PT33.12S");
  SQLInterval intval8("P3DT14H");
  SQLInterval intval9("P7DT14H12M");
  SQLInterval intval10("P8DT12H26M18S");
  SQLInterval intval11("PT17H13M08.5S");
  SQLInterval intval12("PT17H13M");
  SQLInterval intval13("PT13M08.5S");

  printf("Interval year             : %s\n",intval1.AsString().GetString());
  printf("Interval year to month    : %s\n",intval2.AsString().GetString());
  printf("Interval month            : %s\n",intval3.AsString().GetString());
  printf("Interval day              : %s\n",intval4.AsString().GetString());
  printf("Interval hour             : %s\n",intval5.AsString().GetString());
  printf("Interval minute           : %s\n",intval6.AsString().GetString());
  printf("Interval second           : %s\n",intval7.AsString(true).GetString());
  printf("Interval day to hour      : %s\n",intval8.AsString().GetString());
  printf("Interval day to minute    : %s\n",intval9.AsString().GetString());
  printf("Interval day to second    : %s\n",intval10.AsString().GetString());
  printf("Interval hour to second   : %s\n",intval11.AsString(true).GetString());
  printf("Interval hour to minute   : %s\n",intval12.AsString().GetString());
  printf("Interval minute to second : %s\n",intval13.AsString(true).GetString());
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
