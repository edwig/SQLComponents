////////////////////////////////////////////////////////////////////////
//
// File: CalculateTest.cpp
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
#include "SQLVariant.h"
#include "SQLVariantOperator.h"
#include "SQLDate.h"
#include "SQLTime.h"
#include "SQLTimestamp.h"
#include "SQLInterval.h"
#include "bcd.h"
#include "UnitTest.h"

namespace OperatorUnitTest
{
  extern SQLVariant variants[];
  extern SQLVariant other   [];

  TEST_CLASS(SQLVariantCalculateTest)
  {
  public:
    TEST_METHOD(CalculateTesting)
    {
      // Method does no test, just fills the value arrays
      Logger::WriteMessage("In unit test calculation operators");
    }

    void FillArrays()
    {
      bcd num1((long)42);
      bcd num2((long)5);

      // Init the test array
      variants[CT_CHAR]     = SQLVariant("42");
      variants[CT_SSHORT]   = SQLVariant((short)42);
      variants[CT_USHORT]   = SQLVariant((unsigned short)42);
      variants[CT_SLONG]    = SQLVariant((long)42);
      variants[CT_ULONG]    = SQLVariant((unsigned int)42);
      variants[CT_FLOAT]    = SQLVariant((float)42.0);
      variants[CT_DOUBLE]   = SQLVariant((double)42.0);
      variants[CT_BIT]      = SQLVariant(true);
      variants[CT_STINYINT] = SQLVariant((char)42);
      variants[CT_UTINYINT] = SQLVariant((unsigned char)42);
      variants[CT_SBIGINT]  = SQLVariant((__int64)42);
      variants[CT_UBIGINT]  = SQLVariant((unsigned __int64)42);
      variants[CT_NUMERIC]  = SQLVariant(&num1);

      other[CT_CHAR]     = SQLVariant("5");
      other[CT_SSHORT]   = SQLVariant((short)5);
      other[CT_USHORT]   = SQLVariant((unsigned short)5);
      other[CT_SLONG]    = SQLVariant((long)5);
      other[CT_ULONG]    = SQLVariant((unsigned int)5);
      other[CT_FLOAT]    = SQLVariant((float)5);
      other[CT_DOUBLE]   = SQLVariant((double)5);
      other[CT_BIT]      = SQLVariant(true);
      other[CT_STINYINT] = SQLVariant((char)5);
      other[CT_UTINYINT] = SQLVariant((unsigned char)5);
      other[CT_SBIGINT]  = SQLVariant((__int64)5);
      other[CT_UBIGINT]  = SQLVariant((unsigned __int64)5);
      other[CT_NUMERIC]  = SQLVariant(&num2);
    }

    void FreeArrays()
    {
      for(int x = CT_CHAR; x <= CT_NUMERIC; ++x)
      {
        variants[x].Init();
        other[x].Init();
      }
    }

    TEST_METHOD(CalculateAddTest)
    {
      Logger::WriteMessage("In unit test add operators");
      FillArrays();

      for(int x = CT_CHAR; x <= CT_NUMERIC; ++x)
      {
        for(int y = CT_CHAR; y <= CT_NUMERIC; ++y)
        {
          if(x == CT_BIT || y == CT_BIT)
          {
            continue;
          }

          SQLVariant* left  = &variants[x];
          SQLVariant* right = &other[y];

          CString leftType  = left ->FindDatatype(left ->GetDataType());
          CString rightType = right->FindDatatype(right->GetDataType());
          CString message;
          message.Format("Add test %s + %s",leftType,rightType);
          Logger::WriteMessage(message);

          SQLVariant result = *left + *right; // TEST
          CString res;
          result.GetAsString(res);

          if(x == CT_CHAR && y == CT_CHAR)
          {
            Assert::AreEqual("425",res);  // Adding strings
          }
          else
          {
            Assert::AreEqual("47",res);   // Adding values
          }
          number_of_tests++;
        }
      }
      FreeArrays();
    }

    TEST_METHOD(CalculateSubtractTest)
    {
      Logger::WriteMessage("In unit test subtraction operators");
      FillArrays();
      int numtests = 0;

      for(int x = CT_CHAR; x <= CT_NUMERIC; ++x)
      {
        for(int y = CT_CHAR; y <= CT_NUMERIC; ++y)
        {
          if(x == CT_BIT || y == CT_BIT)
          {
            continue;
          }

          SQLVariant* left  = &variants[x];
          SQLVariant* right = &other[y];

          CString leftType  = left->FindDatatype(left->GetDataType());
          CString rightType = right->FindDatatype(right->GetDataType());
          CString message;
          message.Format("Subtraction test %s - %s",leftType,rightType);
          Logger::WriteMessage(message);

          if(x == CT_CHAR && y == CT_CHAR)
          {
            // Subtraction not implemented!
          }
          else
          {
            ++numtests;
            SQLVariant result = *left - *right; // TEST
            CString res;
            result.GetAsString(res);

            Assert::AreEqual("37",res);   // Adding values
            number_of_tests++;
          }
        }
      }
      FreeArrays();

      CString msg;
      msg.Format("Number of subtraction tests: %d",numtests);
      Logger::WriteMessage(msg);
    }

    TEST_METHOD(CalculateMultiplyTest)
    {
      Logger::WriteMessage("In unit test multiplication operators");
      FillArrays();

      for(int x = CT_CHAR; x <= CT_NUMERIC; ++x)
      {
        for(int y = CT_CHAR; y <= CT_NUMERIC; ++y)
        {
          if(x == CT_BIT || y == CT_BIT)
          {
            continue;
          }

          SQLVariant* left = &variants[x];
          SQLVariant* right = &other[y];

          CString leftType  = left->FindDatatype(left->GetDataType());
          CString rightType = right->FindDatatype(right->GetDataType());
          CString message;
          message.Format("Multiplication test %s * %s",leftType,rightType);
          Logger::WriteMessage(message);

          if(x == CT_CHAR && y == CT_CHAR ||
             x == CT_STINYINT || y == CT_STINYINT)
          {
            // Subtraction not implemented!
          }
          else
          {
            SQLVariant result = *left * *right; // TEST
            CString res;
            result.GetAsString(res);

            Assert::AreEqual("210",res);   // Adding values
            number_of_tests++;
          }
        }
      }
      FreeArrays();
    }

    TEST_METHOD(CalculateDivisionTest)
    {
      Logger::WriteMessage("In unit test division operators");
      FillArrays();

      for(int x = CT_CHAR; x <= CT_NUMERIC; ++x)
      {
        for(int y = CT_CHAR; y <= CT_NUMERIC; ++y)
        {
          if(x == CT_BIT || y == CT_BIT)
          {
            continue;
          }

          SQLVariant* left  = &variants[x];
          SQLVariant* right = &other[y];

          CString leftType  = left ->FindDatatype(left ->GetDataType());
          CString rightType = right->FindDatatype(right->GetDataType());
          CString message;
          message.Format("Division test %s / %s",leftType,rightType);
          Logger::WriteMessage(message);

          if(x == CT_CHAR && y == CT_CHAR)
          {
            // Subtraction not implemented!
          }
          else
          {
            SQLVariant result = *left / *right; // TEST
            CString res;
            result.GetAsString(res);
            res.TrimRight('0');

            if(res.Compare("8") && res.Compare("8.4"))
            {
              Assert::Fail(L"Division error");
            }
            number_of_tests++;
          }
        }
      }
      FreeArrays();
    }

    TEST_METHOD(CalculateModuloTest)
    {
      Logger::WriteMessage("In unit test modulo operators");
      FillArrays();

      for(int x = CT_CHAR; x <= CT_NUMERIC; ++x)
      {
        for(int y = CT_CHAR; y <= CT_NUMERIC; ++y)
        {
          if(x == CT_BIT || y == CT_BIT)
          {
            continue;
          }

          SQLVariant* left  = &variants[x];
          SQLVariant* right = &other[y];

          CString leftType  = left ->FindDatatype(left ->GetDataType());
          CString rightType = right->FindDatatype(right->GetDataType());
          CString message;
          message.Format("Modulo test %s %% %s",leftType,rightType);
          Logger::WriteMessage(message);

          if(x == CT_CHAR && y == CT_CHAR)
          {
            // Subtraction not implemented!
          }
          else
          {
            SQLVariant result = *left % *right; // TEST
            CString res;
            result.GetAsString(res);
            res.TrimRight('0');
            res.TrimRight('.');

            Assert::AreEqual("2",res);
            number_of_tests++;
          }
        }
      }
      FreeArrays();
    }
  };
}
