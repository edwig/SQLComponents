////////////////////////////////////////////////////////////////////////
//
// File: TestBCD.cpp
//
// Copyright (c) 1998-2020 ir. W.E. Huisman
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
#include "HPFCounter.h"
#include "bcd.h"
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

  char* operators[] =
  {
     "adding"
    ,"subtracting"
    ,"multiplication"
    ,"division"
    ,"modulo"
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

  char* functions[] =
  {
     "sine"
    ,"cosine"
    ,"tangent"
    ,"arcsine"
    ,"arccosine"
    ,"arctangent"
    ,"tangens2points"
    ,"floor"
    ,"ceiling"
    ,"absolute"
    ,"squareroot"
    ,"log10"
    ,"log"
    ,"exp"
    ,"power"
    ,"splitfloat"
    ,"modulo"
    ,"mantissa-exponent split"
    ,"mult-2-power"
    ,NULL
  };

  TEST_CLASS(TestBCD)
  {
    TEST_METHOD(TotalBCDTest)
    {
      Logger::WriteMessage("TESTPROGRAM EXACT NUMERICS WITH LARGE PRECISION");
      Logger::WriteMessage("===============================================");
      Logger::WriteMessage("");
      Logger::WriteMessage("Legenda:");
      Logger::WriteMessage("--------");
      Logger::WriteMessage("calc   -> Calculations in MS-Calc (standard calculator)");
      Logger::WriteMessage("double -> Build in C++ datatype 'double'");
      Logger::WriteMessage("bcd    -> Datatype 'Binairy Coded Decimal' of Edwig Huisman");
      Logger::WriteMessage("");

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
          case 0: num = 0L; break;
        }
      }
      count.Stop();
      delta = count.GetCounter();

      CString msg;
      msg.Format("Calibrating delta = %.6f",delta);
      Logger::WriteMessage(msg);
    }

    void PrintConstants(int p_count)
    {
      bcd  c_pi;
      bcd  c_ln10;
      bcd  c_ln2;
      CString msg;

      msg.Format("Floating point constants in [%d] iterations are:",p_count);
      Logger::WriteMessage(msg);
      Logger::WriteMessage("Constant Type   Time     Value");
      Logger::WriteMessage("-------  ------ -------- ---------------------------------------------");

      // Calculate PI
      HPFCounter count3;
      for(int x = 0; x < p_count; ++x)
      {
        c_pi = bcd::PI();
      }
      count3.Stop();
      Logger::WriteMessage("PI       Calc   0.000000 +3,1415926535897932384626433832795");
      msg.Format          ("         bcd    %0.6f %s",count3.GetCounter(),c_pi.AsString(bcd::Bookkeeping,true));
      Logger::WriteMessage(msg);

      // BEREKEN LN(10)
      count3.Start();
      for(int x = 0; x < p_count; ++x)
      {
        c_ln10 = bcd::LN10();
      }
      count3.Stop();
      Logger::WriteMessage("LN10     calc   0.000000 +2,3025850929940456840179914546844");
      msg.Format          ("         bcd    %0.6f %s",count3.GetCounter(),c_ln10.AsString(bcd::Bookkeeping,true));
      Logger::WriteMessage(msg);

      // BEREKEN LN(2)
      count3.Start();
      for(int x = 0; x < p_count; ++x)
      {
        c_ln2 = bcd::LN2();
      }
      count3.Stop();
      Logger::WriteMessage("LN2      calc   0.000000 +0,69314718055994530941723212145818");
      msg.Format          ("         bcd    %0.6f %s",count3.GetCounter(),c_ln2.AsString(bcd::Bookkeeping,true));
      Logger::WriteMessage(msg);
      Logger::WriteMessage("");

      number_of_tests += 4;
    }

    void TestOperatoren(int p_count)
    {
      char* a_big1      = "1234567890123456.1234567890123456";
      char* a_big2      =                "5.1234567890123456";
      char* a_big3      =  "876543210876543.21087654321087654321";
      char* r_big_mult  = "6325255238149668.8052126159533604";
      char* r_big_div   =  "240963853305269.14298709106993387";
      char* r_big_mod   =                "0.7325881824833792";

      char* a_small1    = "0.8347366295099261173476093153791068840878";
      char* a_small2    = "0.5506493978516714425340844117564672940305";
      char* r_small_add = "1.5159130887395963684392407742872";
      char* r_small_div = "2111111100999999,3343333322232221";
      char* r_big_min   =  "358024679246912,91258024580146906";

      DoOperatorTest(Operator_mult, a_big1,   a_big2,   r_big_mult,   p_count);
      DoOperatorTest(Operator_div,  a_big1,   a_big2,   r_big_div,    p_count);
      DoOperatorTest(Operator_add,  a_big1,   a_big3,   r_small_add,  p_count);
      DoOperatorTest(Operator_minus,a_big1,   a_big3,   r_big_min,    p_count);
      DoOperatorTest(Operator_mod,  a_big1,   a_big2,   r_big_mod,    p_count);
      DoOperatorTest(Operator_div,  a_small1, a_small2, r_small_div,  p_count);
    }

    void DoOperatorTest(TestOperator p_operator,char* p_een,char* p_two,char* p_expect,int p_count)
    {
      CString msg;
      char* name = operators[p_operator];

      msg.Format("Testing [%s] for a total of [%d] iterations:",name,p_count);
      Logger::WriteMessage(msg);

      Logger::WriteMessage("Type         Time Value");
      Logger::WriteMessage("------ ---------- ------------------------------------------------------");
      msg.Format("calc     0.000000 +%s",p_expect);
      Logger::WriteMessage(msg);

      // DOUBLE
      double d_number1 = atof(p_een);
      double d_number2 = atof(p_two);
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
      msg.Format("double %10.6f +%.15f",counter1.GetCounter(),d_result);
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
      msg.Format("bcd    %10.6f %s",counter5.GetCounter(),c_result.AsString(bcd::Bookkeeping,true));
      Logger::WriteMessage(msg);
    }

    void TestFuncties(int p_count)
    {
      char* a_angle     = "0.9876543210123456";
      char* r_sine      = "0,83473662950992611734760931537911";
      char* r_cosine    = "0,55064939785167144253408441175647";
      char* r_tangent   = "1,5159130887395963684392407742872";

      char* a_ratio     =  "0.765498765404321098765";
      char* r_asine     = "0,87181613107055910102494602134303";
      char* r_acosine   = "0,69898019572433751820637567029672";
      char* r_atangent  = "0,653346752384431270749403109172";

      char* a_big       =  "98765432109876543210.123456789012345678901234567890";
      char* a_big_n     = "-98765432109876543210.123456789012345678901234567890";
      char* a_small1    =  "26.5566778899001122334455";
      char* a_small2    =   "7.6655443322110099887766";
      char* a_mini      =   "0.00000000000000077665544332211998877665544332211";
      char* r_sqroot    =  "9938079900.558082311745752865316";
      char* r_floor     =  "98765432109876543210";
      char* r_ceiling   =  "98765432109876543211";
      char* r_log10     =  "19,994604968162151965673558368195";
      char* r_log       =  "46,039279339994856527044707840045";
      char* r_exp       =  "341521984409,089389680737393624";
      char* r_pow       =  "82616536947,2042654425347359351";

      char* a_big1      = "1234567890123456.1234567890123456";
      char* a_big2      =                "5.1234567890123456";
      char* r_big_mod   =                "0.7325881824833792";
      char* r_modf1     =                "0.1234567890123456";
      char* r_modf2     = "1234567890123456";
      char* r_frexp1    = "1.2345678901234561234567890123456";
      char* r_frexp2    = "15";
      char* r_frexp3    = "7,7665544332211998877665544332211";
      char* r_frexp4    = "-16";
      char* r_ldexp     = "3399.254769907214365881024";

      DoFunctionTest(Func_sine,   a_angle,     "0",      r_sine,     "0",         p_count);
      DoFunctionTest(Func_cosine, a_angle,     "0",      r_cosine,   "0",         p_count);
      DoFunctionTest(Func_tangent,a_angle,     "0",      r_tangent,  "0",         p_count);
      DoFunctionTest(Func_asin,   a_ratio,     "0",      r_asine,    "0",         p_count);
      DoFunctionTest(Func_acos,   a_ratio,     "0",      r_acosine,  "0",         p_count);
      DoFunctionTest(Func_atan,   a_ratio,     "0",      r_atangent, "0",         p_count);
      DoFunctionTest(Func_sqrt,   a_big,       "0",      r_sqroot,   "0",         p_count);
      DoFunctionTest(Func_floor,  a_big,       "0",      r_floor,    "0",         p_count);
      DoFunctionTest(Func_ceiling,a_big,       "0",      r_ceiling,  "0",         p_count);
      DoFunctionTest(Func_fabs,   a_big_n,     "0",      a_big,      "0",         p_count);
      DoFunctionTest(Func_log10,  a_big,       "0",      r_log10,    "0",         p_count);
      DoFunctionTest(Func_log,    a_big,       "0",      r_log,      "0",         p_count);
      DoFunctionTest(Func_exp,    a_small1,    "0",      r_exp,      "0",         p_count);
      DoFunctionTest(Func_pow,    a_small1,    a_small2, r_pow,      "0",         p_count);
      DoFunctionTest(Func_modf,   a_big1,      "0",      r_modf1,    r_modf2,     p_count);
      DoFunctionTest(Func_fmod,   a_big1,      a_big2,   r_big_mod,  "0",         p_count);
      DoFunctionTest(Func_frexp,  a_big1,      "0",      r_frexp1,   r_frexp2,    p_count);
      DoFunctionTest(Func_frexp,  a_big_n,     "0",      r_frexp1,   r_frexp2,    p_count);
      DoFunctionTest(Func_frexp,  a_mini,      "0",      r_frexp3,   r_frexp4,    p_count);
      DoFunctionTest(Func_ldexp,  a_small1,    a_small2, r_ldexp,    "0",         p_count);
    }

    void DoFunctionTest(TestFunction p_function
                       ,char*        p_number1
                       ,char*        p_number2
                       ,char*        p_expect
                       ,char*        p_extra
                       ,int          p_count)
    {
      CString msg;
      bool extraFloat   = p_function == Func_modf  ? true : false;
      bool extraInteger = p_function == Func_frexp ? true : false;

      char* name = functions[p_function];
      msg.Format("Testing the function [%s] for a total of [%d] iterations:",name,p_count);
      Logger::WriteMessage(msg);

      if(strcmp(p_number2,"0"))
      {
        msg.Format("Input1: %s",p_number1);
        Logger::WriteMessage(msg);
        msg.Format("Input2: %s",p_number2);
        Logger::WriteMessage(msg);
      }
      else
      {
        msg.Format("Input: %s",p_number1);
        Logger::WriteMessage(msg);
      }
      Logger::WriteMessage("");

      Logger::WriteMessage("Type         Time Value");
      Logger::WriteMessage("------ ---------- ------------------------------------------------------");
      msg.Format("calc     0.000000 +%s",p_expect);
      Logger::WriteMessage(msg);

      if(extraInteger || extraFloat)
      {
        msg.Format("         0.000000 +%s",p_extra);
        Logger::WriteMessage(msg);
      }

      // DOUBLE
      double d_number1 = atof(p_number1);
      double d_number2 = atof(p_number2);
      double d_result;
      int    exponent  = atoi(p_number2);
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
          default:            printf("Unknown function in DoFunctionTest -> double");
                              break;
        }
        number_of_tests++;
      }
      counter.Stop();
      msg.Format("double %10.6f +%.15f",counter.GetCounter(),d_result);
      Logger::WriteMessage(msg);

      if(extraInteger)
      {
        msg.Format("                  +%d",exponent);
        Logger::WriteMessage(msg);
      }
      if(extraFloat)
      {
        msg.Format("                  +%.15f",d_number2);
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
          default:            printf("Unknown function in DoFunctionTest -> bcd");
                              break;
        }
        number_of_tests++;
      }
      counter4.Stop();
      msg.Format("bcd    %10.6f %s",counter4.GetCounter(),c_result.AsString(bcd::Bookkeeping,true));
      Logger::WriteMessage(msg);
      if(extraInteger)
      {
        msg.Format("                  +%d",exponent);
        Logger::WriteMessage(msg);
      }
      if(extraFloat)
      {
        msg.Format("                  %s",c_number2.AsString(bcd::Bookkeeping,true));
        Logger::WriteMessage(msg);
      }
    }

    int TestNumeric(int p_count)
    {
      CString msg;
      // Header
      msg.Format("Testing SQL_NUMERIC_STRUCT for a total of [%d] iterations",p_count);
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
      msg.Format("SQL_NUMERIC_STRUCT -> bcd %10.6f : %s",counter.GetCounter(),ten.AsString().GetString());
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
      msg.Format("bcd -> SQL_NUMERIC_STRUCT %10.6f : %s",cnt2.GetCounter(),check.AsString());
      Logger::WriteMessage(msg);
      number_of_tests++;

      Logger::WriteMessage("");
      return 0;
    }

    void PrecisionTest(CString p_input,int p_precision,int p_scale)
    {
      SQL_NUMERIC_STRUCT numeric;
      bcd num(p_input);
      num.AsNumeric(&numeric);

      CString text;
      text.Format("BCD Precision/Scale [%d:%d] for: %s",(int)numeric.precision,(int)numeric.scale,p_input);
      Logger::WriteMessage(text);

      Assert::AreEqual((SQLCHAR) p_precision,numeric.precision);
      Assert::AreEqual((SQLSCHAR)p_scale,    numeric.scale);

      number_of_tests += 2;
    }

    TEST_METHOD(BCDPrecisionTesting)
    {
      Logger::WriteMessage("Testing BCD->Numeric precision and scale");

      PrecisionTest("1.111111111111111111111111111111111111",37,36);
      PrecisionTest("11.11111111111111111111111111111111111",37,35);
      PrecisionTest("111.1111111111111111111111111111111111",37,34);
      PrecisionTest("1111.111111111111111111111111111111111",37,33);
      PrecisionTest("11111.11111111111111111111111111111111",37,32);
      PrecisionTest("111111.1111111111111111111111111111111",37,31);
      PrecisionTest("1111111.111111111111111111111111111111",37,30);

      PrecisionTest("11111.11111",10,5);
      PrecisionTest("111111.1111",10,4);
      PrecisionTest("1111111.111",10,3);
      PrecisionTest("11111111.11",10,2);
      PrecisionTest("111111111.1",10,1);
      PrecisionTest( "1111111111",10,0);

      PrecisionTest("1",1,0);
      PrecisionTest("0.1",2,1);
      PrecisionTest("0.01",3,2);
      PrecisionTest("0.001",4,3);
      PrecisionTest("0.0001",5,4);
      PrecisionTest("0.00001",6,5);
      PrecisionTest("0.000001",7,6);
      PrecisionTest("0.0000001",8,7);
    }

    TEST_METHOD(BCDDisplayString)
    {
      Logger::WriteMessage("Testing BCD Display string capabilities");

      bcd basenum("123456.123456");

      // BEWARE: I'am testing in "The Netherlands", so my locale settings are:
      // Thousand seperator: .
      // Decimal  seperator: ,
      CString test;
      test = TestDisplay(basenum,0);   Assert::AreEqual("123.456",         test.GetString());
      test = TestDisplay(basenum,1);   Assert::AreEqual("123.456,1",       test.GetString());
      test = TestDisplay(basenum,2);   Assert::AreEqual("123.456,12",      test.GetString());
      test = TestDisplay(basenum,3);   Assert::AreEqual("123.456,123",     test.GetString());
      test = TestDisplay(basenum,4);   Assert::AreEqual("123.456,1235",    test.GetString());
      test = TestDisplay(basenum,5);   Assert::AreEqual("123.456,12346",   test.GetString());
      test = TestDisplay(basenum,6);   Assert::AreEqual("123.456,123456",  test.GetString());
      test = TestDisplay(basenum,7);   Assert::AreEqual("123.456,1234560", test.GetString());
      test = TestDisplay(basenum,8);   Assert::AreEqual("123.456,12345600",test.GetString());

      basenum = bcd("123");
      test = TestDisplay(basenum,0);   Assert::AreEqual("123",    test.GetString());
      test = TestDisplay(basenum,1);   Assert::AreEqual("123,0",  test.GetString());
      test = TestDisplay(basenum,2);   Assert::AreEqual("123,00", test.GetString());
      test = TestDisplay(basenum,3);   Assert::AreEqual("123,000",test.GetString());
    }

    CString TestDisplay(bcd p_num,int precision)
    {
      bcd number(p_num);
      CString str = number.AsDisplayString(precision);
      Logger::WriteMessage(str);
      return str;
    }
  };
}
