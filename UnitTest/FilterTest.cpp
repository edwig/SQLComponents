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
#include <bcd.h>

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

      // STRINGS

      // Unary string function call
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

      // dual string function call
      TestFunctionString2("fieldname",OP_Equal,FN_CONCAT,           "value","fieldvalue","{fn CONCAT(fieldname,?)} = ?");
      TestFunctionString2("fieldname",OP_Equal,FN_DIFFERENCE,       "value","fieldvalue","{fn DIFFERENCE(fieldname,?)} = ?");
      TestFunctionString2("fieldname",OP_Equal,FN_LEFT,             "value","fieldvalue","{fn LEFT(fieldname,?)} = ?");
      TestFunctionString2("fieldname",OP_Equal,FN_POSITION,         "value","fieldvalue","{fn POSITION(fieldname IN ?)} = ?");
      TestFunctionString2("fieldname",OP_Equal,FN_REPEAT,           "value","fieldvalue","{fn REPEAT(fieldname,?)} = ?");
      TestFunctionString2("fieldname",OP_Equal,FN_RIGHT,            "value","fieldvalue","{fn RIGHT(fieldname,?)} = ?");
      
      // Ternary string function call
      TestFunctionString3("fieldname",OP_Equal,FN_SUBSTRING,        "value","fieldvalue","2","{fn SUBSTRING(fieldname,?,?)} = ?");
      TestFunctionString3("fieldname",OP_Equal,FN_REPLACE,          "value","fieldvalue","2","{fn REPLACE(fieldname,?,?)} = ?");
      
      TestFunctionString4("fieldname",OP_Equal,FN_INSERT,           "value","fieldvalue","1","2","{fn INSERT(fieldname,?,?,?)} = ?");
      TestFunctionString4("fieldname",OP_Equal,FN_LOCATE,           "value","fieldvalue","1","2","{fn LOCATE(fieldname,?,?,?)} = ?");

      // NUMBERS

      // Constant value
      TestFunctionConstant("fieldname",OP_Greater,FN_PI,"fieldname > {fn PI()}");
      // Unary function call
      TestFunctionNumber1("fieldname",OP_Equal,FN_ABS,              "18", "{fn ABS(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_ACOS,             "0.5","{fn ACOS(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_ASIN,             "0.5","{fn ASIN(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_ATAN,             "0.5","{fn ATAN(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_ATAN2,            "0.5","{fn ATAN2(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_CEILING,          "5",  "{fn CEILING(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_COS,              "0.5","{fn COS(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_COT,              "0.5","{fn COT(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_DEGREES,          "180","{fn DEGREES(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_EXP,              "5E6","{fn EXP(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_FLOOR,            "4",  "{fn FLOOR(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_LOG,              "4",  "{fn LOG(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_LOG10,            "4",  "{fn LOG10(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_RADIANS,          "0.5","{fn RADIANS(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_RAND,             "0.5","{fn RAND(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_SIGN,             "-5", "{fn SIGN(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_SIN,              "0.5","{fn SIN(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_SQRT,             "2",  "{fn SQRT(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_TAN,              "0.5","{fn TAN(fieldname)} = ?");
      // Dual number function call
      TestFunctionNumber2("fieldname",OP_Equal,FN_MOD,              "12","5","{fn MOD(fieldname,?)} = ?");
      TestFunctionNumber2("fieldname",OP_Equal,FN_POWER,            "2","64","{fn POWER(fieldname,?)} = ?");
      TestFunctionNumber2("fieldname",OP_Equal,FN_ROUND,            "3.52","1","{fn ROUND(fieldname,?)} = ?");
      TestFunctionNumber2("fieldname",OP_Equal,FN_TRUNCATE,         "3",   "1","{fn TRUNCATE(fieldname,?)} = ?");

      // DATE/TIME/INTERVAL

      // Constant value
      TestFunctionConstant("fieldname",OP_Smaller,FN_CURRENT_DATE,  "fieldname < {fn CURRENT_DATE()}");
      TestFunctionConstant("fieldname",OP_Smaller,FN_CURDATE,       "fieldname < {fn CURDATE()}");
      TestFunctionConstant("fieldname",OP_Smaller,FN_CURTIME,       "fieldname < {fn CURTIME()}");
      TestFunctionConstant("fieldname",OP_Smaller,FN_NOW,           "fieldname < {fn NOW()}");

      // Unary date/time function
      TestFunctionString1("fieldname",OP_Equal,FN_DAYNAME,          "Sunday",   "{fn DAYNAME(fieldname)} = ?");
      TestFunctionString1("fieldname",OP_Equal,FN_MONTHNAME,        "June",     "{fn MONTHNAME(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_DAYOFMONTH,       "1",        "{fn DAYOFMONTH(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_DAYOFWEEK,        "1",        "{fn DAYOFWEEK(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_DAYOFYEAR,        "172",      "{fn DAYOFYEAR(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_QUARTER,          "2",        "{fn QUARTER(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_YEAR,             "2019",     "{fn YEAR(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_MONTH,            "6",        "{fn MONTH(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_WEEK,             "23",       "{fn WEEK(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_HOUR,             "17",       "{fn HOUR(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_MINUTE,           "53",       "{fn MINUTE(fieldname)} = ?");
      TestFunctionNumber1("fieldname",OP_Equal,FN_SECOND,           "12",       "{fn SECOND(fieldname)} = ?");

      // Secondary date/time functions
      TestFunctionExtract("fieldname",OP_Equal,FN_EXTRACT,TS_EXT_YEAR,  "2019","{fn EXTRACT(YEAR FROM fieldname)} = ?");
      TestFunctionExtract("fieldname",OP_Equal,FN_EXTRACT,TS_EXT_MONTH,    "6","{fn EXTRACT(MONTH FROM fieldname)} = ?");
      TestFunctionExtract("fieldname",OP_Equal,FN_EXTRACT,TS_EXT_DAY,     "21","{fn EXTRACT(DAY FROM fieldname)} = ?");
      TestFunctionExtract("fieldname",OP_Equal,FN_EXTRACT,TS_EXT_HOUR,    "19","{fn EXTRACT(HOUR FROM fieldname)} = ?");
      TestFunctionExtract("fieldname",OP_Equal,FN_EXTRACT,TS_EXT_MINUTE,  "55","{fn EXTRACT(MINUTE FROM fieldname)} = ?");
      TestFunctionExtract("fieldname",OP_Equal,FN_EXTRACT,TS_EXT_SECOND,  "12","{fn EXTRACT(SECOND FROM fieldname)} = ?");

      // Ternary date/time functions
      TestFunctionTSCalc("fieldname",OP_Smaller,FN_TIMESTAMPADD,SQL_TSI_YEAR,   3,"2019-06-01","{fn TIMESTAMPADD(SQL_TSI_YEAR,?,fieldname)} < ?");
      TestFunctionTSCalc("fieldname",OP_Smaller,FN_TIMESTAMPADD,SQL_TSI_MONTH,  3,"2019-06-01","{fn TIMESTAMPADD(SQL_TSI_MONTH,?,fieldname)} < ?");
      TestFunctionTSCalc("fieldname",OP_Smaller,FN_TIMESTAMPADD,SQL_TSI_DAY,    3,"2019-06-01","{fn TIMESTAMPADD(SQL_TSI_DAY,?,fieldname)} < ?");
      TestFunctionTSCalc("fieldname",OP_Smaller,FN_TIMESTAMPADD,SQL_TSI_HOUR,   3,"2019-06-01","{fn TIMESTAMPADD(SQL_TSI_HOUR,?,fieldname)} < ?");
      TestFunctionTSCalc("fieldname",OP_Smaller,FN_TIMESTAMPADD,SQL_TSI_MINUTE, 3,"2019-06-01","{fn TIMESTAMPADD(SQL_TSI_MINUTE,?,fieldname)} < ?");
      TestFunctionTSCalc("fieldname",OP_Smaller,FN_TIMESTAMPADD,SQL_TSI_SECOND, 3,"2019-06-01","{fn TIMESTAMPADD(SQL_TSI_SECOND,?,fieldname)} < ?");

      // SYSTEM FUNCTIONS

      TestFunctionConstant("'testing'",OP_Equal,FN_DATABASE,        "'testing' = {fn DATABASE()}");
      TestFunctionConstant("'sysdba'", OP_Equal,FN_USER,            "'sysdba' = {fn USER()}");
      TestFunctionString2 ("fieldname",OP_Equal,FN_IFNULL,          "other", "other", "{fn IFNULL(fieldname,?)} = ?");
    }

    void TestFunctionConstant(CString p_field,SQLOperator p_oper,SQLFunction p_function,CString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      filter.SetFunction(p_function);
      CString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage("Filter with constant: " + condition);
      Assert::AreEqual(p_expect.GetString(), condition.GetString());
      ++number_of_tests;
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

    void TestFunctionNumber1(CString p_field,SQLOperator p_oper,SQLFunction p_function,bcd p_number,CString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      SQLVariant val1(&p_number);
      filter.SetFunction(p_function);
      filter.AddValue(&val1);
      CString  condition = filter.GetSQLFilter(query);

      Logger::WriteMessage("Number function filter: " + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      ++number_of_tests;
    }

    void TestFunctionNumber2(CString p_field,SQLOperator p_oper,SQLFunction p_function,bcd p_number1,bcd p_number2,CString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      SQLVariant val1(&p_number1);
      SQLVariant val2(&p_number2);
      filter.SetFunction(p_function);
      filter.AddValue(&val1);
      filter.AddValue(&val2);
      CString  condition = filter.GetSQLFilter(query);

      Logger::WriteMessage("Number function filter: " + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      ++number_of_tests;
    }

    void TestFunctionExtract(CString p_field,SQLOperator p_oper,SQLFunction p_function,SQLExtractPart p_part,CString p_value,CString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      SQLVariant val1(&p_value);
      filter.SetFunction(p_function);
      filter.SetExtractPart(p_part);
      filter.AddValue(&val1);
      CString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage("EXTRACT function filter: " + condition);
      Assert::AreEqual(p_expect.GetString(), condition.GetString());
      ++number_of_tests;
    }

    void TestFunctionTSCalc(CString p_field,SQLOperator p_oper,SQLFunction p_function,SQLTimestampCalcPart p_part,int p_add,CString p_value,CString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field, p_oper);
      SQLVariant val1(p_add);
      SQLVariant val2(&p_value);
      filter.SetFunction(p_function);
      filter.SetTimestampPart(p_part);
      filter.AddValue(&val1);
      filter.AddValue(&val2);
      CString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage("TIMSTAMP ADD/DIFF function filter: " + condition);
      Assert::AreEqual(p_expect.GetString(), condition.GetString());
      ++number_of_tests;
    }

  };
}

