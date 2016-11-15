////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantTrim.h
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

// Internal conversion and trimming
long           ULongToLong(unsigned long p_value);
long           FloatToLong(float p_value);
long           DoubleToLong(double p_value);
long           BIGINTToLong(SQLBIGINT p_value);
long           UBIGINTToLong(SQLUBIGINT p_value);
short          UShortToShort(unsigned short p_value);
short          SLongToShort(long p_value);
short          ULongToShort(unsigned long p_value);
short          FloatToShort(float p_value);
short          DoubleToShort(double p_value);
short          BIGINTToShort(SQLBIGINT p_value);
short          UBIGINTToShort(SQLUBIGINT p_value);
float          DoubleToFloat(double p_value);
float          BIGINTToFloat(SQLBIGINT p_value);
float          UBIGINTToFloat(SQLUBIGINT p_value);
double         BIGINTToDouble(SQLBIGINT p_value);
double         UBIGINTToDouble(SQLUBIGINT p_value);
SQLBIGINT      FloatToBIGINT(float p_value);
SQLBIGINT      DoubleToBIGINT(double p_value);
SQLBIGINT      UBIGINTToBIGINT(SQLUBIGINT p_value);
char           ShortToTinyInt(short p_value);
char           UShortToTinyInt(unsigned short p_value);
char           SLongToTinyInt(long p_value);
char           ULongToTinyInt(long p_value);
char           FloatToTinyInt(float p_value);
char           DoubleToTinyInt(double p_value);
char           UTinyIntToTinyInt(unsigned char p_value);
char           SBIGINTToTinyInt(SQLBIGINT p_value);
char           UBIGINTToTinyInt(SQLUBIGINT p_value);
unsigned long  ShortToULong(short p_value);
unsigned long  LongToULong(long p_value);
unsigned long  FloatToULong(float p_value);
unsigned long  DoubleToULong(double p_value);
unsigned long  TinyIntToULong(char p_value);
unsigned long  BIGINTToULong(SQLBIGINT p_value);
unsigned long  UBIGINTToULong(SQLUBIGINT p_value);
SQLUBIGINT     ShortToUBIGINT(short p_value);
SQLUBIGINT     LongToUBIGINT(long p_value);
SQLUBIGINT     FloatToUBIGINT(float p_value);
SQLUBIGINT     DoubleToUBIGINT(double p_value);
SQLUBIGINT     TinyIntToUBIGINT(char p_value);
SQLUBIGINT     SBIGINTToUBIGINT(SQLBIGINT p_value);
unsigned short ShortToUShort(short p_value);
unsigned short SLongToUShort(long p_value);
unsigned short ULongToUShort(unsigned long p_value);
unsigned short FloatToUShort(float p_value);
unsigned short DoubleToUShort(double p_value);
unsigned short TinyIntToUShort(char p_value);
unsigned short BIGINTToUShort(SQLBIGINT p_value);
unsigned short UBIGINTToUShort(SQLUBIGINT p_value);
unsigned char  ShortToUTinyInt(short p_value);
unsigned char  UShortToUTinyInt(short p_value);
unsigned char  SLongToUTinyInt(long p_value);
unsigned char  ULongToUTinyInt(unsigned long p_value);
unsigned char  FloatToUTinyInt(float p_value);
unsigned char  DoubleToUTinyInt(double p_value);
unsigned char  TinyIntToUTinyInt(char p_value);
unsigned char  SBIGINTToUTinyInt(SQLBIGINT p_value);
unsigned char  UBIGINTToUTinyInt(SQLUBIGINT p_value);

// Throw error as a result of internal trimming
void  ThrowErrorDatatype(int p_getas);
void  ThrowErrorTruncate(int p_from,int p_to);

