////////////////////////////////////////////////////////////////////////
//
// File: VariantFormatting.cpp
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
// Last Revision:   04-12-2016
// Version number:  1.3.0
//
#include "stdafx.h"
#include "CppUnitTest.h"
#include "SQLVariantFormat.h"
#include "SQLTimestamp.h"
#include "SQLTime.h"
#include "SQLDate.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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

      form.StringStartCapital();
      Assert::AreEqual("This is a long and tiresome test",form.GetFormat());
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
    }

    TEST_METHOD(FormatDate)
    {
      Logger::WriteMessage("Formatting: Formatting the date/time");

      CString dateString("15-10-1959");
      SQLDate date(dateString);
      var dateVar(&date);
      SQLVariantFormat form(dateVar);

      form.FormatDate("dddd d MMMM yyyy|H:mm:ss");
      Assert::AreEqual("donderdag 15 oktober 1959",form.GetFormat());

      form.FormatDate("ddd dd MMM yyyy|H:mm:ss");
      Logger::WriteMessage(form.GetFormat());
      Assert::AreEqual("do 15 okt 1959",form.GetFormat());

      CString stampString("1959-10-15 15:50:20");
      SQLTimestamp stamp(stampString);
      var stampVar(&stamp);
      SQLVariantFormat st(stampVar);

      st.FormatDate("dddd d MMMM yyyy |H:mm:ss");
      Assert::AreEqual("donderdag 15 oktober 1959 15:50:20",st.GetFormat());

      st.FormatDate("@");
      Assert::AreEqual("donderdag 15 oktober 1959",st.GetFormat());

      st.FormatDate("");
      Assert::AreEqual("15-10-1959",st.GetFormat());

      form.Reset();
      st.ReFormat();
      st.ResetValue();
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

      form.FormatNumber("#",false);
      Logger::WriteMessage(form.GetFormat());
      Assert::AreEqual("45123",form.GetFormat());

      form.FormatNumber("###,###.00",false);      
      Assert::AreEqual(" 45.123,12",    form.GetFormat());

      form.FormatNumber("&&&,&&&.00",false);
      Assert::AreEqual("45.123,12",form.GetFormat());

      form.FormatNumber("&&&,&&&.x",false);
      Assert::AreEqual("45.123,1",form.GetFormat());

      form.FormatNumber("&&&,&&&.X",false);
      Assert::AreEqual("45.123",form.GetFormat());

      form.FormatNumber("#:00",false);
      Assert::AreEqual("12",form.GetFormat());

      form.FormatNumber("+###,###.00",false);
      Assert::AreEqual ("  45.123,12",form.GetFormat());
      negf.FormatNumber("+###,###.00",false);
      Assert::AreEqual ("- 66.789,56",negf.GetFormat());

      form.FormatNumber("-###,###.00",false);
      Assert::AreEqual ("+ 45.123,12",form.GetFormat());
      negf.FormatNumber("-###,###.00",false);
      Assert::AreEqual ("- 66.789,56",negf.GetFormat());
      negf.FormatNumber("~###,###.00",false);
      Assert::AreEqual ("- 66.789,56",negf.GetFormat());
    }
  };
}
