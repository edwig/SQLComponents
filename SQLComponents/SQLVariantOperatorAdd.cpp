////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorAdd.cpp
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

CalculateFunctionArray
SQLVariant::OperatorAdd[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                        CT_SSHORT                        CT_USHORT                        CT_SLONG                        CT_ULONG                        CT_FLOAT                        CT_DOUBLE                        CT_BIT                        CT_STINYINT                     CT_UTINYINT                     CT_SBIGINT                     CT_UBIGINT                     CT_NUMERIC                 CT_GUID  CT_BINARY CT_DATE                       CT_TIME                       CT_TIMESTAMP                   CT_INTERVAL_YM                 CT_INTERVAL_DS
                         // ------------------------------ -------------------------------- -------------------------------- ------------------------------- ------------------------------- ------------------------------- -------------------------------- ----------------------------- ------------------------------- ------------------------------- ------------------------------ ------------------------------ -------------------------- -------- --------- ----------------------------- ----------------------------- ------------------------------ ------------------------------ --------------------------------
  /* CT_CHAR        */   {  &SQLVariant::OperCharAddChar  ,&SQLVariant::OperCharAddSShort  ,&SQLVariant::OperCharAddUShort  ,&SQLVariant::OperCharAddSLong  ,&SQLVariant::OperCharAddULong  ,&SQLVariant::OperCharAddFloat  ,&SQLVariant::OperCharAddDouble  ,&SQLVariant::OperCharAddBit  ,&SQLVariant::OperCharAddSTiny  ,&SQLVariant::OperCharAddUTiny  ,&SQLVariant::OperCharAddSBig  ,&SQLVariant::OperCharAddUBig  ,&SQLVariant::OperVarAddNum,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_SSHORT      */  ,{  &SQLVariant::OperSShortAddChar,&SQLVariant::OperSShortAddSShort,&SQLVariant::OperSShortAddUShort,&SQLVariant::OperSShortAddSLong,&SQLVariant::OperSShortAddULong,&SQLVariant::OperSShortAddFloat,&SQLVariant::OperSShortAddDouble,&SQLVariant::OperSShortAddBit,&SQLVariant::OperSShortAddSTiny,&SQLVariant::OperSShortAddUTiny,&SQLVariant::OperSShortAddSBig,&SQLVariant::OperSShortAddUBig,&SQLVariant::OperVarAddNum,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_USHORT      */  ,{  &SQLVariant::OperUShortAddChar,&SQLVariant::OperUShortAddSShort,&SQLVariant::OperUShortAddUShort,&SQLVariant::OperUShortAddSLong,&SQLVariant::OperUShortAddULong,&SQLVariant::OperUShortAddFloat,&SQLVariant::OperUShortAddDouble,&SQLVariant::OperUShortAddBit,&SQLVariant::OperUShortAddSTiny,&SQLVariant::OperUShortAddUTiny,&SQLVariant::OperUShortAddSBig,&SQLVariant::OperUShortAddUBig,&SQLVariant::OperVarAddNum,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_SLONG       */  ,{  &SQLVariant::OperSLongAddChar ,&SQLVariant::OperSLongAddSShort ,&SQLVariant::OperSLongAddUShort ,&SQLVariant::OperSLongAddSLong ,&SQLVariant::OperSLongAddULong ,&SQLVariant::OperSLongAddFloat ,&SQLVariant::OperSLongAddDouble ,&SQLVariant::OperSLongAddBit ,&SQLVariant::OperSLongAddSTiny ,&SQLVariant::OperSLongAddUTiny ,&SQLVariant::OperSLongAddSBig ,&SQLVariant::OperSLongAddUBig ,&SQLVariant::OperVarAddNum,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_ULONG       */  ,{  &SQLVariant::OperULongAddChar ,&SQLVariant::OperULongAddSShort ,&SQLVariant::OperUlongAddUShort ,&SQLVariant::OperULongAddSLong ,&SQLVariant::OperULongAddULong ,&SQLVariant::OperULongAddFloat ,&SQLVariant::OperULongAddDouble ,&SQLVariant::OperULongAddBit ,&SQLVariant::OperULongAddSTiny ,&SQLVariant::OperULongAddUTiny ,&SQLVariant::OperULongAddSBig ,&SQLVariant::OperULongAddUBig ,&SQLVariant::OperVarAddNum,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_FLOAT       */  ,{  &SQLVariant::OperFloatAddChar ,&SQLVariant::OperFloatAddSShort ,&SQLVariant::OperFloatAddUShort ,&SQLVariant::OperFloatAddSLong ,&SQLVariant::OperFloatAddULong ,&SQLVariant::OperFloatAddFloat ,&SQLVariant::OperFloatAddDouble ,&SQLVariant::OperFloatAddBit ,&SQLVariant::OperFloatAddSTiny ,&SQLVariant::OperFloatAddUTiny ,&SQLVariant::OperFloatAddSBig ,&SQLVariant::OperFloatAddUBig ,&SQLVariant::OperVarAddNum,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_DOUBLE      */  ,{  &SQLVariant::OperDoubleAddChar,&SQLVariant::OperDoubleAddSShort,&SQLVariant::OperDoubleAddUShort,&SQLVariant::OperDoubleAddSLong,&SQLVariant::OperDoubleAddULong,&SQLVariant::OperDoubleAddFloat,&SQLVariant::OperDoubleAddDouble,&SQLVariant::OperDoubleAddBit,&SQLVariant::OperDoubleAddSTiny,&SQLVariant::OperDoubleAddUTiny,&SQLVariant::OperDoubleAddSBig,&SQLVariant::OperDoubleAddUBig,&SQLVariant::OperVarAddNum,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_BIT         */  ,{  &SQLVariant::OperBitAddChar   ,&SQLVariant::OperBitAddSShort   ,&SQLVariant::OperBitAddUShort   ,&SQLVariant::OperBitAddSLong   ,&SQLVariant::OperBitAddULong   ,&SQLVariant::OperBitAddFloat   ,&SQLVariant::OperBitAddDouble   ,&SQLVariant::OperBitAddBit   ,&SQLVariant::OperBitAddSTiny   ,&SQLVariant::OperBitAddUTiny   ,&SQLVariant::OperBitAddSBig   ,&SQLVariant::OperBitAddUBig   ,&SQLVariant::OperVarAddNum,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_STINYINT    */  ,{  &SQLVariant::OperSTinyAddChar ,&SQLVariant::OperSTinyAddSShort ,&SQLVariant::OperSTinyAddUShort ,&SQLVariant::OperSTinyAddSLong ,&SQLVariant::OperSTinyAddULong ,&SQLVariant::OperSTinyAddFloat ,&SQLVariant::OperSTinyAddDouble ,&SQLVariant::OperSTinyAddBit ,&SQLVariant::OperSTinyAddSTiny ,&SQLVariant::OperSTinyAddUTiny ,&SQLVariant::OperSTinyAddSBig ,&SQLVariant::OperSTinyAddUBig ,&SQLVariant::OperVarAddNum,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_UTINYINT    */  ,{  &SQLVariant::OperUTinyAddChar ,&SQLVariant::OperUTinyAddSShort ,&SQLVariant::OperUTinyAddUShort ,&SQLVariant::OperUTinyAddSLong ,&SQLVariant::OperUTinyAddULong ,&SQLVariant::OperUTinyAddFloat ,&SQLVariant::OperUTinyAddDouble ,&SQLVariant::OperUTinyAddBit ,&SQLVariant::OperUTinyAddSTiny ,&SQLVariant::OperUTinyAddUTiny ,&SQLVariant::OperUTinyAddSBig ,&SQLVariant::OperUTinyAddUBig ,&SQLVariant::OperVarAddNum,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_SBIGINT     */  ,{  &SQLVariant::OperSBigAddChar  ,&SQLVariant::OperSBigAddSShort  ,&SQLVariant::OperSBigAddUShort  ,&SQLVariant::OperSBigAddSLong  ,&SQLVariant::OperSBigAddULong  ,&SQLVariant::OperSBigAddFloat  ,&SQLVariant::OperSBigAddDouble  ,&SQLVariant::OperSBigAddBit  ,&SQLVariant::OperSBigAddSTiny  ,&SQLVariant::OperSBigAddUTiny  ,&SQLVariant::OperSBigAddSBig  ,&SQLVariant::OperSBigAddUBig  ,&SQLVariant::OperVarAddNum,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_UBIGINT     */  ,{  &SQLVariant::OperUBigAddChar  ,&SQLVariant::OperUBigAddSShort  ,&SQLVariant::OperUBigAddUShort  ,&SQLVariant::OperUBigAddSLong  ,&SQLVariant::OperUBigAddULong  ,&SQLVariant::OperUBigAddFloat  ,&SQLVariant::OperUBigAddDouble  ,&SQLVariant::OperUBigAddBit  ,&SQLVariant::OperUBigAddSTiny  ,&SQLVariant::OperUBigAddUTiny  ,&SQLVariant::OperUBigAddSBig  ,&SQLVariant::OperUBigAddUBig  ,&SQLVariant::OperVarAddNum,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_NUMERIC     */  ,{  &SQLVariant::OperNumAddChar   ,&SQLVariant::OperNumAddSShort   ,&SQLVariant::OperNumAddUShort   ,&SQLVariant::OperNumAddSLong   ,&SQLVariant::OperNumAddULong   ,&SQLVariant::OperNumAddFloat   ,&SQLVariant::OperNumAddDouble   ,&SQLVariant::OperNumAddBit   ,&SQLVariant::OperNumAddSTiny   ,&SQLVariant::OperNumAddUTiny   ,&SQLVariant::OperNumAddSBig   ,&SQLVariant::OperNumAddUBig   ,&SQLVariant::OperVarAddNum,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_GUID        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                   ,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_BINARY      */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                   ,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_DATE        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                   ,nullptr ,nullptr  ,nullptr                      ,&SQLVariant::OperDateAddTime ,nullptr                       ,nullptr                       ,&SQLVariant::OperDateAddIntDS  }
  /* CT_TIME        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                   ,nullptr ,nullptr  ,&SQLVariant::OperTimeAddDate ,nullptr                      ,nullptr                       ,nullptr                       ,&SQLVariant::OperTimeAddIntDS  }
  /* CT_TIMESTAMP   */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                   ,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,nullptr                       ,&SQLVariant::OperStampAddIntDS }
  /* CT_INTERVAL_YM */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                   ,nullptr ,nullptr  ,nullptr                      ,nullptr                      ,nullptr                       ,&SQLVariant::OperIntYMAddIntYM,nullptr                        }
  /* CT_INTERVAL_DS */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                   ,nullptr ,nullptr  ,&SQLVariant::OperIntDSAddDate,&SQLVariant::OperIntDSAddTime,&SQLVariant::OperIntDSAddStamp,nullptr                       ,&SQLVariant::OperIntDSAddIntDS }
};

// Add operator for SQLVariant
SQLVariant
SQLVariant::operator+(SQLVariant& p_right)
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
  OperatorCalculate function = OperatorAdd[left][right].function;
  if(function)
  {
    return (this->*(function))(p_right);
  }
  // No compare function found
  // Data types are not comparable
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the add operator on (%s + %s)",leftType,rightType);
  throw error;
}

SQLVariant
SQLVariant::OperCharAddChar(SQLVariant& p_right)
{
  CString left;
  CString right;

  GetAsString(left);
  p_right.GetAsString(right);

  CString result = left + right;
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortAddChar(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortAddChar(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongAddChar(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}
SQLVariant 
SQLVariant::OperULongAddChar(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatAddChar(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleAddChar(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitAddChar(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyAddChar(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyAddChar(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigAddChar(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigAddChar(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumAddChar(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == SSHORT

SQLVariant 
SQLVariant::OperCharAddSShort(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortAddSShort(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortAddSShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant SQLVariant::OperSLongAddSShort(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongAddSShort(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatAddSShort(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleAddSShort(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitAddSShort(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyAddSShort(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyAddSShort(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigAddSShort(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigAddSShort(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumAddSShort(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == USHORT

SQLVariant 
SQLVariant::OperCharAddUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortAddUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortAddUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}


SQLVariant 
SQLVariant::OperSLongAddUShort(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUlongAddUShort(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatAddUShort(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleAddUShort(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitAddUShort(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyAddUShort(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyAddUShort(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigAddUShort(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigAddUShort(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumAddUShort(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == SLONG

SQLVariant 
SQLVariant::OperCharAddSLong(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortAddSLong(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortAddSLong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongAddSLong(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongAddSLong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatAddSLong(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleAddSLong(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitAddSLong(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyAddSLong(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyAddSLong(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigAddSLong(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigAddSLong(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumAddSLong(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == ULONG

SQLVariant 
SQLVariant::OperCharAddULong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortAddULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortAddULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongAddULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongAddULong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatAddULong(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleAddULong(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitAddULong(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyAddULong(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyAddULong(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigAddULong(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigAddULong(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumAddULong(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == FLOAT

SQLVariant 
SQLVariant::OperCharAddFloat(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortAddFloat(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortAddFloat(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongAddFloat(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongAddFloat(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatAddFloat(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleAddFloat(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitAddFloat(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyAddFloat(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyAddFloat(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigAddFloat(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigAddFloat(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumAddFloat(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == DOUBLE

SQLVariant 
SQLVariant::OperCharAddDouble(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortAddDouble(SQLVariant& p_right)
{
  short result = GetAsUShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortAddDouble(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongAddDouble(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongAddDouble(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatAddDouble(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleAddDouble(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitAddDouble(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyAddDouble(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyAddDouble(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigAddDouble(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigAddDouble(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumAddDouble(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == BIT

SQLVariant 
SQLVariant::OperCharAddBit(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortAddBit(SQLVariant& p_right)
{
  short result = GetAsSShort();
  if(p_right.GetAsBit()) 
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortAddBit(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  if(p_right.GetAsBit())
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongAddBit(SQLVariant& p_right)
{
  long result = GetAsSLong();
  if(p_right.GetAsBit())
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongAddBit(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  if(p_right.GetAsBit())
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatAddBit(SQLVariant& p_right)
{
  float result = GetAsFloat();
  if(p_right.GetAsBit())
  {
    result += 1.0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleAddBit(SQLVariant& p_right)
{
  double result = GetAsDouble();
  if(p_right.GetAsBit())
  {
    result += 1.0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitAddBit(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyAddBit(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  if(p_right.GetAsBit())
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyAddBit(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  if(p_right.GetAsBit())
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigAddBit(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  if(p_right.GetAsBit())
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigAddBit(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  if(p_right.GetAsBit())
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumAddBit(SQLVariant& p_right)
{
  double result = GetAsDouble();
  if(p_right.GetAsBit())
  {
    result += 1.0;
  }
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == STINYINT

SQLVariant 
SQLVariant::OperCharAddSTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortAddSTiny(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortAddSTiny(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongAddSTiny(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongAddSTiny(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatAddSTiny(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleAddSTiny(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitAddSTiny(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyAddSTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyAddSTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigAddSTiny(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigAddSTiny(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumAddSTiny(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE = UTINYINT

SQLVariant 
SQLVariant::OperCharAddUTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortAddUTiny(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortAddUTiny(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongAddUTiny(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongAddUTiny(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatAddUTiny(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleAddUTiny(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitAddUTiny(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyAddUTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyAddUTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigAddUTiny(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigAddUTiny(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumAddUTiny(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == SBIGINT

SQLVariant 
SQLVariant::OperCharAddSBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortAddSBig(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortAddSBig(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongAddSBig(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongAddSBig(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatAddSBig(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleAddSBig(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitAddSBig(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyAddSBig(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyAddSBig(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigAddSBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigAddSBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumAddSBig(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == UBIGINT

SQLVariant 
SQLVariant::OperCharAddUBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortAddUBig(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortAddUBig(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongAddUBig(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongAddUBig(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatAddUBig(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleAddUBig(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitAddUBig(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyAddUBig(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyAddUBig(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigAddUBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigAddUBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumAddUBig(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == NUMERIC

SQLVariant 
SQLVariant::OperVarAddNum(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result += p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == DATE

SQLVariant 
SQLVariant::OperTimeAddDate(SQLVariant& p_right)
{
  SQLTimestamp result(p_right.GetAsSQLDate(),GetAsSQLTime());
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSAddDate(SQLVariant& p_right)
{
  SQLDate result = p_right.GetAsSQLDate();
  result = result + GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == TIME

SQLVariant 
SQLVariant::OperDateAddTime(SQLVariant& p_right)
{
  SQLTimestamp result(GetAsSQLDate(),p_right.GetAsSQLTime());
  return SQLVariant(&result);
}
SQLVariant 
SQLVariant::OperIntDSAddTime(SQLVariant& p_right)
{
  SQLTime result = p_right.GetAsSQLTime();
  result = result + GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == TIMESTAMP

SQLVariant 
SQLVariant::OperIntDSAddStamp(SQLVariant& p_right)
{
  SQLTimestamp result = p_right.GetAsSQLTimestamp();
  result = result + GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == INTERVAL_YEAR_MONTH
#pragma warning (disable: 4239)

SQLVariant 
SQLVariant::OperIntYMAddIntYM(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval();
  result = result + p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == INTERVAL_DAY_SECOND

SQLVariant 
SQLVariant::OperDateAddIntDS(SQLVariant& p_right)
{
  SQLDate result = GetAsSQLDate();
  result = result + p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperTimeAddIntDS(SQLVariant& p_right)
{
  SQLTime result = GetAsSQLTime();
  result = result + p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperStampAddIntDS(SQLVariant& p_right)
{
  SQLTimestamp result = GetAsSQLTimestamp();
  result = result + p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSAddIntDS(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() + p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}
