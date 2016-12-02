O////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorEqual.cpp
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
SQLVariant::OperatorEqual[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                         CT_SSHORT                           CT_USHORT                           CT_SLONG                            CT_ULONG                           CT_FLOAT                           CT_DOUBLE                           CT_BIT                           CT_STINYINT                        CT_UTINYINT                        CT_SBIGINT                        CT_UBIGINT                        CT_NUMERIC                       CT_GUID                         CT_BINARY                           CT_DATE                          CT_TIME                          CT_TIMESTAMP                      CT_INTERVAL_YM                    CT_INTERVAL_DS
                         // ------------------------------- ----------------------------------- ----------------------------------- ----------------------------------- ---------------------------------- ---------------------------------- ----------------------------------- -------------------------------- ---------------------------------- ---------------------------------- --------------------------------- --------------------------------- -------------------------------- ------------------------------- ----------------------------------- -------------------------------- -------------------------------- --------------------------------- --------------------------------- -----------------------------------
  /* CT_CHAR        */   {  &SQLVariant::OperVarEqualsChar ,&SQLVariant::OperVarEqualsChar     ,&SQLVariant::OperVarEqualsChar     ,&SQLVariant::OperVarEqualsChar     ,&SQLVariant::OperVarEqualsChar    ,&SQLVariant::OperVarEqualsChar    ,&SQLVariant::OperVarEqualsChar     ,&SQLVariant::OperVarEqualsBit   ,&SQLVariant::OperVarEqualsChar    ,&SQLVariant::OperVarEqualsChar    ,&SQLVariant::OperVarEqualsChar   ,&SQLVariant::OperVarEqualsChar   ,&SQLVariant::OperVarEqualsChar  ,&SQLVariant::OperVarEqualsChar ,nullptr                            ,&SQLVariant::OperVarEqualsChar  ,&SQLVariant::OperVarEqualsChar  ,&SQLVariant::OperVarEqualsChar   ,&SQLVariant::OperVarEqualsChar   ,&SQLVariant::OperVarEqualsChar    }
  /* CT_SSHORT      */  ,{  &SQLVariant::OperVarEqualsChar ,&SQLVariant::OperSShortEqualsSShort,&SQLVariant::OperSShortEqualsUShort,&SQLVariant::OperSShortEqualsSLong ,&SQLVariant::OperSShortEqualsULong,&SQLVariant::OperSShortEqualsFloat,&SQLVariant::OperSShortEqualsDouble,&SQLVariant::OperSShortEqualsBit,&SQLVariant::OperSShortEqualsSTiny,&SQLVariant::OperSShortEqualsUTiny,&SQLVariant::OperSShortEqualsSBig,&SQLVariant::OperSShortEqualsUBig,&SQLVariant::OperSShortEqualsNum,nullptr                        ,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_USHORT      */  ,{  &SQLVariant::OperVarEqualsChar ,&SQLVariant::OperUShortEqualsSShort,&SQLVariant::OperUShortEqualsUShort,&SQLVariant::OperUShortEqualsSLong ,&SQLVariant::OperUShortEqualsULong,&SQLVariant::OperUShortEqualsFloat,&SQLVariant::OperUShortEqualsDouble,&SQLVariant::OperUShortEqualsBit,&SQLVariant::OperUShortEqualsSTiny,&SQLVariant::OperUShortEqualsUTiny,&SQLVariant::OperUShortEqualsSBig,&SQLVariant::OperUShortEqualsUBig,&SQLVariant::OperUShortEqualsNum,nullptr                        ,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_SLONG       */  ,{  &SQLVariant::OperVarEqualsChar ,&SQLVariant::OperSLongEqualsSShort ,&SQLVariant::OperSLongEqualsUShort ,&SQLVariant::OperSLongEqualsSLong  ,&SQLVariant::OperSLongEqualsULong ,&SQLVariant::OperSLongEqualsFloat ,&SQLVariant::OperSLongEqualsDouble ,&SQLVariant::OperSLongEqualsBit ,&SQLVariant::OperSLongEqualsSTiny ,&SQLVariant::OperSLongEqualsUTiny ,&SQLVariant::OperSLongEqualsSBig ,&SQLVariant::OperSLongEqualsUBig ,&SQLVariant::OperSLongEqualsNum ,nullptr                        ,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_ULONG       */  ,{  &SQLVariant::OperVarEqualsChar ,&SQLVariant::OperULongEqualsSShort ,&SQLVariant::OperUlongEqualsUShort ,&SQLVariant::OperULongEqualsSLong  ,&SQLVariant::OperULongEqualsULong ,&SQLVariant::OperULongEqualsFloat ,&SQLVariant::OperULongEqualsDouble ,&SQLVariant::OperULongEqualsBit ,&SQLVariant::OperULongEqualsSTiny ,&SQLVariant::OperULongEqualsUTiny ,&SQLVariant::OperULongEqualsSBig ,&SQLVariant::OperULongEqualsUBig ,&SQLVariant::OperULongEqualsNum ,nullptr                        ,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_FLOAT       */  ,{  &SQLVariant::OperVarEqualsChar ,&SQLVariant::OperFloatEqualsSShort ,&SQLVariant::OperFloatEqualsUShort ,&SQLVariant::OperFloatEqualsSLong  ,&SQLVariant::OperFloatEqualsULong ,&SQLVariant::OperFloatEqualsFloat ,&SQLVariant::OperFloatEqualsDouble ,&SQLVariant::OperFloatEqualsBit ,&SQLVariant::OperFloatEqualsSTiny ,&SQLVariant::OperFloatEqualsUTiny ,&SQLVariant::OperFloatEqualsSBig ,&SQLVariant::OperFloatEqualsUBig ,&SQLVariant::OperFloatEqualsNum ,nullptr                        ,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_DOUBLE      */  ,{  &SQLVariant::OperVarEqualsChar ,&SQLVariant::OperDoubleEqualsSShort,&SQLVariant::OperDoubleEqualsUShort,&SQLVariant::OperDoubleEequalsSLong,&SQLVariant::OperDoubleEqualsULong,&SQLVariant::OperDoubleEqualsFloat,&SQLVariant::OperDoubleEqualsDouble,&SQLVariant::OperDoubleEqualsBit,&SQLVariant::OperDoubleEqualsSTiny,&SQLVariant::OperDoubleEqualsUTiny,&SQLVariant::OperDoubleEqualsSBig,&SQLVariant::OperDoubleEqualsUBig,&SQLVariant::OperDoubleEqualsNum,nullptr                        ,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_BIT         */  ,{  &SQLVariant::OperBitEqualsChar ,&SQLVariant::OperBitEqualsSShort   ,&SQLVariant::OperBitEqualsUShort   ,&SQLVariant::OperBitEqualsSLong    ,&SQLVariant::OperBitEqualsULong   ,&SQLVariant::OperBitEqualsFloat   ,&SQLVariant::OperBitEqualsDouble   ,&SQLVariant::OperBitEqualsBit   ,&SQLVariant::OperBitEqualsSTiny   ,&SQLVariant::OperBitEqualsUTiny   ,&SQLVariant::OperBitEqualsSBig   ,&SQLVariant::OperBitEqualsUBig   ,&SQLVariant::OperBitEqualsNum   ,nullptr                        ,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_STINYINT    */  ,{  &SQLVariant::OperVarEqualsChar ,&SQLVariant::OperSTinyEqualsSShort ,&SQLVariant::OperSTinyEqualsUShort ,&SQLVariant::OperSTinyEqualsSLong  ,&SQLVariant::OperSTinyEqualsULong ,&SQLVariant::OperSTinyEqualsFloat ,&SQLVariant::OperSTinyEqualsDouble ,&SQLVariant::OperSTinyEqualsBit ,&SQLVariant::OperSTinyEqualsSTiny ,&SQLVariant::OperSTinyEqualsUTiny ,&SQLVariant::OperSTinyEqualsSBig ,&SQLVariant::OperSTinyEqualsUBig ,&SQLVariant::OperSTinyEqualsNum ,nullptr                        ,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_UTINYINT    */  ,{  &SQLVariant::OperVarEqualsChar ,&SQLVariant::OperUTinyEqualsSShort ,&SQLVariant::OperUTinyEqualsUShort ,&SQLVariant::OperUTinyEqualsSLong  ,&SQLVariant::OperUTinyEqualsULong ,&SQLVariant::OperUTinyEqualsFloat ,&SQLVariant::OperUTinyEqualsDouble ,&SQLVariant::OperUTinyEqualsBit ,&SQLVariant::OperUTinyEqualsSTiny ,&SQLVariant::OperUTinyEqualsUTiny ,&SQLVariant::OperUTinyEqualsSBig ,&SQLVariant::OperUTinyEqualsUBig ,&SQLVariant::OperUTinyEqualsNum ,nullptr                        ,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_SBIGINT     */  ,{  &SQLVariant::OperVarEqualsChar ,&SQLVariant::OperSBigEqualsSShort  ,&SQLVariant::OperSBigEqualsUShort  ,&SQLVariant::OperSBigEqualsSLong   ,&SQLVariant::OperSBigEqualsULong  ,&SQLVariant::OperSBigEqualsFloat  ,&SQLVariant::OperSBigEqualsDouble  ,&SQLVariant::OperSBigEqualsBit  ,&SQLVariant::OperSBigEqualsSTiny  ,&SQLVariant::OperSBigEqualsUTiny  ,&SQLVariant::OperSBigEqualsSBig  ,&SQLVariant::OperSBigEqualsUBig  ,&SQLVariant::OperSBigEqualsNum  ,nullptr                        ,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_UBIGINT     */  ,{  &SQLVariant::OperVarEqualsChar ,&SQLVariant::OperUBigEqualsSShort  ,&SQLVariant::OperUBigEqualsUShort  ,&SQLVariant::OperUBigEqualsSLong   ,&SQLVariant::OperUBigEqualsULong  ,&SQLVariant::OperUBigEqualsFloat  ,&SQLVariant::OperUBigEqualsDouble  ,&SQLVariant::OperUBigEqualsBit  ,&SQLVariant::OperUBigEqualsSTiny  ,&SQLVariant::OperUBigEqualsUTiny  ,&SQLVariant::OperUBigEqualsSBig  ,&SQLVariant::OperUBigEqualsUBig  ,&SQLVariant::OperUBigEqualsNum  ,nullptr                        ,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_NUMERIC     */  ,{  &SQLVariant::OperVarEqualsChar ,&SQLVariant::OperNumEqualsSShort   ,&SQLVariant::OperNumEqualsUShort   ,&SQLVariant::OperNumEqualsSLong    ,&SQLVariant::OperNumEqualsULong   ,&SQLVariant::OperNumEqualsFloat   ,&SQLVariant::OperNumEqualsDouble   ,&SQLVariant::OperNumEqualsBit   ,&SQLVariant::OperNumEqualsSTiny   ,&SQLVariant::OperNumEqualsUTiny   ,&SQLVariant::OperNumEqualsSBig   ,&SQLVariant::OperNumEqualsUBig   ,&SQLVariant::OperNumEqualsNum   ,nullptr                        ,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_GUID        */  ,{  &SQLVariant::OperGuidEqualsChar,nullptr                            ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                            ,nullptr                         ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                          ,nullptr                         ,&SQLVariant::OperGuidEqualsGuid,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_BINARY      */  ,{  nullptr                        ,nullptr                            ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                            ,nullptr                         ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                          ,nullptr                         ,nullptr                        ,&SQLVariant::OperBinaryEqualsBinary,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_DATE        */  ,{  &SQLVariant::OperVarEqualsChar ,nullptr                            ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                            ,nullptr                         ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                          ,nullptr                         ,nullptr                        ,nullptr                            ,&SQLVariant::OperDateEqualsDate ,nullptr                         ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_TIME        */  ,{  &SQLVariant::OperVarEqualsChar ,nullptr                            ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                            ,nullptr                         ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                          ,nullptr                         ,nullptr                        ,nullptr                            ,nullptr                         ,&SQLVariant::OperTimeEqualsTime ,nullptr                          ,nullptr                          ,nullptr                           }
  /* CT_TIMESTAMP   */  ,{  &SQLVariant::OperVarEqualsChar ,nullptr                            ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                            ,nullptr                         ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                          ,nullptr                         ,nullptr                        ,nullptr                            ,&SQLVariant::OperStampEqualsDate,&SQLVariant::OperStampEqualsTime,&SQLVariant::OperStampEqualsStamp,nullptr                          ,nullptr                           }
  /* CT_INTERVAL_YM */  ,{  &SQLVariant::OperVarEqualsChar ,nullptr                            ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                            ,nullptr                         ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                          ,nullptr                         ,nullptr                        ,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,&SQLVariant::OperIntYMEqualsIntYM,nullptr                           }
  /* CT_INTERVAL_DS */  ,{  &SQLVariant::OperVarEqualsChar ,nullptr                            ,nullptr                            ,nullptr                            ,nullptr                           ,nullptr                           ,nullptr                            ,nullptr                         ,nullptr                           ,nullptr                           ,nullptr                          ,nullptr                          ,nullptr                         ,nullptr                        ,nullptr                            ,nullptr                         ,nullptr                         ,nullptr                          ,nullptr                          ,&SQLVariant::OperIntDSEqualsIntDS }
};

// Equality operator for SQLVariant
bool
SQLVariant::operator==(SQLVariant& p_right)
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
  OperatorCompare function = OperatorEqual[left][right].function;
  if(function)
  {
    return (this->*(function))(p_right);
  }
  // No compare function found
  // Data types are not comparable
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the equality operator on (%s == %s)",leftType,rightType);
  throw error;
}

// Inequality operator for SQLVariant
bool
SQLVariant::operator!=(SQLVariant& p_right)
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
  OperatorCompare function = OperatorEqual[left][right].function;
  if(function)
  {
    // Because we use the equality function, 
    // we must do a negate of the result
    return !(this->*(function))(p_right);
  }
  // No compare function found
  // Data types are not comparable
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the inequality operator on (%s != %s)",leftType,rightType);
  throw error;
}

//////////////////////////////////////////////////////////////////////////
//
// ALL EQUALITY OPERATOR FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

bool
SQLVariant::OperVarEqualsChar(SQLVariant& p_right)
{
  CString leftString,rightString;
  GetAsString(leftString);
  p_right.GetAsString(rightString);

  return leftString.Compare(rightString) == 0;
}

bool   
SQLVariant::OperBitEqualsChar(SQLVariant& p_right)
{
  if((GetAsBit() == 0 && p_right.GetAsDouble() == 0.0) ||
     (GetAsBit() != 0 && p_right.GetAsDouble() != 0.0) )
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperGuidEqualsChar(SQLVariant& p_right)
{
  CString leftString,rightString;
  GetAsString(leftString);
  p_right.GetAsString(rightString);

  leftString. TrimLeft ('{');
  leftString. TrimRight('}');
  rightString.TrimLeft ('}');
  rightString.TrimRight('{');

  return leftString.Compare(rightString) == 0;
}

// SIGNED SHORT

bool
SQLVariant::OperSShortEqualsSShort(SQLVariant& p_right)
{
  return GetAsSShort() == p_right.GetAsSShort();
}

bool
SQLVariant::OperUShortEqualsSShort(SQLVariant& p_right)
{
  return GetAsUShort() == ShortToUShort(p_right.GetAsSShort());
}

bool
SQLVariant::OperSLongEqualsSShort(SQLVariant& p_right)
{
  return SLongToShort(GetAsSLong()) == p_right.GetAsSShort();
}

bool
SQLVariant::OperULongEqualsSShort(SQLVariant& p_right)
{
  return ULongToUShort(GetAsULong()) == ShortToUShort(p_right.GetAsSShort());
}

bool
SQLVariant::OperFloatEqualsSShort(SQLVariant& p_right)
{
  return FloatToShort(GetAsFloat()) == p_right.GetAsSShort();
}

bool
SQLVariant::OperDoubleEqualsSShort(SQLVariant& p_right)
{
  return DoubleToShort(GetAsDouble()) == p_right.GetAsSShort();
}

bool
SQLVariant::OperBitEqualsSShort(SQLVariant& p_right)
{
  if((GetAsBit() != 0 && p_right.GetAsSShort() != 0) ||
     (GetAsBit() == 0 && p_right.GetAsSShort() == 0) )
  {
    return true;
  }
  return false;
}

bool   
SQLVariant::OperSTinyEqualsSShort(SQLVariant& p_right)
{
  return GetAsSTinyInt() == ShortToTinyInt(p_right.GetAsSShort());
}

bool
SQLVariant::OperUTinyEqualsSShort(SQLVariant& p_right)
{
  return GetAsUTinyInt() == ShortToUTinyInt(p_right.GetAsSShort());
}

bool
SQLVariant::OperSBigEqualsSShort(SQLVariant& p_right)
{
  return BIGINTToShort(GetAsSBigInt()) == p_right.GetAsSShort();
}

bool
SQLVariant::OperUBigEqualsSShort(SQLVariant& p_right)
{
  return UBIGINTToShort(GetAsUBigInt()) == p_right.GetAsSShort();
}

bool
SQLVariant::OperNumEqualsSShort(SQLVariant& p_right)
{
  return SLongToShort(GetAsBCD().AsLong()) == p_right.GetAsSShort();
}

// UNSIGNED SHORT

bool   
SQLVariant::OperSShortEqualsUShort(SQLVariant& p_right)
{
  return ShortToUShort(GetAsSShort()) == p_right.GetAsUShort();
}

bool   
SQLVariant::OperUShortEqualsUShort(SQLVariant& p_right)
{
  return GetAsUShort() == p_right.GetAsUShort();
}

bool
SQLVariant::OperSLongEqualsUShort(SQLVariant& p_right)
{
  return SLongToUShort(GetAsSLong()) == p_right.GetAsUShort();
}

bool
SQLVariant::OperUlongEqualsUShort(SQLVariant& p_right)
{
  return ULongToUShort(GetAsULong()) == p_right.GetAsUShort();
}

bool
SQLVariant::OperFloatEqualsUShort(SQLVariant& p_right)
{
  return FloatToUShort(GetAsFloat()) == p_right.GetAsUShort();
}

bool
SQLVariant::OperDoubleEqualsUShort(SQLVariant& p_right)
{
  return DoubleToUShort(GetAsDouble()) == p_right.GetAsUShort();
}

bool
SQLVariant::OperBitEqualsUShort(SQLVariant& p_right)
{
  if((GetAsBit() == 0 && p_right.GetAsUShort() == 0) ||
     (GetAsBit() != 0 && p_right.GetAsUShort() != 0) )
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperSTinyEqualsUShort(SQLVariant& p_right)
{
  return GetAsSTinyInt() == UShortToTinyInt(p_right.GetAsUShort());
}

bool
SQLVariant::OperUTinyEqualsUShort(SQLVariant& p_right)
{
  return GetAsUTinyInt() == UShortToUTinyInt(p_right.GetAsUShort());
}

bool
SQLVariant::OperSBigEqualsUShort(SQLVariant& p_right)
{
  return BIGINTToUShort(GetAsSBigInt()) == p_right.GetAsUShort();
}

bool
SQLVariant::OperUBigEqualsUShort(SQLVariant& p_right)
{
  return UBIGINTToUShort(GetAsUBigInt()) == p_right.GetAsUShort();
}

bool   
SQLVariant::OperNumEqualsUShort(SQLVariant& p_right)
{
  return SLongToUShort(GetAsBCD().AsLong()) == p_right.GetAsUShort();
}

// SIGNED LONG

bool   
SQLVariant::OperSShortEqualsSLong(SQLVariant& p_right)
{
  return GetAsSShort() == SLongToShort(p_right.GetAsSLong());
}

bool
SQLVariant::OperUShortEqualsSLong(SQLVariant& p_right)
{
  return GetAsUShort() == SLongToUShort(p_right.GetAsSLong());
}

bool
SQLVariant::OperSLongEqualsSLong(SQLVariant& p_right)
{
  return GetAsSLong() == p_right.GetAsSLong();
}

bool
SQLVariant::OperULongEqualsSLong(SQLVariant& p_right)
{
  return GetAsULong() == LongToULong(p_right.GetAsSLong());
}

bool
SQLVariant::OperFloatEqualsSLong(SQLVariant& p_right)
{
  return FloatToLong(GetAsFloat()) == p_right.GetAsSLong();
}

bool
SQLVariant::OperDoubleEequalsSLong(SQLVariant& p_right)
{
  return DoubleToLong(GetAsDouble()) == p_right.GetAsSLong();
}

bool
SQLVariant::OperBitEqualsSLong(SQLVariant& p_right)
{
  if((GetAsBit() == 0 && p_right.GetAsSLong() == 0) ||
     (GetAsBit() != 0 && p_right.GetAsSLong() != 0) )
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperSTinyEqualsSLong(SQLVariant& p_right)
{
  return GetAsSTinyInt() == SLongToTinyInt(p_right.GetAsSLong());
}

bool
SQLVariant::OperUTinyEqualsSLong(SQLVariant& p_right)
{
  return GetAsUTinyInt() == SLongToUTinyInt(p_right.GetAsSLong());
}

bool
SQLVariant::OperSBigEqualsSLong(SQLVariant& p_right)
{
  return BIGINTToLong(GetAsSBigInt()) == p_right.GetAsSLong();
}

bool
SQLVariant::OperUBigEqualsSLong(SQLVariant& p_right)
{
  return UBIGINTToLong(GetAsUBigInt()) == p_right.GetAsSLong();
}

bool
SQLVariant::OperNumEqualsSLong(SQLVariant& p_right)
{
  return GetAsBCD().AsLong() == p_right.GetAsSLong();
}

// UNSIGNED LONG

bool   
SQLVariant::OperSShortEqualsULong(SQLVariant& p_right)
{
  return ShortToUShort(GetAsSShort()) == ULongToUShort(p_right.GetAsULong());
}

bool
SQLVariant::OperUShortEqualsULong(SQLVariant& p_right)
{
  return GetAsUShort() == ULongToUShort(p_right.GetAsULong());
}

bool
SQLVariant::OperSLongEqualsULong(SQLVariant& p_right)
{
  return LongToULong(GetAsSLong()) == p_right.GetAsULong();
}

bool
SQLVariant::OperULongEqualsULong(SQLVariant& p_right)
{
  return GetAsULong() == p_right.GetAsULong();
}

bool
SQLVariant::OperFloatEqualsULong(SQLVariant& p_right)
{
  return FloatToULong(GetAsFloat()) == p_right.GetAsULong();
}

bool
SQLVariant::OperDoubleEqualsULong(SQLVariant& p_right)
{
  return DoubleToULong(GetAsDouble()) == p_right.GetAsULong();
}

bool
SQLVariant::OperBitEqualsULong(SQLVariant& p_right)
{
  if((GetAsBit() == 0 && p_right.GetAsULong() == 0) ||
     (GetAsBit() != 0 && p_right.GetAsULong() != 0) )
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperSTinyEqualsULong(SQLVariant& p_right)
{
  return TinyIntToUTinyInt(GetAsSTinyInt()) == ULongToUTinyInt(p_right.GetAsULong());
}

bool
SQLVariant::OperUTinyEqualsULong(SQLVariant& p_right)
{
  return GetAsUTinyInt() == ULongToUTinyInt(p_right.GetAsULong());
}

bool
SQLVariant::OperSBigEqualsULong(SQLVariant& p_right)
{
  return BIGINTToULong(GetAsSBigInt()) == p_right.GetAsULong();
}

bool
SQLVariant::OperUBigEqualsULong(SQLVariant& p_right)
{
  return UBIGINTToULong(GetAsUBigInt()) == p_right.GetAsULong();
}

bool
SQLVariant::OperNumEqualsULong(SQLVariant& p_right)
{
  return UBIGINTToULong(GetAsBCD().AsUInt64()) == p_right.GetAsULong();
}

// FLOAT

bool   
SQLVariant::OperSShortEqualsFloat(SQLVariant& p_right)
{
  return GetAsSShort() == FloatToShort(p_right.GetAsFloat());
}

bool   
SQLVariant::OperUShortEqualsFloat(SQLVariant& p_right)
{
  return GetAsUShort() == FloatToUShort(p_right.GetAsFloat());
}

bool
SQLVariant::OperSLongEqualsFloat(SQLVariant& p_right)
{
  return GetAsSLong() == FloatToLong(p_right.GetAsFloat());
}

bool
SQLVariant::OperULongEqualsFloat(SQLVariant& p_right)
{
  return GetAsULong() == FloatToULong(p_right.GetAsFloat());
}

bool
SQLVariant::OperFloatEqualsFloat(SQLVariant& p_right)
{
  return GetAsFloat() == p_right.GetAsFloat();
}

bool
SQLVariant::OperDoubleEqualsFloat(SQLVariant& p_right)
{
  return DoubleToFloat(GetAsDouble()) == p_right.GetAsFloat();
}

bool
SQLVariant::OperBitEqualsFloat(SQLVariant& p_right)
{
  if((GetAsBit() == 0 && p_right.GetAsFloat() == 0.0) ||
     (GetAsBit() != 0 && p_right.GetAsFloat() != 0.0) )
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperSTinyEqualsFloat(SQLVariant& p_right)
{
  return GetAsSTinyInt() == FloatToTinyInt(p_right.GetAsFloat());
}

bool
SQLVariant::OperUTinyEqualsFloat(SQLVariant& p_right)
{
  return GetAsUTinyInt() == FloatToUTinyInt(p_right.GetAsFloat());
}

bool
SQLVariant::OperSBigEqualsFloat(SQLVariant& p_right)
{
  return GetAsSBigInt() == FloatToBIGINT(p_right.GetAsFloat());
}

bool
SQLVariant::OperUBigEqualsFloat(SQLVariant& p_right)
{
  return GetAsUBigInt() == FloatToUBIGINT(p_right.GetAsFloat());
}

bool
SQLVariant::OperNumEqualsFloat(SQLVariant& p_right)
{
  return GetAsBCD().AsDouble() == (double) p_right.GetAsFloat();
}

// DOUBLE

bool
SQLVariant::OperSShortEqualsDouble(SQLVariant& p_right)
{
  return GetAsSShort() == DoubleToShort(p_right.GetAsDouble());
}

bool
SQLVariant::OperUShortEqualsDouble(SQLVariant& p_right)
{
  return GetAsUShort() == DoubleToUShort(p_right.GetAsDouble());
}

bool
SQLVariant::OperSLongEqualsDouble(SQLVariant& p_right)
{
  return GetAsSLong() == DoubleToLong(p_right.GetAsDouble());
}

bool
SQLVariant::OperULongEqualsDouble(SQLVariant& p_right)
{
  return GetAsULong() == DoubleToULong(p_right.GetAsDouble());
}

bool
SQLVariant::OperFloatEqualsDouble(SQLVariant& p_right)
{
  return GetAsFloat() == DoubleToFloat(p_right.GetAsDouble());
}

bool
SQLVariant::OperDoubleEqualsDouble(SQLVariant& p_right)
{
  return GetAsDouble() == p_right.GetAsDouble();
}

bool
SQLVariant::OperBitEqualsDouble(SQLVariant& p_right)
{
  if((GetAsBit() == 0 && p_right.GetAsDouble() == 0.0) ||
     (GetAsBit() != 0 && p_right.GetAsDouble() != 0.0))
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperSTinyEqualsDouble(SQLVariant& p_right)
{
  return GetAsSTinyInt() == DoubleToTinyInt(p_right.GetAsDouble());
}

bool
SQLVariant::OperUTinyEqualsDouble(SQLVariant& p_right)
{
  return GetAsUTinyInt() == DoubleToUTinyInt(p_right.GetAsDouble());
}

bool
SQLVariant::OperSBigEqualsDouble(SQLVariant& p_right)
{
  return GetAsSBigInt() == DoubleToBIGINT(p_right.GetAsDouble());
}

bool
SQLVariant::OperUBigEqualsDouble(SQLVariant& p_right)
{
  return GetAsUBigInt() == DoubleToUBIGINT(p_right.GetAsDouble());
}

bool
SQLVariant::OperNumEqualsDouble(SQLVariant& p_right)
{
  return GetAsBCD().AsDouble() == p_right.GetAsDouble();
}

// BIT

bool
SQLVariant::OperVarEqualsBit(SQLVariant& p_right)
{
  if((GetAsDouble() == 0.0 && p_right.GetAsBit() == 0) ||
     (GetAsDouble() != 0.0 && p_right.GetAsBit() != 0) )
  {
    return true;
  }
  return false;
}

bool   
SQLVariant::OperSShortEqualsBit(SQLVariant& p_right)
{
  if((GetAsSShort() == 0 && p_right.GetAsBit() == 0) ||
     (GetAsSShort() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperUShortEqualsBit(SQLVariant& p_right)
{
  if((GetAsUShort() == 0 && p_right.GetAsBit() == 0) ||
     (GetAsUShort() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool   
SQLVariant::OperSLongEqualsBit(SQLVariant& p_right)
{
  if((GetAsSLong() == 0 && p_right.GetAsBit() == 0) ||
     (GetAsSLong() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperULongEqualsBit(SQLVariant& p_right)
{
  if((GetAsULong() == 0 && p_right.GetAsBit() == 0) ||
     (GetAsULong() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperFloatEqualsBit(SQLVariant& p_right)
{
  if((GetAsFloat() == 0.0 && p_right.GetAsBit() == 0) ||
     (GetAsFloat() != 0.0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperDoubleEqualsBit(SQLVariant& p_right)
{
  if((GetAsDouble() == 0.0 && p_right.GetAsBit() == 0) ||
     (GetAsDouble() != 0.0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperBitEqualsBit(SQLVariant& p_right)
{
  return GetAsBit() == p_right.GetAsBit();
}

bool
SQLVariant::OperSTinyEqualsBit(SQLVariant& p_right)
{
  if((GetAsSTinyInt() == 0 && p_right.GetAsBit() == 0) ||
     (GetAsSTinyInt() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperUTinyEqualsBit(SQLVariant& p_right)
{
  if((GetAsUTinyInt() == 0 && p_right.GetAsBit() == 0) ||
     (GetAsUTinyInt() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperSBigEqualsBit(SQLVariant& p_right)
{
  if((GetAsSBigInt() == 0 && p_right.GetAsBit() == 0) ||
     (GetAsSBigInt() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperUBigEqualsBit(SQLVariant& p_right)
{
  if((GetAsUBigInt() == 0 && p_right.GetAsBit() == 0) ||
     (GetAsUBigInt() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperNumEqualsBit(SQLVariant& p_right)
{
  CString left;
  GetAsString(left);
  int lf = atoi(left);
 
  if((lf == 0 && p_right.GetAsBit() == 0) ||
     (lf != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

// SIGNED TINYINT

bool
SQLVariant::OperSShortEqualsSTiny(SQLVariant& p_right)
{
  return ShortToTinyInt(GetAsSShort()) == p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUShortEqualsSTiny(SQLVariant& p_right)
{
  return UShortToTinyInt(GetAsUShort()) == p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperSLongEqualsSTiny(SQLVariant& p_right)
{
  return SLongToTinyInt(GetAsSLong()) == p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperULongEqualsSTiny(SQLVariant& p_right)
{
  return ULongToTinyInt(GetAsULong()) == p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperFloatEqualsSTiny(SQLVariant& p_right)
{
  return FloatToTinyInt(GetAsFloat()) == p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperDoubleEqualsSTiny(SQLVariant& p_right)
{
  return DoubleToTinyInt(GetAsDouble()) == p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperBitEqualsSTiny(SQLVariant& p_right)
{
  if((GetAsBit() == 0 && p_right.GetAsSTinyInt() == 0) ||
     (GetAsBit() != 0 && p_right.GetAsSTinyInt() != 0))
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperSTinyEqualsSTiny(SQLVariant& p_right)
{
  return GetAsSTinyInt() == p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUTinyEqualsSTiny(SQLVariant& p_right)
{
  return GetAsUTinyInt() == TinyIntToUTinyInt(p_right.GetAsSTinyInt());
}

bool
SQLVariant::OperSBigEqualsSTiny(SQLVariant& p_right)
{
  return SBIGINTToTinyInt(GetAsSBigInt()) == p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperUBigEqualsSTiny(SQLVariant& p_right)
{
  return UBIGINTToTinyInt(GetAsUBigInt()) == p_right.GetAsSTinyInt();
}

bool
SQLVariant::OperNumEqualsSTiny(SQLVariant& p_right)
{
  return SLongToTinyInt(GetAsBCD().AsLong()) == p_right.GetAsSTinyInt();
}

// UNSIGNED TINYINT

bool
SQLVariant::OperSShortEqualsUTiny(SQLVariant& p_right)
{
  return ShortToUTinyInt(GetAsSShort()) == p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUShortEqualsUTiny(SQLVariant& p_right)
{
  return UShortToUTinyInt(GetAsUShort()) == p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperSLongEqualsUTiny(SQLVariant& p_right)
{
  return SLongToUTinyInt(GetAsSLong()) == p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperULongEqualsUTiny(SQLVariant& p_right)
{
  return ULongToUTinyInt(GetAsULong()) == p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperFloatEqualsUTiny(SQLVariant& p_right)
{
  return FloatToUTinyInt(GetAsFloat()) == p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperDoubleEqualsUTiny(SQLVariant& p_right)
{
  return DoubleToUTinyInt(GetAsDouble()) == p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperBitEqualsUTiny(SQLVariant& p_right)
{
  if((GetAsBit() == 0 && p_right.GetAsUTinyInt() == 0) ||
     (GetAsBit() != 0 && p_right.GetAsUTinyInt() != 0))
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperSTinyEqualsUTiny(SQLVariant& p_right)
{
  return TinyIntToUTinyInt(GetAsSTinyInt()) == p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUTinyEqualsUTiny(SQLVariant& p_right)
{
  return GetAsUTinyInt() == p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperSBigEqualsUTiny(SQLVariant& p_right)
{
  return SBIGINTToUTinyInt(GetAsSBigInt()) == p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperUBigEqualsUTiny(SQLVariant& p_right)
{
  return UBIGINTToUTinyInt(GetAsUBigInt()) == p_right.GetAsUTinyInt();
}

bool
SQLVariant::OperNumEqualsUTiny(SQLVariant& p_right)
{
  return SLongToUTinyInt(GetAsBCD().AsLong()) == p_right.GetAsUTinyInt();
}

// SIGNED BIGINT

bool
SQLVariant::OperSShortEqualsSBig(SQLVariant& p_right)
{
  return GetAsSShort() == BIGINTToShort(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperUShortEqualsSBig(SQLVariant& p_right)
{
  return GetAsUShort() == BIGINTToUShort(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperSLongEqualsSBig(SQLVariant& p_right)
{
  return GetAsSLong() == BIGINTToLong(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperULongEqualsSBig(SQLVariant& p_right)
{
  return GetAsULong() == BIGINTToULong(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperFloatEqualsSBig(SQLVariant& p_right)
{
  return GetAsFloat() == BIGINTToFloat(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperDoubleEqualsSBig(SQLVariant& p_right)
{
  return GetAsDouble() == BIGINTToDouble(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperBitEqualsSBig(SQLVariant& p_right)
{
  if((GetAsBit() == 0 && p_right.GetAsSBigInt() == 0) ||
     (GetAsBit() != 0 && p_right.GetAsSBigInt() != 0))
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperSTinyEqualsSBig(SQLVariant& p_right)
{
  return GetAsSTinyInt() == SBIGINTToTinyInt(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperUTinyEqualsSBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() == SBIGINTToUTinyInt(p_right.GetAsSBigInt());
}

bool
SQLVariant::OperSBigEqualsSBig(SQLVariant& p_right)
{
  return GetAsSBigInt() == p_right.GetAsSBigInt();
}

bool
SQLVariant::OperUBigEqualsSBig(SQLVariant& p_right)
{
  return GetAsUBigInt() == SBIGINTToUBIGINT(p_right.GetAsSBigInt());
}

bool   
SQLVariant::OperNumEqualsSBig(SQLVariant& p_right)
{
  return GetAsBCD().AsInt64() == p_right.GetAsSBigInt();
}

// UNSIGNED BIGINT

bool
SQLVariant::OperSShortEqualsUBig(SQLVariant& p_right)
{
  return GetAsSShort() == UBIGINTToShort(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperUShortEqualsUBig(SQLVariant& p_right)
{
  return GetAsUShort() == UBIGINTToUShort(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperSLongEqualsUBig(SQLVariant& p_right)
{
  return GetAsSLong() == UBIGINTToLong(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperULongEqualsUBig(SQLVariant& p_right)
{
  return GetAsULong() == UBIGINTToULong(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperFloatEqualsUBig(SQLVariant& p_right)
{
  return GetAsFloat() == UBIGINTToFloat(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperDoubleEqualsUBig(SQLVariant& p_right)
{
  return GetAsDouble() == UBIGINTToDouble(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperBitEqualsUBig(SQLVariant& p_right)
{
  if((GetAsBit() == 0 && p_right.GetAsUBigInt() == 0) ||
     (GetAsBit() != 0 && p_right.GetAsUBigInt() != 0))
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperSTinyEqualsUBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() == UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperUTinyEqualsUBig(SQLVariant& p_right)
{
  return GetAsUTinyInt() == UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
SQLVariant::OperSBigEqualsUBig(SQLVariant& p_right)
{
  return SBIGINTToUBIGINT(GetAsSBigInt()) == p_right.GetAsUBigInt();
}

bool
SQLVariant::OperUBigEqualsUBig(SQLVariant& p_right)
{
  return GetAsUBigInt() == p_right.GetAsUBigInt();
}

bool
SQLVariant::OperNumEqualsUBig(SQLVariant& p_right)
{
  return p_right.GetAsBCD().AsUInt64() == p_right.GetAsUBigInt();
}

// NUMERIC

bool
SQLVariant::OperSShortEqualsNum(SQLVariant& p_right)
{
  return GetAsSShort() == (short) p_right.GetAsBCD().AsLong();
}

bool
SQLVariant::OperUShortEqualsNum(SQLVariant& p_right)
{
  return GetAsUShort() == (unsigned short) p_right.GetAsBCD().AsLong();
}

bool
SQLVariant::OperSLongEqualsNum(SQLVariant& p_right)
{
  return GetAsSLong() == p_right.GetAsBCD().AsLong();
}

bool
SQLVariant::OperULongEqualsNum(SQLVariant& p_right)
{
  return GetAsULong() == (unsigned long) p_right.GetAsBCD().AsInt64();
}

bool
SQLVariant::OperFloatEqualsNum(SQLVariant& p_right)
{
  return GetAsFloat() == (float)p_right.GetAsBCD().AsDouble();
}

bool
SQLVariant::OperDoubleEqualsNum(SQLVariant& p_right)
{
  return GetAsDouble() == p_right.GetAsBCD().AsDouble();
}

bool
SQLVariant::OperBitEqualsNum(SQLVariant& p_right)
{
  int ri = p_right.GetAsBCD().AsLong();

  if((GetAsBit() == 0 && ri == 0) ||
     (GetAsBit() != 0 && ri != 0) )
  {
    return true;
  }
  return false;
}

bool
SQLVariant::OperSTinyEqualsNum(SQLVariant& p_right)
{
  return GetAsSTinyInt() == SLongToTinyInt(p_right.GetAsBCD().AsLong());
}

bool
SQLVariant::OperUTinyEqualsNum(SQLVariant& p_right)
{
  return GetAsUTinyInt() == SLongToUTinyInt(p_right.GetAsBCD().AsLong());
}

bool
SQLVariant::OperSBigEqualsNum(SQLVariant& p_right)
{
  return GetAsSBigInt() == p_right.GetAsBCD().AsInt64();
}

bool
SQLVariant::OperUBigEqualsNum(SQLVariant& p_right)
{
  return GetAsUBigInt() == p_right.GetAsBCD().AsUInt64();
}

bool
SQLVariant::OperNumEqualsNum(SQLVariant& p_right)
{
  return GetAsBCD() == p_right.GetAsBCD();
}

// TYPE == GUID

bool
SQLVariant::OperGuidEqualsGuid(SQLVariant& p_right)
{
  SQLGUID* left  = GetAsGUID();
  SQLGUID* right = p_right.GetAsGUID();

  return memcmp(left,right,sizeof(SQLGUID)) == 0;
}

// TYPE == BINARY

bool
SQLVariant::OperBinaryEqualsBinary(SQLVariant& p_right)
{
  void* left   = GetAsBinary();
  void* right  = p_right.GetAsBinary();
  // Take longest of two binaries in comparison
  long  length = m_binaryLength > p_right.m_binaryLength ? m_binaryLength : p_right.m_binaryLength;

  return memcmp(left,right,length) == 0;
}

// TYPE == DATE

bool
SQLVariant::OperDateEqualsDate(SQLVariant& p_right)
{
  // Relies on the == operator of SQLDate
  return GetAsSQLDate() == p_right.GetAsSQLDate();
}

bool
SQLVariant::OperStampEqualsDate(SQLVariant& p_right)
{
  SQLDate date = GetAsSQLTimestamp().AsSQLDate();
  return date == p_right.GetAsSQLDate();
}

// TYPE == TIME

bool
SQLVariant::OperTimeEqualsTime(SQLVariant& p_right)
{
  // Relies on the == operator of SQLTime
  return GetAsSQLTime() == p_right.GetAsSQLTime();
}

bool
SQLVariant::OperStampEqualsTime(SQLVariant& p_right)
{
  SQLTime time = GetAsSQLTimestamp().AsSQLTime();
  return time == p_right.GetAsSQLTime();
}

// TYPE == TIMESTAMP

bool
SQLVariant::OperStampEqualsStamp(SQLVariant& p_right)
{
  // Relies on the == operator of SQLTimestamp
  return GetAsSQLTimestamp() == p_right.GetAsSQLTimestamp();
}

// TYPE == INTERVAL_YEAR_MONTH

bool
SQLVariant::OperIntYMEqualsIntYM(SQLVariant& p_right)
{
  // Relies on the calculation of months in SQLInterval
  return GetAsSQLInterval().AsDatabaseDouble() == p_right.GetAsSQLInterval().AsDatabaseDouble();
}

// TYPE == INTERVAL_DAY_SECOND

bool
SQLVariant::OperIntDSEqualsIntDS(SQLVariant& p_right)
{
  // Relies on the calculation of seconds in SQLInterval
  return GetAsSQLInterval().AsDatabaseDouble() == p_right.GetAsSQLInterval().AsDatabaseDouble();
}
