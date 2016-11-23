////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorEqual.h
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

// BEWARE
// THE CONTENTS OF THIS FILE IS PART (INCLUDED) OF THE DEFINITION OF SQLVariant
// This is the part of the class that takes care of the == operator


   // Arrays with comparison functions for the operators
   static CompareFunctionArray OperatorEqual[CT_LAST][CT_LAST];

   // Comparison functions for the EQUAL operator
   bool   OperVarEqualsChar     (SQLVariant& p_right);  // TYPE == CHAR
   bool   OperBitEqualsChar     (SQLVariant& p_right);
   bool   OperGuidEqualsChar    (SQLVariant& p_right);
   bool   OperSShortEqualsSShort(SQLVariant& p_right);  // TYPE == SSHORT
   bool   OperUShortEqualsSShort(SQLVariant& p_right);
   bool   OperSLongEqualsSShort (SQLVariant& p_right);
   bool   OperULongEqualsSShort (SQLVariant& p_right);
   bool   OperFloatEqualsSShort (SQLVariant& p_right);
   bool   OperDoubleEqualsSShort(SQLVariant& p_right);
   bool   OperBitEqualsSShort   (SQLVariant& p_right);
   bool   OperSTinyEqualsSShort (SQLVariant& p_right);
   bool   OperUTinyEqualsSShort (SQLVariant& p_right);
   bool   OperSBigEqualsSShort  (SQLVariant& p_right);
   bool   OperUBigEqualsSShort  (SQLVariant& p_right);
   bool   OperSShortEqualsUShort(SQLVariant& p_right);  // TYPE == USHORT
   bool   OperUShortEqualsUShort(SQLVariant& p_right);
   bool   OperSLongEqualsUShort (SQLVariant& p_right);
   bool   OperUlongEqualsUShort (SQLVariant& p_right);
   bool   OperFloatEqualsUShort (SQLVariant& p_right);
   bool   OperDoubleEqualsUShort(SQLVariant& p_right);
   bool   OperBitEqualsUShort   (SQLVariant& p_right);
   bool   OperSTinyEqualsUShort (SQLVariant& p_right);
   bool   OperUTinyEqualsUShort (SQLVariant& p_right);
   bool   OperSBigEqualsUShort  (SQLVariant& p_right);
   bool   OperUBigEqualsUShort  (SQLVariant& p_right);
   bool   OperSShortEqualsSLong (SQLVariant& p_right);  // TYPE == SLONG
   bool   OperUShortEqualsSLong (SQLVariant& p_right);
   bool   OperSLongEqualsSLong  (SQLVariant& p_right);
   bool   OperULongEqualsSLong  (SQLVariant& p_right);
   bool   OperFloatEqualsSLong  (SQLVariant& p_right);
   bool   OperDoubleEequalsSLong(SQLVariant& p_right);
   bool   OperBitEqualsSLong    (SQLVariant& p_right);
   bool   OperSTinyEqualsSLong  (SQLVariant& p_right);
   bool   OperUTinyEqualsSLong  (SQLVariant& p_right);
   bool   OperSBigEqualsSLong   (SQLVariant& p_right);
   bool   OperUBigEqualsSLong   (SQLVariant& p_right);
   bool   OperSShortEqualsULong (SQLVariant& p_right);  // TYPE == ULONG
   bool   OperUShortEqualsULong (SQLVariant& p_right);
   bool   OperSLongEqualsULong  (SQLVariant& p_right);
   bool   OperULongEqualsULong  (SQLVariant& p_right);
   bool   OperFloatEqualsULong  (SQLVariant& p_right);
   bool   OperDoubleEqualsULong (SQLVariant& p_right);
   bool   OperBitEqualsULong    (SQLVariant& p_right);
   bool   OperSTinyEqualsULong  (SQLVariant& p_right);
   bool   OperUTinyEqualsULong  (SQLVariant& p_right);
   bool   OperSBigEqualsULong   (SQLVariant& p_right);
   bool   OperUBigEqualsULong   (SQLVariant& p_right);
   bool   OperSShortEqualsFloat (SQLVariant& p_right);  // TYPE == FLOAT
   bool   OperUShortEqualsFloat (SQLVariant& p_right);
   bool   OperSLongEqualsFloat  (SQLVariant& p_right);
   bool   OperULongEqualsFloat  (SQLVariant& p_right);
   bool   OperFloatEqualsFloat  (SQLVariant& p_right);
   bool   OperDoubleEqualsFloat (SQLVariant& p_right);
   bool   OperBitEqualsFloat    (SQLVariant& p_right);
   bool   OperSTinyEqualsFloat  (SQLVariant& p_right);
   bool   OperUTinyEqualsFloat  (SQLVariant& p_right);
   bool   OperSBigEqualsFloat   (SQLVariant& p_right);
   bool   OperUBigEqualsFloat   (SQLVariant& p_right);
   bool   OperNumEqualsFloat    (SQLVariant& p_right);
   bool   OperSShortEqualsDouble(SQLVariant& p_right);  // TYPE == DOUBLE
   bool   OperUShortEqualsDouble(SQLVariant& p_right);
   bool   OperSLongEqualsDouble (SQLVariant& p_right);
   bool   OperULongEqualsDouble (SQLVariant& p_right);
   bool   OperFloatEqualsDouble (SQLVariant& p_right);
   bool   OperDoubleEqualsDouble(SQLVariant& p_right);
   bool   OperBitEqualsDouble   (SQLVariant& p_right);
   bool   OperSTinyEqualsDouble (SQLVariant& p_right);
   bool   OperUTinyEqualsDouble (SQLVariant& p_right);
   bool   OperSBigEqualsDouble  (SQLVariant& p_right);
   bool   OperUBigEqualsDouble  (SQLVariant& p_right);
   bool   OperNumEqualsDouble   (SQLVariant& p_right);
   bool   OperVarEqualsBit      (SQLVariant& p_right);  // TYPE == BIT
   bool   OperSShortEqualsBit   (SQLVariant& p_right);
   bool   OperUShortEqualsBit   (SQLVariant& p_right);
   bool   OperSLongEqualsBit    (SQLVariant& p_right);
   bool   OperULongEqualsBit    (SQLVariant& p_right);
   bool   OperFloatEqualsBit    (SQLVariant& p_right);
   bool   OperDoubleEqualsBit   (SQLVariant& p_right);
   bool   OperBitEqualsBit      (SQLVariant& p_right);
   bool   OperSTinyEqualsBit    (SQLVariant& p_right);
   bool   OperUTinyEqualsBit    (SQLVariant& p_right);
   bool   OperSBigEqualsBit     (SQLVariant& p_right);
   bool   OperUBigEqualsBit     (SQLVariant& p_right);
   bool   OperNumEqualsBit      (SQLVariant& p_right);
   bool   OperSShortEqualsSTiny (SQLVariant& p_right);  // TYPE == STINYINT
   bool   OperUShortEqualsSTiny (SQLVariant& p_right);
   bool   OperSLongEqualsSTiny  (SQLVariant& p_right);
   bool   OperULongEqualsSTiny  (SQLVariant& p_right);
   bool   OperFloatEqualsSTiny  (SQLVariant& p_right);
   bool   OperDoubleEqualsSTiny (SQLVariant& p_right);
   bool   OperBitEqualsSTiny    (SQLVariant& p_right);
   bool   OperSTinyEqualsSTiny  (SQLVariant& p_right);
   bool   OperUTinyEqualsSTiny  (SQLVariant& p_right);
   bool   OperSBigEqualsSTiny   (SQLVariant& p_right);
   bool   OperUBigEqualsSTiny   (SQLVariant& p_right);
   bool   OperNumEqualsSTiny    (SQLVariant& p_right);
   bool   OperSShortEqualsUTiny (SQLVariant& p_right);  // TYPE == UTINYINT
   bool   OperUShortEqualsUTiny (SQLVariant& p_right);
   bool   OperSLongEqualsUTiny  (SQLVariant& p_right);
   bool   OperULongEqualsUTiny  (SQLVariant& p_right);
   bool   OperFloatEqualsUTiny  (SQLVariant& p_right);
   bool   OperDoubleEqualsUTiny (SQLVariant& p_right);
   bool   OperBitEqualsUTiny    (SQLVariant& p_right);
   bool   OperSTinyEqualsUTiny  (SQLVariant& p_right);
   bool   OperUTinyEqualsUTiny  (SQLVariant& p_right);
   bool   OperSBigEqualsUTiny   (SQLVariant& p_right);
   bool   OperUBigEqualsUTiny   (SQLVariant& p_right);
   bool   OperNumEqualsUTiny    (SQLVariant& p_right);
   bool   OperSShortEqualsSBig  (SQLVariant& p_right);  // TYPE == SBIGINT
   bool   OperUShortEqualsSBig  (SQLVariant& p_right);
   bool   OperSLongEqualsSBig   (SQLVariant& p_right);
   bool   OperULongEqualsSBig   (SQLVariant& p_right);
   bool   OperFloatEqualsSBig   (SQLVariant& p_right);
   bool   OperDoubleEqualsSBig  (SQLVariant& p_right);
   bool   OperBitEqualsSBig     (SQLVariant& p_right);
   bool   OperSTinyEqualsSBig   (SQLVariant& p_right);
   bool   OperUTinyEqualsSBig   (SQLVariant& p_right);
   bool   OperSBigEqualsSBig    (SQLVariant& p_right);
   bool   OperUBigEqualsSBig    (SQLVariant& p_right);
   bool   OperNumEqualsSBig     (SQLVariant& p_right);
   bool   OperSShortEqualsUBig  (SQLVariant& p_right);  // TYPE == UBIGINT
   bool   OperUShortEqualsUBig  (SQLVariant& p_right);
   bool   OperSLongEqualsUBig   (SQLVariant& p_right);
   bool   OperULongEqualsUBig   (SQLVariant& p_right);
   bool   OperFloatEqualsUBig   (SQLVariant& p_right);
   bool   OperDoubleEqualsUBig  (SQLVariant& p_right);
   bool   OperBitEqualsUBig     (SQLVariant& p_right);
   bool   OperSTinyEqualsUBig   (SQLVariant& p_right);
   bool   OperUTinyEqualsUBig   (SQLVariant& p_right);
   bool   OperSBigEqualsUBig    (SQLVariant& p_right);
   bool   OperUBigEqualsUBig    (SQLVariant& p_right);
   bool   OperNumEqualsUBig     (SQLVariant& p_right);
   bool   OperSShortEqualsNum   (SQLVariant& p_right);  // TYPE == NUMERIC
   bool   OperUShortEqualsNum   (SQLVariant& p_right);
   bool   OperSLongEqualsNum    (SQLVariant& p_right);
   bool   OperULongEqualsNum    (SQLVariant& p_right);
   bool   OperFloatEqualsNum    (SQLVariant& p_right);
   bool   OperDoubleEqualsNum   (SQLVariant& p_right);
   bool   OperBitEqualsNum      (SQLVariant& p_right);
   bool   OperSTinyEqualsNum    (SQLVariant& p_right);
   bool   OperUTinyEqualsNum    (SQLVariant& p_right);
   bool   OperSBigEqualsNum     (SQLVariant& p_right);
   bool   OperUBigEqualsNum     (SQLVariant& p_right);
   bool   OperNumEqualsNum      (SQLVariant& p_right);
   bool   OperGuidEqualsGuid    (SQLVariant& p_right);  // TYPE == GUID
   bool   OperBinaryEqualsBinary(SQLVariant& p_right);  // TYPE == BINARY
   bool   OperDateEqualsDate    (SQLVariant& p_right);  // TYPE == DATE
   bool   OperStampEqualsDate   (SQLVariant& p_right);
   bool   OperTimeEqualsTime    (SQLVariant& p_right);  // TYPE == TIME
   bool   OperStampEqualsTime   (SQLVariant& p_right);
   bool   OperStampEqualsStamp  (SQLVariant& p_right);  // TYPE == TIMESTAMP
   bool   OperIntYMEqualsIntYM  (SQLVariant& p_right);  // TYPE == INTERVAL_YEAR_MONTH
   bool   OperIntDSEqualsIntDS  (SQLVariant& p_right);  // TYPE == INTERVAL_DAY_SECOND


