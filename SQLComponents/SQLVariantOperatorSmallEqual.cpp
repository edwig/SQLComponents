////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorSmallEQ.cpp
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

CompareFunctionArray
SQLVariant::OperatorSmallerEqual[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                          CT_SSHORT                            CT_USHORT                            CT_SLONG                             CT_ULONG                            CT_FLOAT                            CT_DOUBLE                            CT_BIT                            CT_STINYINT                         CT_UTINYINT                         CT_SBIGINT                         CT_UBIGINT                         CT_NUMERIC                        CT_GUID                          CT_BINARY                            CT_DATE                           CT_TIME                           CT_TIMESTAMP                       CT_INTERVAL_YM                     CT_INTERVAL_DS
                         // -------------------------------- ------------------------------------ ------------------------------------ ------------------------------------ ----------------------------------- ----------------------------------- ------------------------------------ --------------------------------- ----------------------------------- ----------------------------------- ---------------------------------- ---------------------------------- --------------------------------- -------------------------------- ------------------------------------ --------------------------------- --------------------------------- ---------------------------------- ---------------------------------- -----------------------------------
  /* CT_CHAR        */   {  &SQLVariant::OperVarSmallEQChar ,&SQLVariant::OperVarSmallEQChar     ,&SQLVariant::OperVarSmallEQChar     ,&SQLVariant::OperVarSmallEQChar     ,&SQLVariant::OperVarSmallEQChar    ,&SQLVariant::OperVarSmallEQChar    ,&SQLVariant::OperVarSmallEQChar     ,&SQLVariant::OperVarSmallEQChar  ,&SQLVariant::OperVarSmallEQChar    ,&SQLVariant::OperVarSmallEQChar    ,&SQLVariant::OperVarSmallEQChar   ,&SQLVariant::OperVarSmallEQChar   ,&SQLVariant::OperVarSmallEQChar  ,&SQLVariant::OperVarSmallEQChar ,nullptr                             ,&SQLVariant::OperVarSmallEQChar  ,&SQLVariant::OperVarSmallEQChar  ,&SQLVariant::OperVarSmallEQChar   ,&SQLVariant::OperVarSmallEQChar   ,&SQLVariant::OperVarSmallEQChar    }
  /* CT_SSHORT      */  ,{  &SQLVariant::OperVarSmallEQChar ,&SQLVariant::OperSShortSmallEQSShort,&SQLVariant::OperSShortSmallEQUShort,&SQLVariant::OperSShortSmallEQSLong ,&SQLVariant::OperSShortSmallEQULong,&SQLVariant::OperSShortSmallEQFloat,&SQLVariant::OperSShortSmallEQDouble,&SQLVariant::OperSShortSmallEQBit,&SQLVariant::OperSShortSmallEQSTiny,&SQLVariant::OperSShortSmallEQUTiny,&SQLVariant::OperSShortSmallEQSBig,&SQLVariant::OperSShortSmallEQUBig,&SQLVariant::OperSShortSmallEQNum,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_USHORT      */  ,{  &SQLVariant::OperVarSmallEQChar ,&SQLVariant::OperUShortSmallEQSShort,&SQLVariant::OperUShortSmallEQUShort,&SQLVariant::OperUShortSmallEQSLong ,&SQLVariant::OperUShortSmallEQULong,&SQLVariant::OperUShortSmallEQFloat,&SQLVariant::OperUShortSmallEQDouble,&SQLVariant::OperUShortSmallEQBit,&SQLVariant::OperUShortSmallEQSTiny,&SQLVariant::OperUShortSmallEQUTiny,&SQLVariant::OperUShortSmallEQSBig,&SQLVariant::OperUShortSmallEQUBig,&SQLVariant::OperUShortSmallEQNum,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_SLONG       */  ,{  &SQLVariant::OperVarSmallEQChar ,&SQLVariant::OperSLongSmallEQSShort ,&SQLVariant::OperSLongSmallEQUShort ,&SQLVariant::OperSLongSmallEQSLong  ,&SQLVariant::OperSLongSmallEQULong ,&SQLVariant::OperSLongSmallEQFloat ,&SQLVariant::OperSLongSmallEQDouble ,&SQLVariant::OperSLongSmallEQBit ,&SQLVariant::OperSLongSmallEQSTiny ,&SQLVariant::OperSLongSmallEQUTiny ,&SQLVariant::OperSLongSmallEQSBig ,&SQLVariant::OperSLongSmallEQUBig ,&SQLVariant::OperSLongSmallEQNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_ULONG       */  ,{  &SQLVariant::OperVarSmallEQChar ,&SQLVariant::OperULongSmallEQSShort ,&SQLVariant::OperUlongSmallEQUShort ,&SQLVariant::OperULongSmallEQSLong  ,&SQLVariant::OperULongSmallEQULong ,&SQLVariant::OperULongSmallEQFloat ,&SQLVariant::OperULongSmallEQDouble ,&SQLVariant::OperULongSmallEQBit ,&SQLVariant::OperULongSmallEQSTiny ,&SQLVariant::OperULongSmallEQUTiny ,&SQLVariant::OperULongSmallEQSBig ,&SQLVariant::OperULongSmallEQUBig ,&SQLVariant::OperULongSmallEQNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_FLOAT       */  ,{  &SQLVariant::OperVarSmallEQChar ,&SQLVariant::OperFloatSmallEQSShort ,&SQLVariant::OperFloatSmallEQUShort ,&SQLVariant::OperFloatSmallEQSLong  ,&SQLVariant::OperFloatSmallEQULong ,&SQLVariant::OperFloatSmallEQFloat ,&SQLVariant::OperFloatSmallEQDouble ,&SQLVariant::OperFloatSmallEQBit ,&SQLVariant::OperFloatSmallEQSTiny ,&SQLVariant::OperFloatSmallEQUTiny ,&SQLVariant::OperFloatSmallEQSBig ,&SQLVariant::OperFloatSmallEQUBig ,&SQLVariant::OperFloatSmallEQNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_DOUBLE      */  ,{  &SQLVariant::OperVarSmallEQChar ,&SQLVariant::OperDoubleSmallEQSShort,&SQLVariant::OperDoubleSmallEQUShort,&SQLVariant::OperDoubleESmallEQSLong,&SQLVariant::OperDoubleSmallEQULong,&SQLVariant::OperDoubleSmallEQFloat,&SQLVariant::OperDoubleSmallEQDouble,&SQLVariant::OperDoubleSmallEQBit,&SQLVariant::OperDoubleSmallEQSTiny,&SQLVariant::OperDoubleSmallEQUTiny,&SQLVariant::OperDoubleSmallEQSBig,&SQLVariant::OperDoubleSmallEQUBig,&SQLVariant::OperDoubleSmallEQNum,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_BIT         */  ,{  &SQLVariant::OperVarSmallEQChar ,&SQLVariant::OperBitSmallEQSShort   ,&SQLVariant::OperBitSmallEQUShort   ,&SQLVariant::OperBitSmallEQSLong    ,&SQLVariant::OperBitSmallEQULong   ,&SQLVariant::OperBitSmallEQFloat   ,&SQLVariant::OperBitSmallEQDouble   ,&SQLVariant::OperBitSmallEQBit   ,&SQLVariant::OperBitSmallEQSTiny   ,&SQLVariant::OperBitSmallEQUTiny   ,&SQLVariant::OperBitSmallEQSBig   ,&SQLVariant::OperBitSmallEQUBig   ,&SQLVariant::OperBitSmallEQNum   ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_STINYINT    */  ,{  &SQLVariant::OperVarSmallEQChar ,&SQLVariant::OperSTinySmallEQSShort ,&SQLVariant::OperSTinySmallEQUShort ,&SQLVariant::OperSTinySmallEQSLong  ,&SQLVariant::OperSTinySmallEQULong ,&SQLVariant::OperSTinySmallEQFloat ,&SQLVariant::OperSTinySmallEQDouble ,&SQLVariant::OperSTinySmallEQBit ,&SQLVariant::OperSTinySmallEQSTiny ,&SQLVariant::OperSTinySmallEQUTiny ,&SQLVariant::OperSTinySmallEQSBig ,&SQLVariant::OperSTinySmallEQUBig ,&SQLVariant::OperSTinySmallEQNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_UTINYINT    */  ,{  &SQLVariant::OperVarSmallEQChar ,&SQLVariant::OperUTinySmallEQSShort ,&SQLVariant::OperUTinySmallEQUShort ,&SQLVariant::OperUTinySmallEQSLong  ,&SQLVariant::OperUTinySmallEQULong ,&SQLVariant::OperUTinySmallEQFloat ,&SQLVariant::OperUTinySmallEQDouble ,&SQLVariant::OperUTinySmallEQBit ,&SQLVariant::OperUTinySmallEQSTiny ,&SQLVariant::OperUTinySmallEQUTiny ,&SQLVariant::OperUTinySmallEQSBig ,&SQLVariant::OperUTinySmallEQUBig ,&SQLVariant::OperUTinySmallEQNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_SBIGINT     */  ,{  &SQLVariant::OperVarSmallEQChar ,&SQLVariant::OperSBigSmallEQSShort  ,&SQLVariant::OperSBigSmallEQUShort  ,&SQLVariant::OperSBigSmallEQSLong   ,&SQLVariant::OperSBigSmallEQULong  ,&SQLVariant::OperSBigSmallEQFloat  ,&SQLVariant::OperSBigSmallEQDouble  ,&SQLVariant::OperSBigSmallEQBit  ,&SQLVariant::OperSBigSmallEQSTiny  ,&SQLVariant::OperSBigSmallEQUTiny  ,&SQLVariant::OperSBigSmallEQSBig  ,&SQLVariant::OperSBigSmallEQUBig  ,&SQLVariant::OperSBigSmallEQNum  ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_UBIGINT     */  ,{  &SQLVariant::OperVarSmallEQChar ,&SQLVariant::OperUBigSmallEQSShort  ,&SQLVariant::OperUBigSmallEQUShort  ,&SQLVariant::OperUBigSmallEQSLong   ,&SQLVariant::OperUBigSmallEQULong  ,&SQLVariant::OperUBigSmallEQFloat  ,&SQLVariant::OperUBigSmallEQDouble  ,&SQLVariant::OperUBigSmallEQBit  ,&SQLVariant::OperUBigSmallEQSTiny  ,&SQLVariant::OperUBigSmallEQUTiny  ,&SQLVariant::OperUBigSmallEQSBig  ,&SQLVariant::OperUBigSmallEQUBig  ,&SQLVariant::OperUBigSmallEQNum  ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_NUMERIC     */  ,{  &SQLVariant::OperVarSmallEQChar ,&SQLVariant::OperVarSmallEQChar     ,&SQLVariant::OperVarSmallEQChar     ,&SQLVariant::OperVarSmallEQChar     ,&SQLVariant::OperVarSmallEQChar    ,&SQLVariant::OperNumSmallEQFloat   ,&SQLVariant::OperNumSmallEQDouble   ,&SQLVariant::OperNumSmallEQBit   ,&SQLVariant::OperNumSmallEQSTiny   ,&SQLVariant::OperNumSmallEQUTiny   ,&SQLVariant::OperNumSmallEQSBig   ,&SQLVariant::OperNumSmallEQUBig   ,&SQLVariant::OperNumSmallEQNum   ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_GUID        */  ,{  &SQLVariant::OperGuidSmallEQChar,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,&SQLVariant::OperGuidSmallEQGuid,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_BINARY      */  ,{  nullptr                         ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,&SQLVariant::OperBinarySmallEQBinary,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_DATE        */  ,{  &SQLVariant::OperVarSmallEQChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,&SQLVariant::OperDateSmallEQDate ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_TIME        */  ,{  &SQLVariant::OperVarSmallEQChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,nullptr                          ,&SQLVariant::OperTimeSmallEQTime ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_TIMESTAMP   */  ,{  &SQLVariant::OperVarSmallEQChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,&SQLVariant::OperStampSmallEQDate,&SQLVariant::OperStampSmallEQTime,&SQLVariant::OperStampSmallEQStamp,nullptr                           ,nullptr                            }
  /* CT_INTERVAL_YM */  ,{  &SQLVariant::OperVarSmallEQChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,&SQLVariant::OperIntYMSmallEQIntYM,nullptr                            }
  /* CT_INTERVAL_DS */  ,{  &SQLVariant::OperVarSmallEQChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,&SQLVariant::OperIntDSSmallEQIntDS }
};

// Smaller-Equal operator for SQLVariant
bool
SQLVariant::operator<=(SQLVariant& p_right)
{
  // If one of both is NULL, the result is false
  if(IsNULL() || p_right.IsNULL())
  {
    return false;
  }

  // Getting the concise type
  SQLConciseType left  = SQLTypeToConciseType(m_datatype);
  SQLConciseType right = SQLTypeToConciseType(p_right.m_datatype);

  // Check whether both datatypes are valid
  if(left == CT_LAST || right == CT_LAST)
  {
    return false;
  }

  // Find our comparison function
  OperatorCompare function = OperatorSmallerEqual[left][right].function;
  if(function)
  {
    return (this->*(function))(p_right);
  }
  // No compare function found
  // Data types are not comparable
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the smaller-equal operator on (%s <= %s)",leftType,rightType);
  throw error;
}

//////////////////////////////////////////////////////////////////////////
//
// ALL EQUALITY OPERATOR FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

bool
SQLVariant::OperVarSmallEQChar(SQLVariant& p_right)
{
  CString leftString,rightString;
  GetAsString(leftString);
  p_right.GetAsString(rightString);

  return leftString.Compare(rightString) <= 0;
}

bool
SQLVariant::OperGuidSmallEQChar(SQLVariant& p_right)
{
  CString leftString,rightString;
  GetAsString(leftString);
  p_right.GetAsString(rightString);

  leftString. TrimLeft ('{');
  leftString. TrimRight('}');
  rightString.TrimLeft ('}');
  rightString.TrimRight('{');

  return leftString.Compare(rightString) <= 0;
}

// SIGNED SHORT

bool
SQLVariant::OperSShortSmallEQSShort(SQLVariant& p_right)
{
  return GetAsSShort() <= p_right.GetAsSShort();
}

bool
SQLVariant::OperUShortSmallEQSShort(SQLVariant& p_right)
{
  return GetAsUShort() <= ShortToUShort(p_right.GetAsSShort());
}

bool
SQLVariant::OperSLongSmallEQSShort(SQLVariant& p_right)
{
  return SLongToShort(GetAsSLong()) <= p_right.GetAsSShort();
}

bool
SQLVariant::OperULongSmallEQSShort(SQLVariant& p_right)
{
  return ULongToUShort(GetAsULong()) <= ShortToUShort(p_right.GetAsSShort());
}

bool
SQLVariant::OperFloatSmallEQSShort(SQLVariant& p_right)
{
  return FloatToShort(GetAsFloat()) <= p_right.GetAsSShort();
}

bool
SQLVariant::OperDoubleSmallEQSShort(SQLVariant& p_right)
{
  return DoubleToShort(GetAsDouble()) <= p_right.GetAsSShort();
}

bool
SQLVariant::OperBitSmallEQSShort(SQLVariant& p_right)
{
  return p_right.GetAsSShort() != 0;
}

bool   
SQLVariant::OperSTinySmallEQSShort(SQLVariant& p_right)
{
  return GetAsSTinyInt() <= ShortToTinyInt(p_right.GetAsSShort());
}

bool
SQLVariant::OperUTinySmallEQSShort(SQLVariant& p_right)
{
  return GetAsUTinyInt() <= ShortToUTinyInt(p_right.GetAsSShort());
}

bool
SQLVariant::OperSBigSmallEQSShort(SQLVariant& p_right)
{
  return BIGINTToShort(GetAsSBigInt()) <= p_right.GetAsSShort();
}

bool
SQLVariant::OperUBigSmallEQSShort(SQLVariant& p_right)
{
  return UBIGINTToShort(GetAsUBigInt()) <= p_right.GetAsSShort();
}

// UNSIGNED SHORT

bool   
SQLVariant::OperSShortSmallEQUShort(SQLVariant& p_right)
{
  return ShortToUShort(GetAsSShort()) <= p_right.GetAsUShort();
}

bool   
SQLVariant::OperUShortSmallEQUShort(SQLVariant& p_right)
{
  return GetAsUShort() <= p_right.GetAsUShort();
}

bool
SQLVariant::OperSLongSmallEQUShort(SQLVariant& p_right)
{
  return SLongToUShort(GetAsSLong()) <= p_right.GetAsUShort();
}

bool
SQLVariant::OperUlongSmallEQUShort(SQLVariant& p_right)
{
  return ULongToUShort(GetAsULong()) <= p_right.GetAsUShort();
}

bool
SQLVariant::OperFloatSmallEQUShort(SQLVariant& p_right)
{
  return FloatToUShort(GetAsFloat()) <= p_right.GetAsUShort();
}

bool
SQLVariant::OperDoubleSmallEQUShort(SQLVariant& p_right)
{
  return DoubleToUShort(GetAsDouble()) <= p_right.GetAsUShort();
}

bool
SQLVariant::OperBitSmallEQUShort(SQLVariant& p_right)
{
  return p_right.GetAsUShort() != 0;
}

bool
SQLVariant::OperSTinySmallEQUShort(SQLVariant& p_right)
{
  return GetAsSTinyInt() <= UShortToTinyInt(p_right.GetAsUShort());
}

bool
SQLVariant::OperUTinySmallEQUShort(SQLVariant& p_right)
{
  return GetAsUTinyInt() <= UShortToUTinyInt(p_right.GetAsUShort());
}

bool
SQLVariant::OperSBigSmallEQUShort(SQLVariant& p_right)
{
  return BIGINTToUShort(GetAsSBigInt()) <= p_right.GetAsUShort();
}

bool
SQLVariant::OperUBigSmallEQUShort(SQLVariant& p_right)
{
  return UBIGINTToUShort(GetAsUBigInt()) <= p_right.GetAsUShort();
}

// SIGNED LONG

bool   
SQLVariant::OperSShortSmallEQSLong(SQLVariant& p_right)
{
  return GetAsSShort() <= SLongToShort(p_right.GetAsSLong());
}

bool
SQLVariant::OperUShortSmallEQSLong(SQLVariant& p_right)
{
  return GetAsUShort() <= SLongToUShort(p_right.GetAsSLong());
}

bool
SQLVariant::OperSLongSmallEQSLong(SQLVariant& p_right)
{
  return GetAsSLong() <= p_right.GetAsSLong();
}

bool
SQLVariant::OperULongSmallEQSLong(SQLVariant& p_right)
{
  return GetAsULong() <= LongToULong(p_right.GetAsSLong());
}

bool
SQLVariant::OperFloatSmallEQSLong(SQLVariant& p_right)
{
  return FloatToLong(GetAsFloat()) <= p_right.GetAsSLong();
}

bool
SQLVariant::OperDoubleESmallEQSLong(SQLVariant& p_right)
{
  return DoubleToLong(GetAsDouble()) <= p_right.GetAsSLong();
}

bool
SQLVariant::OperBitSmallEQSLong(SQLVariant& p_right)
{
  return p_right.GetAsSLong() != 0;
}

bool
SQLVariant::OperSTinySmallEQSLong(SQLVariant& p_right)
{
  return GetAsSTinyInt() <= SLongToTinyInt(p_right.GetAsSLong());
}

bool
SQLVariant::OperUTinySmallEQSLong(SQLVariant& p_right)
{
  return GetAsUTinyInt() <= SLongToUTinyInt(p_right.GetAsSLong());
}

bool
SQLVariant::OperSBigSmallEQSLong(SQLVariant& p_right)
{
  return BIGINTToLong(GetAsSBigInt()) <= p_right.GetAsSLong();
}

bool
SQLVariant::OperUBigSmallEQSLong(SQLVariant& p_right)
{
  return UBIGINTToLong(GetAsUBigInt()) <= p_right.GetAsSLong();
}

// UNSIGNED LONG

bool   
SQLVariant::OperSShortSmallEQULong(SQLVariant& p_right)
{
  return ShortToUShort(GetAsSShort()) <= ULongToUShort(p_right.GetAsULong());
}

bool
SQLVariant::OperUShortSmallEQULong(SQLVariant& p_right)
{
  return GetAsUShort() <= ULongToUShort(p_right.GetAsULong());
}

bool
SQLVariant::OperSLongSmallEQULong(SQLVariant& p_right)
{
  return LongToULong(GetAsSLong()) <= p_right.GetAsULong();
}

bool
SQLVariant::OperULongSmallEQULong(SQLVariant& p_right)
{
  return GetAsULong() <= p_right.GetAsULong();
}

bool
SQLVariant::OperFloatSmallEQULong(SQLVariant& p_right)
{
  return FloatToULong(GetAsFloat()) <= p_right.GetAsULong();
}

bool
SQLVariant::OperDoubleSmallEQULong(SQLVariant& p_right)
{
  return DoubleToULong(GetAsDouble()) <= p_right.GetAsULong();
}

bool
SQLVariant::OperBitSmallEQULong(SQLVariant& p_right)
{
  return p_right.GetAsULong() != 0;
}

bool
SQLVariant::OperSTinySmallEQULong(SQLVariant& p_right)
{
  return TinyIntToUTinyInt(GetAsSTinyInt()) <= ULongToUTinyInt(p_right.GetAsULong());
}

bool
SQLVariant::OperUTinySmallEQULong(SQLVariant& p_right)
{
  return GetAsUTinyInt() <= ULongToUTinyInt(p_right.GetAsULong());
}

bool
SQLVariant::OperSBigSmallEQULong(SQLVariant& p_right)
{
  return BIGINTToULong(GetAsSBigInt()) <= p_right.GetAsULong();
}

bool
SQLVariant::OperUBigSmallEQULong(SQLVariant& p_right)
{
  return UBIGINTToULong(GetAsUBigInt()) <= p_right.GetAsULong();
}

// FLOAT

bool   
SQLVariant::OperSShortSmallEQFloat(SQLVariant& p_right)
{
  return GetAsSShort() <= FloatToShort(p_right.GetAsFloat());
}

bool   
SQLVariant::OperUShortSmallEQFloat(SQLVariant& p_right)
{
  return GetAsUShort() <= FloatToUShort(p_right.GetAsFloat());
}

bool
SQLVariant::OperSLongSmallEQFloat(SQLVariant& p_right)
{
  return GetAsSLong() <= FloatToLong(p_right.GetAsFloat());
}

bool
SQLVariant::OperULongSmallEQFloat(SQLVariant& p_right)
{
  return GetAsULong() <= FloatToULong(p_right.GetAsFloat());
}

bool
SQLVariant::OperFloatSmallEQFloat(SQLVariant& p_right)
{
  return GetAsFloat() <= p_right.GetAsFloat();
}

bool
SQLVariant::OperDoubleSmallEQFloat(SQLVariant& p_right)
{
  return DoubleToFloat(GetAsDouble()) <= p_right.GetAsFloat();
}

bool
SQLVariant::OperBitSmallEQFloat(SQLVariant& p_right)
{
  return p_right.GetAsFloat() != 0.0;
}

bool
SQLVariant::OperSTinySmallEQFloat(SQLVariant& p_right)
{
  return GetAsSTinyInt() <= FloatToTinyInt(p_right.GetAsFloat());
}

bool
SQLVariant::OperUTinySmallEQFloat(SQLVariant& p_right)
{
  return GetAsUTinyInt() <= FloatToUTinyInt(p_right.GetAsFloat());
}

bool
SQLVariant::OperSBigSmallEQFloat(SQLVariant& p_right)
{
  return GetAsSBigInt() <= FloatToBIGINT(p_right.GetAsFloat());
}

bool
SQLVariant::OperUBigSmallEQFloat(SQLVariant& p_right)
{
  return GetAsUBigInt() <= FloatToUBIGINT(p_right.GetAsFloat());
}

bool
SQLVariant::OperNumSmallEQFloat(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  double lfdouble = atof(left);

  return DoubleToFloat(lfdouble) <= p_right.GetAsFloat();
}

// DOUBLE

bool
SQLVariant::OperSShortSmallEQDouble(SQLVariant& p_right)
{
  return GetAsSShort() <= DoubleToShort(p_right.GetAsDouble());
}

bool
SQLVariant::OperUShortSmallEQDouble(SQLVariant& p_right)
{
  return GetAsUShort() <= DoubleToUShort(p_right.GetAsDouble());
}

bool
SQLVariant::OperSLongSmallEQDouble(SQLVariant& p_right)
{
  return GetAsSLong() <= DoubleToLong(p_right.GetAsDouble());
}

bool
SQLVariant::OperULongSmallEQDouble(SQLVariant& p_right)
{
  return GetAsULong() <= DoubleToULong(p_right.GetAsDouble());
}

bool
SQLVariant::OperFloatSmallEQDouble(SQLVariant& p_right)
{
  return GetAsFloat() <= DoubleToFloat(p_right.GetAsDouble());
}

bool
SQLVariant::OperDoubleSmallEQDouble(SQLVariant& p_right)
{
  return GetAsDouble() <= p_right.GetAsDouble();
}

bool
SQLVariant::OperBitSmallEQDouble(SQLVariant& p_right)
{
  return p_right.GetAsDouble() != 0.0;
}

bool
SQLVariant::OperSTinySmallEQDouble(SQLVariant& p_right)
{
  return GetAsSTinyInt() <= DoubleToTinyInt(p_right.GetAsDouble());
}

bool
SQLVariant::OperUTinySmallEQDouble(SQLVariant& p_right)
{
  return GetAsUTinyInt() <= DoubleToUTinyInt(p_right.GetAsDouble());
}

bool
SQLVariant::OperSBigSmallEQDouble(SQLVariant& p_right)
{
  return GetAsSBigInt() <= DoubleToBIGINT(p_right.GetAsDouble());
}

bool
SQLVariant::OperUBigSmallEQDouble(SQLVariant& p_right)
{
  return GetAsUBigInt() <= DoubleToUBIGINT(p_right.GetAsDouble());
}

bool
SQLVariant::OperNumSmallEQDouble(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  double lfdouble = atof(left);
  return lfdouble <= p_right.GetAsDouble();
}

// BIT

bool   
SQLVariant::OperSShortSmallEQBit(SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperUShortSmallEQBit(SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool   
SQLVariant::OperSLongSmallEQBit(SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperULongSmallEQBit(SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperFloatSmallEQBit(SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperDoubleSmallEQBit(SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperBitSmallEQBit(SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperSTinySmallEQBit(SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperUTinySmallEQBit(SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperSBigSmallEQBit(SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperUBigSmallEQBit(SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperNumSmallEQBit(SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

// SIGNED TINYINT

bool
SQLVariant::OperSShortSmallEQSTiny(SQLVariant& p_right)
{
  return ShortToTinyInt(GetAsSShort()) <= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUShortSmallEQSTiny(SQLVariant& p_right)
{
  return UShortToTinyInt(GetAsUShort()) <= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperSLongSmallEQSTiny(SQLVariant& p_right)
{
  return SLongToTinyInt(GetAsSLong()) <= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperULongSmallEQSTiny(SQLVariant& p_right)
{
  return ULongToTinyInt(GetAsULong()) <= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperFloatSmallEQSTiny(SQLVariant& p_right)
{
  return FloatToTinyInt(GetAsFloat()) <= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperDoubleSmallEQSTiny(SQLVariant& p_right)
{
  return DoubleToTinyInt(GetAsDouble()) <= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperBitSmallEQSTiny(SQLVariant& p_right)
{
  return p_right.GetAsSTinyInt() != 0;
}

bool
SQLVariant::OperSTinySmallEQSTiny(SQLVariant& p_right)
{
  return GetAsSTinyInt() <= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUTinySmallEQSTiny(SQLVariant& p_right)
{
  return GetAsUTinyInt() <= TinyIntToUTinyInt(p_right.GetAsSTinyInt());
}

bool
SQLVariant::OperSBigSmallEQSTiny(SQLVariant& p_right)
{
  return SBIGINTToTinyInt(GetAsSBigInt()) <= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUBigSmallEQSTiny(SQLVariant& p_right)
{
  return UBIGINTToTinyInt(GetAsUBigInt()) <= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperNumSmallEQSTiny(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  int lf = atoi(left);

  return SLongToTinyInt(lf) <= p_right.GetAsSTinyInt();
}

// UNSIGNED TINYINT

bool
SQLVariant::OperSShortSmallEQUTiny(SQLVariant& p_right)
{
  return ShortToUTinyInt(GetAsSShort()) <= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUShortSmallEQUTiny(SQLVariant& p_right)
{
  return UShortToUTinyInt(GetAsUShort()) <= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperSLongSmallEQUTiny(SQLVariant& p_right)
{
  return SLongToUTinyInt(GetAsSLong()) <= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperULongSmallEQUTiny(SQLVariant& p_right)
{
  return ULongToUTinyInt(GetAsULong()) <= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperFloatSmallEQUTiny(SQLVariant& p_right)
{
  return FloatToUTinyInt(GetAsFloat()) <= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperDoubleSmallEQUTiny(SQLVariant& p_right)
{
  return DoubleToUTinyInt(GetAsDouble()) <= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperBitSmallEQUTiny(SQLVariant& p_right)
{
  return p_right.GetAsUTinyInt() != 0;
}

bool
SQLVariant::OperSTinySmallEQUTiny(SQLVariant& p_right)
{
  return TinyIntToUTinyInt(GetAsSTinyInt()) <= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUTinySmallEQUTiny(SQLVariant& p_right)
{
  return GetAsUTinyInt() <= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperSBigSmallEQUTiny(SQLVariant& p_right)
{
  return SBIGINTToUTinyInt(GetAsSBigInt()) <= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUBigSmallEQUTiny(SQLVariant& p_right)
{
  return UBIGINTToUTinyInt(GetAsUBigInt()) <= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperNumSmallEQUTiny(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  int lf = atoi(left);

  return SLongToUTinyInt(lf) <= p_right.GetAsUTinyInt();
}

// SIGNED BIGINT

bool
SQLVariant::OperSShortSmallEQSBig(SQLVariant& p_right)
{
  return GetAsSShort() <= BIGINTToShort(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperUShortSmallEQSBig(SQLVariant& p_right)
{
  return GetAsUShort() <= BIGINTToUShort(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperSLongSmallEQSBig(SQLVariant& p_right)
{
  return GetAsSLong() <= BIGINTToLong(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperULongSmallEQSBig(SQLVariant& p_right)
{
  return GetAsULong() <= BIGINTToULong(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperFloatSmallEQSBig(SQLVariant& p_right)
{
  return GetAsFloat() <= BIGINTToFloat(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperDoubleSmallEQSBig(SQLVariant& p_right)
{
  return GetAsDouble() <= BIGINTToDouble(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperBitSmallEQSBig(SQLVariant& p_right)
{
  return p_right.GetAsSBigInt() != 0;
}

bool
SQLVariant::OperSTinySmallEQSBig(SQLVariant& p_right)
{
  return GetAsSTinyInt() <= SBIGINTToTinyInt(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperUTinySmallEQSBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() <= SBIGINTToUTinyInt(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperSBigSmallEQSBig(SQLVariant& p_right)
{
  return GetAsSBigInt() <= p_right.GetAsSBigInt();
}

bool
SQLVariant::OperUBigSmallEQSBig(SQLVariant& p_right)
{
  return GetAsUBigInt() <= SBIGINTToUBIGINT(p_right.GetAsSBigInt());
}

bool   
SQLVariant::OperNumSmallEQSBig(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  SQLBIGINT lf = _atoi64(left);

  return lf <= p_right.GetAsSBigInt();
}

// UNSIGNED BIGINT

bool
SQLVariant::OperSShortSmallEQUBig(SQLVariant& p_right)
{
  return GetAsSShort() <= UBIGINTToShort(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperUShortSmallEQUBig(SQLVariant& p_right)
{
  return GetAsUShort() <= UBIGINTToUShort(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperSLongSmallEQUBig(SQLVariant& p_right)
{
  return GetAsSLong() <= UBIGINTToLong(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperULongSmallEQUBig(SQLVariant& p_right)
{
  return GetAsULong() <= UBIGINTToULong(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperFloatSmallEQUBig(SQLVariant& p_right)
{
  return GetAsFloat() <= UBIGINTToFloat(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperDoubleSmallEQUBig(SQLVariant& p_right)
{
  return GetAsDouble() <= UBIGINTToDouble(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperBitSmallEQUBig(SQLVariant& p_right)
{
  return p_right.GetAsUBigInt() != 0;
}

bool
SQLVariant::OperSTinySmallEQUBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() <= UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperUTinySmallEQUBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() <= UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperSBigSmallEQUBig(SQLVariant& p_right)
{
  return SBIGINTToUBIGINT(GetAsSBigInt()) <= p_right.GetAsUBigInt();
}

bool
SQLVariant::OperUBigSmallEQUBig(SQLVariant& p_right)
{
  return GetAsUBigInt() <= p_right.GetAsUBigInt();
}

bool
SQLVariant::OperNumSmallEQUBig(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  SQLBIGINT lf = _atoi64(left);

  return lf <= UBIGINTToBIGINT(p_right.GetAsUBigInt());
}

// NUMERIC

bool
SQLVariant::OperSShortSmallEQNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  short ri = SLongToShort(atoi(right));

  return GetAsSShort() <= ri;
}

bool
SQLVariant::OperUShortSmallEQNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  unsigned short ri = SLongToUShort(atoi(right));

  return GetAsUShort() <= ri;
}

bool
SQLVariant::OperSLongSmallEQNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  long ri = atoi(right);

  return GetAsSLong() <= ri;
}

bool
SQLVariant::OperULongSmallEQNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  unsigned long ri = LongToULong(atoi(right));

  return GetAsULong() <= ri;
}

bool
SQLVariant::OperFloatSmallEQNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  float ri = DoubleToFloat(atof(right));

  return GetAsFloat() <= ri;
}

bool
SQLVariant::OperDoubleSmallEQNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  double ri = atof(right);

  return GetAsFloat() <= ri;
}

bool
SQLVariant::OperBitSmallEQNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  int ri = atoi(right);

  return ri != 0;
}

bool
SQLVariant::OperSTinySmallEQNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  long ri = atoi(right);

  return GetAsSTinyInt() <= SLongToTinyInt(ri);
}

bool
SQLVariant::OperUTinySmallEQNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  long ri = atoi(right);

  return GetAsUTinyInt() <= SLongToUTinyInt(ri);
}

bool
SQLVariant::OperSBigSmallEQNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  SQLBIGINT ri = _atoi64(right);

  return GetAsSBigInt() <= ri;
}

bool
SQLVariant::OperUBigSmallEQNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  SQLUBIGINT ri = SBIGINTToUBIGINT(_atoi64(right));

  return GetAsUBigInt() <= ri;
}

bool
SQLVariant::OperNumSmallEQNum(SQLVariant& p_right)
{
  SQL_NUMERIC_STRUCT* left  = GetAsNumeric();
  SQL_NUMERIC_STRUCT* right = p_right.GetAsNumeric();

  return memcmp(left,right,sizeof(SQL_NUMERIC_STRUCT)) <= 0;
}

// TYPE == GUID

bool
SQLVariant::OperGuidSmallEQGuid(SQLVariant& p_right)
{
  SQLGUID* left  = GetAsGUID();
  SQLGUID* right = p_right.GetAsGUID();

  return memcmp(left,right,sizeof(SQLGUID)) <= 0;
}

// TYPE == BINARY

bool
SQLVariant::OperBinarySmallEQBinary(SQLVariant& p_right)
{
  void* left   = GetAsBinary();
  void* right  = p_right.GetAsBinary();
  // Take longest of two binaries in comparison
  long  length = m_binaryLength < p_right.m_binaryLength ? m_binaryLength : p_right.m_binaryLength;

  return memcmp(left,right,length) <= 0;
}

// TYPE == DATE

bool
SQLVariant::OperDateSmallEQDate(SQLVariant& p_right)
{
  // Relies on the < operator of SQLDate
  return GetAsSQLDate() <= p_right.GetAsSQLDate();
}

bool
SQLVariant::OperStampSmallEQDate(SQLVariant& p_right)
{
  SQLDate date = GetAsSQLTimestamp().AsSQLDate();
  return date <= p_right.GetAsSQLDate();
}

// TYPE == TIME

bool
SQLVariant::OperTimeSmallEQTime(SQLVariant& p_right)
{
  // Relies on the < operator of SQLTime
  return GetAsSQLTime() <= p_right.GetAsSQLTime();
}

bool
SQLVariant::OperStampSmallEQTime(SQLVariant& p_right)
{
  SQLTime time = GetAsSQLTimestamp().AsSQLTime();
  return time <= p_right.GetAsSQLTime();
}

// TYPE == TIMESTAMP

bool
SQLVariant::OperStampSmallEQStamp(SQLVariant& p_right)
{
  // Relies on the < operator of SQLTimestamp
  return GetAsSQLTimestamp() <= p_right.GetAsSQLTimestamp();
}

// TYPE == INTERVAL_YEAR_MONTH

bool
SQLVariant::OperIntYMSmallEQIntYM(SQLVariant& p_right)
{
  // Relies on the calculation of months in SQLInterval
  return GetAsSQLInterval().AsDatabaseDouble() <= p_right.GetAsSQLInterval().AsDatabaseDouble();
}

// TYPE == INTERVAL_DAY_SECOND

bool
SQLVariant::OperIntDSSmallEQIntDS(SQLVariant& p_right)
{
  // Relies on the calculation of seconds in SQLInterval
  return GetAsSQLInterval().AsDatabaseDouble() <= p_right.GetAsSQLInterval().AsDatabaseDouble();
}
