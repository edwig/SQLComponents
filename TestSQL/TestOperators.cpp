////////////////////////////////////////////////////////////////////////
//
// File: TestOperators.cpp
//
// Copyright (c) 1998-2021 ir. W.E. Huisman
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
#include "TestSQL.h"
#include "SQLVariant.h"
#include "SQLVariantOperator.h"
#include "SQLDate.h"
#include "bcd.h"

SQLVariant variants[CT_LAST];
SQLVariant others  [CT_LAST];

void TestGreater()
{
  printf("Testing SQLVariant > SQLVariant\n");
  printf("===============================\n");

  SQLVariant var1((short)13);
  SQLVariant var2((long) 15);

  if(var1 > var2)
  {
    printf("Error in operator (short > long)\n");
  }
  else
  {
    printf("OK\n");
  }
}

void TestSub()
{
  printf("Testing SQLDate - SQLDate -> SQLInterval\n");
  printf("========================================\n");

  SQLDate date1("15-10-1959");
  SQLDate date2("20-11-2016");
  SQLVariant var1(&date1);
  SQLVariant var2(&date2);

  SQLVariant var3 = var2 - var1;
  printf("Found interval is: %s\n",var3.GetAsChar());
}

void TestEqual()
{
  printf("Testing the equality operator of SQLVariant\n");
  printf("===========================================\n");

  // Init the test array
  variants[CT_CHAR]     = SQLVariant("42");
  variants[CT_SSHORT]   = SQLVariant((short)42);
  variants[CT_USHORT]   = SQLVariant((unsigned short)42);
  variants[CT_SLONG]    = SQLVariant((int)42);
  variants[CT_ULONG]    = SQLVariant((unsigned)42);
  variants[CT_FLOAT]    = SQLVariant((float)42.0);
  variants[CT_DOUBLE]   = SQLVariant((double)42.0);
  variants[CT_BIT]      = SQLVariant(true);
  variants[CT_STINYINT] = SQLVariant((char)42);
  variants[CT_UTINYINT] = SQLVariant((unsigned char)42);
  variants[CT_SBIGINT]  = SQLVariant((__int64)42);
  variants[CT_UBIGINT]  = SQLVariant((unsigned __int64)42);

  for(int x = CT_CHAR; x <= CT_UBIGINT; ++x)
  {
    for(int y = CT_CHAR; y <= CT_UBIGINT; ++y)
    {
      SQLVariant* left  = &variants[x];
      SQLVariant* right = &variants[y];

      CString leftType  = left ->FindDatatype(left ->GetDataType());
      CString rightType = right->FindDatatype(right->GetDataType());
      CString message;
      message.Format("Equality test %s == %s ",leftType,rightType);
      printf(message);

      printf(*left == *right ? "OK\n" : "ERROR\n");
    }
  }

  // Clean out the test data
  for(int x = CT_CHAR; x <= CT_UBIGINT; ++x)
  {
    variants[x].SetData(SQL_C_LONG,"0");
  }
}

void
TestAddVar()
{
  
}

void
TestAdd()
{
  printf("Testing the add operator of SQLVariant\n");
  printf("======================================\n");

  // Init the test array
  others[CT_CHAR]     = SQLVariant("5");
  others[CT_SSHORT]   = SQLVariant((short)5);
  others[CT_USHORT]   = SQLVariant((unsigned short)5);
  others[CT_SLONG]    = SQLVariant((int)5);
  others[CT_ULONG]    = SQLVariant((unsigned)5);
  others[CT_FLOAT]    = SQLVariant((float)5);
  others[CT_DOUBLE]   = SQLVariant((double)5);
  others[CT_BIT]      = SQLVariant(true);
  others[CT_STINYINT] = SQLVariant((char)5);
  others[CT_UTINYINT] = SQLVariant((unsigned char)5);
  others[CT_SBIGINT]  = SQLVariant((__int64)5);
  others[CT_UBIGINT]  = SQLVariant((unsigned __int64)5);

  for(int x = CT_CHAR; x <= CT_UBIGINT; ++x)
  {
    for(int y = CT_CHAR; y <= CT_UBIGINT; ++y)
    {
      if(x == CT_BIT || y == CT_BIT)
      {
        continue;
      }

      SQLVariant* left  = &variants[x];
      SQLVariant* right = &others[y];

      CString leftType  = left ->FindDatatype(left ->GetDataType());
      CString rightType = right->FindDatatype(right->GetDataType());
      CString message;
      message.Format("Add test %s + %s ",leftType,rightType);
      printf(message);

      SQLVariant result(*left + *right); // TEST
      CString res;
      result.GetAsString(res);

      if(x == CT_CHAR && y == CT_CHAR)
      {
        // Adding strings
        printf(res.Compare("425") ? "ERROR\n" : "OK\n");
      }
      else
      {
        printf(res.Compare("47") ? "ERROR\n" : "OK\n");
      }
    }
  }

  // Clean out the test data
  for(int x = CT_CHAR; x <= CT_UBIGINT; ++x)
  {
    others[x].SetData(SQL_C_LONG,"0");
  }
}

void TestMul()
{
  CString numString("42");
  bcd num(5);

  SQLVariant var1(numString);
  SQLVariant var2(&num);

  SQLVariant var3 = var1 * var2;
  printf("The result of the char*decimal = %s\n",var3.GetAsChar());
}

void TestCast()
{
  printf("Testing the cast operators of SQLVariant\n");
  printf("========================================\n");

  SQLVariant varBit   ((bool) true);
  SQLVariant varSTiny ((char) -67);
  SQLVariant varUTiny ((unsigned char) 189);
  SQLVariant varSShort((short) -567);
  SQLVariant varUShort((ushort) 567);
  SQLVariant varInt   ((int) 2 * 42);
  SQLVariant varUInt  ((unsigned) 0x84556677);
  
  bool     numberBit     = varBit;
  char     numberSTiny   = varSTiny;
  uchar    numberUTiny   = varUTiny;
  short    numberSShort  = varSShort;
  ushort   numberUShort  = varUShort;
  int      numberInt     = varInt;
  unsigned numberUInt    = varUInt;

  printf("Test for correct bit cast               : %s\n",numberBit ? "true" : "false");
  printf("Test for correct signed tiny int cast   : %d\n",(int) numberSTiny);
  printf("Test for correct unsigned tiny int cast : %d\n",(int) numberUTiny);
  printf("Test for correct signed short int cast  : %d\n",(int) numberSShort);
  printf("Test for correct unsigned short int cast: %d\n",(int) numberUShort);
  printf("Test for correct int cast               : %d\n",numberInt);
  printf("Test for correct unsigned int cast      : %X\n",numberUInt);
}

void TestOperators()
{
  TestGreater();
  TestSub();
  TestEqual();
  TestAddVar();
  TestAdd();
  TestMul();
  TestCast();
}