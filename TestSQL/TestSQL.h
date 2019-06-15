////////////////////////////////////////////////////////////////////////
//
// File: TestSQL.h
//
// Copyright (c) 1998-2018 ir. W.E. Huisman
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
// Last Revision:  15-06-2019
// Version number: 1.5.5
//
#pragma once
#include "resource.h"
#include <time.h>

using namespace SQLComponents;

extern CString g_dsn;
extern CString g_user;
extern CString g_password;
extern CString g_schema;

extern void CALLBACK LogPrint(void* p_context,const char* p_text);
extern int  CALLBACK LogLevel(void* p_context);

extern void TestConnection();
extern void TestClosingCursor();
extern void TestFormatting();
extern void TestDataSet();
extern void TestNULL();
extern void TestOperators();
extern void TestNumeric();
extern void TestBcd();
extern void TestCalling();
