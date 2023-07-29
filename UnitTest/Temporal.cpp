////////////////////////////////////////////////////////////////////////
//
// File: Temporal.cpp
//
// Copyright (c) 1998-2022 ir. W.E. Huisman
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
#include "SQLDate.h"
#include "UnitTest.h"

namespace DatabaseUnitTest
{
  TEST_CLASS(TemporalTest)
  {
    TEST_METHOD(WeekdayNames)
    {
      Logger::WriteMessage("Unit testing names of the days of the week");

      InitSQLComponents();

      SQLDate birth("15-10-1959");

      XString in_english = birth.WeekDayName();
      XString in_dutch   = birth.WeekDayName(LN_DUTCH);
      XString in_german  = birth.WeekDayName(LN_GERMAN);
      XString in_french  = birth.WeekDayName(LN_FRENCH);

      Logger::WriteMessage("English: " + in_english);
      Logger::WriteMessage("Dutch  : " + in_dutch);
      Logger::WriteMessage("German : " + in_german);
      Logger::WriteMessage("French : " + in_french);

      Assert::AreEqual("thursday",  in_english);
      Assert::AreEqual("donderdag", in_dutch);
      Assert::AreEqual("Donnerstag",in_german);
      Assert::AreEqual("jeudi",     in_french);
      number_of_tests += 4;
    }

    TEST_METHOD(MonthNames)
    {
      Logger::WriteMessage("Unit testing names of the months for the year");

      SetDefaultSQLLanguage(LN_ENGLISH);
      SQLDate birth("15-10-1959");

      XString in_english = birth.MonthName();
      XString in_dutch   = birth.MonthName(LN_DUTCH);
      XString in_german  = birth.MonthName(LN_GERMAN);
      XString in_french  = birth.MonthName(LN_FRENCH);

      Logger::WriteMessage("English: " + in_english);
      Logger::WriteMessage("Dutch  : " + in_dutch);
      Logger::WriteMessage("German : " + in_german);
      Logger::WriteMessage("French : " + in_french);

      Assert::AreEqual("october",in_english);
      Assert::AreEqual("oktober",in_dutch);
      Assert::AreEqual("Oktober",in_german);
      Assert::AreEqual("octobre",in_french);

      number_of_tests += 4;
    }

    TEST_METHOD(FullDate)
    {
      Logger::WriteMessage("Unit testing full date string");

      InitSQLComponents();

      SQLDate birth("15-10-1959");

      XString in_english = birth.FullDate();
      XString in_dutch   = birth.FullDate(LN_DUTCH);
      XString in_german  = birth.FullDate(LN_GERMAN);
      XString in_french  = birth.FullDate(LN_FRENCH);

      Logger::WriteMessage("English: " + in_english);
      Logger::WriteMessage("Dutch  : " + in_dutch);
      Logger::WriteMessage("German : " + in_german);
      Logger::WriteMessage("French : " + in_french);

      Assert::AreEqual("thursday 15 october 1959",  in_english);
      Assert::AreEqual("donderdag 15 oktober 1959", in_dutch);
      Assert::AreEqual("Donnerstag 15 Oktober 1959",in_german);
      Assert::AreEqual("jeudi 15 octobre 1959",     in_french);

      number_of_tests += 4;
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

      number_of_tests += 6;
    }

    TEST_METHOD(DateCalculation)
    {
      Logger::WriteMessage("Unit testing attributes of an SQLDate");

      InitSQLComponents();

      SQLDate birth("15-10-1959");
      birth = birth.AddYears(2);
      birth = birth.AddMonths(2);
      birth = birth.AddDays(2);

      XString result = birth.AsString();
      Logger::WriteMessage("Added 2-2-2 to day of birth: " + result);
      Assert::AreEqual("17-12-1961",result);

      SQLDate other(17,12,1961);
      birth.SetDate("15101959");
      SQLInterval val = other - birth;
      XString diff = val.AsString();

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

      number_of_tests += 10;
    }

    TEST_METHOD(DateSubtraction)
    {
      Logger::WriteMessage("Date subtraction tests");
      InitSQLComponents();

      int difference = 0;
      SQLDate dayInThePast("2003-10-09 12:11:33");
      SQLDate today("20-08-2019");
      SQLInterval diff = today - dayInThePast;
      difference = diff.GetDays();

      if(dayInThePast.Valid() == false)
      {
        SQLTimestamp inThePast("2003-10-09 12:11:33");
        if(inThePast.Valid())
        {
          SQLTimestamp now("2019-08-20 12:22:33");
          SQLInterval days = now - inThePast;
          difference = days.GetDays();
        }
      }

      XString result;
      result.Format("Difference in days : %d",difference);
      Logger::WriteMessage(result);
      Assert::AreEqual(5794,difference);

      ++number_of_tests;
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
      XString full = today.FullDate();
      // Strip day of the week name
      int pos = full.Find(' ');
      if(pos > 0)
      {
        full = full.Mid(pos + 1);
      }
      SQLDate todayFull(full);

      // Test for short strings
      XString date4,date6;
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
      number_of_tests += 12;

      // Try a wrong date
      try
      {
        XString wrong("Not a date");
        SQLDate testing(wrong);

        Assert::Fail(L"Should not come here");
      }
      catch (StdException& er)
      {
        XString message = er.GetErrorMessage();
        int pos = message.Find("wrong");
        Assert::IsTrue(pos > 0);
      }
      number_of_tests++;
    }

    TEST_METHOD(XMLDate)
    {
      Logger::WriteMessage("Unit testing recognition of XML dates");

      InitSQLComponents();

      SQLDate day("1959-10-15");
      XString display = day.AsString();
      XString expect  = "15-10-1959";
      Assert::AreEqual(expect.GetString(),display.GetString());
    }

    TEST_METHOD(XMLDuration)
    {
      Logger::WriteMessage("Unit testing producing XML_Duration periods");

      InitSQLComponents();

      // Interval to Period (Day-second)
      SQLInterval intval1(SQL_IS_DAY_TO_SECOND,XString("10 18:11:44"));
      XString display = intval1.AsXMLDuration();
      XString expect = "P10DT18H11M44S";
      Assert::AreEqual(expect.GetString(), display.GetString());

      // Interval to Period (Year-Month)
      SQLInterval intval2(SQL_IS_YEAR_TO_MONTH, XString("12 7"));
      display = intval2.AsXMLDuration();
      expect = "P12Y7M";
      Assert::AreEqual(expect.GetString(), display.GetString());

      // Period to interval Duration to Day-second interval
      SQLInterval intval3(XString("P3DT6H7M12S"));
      Assert::AreEqual((int)0,     intval3.GetYears());
      Assert::AreEqual((int)0,     intval3.GetMonths());
      Assert::AreEqual((int)3,     intval3.GetDays());
      Assert::AreEqual((int)78,    intval3.GetHours());
      Assert::AreEqual((int)4687,  intval3.GetMinutes());
      Assert::AreEqual((int)281232,intval3.GetSeconds());

      display = intval3.AsString();
      expect  = "3 6:7:12";
      Assert::AreEqual(expect.GetString(), display.GetString());
    }

    TEST_METHOD(StringToTimestamp)
    {
      Logger::WriteMessage("Unit testing string names to timestamp");

      InitSQLComponents();

      try
      {
        XString now   ("NOW");
  //    XString name  ("NOW - 1000000 MIN");     // Works OK
        XString name  ("NOW - 1000000 MINUTE");  // Works OK
        XString plural("NOW - 1000000 MINUTES"); // Works OK
        SQLTimestamp stamp1(now);
        SQLTimestamp stamp2(name);
        SQLTimestamp stamp3(plural);
        Logger::WriteMessage(stamp1.AsString());
        Logger::WriteMessage(stamp2.AsString());
        Logger::WriteMessage(stamp3.AsString());

        XString years("NOW - 2 YEAR");
        SQLTimestamp backthen(years);
        Logger::WriteMessage(backthen.AsString());

        Assert::IsFalse(stamp1.IsNull());
        Assert::IsFalse(stamp2.IsNull());
        Assert::IsFalse(stamp3.IsNull());
        Assert::IsFalse(backthen.IsNull());

        // Check that we went back in time
        // Test can fail on the brink of a whole second!
        Assert::IsTrue(stamp2 < stamp1);
        Assert::IsTrue(stamp3 < stamp1);
      }
      catch(StdException& ex)
      {
        Logger::WriteMessage("ERROR: " + ex.GetErrorMessage());
        Assert::Fail();
      }
    }

    TEST_METHOD(Timestamp_Local_UTC_TZ)
    {
      Logger::WriteMessage("Timestamps in ISO8601");

      InitSQLComponents();

      try
      {
        SQLTimestamp  stamp("28-02-2022 13:12:11.678");
        CString expect     ("28-02-2022 13:12:11");
        CString expectLocal("2022-02-28T13:12:11.67800");
        CString expectUTC  ("2022-02-28T1:12:11.67800Z");
        CString expectTZ   ("2022-02-28T13:12:11.68-02:00");

        CString stampString = stamp.AsString();
        Assert::AreEqual(expect.GetString(),stampString.GetString());

        CString stampLocal = stamp.AsXMLString(5);
        Assert::AreEqual(expectLocal.GetString(),stampLocal.GetString());

//         // BEWARE: This test only succeeds in WEST-EUROPE during the summertime period!
//         CString stampUTC = stamp.AsXMLStringUTC(5);
//         Assert::AreEqual(expectUTC.GetString(),stampUTC.GetString());
// 
//         // BEWARE: This test only succeeds in WEST-EUROPE during the summertime period!
//         // Also tests the rounding of the fraction!! must be "0.678" -> "0.68" !!
//         CString stampTZ = stamp.AsXMLStringTZ(2);
//         Assert::AreEqual(expectTZ.GetString(),stampTZ.GetString());
      }
      catch (StdException& ex)
      {
        Logger::WriteMessage("ERROR: " + ex.GetErrorMessage());
        Assert::Fail();
      }
    }

    TEST_METHOD(WeekdayNamesTimestamp)
    {
      Logger::WriteMessage("Unit testing names of the days of the week");

      InitSQLComponents();

      SQLTimestamp birth("15-10-1959 15:40:00");

      XString in_english = birth.WeekDayName();
      XString in_dutch   = birth.WeekDayName(LN_DUTCH);
      XString in_german  = birth.WeekDayName(LN_GERMAN);
      XString in_french  = birth.WeekDayName(LN_FRENCH);

      Logger::WriteMessage("English: " + in_english);
      Logger::WriteMessage("Dutch  : " + in_dutch);
      Logger::WriteMessage("German : " + in_german);
      Logger::WriteMessage("French : " + in_french);

      Assert::AreEqual("thursday",in_english);
      Assert::AreEqual("donderdag",in_dutch);
      Assert::AreEqual("Donnerstag",in_german);
      Assert::AreEqual("jeudi",in_french);
      number_of_tests += 4;
    }

    TEST_METHOD(MonthNamesTimestamp)
    {
      Logger::WriteMessage("Unit testing names of the months for the year");
      InitSQLComponents();

      SetDefaultSQLLanguage(LN_ENGLISH);
      SQLTimestamp birth("1959-10-15 15:40:00");

      XString in_english = birth.MonthName();
      XString in_dutch   = birth.MonthName(LN_DUTCH);
      XString in_german  = birth.MonthName(LN_GERMAN);
      XString in_french  = birth.MonthName(LN_FRENCH);

      Logger::WriteMessage("English: " + in_english);
      Logger::WriteMessage("Dutch  : " + in_dutch);
      Logger::WriteMessage("German : " + in_german);
      Logger::WriteMessage("French : " + in_french);

      Assert::AreEqual("october",in_english);
      Assert::AreEqual("oktober",in_dutch);
      Assert::AreEqual("Oktober",in_german);
      Assert::AreEqual("octobre",in_french);

      number_of_tests += 4;
    }

  };
}
