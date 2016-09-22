// TestSQL.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "TestSQL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning (disable: 4477)

// CString g_dsn("owoc09");
// CString g_user("k2b");
// CString g_password("k2b");

// MS-Access
// CString g_dsn("testing");
// CString g_user("sa");
// CString g_password("word");
// 

CString g_dsn("ok2b06");
CString g_user("k2b");
CString g_password("k2b");

void CALLBACK LogPrint(void* p_context,const char* p_text)
{
  const char *context = (const char*) p_context;
  printf(context);
  printf(p_text);
}

int CALLBACK LogLevel(void* p_context)
{
  return 6; // = LOGLEVEL_MAX from SQLDatabase.h
}

// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
  int nRetCode = 0;

  if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
  {
    _tprintf(_T("Fatal Error: MFC initialization failed\n"));
    nRetCode = 1;
  }
  else
  {
    // Test connection first
    TestConnection();
    
    TestClosingCursor();

    TestNULL();

    // Test formatting of Date/Time/Timestamp
    TestFormatting();

    // Test dataset
    TestDataSet();
  }

  //////////////////////////////////////////////////////////////////////////
  // 
  // READY
  //
  //////////////////////////////////////////////////////////////////////////

  printf("Give your opinion: ");
  fflush(stdout);
  char buffer[101] = "";
  gets_s(buffer,100);

  return nRetCode;
}
