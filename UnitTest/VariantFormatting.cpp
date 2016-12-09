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

      Assert::AreEqual(format1,format2);
    }

    TEST_METHOD(FormatDate)
    {

    }
  };
}
