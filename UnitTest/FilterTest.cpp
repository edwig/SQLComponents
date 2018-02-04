////////////////////////////////////////////////////////////////////////
//
// File: FilterTest.cpp
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
// Last Revision:   04-02-2018
// Version number:  1.4.5
//
#include "stdafx.h"
#include "SQLFilter.h"
#include "SQLVariant.h"
#include "SQLDate.h"
#include "UnitTest.h"

namespace OperatorUnitTest
{

  TEST_CLASS(SQLFilterTest)
  {
    TEST_METHOD(FilterTest)
    {
      Logger::WriteMessage("SQLFilter test for SQL WHERE conditions");
      Logger::WriteMessage("=======================================");

      // Needed for date/time/timestamp
      InitSQLComponents(LN_ENGLISH);

      SQLVariant valText("Text");
      SQLVariant valNumber(123L);
      SQLDate datum("15-10-1959");
      SQLVariant valDate(&datum);
      SQLTime then("08:23:49");
      SQLVariant valTime(&then);
      SQLTimestamp stamp("2017-01-22 11:45:16");
      SQLVariant valStamp(&stamp);
      SQLVariant valTextExtra("ZZZ");
      SQLVariant valNumberHigh(999L);
      SQLVariant valEx3(888L);
      SQLVariant valEx4(777L);

      TestFilter("fieldname",OP_Equal,valText,  "fieldname = 'Text'");
      TestFilter("fieldname",OP_Equal,valNumber,"fieldname = 123");
      TestFilter("fieldname",OP_Equal,valDate,  "fieldname = {d '1959-10-15'}");
      TestFilter("fieldname",OP_Equal,valTime,  "fieldname = {t '08:23:49'}");
      TestFilter("fieldname",OP_Equal,valStamp, "fieldname = {ts '2017-01-22 11:45:16'}");

      TestFilter("fieldname",OP_NotEqual,    valNumber,"fieldname <> 123");
      TestFilter("fieldname",OP_Greater,     valNumber,"fieldname > 123");
      TestFilter("fieldname",OP_GreaterEqual,valNumber,"fieldname >= 123");
      TestFilter("fieldname",OP_Smaller,     valNumber,"fieldname < 123");
      TestFilter("fieldname",OP_SmallerEqual,valNumber,"fieldname <= 123");

      TestFilter("fieldname",OP_IsNULL,    valText,"fieldname IS NULL");
      TestFilter("fieldname",OP_LikeBegin, valText,"fieldname LIKE 'Text%'");
      TestFilter("fieldname",OP_LikeMiddle,valText,"fieldname LIKE '%Text%'");

      TestBetween("fieldname",OP_Between,valText,  valTextExtra, "fieldname BETWEEN 'Text' AND 'ZZZ'");
      TestBetween("fieldname",OP_Between,valNumber,valNumberHigh,"fieldname BETWEEN 123 AND 999");

      TestIN("fieldname",OP_IN,valNumber,valEx4,valEx3,valNumberHigh,"fieldname IN (123,777,888,999)");
      TestExpression("fieldname",OP_Equal,"{fn UCASE(TheFunction(other))}","fieldname = {fn UCASE(TheFunction(other))}");

      TestNegate("fieldname",OP_Equal,valText,  "NOT fieldname = 'Text'");
      TestNegate("fieldname",OP_Equal,valNumber,"NOT fieldname = 123");
      TestNegate("fieldname",OP_Equal,valDate,  "NOT fieldname = {d '1959-10-15'}");
      TestNegate("fieldname",OP_Equal,valTime,  "NOT fieldname = {t '08:23:49'}");
      TestNegate("fieldname",OP_Equal,valStamp, "NOT fieldname = {ts '2017-01-22 11:45:16'}");

      TestNegate("fieldname",OP_NotEqual,    valNumber,"NOT fieldname <> 123");
      TestNegate("fieldname",OP_Greater,     valNumber,"NOT fieldname > 123");
      TestNegate("fieldname",OP_GreaterEqual,valNumber,"NOT fieldname >= 123");
      TestNegate("fieldname",OP_Smaller,     valNumber,"NOT fieldname < 123");
      TestNegate("fieldname",OP_SmallerEqual,valNumber,"NOT fieldname <= 123");

      TestNegate("fieldname",OP_IsNULL,    valText,"NOT fieldname IS NULL");
      TestNegate("fieldname",OP_LikeBegin, valText,"NOT fieldname LIKE 'Text%'");
      TestNegate("fieldname",OP_LikeMiddle,valText,"NOT fieldname LIKE '%Text%'");
    }

    void TestFilter(CString p_field,SQLOperator p_oper,SQLVariant& p_variant,CString p_expect)
    {
      SQLFilter filter(p_field,p_oper,&p_variant);
      CString condition = filter.GetSQLFilter();

      Logger::WriteMessage("Filter: " + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      number_of_tests++;
    }

    void TestNegate(CString p_field,SQLOperator p_oper,SQLVariant& p_variant,CString p_expect)
    {
      SQLFilter filter(p_field,p_oper,&p_variant);
      filter.Negate();
      CString condition = filter.GetSQLFilter();

      Logger::WriteMessage("Filter: " + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      number_of_tests++;
    }

    void TestBetween(CString     p_field
                    ,SQLOperator p_oper
                    ,SQLVariant& p_variant1
                    ,SQLVariant& p_variant2
                    ,CString     p_expect)
    {
      SQLFilter filter(p_field,p_oper,&p_variant1);
      filter.AddValue(&p_variant2);
      CString condition = filter.GetSQLFilter();

      Logger::WriteMessage("Filter: " + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      number_of_tests++;

      // Test the getting of the values
      SQLVariant* var = filter.GetValue(1);
      Assert::IsTrue(p_variant2 == *var);
      number_of_tests++;
    }

    void TestIN(CString     p_field
               ,SQLOperator p_oper
               ,SQLVariant& p_variant1
               ,SQLVariant& p_variant2
               ,SQLVariant& p_variant3
               ,SQLVariant& p_variant4
               ,CString     p_expect)
    {
      SQLFilter filter(p_field,p_oper,&p_variant1);
      filter.AddValue(&p_variant2);
      filter.AddValue(&p_variant3);
      filter.AddValue(&p_variant4);
      CString condition = filter.GetSQLFilter();

      Logger::WriteMessage("Filter: " + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      number_of_tests++;
    }

    void TestExpression(CString p_field,SQLOperator p_oper,CString p_expression,CString p_expect)
    {
      SQLFilter filter(p_field,p_oper);
      filter.AddExpression(p_expression);
      CString condition = filter.GetSQLFilter();

      Logger::WriteMessage("Filter: " + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      number_of_tests++;
    }
  };
}

