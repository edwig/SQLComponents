////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorSmaller.cpp
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
SQLVariant::OperatorSmaller[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                          CT_SSHORT                            CT_USHORT                            CT_SLONG                             CT_ULONG                            CT_FLOAT                            CT_DOUBLE                            CT_BIT                            CT_STINYINT                         CT_UTINYINT                         CT_SBIGINT                         CT_UBIGINT                         CT_NUMERIC                        CT_GUID                          CT_BINARY                            CT_DATE                           CT_TIME                           CT_TIMESTAMP                       CT_INTERVAL_YM                     CT_INTERVAL_DS
                         // -------------------------------- ------------------------------------ ------------------------------------ ------------------------------------ ----------------------------------- ----------------------------------- ------------------------------------ --------------------------------- ----------------------------------- ----------------------------------- ---------------------------------- ---------------------------------- --------------------------------- -------------------------------- ------------------------------------ --------------------------------- --------------------------------- ---------------------------------- ---------------------------------- -----------------------------------
  /* CT_CHAR        */   {  &SQLVariant::OperVarSmallerChar ,&SQLVariant::OperVarSmallerChar     ,&SQLVariant::OperVarSmallerChar     ,&SQLVariant::OperVarSmallerChar     ,&SQLVariant::OperVarSmallerChar    ,&SQLVariant::OperVarSmallerChar    ,&SQLVariant::OperVarSmallerChar     ,&SQLVariant::OperVarSmallerChar  ,&SQLVariant::OperVarSmallerChar    ,&SQLVariant::OperVarSmallerChar    ,&SQLVariant::OperVarSmallerChar   ,&SQLVariant::OperVarSmallerChar   ,&SQLVariant::OperVarSmallerChar  ,&SQLVariant::OperVarSmallerChar ,nullptr                             ,&SQLVariant::OperVarSmallerChar  ,&SQLVariant::OperVarSmallerChar  ,&SQLVariant::OperVarSmallerChar   ,&SQLVariant::OperVarSmallerChar   ,&SQLVariant::OperVarSmallerChar    }
  /* CT_SSHORT      */  ,{  &SQLVariant::OperVarSmallerChar ,&SQLVariant::OperSShortSmallerSShort,&SQLVariant::OperSShortSmallerUShort,&SQLVariant::OperSShortSmallerSLong ,&SQLVariant::OperSShortSmallerULong,&SQLVariant::OperSShortSmallerFloat,&SQLVariant::OperSShortSmallerDouble,&SQLVariant::OperSShortSmallerBit,&SQLVariant::OperSShortSmallerSTiny,&SQLVariant::OperSShortSmallerUTiny,&SQLVariant::OperSShortSmallerSBig,&SQLVariant::OperSShortSmallerUBig,&SQLVariant::OperSShortSmallerNum,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_USHORT      */  ,{  &SQLVariant::OperVarSmallerChar ,&SQLVariant::OperUShortSmallerSShort,&SQLVariant::OperUShortSmallerUShort,&SQLVariant::OperUShortSmallerSLong ,&SQLVariant::OperUShortSmallerULong,&SQLVariant::OperUShortSmallerFloat,&SQLVariant::OperUShortSmallerDouble,&SQLVariant::OperUShortSmallerBit,&SQLVariant::OperUShortSmallerSTiny,&SQLVariant::OperUShortSmallerUTiny,&SQLVariant::OperUShortSmallerSBig,&SQLVariant::OperUShortSmallerUBig,&SQLVariant::OperUShortSmallerNum,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_SLONG       */  ,{  &SQLVariant::OperVarSmallerChar ,&SQLVariant::OperSLongSmallerSShort ,&SQLVariant::OperSLongSmallerUShort ,&SQLVariant::OperSLongSmallerSLong  ,&SQLVariant::OperSLongSmallerULong ,&SQLVariant::OperSLongSmallerFloat ,&SQLVariant::OperSLongSmallerDouble ,&SQLVariant::OperSLongSmallerBit ,&SQLVariant::OperSLongSmallerSTiny ,&SQLVariant::OperSLongSmallerUTiny ,&SQLVariant::OperSLongSmallerSBig ,&SQLVariant::OperSLongSmallerUBig ,&SQLVariant::OperSLongSmallerNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_ULONG       */  ,{  &SQLVariant::OperVarSmallerChar ,&SQLVariant::OperULongSmallerSShort ,&SQLVariant::OperUlongSmallerUShort ,&SQLVariant::OperULongSmallerSLong  ,&SQLVariant::OperULongSmallerULong ,&SQLVariant::OperULongSmallerFloat ,&SQLVariant::OperULongSmallerDouble ,&SQLVariant::OperULongSmallerBit ,&SQLVariant::OperULongSmallerSTiny ,&SQLVariant::OperULongSmallerUTiny ,&SQLVariant::OperULongSmallerSBig ,&SQLVariant::OperULongSmallerUBig ,&SQLVariant::OperULongSmallerNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_FLOAT       */  ,{  &SQLVariant::OperVarSmallerChar ,&SQLVariant::OperFloatSmallerSShort ,&SQLVariant::OperFloatSmallerUShort ,&SQLVariant::OperFloatSmallerSLong  ,&SQLVariant::OperFloatSmallerULong ,&SQLVariant::OperFloatSmallerFloat ,&SQLVariant::OperFloatSmallerDouble ,&SQLVariant::OperFloatSmallerBit ,&SQLVariant::OperFloatSmallerSTiny ,&SQLVariant::OperFloatSmallerUTiny ,&SQLVariant::OperFloatSmallerSBig ,&SQLVariant::OperFloatSmallerUBig ,&SQLVariant::OperFloatSmallerNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_DOUBLE      */  ,{  &SQLVariant::OperVarSmallerChar ,&SQLVariant::OperDoubleSmallerSShort,&SQLVariant::OperDoubleSmallerUShort,&SQLVariant::OperDoubleESmallerSLong,&SQLVariant::OperDoubleSmallerULong,&SQLVariant::OperDoubleSmallerFloat,&SQLVariant::OperDoubleSmallerDouble,&SQLVariant::OperDoubleSmallerBit,&SQLVariant::OperDoubleSmallerSTiny,&SQLVariant::OperDoubleSmallerUTiny,&SQLVariant::OperDoubleSmallerSBig,&SQLVariant::OperDoubleSmallerUBig,&SQLVariant::OperDoubleSmallerNum,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_BIT         */  ,{  &SQLVariant::OperVarSmallerChar ,&SQLVariant::OperBitSmallerSShort   ,&SQLVariant::OperBitSmallerUShort   ,&SQLVariant::OperBitSmallerSLong    ,&SQLVariant::OperBitSmallerULong   ,&SQLVariant::OperBitSmallerFloat   ,&SQLVariant::OperBitSmallerDouble   ,&SQLVariant::OperBitSmallerBit   ,&SQLVariant::OperBitSmallerSTiny   ,&SQLVariant::OperBitSmallerUTiny   ,&SQLVariant::OperBitSmallerSBig   ,&SQLVariant::OperBitSmallerUBig   ,&SQLVariant::OperBitSmallerNum   ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_STINYINT    */  ,{  &SQLVariant::OperVarSmallerChar ,&SQLVariant::OperSTinySmallerSShort ,&SQLVariant::OperSTinySmallerUShort ,&SQLVariant::OperSTinySmallerSLong  ,&SQLVariant::OperSTinySmallerULong ,&SQLVariant::OperSTinySmallerFloat ,&SQLVariant::OperSTinySmallerDouble ,&SQLVariant::OperSTinySmallerBit ,&SQLVariant::OperSTinySmallerSTiny ,&SQLVariant::OperSTinySmallerUTiny ,&SQLVariant::OperSTinySmallerSBig ,&SQLVariant::OperSTinySmallerUBig ,&SQLVariant::OperSTinySmallerNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_UTINYINT    */  ,{  &SQLVariant::OperVarSmallerChar ,&SQLVariant::OperUTinySmallerSShort ,&SQLVariant::OperUTinySmallerUShort ,&SQLVariant::OperUTinySmallerSLong  ,&SQLVariant::OperUTinySmallerULong ,&SQLVariant::OperUTinySmallerFloat ,&SQLVariant::OperUTinySmallerDouble ,&SQLVariant::OperUTinySmallerBit ,&SQLVariant::OperUTinySmallerSTiny ,&SQLVariant::OperUTinySmallerUTiny ,&SQLVariant::OperUTinySmallerSBig ,&SQLVariant::OperUTinySmallerUBig ,&SQLVariant::OperUTinySmallerNum ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_SBIGINT     */  ,{  &SQLVariant::OperVarSmallerChar ,&SQLVariant::OperSBigSmallerSShort  ,&SQLVariant::OperSBigSmallerUShort  ,&SQLVariant::OperSBigSmallerSLong   ,&SQLVariant::OperSBigSmallerULong  ,&SQLVariant::OperSBigSmallerFloat  ,&SQLVariant::OperSBigSmallerDouble  ,&SQLVariant::OperSBigSmallerBit  ,&SQLVariant::OperSBigSmallerSTiny  ,&SQLVariant::OperSBigSmallerUTiny  ,&SQLVariant::OperSBigSmallerSBig  ,&SQLVariant::OperSBigSmallerUBig  ,&SQLVariant::OperSBigSmallerNum  ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_UBIGINT     */  ,{  &SQLVariant::OperVarSmallerChar ,&SQLVariant::OperUBigSmallerSShort  ,&SQLVariant::OperUBigSmallerUShort  ,&SQLVariant::OperUBigSmallerSLong   ,&SQLVariant::OperUBigSmallerULong  ,&SQLVariant::OperUBigSmallerFloat  ,&SQLVariant::OperUBigSmallerDouble  ,&SQLVariant::OperUBigSmallerBit  ,&SQLVariant::OperUBigSmallerSTiny  ,&SQLVariant::OperUBigSmallerUTiny  ,&SQLVariant::OperUBigSmallerSBig  ,&SQLVariant::OperUBigSmallerUBig  ,&SQLVariant::OperUBigSmallerNum  ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_NUMERIC     */  ,{  &SQLVariant::OperVarSmallerChar ,&SQLVariant::OperVarSmallerChar     ,&SQLVariant::OperVarSmallerChar     ,&SQLVariant::OperVarSmallerChar     ,&SQLVariant::OperVarSmallerChar    ,&SQLVariant::OperNumSmallerFloat   ,&SQLVariant::OperNumSmallerDouble   ,&SQLVariant::OperNumSmallerBit   ,&SQLVariant::OperNumSmallerSTiny   ,&SQLVariant::OperNumSmallerUTiny   ,&SQLVariant::OperNumSmallerSBig   ,&SQLVariant::OperNumSmallerUBig   ,&SQLVariant::OperNumSmallerNum   ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_GUID        */  ,{  &SQLVariant::OperGuidSmallerChar,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,&SQLVariant::OperGuidSmallerGuid,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_BINARY      */  ,{  nullptr                         ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,&SQLVariant::OperBinarySmallerBinary,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_DATE        */  ,{  &SQLVariant::OperVarSmallerChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,&SQLVariant::OperDateSmallerDate ,nullptr                          ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_TIME        */  ,{  &SQLVariant::OperVarSmallerChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,nullptr                          ,&SQLVariant::OperTimeSmallerTime ,nullptr                           ,nullptr                           ,nullptr                            }
  /* CT_TIMESTAMP   */  ,{  &SQLVariant::OperVarSmallerChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,&SQLVariant::OperStampSmallerDate,&SQLVariant::OperStampSmallerTime,&SQLVariant::OperStampSmallerStamp,nullptr                           ,nullptr                            }
  /* CT_INTERVAL_YM */  ,{  &SQLVariant::OperVarSmallerChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,&SQLVariant::OperIntYMSmallerIntYM,nullptr                            }
  /* CT_INTERVAL_DS */  ,{  &SQLVariant::OperVarSmallerChar ,nullptr                             ,nullptr                             ,nullptr                             ,nullptr                            ,nullptr                            ,nullptr                             ,nullptr                          ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                         ,nullptr                             ,nullptr                          ,nullptr                          ,nullptr                           ,nullptr                           ,&SQLVariant::OperIntDSSmallerIntDS }
};

// Smaller operator for SQLVariant
bool
SQLVariant::operator<(SQLVariant& p_right)
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
  OperatorCompare function = OperatorSmaller[left][right].function;
  if(function)
  {
    return (this->*(function))(p_right);
  }
  // No compare function found
  // Data types are not comparable
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the Smaller operator on (%s < %s)",leftType,rightType);
  throw error;
}

//////////////////////////////////////////////////////////////////////////
//
// ALL EQUALITY OPERATOR FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

bool
SQLVariant::OperVarSmallerChar(SQLVariant& p_right)
{
  CString leftString,rightString;
  GetAsString(leftString);
  p_right.GetAsString(rightString);

  return leftString.Compare(rightString) < 0;
}

bool
SQLVariant::OperGuidSmallerChar(SQLVariant& p_right)
{
  CString leftString,rightString;
  GetAsString(leftString);
  p_right.GetAsString(rightString);

  leftString. TrimLeft ('{');
  leftString. TrimRight('}');
  rightString.TrimLeft ('}');
  rightString.TrimRight('{');

  return leftString.Compare(rightString) < 0;
}

// SIGNED SHORT

bool
SQLVariant::OperSShortSmallerSShort(SQLVariant& p_right)
{
  return GetAsSShort() < p_right.GetAsSShort();
}

bool
SQLVariant::OperUShortSmallerSShort(SQLVariant& p_right)
{
  return GetAsUShort() < ShortToUShort(p_right.GetAsSShort());
}

bool
SQLVariant::OperSLongSmallerSShort(SQLVariant& p_right)
{
  return SLongToShort(GetAsSLong()) < p_right.GetAsSShort();
}

bool
SQLVariant::OperULongSmallerSShort(SQLVariant& p_right)
{
  return ULongToUShort(GetAsULong()) < ShortToUShort(p_right.GetAsSShort());
}

bool
SQLVariant::OperFloatSmallerSShort(SQLVariant& p_right)
{
  return FloatToShort(GetAsFloat()) < p_right.GetAsSShort();
}

bool
SQLVariant::OperDoubleSmallerSShort(SQLVariant& p_right)
{
  return DoubleToShort(GetAsDouble()) < p_right.GetAsSShort();
}

bool
SQLVariant::OperBitSmallerSShort(SQLVariant& p_right)
{
  return GetAsBit() == 0 && p_right.GetAsSShort() != 0;
}

bool   
SQLVariant::OperSTinySmallerSShort(SQLVariant& p_right)
{
  return GetAsSTinyInt() < ShortToTinyInt(p_right.GetAsSShort());
}

bool
SQLVariant::OperUTinySmallerSShort(SQLVariant& p_right)
{
  return GetAsUTinyInt() < ShortToUTinyInt(p_right.GetAsSShort());
}

bool
SQLVariant::OperSBigSmallerSShort(SQLVariant& p_right)
{
  return BIGINTToShort(GetAsSBigInt()) < p_right.GetAsSShort();
}

bool
SQLVariant::OperUBigSmallerSShort(SQLVariant& p_right)
{
  return UBIGINTToShort(GetAsUBigInt()) < p_right.GetAsSShort();
}

// UNSIGNED SHORT

bool   
SQLVariant::OperSShortSmallerUShort(SQLVariant& p_right)
{
  return ShortToUShort(GetAsSShort()) < p_right.GetAsUShort();
}

bool   
SQLVariant::OperUShortSmallerUShort(SQLVariant& p_right)
{
  return GetAsUShort() < p_right.GetAsUShort();
}

bool
SQLVariant::OperSLongSmallerUShort(SQLVariant& p_right)
{
  return SLongToUShort(GetAsSLong()) < p_right.GetAsUShort();
}

bool
SQLVariant::OperUlongSmallerUShort(SQLVariant& p_right)
{
  return ULongToUShort(GetAsULong()) < p_right.GetAsUShort();
}

bool
SQLVariant::OperFloatSmallerUShort(SQLVariant& p_right)
{
  return FloatToUShort(GetAsFloat()) < p_right.GetAsUShort();
}

bool
SQLVariant::OperDoubleSmallerUShort(SQLVariant& p_right)
{
  return DoubleToUShort(GetAsDouble()) < p_right.GetAsUShort();
}

bool
SQLVariant::OperBitSmallerUShort(SQLVariant& p_right)
{
  return GetAsBit() == 0 && p_right.GetAsUShort() != 0;
}

bool
SQLVariant::OperSTinySmallerUShort(SQLVariant& p_right)
{
  return GetAsSTinyInt() < UShortToTinyInt(p_right.GetAsUShort());
}

bool
SQLVariant::OperUTinySmallerUShort(SQLVariant& p_right)
{
  return GetAsUTinyInt() < UShortToUTinyInt(p_right.GetAsUShort());
}

bool
SQLVariant::OperSBigSmallerUShort(SQLVariant& p_right)
{
  return BIGINTToUShort(GetAsSBigInt()) < p_right.GetAsUShort();
}

bool
SQLVariant::OperUBigSmallerUShort(SQLVariant& p_right)
{
  return UBIGINTToUShort(GetAsUBigInt()) < p_right.GetAsUShort();
}

// SIGNED LONG

bool   
SQLVariant::OperSShortSmallerSLong(SQLVariant& p_right)
{
  return GetAsSShort() < SLongToShort(p_right.GetAsSLong());
}

bool
SQLVariant::OperUShortSmallerSLong(SQLVariant& p_right)
{
  return GetAsUShort() < SLongToUShort(p_right.GetAsSLong());
}

bool
SQLVariant::OperSLongSmallerSLong(SQLVariant& p_right)
{
  return GetAsSLong() < p_right.GetAsSLong();
}

bool
SQLVariant::OperULongSmallerSLong(SQLVariant& p_right)
{
  return GetAsULong() < LongToULong(p_right.GetAsSLong());
}

bool
SQLVariant::OperFloatSmallerSLong(SQLVariant& p_right)
{
  return FloatToLong(GetAsFloat()) < p_right.GetAsSLong();
}

bool
SQLVariant::OperDoubleESmallerSLong(SQLVariant& p_right)
{
  return DoubleToLong(GetAsDouble()) < p_right.GetAsSLong();
}

bool
SQLVariant::OperBitSmallerSLong(SQLVariant& p_right)
{
  return GetAsBit() == 0 && p_right.GetAsSLong() != 0;
}

bool
SQLVariant::OperSTinySmallerSLong(SQLVariant& p_right)
{
  return GetAsSTinyInt() < SLongToTinyInt(p_right.GetAsSLong());
}

bool
SQLVariant::OperUTinySmallerSLong(SQLVariant& p_right)
{
  return GetAsUTinyInt() < SLongToUTinyInt(p_right.GetAsSLong());
}

bool
SQLVariant::OperSBigSmallerSLong(SQLVariant& p_right)
{
  return BIGINTToLong(GetAsSBigInt()) < p_right.GetAsSLong();
}

bool
SQLVariant::OperUBigSmallerSLong(SQLVariant& p_right)
{
  return UBIGINTToLong(GetAsUBigInt()) < p_right.GetAsSLong();
}

// UNSIGNED LONG

bool   
SQLVariant::OperSShortSmallerULong(SQLVariant& p_right)
{
  return ShortToUShort(GetAsSShort()) < ULongToUShort(p_right.GetAsULong());
}

bool
SQLVariant::OperUShortSmallerULong(SQLVariant& p_right)
{
  return GetAsUShort() < ULongToUShort(p_right.GetAsULong());
}

bool
SQLVariant::OperSLongSmallerULong(SQLVariant& p_right)
{
  return LongToULong(GetAsSLong()) < p_right.GetAsULong();
}

bool
SQLVariant::OperULongSmallerULong(SQLVariant& p_right)
{
  return GetAsULong() < p_right.GetAsULong();
}

bool
SQLVariant::OperFloatSmallerULong(SQLVariant& p_right)
{
  return FloatToULong(GetAsFloat()) < p_right.GetAsULong();
}

bool
SQLVariant::OperDoubleSmallerULong(SQLVariant& p_right)
{
  return DoubleToULong(GetAsDouble()) < p_right.GetAsULong();
}

bool
SQLVariant::OperBitSmallerULong(SQLVariant& p_right)
{
  return GetAsBit() == 0 && p_right.GetAsULong() != 0;
}

bool
SQLVariant::OperSTinySmallerULong(SQLVariant& p_right)
{
  return TinyIntToUTinyInt(GetAsSTinyInt()) < ULongToUTinyInt(p_right.GetAsULong());
}

bool
SQLVariant::OperUTinySmallerULong(SQLVariant& p_right)
{
  return GetAsUTinyInt() < ULongToUTinyInt(p_right.GetAsULong());
}

bool
SQLVariant::OperSBigSmallerULong(SQLVariant& p_right)
{
  return BIGINTToULong(GetAsSBigInt()) < p_right.GetAsULong();
}

bool
SQLVariant::OperUBigSmallerULong(SQLVariant& p_right)
{
  return UBIGINTToULong(GetAsUBigInt()) < p_right.GetAsULong();
}

// FLOAT

bool   
SQLVariant::OperSShortSmallerFloat(SQLVariant& p_right)
{
  return GetAsSShort() < FloatToShort(p_right.GetAsFloat());
}

bool   
SQLVariant::OperUShortSmallerFloat(SQLVariant& p_right)
{
  return GetAsUShort() < FloatToUShort(p_right.GetAsFloat());
}

bool
SQLVariant::OperSLongSmallerFloat(SQLVariant& p_right)
{
  return GetAsSLong() < FloatToLong(p_right.GetAsFloat());
}

bool
SQLVariant::OperULongSmallerFloat(SQLVariant& p_right)
{
  return GetAsULong() < FloatToULong(p_right.GetAsFloat());
}

bool
SQLVariant::OperFloatSmallerFloat(SQLVariant& p_right)
{
  return GetAsFloat() < p_right.GetAsFloat();
}

bool
SQLVariant::OperDoubleSmallerFloat(SQLVariant& p_right)
{
  return DoubleToFloat(GetAsDouble()) < p_right.GetAsFloat();
}

bool
SQLVariant::OperBitSmallerFloat(SQLVariant& p_right)
{
  return GetAsBit() == 0 && p_right.GetAsFloat() != 0.0;
}

bool
SQLVariant::OperSTinySmallerFloat(SQLVariant& p_right)
{
  return GetAsSTinyInt() < FloatToTinyInt(p_right.GetAsFloat());
}

bool
SQLVariant::OperUTinySmallerFloat(SQLVariant& p_right)
{
  return GetAsUTinyInt() < FloatToUTinyInt(p_right.GetAsFloat());
}

bool
SQLVariant::OperSBigSmallerFloat(SQLVariant& p_right)
{
  return GetAsSBigInt() < FloatToBIGINT(p_right.GetAsFloat());
}

bool
SQLVariant::OperUBigSmallerFloat(SQLVariant& p_right)
{
  return GetAsUBigInt() < FloatToUBIGINT(p_right.GetAsFloat());
}

bool
SQLVariant::OperNumSmallerFloat(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  double lfdouble = atof(left);

  return DoubleToFloat(lfdouble) < p_right.GetAsFloat();
}

// DOUBLE

bool
SQLVariant::OperSShortSmallerDouble(SQLVariant& p_right)
{
  return GetAsSShort() < DoubleToShort(p_right.GetAsDouble());
}

bool
SQLVariant::OperUShortSmallerDouble(SQLVariant& p_right)
{
  return GetAsUShort() < DoubleToUShort(p_right.GetAsDouble());
}

bool
SQLVariant::OperSLongSmallerDouble(SQLVariant& p_right)
{
  return GetAsSLong() < DoubleToLong(p_right.GetAsDouble());
}

bool
SQLVariant::OperULongSmallerDouble(SQLVariant& p_right)
{
  return GetAsULong() < DoubleToULong(p_right.GetAsDouble());
}

bool
SQLVariant::OperFloatSmallerDouble(SQLVariant& p_right)
{
  return GetAsFloat() < DoubleToFloat(p_right.GetAsDouble());
}

bool
SQLVariant::OperDoubleSmallerDouble(SQLVariant& p_right)
{
  return GetAsDouble() < p_right.GetAsDouble();
}

bool
SQLVariant::OperBitSmallerDouble(SQLVariant& p_right)
{
  return GetAsBit() == 0 && p_right.GetAsDouble() != 0.0;
}

bool
SQLVariant::OperSTinySmallerDouble(SQLVariant& p_right)
{
  return GetAsSTinyInt() < DoubleToTinyInt(p_right.GetAsDouble());
}

bool
SQLVariant::OperUTinySmallerDouble(SQLVariant& p_right)
{
  return GetAsUTinyInt() < DoubleToUTinyInt(p_right.GetAsDouble());
}

bool
SQLVariant::OperSBigSmallerDouble(SQLVariant& p_right)
{
  return GetAsSBigInt() < DoubleToBIGINT(p_right.GetAsDouble());
}

bool
SQLVariant::OperUBigSmallerDouble(SQLVariant& p_right)
{
  return GetAsUBigInt() < DoubleToUBIGINT(p_right.GetAsDouble());
}

bool
SQLVariant::OperNumSmallerDouble(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  double lfdouble = atof(left);
  return lfdouble < p_right.GetAsDouble();
}

// BIT

bool   
SQLVariant::OperSShortSmallerBit(SQLVariant& p_right)
{
  return GetAsSShort() == 0 && p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperUShortSmallerBit(SQLVariant& p_right)
{
  return GetAsUShort() == 0 && p_right.GetAsBit() != 0;
}

bool   
SQLVariant::OperSLongSmallerBit(SQLVariant& p_right)
{
  return GetAsSLong() == 0 && p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperULongSmallerBit(SQLVariant& p_right)
{
  return GetAsULong() == 0 && p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperFloatSmallerBit(SQLVariant& p_right)
{
  return GetAsFloat() == 0.0 && p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperDoubleSmallerBit(SQLVariant& p_right)
{
  return GetAsDouble() == 0.0 && p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperBitSmallerBit(SQLVariant& p_right)
{
  return GetAsBit() < p_right.GetAsBit();
}

bool
SQLVariant::OperSTinySmallerBit(SQLVariant& p_right)
{
  return GetAsSTinyInt() == 0 && p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperUTinySmallerBit(SQLVariant& p_right)
{
  return GetAsUTinyInt() == 0 && p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperSBigSmallerBit(SQLVariant& p_right)
{
  return GetAsSBigInt() == 0 && p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperUBigSmallerBit(SQLVariant& p_right)
{
  return GetAsUBigInt() == 0 && p_right.GetAsBit() != 0;
}

bool
SQLVariant::OperNumSmallerBit(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  int lf = atoi(left);
 
  return lf == 0 && p_right.GetAsBit() != 0;
}

// SIGNED TINYINT

bool
SQLVariant::OperSShortSmallerSTiny(SQLVariant& p_right)
{
  return ShortToTinyInt(GetAsSShort()) < p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUShortSmallerSTiny(SQLVariant& p_right)
{
  return UShortToTinyInt(GetAsUShort()) < p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperSLongSmallerSTiny(SQLVariant& p_right)
{
  return SLongToTinyInt(GetAsSLong()) < p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperULongSmallerSTiny(SQLVariant& p_right)
{
  return ULongToTinyInt(GetAsULong()) < p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperFloatSmallerSTiny(SQLVariant& p_right)
{
  return FloatToTinyInt(GetAsFloat()) < p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperDoubleSmallerSTiny(SQLVariant& p_right)
{
  return DoubleToTinyInt(GetAsDouble()) < p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperBitSmallerSTiny(SQLVariant& p_right)
{
  return GetAsBit() == 0 && p_right.GetAsSTinyInt() != 0;
}

bool
SQLVariant::OperSTinySmallerSTiny(SQLVariant& p_right)
{
  return GetAsSTinyInt() < p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUTinySmallerSTiny(SQLVariant& p_right)
{
  return GetAsUTinyInt() < TinyIntToUTinyInt(p_right.GetAsSTinyInt());
}

bool
SQLVariant::OperSBigSmallerSTiny(SQLVariant& p_right)
{
  return SBIGINTToTinyInt(GetAsSBigInt()) < p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUBigSmallerSTiny(SQLVariant& p_right)
{
  return UBIGINTToTinyInt(GetAsUBigInt()) < p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperNumSmallerSTiny(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  int lf = atoi(left);

  return SLongToTinyInt(lf) < p_right.GetAsSTinyInt();
}

// UNSIGNED TINYINT

bool
SQLVariant::OperSShortSmallerUTiny(SQLVariant& p_right)
{
  return ShortToUTinyInt(GetAsSShort()) < p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUShortSmallerUTiny(SQLVariant& p_right)
{
  return UShortToUTinyInt(GetAsUShort()) < p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperSLongSmallerUTiny(SQLVariant& p_right)
{
  return SLongToUTinyInt(GetAsSLong()) < p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperULongSmallerUTiny(SQLVariant& p_right)
{
  return ULongToUTinyInt(GetAsULong()) < p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperFloatSmallerUTiny(SQLVariant& p_right)
{
  return FloatToUTinyInt(GetAsFloat()) < p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperDoubleSmallerUTiny(SQLVariant& p_right)
{
  return DoubleToUTinyInt(GetAsDouble()) < p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperBitSmallerUTiny(SQLVariant& p_right)
{
  return GetAsBit() == 0 && p_right.GetAsUTinyInt() != 0;
}

bool
SQLVariant::OperSTinySmallerUTiny(SQLVariant& p_right)
{
  return TinyIntToUTinyInt(GetAsSTinyInt()) < p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUTinySmallerUTiny(SQLVariant& p_right)
{
  return GetAsUTinyInt() < p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperSBigSmallerUTiny(SQLVariant& p_right)
{
  return SBIGINTToUTinyInt(GetAsSBigInt()) < p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUBigSmallerUTiny(SQLVariant& p_right)
{
  return UBIGINTToUTinyInt(GetAsUBigInt()) < p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperNumSmallerUTiny(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  int lf = atoi(left);

  return SLongToUTinyInt(lf) < p_right.GetAsUTinyInt();
}

// SIGNED BIGINT

bool
SQLVariant::OperSShortSmallerSBig(SQLVariant& p_right)
{
  return GetAsSShort() < BIGINTToShort(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperUShortSmallerSBig(SQLVariant& p_right)
{
  return GetAsUShort() < BIGINTToUShort(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperSLongSmallerSBig(SQLVariant& p_right)
{
  return GetAsSLong() < BIGINTToLong(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperULongSmallerSBig(SQLVariant& p_right)
{
  return GetAsULong() < BIGINTToULong(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperFloatSmallerSBig(SQLVariant& p_right)
{
  return GetAsFloat() < BIGINTToFloat(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperDoubleSmallerSBig(SQLVariant& p_right)
{
  return GetAsDouble() < BIGINTToDouble(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperBitSmallerSBig(SQLVariant& p_right)
{
  return GetAsBit() == 0 && p_right.GetAsSBigInt() != 0;
}

bool
SQLVariant::OperSTinySmallerSBig(SQLVariant& p_right)
{
  return GetAsSTinyInt() < SBIGINTToTinyInt(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperUTinySmallerSBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() < SBIGINTToUTinyInt(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperSBigSmallerSBig(SQLVariant& p_right)
{
  return GetAsSBigInt() < p_right.GetAsSBigInt();
}

bool
SQLVariant::OperUBigSmallerSBig(SQLVariant& p_right)
{
  return GetAsUBigInt() < SBIGINTToUBIGINT(p_right.GetAsSBigInt());
}

bool   
SQLVariant::OperNumSmallerSBig(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  SQLBIGINT lf = _atoi64(left);

  return lf < p_right.GetAsSBigInt();
}

// UNSIGNED BIGINT

bool
SQLVariant::OperSShortSmallerUBig(SQLVariant& p_right)
{
  return GetAsSShort() < UBIGINTToShort(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperUShortSmallerUBig(SQLVariant& p_right)
{
  return GetAsUShort() < UBIGINTToUShort(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperSLongSmallerUBig(SQLVariant& p_right)
{
  return GetAsSLong() < UBIGINTToLong(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperULongSmallerUBig(SQLVariant& p_right)
{
  return GetAsULong() < UBIGINTToULong(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperFloatSmallerUBig(SQLVariant& p_right)
{
  return GetAsFloat() < UBIGINTToFloat(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperDoubleSmallerUBig(SQLVariant& p_right)
{
  return GetAsDouble() < UBIGINTToDouble(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperBitSmallerUBig(SQLVariant& p_right)
{
  return GetAsBit() == 0 && p_right.GetAsUBigInt() != 0;
}

bool
SQLVariant::OperSTinySmallerUBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() < UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperUTinySmallerUBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() < UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperSBigSmallerUBig(SQLVariant& p_right)
{
  return SBIGINTToUBIGINT(GetAsSBigInt()) < p_right.GetAsUBigInt();
}

bool
SQLVariant::OperUBigSmallerUBig(SQLVariant& p_right)
{
  return GetAsUBigInt() < p_right.GetAsUBigInt();
}

bool
SQLVariant::OperNumSmallerUBig(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  SQLBIGINT lf = _atoi64(left);

  return lf < UBIGINTToBIGINT(p_right.GetAsUBigInt());
}

// NUMERIC

bool
SQLVariant::OperSShortSmallerNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  short ri = SLongToShort(atoi(right));

  return GetAsSShort() < ri;
}

bool
SQLVariant::OperUShortSmallerNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  unsigned short ri = SLongToUShort(atoi(right));

  return GetAsUShort() < ri;
}

bool
SQLVariant::OperSLongSmallerNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  long ri = atoi(right);

  return GetAsSLong() < ri;
}

bool
SQLVariant::OperULongSmallerNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  unsigned long ri = LongToULong(atoi(right));

  return GetAsULong() < ri;
}

bool
SQLVariant::OperFloatSmallerNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  float ri = DoubleToFloat(atof(right));

  return GetAsFloat() < ri;
}

bool
SQLVariant::OperDoubleSmallerNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  double ri = atof(right);

  return GetAsFloat() < ri;
}

bool
SQLVariant::OperBitSmallerNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  int ri = atoi(right);

  return GetAsBit() == 0 && ri != 0;
}

bool
SQLVariant::OperSTinySmallerNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  long ri = atoi(right);

  return GetAsSTinyInt() < SLongToTinyInt(ri);
}

bool
SQLVariant::OperUTinySmallerNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  long ri = atoi(right);

  return GetAsUTinyInt() < SLongToUTinyInt(ri);
}

bool
SQLVariant::OperSBigSmallerNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  SQLBIGINT ri = _atoi64(right);

  return GetAsSBigInt() < ri;
}

bool
SQLVariant::OperUBigSmallerNum(SQLVariant& p_right)
{
  CString right;
  p_right.GetAsString(right);
  SQLUBIGINT ri = SBIGINTToUBIGINT(_atoi64(right));

  return GetAsUBigInt() < ri;
}

bool
SQLVariant::OperNumSmallerNum(SQLVariant& p_right)
{
  SQL_NUMERIC_STRUCT* left  = GetAsNumeric();
  SQL_NUMERIC_STRUCT* right = p_right.GetAsNumeric();

  return memcmp(left,right,sizeof(SQL_NUMERIC_STRUCT)) < 0;
}

// TYPE == GUID

bool
SQLVariant::OperGuidSmallerGuid(SQLVariant& p_right)
{
  SQLGUID* left  = GetAsGUID();
  SQLGUID* right = p_right.GetAsGUID();

  return memcmp(left,right,sizeof(SQLGUID)) < 0;
}

// TYPE == BINARY

bool
SQLVariant::OperBinarySmallerBinary(SQLVariant& p_right)
{
  void* left   = GetAsBinary();
  void* right  = p_right.GetAsBinary();
  // Take longest of two binaries in comparison
  long  length = m_binaryLength < p_right.m_binaryLength ? m_binaryLength : p_right.m_binaryLength;

  return memcmp(left,right,length) < 0;
}

// TYPE == DATE

bool
SQLVariant::OperDateSmallerDate(SQLVariant& p_right)
{
  // Relies on the < operator of SQLDate
  return GetAsSQLDate() < p_right.GetAsSQLDate();
}

bool
SQLVariant::OperStampSmallerDate(SQLVariant& p_right)
{
  SQLDate date = GetAsSQLTimestamp().AsSQLDate();
  return date < p_right.GetAsSQLDate();
}

// TYPE == TIME

bool
SQLVariant::OperTimeSmallerTime(SQLVariant& p_right)
{
  // Relies on the < operator of SQLTime
  return GetAsSQLTime() < p_right.GetAsSQLTime();
}

bool
SQLVariant::OperStampSmallerTime(SQLVariant& p_right)
{
  SQLTime time = GetAsSQLTimestamp().AsSQLTime();
  return time < p_right.GetAsSQLTime();
}

// TYPE == TIMESTAMP

bool
SQLVariant::OperStampSmallerStamp(SQLVariant& p_right)
{
  // Relies on the < operator of SQLTimestamp
  return GetAsSQLTimestamp() < p_right.GetAsSQLTimestamp();
}

// TYPE == INTERVAL_YEAR_MONTH

bool
SQLVariant::OperIntYMSmallerIntYM(SQLVariant& p_right)
{
  // Relies on the calculation of months in SQLInterval
  return GetAsSQLInterval().AsDatabaseDouble() < p_right.GetAsSQLInterval().AsDatabaseDouble();
}

// TYPE == INTERVAL_DAY_SECOND

bool
SQLVariant::OperIntDSSmallerIntDS(SQLVariant& p_right)
{
  // Relies on the calculation of seconds in SQLInterval
  return GetAsSQLInterval().AsDatabaseDouble() < p_right.GetAsSQLInterval().AsDatabaseDouble();
}
