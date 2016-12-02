////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorMod.cpp
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
SQLVariant::OperatorMod[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                        CT_SSHORT                        CT_USHORT                        CT_SLONG                        CT_ULONG                        CT_FLOAT                        CT_DOUBLE                        CT_BIT                        CT_STINYINT                     CT_UTINYINT                     CT_SBIGINT                     CT_UBIGINT                     CT_NUMERIC                    CT_GUID  CT_BINARY CT_DATE  CT_TIME  CT_TIMESTAMP CT_INTERVAL_YM CT_INTERVAL_DS
                         // ------------------------------ -------------------------------- -------------------------------- ------------------------------- ------------------------------- ------------------------------- -------------------------------- ----------------------------- ------------------------------- ------------------------------- ------------------------------ ------------------------------ ----------------------------- -------- --------- -------- -------- ------------ -------------- --------------
  /* CT_CHAR        */   {  nullptr                       ,&SQLVariant::OperCharModSShort  ,&SQLVariant::OperCharModUShort  ,&SQLVariant::OperCharModSLong  ,&SQLVariant::OperCharModULong  ,&SQLVariant::OperCharModFloat  ,&SQLVariant::OperCharModDouble  ,&SQLVariant::OperCharModBit  ,&SQLVariant::OperCharModSTiny  ,&SQLVariant::OperCharModUTiny  ,&SQLVariant::OperCharModSBig  ,&SQLVariant::OperCharModUBig  ,&SQLVariant::OperCharModNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SSHORT      */  ,{  &SQLVariant::OperSShortModChar,&SQLVariant::OperSShortModSShort,&SQLVariant::OperSShortModUShort,&SQLVariant::OperSShortModSLong,&SQLVariant::OperSShortModULong,&SQLVariant::OperSShortModFloat,&SQLVariant::OperSShortModDouble,&SQLVariant::OperSShortModBit,&SQLVariant::OperSShortModSTiny,&SQLVariant::OperSShortModUTiny,&SQLVariant::OperSShortModSBig,&SQLVariant::OperSShortModUBig,&SQLVariant::OperSShortModNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_USHORT      */  ,{  &SQLVariant::OperUShortModChar,&SQLVariant::OperUShortModSShort,&SQLVariant::OperUShortModUShort,&SQLVariant::OperUShortModSLong,&SQLVariant::OperUShortModULong,&SQLVariant::OperUShortModFloat,&SQLVariant::OperUShortModDouble,&SQLVariant::OperUShortModBit,&SQLVariant::OperUShortModSTiny,&SQLVariant::OperUShortModUTiny,&SQLVariant::OperUShortModSBig,&SQLVariant::OperUShortModUBig,&SQLVariant::OperUShortModNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SLONG       */  ,{  &SQLVariant::OperSLongModChar ,&SQLVariant::OperSLongModSShort ,&SQLVariant::OperSLongModUShort ,&SQLVariant::OperSLongModSLong ,&SQLVariant::OperSLongModULong ,&SQLVariant::OperSLongModFloat ,&SQLVariant::OperSLongModDouble ,&SQLVariant::OperSLongModBit ,&SQLVariant::OperSLongModSTiny ,&SQLVariant::OperSLongModUTiny ,&SQLVariant::OperSLongModSBig ,&SQLVariant::OperSLongModUBig ,&SQLVariant::OperSLongModNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_ULONG       */  ,{  &SQLVariant::OperULongModChar ,&SQLVariant::OperULongModSShort ,&SQLVariant::OperUlongModUShort ,&SQLVariant::OperULongModSLong ,&SQLVariant::OperULongModULong ,&SQLVariant::OperULongModFloat ,&SQLVariant::OperULongModDouble ,&SQLVariant::OperULongModBit ,&SQLVariant::OperULongModSTiny ,&SQLVariant::OperULongModUTiny ,&SQLVariant::OperULongModSBig ,&SQLVariant::OperULongModUBig ,&SQLVariant::OperULongModNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_FLOAT       */  ,{  &SQLVariant::OperFloatModChar ,&SQLVariant::OperFloatModSShort ,&SQLVariant::OperFloatModUShort ,&SQLVariant::OperFloatModSLong ,&SQLVariant::OperFloatModULong ,&SQLVariant::OperFloatModFloat ,&SQLVariant::OperFloatModDouble ,&SQLVariant::OperFloatModBit ,&SQLVariant::OperFloatModSTiny ,&SQLVariant::OperFloatModUTiny ,&SQLVariant::OperFloatModSBig ,&SQLVariant::OperFloatModUBig ,&SQLVariant::OperFloatModNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_DOUBLE      */  ,{  &SQLVariant::OperDoubleModChar,&SQLVariant::OperDoubleModSShort,&SQLVariant::OperDoubleModUShort,&SQLVariant::OperDoubleModSLong,&SQLVariant::OperDoubleModULong,&SQLVariant::OperDoubleModFloat,&SQLVariant::OperDoubleModDouble,&SQLVariant::OperDoubleModBit,&SQLVariant::OperDoubleModSTiny,&SQLVariant::OperDoubleModUTiny,&SQLVariant::OperDoubleModSBig,&SQLVariant::OperDoubleModUBig,&SQLVariant::OperDoubleModNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_BIT         */  ,{  &SQLVariant::OperBitModChar   ,&SQLVariant::OperBitModSShort   ,&SQLVariant::OperBitModUShort   ,&SQLVariant::OperBitModSLong   ,&SQLVariant::OperBitModULong   ,&SQLVariant::OperBitModFloat   ,&SQLVariant::OperBitModDouble   ,&SQLVariant::OperBitModBit   ,&SQLVariant::OperBitModSTiny   ,&SQLVariant::OperBitModUTiny   ,&SQLVariant::OperBitModSBig   ,&SQLVariant::OperBitModUBig   ,&SQLVariant::OperBitModNum   ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_STINYINT    */  ,{  &SQLVariant::OperSTinyModChar ,&SQLVariant::OperSTinyModSShort ,&SQLVariant::OperSTinyModUShort ,&SQLVariant::OperSTinyModSLong ,&SQLVariant::OperSTinyModULong ,&SQLVariant::OperSTinyModFloat ,&SQLVariant::OperSTinyModDouble ,&SQLVariant::OperSTinyModBit ,&SQLVariant::OperSTinyModSTiny ,&SQLVariant::OperSTinyModUTiny ,&SQLVariant::OperSTinyModSBig ,&SQLVariant::OperSTinyModUBig ,&SQLVariant::OperSTinyModNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_UTINYINT    */  ,{  &SQLVariant::OperUTinyModChar ,&SQLVariant::OperUTinyModSShort ,&SQLVariant::OperUTinyModUShort ,&SQLVariant::OperUTinyModSLong ,&SQLVariant::OperUTinyModULong ,&SQLVariant::OperUTinyModFloat ,&SQLVariant::OperUTinyModDouble ,&SQLVariant::OperUTinyModBit ,&SQLVariant::OperUTinyModSTiny ,&SQLVariant::OperUTinyModUTiny ,&SQLVariant::OperUTinyModSBig ,&SQLVariant::OperUTinyModUBig ,&SQLVariant::OperUTinyModNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SBIGINT     */  ,{  &SQLVariant::OperSBigModChar  ,&SQLVariant::OperSBigModSShort  ,&SQLVariant::OperSBigModUShort  ,&SQLVariant::OperSBigModSLong  ,&SQLVariant::OperSBigModULong  ,&SQLVariant::OperSBigModFloat  ,&SQLVariant::OperSBigModDouble  ,&SQLVariant::OperSBigModBit  ,&SQLVariant::OperSBigModSTiny  ,&SQLVariant::OperSBigModUTiny  ,&SQLVariant::OperSBigModSBig  ,&SQLVariant::OperSBigModUBig  ,&SQLVariant::OperSBigModNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_UBIGINT     */  ,{  &SQLVariant::OperUBigModChar  ,&SQLVariant::OperUBigModSShort  ,&SQLVariant::OperUBigModUShort  ,&SQLVariant::OperUBigModSLong  ,&SQLVariant::OperUBigModULong  ,&SQLVariant::OperUBigModFloat  ,&SQLVariant::OperUBigModDouble  ,&SQLVariant::OperUBigModBit  ,&SQLVariant::OperUBigModSTiny  ,&SQLVariant::OperUBigModUTiny  ,&SQLVariant::OperUBigModSBig  ,&SQLVariant::OperUBigModUBig  ,&SQLVariant::OperUBigModNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_NUMERIC     */  ,{  &SQLVariant::OperNumModChar   ,&SQLVariant::OperNumModSShort   ,&SQLVariant::OperNumModUShort   ,&SQLVariant::OperNumModSLong   ,&SQLVariant::OperNumModULong   ,&SQLVariant::OperNumModFloat   ,&SQLVariant::OperNumModDouble   ,&SQLVariant::OperNumModBit   ,&SQLVariant::OperNumModSTiny   ,&SQLVariant::OperNumModUTiny   ,&SQLVariant::OperNumModSBig   ,&SQLVariant::OperNumModUBig   ,&SQLVariant::OperNumModNum   ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_GUID        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                      ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_BINARY      */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                      ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_DATE        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                      ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_TIME        */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                      ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_TIMESTAMP   */  ,{  nullptr                       ,nullptr                         ,nullptr                         ,nullptr                        ,nullptr                        ,nullptr                        ,nullptr                         ,nullptr                      ,nullptr                        ,nullptr                        ,nullptr                       ,nullptr                       ,nullptr                      ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_INTERVAL_YM */  ,{  nullptr                       ,&SQLVariant::OperIntYMModSShort ,&SQLVariant::OperIntYMModUShort ,&SQLVariant::OperIntYMModSLong ,&SQLVariant::OperIntYMModULong ,&SQLVariant::OperIntYMModFloat ,&SQLVariant::OperIntYMModDouble ,&SQLVariant::OperIntYMModBit ,&SQLVariant::OperIntYMModSTiny ,&SQLVariant::OperIntYMModUTiny ,&SQLVariant::OperIntYMModSBig ,&SQLVariant::OperIntYMModUBig ,&SQLVariant::OperIntYMModNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_INTERVAL_DS */  ,{  nullptr                       ,&SQLVariant::OperIntDSModSShort ,&SQLVariant::OperIntDSModUShort ,&SQLVariant::OperIntDSModSLong ,&SQLVariant::OperIntDSModULong ,&SQLVariant::OperIntDSModFloat ,&SQLVariant::OperIntDSModDouble ,&SQLVariant::OperIntDSModBit ,&SQLVariant::OperIntDSModSTiny ,&SQLVariant::OperIntDSModUTiny ,&SQLVariant::OperIntDSModSBig ,&SQLVariant::OperIntDSModUBig ,&SQLVariant::OperIntDSModNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
};

// Modulo operator for SQLVariant
SQLVariant
SQLVariant::operator%(SQLVariant& p_right)
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

  // Check whether there is something to modulo by!
  if(p_right.IsEmpty())
  {
    throw CString("Cannot do a modulo by zero/empty");
  }

  // Find our comparison function
  OperatorCalculate function = OperatorMod[left][right].function;
  if(function)
  {
    return (this->*(function))(p_right);
  }
  // No compare function found
  // Data types are not comparable
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the modulo operator on (%s % %s)",leftType,rightType);
  throw error;
}

SQLVariant 
SQLVariant::OperSShortModChar(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortModChar(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongModChar(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}
SQLVariant 
SQLVariant::OperULongModChar(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatModChar(SQLVariant& p_right)
{
  float result = fmod(GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleModChar(SQLVariant& p_right)
{
  double result = fmod(GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitModChar(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyModChar(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyModChar(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigModChar(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigModChar(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt() % p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumModChar(SQLVariant& p_right)
{
  bcd num = GetAsBCD() % bcd(p_right.GetAsChar());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

// TYPE == SSHORT

SQLVariant 
SQLVariant::OperCharModSShort(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortModSShort(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortModSShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant SQLVariant::OperSLongModSShort(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongModSShort(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatModSShort(SQLVariant& p_right)
{
  float result = fmod(GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleModSShort(SQLVariant& p_right)
{
  double result = fmod(GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitModSShort(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyModSShort(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyModSShort(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigModSShort(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigModSShort(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumModSShort(SQLVariant& p_right)
{
  bcd num = GetAsBCD() % bcd(p_right.GetAsSShort());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMModSShort(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSModSShort(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == USHORT

SQLVariant 
SQLVariant::OperCharModUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortModUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortModUShort(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}


SQLVariant 
SQLVariant::OperSLongModUShort(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUlongModUShort(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatModUShort(SQLVariant& p_right)
{
  float result = fmod(GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleModUShort(SQLVariant& p_right)
{
  double result = fmod(GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitModUShort(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyModUShort(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyModUShort(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigModUShort(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigModUShort(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumModUShort(SQLVariant& p_right)
{
  bcd num = GetAsBCD() % bcd((int)p_right.GetAsUShort());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMModUShort(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSModUShort(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == SLONG

SQLVariant 
SQLVariant::OperCharModSLong(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortModSLong(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortModSLong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongModSLong(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongModSLong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatModSLong(SQLVariant& p_right)
{
  float result = fmod(GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleModSLong(SQLVariant& p_right)
{
  double result = fmod(GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitModSLong(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyModSLong(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyModSLong(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigModSLong(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigModSLong(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumModSLong(SQLVariant& p_right)
{
  bcd num = GetAsBCD() % bcd(p_right.GetAsSLong());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMModSLong(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSModSLong(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == ULONG

SQLVariant 
SQLVariant::OperCharModULong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortModULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortModULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongModULong(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongModULong(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatModULong(SQLVariant& p_right)
{
  float result = fmod(GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleModULong(SQLVariant& p_right)
{
  double result = fmod(GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitModULong(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyModULong(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyModULong(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigModULong(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigModULong(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumModULong(SQLVariant& p_right)
{
  bcd num = GetAsBCD() % bcd((int64)p_right.GetAsUShort());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMModULong(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSModULong(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == FLOAT

SQLVariant 
SQLVariant::OperCharModFloat(SQLVariant& p_right)
{
  float result = fmod(GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortModFloat(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortModFloat(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongModFloat(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongModFloat(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatModFloat(SQLVariant& p_right)
{
  float result = fmod(GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleModFloat(SQLVariant& p_right)
{
  double result = fmod(GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitModFloat(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyModFloat(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyModFloat(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigModFloat(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigModFloat(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumModFloat(SQLVariant& p_right)
{
  bcd num = GetAsBCD() % bcd((double)p_right.GetAsFloat());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMModFloat(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSModFloat(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}

// TYPE == DOUBLE

SQLVariant 
SQLVariant::OperCharModDouble(SQLVariant& p_right)
{
  double result = fmod(GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortModDouble(SQLVariant& p_right)
{
  short result = GetAsUShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortModDouble(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongModDouble(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongModDouble(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatModDouble(SQLVariant& p_right)
{
  float result = fmod(GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleModDouble(SQLVariant& p_right)
{
  double result = fmod(GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitModDouble(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyModDouble(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyModDouble(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigModDouble(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigModDouble(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumModDouble(SQLVariant& p_right)
{
  bcd num = GetAsBCD() % bcd(p_right.GetAsDouble());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant
SQLVariant::OperIntYMModDouble(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant
SQLVariant::OperIntDSModDouble(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}


// TYPE == BIT

SQLVariant 
SQLVariant::OperCharModBit(SQLVariant& /*p_right*/)
{
  bool result = GetAsBit() != 0;
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortModBit(SQLVariant& /*p_right*/)
{
  short result = GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortModBit(SQLVariant& /*p_right*/)
{
  unsigned short result = GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongModBit(SQLVariant& /*p_right*/)
{
  long result = GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongModBit(SQLVariant& /*p_right*/)
{
  unsigned long result = GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatModBit(SQLVariant& /*p_right*/)
{
  float result = GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleModBit(SQLVariant& /*p_right*/)
{
  double result = GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitModBit(SQLVariant& /*p_right*/)
{
  bool result = GetAsBit() != 0;
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyModBit(SQLVariant& /*p_right*/)
{
  char result = GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyModBit(SQLVariant& /*p_right*/)
{
  unsigned char result = GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigModBit(SQLVariant& /*p_right*/)
{
  SQLBIGINT result = GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigModBit(SQLVariant& /*p_right*/)
{
  SQLUBIGINT result = GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumModBit(SQLVariant& /*p_right*/)
{
  SQLVariant result(this);
  return result;
}

SQLVariant 
SQLVariant::OperIntYMModBit(SQLVariant& /*p_right*/)
{
  SQLInterval result = GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSModBit(SQLVariant& /*p_right*/)
{
  SQLInterval result = GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == STINYINT

SQLVariant 
SQLVariant::OperCharModSTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortModSTiny(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortModSTiny(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongModSTiny(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongModSTiny(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatModSTiny(SQLVariant& p_right)
{
  float result = fmod(GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleModSTiny(SQLVariant& p_right)
{
  double result = fmod(GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitModSTiny(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyModSTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyModSTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigModSTiny(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigModSTiny(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumModSTiny(SQLVariant& p_right)
{
  bcd num = GetAsBCD() % bcd(p_right.GetAsSTinyInt());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMModSTiny(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSModSTiny(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE = UTINYINT

SQLVariant 
SQLVariant::OperCharModUTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortModUTiny(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortModUTiny(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongModUTiny(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongModUTiny(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatModUTiny(SQLVariant& p_right)
{
  float result = fmod(GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleModUTiny(SQLVariant& p_right)
{
  double result = fmod(GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitModUTiny(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyModUTiny(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyModUTiny(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigModUTiny(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigModUTiny(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumModUTiny(SQLVariant& p_right)
{
  bcd num = GetAsBCD() % bcd((int)p_right.GetAsUTinyInt());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant
SQLVariant::OperIntYMModUTiny(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
SQLVariant::OperIntDSModUTiny(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == SBIGINT

SQLVariant 
SQLVariant::OperCharModSBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortModSBig(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortModSBig(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongModSBig(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongModSBig(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatModSBig(SQLVariant& p_right)
{
  float result = fmod(GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleModSBig(SQLVariant& p_right)
{
  double result = fmod(GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitModSBig(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyModSBig(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyModSBig(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigModSBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigModSBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumModSBig(SQLVariant& p_right)
{
  bcd num = GetAsBCD() % bcd(p_right.GetAsSBigInt());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMModSBig(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSModSBig(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == UBIGINT

SQLVariant 
SQLVariant::OperCharModUBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSShortModUBig(SQLVariant& p_right)
{
  short result = GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUShortModUBig(SQLVariant& p_right)
{
  unsigned short result = GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSLongModUBig(SQLVariant& p_right)
{
  long result = GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperULongModUBig(SQLVariant& p_right)
{
  unsigned long result = GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperFloatModUBig(SQLVariant& p_right)
{
  float result = fmod(GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperDoubleModUBig(SQLVariant& p_right)
{
  double result = fmod(GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperBitModUBig(SQLVariant& p_right)
{
  bool result = false;
  if(GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSTinyModUBig(SQLVariant& p_right)
{
  char result = GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUTinyModUBig(SQLVariant& p_right)
{
  unsigned char result = GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperSBigModUBig(SQLVariant& p_right)
{
  SQLBIGINT result = GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperUBigModUBig(SQLVariant& p_right)
{
  SQLUBIGINT result = GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQLVariant::OperNumModUBig(SQLVariant& p_right)
{
  bcd num = GetAsBCD() % bcd(p_right.GetAsUBigInt());
  SQLVariant var(&num,m_data.m_dataNUMERIC.precision,m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant
SQLVariant::OperIntYMModUBig(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
SQLVariant::OperIntDSModUBig(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == NUMERIC

SQLVariant 
SQLVariant::OperCharModNum(SQLVariant& p_right)
{
  double num = fmod(GetAsDouble(),p_right.GetAsBCD().AsDouble());
  CString str;
  str.Format("%lf",num);
  SQLVariant var(str);
  return var;
}

SQLVariant 
SQLVariant::OperSShortModNum(SQLVariant& p_right)
{
  short num = GetAsSShort() % p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperUShortModNum(SQLVariant& p_right)
{
  unsigned short num = GetAsUShort() % p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperSLongModNum(SQLVariant& p_right)
{
  long num = GetAsSLong() % p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperULongModNum(SQLVariant& p_right)
{
  unsigned long num = GetAsULong() % p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperFloatModNum(SQLVariant& p_right)
{
  float num = (float) fmod(GetAsFloat(),p_right.GetAsBCD().AsDouble());
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperDoubleModNum(SQLVariant& p_right)
{
  double num = fmod(GetAsDouble(),p_right.GetAsBCD().AsDouble());
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperBitModNum(SQLVariant& /*p_right*/)
{
  SQLVariant var(GetAsBit());
  return var;
}

SQLVariant 
SQLVariant::OperSTinyModNum(SQLVariant& p_right)
{
  char num = GetAsSTinyInt() % (char) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperUTinyModNum(SQLVariant& p_right)
{
  unsigned char num = GetAsUTinyInt() % (unsigned char)p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperSBigModNum(SQLVariant& p_right)
{
  int64 num = GetAsSBigInt() % p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperUBigModNum(SQLVariant& p_right)
{
  uint64 num = GetAsUBigInt() % p_right.GetAsBCD().AsUInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQLVariant::OperNumModNum(SQLVariant& p_right)
{
  bcd num = GetAsBCD() % p_right.GetAsBCD();
  SQLVariant var(&num,p_right.m_data.m_dataNUMERIC.precision,p_right.m_data.m_dataNUMERIC.scale);
  return var;
}

SQLVariant 
SQLVariant::OperIntYMModNum(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
SQLVariant::OperIntDSModNum(SQLVariant& p_right)
{
  SQLInterval result = GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}
