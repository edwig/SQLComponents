////////////////////////////////////////////////////////////////////////
//
// File: Temporal.cpp
//
// Copyright (c) 1998-2017 ir. W.E. Huisman
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
// Last Revision:   08-01-2017
// Version number:  1.4.0
//
#include "stdafx.h"
#include "SQLDate.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SQLComponents;

namespace DatabaseUnitTest
{
  TEST_CLASS(TemporalTest)
  {
    TEST_METHOD(WeekdayNames)
    {
      Logger::WriteMessage("Unit testing names of the days of the week");

      InitSQLComponents();

      SQLDate birth("15-10-1959");

      CString in_english = birth.WeekDayName();
      CString in_dutch   = birth.WeekDayName(LN_DUTCH);
      CString in_german  = birth.WeekDayName(LN_GERMAN);
      CString in_french  = birth.WeekDayName(LN_FRENCH);

      Logger::WriteMessage("English: " + in_english);
      Logger::WriteMessage("Dutch  : " + in_dutch);
      Logger::WriteMessage("German : " + in_german);
      Logger::WriteMessage("French : " + in_french);

      Assert::AreEqual("thursday",  in_english);
      Assert::AreEqual("donderdag", in_dutch);
      Assert::AreEqual("Donnerstag",in_german);
      Assert::AreEqual("jeudi",     in_french);
    }

    TEST_METHOD(MonthNames)
    {
      Logger::WriteMessage("Unit testing names of the months fo the year");

      SetDefaultSQLLanguage(LN_ENGLISH);
      SQLDate birth("15-10-1959");

      CString in_english = birth.MonthName();
      CString in_dutch   = birth.MonthName(LN_DUTCH);
      CString in_german  = birth.MonthName(LN_GERMAN);
      CString in_french  = birth.MonthName(LN_FRENCH);

      Logger::WriteMessage("English: " + in_english);
      Logger::WriteMessage("Dutch  : " + in_dutch);
      Logger::WriteMessage("German : " + in_german);
      Logger::WriteMessage("French : " + in_french);

      Assert::AreEqual("october",in_english);
      Assert::AreEqual("oktober",in_dutch);
      Assert::AreEqual("Oktober",in_german);
      Assert::AreEqual("octobre",in_french);
    }

    TEST_METHOD(FullDate)
    {
      Logger::WriteMessage("Unit testing full date string");

      InitSQLComponents();

      SQLDate birth("15-10-1959");

      CString in_english = birth.FullDate();
      CString in_dutch   = birth.FullDate(LN_DUTCH);
      CString in_german  = birth.FullDate(LN_GERMAN);
      CString in_french  = birth.FullDate(LN_FRENCH);

      Logger::WriteMessage("English: " + in_english);
      Logger::WriteMessage("Dutch  : " + in_dutch);
      Logger::WriteMessage("German : " + in_german);
      Logger::WriteMessage("French : " + in_french);

      Assert::AreEqual("thursday 15 october 1959",  in_english);
      Assert::AreEqual("donderdag 15 oktober 1959", in_dutch);
      Assert::AreEqual("Donnerstag 15 Oktober 1959",in_german);
      Assert::AreEqual("jeudi 15 octobre 1959",     in_french);
    }

    TEST_METHOD(DateAttributes)
    {
      Logger::WriteMessage("Unit testing attributes of an SQLDate");
      Logger::WriteMessage("Today is: " + SQLDate::Today().AsString());

      InitSQLComponents();

      SQLDate birth("15-10-1959");

      Assert::AreEqual(true, birth.Valid());
      Assert::AreEqual(false,birth.IsNull());
      Assert::AreEqual(1959, (int)birth.Year());
      Assert::AreEqual(10,   (int)birth.Month());
      Assert::AreEqual(15,   (int)birth.Day());
      Assert::AreEqual(31,   (int)birth.DaysInMonth());
    }

    TEST_METHOD(DateCalculation)
    {
      Logger::WriteMessage("Unit testing attributes of an SQLDate");

      InitSQLComponents();

      SQLDate birth("15-10-1959");
      birth = birth.AddYears(2);
      birth = birth.AddMonths(2);
      birth = birth.AddDays(2);

      CString result = birth.AsString();
      Logger::WriteMessage("Added 2-2-2 to day of birth: " + result);
      Assert::AreEqual("17-12-1961",result);

      SQLDate other(17,12,1961);
      birth.SetDate("15101959");
      SQLInterval val = other - birth;
      CString diff = val.AsString();

      Logger::WriteMessage("Difference in dates: " + diff);
      Assert::AreEqual("794",diff);

      SQLDate back = other - val;
      Logger::WriteMessage("After interval     : " + back.AsString());
      Assert::AreEqual("15-10-1959",back.AsString());

      // Adding an interval
      SQLDate plusdate = birth + val;

      Assert::AreEqual(true,birth == back);
      Assert::IsTrue(other  > birth);
      Assert::IsTrue(other >= birth);
      Assert::IsTrue(birth >= birth);
      Assert::IsTrue(birth  < other);
      Assert::IsTrue(birth <= other);
      Assert::IsTrue(other == plusdate);
    }

    TEST_METHOD(DateSpecialStrings)
    {
      Logger::WriteMessage("Unit testing special strings in dates");

      InitSQLComponents();

      SQLDate today = SQLDate::Today();
      SQLDate todayString("TODAY");
      SQLDate todayFOM("FOM");  // First of month
      SQLDate todayLOM("LOM");  // Last of month
      SQLDate todayCY("CY");    // Current year
      SQLDate todayLY("LOY");   // Last of year
      SQLDate nextweek("TODAY +1 WEEK");
      SQLDate otherToday("00-00-00");
      
      // Roundtrip for full date
      CString full = today.FullDate();
      // Strip day of the week name
      int pos = full.Find(' ');
      if(pos > 0)
      {
        full = full.Mid(pos + 1);
      }
      SQLDate todayFull(full);

      // Test for short strings
      CString date4,date6;
      date4.Format("%2.2d%2.2d",today.Day(),today.Month());
      date6.Format("%2.2d%2.2d%2.2d",today.Day(),today.Month(),today.Year() - 2000);
      SQLDate today4(date4);
      SQLDate today6(date6);

      // Empty date
      SQLDate empty("");

      Assert::IsTrue(today == todayString);
      Assert::IsTrue(today == otherToday);
      Assert::IsTrue(today == todayFull);
      Assert::IsTrue(today == today4);
      Assert::IsTrue(today == today6);
      Assert::IsTrue(empty.IsNull());
      Assert::IsTrue(today.Year() == todayFOM.Year() && today.Month() == todayFOM.Month());
      Assert::IsTrue(today.Year() == todayLOM.Year() && today.Month() == todayLOM.Month());
      Assert::IsTrue(today.Year() == todayCY.Year() && todayCY.Month() == 1 && todayCY.Day() == 1);
      Assert::IsTrue(today.Year() == todayLY.Year() && todayLY.Month() == 12 && todayLY.Day() == 31);
      Assert::IsTrue(todayFOM.Day() == 1);
      Assert::AreEqual(7,(int)(nextweek.AsNumber() - today.AsNumber()));

      // Try a wrong date
      try
      {
        CString wrong("Not a date");
        SQLDate testing(wrong);

        Assert::Fail(L"Should not come here");
      }
      catch (CString& er)
      {
        int pos = er.Find("wrong");
        Assert::IsTrue(pos > 0);
      }
    }

  };
}