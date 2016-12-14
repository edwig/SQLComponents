////////////////////////////////////////////////////////////////////////
//
// File: CompareTest.cpp
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
// Last Revision:   14-12-2016
// Version number:  1.3.0
//
#include "stdafx.h"
#include "CppUnitTest.h"
#include "SQLVariant.h"
#include "SQLVariantOperator.h"
#include "SQLDate.h"
#include "SQLTime.h"
#include "SQLTimestamp.h"
#include "SQLInterval.h"
#include "bcd.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OperatorUnitTest
{
  SQLVariant variants[CT_LAST];
  SQLVariant other   [CT_LAST];

  TEST_CLASS(SQLVariantCompareTest)
  {
  public:
    TEST_METHOD(CompareTesting)
    {
      // Method does no test, just fills the value arrays
      Logger::WriteMessage("In unit test comparison operators");
    }

    void FillArrays()
    {
      bcd num1(42);
      bcd num2(24);

      // Init the test array
      variants[CT_CHAR]     = SQLVariant("42");
      variants[CT_SSHORT]   = SQLVariant((short)42);
      variants[CT_USHORT]   = SQLVariant((unsigned short)42);
      variants[CT_SLONG]    = SQLVariant((long)42);
      variants[CT_ULONG]    = SQLVariant((unsigned long)42);
      variants[CT_FLOAT]    = SQLVariant((float)42.0);
      variants[CT_DOUBLE]   = SQLVariant((double)42.0);
      variants[CT_BIT]      = SQLVariant(true);
      variants[CT_STINYINT] = SQLVariant((char)42);
      variants[CT_UTINYINT] = SQLVariant((unsigned char)42);
      variants[CT_SBIGINT]  = SQLVariant((__int64)42);
      variants[CT_UBIGINT]  = SQLVariant((unsigned __int64)42);
      variants[CT_NUMERIC]  = SQLVariant(&num1);

      other[CT_CHAR]     = SQLVariant("24");
      other[CT_SSHORT]   = SQLVariant((short)24);
      other[CT_USHORT]   = SQLVariant((unsigned short)24);
      other[CT_SLONG]    = SQLVariant((long)24);
      other[CT_ULONG]    = SQLVariant((unsigned long)24);
      other[CT_FLOAT]    = SQLVariant((float)24.0);
      other[CT_DOUBLE]   = SQLVariant((double)24.0);
      other[CT_BIT]      = SQLVariant(false);
      other[CT_STINYINT] = SQLVariant((char)24);
      other[CT_UTINYINT] = SQLVariant((unsigned char)24);
      other[CT_SBIGINT]  = SQLVariant((__int64)24);
      other[CT_UBIGINT]  = SQLVariant((unsigned __int64)24);
      other[CT_NUMERIC]  = SQLVariant(&num2);
    }

    TEST_METHOD(CompareEqualityTest)
    {
      Logger::WriteMessage("In unit test equality operators");
      FillArrays();

      for(int x = CT_CHAR; x <= CT_NUMERIC; ++x)
      {
        for(int y = CT_CHAR; y <= CT_NUMERIC; ++y)
        {
          SQLVariant* left  = &variants[x];
          SQLVariant* right = &variants[y];

          CString leftType  = left ->FindDatatype(left ->GetDataType());
          CString rightType = right->FindDatatype(right->GetDataType());
          CString message;
          message.Format("Equality test %s == %s",leftType,rightType);
          Logger::WriteMessage(message);

          Assert::IsTrue(*left == *right);  // TEST
        }
      }
    }

    TEST_METHOD(CompareInequalityTest)
    {
      Logger::WriteMessage("In unit test inequality operators");
      FillArrays();

      for(int x = CT_CHAR; x <= CT_NUMERIC; ++x)
      {
        for(int y = CT_CHAR; y <= CT_NUMERIC; ++y)
        {
          // Cannot test this case true == 24 / true == 42 :-)
          if(x == CT_BIT)
          {
            continue;
          }

          SQLVariant* left  = &variants[x];
          SQLVariant* right = &other[y];

          CString leftType  = left ->FindDatatype(left ->GetDataType());
          CString rightType = right->FindDatatype(right->GetDataType());
          CString message;
          message.Format("InEquality test %s != %s",leftType,rightType);
          Logger::WriteMessage(message);

          Assert::IsTrue(*left != *right);  // TEST
        }
      }
    }

    TEST_METHOD(CompareGreaterTest)
    {
      Logger::WriteMessage("In unit test greater than operators");
      FillArrays();

      for(int x = CT_CHAR; x <= CT_NUMERIC; ++x)
      {
        for(int y = CT_CHAR; y <= CT_NUMERIC; ++y)
        {
          // Cannot test this case true == 24 / true == 42 :-)
          if(x == CT_BIT)
          {
            continue;
          }

          SQLVariant* left  = &variants[x];
          SQLVariant* right = &other[y];

          CString leftType  = left ->FindDatatype(left ->GetDataType());
          CString rightType = right->FindDatatype(right->GetDataType());
          CString message;
          message.Format("InEquality test %s > %s",leftType,rightType);
          Logger::WriteMessage(message);

          Assert::IsTrue(*left > *right);  // TEST
        }
      }
    }

    TEST_METHOD(CompareGreaterEqualTest)
    {
      Logger::WriteMessage("In unit test greater than or equal operators");
      FillArrays();

      for(int x = CT_CHAR; x <= CT_NUMERIC; ++x)
      {
        for(int y = CT_CHAR; y <= CT_NUMERIC; ++y)
        {
          // Cannot test this case true == 24 / true == 42 :-)
          if(x == CT_BIT)
          {
            continue;
          }

          SQLVariant* left  = &variants[x];
          SQLVariant* right = &other[y];

          CString leftType  = left ->FindDatatype(left ->GetDataType());
          CString rightType = right->FindDatatype(right->GetDataType());
          CString message;
          message.Format("InEquality test %s >= %s",leftType,rightType);
          Logger::WriteMessage(message);

          Assert::IsTrue(*left >= *right);  // GREATER
          Assert::IsTrue(*left >= *left);   // EQUAL
        }
      }
    }

    TEST_METHOD(CompareSmallerTest)
    {
      Logger::WriteMessage("In unit test smaller than operators");
      FillArrays();

      for(int y = CT_CHAR; y <= CT_NUMERIC; ++y)
      {
        for(int x = CT_CHAR; x <= CT_NUMERIC; ++x)
        {
          // Cannot test this case true == 24 / true == 42 :-)
          if(x == CT_BIT)
          {
            continue;
          }

          SQLVariant* left  = &variants[x];
          SQLVariant* right = &other[y];

          CString leftType  = left ->FindDatatype(left ->GetDataType());
          CString rightType = right->FindDatatype(right->GetDataType());
          CString message;
          message.Format("InEquality test %s < %s",rightType,leftType);
          Logger::WriteMessage(message);

          Assert::IsTrue(*right < *left);  // SMALLER
        }
      }
    }

    TEST_METHOD(CompareSmallerEqualTest)
    {
      Logger::WriteMessage("In unit test smaller than or equal operators");
      FillArrays();

      for(int y = CT_CHAR; y <= CT_NUMERIC; ++y)
      {
        for(int x = CT_CHAR; x <= CT_NUMERIC; ++x)
        {
          // Cannot test this case in this way: true == 24 / true == 42 :-)
          if(x == CT_BIT)
          {
            continue;
          }

          SQLVariant* left = &variants[x];
          SQLVariant* right = &other[y];

          CString leftType = left->FindDatatype(left->GetDataType());
          CString rightType = right->FindDatatype(right->GetDataType());
          CString message;
          message.Format("InEquality test %s <= %s",rightType,leftType);
          Logger::WriteMessage(message);

          Assert::IsTrue(*right < *left);  // SMALLER
          Assert::IsTrue(*left <= *left);  // SMALLER/EQUAL
        }
      }
    }
  };
}