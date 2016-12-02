////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorGreatEqual.cpp
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

CompareFunctionArray
SQLVariant::OperatorGreaterEqual[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                          CT_SSHORT                            CT_USHORT                            CT_SLONG                             CT_ULONG                            CT_FLOAT                            CT_DOUBLE                            CT_BIT                            CT_STINYINT                         CT_UTINYINT                         CT_SBIGINT                         CT_UBIGINT                         CT_NUMERIC                        CT_GUID                          CT_BINARY                            CT_DATE                           CT_TIME                           CT_TIMESTAMP                       CT_INTERVAL_YM                     CT_INTERVAL_DS
                         // -------------------------------- ------------------------------------ ------------------------------------ ------------------------------------ ----------------------------------- ----------------------------------- ------------------------------------ --------------------------------- ----------------------------------- ----------------------------------- ---------------------------------- ---------------------------------- --------------------------------- -------------------------------- ------------------------------------ --------------------------------- --------------------------------- ---------------------------------- ---------------------------------- -----------------------------------
  /* CT_CHAR        */   {  &SQLVariant::OperVarGreatEQChar ,&SQLVariant::OperVarGreatEQChar     ,&SQLVariant::OperVarGreatEQChar     ,&SQLVariant::OperVarGreatEQChar     ,&SQLVariant::OperVarGreatEQChar    ,&SQLVariant::OperVarGreatEQChar    ,&SQLVariant::OperVarGreatEQChar     ,&SQLVariant::OperVarGreatEQChar  ,&SQLVariant::OperVarGreatEQChar    ,&SQLVariant::OperVarGreatEQChar    ,&SQLVariant::OperVarGreatEQChar   ,&SQLVariant::OperVarGreatEQChar   ,&SQLVariant::OperVarGreatEQChar  ,&SQLVariant::OperVarGreatEQChar ,nullptr                             ,&SQLVariant::OperVarGreatEQChar  ,&SQLVariant::OperVarGreatEQChar  ,&SQLVariant::OperVarGreatEQChar   ,&SQLVariant::OperVarGreatEQChar   ,&SQLVariant::OperVarGreatEQChar    }
  /* CT_SSHORT      */  ,{  &SQLVariant::OperVarGreatEQChar ,&SQLVariant::OperSShortGreatEQSShort,&SQLVariant::OperSShortGreatEQUShort,&SQLVariant::OperSShortGreatEQSLong ,&SQLVariant::OperSShortGreatEQULong,&SQLVariant::OperSShortGreatEQFloat,&SQLVariant::OperSShortGreatEQDouble,&SQLVariant::OperSShortGreatEQBit,&SQLVariant::OperSShortGreatEQSTiny,&SQLVariant::OperSShortGreatEQUTiny,&SQLVariant::OperSShortGreatEQSBig,&SQLVariant::OperSShortGreatEQUBig,&SQLVariant::OperSShortGreatEQNum,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_USHORT      */  ,{  &SQLVariant::OperVarGreatEQChar ,&SQLVariant::OperUShortGreatEQSShort,&SQLVariant::OperUShortGreatEQUShort,&SQLVariant::OperUShortGreatEQSLong ,&SQLVariant::OperUShortGreatEQULong,&SQLVariant::OperUShortGreatEQFloat,&SQLVariant::OperUShortGreatEQDouble,&SQLVariant::OperUShortGreatEQBit,&SQLVariant::OperUShortGreatEQSTiny,&SQLVariant::OperUShortGreatEQUTiny,&SQLVariant::OperUShortGreatEQSBig,&SQLVariant::OperUShortGreatEQUBig,&SQLVariant::OperUShortGreatEQNum,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_SLONG       */  ,{  &SQLVariant::OperVarGreatEQChar ,&SQLVariant::OperSLongGreatEQSShort ,&SQLVariant::OperSLongGreatEQUShort ,&SQLVariant::OperSLongGreatEQSLong  ,&SQLVariant::OperSLongGreatEQULong ,&SQLVariant::OperSLongGreatEQFloat ,&SQLVariant::OperSLongGreatEQDouble ,&SQLVariant::OperSLongGreatEQBit ,&SQLVariant::OperSLongGreatEQSTiny ,&SQLVariant::OperSLongGreatEQUTiny ,&SQLVariant::OperSLongGreatEQSBig ,&SQLVariant::OperSLongGreatEQUBig ,&SQLVariant::OperSLongGreatEQNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_ULONG       */  ,{  &SQLVariant::OperVarGreatEQChar ,&SQLVariant::OperULongGreatEQSShort ,&SQLVariant::OperUlongGreatEQUShort ,&SQLVariant::OperULongGreatEQSLong  ,&SQLVariant::OperULongGreatEQULong ,&SQLVariant::OperULongGreatEQFloat ,&SQLVariant::OperULongGreatEQDouble ,&SQLVariant::OperULongGreatEQBit ,&SQLVariant::OperULongGreatEQSTiny ,&SQLVariant::OperULongGreatEQUTiny ,&SQLVariant::OperULongGreatEQSBig ,&SQLVariant::OperULongGreatEQUBig ,&SQLVariant::OperULongGreatEQNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_FLOAT       */  ,{  &SQLVariant::OperVarGreatEQChar ,&SQLVariant::OperFloatGreatEQSShort ,&SQLVariant::OperFloatGreatEQUShort ,&SQLVariant::OperFloatGreatEQSLong  ,&SQLVariant::OperFloatGreatEQULong ,&SQLVariant::OperFloatGreatEQFloat ,&SQLVariant::OperFloatGreatEQDouble ,&SQLVariant::OperFloatGreatEQBit ,&SQLVariant::OperFloatGreatEQSTiny ,&SQLVariant::OperFloatGreatEQUTiny ,&SQLVariant::OperFloatGreatEQSBig ,&SQLVariant::OperFloatGreatEQUBig ,&SQLVariant::OperFloatGreatEQNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_DOUBLE      */  ,{  &SQLVariant::OperVarGreatEQChar ,&SQLVariant::OperDoubleGreatEQSShort,&SQLVariant::OperDoubleGreatEQUShort,&SQLVariant::OperDoubleEGreatEQSLong,&SQLVariant::OperDoubleGreatEQULong,&SQLVariant::OperDoubleGreatEQFloat,&SQLVariant::OperDoubleGreatEQDouble,&SQLVariant::OperDoubleGreatEQBit,&SQLVariant::OperDoubleGreatEQSTiny,&SQLVariant::OperDoubleGreatEQUTiny,&SQLVariant::OperDoubleGreatEQSBig,&SQLVariant::OperDoubleGreatEQUBig,&SQLVariant::OperDoubleGreatEQNum,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_BIT         */  ,{  &SQLVariant::OperVarGreatEQChar ,&SQLVariant::OperBitGreatEQSShort   ,&SQLVariant::OperBitGreatEQUShort   ,&SQLVariant::OperBitGreatEQSLong    ,&SQLVariant::OperBitGreatEQULong   ,&SQLVariant::OperBitGreatEQFloat   ,&SQLVariant::OperBitGreatEQDouble   ,&SQLVariant::OperBitGreatEQBit   ,&SQLVariant::OperBitGreatEQSTiny   ,&SQLVariant::OperBitGreatEQUTiny   ,&SQLVariant::OperBitGreatEQSBig   ,&SQLVariant::OperBitGreatEQUBig   ,&SQLVariant::OperBitGreatEQNum   ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_STINYINT    */  ,{  &SQLVariant::OperVarGreatEQChar ,&SQLVariant::OperSTinyGreatEQSShort ,&SQLVariant::OperSTinyGreatEQUShort ,&SQLVariant::OperSTinyGreatEQSLong  ,&SQLVariant::OperSTinyGreatEQULong ,&SQLVariant::OperSTinyGreatEQFloat ,&SQLVariant::OperSTinyGreatEQDouble ,&SQLVariant::OperSTinyGreatEQBit ,&SQLVariant::OperSTinyGreatEQSTiny ,&SQLVariant::OperSTinyGreatEQUTiny ,&SQLVariant::OperSTinyGreatEQSBig ,&SQLVariant::OperSTinyGreatEQUBig ,&SQLVariant::OperSTinyGreatEQNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_UTINYINT    */  ,{  &SQLVariant::OperVarGreatEQChar ,&SQLVariant::OperUTinyGreatEQSShort ,&SQLVariant::OperUTinyGreatEQUShort ,&SQLVariant::OperUTinyGreatEQSLong  ,&SQLVariant::OperUTinyGreatEQULong ,&SQLVariant::OperUTinyGreatEQFloat ,&SQLVariant::OperUTinyGreatEQDouble ,&SQLVariant::OperUTinyGreatEQBit ,&SQLVariant::OperUTinyGreatEQSTiny ,&SQLVariant::OperUTinyGreatEQUTiny ,&SQLVariant::OperUTinyGreatEQSBig ,&SQLVariant::OperUTinyGreatEQUBig ,&SQLVariant::OperUTinyGreatEQNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_SBIGINT     */  ,{  &SQLVariant::OperVarGreatEQChar ,&SQLVariant::OperSBigGreatEQSShort  ,&SQLVariant::OperSBigGreatEQUShort  ,&SQLVariant::OperSBigGreatEQSLong   ,&SQLVariant::OperSBigGreatEQULong  ,&SQLVariant::OperSBigGreatEQFloat  ,&SQLVariant::OperSBigGreatEQDouble  ,&SQLVariant::OperSBigGreatEQBit  ,&SQLVariant::OperSBigGreatEQSTiny  ,&SQLVariant::OperSBigGreatEQUTiny  ,&SQLVariant::OperSBigGreatEQSBig  ,&SQLVariant::OperSBigGreatEQUBig  ,&SQLVariant::OperSBigGreatEQNum  ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_UBIGINT     */  ,{  &SQLVariant::OperVarGreatEQChar ,&SQLVariant::OperUBigGreatEQSShort  ,&SQLVariant::OperUBigGreatEQUShort  ,&SQLVariant::OperUBigGreatEQSLong   ,&SQLVariant::OperUBigGreatEQULong  ,&SQLVariant::OperUBigGreatEQFloat  ,&SQLVariant::OperUBigGreatEQDouble  ,&SQLVariant::OperUBigGreatEQBit  ,&SQLVariant::OperUBigGreatEQSTiny  ,&SQLVariant::OperUBigGreatEQUTiny  ,&SQLVariant::OperUBigGreatEQSBig  ,&SQLVariant::OperUBigGreatEQUBig  ,&SQLVariant::OperUBigGreatEQNum  ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_NUMERIC     */  ,{  &SQLVariant::OperVarGreatEQChar ,&SQLVariant::OperNumGreatEQSShort   ,&SQLVariant::OperNumGreatEQUShort   ,&SQLVariant::OperNumGreatEQSLong    ,&SQLVariant::OperNumGreatEQULong   ,&SQLVariant::OperNumGreatEQFloat   ,&SQLVariant::OperNumGreatEQDouble   ,&SQLVariant::OperNumGreatEQBit   ,&SQLVariant::OperNumGreatEQSTiny   ,&SQLVariant::OperNumGreatEQUTiny   ,&SQLVariant::OperNumGreatEQSBig   ,&SQLVariant::OperNumGreatEQUBig   ,&SQLVariant::OperNumGreatEQNum   ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_GUID        */  ,{  &SQLVariant::OperGuidGreatEQChar,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,&SQLVariant::OperGuidGreatEQGuid,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_BINARY      */  ,{  nullptr                         ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,&SQLVariant::OperBinaryGreatEQBinary,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_DATE        */  ,{  &SQLVariant::OperVarGreatEQChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,&SQLVariant::OperDateGreatEQDate ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_TIME        */  ,{  &SQLVariant::OperVarGreatEQChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,nullptr                          ,&SQLVariant::OperTimeGreatEQTime ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_TIMESTAMP   */  ,{  &SQLVariant::OperVarGreatEQChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,&SQLVariant::OperStampGreatEQDate,&SQLVariant::OperStampGreatEQTime,&SQLVariant::OperStampGreatEQStamp,nullptr                           ,nullptr                            }
  /* CT_INTERVAL_YM */  ,{  &SQLVariant::OperVarGreatEQChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,&SQLVariant::OperIntYMGreatEQIntYM,nullptr                            }
  /* CT_INTERVAL_DS */  ,{  &SQLVariant::OperVarGreatEQChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,&SQLVariant::OperIntDSGreatEQIntDS }
};

// Greater-equal operator for SQLVariant
bool
SQLVariant::operator>=(SQLVariant& p_right)
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
  OperatorCompare function = OperatorGreaterEqual[left][right].function;
  if(function)
  {
    return (this->*(function))(p_right);
  }
  // No compare function found
  // Data types are not comparable
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the greater-equal operator on (%s >= %s)",leftType,rightType);
  throw error;
}

//////////////////////////////////////////////////////////////////////////
//
// ALL EQUALITY OPERATOR FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

bool
SQLVariant::OperVarGreatEQChar(SQLVariant& p_right)
{
  CString leftString,rightString;
  GetAsString(leftString);
  p_right.GetAsString(rightString);

  return leftString.Compare(rightString) >= 0;
}

bool
SQLVariant::OperGuidGreatEQChar(SQLVariant& p_right)
{
  CString leftString,rightString;
  GetAsString(leftString);
  p_right.GetAsString(rightString);

  leftString. TrimLeft ('{');
  leftString. TrimRight('}');
  rightString.TrimLeft ('}');
  rightString.TrimRight('{');

  return leftString.Compare(rightString) >= 0;
}

// SIGNED SHORT

bool
SQLVariant::OperSShortGreatEQSShort(SQLVariant& p_right)
{
  return GetAsSShort() >= p_right.GetAsSShort();
}

bool
SQLVariant::OperUShortGreatEQSShort(SQLVariant& p_right)
{
  return GetAsUShort() >= ShortToUShort(p_right.GetAsSShort());
}

bool
SQLVariant::OperSLongGreatEQSShort(SQLVariant& p_right)
{
  return SLongToShort(GetAsSLong()) >= p_right.GetAsSShort();
}

bool
SQLVariant::OperULongGreatEQSShort(SQLVariant& p_right)
{
  return ULongToUShort(GetAsULong()) >= ShortToUShort(p_right.GetAsSShort());
}

bool
SQLVariant::OperFloatGreatEQSShort(SQLVariant& p_right)
{
  return FloatToShort(GetAsFloat()) >= p_right.GetAsSShort();
}

bool
SQLVariant::OperDoubleGreatEQSShort(SQLVariant& p_right)
{
  return DoubleToShort(GetAsDouble()) >= p_right.GetAsSShort();
}

bool
SQLVariant::OperBitGreatEQSShort(SQLVariant& /*p_right*/)
{
  return GetAsBit() != 0;
}

bool   
SQLVariant::OperSTinyGreatEQSShort(SQLVariant& p_right)
{
  return GetAsSTinyInt() >= ShortToTinyInt(p_right.GetAsSShort());
}

bool
SQLVariant::OperUTinyGreatEQSShort(SQLVariant& p_right)
{
  return GetAsUTinyInt() >= ShortToUTinyInt(p_right.GetAsSShort());
}

bool
SQLVariant::OperSBigGreatEQSShort(SQLVariant& p_right)
{
  return BIGINTToShort(GetAsSBigInt()) >= p_right.GetAsSShort();
}

bool
SQLVariant::OperUBigGreatEQSShort(SQLVariant& p_right)
{
  return UBIGINTToShort(GetAsUBigInt()) >= p_right.GetAsSShort();
}

bool   
SQLVariant::OperNumGreatEQSShort(SQLVariant& p_right)
{
  return SLongToShort(GetAsBCD().AsLong()) >= p_right.GetAsSShort();
}

// UNSIGNED SHORT

bool   
SQLVariant::OperSShortGreatEQUShort(SQLVariant& p_right)
{
  return ShortToUShort(GetAsSShort()) >= p_right.GetAsUShort();
}

bool   
SQLVariant::OperUShortGreatEQUShort(SQLVariant& p_right)
{
  return GetAsUShort() >= p_right.GetAsUShort();
}

bool
SQLVariant::OperSLongGreatEQUShort(SQLVariant& p_right)
{
  return SLongToUShort(GetAsSLong()) >= p_right.GetAsUShort();
}

bool
SQLVariant::OperUlongGreatEQUShort(SQLVariant& p_right)
{
  return ULongToUShort(GetAsULong()) >= p_right.GetAsUShort();
}

bool
SQLVariant::OperFloatGreatEQUShort(SQLVariant& p_right)
{
  return FloatToUShort(GetAsFloat()) >= p_right.GetAsUShort();
}

bool
SQLVariant::OperDoubleGreatEQUShort(SQLVariant& p_right)
{
  return DoubleToUShort(GetAsDouble()) >= p_right.GetAsUShort();
}

bool
SQLVariant::OperBitGreatEQUShort(SQLVariant& /*p_right*/)
{
  return GetAsBit() != 0;
}

bool
SQLVariant::OperSTinyGreatEQUShort(SQLVariant& p_right)
{
  return GetAsSTinyInt() >= UShortToTinyInt(p_right.GetAsUShort());
}

bool
SQLVariant::OperUTinyGreatEQUShort(SQLVariant& p_right)
{
  return GetAsUTinyInt() >= UShortToUTinyInt(p_right.GetAsUShort());
}

bool
SQLVariant::OperSBigGreatEQUShort(SQLVariant& p_right)
{
  return BIGINTToUShort(GetAsSBigInt()) >= p_right.GetAsUShort();
}

bool
SQLVariant::OperUBigGreatEQUShort(SQLVariant& p_right)
{
  return UBIGINTToUShort(GetAsUBigInt()) >= p_right.GetAsUShort();
}

bool
SQLVariant::OperNumGreatEQUShort(SQLVariant& p_right)
{
  return SLongToUShort(GetAsBCD().AsLong()) >= p_right.GetAsUShort();
}

// SIGNED LONG

bool   
SQLVariant::OperSShortGreatEQSLong(SQLVariant& p_right)
{
  return GetAsSShort() >= SLongToShort(p_right.GetAsSLong());
}

bool
SQLVariant::OperUShortGreatEQSLong(SQLVariant& p_right)
{
  return GetAsUShort() >= SLongToUShort(p_right.GetAsSLong());
}

bool
SQLVariant::OperSLongGreatEQSLong(SQLVariant& p_right)
{
  return GetAsSLong() >= p_right.GetAsSLong();
}

bool
SQLVariant::OperULongGreatEQSLong(SQLVariant& p_right)
{
  return GetAsULong() >= LongToULong(p_right.GetAsSLong());
}

bool
SQLVariant::OperFloatGreatEQSLong(SQLVariant& p_right)
{
  return FloatToLong(GetAsFloat()) >= p_right.GetAsSLong();
}

bool
SQLVariant::OperDoubleEGreatEQSLong(SQLVariant& p_right)
{
  return DoubleToLong(GetAsDouble()) >= p_right.GetAsSLong();
}

bool
SQLVariant::OperBitGreatEQSLong(SQLVariant& /*p_right*/)
{
  return GetAsBit() != 0;
}

bool
SQLVariant::OperSTinyGreatEQSLong(SQLVariant& p_right)
{
  return GetAsSTinyInt() >= SLongToTinyInt(p_right.GetAsSLong());
}

bool
SQLVariant::OperUTinyGreatEQSLong(SQLVariant& p_right)
{
  return GetAsUTinyInt() >= SLongToUTinyInt(p_right.GetAsSLong());
}

bool
SQLVariant::OperSBigGreatEQSLong(SQLVariant& p_right)
{
  return BIGINTToLong(GetAsSBigInt()) >= p_right.GetAsSLong();
}

bool
SQLVariant::OperUBigGreatEQSLong(SQLVariant& p_right)
{
  return UBIGINTToLong(GetAsUBigInt()) >= p_right.GetAsSLong();
}

bool   
SQLVariant::OperNumGreatEQSLong(SQLVariant& p_right)
{
  return GetAsBCD().AsLong() >= p_right.GetAsSLong();
}

// UNSIGNED LONG

bool   
SQLVariant::OperSShortGreatEQULong(SQLVariant& p_right)
{
  return ShortToUShort(GetAsSShort()) >= ULongToUShort(p_right.GetAsULong());
}

bool
SQLVariant::OperUShortGreatEQULong(SQLVariant& p_right)
{
  return GetAsUShort() >= ULongToUShort(p_right.GetAsULong());
}

bool
SQLVariant::OperSLongGreatEQULong(SQLVariant& p_right)
{
  return LongToULong(GetAsSLong()) >= p_right.GetAsULong();
}

bool
SQLVariant::OperULongGreatEQULong(SQLVariant& p_right)
{
  return GetAsULong() >= p_right.GetAsULong();
}

bool
SQLVariant::OperFloatGreatEQULong(SQLVariant& p_right)
{
  return FloatToULong(GetAsFloat()) >= p_right.GetAsULong();
}

bool
SQLVariant::OperDoubleGreatEQULong(SQLVariant& p_right)
{
  return DoubleToULong(GetAsDouble()) >= p_right.GetAsULong();
}

bool
SQLVariant::OperBitGreatEQULong(SQLVariant& /*p_right*/)
{
  return GetAsBit() != 0;
}

bool
SQLVariant::OperSTinyGreatEQULong(SQLVariant& p_right)
{
  return TinyIntToUTinyInt(GetAsSTinyInt()) >= ULongToUTinyInt(p_right.GetAsULong());
}

bool
SQLVariant::OperUTinyGreatEQULong(SQLVariant& p_right)
{
  return GetAsUTinyInt() >= ULongToUTinyInt(p_right.GetAsULong());
}

bool
SQLVariant::OperSBigGreatEQULong(SQLVariant& p_right)
{
  return BIGINTToULong(GetAsSBigInt()) >= p_right.GetAsULong();
}

bool
SQLVariant::OperUBigGreatEQULong(SQLVariant& p_right)
{
  return UBIGINTToULong(GetAsUBigInt()) >= p_right.GetAsULong();
}

bool
SQLVariant::OperNumGreatEQULong(SQLVariant& p_right)
{
  return BIGINTToULong(GetAsBCD().AsInt64()) >= p_right.GetAsULong();
}

// FLOAT

bool   
SQLVariant::OperSShortGreatEQFloat(SQLVariant& p_right)
{
  return GetAsSShort() >= FloatToShort(p_right.GetAsFloat());
}

bool   
SQLVariant::OperUShortGreatEQFloat(SQLVariant& p_right)
{
  return GetAsUShort() >= FloatToUShort(p_right.GetAsFloat());
}

bool
SQLVariant::OperSLongGreatEQFloat(SQLVariant& p_right)
{
  return GetAsSLong() >= FloatToLong(p_right.GetAsFloat());
}

bool
SQLVariant::OperULongGreatEQFloat(SQLVariant& p_right)
{
  return GetAsULong() >= FloatToULong(p_right.GetAsFloat());
}

bool
SQLVariant::OperFloatGreatEQFloat(SQLVariant& p_right)
{
  return GetAsFloat() >= p_right.GetAsFloat();
}

bool
SQLVariant::OperDoubleGreatEQFloat(SQLVariant& p_right)
{
  return DoubleToFloat(GetAsDouble()) >= p_right.GetAsFloat();
}

bool
SQLVariant::OperBitGreatEQFloat(SQLVariant& /*p_right*/)
{
  return GetAsBit() != 0;
}

bool
SQLVariant::OperSTinyGreatEQFloat(SQLVariant& p_right)
{
  return GetAsSTinyInt() >= FloatToTinyInt(p_right.GetAsFloat());
}

bool
SQLVariant::OperUTinyGreatEQFloat(SQLVariant& p_right)
{
  return GetAsUTinyInt() >= FloatToUTinyInt(p_right.GetAsFloat());
}

bool
SQLVariant::OperSBigGreatEQFloat(SQLVariant& p_right)
{
  return GetAsSBigInt() >= FloatToBIGINT(p_right.GetAsFloat());
}

bool
SQLVariant::OperUBigGreatEQFloat(SQLVariant& p_right)
{
  return GetAsUBigInt() >= FloatToUBIGINT(p_right.GetAsFloat());
}

bool
SQLVariant::OperNumGreatEQFloat(SQLVariant& p_right)
{
  return GetAsBCD().AsDouble() >= (double)p_right.GetAsFloat();
}

// DOUBLE

bool
SQLVariant::OperSShortGreatEQDouble(SQLVariant& p_right)
{
  return GetAsSShort() >= DoubleToShort(p_right.GetAsDouble());
}

bool
SQLVariant::OperUShortGreatEQDouble(SQLVariant& p_right)
{
  return GetAsUShort() >= DoubleToUShort(p_right.GetAsDouble());
}

bool
SQLVariant::OperSLongGreatEQDouble(SQLVariant& p_right)
{
  return GetAsSLong() >= DoubleToLong(p_right.GetAsDouble());
}

bool
SQLVariant::OperULongGreatEQDouble(SQLVariant& p_right)
{
  return GetAsULong() >= DoubleToULong(p_right.GetAsDouble());
}

bool
SQLVariant::OperFloatGreatEQDouble(SQLVariant& p_right)
{
  return GetAsFloat() >= DoubleToFloat(p_right.GetAsDouble());
}

bool
SQLVariant::OperDoubleGreatEQDouble(SQLVariant& p_right)
{
  return GetAsDouble() >= p_right.GetAsDouble();
}

bool
SQLVariant::OperBitGreatEQDouble(SQLVariant& /*p_right*/)
{
  return GetAsBit() != 0;
}

bool
SQLVariant::OperSTinyGreatEQDouble(SQLVariant& p_right)
{
  return GetAsSTinyInt() >= DoubleToTinyInt(p_right.GetAsDouble());
}

bool
SQLVariant::OperUTinyGreatEQDouble(SQLVariant& p_right)
{
  return GetAsUTinyInt() >= DoubleToUTinyInt(p_right.GetAsDouble());
}

bool
SQLVariant::OperSBigGreatEQDouble(SQLVariant& p_right)
{
  return GetAsSBigInt() >= DoubleToBIGINT(p_right.GetAsDouble());
}

bool
SQLVariant::OperUBigGreatEQDouble(SQLVariant& p_right)
{
  return GetAsUBigInt() >= DoubleToUBIGINT(p_right.GetAsDouble());
}

bool
SQLVariant::OperNumGreatEQDouble(SQLVariant& p_right)
{
  return GetAsBCD().AsDouble() >= p_right.GetAsDouble();
}

// BIT

bool   
SQLVariant::OperSShortGreatEQBit(SQLVariant& /*p_right*/)
{
  return GetAsSShort() != 0;
}

bool
SQLVariant::OperUShortGreatEQBit(SQLVariant& /*p_right*/)
{
  return GetAsUShort() > 0;
}

bool   
SQLVariant::OperSLongGreatEQBit(SQLVariant& /*p_right*/)
{
  return GetAsSLong() > 0;
}

bool
SQLVariant::OperULongGreatEQBit(SQLVariant& /*p_right*/)
{
  return GetAsULong() > 0;
}

bool
SQLVariant::OperFloatGreatEQBit(SQLVariant& /*p_right*/)
{
  return GetAsFloat() > 0.0;
}

bool
SQLVariant::OperDoubleGreatEQBit(SQLVariant& /*p_right*/)
{
  return GetAsDouble() > 0.0;
}

bool
SQLVariant::OperBitGreatEQBit(SQLVariant& /*p_right*/)
{
  return GetAsBit() > 0;
}

bool
SQLVariant::OperSTinyGreatEQBit(SQLVariant& /*p_right*/)
{
  return GetAsSTinyInt() > 0;
}

bool
SQLVariant::OperUTinyGreatEQBit(SQLVariant& /*p_right*/)
{
  return GetAsUTinyInt() > 0;
}

bool
SQLVariant::OperSBigGreatEQBit(SQLVariant& /*p_right*/)
{
  return GetAsSBigInt() > 0;
}

bool
SQLVariant::OperUBigGreatEQBit(SQLVariant& /*p_right*/)
{
  return GetAsUBigInt() > 0;
}

bool
SQLVariant::OperNumGreatEQBit(SQLVariant& /*p_right*/)
{
  return GetAsBCD().AsLong() != 0;
}

// SIGNED TINYINT

bool
SQLVariant::OperSShortGreatEQSTiny(SQLVariant& p_right)
{
  return ShortToTinyInt(GetAsSShort()) >= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUShortGreatEQSTiny(SQLVariant& p_right)
{
  return UShortToTinyInt(GetAsUShort()) >= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperSLongGreatEQSTiny(SQLVariant& p_right)
{
  return SLongToTinyInt(GetAsSLong()) >= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperULongGreatEQSTiny(SQLVariant& p_right)
{
  return ULongToTinyInt(GetAsULong()) >= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperFloatGreatEQSTiny(SQLVariant& p_right)
{
  return FloatToTinyInt(GetAsFloat()) >= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperDoubleGreatEQSTiny(SQLVariant& p_right)
{
  return DoubleToTinyInt(GetAsDouble()) >= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperBitGreatEQSTiny(SQLVariant& /*p_right*/)
{
  return GetAsBit() != 0;
}

bool
SQLVariant::OperSTinyGreatEQSTiny(SQLVariant& p_right)
{
  return GetAsSTinyInt() >= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUTinyGreatEQSTiny(SQLVariant& p_right)
{
  return GetAsUTinyInt() >= TinyIntToUTinyInt(p_right.GetAsSTinyInt());
}

bool
SQLVariant::OperSBigGreatEQSTiny(SQLVariant& p_right)
{
  return SBIGINTToTinyInt(GetAsSBigInt()) >= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUBigGreatEQSTiny(SQLVariant& p_right)
{
  return UBIGINTToTinyInt(GetAsUBigInt()) >= p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperNumGreatEQSTiny(SQLVariant& p_right)
{
  return SLongToTinyInt(GetAsBCD().AsLong()) >= p_right.GetAsSTinyInt();
}

// UNSIGNED TINYINT

bool
SQLVariant::OperSShortGreatEQUTiny(SQLVariant& p_right)
{
  return ShortToUTinyInt(GetAsSShort()) >= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUShortGreatEQUTiny(SQLVariant& p_right)
{
  return UShortToUTinyInt(GetAsUShort()) >= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperSLongGreatEQUTiny(SQLVariant& p_right)
{
  return SLongToUTinyInt(GetAsSLong()) >= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperULongGreatEQUTiny(SQLVariant& p_right)
{
  return ULongToUTinyInt(GetAsULong()) >= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperFloatGreatEQUTiny(SQLVariant& p_right)
{
  return FloatToUTinyInt(GetAsFloat()) >= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperDoubleGreatEQUTiny(SQLVariant& p_right)
{
  return DoubleToUTinyInt(GetAsDouble()) >= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperBitGreatEQUTiny(SQLVariant& /*p_right*/)
{
  return GetAsBit() != 0;
}

bool
SQLVariant::OperSTinyGreatEQUTiny(SQLVariant& p_right)
{
  return TinyIntToUTinyInt(GetAsSTinyInt()) >= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUTinyGreatEQUTiny(SQLVariant& p_right)
{
  return GetAsUTinyInt() >= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperSBigGreatEQUTiny(SQLVariant& p_right)
{
  return SBIGINTToUTinyInt(GetAsSBigInt()) >= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUBigGreatEQUTiny(SQLVariant& p_right)
{
  return UBIGINTToUTinyInt(GetAsUBigInt()) >= p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperNumGreatEQUTiny(SQLVariant& p_right)
{
  return SLongToUTinyInt(GetAsBCD().AsLong()) >= p_right.GetAsUTinyInt();
}

// SIGNED BIGINT

bool
SQLVariant::OperSShortGreatEQSBig(SQLVariant& p_right)
{
  return GetAsSShort() >= BIGINTToShort(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperUShortGreatEQSBig(SQLVariant& p_right)
{
  return GetAsUShort() >= BIGINTToUShort(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperSLongGreatEQSBig(SQLVariant& p_right)
{
  return GetAsSLong() >= BIGINTToLong(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperULongGreatEQSBig(SQLVariant& p_right)
{
  return GetAsULong() >= BIGINTToULong(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperFloatGreatEQSBig(SQLVariant& p_right)
{
  return GetAsFloat() >= BIGINTToFloat(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperDoubleGreatEQSBig(SQLVariant& p_right)
{
  return GetAsDouble() >= BIGINTToDouble(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperBitGreatEQSBig(SQLVariant& /*p_right*/)
{
  return GetAsBit() != 0;
}

bool
SQLVariant::OperSTinyGreatEQSBig(SQLVariant& p_right)
{
  return GetAsSTinyInt() >= SBIGINTToTinyInt(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperUTinyGreatEQSBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() >= SBIGINTToUTinyInt(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperSBigGreatEQSBig(SQLVariant& p_right)
{
  return GetAsSBigInt() >= p_right.GetAsSBigInt();
}

bool
SQLVariant::OperUBigGreatEQSBig(SQLVariant& p_right)
{
  return GetAsUBigInt() >= SBIGINTToUBIGINT(p_right.GetAsSBigInt());
}

bool   
SQLVariant::OperNumGreatEQSBig(SQLVariant& p_right)
{
  return GetAsBCD().AsInt64() >= p_right.GetAsSBigInt();
}

// UNSIGNED BIGINT

bool
SQLVariant::OperSShortGreatEQUBig(SQLVariant& p_right)
{
  return GetAsSShort() >= UBIGINTToShort(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperUShortGreatEQUBig(SQLVariant& p_right)
{
  return GetAsUShort() >= UBIGINTToUShort(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperSLongGreatEQUBig(SQLVariant& p_right)
{
  return GetAsSLong() >= UBIGINTToLong(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperULongGreatEQUBig(SQLVariant& p_right)
{
  return GetAsULong() >= UBIGINTToULong(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperFloatGreatEQUBig(SQLVariant& p_right)
{
  return GetAsFloat() >= UBIGINTToFloat(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperDoubleGreatEQUBig(SQLVariant& p_right)
{
  return GetAsDouble() >= UBIGINTToDouble(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperBitGreatEQUBig(SQLVariant& /*p_right*/)
{
  return GetAsBit() != 0;
}

bool
SQLVariant::OperSTinyGreatEQUBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() >= UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperUTinyGreatEQUBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() >= UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperSBigGreatEQUBig(SQLVariant& p_right)
{
  return SBIGINTToUBIGINT(GetAsSBigInt()) >= p_right.GetAsUBigInt();
}

bool
SQLVariant::OperUBigGreatEQUBig(SQLVariant& p_right)
{
  return GetAsUBigInt() >= p_right.GetAsUBigInt();
}

bool
SQLVariant::OperNumGreatEQUBig(SQLVariant& p_right)
{
  return GetAsBCD().AsUInt64() >= p_right.GetAsUBigInt();
}

// NUMERIC

bool
SQLVariant::OperSShortGreatEQNum(SQLVariant& p_right)
{
  return GetAsSShort() >= SLongToShort(p_right.GetAsBCD().AsLong());
}

bool
SQLVariant::OperUShortGreatEQNum(SQLVariant& p_right)
{
  return GetAsUShort() >= SLongToUShort(p_right.GetAsBCD().AsLong());
}

bool
SQLVariant::OperSLongGreatEQNum(SQLVariant& p_right)
{
  return GetAsSLong() >= p_right.GetAsBCD().AsLong();
}

bool
SQLVariant::OperULongGreatEQNum(SQLVariant& p_right)
{
  return GetAsULong() >= BIGINTToULong(p_right.GetAsBCD().AsInt64());
}

bool
SQLVariant::OperFloatGreatEQNum(SQLVariant& p_right)
{
  return GetAsFloat() >= (float) p_right.GetAsBCD().AsDouble();
}

bool
SQLVariant::OperDoubleGreatEQNum(SQLVariant& p_right)
{
  return GetAsDouble() >= p_right.GetAsBCD().AsDouble();
}

bool
SQLVariant::OperBitGreatEQNum(SQLVariant& /*p_right*/)
{
  return GetAsBit() != 0;
}

bool
SQLVariant::OperSTinyGreatEQNum(SQLVariant& p_right)
{
  return GetAsSTinyInt() >= SLongToTinyInt(p_right.GetAsBCD().AsLong());
}

bool
SQLVariant::OperUTinyGreatEQNum(SQLVariant& p_right)
{
  return GetAsUTinyInt() >= SLongToUTinyInt(p_right.GetAsBCD().AsLong());
}

bool
SQLVariant::OperSBigGreatEQNum(SQLVariant& p_right)
{
  return GetAsSBigInt() >= p_right.GetAsBCD().AsInt64();
}

bool
SQLVariant::OperUBigGreatEQNum(SQLVariant& p_right)
{
  return GetAsUBigInt() >= p_right.GetAsBCD().AsUInt64();
}

bool
SQLVariant::OperNumGreatEQNum(SQLVariant& p_right)
{
  return GetAsBCD() >= p_right.GetAsBCD();
}

// TYPE == GUID

bool
SQLVariant::OperGuidGreatEQGuid(SQLVariant& p_right)
{
  SQLGUID* left  = GetAsGUID();
  SQLGUID* right = p_right.GetAsGUID();

  return memcmp(left,right,sizeof(SQLGUID)) >= 0;
}

// TYPE == BINARY

bool
SQLVariant::OperBinaryGreatEQBinary(SQLVariant& p_right)
{
  void* left   = GetAsBinary();
  void* right  = p_right.GetAsBinary();
  // Take longest of two binaries in comparison
  long  length = m_binaryLength > p_right.m_binaryLength ? m_binaryLength : p_right.m_binaryLength;

  return memcmp(left,right,length) >= 0;
}

// TYPE == DATE

bool
SQLVariant::OperDateGreatEQDate(SQLVariant& p_right)
{
  // Relies on the > operator of SQLDate
  return GetAsSQLDate() >= p_right.GetAsSQLDate();
}

bool
SQLVariant::OperStampGreatEQDate(SQLVariant& p_right)
{
  SQLDate date = GetAsSQLTimestamp().AsSQLDate();
  return date >= p_right.GetAsSQLDate();
}

// TYPE == TIME

bool
SQLVariant::OperTimeGreatEQTime(SQLVariant& p_right)
{
  // Relies on the > operator of SQLTime
  return GetAsSQLTime() >= p_right.GetAsSQLTime();
}

bool
SQLVariant::OperStampGreatEQTime(SQLVariant& p_right)
{
  SQLTime time = GetAsSQLTimestamp().AsSQLTime();
  return time >= p_right.GetAsSQLTime();
}

// TYPE == TIMESTAMP

bool
SQLVariant::OperStampGreatEQStamp(SQLVariant& p_right)
{
  // Relies on the > operator of SQLTimestamp
  return GetAsSQLTimestamp() >= p_right.GetAsSQLTimestamp();
}

// TYPE == INTERVAL_YEAR_MONTH

bool
SQLVariant::OperIntYMGreatEQIntYM(SQLVariant& p_right)
{
  // Relies on the calculation of months in SQLInterval
  return GetAsSQLInterval().AsDatabaseDouble() >= p_right.GetAsSQLInterval().AsDatabaseDouble();
}

// TYPE == INTERVAL_DAY_SECOND

bool
SQLVariant::OperIntDSGreatEQIntDS(SQLVariant& p_right)
{
  // Relies on the calculation of seconds in SQLInterval
  return GetAsSQLInterval().AsDatabaseDouble() >= p_right.GetAsSQLInterval().AsDatabaseDouble();
}
