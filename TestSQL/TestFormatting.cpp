#include "StdAfx.h"
#include "TestSQL.h"
#include "SQLDatabase.h"
#include "SQLTimestamp.h"
#include "SQLDate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning (disable: 4477)

void
TestXMLStrings()
{
  printf("Testing parsing of XML Strings:\n");
  printf("===============================\n");

  SQLTimestamp stamp1("15-10-1959 16:10:12");
  SQLTimestamp stamp2("15-10-1959T16:10:12");
  SQLTimestamp stamp3("1959-10-15T16:10:12");

  printf("Normal  timestamp: %s\n",stamp1.AsString());
  printf("Wrong   timestamp: %s\n",stamp2.AsString());
  printf("XML     timestamp: %s\n",stamp3.AsString());

  SQLTimestamp stamp4("1959-10-15T16:10:12Z");
  SQLTimestamp stamp5("1959-10-15T16:10:12.1234Z");
  SQLTimestamp stamp6("1959-10-15T16:10:12+01:20");
  SQLTimestamp stamp7("1959-10-15T16:10:12.1234+01:20");

  printf("XML UTC timestamp: %s\n",stamp4.AsString());
  printf("XML UTC timestamp: %s\n",stamp5.AsString());
  printf("XML AT  timestamp: %s\n",stamp6.AsString());
  printf("XML AT  timestamp: %s\n",stamp7.AsString());

  printf("As UTC XML string: %s\n",stamp1.AsXMLStringUTC());
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
      printf("Print stamp as     string: \"%s\"\n",stamp1.AsString());
      printf("Print stamp as SQL string: \"%s\"\n",stamp1.AsSQLString(&dbs));
      SQLDate date1("151059");
      printf("Print date  as     string: \"%s\"\n",date1.AsString());
      printf("Print date  as SQL string: \"%s\"\n",date1.AsSQLString(&dbs));
      SQLTime time1("16:10:12");
      printf("Print time  as     string: \"%s\"\n",time1.AsString());
      printf("Print time  as SQL string: \"%s\"\n",time1.AsSQLString(&dbs));
    }
    else
    {
      printf("Database ***NOT*** opened.\n");
    }
    dbs.Close();
  }
  catch(CString& s)
  {
    printf("Database ***NOT*** opened. Reason:\n%s\n",s);
  }
  catch(...)
  {
    printf("Database ***NOT*** opened for unknown reasons\n");
  }
}

void
TestingNames()
{
  printf("Testing names in languages:\n");
  printf("===========================\n");

  SQLDate date("15-10-1959");

  printf("Weekday default language: %s\n",date.WeekDayName());
  printf("Month   default language: %s\n",date.MonthName());

  // Changing default language
  g_defaultLanguage = LN_DUTCH;

  printf("Weekday default language: %s\n",date.WeekDayName());
  printf("Month   default language: %s\n",date.MonthName());

  // Explicit language
  g_defaultLanguage = LN_ENGLISH;
  printf("Weekday default language: %s\n",date.WeekDayName(LN_DUTCH));
  printf("Month   default language: %s\n",date.MonthName  (LN_DUTCH));

  printf("Fulldate: %s\n",date.FullDate(LN_DUTCH));
  printf("Fulldate: %s\n",date.FullDate(LN_FRENCH));
}

void TestMJD()
{
  SQLDate date1("15-10-1959");// ("17-11-1858");

  printf("Test construction from string\n");
  printf("The date1  is : %s\n", date1.AsString());
  printf("The value1 is : %ld\n",date1.AsNumber());

  SQLDate date2(date1.AsNumber());

  printf("Test construction from number\n");
  printf("The date2  is : %s\n", date2.AsString());
  printf("The value2 is : %ld\n",date2.AsNumber());

  SQLDate date3("");
  printf("Test NULL Date\n");
  printf("The date3  is : %s\n", date3.AsString());
  printf("The value3 is : %ld\n",date3.AsNumber());

  SQLTimestamp stamp1("");
  printf("Test NULL Timestamp\n");
  printf("The stamp1 is : %s\n", stamp1.AsString());
  printf("The value1 is : %ld\n",stamp1.Value());

  SQLTimestamp past = SQLTimestamp::FarInThePast();
  printf("Far in the past is  : %s\n",past.AsString());

  SQLTimestamp future = SQLTimestamp::FarInTheFuture();
  printf("Far in the future is: %s\n",future.AsString());

  SQLTimestamp now = SQLTimestamp::CurrentTimestamp(true);
  printf("Current timestmap   : %s\n",now.AsString(9));
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
  
  printf("Input [%-20s] Datum: %s\n",string1,date1.AsString());
  printf("Input [%-20s] Datum: %s\n",string2,date2.AsString());
  printf("Input [%-20s] Datum: %s\n",string3,date3.AsString());
  printf("Input [%-20s] Datum: %s\n",string4,date4.AsString());
  printf("Input [%-20s] Datum: %s\n",string5,date5.AsString());
  printf("Input [%-20s] Datum: %s\n",string6,date6.AsString());

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
  
  printf("Input [%-30s] Timestamp: %s\n",string1,stamp1.AsString());
  printf("Input [%-30s] Timestamp: %s\n",string7,stamp7.AsString());
  printf("Input [%-30s] Timestamp: %s\n",string8,stamp8.AsString());
  printf("Input [%-30s] Timestamp: %s\n",string9,stamp9.AsString());
  printf("Input [%-30s] Timestamp: %s\n",strng10,stmp10.AsString());
  printf("Input [%-30s] Timestamp: %s\n",strng11,stmp11.AsString());
  printf("Input [%-30s] Timestamp: %s\n",strng12,stmp12.AsString());
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

  printf("Interval year             : %s\n",intval1.AsString());
  printf("Interval year to month    : %s\n",intval2.AsString());
  printf("Interval month            : %s\n",intval3.AsString());
  printf("Interval day              : %s\n",intval4.AsString());
  printf("Interval hour             : %s\n",intval5.AsString());
  printf("Interval minute           : %s\n",intval6.AsString());
  printf("Interval second           : %s\n",intval7.AsString(true));
  printf("Interval day to hour      : %s\n",intval8.AsString());
  printf("Interval day to minute    : %s\n",intval9.AsString());
  printf("Interval day to second    : %s\n",intval10.AsString());
  printf("Interval hour to second   : %s\n",intval11.AsString(true));
  printf("Interval hour to minute   : %s\n",intval12.AsString());
  printf("Interval minute to second : %s\n",intval13.AsString(true));
}

void
TestFormatting()
{
  TestXMLStrings();
  TestingNames();
  TestSQLFormatting();
  TestMJD();
  TestNamedDates();
  TestXMLDurations();
}
