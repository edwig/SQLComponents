////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorMul.cpp
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
// Last Revision:   11-11-2016
// Version number:  1.2.0
//
#include "stdafx.h"
#include "SQLVariant.h"
#include "SQLVariantOperator.h"
#include "SQLDate.h"

#pragma warning (disable: 4239)

CalculateFunctionArray
SQLVariant::OperatorMul[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                        CT_SSHORT                        CT_USHORT                        CT_SLONG                        CT_ULONG                        CT_FLOAT                        CT_DOUBLE                        CT_BIT                        CT_STINYINT                     CT_UTINYINT                     CT_SBIGINT                     CT_UBIGINT                     CT_NUMERIC                   CT_GUID  CT_BINARY CT_DATE  CT_TIME  CT_TIMESTAMP CT_INTERVAL_YM CT_INTERVAL_DS
                         // ------------------------------ -------------------------------- -------------------------------- ------------------------------- ------------------------------- ------------------------------- -------------------------------- ----------------------------- ------------------------------- ------------------------------- ------------------------------ ------------------------------ ---------------------------- -------- --------- -------- -------- ------------ -------------- --------------
  /* CT_CHAR        */   {  nullptr                       ,&SQLVariant::OperCharMulSShort  ,&SQLVariant::OperCharMulUShort  ,&SQLVariant::OperCharMulSLong  ,&SQLVariant::OperCharMulULong  ,&SQLVariant::OperCharMulFloat  ,&SQLVariant::OperCharMulDouble  ,&SQLVariant::OperCharMulBit  ,&SQLVariant::OperCharMulSTiny  ,&SQLVariant::OperCharMulUTiny  ,&SQLVariant::OperCharMulSBig  ,&SQLVariant::OperCharMulUBig  ,&SQLVariant::OperVarMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SSHORT      */  ,{  &SQLVariant::OperSShortMulChar,&SQLVariant::OperSShortMulSShort,&SQLVariant::OperSShortMulUShort,&SQLVariant::OperSShortMulSLong,&SQLVariant::OperSShortMulULong,&SQLVariant::OperSShortMulFloat,&SQLVariant::OperSShortMulDouble,&SQLVariant::OperSShortMulBit,&SQLVariant::OperSShortMulSTiny,&SQLVariant::OperSShortMulUTiny,&SQLVariant::OperSShortMulSBig,&SQLVariant::OperSShortMulUBig,&SQLVariant::OperVarMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_USHORT      */  ,{  &SQLVariant::OperUShortMulChar,&SQLVariant::OperUShortMulSShort,&SQLVariant::OperUShortMulUShort,&SQLVariant::OperUShortMulSLong,&SQLVariant::OperUShortMulULong,&SQLVariant::OperUShortMulFloat,&SQLVariant::OperUShortMulDouble,&SQLVariant::OperUShortMulBit,&SQLVariant::OperUShortMulSTiny,&SQLVariant::OperUShortMulUTiny,&SQLVariant::OperUShortMulSBig,&SQLVariant::OperUShortMulUBig,&SQLVariant::OperVarMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SLONG       */  ,{  &SQLVariant::OperSLongMulChar ,&SQLVariant::OperSLongMulSShort ,&SQLVariant::OperSLongMulUShort ,&SQLVariant::OperSLongMulSLong ,&SQLVariant::OperSLongMulULong ,&SQLVariant::OperSLongMulFloat ,&SQLVariant::OperSLongMulDouble ,&SQLVariant::OperSLongMulBit ,&SQLVariant::OperSLongMulSTiny ,&SQLVariant::OperSLongMulUTiny ,&SQLVariant::OperSLongMulSBig ,&SQLVariant::OperSLongMulUBig ,&SQLVariant::OperVarMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_ULONG       */  ,{  &SQLVariant::OperULongMulChar ,&SQLVariant::OperULongMulSShort ,&SQLVariant::OperUlongMulUShort ,&SQLVariant::OperULongMulSLong ,&SQLVariant::OperULongMulULong ,&SQLVariant::OperULongMulFloat ,&SQLVariant::OperULongMulDouble ,&SQLVariant::OperULongMulBit ,&SQLVariant::OperULongMulSTiny ,&SQLVariant::OperULongMulUTiny ,&SQLVariant::OperULongMulSBig ,&SQLVariant::OperULongMulUBig ,&SQLVariant::OperVarMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_FLOAT       */  ,{  &SQLVariant::OperFloatMulChar ,&SQLVariant::OperFloatMulSShort ,&SQLVariant::OperFloatMulUShort ,&SQLVariant::OperFloatMulSLong ,&SQLVariant::OperFloatMulULong ,&SQLVariant::OperFloatMulFloat ,&SQLVariant::OperFloatMulDouble ,&SQLVariant::OperFloatMulBit ,&SQLVariant::OperFloatMulSTiny ,&SQLVariant::OperFloatMulUTiny ,&SQLVariant::OperFloatMulSBig ,&SQLVariant::OperFloatMulUBig ,&SQLVariant::OperVarMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_DOUBLE      */  ,{  &SQLVariant::OperDoubleMulChar,&SQLVariant::OperDoubleMulSShort,&SQLVariant::OperDoubleMulUShort,&SQLVariant::OperDoubleMulSLong,&SQLVariant::OperDoubleMulULong,&SQLVariant::OperDoubleMulFloat,&SQLVariant::OperDoubleMulDouble,&SQLVariant::OperDoubleMulBit,&SQLVariant::OperDoubleMulSTiny,&SQLVariant::OperDoubleMulUTiny,&SQLVariant::OperDoubleMulSBig,&SQLVariant::OperDoubleMulUBig,&SQLVariant::OperVarMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_BIT         */  ,{  &SQLVariant::OperBitMulChar   ,&SQLVariant::OperBitMulSShort   ,&SQLVariant::OperBitMulUShort   ,&SQLVariant::OperBitMulSLong   ,&SQLVariant::OperBitMulULong   ,&SQLVariant::OperBitMulFloat   ,&SQLVariant::OperBitMulDouble   ,&SQLVariant::OperBitMulBit   ,&SQLVariant::OperBitMulSTiny   ,&SQLVariant::OperBitMulUTiny   ,&SQLVariant::OperBitMulSBig   ,&SQLVariant::OperBitMulUBig   ,&SQLVariant::OperVarMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_STINYINT    */  ,{  &SQLVariant::OperSTinyMulChar ,&SQLVariant::OperSTinyMulSShort ,&SQLVariant::OperSTinyMulUShort ,&SQLVariant::OperSTinyMulSLong ,&SQLVariant::OperSTinyMulULong ,&SQLVariant::OperSTinyMulFloat ,&SQLVariant::OperSTinyMulDouble ,&SQLVariant::OperSTinyMulBit ,&SQLVariant::OperSTinyMulSTiny ,&SQLVariant::OperSTinyMulUTiny ,&SQLVariant::OperSTinyMulSBig ,&SQLVariant::OperSTinyMulUBig ,&SQLVariant::OperVarMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_UTINYINT    */  ,{  &SQLVariant::OperUTinyMulChar ,&SQLVariant::OperUTinyMulSShort ,&SQLVariant::OperUTinyMulUShort ,&SQLVariant::OperUTinyMulSLong ,&SQLVariant::OperUTinyMulULong ,&SQLVariant::OperUTinyMulFloat ,&SQLVariant::OperUTinyMulDouble ,&SQLVariant::OperUTinyMulBit ,&SQLVariant::OperUTinyMulSTiny ,&SQLVariant::OperUTinyMulUTiny ,&SQLVariant::OperUTinyMulSBig ,&SQLVariant::OperUTinyMulUBig ,&SQLVariant::OperVarMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SBIGINT     */  ,{  &SQLVariant::OperSBigMulChar  ,&SQLVariant::OperSBigMulSShort  ,&SQLVariant::OperSBigMulUShort  ,&SQLVariant::OperSBigMulSLong  ,&SQLVariant::OperSBigMulULong  ,&SQLVariant::OperSBigMulFloat  ,&SQLVariant::OperSBigMulDouble  ,&SQLVariant::OperSBigMulBit  ,&SQLVariant::OperSBigMulSTiny  ,&SQLVariant::OperSBigMulUTiny  ,&SQLVariant::OperSBigMulSBig  ,&SQLVariant::OperSBigMulUBig  ,&SQLVariant::OperVarMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_UBIGINT     */  ,{  &SQLVariant::OperUBigMulChar  ,&SQLVariant::OperUBigMulSShort  ,&SQLVariant::OperUBigMulUShort  ,&SQLVariant::OperUBigMulSLong  ,&SQLVariant::OperUBigMulULong  ,&SQLVariant::OperUBigMulFloat  ,&SQLVariant::OperUBigMulDouble  ,&SQLVariant::OperUBigMulBit  ,&SQLVariant::OperUBigMulSTiny  ,&SQLVariant::OperUBigMulUTiny  ,&SQLVariant::OperUBigMulSBig  ,&SQLVariant::OperUBigMulUBig  ,&SQLVariant::OperVarMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_NUMERIC     */  ,{  &SQLVariant::OperNumMulChar   ,&SQLVariant::OperNumMulSShort   ,&SQLVariant::OperNumMulUShort   ,&SQLVariant::OperNumMulSLong   ,&SQLVariant::OperNumMulULong   ,&SQLVariant::OperNumMulFloat   ,&SQLVariant::OperNumMulDouble   ,&SQLVariant::OperNumMulBit   ,&SQLVariant::OperNumMulSTiny   ,&SQLVariant::OperNumMulUTiny   ,&SQLVariant::OperNumMulSBig   ,&SQLVariant::OperNumMulUBig   ,&SQLVariant::OperVarMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_GUID        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                     ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_BINARY      */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                     ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_DATE        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                     ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_TIME        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                     ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_TIMESTAMP   */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                     ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_INTERVAL_YM */  ,{  nullptr                       ,&SQLVariant::OperIntYMMulSShort ,&SQLVariant::OperIntYMMulUShort ,&SQLVariant::OperIntYMMulSLong ,&SQLVariant::OperIntYMMulULong ,&SQLVariant::OperIntYMMulFloat ,&SQLVariant::OperIntYMMulDouble ,&SQLVariant::OperIntYMMulBit ,&SQLVariant::OperIntYMMulSTiny ,&SQLVariant::OperIntYMMulUTiny ,&SQLVariant::OperIntYMMulSBig ,&SQLVariant::OperIntYMMulUBig ,&SQLVariant::OperIntYMMulNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_INTERVAL_DS */  ,{  nullptr                       ,&SQLVariant::OperIntDSMulSShort ,&SQLVariant::OperIntDSMulUShort ,&SQLVariant::OperIntDSMulSLong ,&SQLVariant::OperIntDSMulULong ,&SQLVariant::OperIntDSMulFloat ,&SQLVariant::OperIntDSMulDouble ,&SQLVariant::OperIntDSMulBit ,&SQLVariant::OperIntDSMulSTiny ,&SQLVariant::OperIntDSMulUTiny ,&SQLVariant::OperIntDSMulSBig ,&SQLVariant::OperIntDSMulUBig ,&SQLVariant::OperIntDSMulNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
};

// Multiplication operator for SQLVariant
SQLVariant
SQLVariant::operator*(SQLVariant& p_right)
{
  // If one of both is NULL, the result is false
  if(IsNULL() || p_right.IsNULL())
  {
    return SQLVariant();
  }

  // Getting the concise type
  SQLConciseType left  = SQLTypeToConciseType(m_datatype);
  SQLConciseType right = SQLTypeToConciseType(p_right.m_datatype);

  // Check whether both datatypes are valid
  if(left == CT_LAST || right == CT_LAST)
  {
    return SQLVariant();
  }

  // Find our comparison function
  OperatorCalculate function = OperatorMul[left][right].function;
  if(function)
  {
    return (this->*(function))(p_right);
  }
  // No compare function found
  // Data types are not comparable
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the multiplication operator on (%s * %s)",leftType,rightType);
  throw error;
}

SQLVariant 
SQLVariant::OperSShortMulChar(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortMulChar(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongMulChar(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}
SQLVariant 
SQLVariant::OperULongMulChar(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatMulChar(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleMulChar(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitMulChar(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyMulChar(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyMulChar(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigMulChar(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigMulChar(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumMulChar(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == SSHORT

SQLVariant 
SQLVariant::OperCharMulSShort(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortMulSShort(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortMulSShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant SQLVariant::OperSLongMulSShort(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongMulSShort(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatMulSShort(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleMulSShort(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitMulSShort(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyMulSShort(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyMulSShort(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigMulSShort(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigMulSShort(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumMulSShort(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

SQLVariant 
SQLVariant::OperIntYMMulSShort(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSMulSShort(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == USHORT

SQLVariant 
SQLVariant::OperCharMulUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortMulUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortMulUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}


SQLVariant 
SQLVariant::OperSLongMulUShort(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUlongMulUShort(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatMulUShort(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleMulUShort(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitMulUShort(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyMulUShort(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyMulUShort(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigMulUShort(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigMulUShort(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumMulUShort(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

SQLVariant 
SQLVariant::OperIntYMMulUShort(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSMulUShort(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == SLONG

SQLVariant 
SQLVariant::OperCharMulSLong(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortMulSLong(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortMulSLong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongMulSLong(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongMulSLong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatMulSLong(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleMulSLong(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitMulSLong(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyMulSLong(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyMulSLong(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigMulSLong(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigMulSLong(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumMulSLong(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

SQLVariant 
SQLVariant::OperIntYMMulSLong(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSMulSLong(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == ULONG

SQLVariant 
SQLVariant::OperCharMulULong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortMulULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortMulULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongMulULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongMulULong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatMulULong(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleMulULong(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitMulULong(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyMulULong(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyMulULong(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigMulULong(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigMulULong(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumMulULong(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

SQLVariant 
SQLVariant::OperIntYMMulULong(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSMulULong(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == FLOAT

SQLVariant 
SQLVariant::OperCharMulFloat(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortMulFloat(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortMulFloat(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongMulFloat(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongMulFloat(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatMulFloat(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleMulFloat(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitMulFloat(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyMulFloat(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyMulFloat(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigMulFloat(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigMulFloat(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumMulFloat(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

SQLVariant 
SQLVariant::OperIntYMMulFloat(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSMulFloat(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}

// TYPE == DOUBLE

SQLVariant 
SQLVariant::OperCharMulDouble(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortMulDouble(SQLVariant& p_right)
{
  short result = GetAsUShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortMulDouble(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongMulDouble(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongMulDouble(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatMulDouble(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleMulDouble(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitMulDouble(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyMulDouble(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyMulDouble(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigMulDouble(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigMulDouble(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumMulDouble(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

SQLVariant
SQLVariant::OperIntYMMulDouble(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant
SQLVariant::OperIntDSMulDouble(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}


// TYPE == BIT

SQLVariant 
SQLVariant::OperCharMulBit(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortMulBit(SQLVariant& p_right)
{
  short result = GetAsSShort();
  if(p_right.GetAsBit() == 0) 
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortMulBit(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongMulBit(SQLVariant& p_right)
{
  long result = GetAsSLong();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongMulBit(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatMulBit(SQLVariant& p_right)
{
  float result = GetAsFloat();
  if(p_right.GetAsBit() == 0)
  {
    result = 0.0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleMulBit(SQLVariant& p_right)
{
  double result = GetAsDouble();
  if(p_right.GetAsBit() == 0)
  {
    result = 0.0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitMulBit(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyMulBit(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyMulBit(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigMulBit(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigMulBit(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumMulBit(SQLVariant& p_right)
{
  double result = GetAsDouble();
  if(p_right.GetAsBit() == 0)
  {
    result = 0.0;
  }
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

SQLVariant 
SQLVariant::OperIntYMMulBit(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval();
  if(p_right.GetAsBit() == 0)
  {
    // Set 0 years, 0 months
    result.SetInterval(result.GetIntervalType(),0,0);
  }
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSMulBit(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval();
  if(p_right.GetAsBit() == 0)
  {
    // Set 0 days, 0 hours, 0 minutes, 0 seconds, 0 fraction
    result.SetInterval(result.GetIntervalType(),0,0,0,0,0);
  }
  return SQLVariant(&result);
}

// TYPE == STINYINT

SQLVariant 
SQLVariant::OperCharMulSTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortMulSTiny(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortMulSTiny(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongMulSTiny(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongMulSTiny(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatMulSTiny(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleMulSTiny(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitMulSTiny(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyMulSTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyMulSTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigMulSTiny(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigMulSTiny(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumMulSTiny(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

SQLVariant 
SQLVariant::OperIntYMMulSTiny(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSMulSTiny(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE = UTINYINT

SQLVariant 
SQLVariant::OperCharMulUTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortMulUTiny(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortMulUTiny(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongMulUTiny(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongMulUTiny(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatMulUTiny(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleMulUTiny(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitMulUTiny(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyMulUTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyMulUTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigMulUTiny(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigMulUTiny(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumMulUTiny(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

SQLVariant
SQLVariant::OperIntYMMulUTiny(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
SQLVariant::OperIntDSMulUTiny(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == SBIGINT

SQLVariant 
SQLVariant::OperCharMulSBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortMulSBig(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortMulSBig(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongMulSBig(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongMulSBig(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatMulSBig(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleMulSBig(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitMulSBig(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyMulSBig(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyMulSBig(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigMulSBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigMulSBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumMulSBig(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

SQLVariant 
SQLVariant::OperIntYMMulSBig(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSMulSBig(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == UBIGINT

SQLVariant 
SQLVariant::OperCharMulUBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortMulUBig(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortMulUBig(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongMulUBig(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongMulUBig(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatMulUBig(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleMulUBig(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitMulUBig(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyMulUBig(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyMulUBig(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigMulUBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigMulUBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumMulUBig(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

SQLVariant
SQLVariant::OperIntYMMulUBig(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
SQLVariant::OperIntDSMulUBig(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == NUMERIC

SQLVariant 
SQLVariant::OperVarMulNum(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result *= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

SQLVariant 
SQLVariant::OperIntYMMulNum(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSMulNum(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}
