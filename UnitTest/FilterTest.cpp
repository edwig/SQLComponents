////////////////////////////////////////////////////////////////////////
//
// File: FilterTest.cpp
//
// Copyright (c) 1998-2024 ir. W.E. Huisman
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
      Logger::WriteMessage(_T("SQLFilter test for SQL WHERE conditions"));
      Logger::WriteMessage(_T("======================================="));

      // Needed for date/time/timestamp
      InitSQLComponents(LN_ENGLISH);

      SQLVariant valText(_T("Text"));
      SQLVariant valNumber(123);
      SQLDate datum(_T("15-10-1959"));
      SQLVariant valDate(&datum);
      SQLTime then(_T("08:23:49"));
      SQLVariant valTime(&then);
      SQLTimestamp stamp(_T("2017-01-22 11:45:16"));
      SQLVariant valStamp(&stamp);
      SQLVariant valTextExtra(_T("ZZZ"));
      SQLVariant valNumberHigh(999);
      SQLVariant valEx3(888);
      SQLVariant valEx4(777);

      TestFilter(_T("fieldname"),OP_Equal,valText,  _T("fieldname = ? : Text"));
      TestFilter(_T("fieldname"),OP_Equal,valNumber,_T("fieldname = ? : 123"));
      TestFilter(_T("fieldname"),OP_Equal,valDate,  _T("fieldname = ? : 15-10-1959"));
      TestFilter(_T("fieldname"),OP_Equal,valTime,  _T("fieldname = ? : 08:23:49"));
      TestFilter(_T("fieldname"),OP_Equal,valStamp, _T("fieldname = ? : 2017-01-22 11:45:16"));

      TestFilter(_T("fieldname"),OP_NotEqual,    valNumber,_T("fieldname <> ? : 123"));
      TestFilter(_T("fieldname"),OP_Greater,     valNumber,_T("fieldname > ? : 123"));
      TestFilter(_T("fieldname"),OP_GreaterEqual,valNumber,_T("fieldname >= ? : 123"));
      TestFilter(_T("fieldname"),OP_Smaller,     valNumber,_T("fieldname < ? : 123"));
      TestFilter(_T("fieldname"),OP_SmallerEqual,valNumber,_T("fieldname <= ? : 123"));

      TestFilter(_T("fieldname"),OP_IsNULL,    valText,_T("fieldname IS NULL : Text"));
      TestFilter(_T("fieldname"),OP_LikeBegin, valText,_T("fieldname LIKE 'Text%' : Text"));
      TestFilter(_T("fieldname"),OP_LikeMiddle,valText,_T("fieldname LIKE '%Text%' : Text"));

      TestBetween(_T("fieldname"),OP_Between,valText,  valTextExtra, _T("fieldname BETWEEN ? AND ?  : Text : ZZZ"));
      TestBetween(_T("fieldname"),OP_Between,valNumber,valNumberHigh,_T("fieldname BETWEEN ? AND ?  : 123 : 999"));

      TestIN(_T("fieldname"),OP_IN,valNumber,valEx4,valEx3,valNumberHigh,_T("fieldname IN (?,?,?,?) : 123 : 777 : 888 : 999"));
      TestExpression(_T("fieldname"),OP_Equal,_T("{fn UCASE(TheFunction(other))}"),_T("fieldname = {fn UCASE(TheFunction(other))}"));

      TestNegate(_T("fieldname"),OP_Equal,valText,  _T("NOT fieldname = ? : Text"));
      TestNegate(_T("fieldname"),OP_Equal,valNumber,_T("NOT fieldname = ? : 123"));
      TestNegate(_T("fieldname"),OP_Equal,valDate,  _T("NOT fieldname = ? : 15-10-1959"));
      TestNegate(_T("fieldname"),OP_Equal,valTime,  _T("NOT fieldname = ? : 08:23:49"));
      TestNegate(_T("fieldname"),OP_Equal,valStamp, _T("NOT fieldname = ? : 2017-01-22 11:45:16"));

      TestNegate(_T("fieldname"),OP_NotEqual,    valNumber,_T("NOT fieldname <> ? : 123"));
      TestNegate(_T("fieldname"),OP_Greater,     valNumber,_T("NOT fieldname > ? : 123"));
      TestNegate(_T("fieldname"),OP_GreaterEqual,valNumber,_T("NOT fieldname >= ? : 123"));
      TestNegate(_T("fieldname"),OP_Smaller,     valNumber,_T("NOT fieldname < ? : 123"));
      TestNegate(_T("fieldname"),OP_SmallerEqual,valNumber,_T("NOT fieldname <= ? : 123"));

      TestNegate(_T("fieldname"),OP_IsNULL,    valText,_T("NOT fieldname IS NULL : Text"));
      TestNegate(_T("fieldname"),OP_LikeBegin, valText,_T("NOT fieldname LIKE 'Text%' : Text"));
      TestNegate(_T("fieldname"),OP_LikeMiddle,valText,_T("NOT fieldname LIKE '%Text%' : Text"));
    }

    void TestFilter(XString p_field,SQLOperator p_oper,SQLVariant& p_variant,XString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper,&p_variant);
      XString condition = filter.GetSQLFilter(query) + _T(" : ") + p_variant.GetAsChar();

      Logger::WriteMessage(_T("Filter: ") + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      number_of_tests++;
    }

    void TestNegate(XString p_field,SQLOperator p_oper,SQLVariant& p_variant,XString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper,&p_variant);
      filter.Negate();
      XString condition = filter.GetSQLFilter(query) + _T(" : ") + p_variant.GetAsChar();

      Logger::WriteMessage(_T("Filter: ") + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      number_of_tests++;
    }

    void TestBetween(XString     p_field
                    ,SQLOperator p_oper
                    ,SQLVariant& p_variant1
                    ,SQLVariant& p_variant2
                    ,XString     p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper,&p_variant1);
      filter.AddValue(&p_variant2);
      XString condition = filter.GetSQLFilter(query) + _T(" : ") + p_variant1.GetAsChar();
      condition += XString(_T(" : ")) + p_variant2.GetAsChar();

      Logger::WriteMessage(_T("Filter: ") + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      number_of_tests++;

      // Test the getting of the values
      SQLVariant* var = filter.GetValue(1);
      Assert::IsTrue(p_variant2 == *var);
      number_of_tests++;
    }

    void TestIN(XString     p_field
               ,SQLOperator p_oper
               ,SQLVariant& p_variant1
               ,SQLVariant& p_variant2
               ,SQLVariant& p_variant3
               ,SQLVariant& p_variant4
               ,XString     p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper,&p_variant1);
      filter.AddValue(&p_variant2);
      filter.AddValue(&p_variant3);
      filter.AddValue(&p_variant4);
      XString condition = filter.GetSQLFilter(query);
      condition += XString(_T(" : ")) + p_variant1.GetAsChar();
      condition += XString(_T(" : ")) + p_variant2.GetAsChar();
      condition += XString(_T(" : ")) + p_variant3.GetAsChar();
      condition += XString(_T(" : ")) + p_variant4.GetAsChar();

      Logger::WriteMessage(_T("Filter: ") + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      number_of_tests++;
    }

    void TestExpression(XString p_field,SQLOperator p_oper,XString p_expression,XString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      filter.AddExpression(p_expression);
      XString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage(_T("Filter: ") + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      number_of_tests++;
    }

    TEST_METHOD(FilterFunction)
    {
      Logger::WriteMessage(_T("SQLFilter test with ODBC functions combined"));
      Logger::WriteMessage(_T("==========================================="));

      // Needed for date/time/timestamp
      InitSQLComponents(LN_ENGLISH);

      // STRINGS

      // Unary string function call
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_ASCII,            _T("12"),_T("{fn ASCII(fieldname)} = ?"));
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_BIT_LENGTH,       _T("12"),_T("{fn BIT_LENGTH(fieldname)} = ?"));
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_CHAR,             _T("12"),_T("{fn CHAR(fieldname)} = ?"));
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_CHAR_LENGTH,      _T("12"),_T("{fn CHAR_LENGTH(fieldname)} = ?"));
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_CHARACTER_LENGTH, _T("12"),_T("{fn CHARACTER_LENGTH(fieldname)} = ?"));
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_LCASE,            _T("12"),_T("{fn LCASE(fieldname)} = ?"));
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_LENGTH,           _T("12"),_T("{fn LENGTH(fieldname)} = ?"));
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_LTRIM,            _T("12"),_T("{fn LTRIM(fieldname)} = ?"));
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_OCTET_LENGTH,     _T("12"),_T("{fn OCTET_LENGTH(fieldname)} = ?"));
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_RTRIM,            _T("12"),_T("{fn RTRIM(fieldname)} = ?"));
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_SOUNDEX,          _T("12"),_T("{fn SOUNDEX(fieldname)} = ?"));
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_SPACE,            _T("12"),_T("{fn SPACE(fieldname)} = ?"));
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_UCASE,            _T("12"),_T("{fn UCASE(fieldname)} = ?"));

      // dual string function call
      TestFunctionString2(_T("fieldname"),OP_Equal,FN_CONCAT,           _T("value"),_T("fieldvalue"),_T("{fn CONCAT(fieldname,?)} = ?"));
      TestFunctionString2(_T("fieldname"),OP_Equal,FN_DIFFERENCE,       _T("value"),_T("fieldvalue"),_T("{fn DIFFERENCE(fieldname,?)} = ?"));
      TestFunctionString2(_T("fieldname"),OP_Equal,FN_LEFT,             _T("value"),_T("fieldvalue"),_T("{fn LEFT(fieldname,?)} = ?"));
      TestFunctionString2(_T("fieldname"),OP_Equal,FN_POSITION,         _T("value"),_T("fieldvalue"),_T("{fn POSITION(fieldname IN ?)} = ?"));
      TestFunctionString2(_T("fieldname"),OP_Equal,FN_REPEAT,           _T("value"),_T("fieldvalue"),_T("{fn REPEAT(fieldname,?)} = ?"));
      TestFunctionString2(_T("fieldname"),OP_Equal,FN_RIGHT,            _T("value"),_T("fieldvalue"),_T("{fn RIGHT(fieldname,?)} = ?"));
      
      // Ternary string function call
      TestFunctionString3(_T("fieldname"),OP_Equal,FN_SUBSTRING,        _T("value"),_T("fieldvalue"),_T("2"),_T("{fn SUBSTRING(fieldname,?,?)} = ?"));
      TestFunctionString3(_T("fieldname"),OP_Equal,FN_REPLACE,          _T("value"),_T("fieldvalue"),_T("2"),_T("{fn REPLACE(fieldname,?,?)} = ?"));
      
      TestFunctionString4(_T("fieldname"),OP_Equal,FN_INSERT,           _T("value"),_T("fieldvalue"),_T("1"),_T("2"),_T("{fn INSERT(fieldname,?,?,?)} = ?"));
      TestFunctionString4(_T("fieldname"),OP_Equal,FN_LOCATE,           _T("value"),_T("fieldvalue"),_T("1"),_T("2"),_T("{fn LOCATE(fieldname,?,?,?)} = ?"));

      // NUMBERS

      // Constant value
      TestFunctionConstant(_T("fieldname"),OP_Greater,FN_PI,_T("fieldname > {fn PI()}"));
      // Unary function call
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_ABS,              bcd(_T("18")), _T("{fn ABS(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_ACOS,             bcd(_T("0.5")),_T("{fn ACOS(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_ASIN,             bcd(_T("0.5")),_T("{fn ASIN(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_ATAN,             bcd(_T("0.5")),_T("{fn ATAN(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_ATAN2,            bcd(_T("0.5")),_T("{fn ATAN2(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_CEILING,          bcd(_T("5")),  _T("{fn CEILING(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_COS,              bcd(_T("0.5")),_T("{fn COS(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_COT,              bcd(_T("0.5")),_T("{fn COT(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_DEGREES,          bcd(_T("180")),_T("{fn DEGREES(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_EXP,              bcd(_T("5E6")),_T("{fn EXP(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_FLOOR,            bcd(_T("4")),  _T("{fn FLOOR(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_LOG,              bcd(_T("4")),  _T("{fn LOG(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_LOG10,            bcd(_T("4")),  _T("{fn LOG10(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_RADIANS,          bcd(_T("0.5")),_T("{fn RADIANS(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_RAND,             bcd(_T("0.5")),_T("{fn RAND(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_SIGN,             bcd(_T("-5")), _T("{fn SIGN(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_SIN,              bcd(_T("0.5")),_T("{fn SIN(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_SQRT,             bcd(_T("2")),  _T("{fn SQRT(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_TAN,              bcd(_T("0.5")),_T("{fn TAN(fieldname)} = ?"));
      // Dual number function call
      TestFunctionNumber2(_T("fieldname"),OP_Equal,FN_MOD,              bcd(_T("12")),  bcd(_T("5")), _T("{fn MOD(fieldname,?)} = ?"));
      TestFunctionNumber2(_T("fieldname"),OP_Equal,FN_POWER,            bcd(_T("2")),   bcd(_T("64")),_T("{fn POWER(fieldname,?)} = ?"));
      TestFunctionNumber2(_T("fieldname"),OP_Equal,FN_ROUND,            bcd(_T("3.52")),bcd(_T("1")), _T("{fn ROUND(fieldname,?)} = ?"));
      TestFunctionNumber2(_T("fieldname"),OP_Equal,FN_TRUNCATE,         bcd(_T("3")),   bcd(_T("1")), _T("{fn TRUNCATE(fieldname,?)} = ?"));

      // DATE/TIME/INTERVAL

      // Constant value
      TestFunctionConstant(_T("fieldname"),OP_Smaller,FN_CURRENT_DATE,  _T("fieldname < {fn CURRENT_DATE()}"));
      TestFunctionConstant(_T("fieldname"),OP_Smaller,FN_CURDATE,       _T("fieldname < {fn CURDATE()}"));
      TestFunctionConstant(_T("fieldname"),OP_Smaller,FN_CURTIME,       _T("fieldname < {fn CURTIME()}"));
      TestFunctionConstant(_T("fieldname"),OP_Smaller,FN_NOW,           _T("fieldname < {fn NOW()}"));

      // Unary date/time function
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_DAYNAME,          _T("Sunday"),        _T("{fn DAYNAME(fieldname)} = ?"));
      TestFunctionString1(_T("fieldname"),OP_Equal,FN_MONTHNAME,        _T("June"),          _T("{fn MONTHNAME(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_DAYOFMONTH,       bcd(_T("1")),        _T("{fn DAYOFMONTH(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_DAYOFWEEK,        bcd(_T("1")),        _T("{fn DAYOFWEEK(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_DAYOFYEAR,        bcd(_T("172")),      _T("{fn DAYOFYEAR(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_QUARTER,          bcd(_T("2")),        _T("{fn QUARTER(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_YEAR,             bcd(_T("2019")),     _T("{fn YEAR(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_MONTH,            bcd(_T("6")),        _T("{fn MONTH(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_WEEK,             bcd(_T("23")),       _T("{fn WEEK(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_HOUR,             bcd(_T("17")),       _T("{fn HOUR(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_MINUTE,           bcd(_T("53")),       _T("{fn MINUTE(fieldname)} = ?"));
      TestFunctionNumber1(_T("fieldname"),OP_Equal,FN_SECOND,           bcd(_T("12")),       _T("{fn SECOND(fieldname)} = ?"));

      // Secondary date/time functions
      TestFunctionExtract(_T("fieldname"),OP_Equal,FN_EXTRACT,TS_EXT_YEAR,  _T("2019"),_T("{fn EXTRACT(YEAR FROM fieldname)} = ?"));
      TestFunctionExtract(_T("fieldname"),OP_Equal,FN_EXTRACT,TS_EXT_MONTH,    _T("6"),_T("{fn EXTRACT(MONTH FROM fieldname)} = ?"));
      TestFunctionExtract(_T("fieldname"),OP_Equal,FN_EXTRACT,TS_EXT_DAY,     _T("21"),_T("{fn EXTRACT(DAY FROM fieldname)} = ?"));
      TestFunctionExtract(_T("fieldname"),OP_Equal,FN_EXTRACT,TS_EXT_HOUR,    _T("19"),_T("{fn EXTRACT(HOUR FROM fieldname)} = ?"));
      TestFunctionExtract(_T("fieldname"),OP_Equal,FN_EXTRACT,TS_EXT_MINUTE,  _T("55"),_T("{fn EXTRACT(MINUTE FROM fieldname)} = ?"));
      TestFunctionExtract(_T("fieldname"),OP_Equal,FN_EXTRACT,TS_EXT_SECOND,  _T("12"),_T("{fn EXTRACT(SECOND FROM fieldname)} = ?"));

      // Ternary date/time functions
      TestFunctionTSCalc(_T("fieldname"),OP_Smaller,FN_TIMESTAMPADD,SQL_TSI_YEAR,   3,_T("2019-06-01"),_T("{fn TIMESTAMPADD(SQL_TSI_YEAR,?,fieldname)} < ?"));
      TestFunctionTSCalc(_T("fieldname"),OP_Smaller,FN_TIMESTAMPADD,SQL_TSI_MONTH,  3,_T("2019-06-01"),_T("{fn TIMESTAMPADD(SQL_TSI_MONTH,?,fieldname)} < ?"));
      TestFunctionTSCalc(_T("fieldname"),OP_Smaller,FN_TIMESTAMPADD,SQL_TSI_DAY,    3,_T("2019-06-01"),_T("{fn TIMESTAMPADD(SQL_TSI_DAY,?,fieldname)} < ?"));
      TestFunctionTSCalc(_T("fieldname"),OP_Smaller,FN_TIMESTAMPADD,SQL_TSI_HOUR,   3,_T("2019-06-01"),_T("{fn TIMESTAMPADD(SQL_TSI_HOUR,?,fieldname)} < ?"));
      TestFunctionTSCalc(_T("fieldname"),OP_Smaller,FN_TIMESTAMPADD,SQL_TSI_MINUTE, 3,_T("2019-06-01"),_T("{fn TIMESTAMPADD(SQL_TSI_MINUTE,?,fieldname)} < ?"));
      TestFunctionTSCalc(_T("fieldname"),OP_Smaller,FN_TIMESTAMPADD,SQL_TSI_SECOND, 3,_T("2019-06-01"),_T("{fn TIMESTAMPADD(SQL_TSI_SECOND,?,fieldname)} < ?"));

      // SYSTEM FUNCTIONS

      TestFunctionConstant(_T("'testing'"),OP_Equal,FN_DATABASE,        _T("'testing' = {fn DATABASE()}"));
      TestFunctionConstant(_T("'sysdba'"), OP_Equal,FN_USER,            _T("'sysdba' = {fn USER()}"));
      TestFunctionString2 (_T("fieldname"),OP_Equal,FN_IFNULL,          _T("other"), _T("other"), _T("{fn IFNULL(fieldname,?)} = ?"));
    }

    void TestFunctionConstant(XString p_field,SQLOperator p_oper,SQLFunction p_function,XString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      filter.SetFunction(p_function);
      XString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage(_T("Filter with constant: ") + condition);
      Assert::AreEqual(p_expect.GetString(), condition.GetString());
      ++number_of_tests;
    }

    void TestFunctionString1(XString p_field,SQLOperator p_oper,SQLFunction p_function,XString p_value,XString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      SQLVariant val(p_value);
      filter.SetFunction(p_function);
      filter.AddValue(&val);
      XString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage(_T("String function filter: ") + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      ++number_of_tests;
    }

    void TestFunctionString2(XString p_field,SQLOperator p_oper,SQLFunction p_function,XString p_value1,XString p_value2,XString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      SQLVariant val1(p_value1);
      SQLVariant val2(p_value2);
      filter.SetFunction(p_function);
      filter.AddValue(&val1);
      filter.AddValue(&val2);
      XString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage(_T("String function filter: ") + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      ++number_of_tests;
    }

    void TestFunctionString3(XString p_field,SQLOperator p_oper,SQLFunction p_function
                            ,XString p_value1,XString p_value2,XString p_value3
                            ,XString p_expect)
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
      XString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage(_T("String function filter: ") + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      ++number_of_tests;
    }

    void TestFunctionString4(XString p_field,SQLOperator p_oper,SQLFunction p_function
                            ,XString p_value1,XString p_value2,XString p_value3,XString p_value4
                            ,XString p_expect)
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
      XString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage(_T("String function filter: ") + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      ++number_of_tests;
    }

    void TestFunctionNumber1(XString p_field,SQLOperator p_oper,SQLFunction p_function,bcd p_number,XString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      SQLVariant val1(&p_number);
      filter.SetFunction(p_function);
      filter.AddValue(&val1);
      XString  condition = filter.GetSQLFilter(query);

      Logger::WriteMessage(_T("Number function filter: ") + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      ++number_of_tests;
    }

    void TestFunctionNumber2(XString p_field,SQLOperator p_oper,SQLFunction p_function,bcd p_number1,bcd p_number2,XString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      SQLVariant val1(&p_number1);
      SQLVariant val2(&p_number2);
      filter.SetFunction(p_function);
      filter.AddValue(&val1);
      filter.AddValue(&val2);
      XString  condition = filter.GetSQLFilter(query);

      Logger::WriteMessage(_T("Number function filter: ") + condition);
      Assert::AreEqual(p_expect.GetString(),condition.GetString());
      ++number_of_tests;
    }

    void TestFunctionExtract(XString p_field,SQLOperator p_oper,SQLFunction p_function,SQLExtractPart p_part,XString p_value,XString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field,p_oper);
      SQLVariant val1(&p_value);
      filter.SetFunction(p_function);
      filter.SetExtractPart(p_part);
      filter.AddValue(&val1);
      XString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage(_T("EXTRACT function filter: ") + condition);
      Assert::AreEqual(p_expect.GetString(), condition.GetString());
      ++number_of_tests;
    }

    void TestFunctionTSCalc(XString p_field,SQLOperator p_oper,SQLFunction p_function,SQLTimestampCalcPart p_part,int p_add,XString p_value,XString p_expect)
    {
      SQLQuery query;
      SQLFilter filter(p_field, p_oper);
      SQLVariant val1(p_add);
      SQLVariant val2(&p_value);
      filter.SetFunction(p_function);
      filter.SetTimestampPart(p_part);
      filter.AddValue(&val1);
      filter.AddValue(&val2);
      XString condition = filter.GetSQLFilter(query);

      Logger::WriteMessage(_T("TIMSTAMP ADD/DIFF function filter: ") + condition);
      Assert::AreEqual(p_expect.GetString(), condition.GetString());
      ++number_of_tests;
    }

  };
}

