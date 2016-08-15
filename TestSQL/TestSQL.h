#pragma once

#include "resource.h"

extern CString g_dsn;
extern CString g_user;
extern CString g_password;

extern void CALLBACK LogPrint(void* p_context,const char* p_text);
extern int  CALLBACK LogLevel(void* p_context);

extern void TestConnection();
extern void TestClosingCursor();
extern void TestFormatting();
extern void TestDataSet();
