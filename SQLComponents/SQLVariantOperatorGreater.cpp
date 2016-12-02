////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorGreater.cpp
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
SQLVariant::OperatorGreater[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                          CT_SSHORT                            CT_USHORT                            CT_SLONG                             CT_ULONG                            CT_FLOAT                            CT_DOUBLE                            CT_BIT                            CT_STINYINT                         CT_UTINYINT                         CT_SBIGINT                         CT_UBIGINT                         CT_NUMERIC                        CT_GUID                          CT_BINARY                            CT_DATE                           CT_TIME                           CT_TIMESTAMP                       CT_INTERVAL_YM                     CT_INTERVAL_DS
                         // -------------------------------- ------------------------------------ ------------------------------------ ------------------------------------ ----------------------------------- ----------------------------------- ------------------------------------ --------------------------------- ----------------------------------- ----------------------------------- ---------------------------------- ---------------------------------- --------------------------------- -------------------------------- ------------------------------------ --------------------------------- --------------------------------- ---------------------------------- ---------------------------------- -----------------------------------
  /* CT_CHAR        */   {  &SQLVariant::OperVarGreaterChar ,&SQLVariant::OperVarGreaterChar     ,&SQLVariant::OperVarGreaterChar     ,&SQLVariant::OperVarGreaterChar     ,&SQLVariant::OperVarGreaterChar    ,&SQLVariant::OperVarGreaterChar    ,&SQLVariant::OperVarGreaterChar     ,&SQLVariant::OperVarGreaterChar  ,&SQLVariant::OperVarGreaterChar    ,&SQLVariant::OperVarGreaterChar    ,&SQLVariant::OperVarGreaterChar   ,&SQLVariant::OperVarGreaterChar   ,&SQLVariant::OperVarGreaterChar  ,&SQLVariant::OperVarGreaterChar ,nullptr                             ,&SQLVariant::OperVarGreaterChar  ,&SQLVariant::OperVarGreaterChar  ,&SQLVariant::OperVarGreaterChar   ,&SQLVariant::OperVarGreaterChar   ,&SQLVariant::OperVarGreaterChar    }
  /* CT_SSHORT      */  ,{  &SQLVariant::OperVarGreaterChar ,&SQLVariant::OperSShortGreaterSShort,&SQLVariant::OperSShortGreaterUShort,&SQLVariant::OperSShortGreaterSLong ,&SQLVariant::OperSShortGreaterULong,&SQLVariant::OperSShortGreaterFloat,&SQLVariant::OperSShortGreaterDouble,&SQLVariant::OperSShortGreaterBit,&SQLVariant::OperSShortGreaterSTiny,&SQLVariant::OperSShortGreaterUTiny,&SQLVariant::OperSShortGreaterSBig,&SQLVariant::OperSShortGreaterUBig,&SQLVariant::OperSShortGreaterNum,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_USHORT      */  ,{  &SQLVariant::OperVarGreaterChar ,&SQLVariant::OperUShortGreaterSShort,&SQLVariant::OperUShortGreaterUShort,&SQLVariant::OperUShortGreaterSLong ,&SQLVariant::OperUShortGreaterULong,&SQLVariant::OperUShortGreaterFloat,&SQLVariant::OperUShortGreaterDouble,&SQLVariant::OperUShortGreaterBit,&SQLVariant::OperUShortGreaterSTiny,&SQLVariant::OperUShortGreaterUTiny,&SQLVariant::OperUShortGreaterSBig,&SQLVariant::OperUShortGreaterUBig,&SQLVariant::OperUShortGreaterNum,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_SLONG       */  ,{  &SQLVariant::OperVarGreaterChar ,&SQLVariant::OperSLongGreaterSShort ,&SQLVariant::OperSLongGreaterUShort ,&SQLVariant::OperSLongGreaterSLong  ,&SQLVariant::OperSLongGreaterULong ,&SQLVariant::OperSLongGreaterFloat ,&SQLVariant::OperSLongGreaterDouble ,&SQLVariant::OperSLongGreaterBit ,&SQLVariant::OperSLongGreaterSTiny ,&SQLVariant::OperSLongGreaterUTiny ,&SQLVariant::OperSLongGreaterSBig ,&SQLVariant::OperSLongGreaterUBig ,&SQLVariant::OperSLongGreaterNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_ULONG       */  ,{  &SQLVariant::OperVarGreaterChar ,&SQLVariant::OperULongGreaterSShort ,&SQLVariant::OperUlongGreaterUShort ,&SQLVariant::OperULongGreaterSLong  ,&SQLVariant::OperULongGreaterULong ,&SQLVariant::OperULongGreaterFloat ,&SQLVariant::OperULongGreaterDouble ,&SQLVariant::OperULongGreaterBit ,&SQLVariant::OperULongGreaterSTiny ,&SQLVariant::OperULongGreaterUTiny ,&SQLVariant::OperULongGreaterSBig ,&SQLVariant::OperULongGreaterUBig ,&SQLVariant::OperULongGreaterNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_FLOAT       */  ,{  &SQLVariant::OperVarGreaterChar ,&SQLVariant::OperFloatGreaterSShort ,&SQLVariant::OperFloatGreaterUShort ,&SQLVariant::OperFloatGreaterSLong  ,&SQLVariant::OperFloatGreaterULong ,&SQLVariant::OperFloatGreaterFloat ,&SQLVariant::OperFloatGreaterDouble ,&SQLVariant::OperFloatGreaterBit ,&SQLVariant::OperFloatGreaterSTiny ,&SQLVariant::OperFloatGreaterUTiny ,&SQLVariant::OperFloatGreaterSBig ,&SQLVariant::OperFloatGreaterUBig ,&SQLVariant::OperFloatGreaterNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_DOUBLE      */  ,{  &SQLVariant::OperVarGreaterChar ,&SQLVariant::OperDoubleGreaterSShort,&SQLVariant::OperDoubleGreaterUShort,&SQLVariant::OperDoubleEGreaterSLong,&SQLVariant::OperDoubleGreaterULong,&SQLVariant::OperDoubleGreaterFloat,&SQLVariant::OperDoubleGreaterDouble,&SQLVariant::OperDoubleGreaterBit,&SQLVariant::OperDoubleGreaterSTiny,&SQLVariant::OperDoubleGreaterUTiny,&SQLVariant::OperDoubleGreaterSBig,&SQLVariant::OperDoubleGreaterUBig,&SQLVariant::OperDoubleGreaterNum,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_BIT         */  ,{  &SQLVariant::OperVarGreaterChar ,&SQLVariant::OperBitGreaterSShort   ,&SQLVariant::OperBitGreaterUShort   ,&SQLVariant::OperBitGreaterSLong    ,&SQLVariant::OperBitGreaterULong   ,&SQLVariant::OperBitGreaterFloat   ,&SQLVariant::OperBitGreaterDouble   ,&SQLVariant::OperBitGreaterBit   ,&SQLVariant::OperBitGreaterSTiny   ,&SQLVariant::OperBitGreaterUTiny   ,&SQLVariant::OperBitGreaterSBig   ,&SQLVariant::OperBitGreaterUBig   ,&SQLVariant::OperBitGreaterNum   ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_STINYINT    */  ,{  &SQLVariant::OperVarGreaterChar ,&SQLVariant::OperSTinyGreaterSShort ,&SQLVariant::OperSTinyGreaterUShort ,&SQLVariant::OperSTinyGreaterSLong  ,&SQLVariant::OperSTinyGreaterULong ,&SQLVariant::OperSTinyGreaterFloat ,&SQLVariant::OperSTinyGreaterDouble ,&SQLVariant::OperSTinyGreaterBit ,&SQLVariant::OperSTinyGreaterSTiny ,&SQLVariant::OperSTinyGreaterUTiny ,&SQLVariant::OperSTinyGreaterSBig ,&SQLVariant::OperSTinyGreaterUBig ,&SQLVariant::OperSTinyGreaterNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_UTINYINT    */  ,{  &SQLVariant::OperVarGreaterChar ,&SQLVariant::OperUTinyGreaterSShort ,&SQLVariant::OperUTinyGreaterUShort ,&SQLVariant::OperUTinyGreaterSLong  ,&SQLVariant::OperUTinyGreaterULong ,&SQLVariant::OperUTinyGreaterFloat ,&SQLVariant::OperUTinyGreaterDouble ,&SQLVariant::OperUTinyGreaterBit ,&SQLVariant::OperUTinyGreaterSTiny ,&SQLVariant::OperUTinyGreaterUTiny ,&SQLVariant::OperUTinyGreaterSBig ,&SQLVariant::OperUTinyGreaterUBig ,&SQLVariant::OperUTinyGreaterNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_SBIGINT     */  ,{  &SQLVariant::OperVarGreaterChar ,&SQLVariant::OperSBigGreaterSShort  ,&SQLVariant::OperSBigGreaterUShort  ,&SQLVariant::OperSBigGreaterSLong   ,&SQLVariant::OperSBigGreaterULong  ,&SQLVariant::OperSBigGreaterFloat  ,&SQLVariant::OperSBigGreaterDouble  ,&SQLVariant::OperSBigGreaterBit  ,&SQLVariant::OperSBigGreaterSTiny  ,&SQLVariant::OperSBigGreaterUTiny  ,&SQLVariant::OperSBigGreaterSBig  ,&SQLVariant::OperSBigGreaterUBig  ,&SQLVariant::OperSBigGreaterNum  ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_UBIGINT     */  ,{  &SQLVariant::OperVarGreaterChar ,&SQLVariant::OperUBigGreaterSShort  ,&SQLVariant::OperUBigGreaterUShort  ,&SQLVariant::OperUBigGreaterSLong   ,&SQLVariant::OperUBigGreaterULong  ,&SQLVariant::OperUBigGreaterFloat  ,&SQLVariant::OperUBigGreaterDouble  ,&SQLVariant::OperUBigGreaterBit  ,&SQLVariant::OperUBigGreaterSTiny  ,&SQLVariant::OperUBigGreaterUTiny  ,&SQLVariant::OperUBigGreaterSBig  ,&SQLVariant::OperUBigGreaterUBig  ,&SQLVariant::OperUBigGreaterNum  ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_NUMERIC     */  ,{  &SQLVariant::OperVarGreaterChar ,&SQLVariant::OperNumGreaterSShort   ,&SQLVariant::OperNumGreaterUShort   ,&SQLVariant::OperNumGreaterSLong    ,&SQLVariant::OperNumGreaterULong   ,&SQLVariant::OperNumGreaterFloat   ,&SQLVariant::OperNumGreaterDouble   ,&SQLVariant::OperNumGreaterBit   ,&SQLVariant::OperNumGreaterSTiny   ,&SQLVariant::OperNumGreaterUTiny   ,&SQLVariant::OperNumGreaterSBig   ,&SQLVariant::OperNumGreaterUBig   ,&SQLVariant::OperNumGreaterNum   ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_GUID        */  ,{  &SQLVariant::OperGuidGreaterChar,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,&SQLVariant::OperGuidGreaterGuid,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_BINARY      */  ,{  nullptr                         ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,&SQLVariant::OperBinaryGreaterBinary,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_DATE        */  ,{  &SQLVariant::OperVarGreaterChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,&SQLVariant::OperDateGreaterDate ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_TIME        */  ,{  &SQLVariant::OperVarGreaterChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,nullptr                          ,&SQLVariant::OperTimeGreaterTime ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_TIMESTAMP   */  ,{  &SQLVariant::OperVarGreaterChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,&SQLVariant::OperStampGreaterDate,&SQLVariant::OperStampGreaterTime,&SQLVariant::OperStampGreaterStamp,nullptr                           ,nullptr                            }
  /* CT_INTERVAL_YM */  ,{  &SQLVariant::OperVarGreaterChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,&SQLVariant::OperIntYMGreaterIntYM,nullptr                            }
  /* CT_INTERVAL_DS */  ,{  &SQLVariant::OperVarGreaterChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,&SQLVariant::OperIntDSGreaterIntDS }
};

// Greater operator for SQLVariant
bool
SQLVariant::operator>(SQLVariant& p_right)
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
  OperatorCompare function = OperatorGreater[left][right].function;
  if(function)
  {
    return (this->*(function))(p_right);
  }
  // No compare function found
  // Data types are not comparable
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the greater operator on (%s > %s)",leftType,rightType);
  throw error;
}

//////////////////////////////////////////////////////////////////////////
//
// ALL EQUALITY OPERATOR FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

bool
SQLVariant::OperVarGreaterChar(SQLVariant& p_right)
{
  CString leftString,rightString;
  GetAsString(leftString);
  p_right.GetAsString(rightString);

  return leftString.Compare(rightString) > 0;
}

bool
SQLVariant::OperGuidGreaterChar(SQLVariant& p_right)
{
  CString leftString,rightString;
  GetAsString(leftString);
  p_right.GetAsString(rightString);

  leftString. TrimLeft ('{');
  leftString. TrimRight('}');
  rightString.TrimLeft ('}');
  rightString.TrimRight('{');

  return leftString.Compare(rightString) > 0;
}

// SIGNED SHORT

bool
SQLVariant::OperSShortGreaterSShort(SQLVariant& p_right)
{
  return GetAsSShort() > p_right.GetAsSShort();
}

bool
SQLVariant::OperUShortGreaterSShort(SQLVariant& p_right)
{
  return GetAsUShort() > ShortToUShort(p_right.GetAsSShort());
}

bool
SQLVariant::OperSLongGreaterSShort(SQLVariant& p_right)
{
  return SLongToShort(GetAsSLong()) > p_right.GetAsSShort();
}

bool
SQLVariant::OperULongGreaterSShort(SQLVariant& p_right)
{
  return ULongToUShort(GetAsULong()) > ShortToUShort(p_right.GetAsSShort());
}

bool
SQLVariant::OperFloatGreaterSShort(SQLVariant& p_right)
{
  return FloatToShort(GetAsFloat()) > p_right.GetAsSShort();
}

bool
SQLVariant::OperDoubleGreaterSShort(SQLVariant& p_right)
{
  return DoubleToShort(GetAsDouble()) > p_right.GetAsSShort();
}

bool
SQLVariant::OperBitGreaterSShort(SQLVariant& p_right)
{
  return GetAsBit() != 0 && p_right.GetAsSShort() == 0;
}

bool   
SQLVariant::OperSTinyGreaterSShort(SQLVariant& p_right)
{
  return GetAsSTinyInt() > ShortToTinyInt(p_right.GetAsSShort());
}

bool
SQLVariant::OperUTinyGreaterSShort(SQLVariant& p_right)
{
  return GetAsUTinyInt() > ShortToUTinyInt(p_right.GetAsSShort());
}

bool
SQLVariant::OperSBigGreaterSShort(SQLVariant& p_right)
{
  return BIGINTToShort(GetAsSBigInt()) > p_right.GetAsSShort();
}

bool
SQLVariant::OperUBigGreaterSShort(SQLVariant& p_right)
{
  return UBIGINTToShort(GetAsUBigInt()) > p_right.GetAsSShort();
}

bool
SQLVariant::OperNumGreaterSShort(SQLVariant& p_right)
{
  return SLongToShort(GetAsBCD().AsLong()) > p_right.GetAsUShort();
}

// UNSIGNED SHORT

bool   
SQLVariant::OperSShortGreaterUShort(SQLVariant& p_right)
{
  return ShortToUShort(GetAsSShort()) > p_right.GetAsUShort();
}

bool   
SQLVariant::OperUShortGreaterUShort(SQLVariant& p_right)
{
  return GetAsUShort() > p_right.GetAsUShort();
}

bool
SQLVariant::OperSLongGreaterUShort(SQLVariant& p_right)
{
  return SLongToUShort(GetAsSLong()) > p_right.GetAsUShort();
}

bool
SQLVariant::OperUlongGreaterUShort(SQLVariant& p_right)
{
  return ULongToUShort(GetAsULong()) > p_right.GetAsUShort();
}

bool
SQLVariant::OperFloatGreaterUShort(SQLVariant& p_right)
{
  return FloatToUShort(GetAsFloat()) > p_right.GetAsUShort();
}

bool
SQLVariant::OperDoubleGreaterUShort(SQLVariant& p_right)
{
  return DoubleToUShort(GetAsDouble()) > p_right.GetAsUShort();
}

bool
SQLVariant::OperBitGreaterUShort(SQLVariant& p_right)
{
  return GetAsBit() != 0 && p_right.GetAsUShort() == 0;
}

bool
SQLVariant::OperSTinyGreaterUShort(SQLVariant& p_right)
{
  return GetAsSTinyInt() > UShortToTinyInt(p_right.GetAsUShort());
}

bool
SQLVariant::OperUTinyGreaterUShort(SQLVariant& p_right)
{
  return GetAsUTinyInt() > UShortToUTinyInt(p_right.GetAsUShort());
}

bool
SQLVariant::OperSBigGreaterUShort(SQLVariant& p_right)
{
  return BIGINTToUShort(GetAsSBigInt()) > p_right.GetAsUShort();
}

bool
SQLVariant::OperUBigGreaterUShort(SQLVariant& p_right)
{
  return UBIGINTToUShort(GetAsUBigInt()) > p_right.GetAsUShort();
}

bool
SQLVariant::OperNumGreaterUShort(SQLVariant& p_right)
{
  return SLongToUShort(GetAsBCD().AsLong()) > p_right.GetAsUShort();
}


// SIGNED LONG

bool   
SQLVariant::OperSShortGreaterSLong(SQLVariant& p_right)
{
  return GetAsSShort() > SLongToShort(p_right.GetAsSLong());
}

bool
SQLVariant::OperUShortGreaterSLong(SQLVariant& p_right)
{
  return GetAsUShort() > SLongToUShort(p_right.GetAsSLong());
}

bool
SQLVariant::OperSLongGreaterSLong(SQLVariant& p_right)
{
  return GetAsSLong() > p_right.GetAsSLong();
}

bool
SQLVariant::OperULongGreaterSLong(SQLVariant& p_right)
{
  return GetAsULong() > LongToULong(p_right.GetAsSLong());
}

bool
SQLVariant::OperFloatGreaterSLong(SQLVariant& p_right)
{
  return FloatToLong(GetAsFloat()) > p_right.GetAsSLong();
}

bool
SQLVariant::OperDoubleEGreaterSLong(SQLVariant& p_right)
{
  return DoubleToLong(GetAsDouble()) > p_right.GetAsSLong();
}

bool
SQLVariant::OperBitGreaterSLong(SQLVariant& p_right)
{
  return GetAsBit() != 0 && p_right.GetAsSLong() == 0;
}

bool
SQLVariant::OperSTinyGreaterSLong(SQLVariant& p_right)
{
  return GetAsSTinyInt() > SLongToTinyInt(p_right.GetAsSLong());
}

bool
SQLVariant::OperUTinyGreaterSLong(SQLVariant& p_right)
{
  return GetAsUTinyInt() > SLongToUTinyInt(p_right.GetAsSLong());
}

bool
SQLVariant::OperSBigGreaterSLong(SQLVariant& p_right)
{
  return BIGINTToLong(GetAsSBigInt()) > p_right.GetAsSLong();
}

bool
SQLVariant::OperUBigGreaterSLong(SQLVariant& p_right)
{
  return UBIGINTToLong(GetAsUBigInt()) > p_right.GetAsSLong();
}

bool
SQLVariant::OperNumGreaterSLong(SQLVariant& p_right)
{
  return GetAsBCD().AsLong() > p_right.GetAsSLong();
}

// UNSIGNED LONG

bool   
SQLVariant::OperSShortGreaterULong(SQLVariant& p_right)
{
  return ShortToUShort(GetAsSShort()) > ULongToUShort(p_right.GetAsULong());
}

bool
SQLVariant::OperUShortGreaterULong(SQLVariant& p_right)
{
  return GetAsUShort() > ULongToUShort(p_right.GetAsULong());
}

bool
SQLVariant::OperSLongGreaterULong(SQLVariant& p_right)
{
  return LongToULong(GetAsSLong()) > p_right.GetAsULong();
}

bool
SQLVariant::OperULongGreaterULong(SQLVariant& p_right)
{
  return GetAsULong() > p_right.GetAsULong();
}

bool
SQLVariant::OperFloatGreaterULong(SQLVariant& p_right)
{
  return FloatToULong(GetAsFloat()) > p_right.GetAsULong();
}

bool
SQLVariant::OperDoubleGreaterULong(SQLVariant& p_right)
{
  return DoubleToULong(GetAsDouble()) > p_right.GetAsULong();
}

bool
SQLVariant::OperBitGreaterULong(SQLVariant& p_right)
{
  return GetAsBit() != 0 && p_right.GetAsULong() == 0;
}

bool
SQLVariant::OperSTinyGreaterULong(SQLVariant& p_right)
{
  return TinyIntToUTinyInt(GetAsSTinyInt()) > ULongToUTinyInt(p_right.GetAsULong());
}

bool
SQLVariant::OperUTinyGreaterULong(SQLVariant& p_right)
{
  return GetAsUTinyInt() > ULongToUTinyInt(p_right.GetAsULong());
}

bool
SQLVariant::OperSBigGreaterULong(SQLVariant& p_right)
{
  return BIGINTToULong(GetAsSBigInt()) > p_right.GetAsULong();
}

bool
SQLVariant::OperUBigGreaterULong(SQLVariant& p_right)
{
  return UBIGINTToULong(GetAsUBigInt()) > p_right.GetAsULong();
}

bool
SQLVariant::OperNumGreaterULong(SQLVariant& p_right)
{
  return BIGINTToULong(GetAsBCD().AsInt64()) > p_right.GetAsULong();
}

// FLOAT

bool   
SQLVariant::OperSShortGreaterFloat(SQLVariant& p_right)
{
  return GetAsSShort() > FloatToShort(p_right.GetAsFloat());
}

bool   
SQLVariant::OperUShortGreaterFloat(SQLVariant& p_right)
{
  return GetAsUShort() > FloatToUShort(p_right.GetAsFloat());
}

bool
SQLVariant::OperSLongGreaterFloat(SQLVariant& p_right)
{
  return GetAsSLong() > FloatToLong(p_right.GetAsFloat());
}

bool
SQLVariant::OperULongGreaterFloat(SQLVariant& p_right)
{
  return GetAsULong() > FloatToULong(p_right.GetAsFloat());
}

bool
SQLVariant::OperFloatGreaterFloat(SQLVariant& p_right)
{
  return GetAsFloat() > p_right.GetAsFloat();
}

bool
SQLVariant::OperDoubleGreaterFloat(SQLVariant& p_right)
{
  return DoubleToFloat(GetAsDouble()) > p_right.GetAsFloat();
}

bool
SQLVariant::OperBitGreaterFloat(SQLVariant& p_right)
{
  return GetAsBit() != 0 && p_right.GetAsFloat() == 0.0;
}

bool
SQLVariant::OperSTinyGreaterFloat(SQLVariant& p_right)
{
  return GetAsSTinyInt() > FloatToTinyInt(p_right.GetAsFloat());
}

bool
SQLVariant::OperUTinyGreaterFloat(SQLVariant& p_right)
{
  return GetAsUTinyInt() > FloatToUTinyInt(p_right.GetAsFloat());
}

bool
SQLVariant::OperSBigGreaterFloat(SQLVariant& p_right)
{
  return GetAsSBigInt() > FloatToBIGINT(p_right.GetAsFloat());
}

bool
SQLVariant::OperUBigGreaterFloat(SQLVariant& p_right)
{
  return GetAsUBigInt() > FloatToUBIGINT(p_right.GetAsFloat());
}

bool
SQLVariant::OperNumGreaterFloat(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  double lfdouble = atof(left);

  return DoubleToFloat(lfdouble) > p_right.GetAsFloat();
}

// DOUBLE

bool
SQLVariant::OperSShortGreaterDouble(SQLVariant& p_right)
{
  return GetAsSShort() > DoubleToShort(p_right.GetAsDouble());
}

bool
SQLVariant::OperUShortGreaterDouble(SQLVariant& p_right)
{
  return GetAsUShort() > DoubleToUShort(p_right.GetAsDouble());
}

bool
SQLVariant::OperSLongGreaterDouble(SQLVariant& p_right)
{
  return GetAsSLong() > DoubleToLong(p_right.GetAsDouble());
}

bool
SQLVariant::OperULongGreaterDouble(SQLVariant& p_right)
{
  return GetAsULong() > DoubleToULong(p_right.GetAsDouble());
}

bool
SQLVariant::OperFloatGreaterDouble(SQLVariant& p_right)
{
  return GetAsFloat() > DoubleToFloat(p_right.GetAsDouble());
}

bool
SQLVariant::OperDoubleGreaterDouble(SQLVariant& p_right)
{
  return GetAsDouble() > p_right.GetAsDouble();
}

bool
SQLVariant::OperBitGreaterDouble(SQLVariant& p_right)
{
  return GetAsBit() != 0 && p_right.GetAsDouble() == 0.0;
}

bool
SQLVariant::OperSTinyGreaterDouble(SQLVariant& p_right)
{
  return GetAsSTinyInt() > DoubleToTinyInt(p_right.GetAsDouble());
}

bool
SQLVariant::OperUTinyGreaterDouble(SQLVariant& p_right)
{
  return GetAsUTinyInt() > DoubleToUTinyInt(p_right.GetAsDouble());
}

bool
SQLVariant::OperSBigGreaterDouble(SQLVariant& p_right)
{
  return GetAsSBigInt() > DoubleToBIGINT(p_right.GetAsDouble());
}

bool
SQLVariant::OperUBigGreaterDouble(SQLVariant& p_right)
{
  return GetAsUBigInt() > DoubleToUBIGINT(p_right.GetAsDouble());
}

bool
SQLVariant::OperNumGreaterDouble(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  double lfdouble = atof(left);
  return lfdouble > p_right.GetAsDouble();
}

// BIT

bool   
SQLVariant::OperSShortGreaterBit(SQLVariant& p_right)
{
  return GetAsSShort() != 0 && p_right.GetAsBit() == 0;
}

bool
SQLVariant::OperUShortGreaterBit(SQLVariant& p_right)
{
  return GetAsUShort() > 0 && p_right.GetAsBit() == 0;
}

bool   
SQLVariant::OperSLongGreaterBit(SQLVariant& p_right)
{
  return GetAsSLong() > 0 && p_right.GetAsBit() == 0;
}

bool
SQLVariant::OperULongGreaterBit(SQLVariant& p_right)
{
  return GetAsULong() > 0 && p_right.GetAsBit() == 0;
}

bool
SQLVariant::OperFloatGreaterBit(SQLVariant& p_right)
{
  return GetAsFloat() > 0.0 && p_right.GetAsBit() == 0;
}

bool
SQLVariant::OperDoubleGreaterBit(SQLVariant& p_right)
{
  return GetAsDouble() > 0.0 && p_right.GetAsBit() == 0;
}

bool
SQLVariant::OperBitGreaterBit(SQLVariant& p_right)
{
  return GetAsBit() > p_right.GetAsBit();
}

bool
SQLVariant::OperSTinyGreaterBit(SQLVariant& p_right)
{
  return GetAsSTinyInt() > 0 && p_right.GetAsBit() == 0;
}

bool
SQLVariant::OperUTinyGreaterBit(SQLVariant& p_right)
{
  return GetAsUTinyInt() > 0 && p_right.GetAsBit() == 0;
}

bool
SQLVariant::OperSBigGreaterBit(SQLVariant& p_right)
{
  return GetAsSBigInt() > 0 && p_right.GetAsBit() == 0;
}

bool
SQLVariant::OperUBigGreaterBit(SQLVariant& p_right)
{
  return GetAsUBigInt() > 0 && p_right.GetAsBit() == 0;
}

bool
SQLVariant::OperNumGreaterBit(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  int lf = atoi(left);
 
  return lf != 0 && p_right.GetAsBit() == 0;
}

// SIGNED TINYINT

bool
SQLVariant::OperSShortGreaterSTiny(SQLVariant& p_right)
{
  return ShortToTinyInt(GetAsSShort()) > p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUShortGreaterSTiny(SQLVariant& p_right)
{
  return UShortToTinyInt(GetAsUShort()) > p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperSLongGreaterSTiny(SQLVariant& p_right)
{
  return SLongToTinyInt(GetAsSLong()) > p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperULongGreaterSTiny(SQLVariant& p_right)
{
  return ULongToTinyInt(GetAsULong()) > p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperFloatGreaterSTiny(SQLVariant& p_right)
{
  return FloatToTinyInt(GetAsFloat()) > p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperDoubleGreaterSTiny(SQLVariant& p_right)
{
  return DoubleToTinyInt(GetAsDouble()) > p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperBitGreaterSTiny(SQLVariant& p_right)
{
  return GetAsBit() != 0 && p_right.GetAsSTinyInt() == 0;
}

bool
SQLVariant::OperSTinyGreaterSTiny(SQLVariant& p_right)
{
  return GetAsSTinyInt() > p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUTinyGreaterSTiny(SQLVariant& p_right)
{
  return GetAsUTinyInt() > TinyIntToUTinyInt(p_right.GetAsSTinyInt());
}

bool
SQLVariant::OperSBigGreaterSTiny(SQLVariant& p_right)
{
  return SBIGINTToTinyInt(GetAsSBigInt()) > p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUBigGreaterSTiny(SQLVariant& p_right)
{
  return UBIGINTToTinyInt(GetAsUBigInt()) > p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperNumGreaterSTiny(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  int lf = atoi(left);

  return SLongToTinyInt(lf) > p_right.GetAsSTinyInt();
}

// UNSIGNED TINYINT

bool
SQLVariant::OperSShortGreaterUTiny(SQLVariant& p_right)
{
  return ShortToUTinyInt(GetAsSShort()) > p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUShortGreaterUTiny(SQLVariant& p_right)
{
  return UShortToUTinyInt(GetAsUShort()) > p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperSLongGreaterUTiny(SQLVariant& p_right)
{
  return SLongToUTinyInt(GetAsSLong()) > p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperULongGreaterUTiny(SQLVariant& p_right)
{
  return ULongToUTinyInt(GetAsULong()) > p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperFloatGreaterUTiny(SQLVariant& p_right)
{
  return FloatToUTinyInt(GetAsFloat()) > p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperDoubleGreaterUTiny(SQLVariant& p_right)
{
  return DoubleToUTinyInt(GetAsDouble()) > p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperBitGreaterUTiny(SQLVariant& p_right)
{
  return GetAsBit() != 0 && p_right.GetAsUTinyInt() == 0;
}

bool
SQLVariant::OperSTinyGreaterUTiny(SQLVariant& p_right)
{
  return TinyIntToUTinyInt(GetAsSTinyInt()) > p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUTinyGreaterUTiny(SQLVariant& p_right)
{
  return GetAsUTinyInt() > p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperSBigGreaterUTiny(SQLVariant& p_right)
{
  return SBIGINTToUTinyInt(GetAsSBigInt()) > p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUBigGreaterUTiny(SQLVariant& p_right)
{
  return UBIGINTToUTinyInt(GetAsUBigInt()) > p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperNumGreaterUTiny(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  int lf = atoi(left);

  return SLongToUTinyInt(lf) > p_right.GetAsUTinyInt();
}

// SIGNED BIGINT

bool
SQLVariant::OperSShortGreaterSBig(SQLVariant& p_right)
{
  return GetAsSShort() > BIGINTToShort(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperUShortGreaterSBig(SQLVariant& p_right)
{
  return GetAsUShort() > BIGINTToUShort(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperSLongGreaterSBig(SQLVariant& p_right)
{
  return GetAsSLong() > BIGINTToLong(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperULongGreaterSBig(SQLVariant& p_right)
{
  return GetAsULong() > BIGINTToULong(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperFloatGreaterSBig(SQLVariant& p_right)
{
  return GetAsFloat() > BIGINTToFloat(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperDoubleGreaterSBig(SQLVariant& p_right)
{
  return GetAsDouble() > BIGINTToDouble(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperBitGreaterSBig(SQLVariant& p_right)
{
  return GetAsBit() != 0 && p_right.GetAsSBigInt() == 0;
}

bool
SQLVariant::OperSTinyGreaterSBig(SQLVariant& p_right)
{
  return GetAsSTinyInt() > SBIGINTToTinyInt(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperUTinyGreaterSBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() > SBIGINTToUTinyInt(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperSBigGreaterSBig(SQLVariant& p_right)
{
  return GetAsSBigInt() > p_right.GetAsSBigInt();
}

bool
SQLVariant::OperUBigGreaterSBig(SQLVariant& p_right)
{
  return GetAsUBigInt() > SBIGINTToUBIGINT(p_right.GetAsSBigInt());
}

bool   
SQLVariant::OperNumGreaterSBig(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  SQLBIGINT lf = _atoi64(left);

  return lf > p_right.GetAsSBigInt();
}

// UNSIGNED BIGINT

bool
SQLVariant::OperSShortGreaterUBig(SQLVariant& p_right)
{
  return GetAsSShort() > UBIGINTToShort(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperUShortGreaterUBig(SQLVariant& p_right)
{
  return GetAsUShort() > UBIGINTToUShort(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperSLongGreaterUBig(SQLVariant& p_right)
{
  return GetAsSLong() > UBIGINTToLong(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperULongGreaterUBig(SQLVariant& p_right)
{
  return GetAsULong() > UBIGINTToULong(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperFloatGreaterUBig(SQLVariant& p_right)
{
  return GetAsFloat() > UBIGINTToFloat(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperDoubleGreaterUBig(SQLVariant& p_right)
{
  return GetAsDouble() > UBIGINTToDouble(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperBitGreaterUBig(SQLVariant& p_right)
{
  return GetAsBit() != 0 && p_right.GetAsUBigInt() == 0;
}

bool
SQLVariant::OperSTinyGreaterUBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() > UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperUTinyGreaterUBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() > UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperSBigGreaterUBig(SQLVariant& p_right)
{
  return SBIGINTToUBIGINT(GetAsSBigInt()) > p_right.GetAsUBigInt();
}

bool
SQLVariant::OperUBigGreaterUBig(SQLVariant& p_right)
{
  return GetAsUBigInt() > p_right.GetAsUBigInt();
}

bool
SQLVariant::OperNumGreaterUBig(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  SQLBIGINT lf = _atoi64(left);

  return lf > UBIGINTToBIGINT(p_right.GetAsUBigInt());
}

// NUMERIC

bool
SQLVariant::OperSShortGreaterNum(SQLVariant& p_right)
{
  return GetAsSShort() > (short) p_right.GetAsBCD().AsLong();
}

bool
SQLVariant::OperUShortGreaterNum(SQLVariant& p_right)
{
  return GetAsUShort() > (unsigned short) p_right.GetAsBCD().AsLong();
}

bool
SQLVariant::OperSLongGreaterNum(SQLVariant& p_right)
{
  return GetAsSLong() > p_right.GetAsBCD().AsLong();
}

bool
SQLVariant::OperULongGreaterNum(SQLVariant& p_right)
{
  return GetAsULong() > (unsigned long) p_right.GetAsBCD().AsInt64();
}

bool
SQLVariant::OperFloatGreaterNum(SQLVariant& p_right)
{
  return GetAsFloat() > (float) p_right.GetAsBCD().AsDouble();
}

bool
SQLVariant::OperDoubleGreaterNum(SQLVariant& p_right)
{
  return GetAsDouble() > p_right.GetAsBCD().AsDouble();
}

bool
SQLVariant::OperBitGreaterNum(SQLVariant& p_right)
{
  int ri = p_right.GetAsBCD().AsLong();
  return GetAsBit() != 0 && ri == 0;
}

bool
SQLVariant::OperSTinyGreaterNum(SQLVariant& p_right)
{
  return GetAsSTinyInt() > SLongToTinyInt(p_right.GetAsBCD().AsLong());
}

bool
SQLVariant::OperUTinyGreaterNum(SQLVariant& p_right)
{
  return GetAsUTinyInt() > SLongToUTinyInt(p_right.GetAsBCD().AsLong());
}

bool
SQLVariant::OperSBigGreaterNum(SQLVariant& p_right)
{
  return GetAsSBigInt() > p_right.GetAsBCD().AsInt64();
}

bool
SQLVariant::OperUBigGreaterNum(SQLVariant& p_right)
{
  return GetAsUBigInt() > p_right.GetAsBCD().AsUInt64();
}

bool
SQLVariant::OperNumGreaterNum(SQLVariant& p_right)
{
  return GetAsBCD() == p_right.GetAsBCD();
}

// TYPE == GUID

bool
SQLVariant::OperGuidGreaterGuid(SQLVariant& p_right)
{
  SQLGUID* left  = GetAsGUID();
  SQLGUID* right = p_right.GetAsGUID();

  return memcmp(left,right,sizeof(SQLGUID)) > 0;
}

// TYPE == BINARY

bool
SQLVariant::OperBinaryGreaterBinary(SQLVariant& p_right)
{
  void* left   = GetAsBinary();
  void* right  = p_right.GetAsBinary();
  // Take longest of two binaries in comparison
  long  length = m_binaryLength > p_right.m_binaryLength ? m_binaryLength : p_right.m_binaryLength;

  return memcmp(left,right,length) > 0;
}

// TYPE == DATE

bool
SQLVariant::OperDateGreaterDate(SQLVariant& p_right)
{
  // Relies on the > operator of SQLDate
  return GetAsSQLDate() > p_right.GetAsSQLDate();
}

bool
SQLVariant::OperStampGreaterDate(SQLVariant& p_right)
{
  SQLDate date = GetAsSQLTimestamp().AsSQLDate();
  return date > p_right.GetAsSQLDate();
}

// TYPE == TIME

bool
SQLVariant::OperTimeGreaterTime(SQLVariant& p_right)
{
  // Relies on the > operator of SQLTime
  return GetAsSQLTime() > p_right.GetAsSQLTime();
}

bool
SQLVariant::OperStampGreaterTime(SQLVariant& p_right)
{
  SQLTime time = GetAsSQLTimestamp().AsSQLTime();
  return time > p_right.GetAsSQLTime();
}

// TYPE == TIMESTAMP

bool
SQLVariant::OperStampGreaterStamp(SQLVariant& p_right)
{
  // Relies on the > operator of SQLTimestamp
  return GetAsSQLTimestamp() > p_right.GetAsSQLTimestamp();
}

// TYPE == INTERVAL_YEAR_MONTH

bool
SQLVariant::OperIntYMGreaterIntYM(SQLVariant& p_right)
{
  // Relies on the calculation of months in SQLInterval
  return GetAsSQLInterval().AsDatabaseDouble() > p_right.GetAsSQLInterval().AsDatabaseDouble();
}

// TYPE == INTERVAL_DAY_SECOND

bool
SQLVariant::OperIntDSGreaterIntDS(SQLVariant& p_right)
{
  // Relies on the calculation of seconds in SQLInterval
  return GetAsSQLInterval().AsDatabaseDouble() > p_right.GetAsSQLInterval().AsDatabaseDouble();
}
