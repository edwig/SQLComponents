////////////////////////////////////////////////////////////////////////
//
// File: TestSQL.h
//
// Created: 1998-2025 ir. W.E. Huisman
// MIT License
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
// Version number: See SQLComponents.h
//
#pragma once
#include "resource.h"
#include <time.h>

using namespace SQLComponents;

extern XString g_dsn;
extern XString g_user;
extern XString g_password;
extern XString g_schema;

extern void CALLBACK LogPrint(void* p_context,LPCTSTR p_text);
extern int  CALLBACK LogLevel(void* p_context);

extern void TestConnection();
extern void TestSelections();
extern void TestNVarchar();
extern void TestClosingCursor();
extern void TestFormatting();
extern void TestDataSet();
extern void TestNULL();
extern void TestOperators();
extern void TestNumeric();
extern void TestBcd();
extern void TestIcd();
extern void TestBulkICD();
extern void TestBulkDecimals(bool p_trunc);
extern void TestCalling();
extern void TestOracleOID();
extern void TestBCDIndividual();
