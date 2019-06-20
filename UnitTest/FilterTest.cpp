////////////////////////////////////////////////////////////////////////
//
// File: FilterTest.cpp
//
// Copyright (c) 1998-2019 ir. W.E. Huisman
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
// Last Revision:  15-06-2019
// Version number: 1.5.5
//
#include "stdafx.h"
#include "SQLFilter.h"
#include "SQLVariant.h"
#include "SQLDate.h"
#include "SQLQuery.h"
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

      TestFilter("fieldname",OP_Equal,valText,  "fieldname = ? : Text");
      TestFilter("fieldname",OP_Equal,valNumber,"fieldname = ? : 123");
      TestFilter("fieldname",OP_Equal,valDate,  "fieldname = ? : 15-10-1959");
      TestFilter("fieldname",OP_Equal,valTime,  "fieldname = ? : 08:23:49");
      TestFilter("fieldname",OP_Equal,valStamp, "fieldname = ? : 2017-01-22 11:45:16");

      TestFilter("fieldname",OP_NotEqual,    valNumber,"fieldname <> ? : 123");
      TestFilter("fieldname",OP_Greater,     valNumber,"fieldname > ? : 123");
      TestFilter("fieldname",OP_GreaterEqual,valNumber,"fieldname >= ? : 123");
      TestFilter("fieldname",OP_Smaller,     valNumber,"fieldname < ? : 123");
      TestFilter("fieldname",OP_SmallerEqual,valNumber,"fieldname <= ? : 123");

      TestFilter("fieldname",OP_IsNULL,    valText,"fieldname IS NULL : Text");
      TestFilter("fieldname",OP_LikeBegin, valText,"fieldname LIKE 'Text%' : Text");
      TestFilter("fieldname",OP_LikeMiddle,valText,"fieldname LIKE '%Text%' : Text");

      TestBetween("fieldname",OP_Between,valText,  valTextExtra, "fieldname BETWEEN ? AND ?  : Text : ZZZ");
      TestBetween("fieldname",OP_Between,valNumber,valNumberHigh,"fieldname BETWEEN ? AND ?  : 123 : 999");

      TestIN("fieldname",OP_IN,valNumber,valEx4,valEx3,valNumberHigh,"fieldname IN (?,?,?,?) : 123 : 777 : 888 : 999");
      TestExpression("fieldname",OP_Equal,"{fn UCASE(TheFunction(other))}","fieldname = {fn UCASE(TheFunction(other))}");

      TestNegate("fieldname",OP_Equal,valText,  "NOT fieldname = ? : Text");
      TestNegate("fieldname",OP_Equal,valNumber,"NOT fieldname = ? : 123");
      TestNegate("fieldname",OP_Equal,valDate,  "NOT fieldname = ? : 15-10-1959");
      TestNegate("fieldname",OP_Equal,valTime,  "NOT fieldname = ? : 08:23:49");
      TestNegate("fieldname",OP_Equal,valStamp, "NOT fieldname = ? : 2017-01-22 11:45:16");

      TestNegate("fieldname",OP_NotEqual,    valNumber,"NOT fieldname <> ? : 123");
      TestNegate("fieldname",OP_Greater,     valNumber,"NOT fieldname > ? : 123");
      TestNegate("fieldname",OP_GreaterEqual,valNumber,"NOT fieldname >= ? : 123");
      TestNegate("fieldname",OP_Smaller,     valNumber,"NOT fieldname < ? : 123");
      TestNegate("fieldname",OP_SmallerEqual,valNumber,"NOT fieldname <= ? : 123");

      TestNegate("fieldname",OP_IsNULL,    valText,"NOT fieldname IS NULL : Text");
      TestNegate("fieldname",OP_LikeBegin, valText,"NOT fieldname LIKE 'Text%' : Text");
      TestNegate("fieldname",OP_LikeMiddle,valText,"NOT fieldname LIKE '%Text%' : Text");
    }

    void TestFilter(CString p_field,SQLOperator p_oper,SQLVariant& p_variant,CString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper,&p_variant);
      CString condition = filter.GetSQLFilter(query) + " : " + p_variant.GetAsChar();

      Logger::WriteMessage("Filter: " + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      number_of_tests++;
    }

    void TestNegate(CString p_field,SQLOperator p_oper,SQLVariant& p_variant,CString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper,&p_variant);
      filter.Negate();
      CString condition = filter.GetSQLFilter(query) + " : " + p_variant.GetAsChar();

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
      SQLQuery query;
      SQLFilter filter(p_field,p_oper,&p_variant1);
      filter.AddValue(&p_variant2);
      CString condition = filter.GetSQLFilter(query) + " : " + p_variant1.GetAsChar();
      condition += CString(" : ") + p_variant2.GetAsChar();

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
      SQLQuery query;
      SQLFilter filter(p_field,p_oper,&p_variant1);
      filter.AddValue(&p_variant2);
      filter.AddValue(&p_variant3);
      filter.AddValue(&p_variant4);
      CString condition = filter.GetSQLFilter(query);
      condition += CString(" : ") + p_variant1.GetAsChar();
      condition += CString(" : ") + p_variant2.GetAsChar();
      condition += CString(" : ") + p_variant3.GetAsChar();
      condition += CString(" : ") + p_variant4.GetAsChar();

      Logger::WriteMessage("Filter: " + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      number_of_tests++;
    }

    void TestExpression(CString p_field,SQLOperator p_oper,CString p_expression,CString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      filter.AddExpression(p_expression);
      CString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage("Filter: " + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      number_of_tests++;
    }

    TEST_METHOD(FilterFunction)
    {
      Logger::WriteMessage("SQLFilter test with ODBC functions combined");
      Logger::WriteMessage("===========================================");

      // Needed for date/time/timestamp
      InitSQLComponents(LN_ENGLISH);

      // Unairy function call
      TestFunctionString1("fieldname",OP_Equal,FN_ASCII,            "12","{fn ASCII(fieldname)} = ?");
      TestFunctionString1("fieldname",OP_Equal,FN_BIT_LENGTH,       "12","{fn BIT_LENGTH(fieldname)} = ?");
      TestFunctionString1("fieldname",OP_Equal,FN_CHAR,             "12","{fn CHAR(fieldname)} = ?");
      TestFunctionString1("fieldname",OP_Equal,FN_CHAR_LENGTH,      "12","{fn CHAR_LENGTH(fieldname)} = ?");
      TestFunctionString1("fieldname",OP_Equal,FN_CHARACTER_LENGTH, "12","{fn CHARACTER_LENGTH(fieldname)} = ?");
      TestFunctionString1("fieldname",OP_Equal,FN_LCASE,            "12","{fn LCASE(fieldname)} = ?");
      TestFunctionString1("fieldname",OP_Equal,FN_LENGTH,           "12","{fn LENGTH(fieldname)} = ?");
      TestFunctionString1("fieldname",OP_Equal,FN_LTRIM,            "12","{fn LTRIM(fieldname)} = ?");
      TestFunctionString1("fieldname",OP_Equal,FN_OCTET_LENGTH,     "12","{fn OCTET_LENGTH(fieldname)} = ?");
      TestFunctionString1("fieldname",OP_Equal,FN_RTRIM,            "12","{fn RTRIM(fieldname)} = ?");
      TestFunctionString1("fieldname",OP_Equal,FN_SOUNDEX,          "12","{fn SOUNDEX(fieldname)} = ?");
      TestFunctionString1("fieldname",OP_Equal,FN_SPACE,            "12","{fn SPACE(fieldname)} = ?");
      TestFunctionString1("fieldname",OP_Equal,FN_UCASE,            "12","{fn UCASE(fieldname)} = ?");

      TestFunctionString2("fieldname",OP_Equal,FN_CONCAT,           "waarde","veldwaarde","{fn CONCAT(fieldname,?)} = ?");
      TestFunctionString2("fieldname",OP_Equal,FN_DIFFERENCE,       "waarde","veldwaarde","{fn DIFFERENCE(fieldname,?)} = ?");
      TestFunctionString2("fieldname",OP_Equal,FN_LEFT,             "waarde","veldwaarde","{fn LEFT(fieldname,?)} = ?");
      TestFunctionString2("fieldname",OP_Equal,FN_POSITION,         "waarde","veldwaarde","{fn POSITION(fieldname IN ?)} = ?");
      TestFunctionString2("fieldname",OP_Equal,FN_REPEAT,           "waarde","veldwaarde","{fn REPEAT(fieldname,?)} = ?");
      TestFunctionString2("fieldname",OP_Equal,FN_RIGHT,            "waarde","veldwaarde","{fn RIGHT(fieldname,?)} = ?");
      
      TestFunctionString3("fieldname",OP_Equal,FN_SUBSTRING,        "waarde","veldwaarde","2","{fn SUBSTRING(fieldname,?,?)} = ?");
      TestFunctionString3("fieldname",OP_Equal,FN_REPLACE,          "waarde","veldwaarde","2","{fn REPLACE(fieldname,?,?)} = ?");
      
      TestFunctionString4("fieldname",OP_Equal,FN_INSERT,           "waarde","veldwaarde","1","2","{fn INSERT(fieldname,?,?,?)} = ?");
      TestFunctionString4("fieldname",OP_Equal,FN_LOCATE,           "waarde","veldwaarde","1","2","{fn LOCATE(fieldname,?,?,?)} = ?");

    }

    void TestFunctionString1(CString p_field,SQLOperator p_oper,SQLFunction p_function,CString p_value,CString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      SQLVariant val(p_value);
      filter.SetFunction(p_function);
      filter.AddValue(&val);
      CString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage("String function filter: " + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      ++number_of_tests;
    }

    void TestFunctionString2(CString p_field,SQLOperator p_oper,SQLFunction p_function,CString p_value1,CString p_value2,CString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      SQLVariant val1(p_value1);
      SQLVariant val2(p_value2);
      filter.SetFunction(p_function);
      filter.AddValue(&val1);
      filter.AddValue(&val2);
      CString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage("String function filter: " + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      ++number_of_tests;
    }

    void TestFunctionString3(CString p_field,SQLOperator p_oper,SQLFunction p_function
                            ,CString p_value1,CString p_value2,CString p_value3
                            ,CString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      SQLVariant val1(p_value1);
      SQLVariant val2(p_value2);
      SQLVariant val3(p_value3);
      filter.SetFunction(p_function);
      filter.AddValue(&val1);
      filter.AddValue(&val2);
      filter.AddValue(&val3);
      CString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage("String function filter: " + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      ++number_of_tests;
    }

    void TestFunctionString4(CString p_field,SQLOperator p_oper,SQLFunction p_function
                            ,CString p_value1,CString p_value2,CString p_value3,CString p_value4
                            ,CString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      SQLVariant val1(p_value1);
      SQLVariant val2(p_value2);
      SQLVariant val3(p_value3);
      SQLVariant val4(p_value4);
      filter.SetFunction(p_function);
      filter.AddValue(&val1);
      filter.AddValue(&val2);
      filter.AddValue(&val3);
      filter.AddValue(&val4);
      CString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage("String function filter: " + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      ++number_of_tests;
    }
  };
}

