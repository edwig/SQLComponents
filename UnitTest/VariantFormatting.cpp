////////////////////////////////////////////////////////////////////////
//
// File: VariantFormatting.cpp
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
// Last Revision:   28-05-2018
// Version number:  1.5.0
//
#include "stdafx.h"
#include "SQLVariantFormat.h"
#include "SQLTimestamp.h"
#include "SQLTime.h"
#include "SQLDate.h"
#include "UnitTest.h"

namespace DatabaseUnitTest
{
  TEST_CLASS(VariantFormatting)
  {

    TEST_METHOD(InitialWordsAndLines)
    {
      Logger::WriteMessage("Formatting: Initial words");

      var number(1L);
      SQLVariantFormat form(number);
      CString input("THIS is A LONG and TireSome TesT");

      form.SetFormat(input);
      form.StringInitCapital();
      Assert::AreEqual("This Is A Long And Tiresome Test",form.GetFormat());
      number_of_tests++;

      form.StringStartCapital();
      Assert::AreEqual("This is a long and tiresome test",form.GetFormat());
      number_of_tests++;
    }

    TEST_METHOD(IsConstantOrNumber)
    {
      Logger::WriteMessage("Formatting: Is-a-constant-or-a-number (simple form)");

      var number(1L);
      SQLVariantFormat form(number);

      form.SetFormat("123");       Assert::IsTrue(form.IsConstantOrNumber());
      form.SetFormat("123  ");     Assert::IsTrue(form.IsConstantOrNumber());
      form.SetFormat("  123  ");   Assert::IsTrue(form.IsConstantOrNumber());
      form.SetFormat("+123");      Assert::IsTrue(form.IsConstantOrNumber());
      form.SetFormat("-123");      Assert::IsTrue(form.IsConstantOrNumber());
      form.SetFormat("123.456");   Assert::IsTrue(form.IsConstantOrNumber());
      form.SetFormat("+123.456");  Assert::IsTrue(form.IsConstantOrNumber());
      form.SetFormat("-123.456");  Assert::IsTrue(form.IsConstantOrNumber());
      form.SetFormat("123,456");   Assert::IsTrue(form.IsConstantOrNumber(','));

      form.SetFormat("123+12");    Assert::IsFalse(form.IsConstantOrNumber());
      form.SetFormat("-123+12");   Assert::IsFalse(form.IsConstantOrNumber());
      form.SetFormat("-123-12");   Assert::IsFalse(form.IsConstantOrNumber());
      form.SetFormat("+123+12");   Assert::IsFalse(form.IsConstantOrNumber());
      form.SetFormat("+123-12");   Assert::IsFalse(form.IsConstantOrNumber());
      form.SetFormat("123  12");   Assert::IsFalse(form.IsConstantOrNumber());

      number_of_tests += 15;
    }

    TEST_METHOD(Valuta)
    {
      Logger::WriteMessage("Formatting: Removing valuta markers");

      var number(1L);
      SQLVariantFormat form(number);
      CString str;

      str = "EUR. 45";  form.StrValutaNLOmzetten(str,true);  Assert::AreEqual("45",str);
      str = "45 EUR.";  form.StrValutaNLOmzetten(str,true);  Assert::AreEqual("45",str);
      str = "E. 45";    form.StrValutaNLOmzetten(str,true);  Assert::AreEqual("45",str);
      str = "E 45";     form.StrValutaNLOmzetten(str,true);  Assert::AreEqual("45",str);
      str = "45 E.";    form.StrValutaNLOmzetten(str,true);  Assert::AreEqual("45",str);
      str = "45 E";     form.StrValutaNLOmzetten(str,true);  Assert::AreEqual("45",str);

      str = "E. 3.145,12"; form.StrValutaNLOmzetten(str,false); Assert::AreEqual("3145.12",str);

      str = "$. 45";    form.StrValutaAMOmzetten(str,true);  Assert::AreEqual("45",str);
      str = "$ 45";     form.StrValutaAMOmzetten(str,true);  Assert::AreEqual("45",str);
      str = "45 $.";    form.StrValutaAMOmzetten(str,true);  Assert::AreEqual("45",str);
      str = "45 $";     form.StrValutaAMOmzetten(str,true);  Assert::AreEqual("45",str);

      str = "$. 3,145.12"; form.StrValutaAMOmzetten(str,false); Assert::AreEqual("3145,12",str);

      number_of_tests += 12;
    }

    TEST_METHOD(StringDoubleValue)
    {
      Logger::WriteMessage("Formatting: String to double values");

      var number(1L);
      SQLVariantFormat form(number);
      CString str;

      form.SetFormat("1234.5678");      Assert::AreEqual(1234.5678,form.StringDoubleValue());
      form.SetFormat("1234,5678");      Assert::AreEqual(1234.5678,form.StringDoubleValue());
      form.SetFormat("1.234,5678");     Assert::AreEqual(1234.5678,form.StringDoubleValue());
      form.SetFormat("1,234.5678");     Assert::AreEqual(1234.5678,form.StringDoubleValue());
      form.SetFormat("1.2345678E+03");  Assert::AreEqual(1234.5678,form.StringDoubleValue());

      number_of_tests += 5;
    }

    TEST_METHOD(CurrentDate)
    {
      Logger::WriteMessage("Formatting: Set current date");

      var number;
      SQLVariantFormat form(number);
      SQLTimestamp today = SQLTimestamp::CurrentTimestamp();
      form.SetCurrentDate();

      CString format1 = form.GetFormat();
      CString format2 = today.AsXMLString();
      format2.Replace("T"," ");

      Logger::WriteMessage("TODAY1 : " + format1);
      Logger::WriteMessage("TODAY2 : " + format2);

      Assert::IsTrue(format1 == format2);
      number_of_tests++;

      // Try doing it twice
      form.SetCurrentDate();

      // Test resetting the value
      form.ResetValue();
    }

    TEST_METHOD(FormatDate)
    {
      Logger::WriteMessage("Formatting: Formatting the date/time");

      InitSQLComponents();

      CString dateString("15-10-1959");
      SQLDate date(dateString);
      var dateVar(&date);
      SQLVariantFormat form(dateVar);

      form.FormatDate("dddd d MMMM yyyy|H:mm:ss");
      Assert::AreEqual("donderdag 15 oktober 1959",form.GetFormat());
      number_of_tests++;

      form.FormatDate("ddd dd MMM yyyy|H:mm:ss");
      Logger::WriteMessage(form.GetFormat());
      Assert::AreEqual("do 15 okt 1959",form.GetFormat());
      number_of_tests++;

      CString stampString("1959-10-15 15:50:20");
      SQLTimestamp stamp(stampString);
      var stampVar(&stamp);
      SQLVariantFormat st(stampVar);

      st.FormatDate("dddd d MMMM yyyy |H:mm:ss");
      Assert::AreEqual("donderdag 15 oktober 1959 15:50:20",st.GetFormat());
      number_of_tests++;

      st.FormatDate("@");
      Assert::AreEqual("donderdag 15 oktober 1959",st.GetFormat());
      number_of_tests++;

      st.FormatDate("");
      Assert::AreEqual("15-10-1959",st.GetFormat());
      number_of_tests++;

      form.Reset();
      st.ReFormat();
      st.ResetValue();

      // timestamp formatting on a string
      CString  theDate("15-10-1959 15:50:20");
      var date2(theDate);
      SQLVariantFormat fdate(date2);
      // Forcing the string to a timestamp
      fdate.SetFormat(theDate);
      fdate.FormatDate("dddd dd MMMM jjjj |M:mm:ss");
      Assert::AreEqual("donderdag 15 oktober 1959 15:50:20",fdate.GetFormat());
      number_of_tests++;

      // Only for date
      theDate = "15-10-1959";
      var date3(theDate);
      SQLVariantFormat fdate3(date3);
      // Forcing the string to a date
      fdate3.SetFormat(theDate);
      fdate3.FormatDate("dddd dd MMMM jjjj");
      Assert::AreEqual("donderdag 15 oktober 1959",fdate3.GetFormat());
      number_of_tests++;

      // Only for time
      theDate = "15:50:20";
      var date4(theDate);
      SQLVariantFormat fdate4(date4);
      // Forcing to a time
      fdate.SetFormat(theDate);
      fdate.FormatDate("|M:mm:ss");
      Assert::AreEqual("15:50:20",fdate.GetFormat());
      number_of_tests++;
    }

    TEST_METHOD(NumberFormatting)
    {
      Logger::WriteMessage("Formatting: Formatting the date/time");
      var num( 45123.12);
      var neg(-66789.56);
      SQLVariantFormat form(num);
      SQLVariantFormat negf(neg);

      form.FormatNumber("",false);
      Assert::AreEqual("45.123,12",form.GetFormat());
      number_of_tests++;

      form.FormatNumber("#",false);
      Logger::WriteMessage(form.GetFormat());
      Assert::AreEqual("45123",form.GetFormat());
      number_of_tests++;

      form.FormatNumber("###,###.00",false);      
      Assert::AreEqual(" 45.123,12",    form.GetFormat());
      number_of_tests++;

      form.FormatNumber("&&&,&&&.00",false);
      Assert::AreEqual("45.123,12",form.GetFormat());
      number_of_tests++;

      form.FormatNumber("&&&,&&&.x",false);
      Assert::AreEqual("45.123,1",form.GetFormat());
      number_of_tests++;

      form.FormatNumber("&&&,&&&.X",false);
      Assert::AreEqual("45.123",form.GetFormat());
      number_of_tests++;

      form.FormatNumber("#:00",false);
      Assert::AreEqual("12",form.GetFormat());
      number_of_tests++;

      form.FormatNumber("+###,###.00",false);
      Assert::AreEqual ("  45.123,12",form.GetFormat());
      number_of_tests++;
      negf.FormatNumber("+###,###.00",false);
      Assert::AreEqual ("- 66.789,56",negf.GetFormat());
      number_of_tests++;

      form.FormatNumber("-###,###.00",false);
      Assert::AreEqual ("+ 45.123,12",form.GetFormat());
      number_of_tests++;
      negf.FormatNumber("-###,###.00",false);
      Assert::AreEqual ("- 66.789,56",negf.GetFormat());
      number_of_tests++;
      negf.FormatNumber("~###,###.00",false);
      Assert::AreEqual ("- 66.789,56",negf.GetFormat());
      number_of_tests++;
    }

    TEST_METHOD(FormatDateCalculation)
    {
      Logger::WriteMessage("Formatting: date calculations and reformatting");

      InitSQLComponents();

      // ADDITION

      // + n -> interpret as days
      SQLDate date1("2012-12-25");
      var vdat1(&date1);
      SQLVariantFormat fdat1(vdat1);
      fdat1.DateCalculate('+',"9");
      Assert::AreEqual("03-01-2013",fdat1.GetFormat());
      number_of_tests++;

      // + nD -> interpret as days
      SQLDate date2("2012-12-25");
      var vdat2(&date2);
      SQLVariantFormat fdat2(vdat2);
      fdat2.DateCalculate('+',"9d");
      Assert::AreEqual("03-01-2013",fdat2.GetFormat());
      number_of_tests++;

      // + nM -> interpret as months
      SQLDate date3("2012-12-25");
      var vdat3(&date3);
      SQLVariantFormat fdat3(vdat3);
      fdat3.DateCalculate('+',"4m");
      Assert::AreEqual("25-04-2013",fdat3.GetFormat());
      number_of_tests++;

      // + nY -> interpret as years
      SQLDate date4("2012-12-25");
      var vdat4(&date4);
      SQLVariantFormat fdat4(vdat4);
      fdat4.DateCalculate('+',"2Y");
      Assert::AreEqual("25-12-2014",fdat4.GetFormat());
      number_of_tests++;

      // SUBTRACTION 

      // - n -> interpret as days
      SQLDate date5("03-01-2013");
      var vdat5(&date5);
      SQLVariantFormat fdat5(vdat5);
      fdat5.DateCalculate('-',"9");
      Assert::AreEqual("25-12-2012",fdat5.GetFormat());
      number_of_tests++;

      // - nd -> interpret as days
      SQLDate date6("03-01-2013");
      var vdat6(&date6);
      SQLVariantFormat fdat6(vdat6);
      fdat6.DateCalculate('-',"9");
      Assert::AreEqual("25-12-2012",fdat6.GetFormat());
      number_of_tests++;

      // - nM -> interpret as months
      SQLDate date7("25-04-2013");
      var vdat7(&date7);
      SQLVariantFormat fdat7(vdat7);
      fdat7.DateCalculate('-',"4m");
      Assert::AreEqual("25-12-2012",fdat7.GetFormat());
      number_of_tests++;

      // + nY -> interpret as years
      SQLDate date8("2014-12-25");
      var vdat8(&date8);
      SQLVariantFormat fdat8(vdat8);
      fdat8.DateCalculate('-',"2Y");
      Assert::AreEqual("25-12-2012",fdat8.GetFormat());
      number_of_tests++;

      // SUBTRACTION OF DATES

      SQLDate date9("25-12-2012");
      var vdat9(&date9);
      SQLVariantFormat fdat9(vdat9);
      fdat9.DateCalculate('~',"03-01-2013");
      Assert::AreEqual("9",fdat9.GetFormat());
      number_of_tests++;

      SQLDate date10("03-01-2013");
      var vdat10(&date10);
      SQLVariantFormat fdat10(vdat10);
      fdat10.DateCalculate('~',"25-12-2012");
      Assert::AreEqual("-9",fdat10.GetFormat());
      number_of_tests++;

      // From other than a date

      CString date11("25-12-2012");
      var vdat11(date11);
      SQLVariantFormat fdat11(vdat11);
      fdat11.DateCalculate('+',"9d");
      Assert::AreEqual("03-01-2013",fdat11.GetFormat());
      number_of_tests++;

      CString date12("");
      var vdat12(date12);
      SQLVariantFormat fdat12(vdat12);
      fdat12.SetFormat("25-12-2012");
      fdat12.DateCalculate('+',"9d");
      Assert::AreEqual("03-01-2013",fdat12.GetFormat());
      number_of_tests++;

      // Default operator is a '+'
      SQLDate date13("2012-12-25");
      var vdat13(&date13);
      SQLVariantFormat fdat13(vdat13);
      fdat13.DateCalculate(' ',"9d");
      Assert::AreEqual("03-01-2013",fdat13.GetFormat());
      number_of_tests++;

      // Test for negative number of days

      SQLDate date14("03-01-2013");
      var vdat14(&date14);
      SQLVariantFormat fdat14(vdat14);
      fdat14.DateCalculate('+',"-9");
      Assert::AreEqual("25-12-2012",fdat14.GetFormat());
      number_of_tests++;
    }
  };
}

