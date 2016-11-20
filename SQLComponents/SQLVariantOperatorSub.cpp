////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorSub.cpp
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
SQLVariant::OperatorSub[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                        CT_SSHORT                        CT_USHORT                        CT_SLONG                        CT_ULONG                        CT_FLOAT                        CT_DOUBLE                        CT_BIT                        CT_STINYINT                     CT_UTINYINT                     CT_SBIGINT                     CT_UBIGINT                     CT_NUMERIC                 CT_GUID  CT_BINARY CT_DATE                       CT_TIME                      CT_TIMESTAMP                   CT_INTERVAL_YM                 CT_INTERVAL_DS
                         // ------------------------------ -------------------------------- -------------------------------- ------------------------------- ------------------------------- ------------------------------- -------------------------------- ----------------------------- ------------------------------- ------------------------------- ------------------------------ ------------------------------ -------------------------- -------- --------- ----------------------------- ---------------------------- ------------------------------ ------------------------------ --------------------------------
  /* CT_CHAR        */   {  nullptr                       ,&SQLVariant::OperCharSubSShort  ,&SQLVariant::OperCharSubUShort  ,&SQLVariant::OperCharSubSLong  ,&SQLVariant::OperCharSubULong  ,&SQLVariant::OperCharSubFloat  ,&SQLVariant::OperCharSubDouble  ,&SQLVariant::OperCharSubBit  ,&SQLVariant::OperCharSubSTiny  ,&SQLVariant::OperCharSubUTiny  ,&SQLVariant::OperCharSubSBig  ,&SQLVariant::OperCharSubUBig  ,&SQLVariant::OperVarSubNum,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_SSHORT      */  ,{  &SQLVariant::OperSShortSubChar,&SQLVariant::OperSShortSubSShort,&SQLVariant::OperSShortSubUShort,&SQLVariant::OperSShortSubSLong,&SQLVariant::OperSShortSubULong,&SQLVariant::OperSShortSubFloat,&SQLVariant::OperSShortSubDouble,&SQLVariant::OperSShortSubBit,&SQLVariant::OperSShortSubSTiny,&SQLVariant::OperSShortSubUTiny,&SQLVariant::OperSShortSubSBig,&SQLVariant::OperSShortSubUBig,&SQLVariant::OperVarSubNum,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_USHORT      */  ,{  &SQLVariant::OperUShortSubChar,&SQLVariant::OperUShortSubSShort,&SQLVariant::OperUShortSubUShort,&SQLVariant::OperUShortSubSLong,&SQLVariant::OperUShortSubULong,&SQLVariant::OperUShortSubFloat,&SQLVariant::OperUShortSubDouble,&SQLVariant::OperUShortSubBit,&SQLVariant::OperUShortSubSTiny,&SQLVariant::OperUShortSubUTiny,&SQLVariant::OperUShortSubSBig,&SQLVariant::OperUShortSubUBig,&SQLVariant::OperVarSubNum,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_SLONG       */  ,{  &SQLVariant::OperSLongSubChar ,&SQLVariant::OperSLongSubSShort ,&SQLVariant::OperSLongSubUShort ,&SQLVariant::OperSLongSubSLong ,&SQLVariant::OperSLongSubULong ,&SQLVariant::OperSLongSubFloat ,&SQLVariant::OperSLongSubDouble ,&SQLVariant::OperSLongSubBit ,&SQLVariant::OperSLongSubSTiny ,&SQLVariant::OperSLongSubUTiny ,&SQLVariant::OperSLongSubSBig ,&SQLVariant::OperSLongSubUBig ,&SQLVariant::OperVarSubNum,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_ULONG       */  ,{  &SQLVariant::OperULongSubChar ,&SQLVariant::OperULongSubSShort ,&SQLVariant::OperUlongSubUShort ,&SQLVariant::OperULongSubSLong ,&SQLVariant::OperULongSubULong ,&SQLVariant::OperULongSubFloat ,&SQLVariant::OperULongSubDouble ,&SQLVariant::OperULongSubBit ,&SQLVariant::OperULongSubSTiny ,&SQLVariant::OperULongSubUTiny ,&SQLVariant::OperULongSubSBig ,&SQLVariant::OperULongSubUBig ,&SQLVariant::OperVarSubNum,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_FLOAT       */  ,{  &SQLVariant::OperFloatSubChar ,&SQLVariant::OperFloatSubSShort ,&SQLVariant::OperFloatSubUShort ,&SQLVariant::OperFloatSubSLong ,&SQLVariant::OperFloatSubULong ,&SQLVariant::OperFloatSubFloat ,&SQLVariant::OperFloatSubDouble ,&SQLVariant::OperFloatSubBit ,&SQLVariant::OperFloatSubSTiny ,&SQLVariant::OperFloatSubUTiny ,&SQLVariant::OperFloatSubSBig ,&SQLVariant::OperFloatSubUBig ,&SQLVariant::OperVarSubNum,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_DOUBLE      */  ,{  &SQLVariant::OperDoubleSubChar,&SQLVariant::OperDoubleSubSShort,&SQLVariant::OperDoubleSubUShort,&SQLVariant::OperDoubleSubSLong,&SQLVariant::OperDoubleSubULong,&SQLVariant::OperDoubleSubFloat,&SQLVariant::OperDoubleSubDouble,&SQLVariant::OperDoubleSubBit,&SQLVariant::OperDoubleSubSTiny,&SQLVariant::OperDoubleSubUTiny,&SQLVariant::OperDoubleSubSBig,&SQLVariant::OperDoubleSubUBig,&SQLVariant::OperVarSubNum,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_BIT         */  ,{  &SQLVariant::OperBitSubChar   ,&SQLVariant::OperBitSubSShort   ,&SQLVariant::OperBitSubUShort   ,&SQLVariant::OperBitSubSLong   ,&SQLVariant::OperBitSubULong   ,&SQLVariant::OperBitSubFloat   ,&SQLVariant::OperBitSubDouble   ,&SQLVariant::OperBitSubBit   ,&SQLVariant::OperBitSubSTiny   ,&SQLVariant::OperBitSubUTiny   ,&SQLVariant::OperBitSubSBig   ,&SQLVariant::OperBitSubUBig   ,&SQLVariant::OperVarSubNum,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_STINYINT    */  ,{  &SQLVariant::OperSTinySubChar ,&SQLVariant::OperSTinySubSShort ,&SQLVariant::OperSTinySubUShort ,&SQLVariant::OperSTinySubSLong ,&SQLVariant::OperSTinySubULong ,&SQLVariant::OperSTinySubFloat ,&SQLVariant::OperSTinySubDouble ,&SQLVariant::OperSTinySubBit ,&SQLVariant::OperSTinySubSTiny ,&SQLVariant::OperSTinySubUTiny ,&SQLVariant::OperSTinySubSBig ,&SQLVariant::OperSTinySubUBig ,&SQLVariant::OperVarSubNum,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_UTINYINT    */  ,{  &SQLVariant::OperUTinySubChar ,&SQLVariant::OperUTinySubSShort ,&SQLVariant::OperUTinySubUShort ,&SQLVariant::OperUTinySubSLong ,&SQLVariant::OperUTinySubULong ,&SQLVariant::OperUTinySubFloat ,&SQLVariant::OperUTinySubDouble ,&SQLVariant::OperUTinySubBit ,&SQLVariant::OperUTinySubSTiny ,&SQLVariant::OperUTinySubUTiny ,&SQLVariant::OperUTinySubSBig ,&SQLVariant::OperUTinySubUBig ,&SQLVariant::OperVarSubNum,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_SBIGINT     */  ,{  &SQLVariant::OperSBigSubChar  ,&SQLVariant::OperSBigSubSShort  ,&SQLVariant::OperSBigSubUShort  ,&SQLVariant::OperSBigSubSLong  ,&SQLVariant::OperSBigSubULong  ,&SQLVariant::OperSBigSubFloat  ,&SQLVariant::OperSBigSubDouble  ,&SQLVariant::OperSBigSubBit  ,&SQLVariant::OperSBigSubSTiny  ,&SQLVariant::OperSBigSubUTiny  ,&SQLVariant::OperSBigSubSBig  ,&SQLVariant::OperSBigSubUBig  ,&SQLVariant::OperVarSubNum,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_UBIGINT     */  ,{  &SQLVariant::OperUBigSubChar  ,&SQLVariant::OperUBigSubSShort  ,&SQLVariant::OperUBigSubUShort  ,&SQLVariant::OperUBigSubSLong  ,&SQLVariant::OperUBigSubULong  ,&SQLVariant::OperUBigSubFloat  ,&SQLVariant::OperUBigSubDouble  ,&SQLVariant::OperUBigSubBit  ,&SQLVariant::OperUBigSubSTiny  ,&SQLVariant::OperUBigSubUTiny  ,&SQLVariant::OperUBigSubSBig  ,&SQLVariant::OperUBigSubUBig  ,&SQLVariant::OperVarSubNum,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_NUMERIC     */  ,{  &SQLVariant::OperNumSubChar   ,&SQLVariant::OperNumSubSShort   ,&SQLVariant::OperNumSubUShort   ,&SQLVariant::OperNumSubSLong   ,&SQLVariant::OperNumSubULong   ,&SQLVariant::OperNumSubFloat   ,&SQLVariant::OperNumSubDouble   ,&SQLVariant::OperNumSubBit   ,&SQLVariant::OperNumSubSTiny   ,&SQLVariant::OperNumSubUTiny   ,&SQLVariant::OperNumSubSBig   ,&SQLVariant::OperNumSubUBig   ,&SQLVariant::OperVarSubNum,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_GUID        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                   ,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_BINARY      */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                   ,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,nullptr                        }
  /* CT_DATE        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                   ,nullptr ,nullptr  ,&SQLVariant::OperDateSubDate ,nullptr                     ,nullptr                       ,nullptr                       ,&SQLVariant::OperDateSubIntDS  }
  /* CT_TIME        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                   ,nullptr ,nullptr  ,nullptr                      ,&SQLVariant::OperTimeSubTime,nullptr                       ,nullptr                       ,&SQLVariant::OperTimeSubIntDS  }
  /* CT_TIMESTAMP   */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                   ,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,&SQLVariant::OperStampSubStamp,nullptr                       ,&SQLVariant::OperStampSubIntDS }
  /* CT_INTERVAL_YM */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                   ,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,&SQLVariant::OperIntYMSubIntYM,nullptr                        }
  /* CT_INTERVAL_DS */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                   ,nullptr ,nullptr  ,nullptr                      ,nullptr                     ,nullptr                       ,nullptr                       ,&SQLVariant::OperIntDSSubIntDS }
};

// Sub operator for SQLVariant
SQLVariant
SQLVariant::operator-(SQLVariant& p_right)
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
  OperatorCalculate function = OperatorSub[left][right].function;
  if(function)
  {
    return (this->*(function))(p_right);
  }
  // No compare function found
  // Data types are not comparable
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the subtract operator on (%s - %s)",leftType,rightType);
  throw error;
}

SQLVariant 
SQLVariant::OperSShortSubChar(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortSubChar(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongSubChar(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}
SQLVariant 
SQLVariant::OperULongSubChar(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatSubChar(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleSubChar(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitSubChar(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinySubChar(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinySubChar(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigSubChar(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigSubChar(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumSubChar(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == SSHORT

SQLVariant 
SQLVariant::OperCharSubSShort(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortSubSShort(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortSubSShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant SQLVariant::OperSLongSubSShort(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongSubSShort(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatSubSShort(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleSubSShort(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitSubSShort(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinySubSShort(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinySubSShort(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigSubSShort(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigSubSShort(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumSubSShort(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == USHORT

SQLVariant 
SQLVariant::OperCharSubUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortSubUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortSubUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongSubUShort(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUlongSubUShort(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatSubUShort(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleSubUShort(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitSubUShort(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinySubUShort(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinySubUShort(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigSubUShort(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigSubUShort(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumSubUShort(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == SLONG

SQLVariant 
SQLVariant::OperCharSubSLong(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortSubSLong(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortSubSLong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongSubSLong(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongSubSLong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatSubSLong(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleSubSLong(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitSubSLong(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinySubSLong(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinySubSLong(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigSubSLong(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigSubSLong(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumSubSLong(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == ULONG

SQLVariant 
SQLVariant::OperCharSubULong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortSubULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortSubULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongSubULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongSubULong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatSubULong(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleSubULong(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitSubULong(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinySubULong(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinySubULong(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigSubULong(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigSubULong(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumSubULong(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == FLOAT

SQLVariant 
SQLVariant::OperCharSubFloat(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortSubFloat(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortSubFloat(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongSubFloat(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongSubFloat(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatSubFloat(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleSubFloat(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitSubFloat(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinySubFloat(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinySubFloat(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigSubFloat(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigSubFloat(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumSubFloat(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == DOUBLE

SQLVariant 
SQLVariant::OperCharSubDouble(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortSubDouble(SQLVariant& p_right)
{
  short result = GetAsUShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortSubDouble(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongSubDouble(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongSubDouble(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatSubDouble(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleSubDouble(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitSubDouble(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinySubDouble(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinySubDouble(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigSubDouble(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigSubDouble(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumSubDouble(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == BIT

SQLVariant 
SQLVariant::OperCharSubBit(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortSubBit(SQLVariant& p_right)
{
  short result = GetAsSShort();
  if(p_right.GetAsBit()) 
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortSubBit(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongSubBit(SQLVariant& p_right)
{
  long result = GetAsSLong();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongSubBit(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatSubBit(SQLVariant& p_right)
{
  float result = GetAsFloat();
  if(p_right.GetAsBit())
  {
    result -= 1.0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleSubBit(SQLVariant& p_right)
{
  double result = GetAsDouble();
  if(p_right.GetAsBit())
  {
    result -= 1.0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitSubBit(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinySubBit(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinySubBit(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigSubBit(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigSubBit(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumSubBit(SQLVariant& p_right)
{
  double result = GetAsDouble();
  if(p_right.GetAsBit())
  {
    result -= 1.0;
  }
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == STINYINT

SQLVariant 
SQLVariant::OperCharSubSTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortSubSTiny(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortSubSTiny(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongSubSTiny(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongSubSTiny(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatSubSTiny(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleSubSTiny(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitSubSTiny(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinySubSTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinySubSTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigSubSTiny(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigSubSTiny(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumSubSTiny(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE = UTINYINT

SQLVariant 
SQLVariant::OperCharSubUTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortSubUTiny(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortSubUTiny(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongSubUTiny(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongSubUTiny(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatSubUTiny(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleSubUTiny(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitSubUTiny(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinySubUTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinySubUTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigSubUTiny(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigSubUTiny(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumSubUTiny(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == SBIGINT

SQLVariant 
SQLVariant::OperCharSubSBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortSubSBig(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortSubSBig(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongSubSBig(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongSubSBig(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatSubSBig(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleSubSBig(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitSubSBig(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinySubSBig(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinySubSBig(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigSubSBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigSubSBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumSubSBig(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == UBIGINT

SQLVariant 
SQLVariant::OperCharSubUBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortSubUBig(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortSubUBig(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongSubUBig(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongSubUBig(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatSubUBig(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleSubUBig(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitSubUBig(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinySubUBig(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinySubUBig(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigSubUBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigSubUBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumSubUBig(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == NUMERIC

SQLVariant 
SQLVariant::OperVarSubNum(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result -= p_right.GetAsDouble();
  CString resString;
  resString.Format("%.16lf",result);

  SQLVariant var(this);
  var.SetData(SQL_C_NUMERIC,resString);

  return var;
}

// TYPE == DATE
#pragma warning (disable: 4239)

SQLVariant 
SQLVariant::OperDateSubDate(SQLVariant& p_right)
{
  const SQLDate date1 = GetAsSQLDate();
  const SQLDate date2 = p_right.GetAsSQLDate();
  SQLInterval result = date1 - date2;
  return SQLVariant(&result);
}

// TYPE == TIME

SQLVariant 
SQLVariant::OperTimeSubTime(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLTime() - p_right.GetAsSQLTime();
  return SQLVariant(&result);
}

// TYPE == TIMESTAMP

SQLVariant 
SQLVariant::OperStampSubStamp(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLTimestamp() - p_right.GetAsSQLTimestamp();
  return SQLVariant(&result);
}


// TYPE == INTERVAL_YEAR_MONTH

SQLVariant 
SQLVariant::OperIntYMSubIntYM(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() - p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == INTERVAL_DAY_SECOND

SQLVariant 
SQLVariant::OperDateSubIntDS(SQLVariant& p_right)
{
  SQLDate result = GetAsSQLDate();
  result = result - p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperTimeSubIntDS(SQLVariant& p_right)
{
  SQLTime result = GetAsSQLTime();
  result = result - p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperStampSubIntDS(SQLVariant& p_right)
{
  SQLTimestamp result = GetAsSQLTimestamp();
  result = result - p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSSubIntDS(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() - p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}
