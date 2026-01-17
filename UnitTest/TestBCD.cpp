////////////////////////////////////////////////////////////////////////
//
// File: TestBCD.cpp
//
// Created: 1998-2025 ir. W.E. Huisman
// MIT License
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
#include <HPFCounter.h>
#include <bcd.h>
#include "UnitTest.h"

namespace DatabaseUnitTest
{
  double delta = 0.0;

  typedef enum _testoperator
  {
     Operator_add
    ,Operator_minus
    ,Operator_mult
    ,Operator_div
    ,Operator_mod
  }
  TestOperator;

  const TCHAR* operators[] =
  {
     _T("adding")
    ,_T("subtracting")
    ,_T("multiplication")
    ,_T("division")
    ,_T("modulo")
  };

  typedef enum _testfunc
  {
     Func_sine
    ,Func_cosine
    ,Func_tangent
    ,Func_asin
    ,Func_acos
    ,Func_atan
    ,Func_atan2
    ,Func_floor
    ,Func_ceiling
    ,Func_fabs
    ,Func_sqrt
    ,Func_log10
    ,Func_log
    ,Func_exp
    ,Func_pow
    ,Func_modf
    ,Func_fmod
    ,Func_frexp
    ,Func_ldexp
  }
  TestFunction;

  const TCHAR* functions[] =
  {
     _T("sine")
    ,_T("cosine")
    ,_T("tangent")
    ,_T("arcsine")
    ,_T("arccosine")
    ,_T("arctangent")
    ,_T("tangens2points")
    ,_T("floor")
    ,_T("ceiling")
    ,_T("absolute")
    ,_T("squareroot")
    ,_T("log10")
    ,_T("log")
    ,_T("exp")
    ,_T("power")
    ,_T("splitfloat")
    ,_T("modulo")
    ,_T("mantissa-exponent split")
    ,_T("mult-2-power")
    ,NULL
  };

  TEST_CLASS(TestBCD)
  {
    TEST_METHOD(TotalBCDTest)
    {
      Logger::WriteMessage(_T("TESTPROGRAM EXACT NUMERICS WITH LARGE PRECISION"));
      Logger::WriteMessage(_T("==============================================="));
      Logger::WriteMessage(_T(""));
      Logger::WriteMessage(_T("Legenda:"));
      Logger::WriteMessage(_T("--------"));
      Logger::WriteMessage(_T("calc   -> Calculations in MS-Calc (standard calculator)"));
      Logger::WriteMessage(_T("double -> Build in C++ datatype 'double'"));
      Logger::WriteMessage(_T("bcd    -> Datatype 'Binairy Coded Decimal' of Edwig Huisman"));
      Logger::WriteMessage(_T(""));

      int count = 100;

      Calibrate(count);

      // Constants
      PrintConstants(count);

      // Operators
      TestOperatoren(count);

      // Functions
      TestFuncties(count);

      // SQL DECIMAL/NUMERIC conversions
      TestNumeric(count);
    }

    // Calibrating the measurement tolerance (delta)
    // Time spent in the loop without doing the actual measurement
    void Calibrate(int p_count)
    {
      int p = 0;
      bcd num;
      HPFCounter count;
      for(int x = 0; x < p_count; ++x)
      {
        switch(p)
        {
          case 0: num = 0; break;
        }
      }
      count.Stop();
      delta = count.GetCounter();

      XString msg;
      msg.Format(_T("Calibrating delta = %.6f"),delta);
      Logger::WriteMessage(msg);
    }

    void PrintConstants(int p_count)
    {
      bcd  c_pi;
      bcd  c_ln10;
      bcd  c_ln2;
      XString msg;

      msg.Format(_T("Floating point constants in [%d] iterations are:"),p_count);
      Logger::WriteMessage(msg);
      Logger::WriteMessage(_T("Constant Type   Time     Value"));
      Logger::WriteMessage(_T("-------  ------ -------- ---------------------------------------------"));

      // Calculate PI
      HPFCounter count3;
      for(int x = 0; x < p_count; ++x)
      {
        c_pi = bcd::PI();
      }
      count3.Stop();
      Logger::WriteMessage(_T("PI       Calc   0.000000 +3,1415926535897932384626433832795"));
      msg.Format          (_T("         bcd    %0.6f %s"),count3.GetCounter(),c_pi.AsString(bcd::Format::Bookkeeping,true));
      Logger::WriteMessage(msg);

      // BEREKEN LN(10)
      count3.Start();
      for(int x = 0; x < p_count; ++x)
      {
        c_ln10 = bcd::LN10();
      }
      count3.Stop();
      Logger::WriteMessage(_T("LN10     calc   0.000000 +2,3025850929940456840179914546844"));
      msg.Format          (_T("         bcd    %0.6f %s"),count3.GetCounter(),c_ln10.AsString(bcd::Format::Bookkeeping,true));
      Logger::WriteMessage(msg);

      // BEREKEN LN(2)
      count3.Start();
      for(int x = 0; x < p_count; ++x)
      {
        c_ln2 = bcd::LN2();
      }
      count3.Stop();
      Logger::WriteMessage(_T("LN2      calc   0.000000 +0,69314718055994530941723212145818"));
      msg.Format          (_T("         bcd    %0.6f %s"),count3.GetCounter(),c_ln2.AsString(bcd::Format::Bookkeeping,true));
      Logger::WriteMessage(msg);
      Logger::WriteMessage(_T(""));

      number_of_tests += 4;
    }

    void TestOperatoren(int p_count)
    {
      const TCHAR* a_big1      = _T("1234567890123456.1234567890123456");
      const TCHAR* a_big2      =                _T("5.1234567890123456");
      const TCHAR* a_big3      =  _T("876543210876543.21087654321087654321");
      const TCHAR* r_big_mult  = _T("6325255238149668.8052126159533604");
      const TCHAR* r_big_div   =  _T("240963853305269.14298709106993387");
      const TCHAR* r_big_mod   =                _T("0.7325881824833792");

      const TCHAR* a_small1    = _T("0.8347366295099261173476093153791068840878");
      const TCHAR* a_small2    = _T("0.5506493978516714425340844117564672940305");
      const TCHAR* r_small_add = _T("1.5159130887395963684392407742872");
      const TCHAR* r_small_div = _T("2111111100999999,3343333322232221");
      const TCHAR* r_big_min   =  _T("358024679246912,91258024580146906");

      DoOperatorTest(Operator_mult, a_big1,   a_big2,   r_big_mult,   p_count);
      DoOperatorTest(Operator_div,  a_big1,   a_big2,   r_big_div,    p_count);
      DoOperatorTest(Operator_add,  a_big1,   a_big3,   r_small_add,  p_count);
      DoOperatorTest(Operator_minus,a_big1,   a_big3,   r_big_min,    p_count);
      DoOperatorTest(Operator_mod,  a_big1,   a_big2,   r_big_mod,    p_count);
      DoOperatorTest(Operator_div,  a_small1, a_small2, r_small_div,  p_count);
    }

    void DoOperatorTest(TestOperator p_operator,const TCHAR* p_een,const TCHAR* p_two,const TCHAR* p_expect,int p_count)
    {
      XString msg;
      const TCHAR* name = operators[p_operator];

      msg.Format(_T("Testing [%s] for a total of [%d] iterations:"),name,p_count);
      Logger::WriteMessage(msg);

      Logger::WriteMessage(_T("Type         Time Value"));
      Logger::WriteMessage(_T("------ ---------- ------------------------------------------------------"));
      msg.Format(_T("calc     0.000000 +%s"),p_expect);
      Logger::WriteMessage(msg);

      // DOUBLE
      double d_number1 = _ttof(p_een);
      double d_number2 = _ttof(p_two);
      double d_result;

      HPFCounter counter1;
      for(int x = 0;x < p_count; ++x)
      {
        switch(p_operator)
        {
          case Operator_add:   d_result = d_number1 + d_number2; break;
          case Operator_minus: d_result = d_number1 - d_number2; break;
          case Operator_mult:  d_result = d_number1 * d_number2; break;
          case Operator_div:   d_result = d_number1 / d_number2; break;
          case Operator_mod:   d_result = fmod(d_number1,d_number2); break;
        }
        number_of_tests++;
      }
      counter1.Stop();
      msg.Format(_T("double %10.6f +%.15f"),counter1.GetCounter(),d_result);
      Logger::WriteMessage(msg);

      bcd c_number1(p_een);
      bcd c_number2(p_two);
      bcd c_result;

      HPFCounter counter5;
      for(int x = 0;x < p_count; ++x)
      {
        switch(p_operator)
        {
          case Operator_add:    c_result = c_number1 + c_number2; break;
          case Operator_minus:  c_result = c_number1 - c_number2; break;
          case Operator_mult:   c_result = c_number1 * c_number2; break;
          case Operator_div:    c_result = c_number1 / c_number2; break;
          case Operator_mod:    c_result = c_number1 % c_number2; break;
        }
        number_of_tests++;
      }
      counter5.Stop();
      msg.Format(_T("bcd    %10.6f %s"),counter5.GetCounter(),c_result.AsString(bcd::Format::Bookkeeping,true));
      Logger::WriteMessage(msg);
    }

    void TestFuncties(int p_count)
    {
      const TCHAR* a_angle     = _T("0.9876543210123456");
      const TCHAR* r_sine      = _T("0,83473662950992611734760931537911");
      const TCHAR* r_cosine    = _T("0,55064939785167144253408441175647");
      const TCHAR* r_tangent   = _T("1,5159130887395963684392407742872");

      const TCHAR* a_ratio     =  _T("0.765498765404321098765");
      const TCHAR* r_asine     = _T("0,87181613107055910102494602134303");
      const TCHAR* r_acosine   = _T("0,69898019572433751820637567029672");
      const TCHAR* r_atangent  = _T("0,653346752384431270749403109172");

      const TCHAR* a_big       =  _T("98765432109876543210.123456789012345678901234567890");
      const TCHAR* a_big_n     = _T("-98765432109876543210.123456789012345678901234567890");
      const TCHAR* a_small1    =  _T("26.5566778899001122334455");
      const TCHAR* a_small2    =   _T("7.6655443322110099887766");
      const TCHAR* a_mini      =   _T("0.00000000000000077665544332211998877665544332211");
      const TCHAR* r_sqroot    =  _T("9938079900.558082311745752865316");
      const TCHAR* r_floor     =  _T("98765432109876543210");
      const TCHAR* r_ceiling   =  _T("98765432109876543211");
      const TCHAR* r_log10     =  _T("19,994604968162151965673558368195");
      const TCHAR* r_log       =  _T("46,039279339994856527044707840045");
      const TCHAR* r_exp       =  _T("341521984409,089389680737393624");
      const TCHAR* r_pow       =  _T("82616536947,2042654425347359351");

      const TCHAR* a_big1      = _T("1234567890123456.1234567890123456");
      const TCHAR* a_big2      =                _T("5.1234567890123456");
      const TCHAR* r_big_mod   =                _T("0.7325881824833792");
      const TCHAR* r_modf1     =                _T("0.1234567890123456");
      const TCHAR* r_modf2     = _T("1234567890123456");
      const TCHAR* r_frexp1    = _T("1.2345678901234561234567890123456");
      const TCHAR* r_frexp2    = _T("15");
      const TCHAR* r_frexp3    = _T("7,7665544332211998877665544332211");
      const TCHAR* r_frexp4    = _T("-16");
      const TCHAR* r_ldexp     = _T("3399.254769907214365881024");

      DoFunctionTest(Func_sine,   a_angle,     _T("0"),      r_sine,     _T("0"),         p_count);
      DoFunctionTest(Func_cosine, a_angle,     _T("0"),      r_cosine,   _T("0"),         p_count);
      DoFunctionTest(Func_tangent,a_angle,     _T("0"),      r_tangent,  _T("0"),         p_count);
      DoFunctionTest(Func_asin,   a_ratio,     _T("0"),      r_asine,    _T("0"),         p_count);
      DoFunctionTest(Func_acos,   a_ratio,     _T("0"),      r_acosine,  _T("0"),         p_count);
      DoFunctionTest(Func_atan,   a_ratio,     _T("0"),      r_atangent, _T("0"),         p_count);
      DoFunctionTest(Func_sqrt,   a_big,       _T("0"),      r_sqroot,   _T("0"),         p_count);
      DoFunctionTest(Func_floor,  a_big,       _T("0"),      r_floor,    _T("0"),         p_count);
      DoFunctionTest(Func_ceiling,a_big,       _T("0"),      r_ceiling,  _T("0"),         p_count);
      DoFunctionTest(Func_fabs,   a_big_n,     _T("0"),      a_big,      _T("0"),         p_count);
      DoFunctionTest(Func_log10,  a_big,       _T("0"),      r_log10,    _T("0"),         p_count);
      DoFunctionTest(Func_log,    a_big,       _T("0"),      r_log,      _T("0"),         p_count);
      DoFunctionTest(Func_exp,    a_small1,    _T("0"),      r_exp,      _T("0"),         p_count);
      DoFunctionTest(Func_pow,    a_small1,    a_small2, r_pow,      _T("0"),         p_count);
      DoFunctionTest(Func_modf,   a_big1,      _T("0"),      r_modf1,    r_modf2,     p_count);
      DoFunctionTest(Func_fmod,   a_big1,      a_big2,   r_big_mod,  _T("0"),         p_count);
      DoFunctionTest(Func_frexp,  a_big1,      _T("0"),      r_frexp1,   r_frexp2,    p_count);
      DoFunctionTest(Func_frexp,  a_big_n,     _T("0"),      r_frexp1,   r_frexp2,    p_count);
      DoFunctionTest(Func_frexp,  a_mini,      _T("0"),      r_frexp3,   r_frexp4,    p_count);
      DoFunctionTest(Func_ldexp,  a_small1,    a_small2, r_ldexp,    _T("0"),         p_count);
    }

    void DoFunctionTest(TestFunction p_function
                       ,const TCHAR*  p_number1
                       ,const TCHAR*  p_number2
                       ,const TCHAR*  p_expect
                       ,const TCHAR*  p_extra
                       ,int          p_count)
    {
      XString msg;
      bool extraFloat   = p_function == Func_modf  ? true : false;
      bool extraInteger = p_function == Func_frexp ? true : false;

      const TCHAR* name = functions[p_function];
      msg.Format(_T("Testing the function [%s] for a total of [%d] iterations:"),name,p_count);
      Logger::WriteMessage(msg);

      if(_tcscmp(p_number2,_T("0")))
      {
        msg.Format(_T("Input1: %s"),p_number1);
        Logger::WriteMessage(msg);
        msg.Format(_T("Input2: %s"),p_number2);
        Logger::WriteMessage(msg);
      }
      else
      {
        msg.Format(_T("Input: %s"),p_number1);
        Logger::WriteMessage(msg);
      }
      Logger::WriteMessage(_T(""));

      Logger::WriteMessage(_T("Type         Time Value"));
      Logger::WriteMessage(_T("------ ---------- ------------------------------------------------------"));
      msg.Format(_T("calc     0.000000 +%s"),p_expect);
      Logger::WriteMessage(msg);

      if(extraInteger || extraFloat)
      {
        msg.Format(_T("         0.000000 +%s"),p_extra);
        Logger::WriteMessage(msg);
      }

      // DOUBLE
      double d_number1 = _ttof(p_number1);
      double d_number2 = _ttof(p_number2);
      double d_result;
      int    exponent  = _ttoi(p_number2);
      HPFCounter counter;
      for(int x = 0; x < p_count; ++x)
      {
        switch(p_function)
        {
          case Func_sine:     d_result = sin  (d_number1);  break;
          case Func_cosine:   d_result = cos  (d_number1);  break;
          case Func_tangent:  d_result = tan  (d_number1);  break;
          case Func_asin:     d_result = asin (d_number1);  break;
          case Func_acos:     d_result = acos (d_number1);  break;
          case Func_atan:     d_result = atan (d_number1);  break;
          case Func_atan2:    d_result = atan2(d_number1,d_number2); break;
          case Func_floor:    d_result = floor(d_number1);  break;
          case Func_ceiling:  d_result = ceil (d_number1);  break;
          case Func_fabs:     d_result = fabs (d_number1);  break;
          case Func_sqrt:     d_result = sqrt (d_number1);  break;
          case Func_log10:    d_result = log10(d_number1);  break;
          case Func_log:      d_result = log  (d_number1);  break;
          case Func_exp:      d_result = exp  (d_number1);  break;
          case Func_pow:      d_result = pow  (d_number1, d_number2);  break;
          case Func_modf:     d_result = modf (d_number1,&d_number2);  break;
          case Func_fmod:     d_result = fmod (d_number1, d_number2);  break;
          case Func_frexp:    d_result = frexp(d_number1,&exponent);   break;
          case Func_ldexp:    d_result = ldexp(d_number1, exponent);   break;
          default:            _tprintf(_T("Unknown function in DoFunctionTest -> double"));
                              break;
        }
        number_of_tests++;
      }
      counter.Stop();
      msg.Format(_T("double %10.6f +%.15f"),counter.GetCounter(),d_result);
      Logger::WriteMessage(msg);

      if(extraInteger)
      {
        msg.Format(_T("                  +%d"),exponent);
        Logger::WriteMessage(msg);
      }
      if(extraFloat)
      {
        msg.Format(_T("                  +%.15f"),d_number2);
        Logger::WriteMessage(msg);
      }

      // bcd
      bcd c_number1(p_number1);
      bcd c_number2(p_number2);
      bcd c_result;
      HPFCounter counter4;
      for(int x = 0; x < p_count; ++x)
      {
        switch(p_function)
        {
          case Func_sine:     c_result = sin  (c_number1);  break;
          case Func_cosine:   c_result = cos  (c_number1);  break;
          case Func_tangent:  c_result = tan  (c_number1);  break;
          case Func_asin:     c_result = asin (c_number1);  break;
          case Func_acos:     c_result = acos (c_number1);  break;
          case Func_atan:     c_result = atan (c_number1);  break;
          case Func_atan2:    c_result = atan2(c_number1,c_number2); break;
          case Func_floor:    c_result = floor(c_number1);  break;
          case Func_ceiling:  c_result = ceil (c_number1);  break;
          case Func_fabs:     c_result = fabs (c_number1);  break;
          case Func_sqrt:     c_result = sqrt (c_number1);  break;
          case Func_log10:    c_result = log10(c_number1);  break;
          case Func_log:      c_result = log  (c_number1);  break;
          case Func_exp:      c_result = exp  (c_number1);  break;
          case Func_pow:      c_result = pow  (c_number1, c_number2);  break;
          case Func_modf:     c_result = modf (c_number1,&c_number2);  break;
          case Func_fmod:     c_result = fmod (c_number1, c_number2);  break;
          case Func_frexp:    c_result = frexp(c_number1,&exponent);   break;
          case Func_ldexp:    c_result = ldexp(c_number1, exponent);   break;
          default:            _tprintf(_T("Unknown function in DoFunctionTest -> bcd"));
                              break;
        }
        number_of_tests++;
      }
      counter4.Stop();
      msg.Format(_T("bcd    %10.6f %s"),counter4.GetCounter(),c_result.AsString(bcd::Format::Bookkeeping,true));
      Logger::WriteMessage(msg);
      if(extraInteger)
      {
        msg.Format(_T("                  +%d"),exponent);
        Logger::WriteMessage(msg);
      }
      if(extraFloat)
      {
        msg.Format(_T("                  %s"),c_number2.AsString(bcd::Format::Bookkeeping,true));
        Logger::WriteMessage(msg);
      }
    }

    int TestNumeric(int p_count)
    {
      XString msg;
      // Header
      msg.Format(_T("Testing SQL_NUMERIC_STRUCT for a total of [%d] iterations"),p_count);
      Logger::WriteMessage(msg);

      // num = 10.001 (ten and 1 thousandth)
      SQL_NUMERIC_STRUCT num;
      SQL_NUMERIC_STRUCT res;
      memset(&num,0,sizeof(SQL_NUMERIC_STRUCT));
      memset(&res,0,sizeof(SQL_NUMERIC_STRUCT));
      num.sign = 1; // Positive
      num.precision = 6;
      num.scale = 4;

      num.val[0] = 0xAA;
      num.val[1] = 0x86;
      num.val[2] = 0x01;

      HPFCounter counter;
      for(int x = 0; x < p_count; ++x)
      {
        bcd ten(&num);
        number_of_tests++;
      }
      counter.Stop();
      bcd ten(&num);
      msg.Format(_T("SQL_NUMERIC_STRUCT -> bcd %10.6f : %s"),counter.GetCounter(),ten.AsString().GetString());
      Logger::WriteMessage(msg);


      // Now back again to a SQL_NUMERIC_STRUCT
      HPFCounter cnt2;
      for(int x = 0; x < p_count; ++x)
      {
        ten.AsNumeric(&res);
        number_of_tests++;
      }
      cnt2.Stop();


      ten.AsNumeric(&res);
      //     printf("Precision: %d\n",res.precision);
      //     printf("Scale    : %d\n",res.scale);
      //     printf("Sign     : %d\n",res.sign);
      // 
      //     for(unsigned ind = 0;ind < SQL_MAX_NUMERIC_LEN; ++ind)
      //     {
      //       printf("Numeric mantissa [%d:%02.2X]",ind,res.val[ind]);
      //     }

      bcd check(&res);
      msg.Format(_T("bcd -> SQL_NUMERIC_STRUCT %10.6f : %s"),cnt2.GetCounter(),check.AsString());
      Logger::WriteMessage(msg);
      number_of_tests++;

      Logger::WriteMessage(_T(""));
      return 0;
    }

    void PrecisionTest(const XString& p_input,int p_precision,int p_scale)
    {
      SQL_NUMERIC_STRUCT numeric;
      bcd num(p_input);
      num.AsNumeric(&numeric);

      XString text;
      text.Format(_T("BCD Precision/Scale [%d:%d] for: %s"),(int)numeric.precision,(int)numeric.scale,p_input.GetString());
      Logger::WriteMessage(text);

      Assert::AreEqual((SQLCHAR) p_precision,numeric.precision);
      Assert::AreEqual((SQLSCHAR)p_scale,    numeric.scale);

      number_of_tests += 2;
    }

    TEST_METHOD(BCDPrecisionTesting)
    {
      Logger::WriteMessage(_T("Testing BCD->Numeric precision and scale"));

      PrecisionTest(_T("1.111111111111111111111111111111111111"),37,36);
      PrecisionTest(_T("11.11111111111111111111111111111111111"),37,35);
      PrecisionTest(_T("111.1111111111111111111111111111111111"),37,34);
      PrecisionTest(_T("1111.111111111111111111111111111111111"),37,33);
      PrecisionTest(_T("11111.11111111111111111111111111111111"),37,32);
      PrecisionTest(_T("111111.1111111111111111111111111111111"),37,31);
      PrecisionTest(_T("1111111.111111111111111111111111111111"),37,30);

      PrecisionTest(_T("11111.11111"),10,5);
      PrecisionTest(_T("111111.1111"),10,4);
      PrecisionTest(_T("1111111.111"),10,3);
      PrecisionTest(_T("11111111.11"),10,2);
      PrecisionTest(_T("111111111.1"),10,1);
      PrecisionTest( _T("1111111111"),10,0);

      PrecisionTest(   _T("10"),2,0);
      PrecisionTest(  _T("100"),3,0);
      PrecisionTest( _T("1000"),4,0);
      PrecisionTest(_T("10000"),5,0);

      PrecisionTest(_T("1"),1,0);
      PrecisionTest(_T("0.1"),2,1);
      PrecisionTest(_T("0.01"),3,2);
      PrecisionTest(_T("0.001"),4,3);
      PrecisionTest(_T("0.0001"),5,4);
      PrecisionTest(_T("0.00001"),6,5);
      PrecisionTest(_T("0.000001"),7,6);
      PrecisionTest(_T("0.0000001"),8,7);
    }

    TEST_METHOD(BCDDisplayString)
    {
      Logger::WriteMessage(_T("Testing BCD Display string capabilities"));

      bcd basenum(_T("123456.123456"));

      // BEWARE: I'am testing in "The Netherlands", so my locale settings are:
      // Thousand seperator: .
      // Decimal  seperator: ,
      XString test;
      test = TestDisplay(basenum,0);   Assert::AreEqual(_T("123.456"),         test.GetString());
      test = TestDisplay(basenum,1);   Assert::AreEqual(_T("123.456,1"),       test.GetString());
      test = TestDisplay(basenum,2);   Assert::AreEqual(_T("123.456,12"),      test.GetString());
      test = TestDisplay(basenum,3);   Assert::AreEqual(_T("123.456,123"),     test.GetString());
      test = TestDisplay(basenum,4);   Assert::AreEqual(_T("123.456,1235"),    test.GetString());
      test = TestDisplay(basenum,5);   Assert::AreEqual(_T("123.456,12346"),   test.GetString());
      test = TestDisplay(basenum,6);   Assert::AreEqual(_T("123.456,123456"),  test.GetString());
      test = TestDisplay(basenum,7);   Assert::AreEqual(_T("123.456,1234560"), test.GetString());
      test = TestDisplay(basenum,8);   Assert::AreEqual(_T("123.456,12345600"),test.GetString());

      basenum = bcd(_T("123"));
      test = TestDisplay(basenum,0);   Assert::AreEqual(_T("123"),    test.GetString());
      test = TestDisplay(basenum,1);   Assert::AreEqual(_T("123,0"),  test.GetString());
      test = TestDisplay(basenum,2);   Assert::AreEqual(_T("123,00"), test.GetString());
      test = TestDisplay(basenum,3);   Assert::AreEqual(_T("123,000"),test.GetString());
    }

    XString TestDisplay(bcd p_num,int precision)
    {
      bcd number(p_num);
      XString str = number.AsDisplayString(precision);
      Logger::WriteMessage(str);
      return str;
    }
  };
}
