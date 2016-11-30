////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorDiv.cpp
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
#include "bcd.h"

#pragma warning (disable: 4239)

CalculateFunctionArray
SQLVariant::OperatorDiv[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                        CT_SSHORT                        CT_USHORT                        CT_SLONG                        CT_ULONG                        CT_FLOAT                        CT_DOUBLE                        CT_BIT                        CT_STINYINT                     CT_UTINYINT                     CT_SBIGINT                     CT_UBIGINT                     CT_NUMERIC                    CT_GUID  CT_BINARY CT_DATE  CT_TIME  CT_TIMESTAMP CT_INTERVAL_YM CT_INTERVAL_DS
                         // ------------------------------ -------------------------------- -------------------------------- ------------------------------- ------------------------------- ------------------------------- -------------------------------- ----------------------------- ------------------------------- ------------------------------- ------------------------------ ------------------------------ ----------------------------- -------- --------- -------- -------- ------------ -------------- --------------
  /* CT_CHAR        */   {  nullptr                       ,&SQLVariant::OperCharDivSShort  ,&SQLVariant::OperCharDivUShort  ,&SQLVariant::OperCharDivSLong  ,&SQLVariant::OperCharDivULong  ,&SQLVariant::OperCharDivFloat  ,&SQLVariant::OperCharDivDouble  ,&SQLVariant::OperCharDivBit  ,&SQLVariant::OperCharDivSTiny  ,&SQLVariant::OperCharDivUTiny  ,&SQLVariant::OperCharDivSBig  ,&SQLVariant::OperCharDivUBig  ,&SQLVariant::OperCharDivNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SSHORT      */  ,{  &SQLVariant::OperSShortDivChar,&SQLVariant::OperSShortDivSShort,&SQLVariant::OperSShortDivUShort,&SQLVariant::OperSShortDivSLong,&SQLVariant::OperSShortDivULong,&SQLVariant::OperSShortDivFloat,&SQLVariant::OperSShortDivDouble,&SQLVariant::OperSShortDivBit,&SQLVariant::OperSShortDivSTiny,&SQLVariant::OperSShortDivUTiny,&SQLVariant::OperSShortDivSBig,&SQLVariant::OperSShortDivUBig,&SQLVariant::OperSShortDivNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_USHORT      */  ,{  &SQLVariant::OperUShortDivChar,&SQLVariant::OperUShortDivSShort,&SQLVariant::OperUShortDivUShort,&SQLVariant::OperUShortDivSLong,&SQLVariant::OperUShortDivULong,&SQLVariant::OperUShortDivFloat,&SQLVariant::OperUShortDivDouble,&SQLVariant::OperUShortDivBit,&SQLVariant::OperUShortDivSTiny,&SQLVariant::OperUShortDivUTiny,&SQLVariant::OperUShortDivSBig,&SQLVariant::OperUShortDivUBig,&SQLVariant::OperUShortDivNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SLONG       */  ,{  &SQLVariant::OperSLongDivChar ,&SQLVariant::OperSLongDivSShort ,&SQLVariant::OperSLongDivUShort ,&SQLVariant::OperSLongDivSLong ,&SQLVariant::OperSLongDivULong ,&SQLVariant::OperSLongDivFloat ,&SQLVariant::OperSLongDivDouble ,&SQLVariant::OperSLongDivBit ,&SQLVariant::OperSLongDivSTiny ,&SQLVariant::OperSLongDivUTiny ,&SQLVariant::OperSLongDivSBig ,&SQLVariant::OperSLongDivUBig ,&SQLVariant::OperSLongDivNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_ULONG       */  ,{  &SQLVariant::OperULongDivChar ,&SQLVariant::OperULongDivSShort ,&SQLVariant::OperUlongDivUShort ,&SQLVariant::OperULongDivSLong ,&SQLVariant::OperULongDivULong ,&SQLVariant::OperULongDivFloat ,&SQLVariant::OperULongDivDouble ,&SQLVariant::OperULongDivBit ,&SQLVariant::OperULongDivSTiny ,&SQLVariant::OperULongDivUTiny ,&SQLVariant::OperULongDivSBig ,&SQLVariant::OperULongDivUBig ,&SQLVariant::OperULongDivNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_FLOAT       */  ,{  &SQLVariant::OperFloatDivChar ,&SQLVariant::OperFloatDivSShort ,&SQLVariant::OperFloatDivUShort ,&SQLVariant::OperFloatDivSLong ,&SQLVariant::OperFloatDivULong ,&SQLVariant::OperFloatDivFloat ,&SQLVariant::OperFloatDivDouble ,&SQLVariant::OperFloatDivBit ,&SQLVariant::OperFloatDivSTiny ,&SQLVariant::OperFloatDivUTiny ,&SQLVariant::OperFloatDivSBig ,&SQLVariant::OperFloatDivUBig ,&SQLVariant::OperFloatDivNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_DOUBLE      */  ,{  &SQLVariant::OperDoubleDivChar,&SQLVariant::OperDoubleDivSShort,&SQLVariant::OperDoubleDivUShort,&SQLVariant::OperDoubleDivSLong,&SQLVariant::OperDoubleDivULong,&SQLVariant::OperDoubleDivFloat,&SQLVariant::OperDoubleDivDouble,&SQLVariant::OperDoubleDivBit,&SQLVariant::OperDoubleDivSTiny,&SQLVariant::OperDoubleDivUTiny,&SQLVariant::OperDoubleDivSBig,&SQLVariant::OperDoubleDivUBig,&SQLVariant::OperDoubleDivNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_BIT         */  ,{  &SQLVariant::OperBitDivChar   ,&SQLVariant::OperBitDivSShort   ,&SQLVariant::OperBitDivUShort   ,&SQLVariant::OperBitDivSLong   ,&SQLVariant::OperBitDivULong   ,&SQLVariant::OperBitDivFloat   ,&SQLVariant::OperBitDivDouble   ,&SQLVariant::OperBitDivBit   ,&SQLVariant::OperBitDivSTiny   ,&SQLVariant::OperBitDivUTiny   ,&SQLVariant::OperBitDivSBig   ,&SQLVariant::OperBitDivUBig   ,&SQLVariant::OperBitDivNum   ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_STINYINT    */  ,{  &SQLVariant::OperSTinyDivChar ,&SQLVariant::OperSTinyDivSShort ,&SQLVariant::OperSTinyDivUShort ,&SQLVariant::OperSTinyDivSLong ,&SQLVariant::OperSTinyDivULong ,&SQLVariant::OperSTinyDivFloat ,&SQLVariant::OperSTinyDivDouble ,&SQLVariant::OperSTinyDivBit ,&SQLVariant::OperSTinyDivSTiny ,&SQLVariant::OperSTinyDivUTiny ,&SQLVariant::OperSTinyDivSBig ,&SQLVariant::OperSTinyDivUBig ,&SQLVariant::OperSTinyDivNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_UTINYINT    */  ,{  &SQLVariant::OperUTinyDivChar ,&SQLVariant::OperUTinyDivSShort ,&SQLVariant::OperUTinyDivUShort ,&SQLVariant::OperUTinyDivSLong ,&SQLVariant::OperUTinyDivULong ,&SQLVariant::OperUTinyDivFloat ,&SQLVariant::OperUTinyDivDouble ,&SQLVariant::OperUTinyDivBit ,&SQLVariant::OperUTinyDivSTiny ,&SQLVariant::OperUTinyDivUTiny ,&SQLVariant::OperUTinyDivSBig ,&SQLVariant::OperUTinyDivUBig ,&SQLVariant::OperUTinyDivNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SBIGINT     */  ,{  &SQLVariant::OperSBigDivChar  ,&SQLVariant::OperSBigDivSShort  ,&SQLVariant::OperSBigDivUShort  ,&SQLVariant::OperSBigDivSLong  ,&SQLVariant::OperSBigDivULong  ,&SQLVariant::OperSBigDivFloat  ,&SQLVariant::OperSBigDivDouble  ,&SQLVariant::OperSBigDivBit  ,&SQLVariant::OperSBigDivSTiny  ,&SQLVariant::OperSBigDivUTiny  ,&SQLVariant::OperSBigDivSBig  ,&SQLVariant::OperSBigDivUBig  ,&SQLVariant::OperSBigDivNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_UBIGINT     */  ,{  &SQLVariant::OperUBigDivChar  ,&SQLVariant::OperUBigDivSShort  ,&SQLVariant::OperUBigDivUShort  ,&SQLVariant::OperUBigDivSLong  ,&SQLVariant::OperUBigDivULong  ,&SQLVariant::OperUBigDivFloat  ,&SQLVariant::OperUBigDivDouble  ,&SQLVariant::OperUBigDivBit  ,&SQLVariant::OperUBigDivSTiny  ,&SQLVariant::OperUBigDivUTiny  ,&SQLVariant::OperUBigDivSBig  ,&SQLVariant::OperUBigDivUBig  ,&SQLVariant::OperUBigDivNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_NUMERIC     */  ,{  &SQLVariant::OperNumDivChar   ,&SQLVariant::OperNumDivSShort   ,&SQLVariant::OperNumDivUShort   ,&SQLVariant::OperNumDivSLong   ,&SQLVariant::OperNumDivULong   ,&SQLVariant::OperNumDivFloat   ,&SQLVariant::OperNumDivDouble   ,&SQLVariant::OperNumDivBit   ,&SQLVariant::OperNumDivSTiny   ,&SQLVariant::OperNumDivUTiny   ,&SQLVariant::OperNumDivSBig   ,&SQLVariant::OperNumDivUBig   ,&SQLVariant::OperNumDivNum   ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_GUID        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                      ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_BINARY      */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                      ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_DATE        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                      ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_TIME        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                      ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_TIMESTAMP   */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                      ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_INTERVAL_YM */  ,{  nullptr                       ,&SQLVariant::OperIntYMDivSShort ,&SQLVariant::OperIntYMDivUShort ,&SQLVariant::OperIntYMDivSLong ,&SQLVariant::OperIntYMDivULong ,&SQLVariant::OperIntYMDivFloat ,&SQLVariant::OperIntYMDivDouble ,&SQLVariant::OperIntYMDivBit ,&SQLVariant::OperIntYMDivSTiny ,&SQLVariant::OperIntYMDivUTiny ,&SQLVariant::OperIntYMDivSBig ,&SQLVariant::OperIntYMDivUBig ,&SQLVariant::OperIntYMDivNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_INTERVAL_DS */  ,{  nullptr                       ,&SQLVariant::OperIntDSDivSShort ,&SQLVariant::OperIntDSDivUShort ,&SQLVariant::OperIntDSDivSLong ,&SQLVariant::OperIntDSDivULong ,&SQLVariant::OperIntDSDivFloat ,&SQLVariant::OperIntDSDivDouble ,&SQLVariant::OperIntDSDivBit ,&SQLVariant::OperIntDSDivSTiny ,&SQLVariant::OperIntDSDivUTiny ,&SQLVariant::OperIntDSDivSBig ,&SQLVariant::OperIntDSDivUBig ,&SQLVariant::OperIntDSDivNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
};

// Division operator for SQLVariant
SQLVariant
SQLVariant::operator/(SQLVariant& p_right)
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

  // Check whether there is something to divide by!
  if(p_right.IsEmpty())
  {
    throw CString("Cannot do a division by zero/empty");
  }

  // Find our comparison function
  OperatorCalculate function = OperatorDiv[left][right].function;
  if(function)
  {
    return (this->*(function))(p_right);
  }
  // No compare function found
  // Data types are not comparable
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the division operator on (%s / %s)",leftType,rightType);
  throw error;
}

SQLVariant 
SQLVariant::OperSShortDivChar(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortDivChar(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongDivChar(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}
SQLVariant 
SQLVariant::OperULongDivChar(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatDivChar(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleDivChar(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitDivChar(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyDivChar(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyDivChar(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigDivChar(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigDivChar(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumDivChar(SQLVariant& p_right)
{
  bcd num = GetAsBCD() / bcd(p_right.GetAsChar());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

// TYPE == SSHORT

SQLVariant 
SQLVariant::OperCharDivSShort(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortDivSShort(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortDivSShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant SQLVariant::OperSLongDivSShort(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongDivSShort(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatDivSShort(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleDivSShort(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitDivSShort(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyDivSShort(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyDivSShort(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigDivSShort(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigDivSShort(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumDivSShort(SQLVariant& p_right)
{
  bcd num = GetAsBCD() / bcd(p_right.GetAsSShort());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMDivSShort(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSDivSShort(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == USHORT

SQLVariant 
SQLVariant::OperCharDivUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortDivUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortDivUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}


SQLVariant 
SQLVariant::OperSLongDivUShort(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUlongDivUShort(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatDivUShort(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleDivUShort(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitDivUShort(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyDivUShort(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyDivUShort(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigDivUShort(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigDivUShort(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumDivUShort(SQLVariant& p_right)
{
  bcd num = GetAsBCD() / bcd((long)p_right.GetAsUShort());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMDivUShort(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSDivUShort(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == SLONG

SQLVariant 
SQLVariant::OperCharDivSLong(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortDivSLong(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortDivSLong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongDivSLong(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongDivSLong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatDivSLong(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleDivSLong(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitDivSLong(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyDivSLong(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyDivSLong(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigDivSLong(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigDivSLong(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumDivSLong(SQLVariant& p_right)
{
  bcd num = GetAsBCD() / bcd(p_right.GetAsSLong());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMDivSLong(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSDivSLong(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == ULONG

SQLVariant 
SQLVariant::OperCharDivULong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortDivULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortDivULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongDivULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongDivULong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatDivULong(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleDivULong(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitDivULong(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyDivULong(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyDivULong(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigDivULong(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigDivULong(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumDivULong(SQLVariant& p_right)
{
  bcd num = GetAsBCD() / bcd((int64)p_right.GetAsULong());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMDivULong(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSDivULong(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == FLOAT

SQLVariant 
SQLVariant::OperCharDivFloat(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortDivFloat(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortDivFloat(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongDivFloat(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongDivFloat(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatDivFloat(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleDivFloat(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitDivFloat(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyDivFloat(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyDivFloat(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigDivFloat(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigDivFloat(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumDivFloat(SQLVariant& p_right)
{
  bcd num = GetAsBCD() / bcd((double)p_right.GetAsFloat());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMDivFloat(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSDivFloat(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}

// TYPE == DOUBLE

SQLVariant 
SQLVariant::OperCharDivDouble(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortDivDouble(SQLVariant& p_right)
{
  short result = GetAsUShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortDivDouble(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongDivDouble(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongDivDouble(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatDivDouble(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleDivDouble(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitDivDouble(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyDivDouble(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyDivDouble(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigDivDouble(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigDivDouble(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumDivDouble(SQLVariant& p_right)
{
  bcd num = GetAsBCD() / bcd(p_right.GetAsDouble());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant
SQLVariant::OperIntYMDivDouble(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant
SQLVariant::OperIntDSDivDouble(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}


// TYPE == BIT

SQLVariant 
SQLVariant::OperCharDivBit(SQLVariant& /*p_right*/)
{
  bool result = GetAsBit() != 0;
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortDivBit(SQLVariant& /*p_right*/)
{
  short result = GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortDivBit(SQLVariant& /*p_right*/)
{
  unsigned short result = GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongDivBit(SQLVariant& /*p_right*/)
{
  long result = GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongDivBit(SQLVariant& /*p_right*/)
{
  unsigned long result = GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatDivBit(SQLVariant& /*p_right*/)
{
  float result = GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleDivBit(SQLVariant& /*p_right*/)
{
  double result = GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitDivBit(SQLVariant& /*p_right*/)
{
  bool result = GetAsBit() != 0;
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyDivBit(SQLVariant& /*p_right*/)
{
  char result = GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyDivBit(SQLVariant& /*p_right*/)
{
  unsigned char result = GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigDivBit(SQLVariant& /*p_right*/)
{
  SQLBIGINT result = GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigDivBit(SQLVariant& /*p_right*/)
{
  SQLUBIGINT result = GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumDivBit(SQLVariant& /*p_right*/)
{
  SQLVariant result(this);
  return result;
}

SQLVariant 
SQLVariant::OperIntYMDivBit(SQLVariant& /*p_right*/)
{
  SQLInterval result = GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSDivBit(SQLVariant& /*p_right*/)
{
  SQLInterval result = GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == STINYINT

SQLVariant 
SQLVariant::OperCharDivSTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortDivSTiny(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortDivSTiny(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongDivSTiny(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongDivSTiny(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatDivSTiny(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleDivSTiny(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitDivSTiny(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyDivSTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyDivSTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigDivSTiny(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigDivSTiny(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumDivSTiny(SQLVariant& p_right)
{
  bcd num = GetAsBCD() / bcd(p_right.GetAsSTinyInt());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMDivSTiny(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSDivSTiny(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE = UTINYINT

SQLVariant 
SQLVariant::OperCharDivUTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortDivUTiny(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortDivUTiny(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongDivUTiny(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongDivUTiny(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatDivUTiny(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleDivUTiny(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitDivUTiny(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyDivUTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyDivUTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigDivUTiny(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigDivUTiny(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumDivUTiny(SQLVariant& p_right)
{
  bcd num = GetAsBCD() / bcd((short)p_right.GetAsUTinyInt());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant
SQLVariant::OperIntYMDivUTiny(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
SQLVariant::OperIntDSDivUTiny(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == SBIGINT

SQLVariant 
SQLVariant::OperCharDivSBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortDivSBig(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortDivSBig(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongDivSBig(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongDivSBig(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatDivSBig(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleDivSBig(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitDivSBig(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyDivSBig(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyDivSBig(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigDivSBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigDivSBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumDivSBig(SQLVariant& p_right)
{
  bcd num = GetAsBCD() / bcd(p_right.GetAsSBigInt());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMDivSBig(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSDivSBig(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == UBIGINT

SQLVariant 
SQLVariant::OperCharDivUBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortDivUBig(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortDivUBig(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongDivUBig(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongDivUBig(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatDivUBig(SQLVariant& p_right)
{
  float result = GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleDivUBig(SQLVariant& p_right)
{
  double result = GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitDivUBig(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyDivUBig(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyDivUBig(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigDivUBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigDivUBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumDivUBig(SQLVariant& p_right)
{
  bcd num = GetAsBCD() / bcd(p_right.GetAsUBigInt());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant
SQLVariant::OperIntYMDivUBig(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
SQLVariant::OperIntDSDivUBig(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == NUMERIC

SQLVariant 
SQLVariant::OperCharDivNum(SQLVariant& p_right)
{
  double num = GetAsDouble() / p_right.GetAsBCD().AsDouble();
  CString str;
  str.Format("%lf",num);
  SQLVariant var(str);
  return var;
}

SQLVariant 
SQLVariant::OperSShortDivNum(SQLVariant& p_right)
{
  short num = GetAsSShort() / (short) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperUShortDivNum(SQLVariant& p_right)
{
  unsigned short num = GetAsUShort() / (unsigned short)p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperSLongDivNum(SQLVariant& p_right)
{
  long num = GetAsSLong() / p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperULongDivNum(SQLVariant& p_right)
{
  unsigned long num = GetAsULong() / (unsigned long)p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperFloatDivNum(SQLVariant& p_right)
{
  float num = GetAsFloat() / (float)p_right.GetAsBCD().AsDouble();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperDoubleDivNum(SQLVariant& p_right)
{
  double num = GetAsDouble() / p_right.GetAsBCD().AsDouble();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperBitDivNum(SQLVariant& /*p_right*/)
{
  SQLVariant var(GetAsBit());
  return var;
}

SQLVariant 
SQLVariant::OperSTinyDivNum(SQLVariant& p_right)
{
  char num = GetAsSTinyInt() / (char) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperUTinyDivNum(SQLVariant& p_right)
{
  unsigned char num = GetAsUTinyInt() / (unsigned char) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperSBigDivNum(SQLVariant& p_right)
{
  int64 num = GetAsSBigInt() / p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperUBigDivNum(SQLVariant& p_right)
{
  uint64 num = GetAsUBigInt() / p_right.GetAsBCD().AsUInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperNumDivNum(SQLVariant& p_right)
{
  bcd num = GetAsBCD() / p_right.GetAsBCD();
  SQLVariant var(&num,p_right.m_data.m_dataNUMERIC.precision,p_right.m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMDivNum(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSDivNum(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}
