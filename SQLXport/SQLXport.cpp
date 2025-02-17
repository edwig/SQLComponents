////////////////////////////////////////////////////////////////////////
//
// File: XPort.h
//
// Copyright (c) 1998-2025 ir. W.E. Huisman
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
#include "stdafx.h"
#include "XPort.h"
#include "XFile.h"
#include "SQLXport.h"
#include "Parameters.h"
#include <GetExePath.h>
#include <SQLTimestamp.h>
#include <SQLInterval.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// The one and only global parameters object
Parameters params;

// Mapping of all errors
errormap all_errors;

//////////////////////////////////////////////////////////////////////////
//
// General formatting routines for output to screen/logfile
//
//////////////////////////////////////////////////////////////////////////

// print string to terminal and logfile
void
xputs(const TCHAR* p_string)
{
  size_t len = _tcslen(p_string);

  if(params.m_logfile)
  {
    fwrite(p_string,1,len,params.m_logfile);
    fflush(params.m_logfile);
  }
  fwrite(p_string,1,len,stdout);
  fflush(stdout);
}

// Formatted print to terminal and logfile
void
xprint(bool p_sql,const TCHAR* p_string)
{
  if(params.m_listOnly && !p_sql)
  {
    xputs(_T("-- "));
  }
  xputs(p_string);
}

// Formatted print to terminal and logfile
void
xprintf(bool p_sql,const TCHAR* p_format,...)
{
  XString buffer;

  va_list vl;
  va_start(vl,p_format);
  buffer.FormatV(p_format,vl);
  va_end(vl);

  if(params.m_listOnly && !p_sql)
  {
    xputs(_T("-- "));
  }
  xputs(buffer);
}

// Print error to terminal and logfile
void
xerror(const TCHAR* p_format,...)
{
  XString buffer;

  va_list vl;
  va_start(vl,p_format);
  buffer.FormatV(p_format,vl);
  va_end(vl);

  // Show error
  xputs(buffer);

  // Take care of error count
  ++params.m_errors;
}

void
xerror(int p_rownum,XString p_string)
{
  XString text;
  errormap::iterator pos = all_errors.find(p_string);

  if(pos == all_errors.end())
  {
    all_errors.insert(std::make_pair(p_string,1));
    // Show total error + row number
    text.Format(_T("\n%s. Row: %d\n"),p_string.GetString(),p_rownum);

    // Show on terminal and in logfile
    xputs(text);
  }
  else
  {
    // Only show the record number
    text.Format(_T("Row number: %d\n"),p_rownum);
    xputs(text);
    // Number of times found this error
    pos->second++;
  }
  // Take care of the global error count
  ++params.m_errors;
}

// Print the error stack and the number of times the error was encountered
// But only if there where more than 1 error, otherwise the error was already
// shown in the "xerror" method
void
print_all_errors()
{
  for(auto& err : all_errors)
  {
    if(err.second > 1)
    {
      XString text;
      text.Format(_T("%s: Total errors: %d\n"),err.first.GetString(),err.second);
    }
  }
}

void
PrintCopyright()
{
  xprintf(false,_T("SQLXport: SQL-Server export/import program for ODBC connections\n"));
  xprintf(false,_T("Copyright (c) ") XPORT_YEAR _T(" ir. W.E. Huisman\n"));
  xprintf(false,_T("Version: ") XPORT_VERSION _T(" Date: ") XPORT_DATE _T("\n"));
  xprintf(false,_T("\n"));
}

void
PrintTotalTime(SQLTimestamp& p_starting,SQLTimestamp& p_stopping)
{
  bool printing = false;
  SQLInterval howlong = p_stopping - p_starting;

  xprintf(false, _T("SQLXport total processing time: "));
  if (howlong.GetDays())
  {
    xprintf(false,_T("%d days "),howlong.GetDays());
    printing = true;
  }
  if(howlong.GetHours() || printing)
  {
    xprintf(false,_T("%d hours "),howlong.GetHours() % 24);
    printing = true;
  }
  if(howlong.GetMinutes() || printing)
  {
    xprintf(false,_T("%d min "),howlong.GetMinutes() % 60);
  }
  xprintf(false,_T("%d seconds\n"),howlong.GetSeconds() % 60);
}

//////////////////////////////////////////////////////////////////////////
//
// THE REAL EXPORT / IMPORT
//
//////////////////////////////////////////////////////////////////////////

void
ExportImport()
{
  // Make logfile now we have a dump file name
  params.MakeLogfile();

  // Print arguments
  params.PrintArguments();

  SQLTimestamp starting = SQLTimestamp::CurrentTimestamp();

  // Main processing of export or import
  try
  {
    if(params.m_direction.CompareNoCase(_T("export")) == 0)
    {
      // Export object on the stack
      XPort xport(params);
      xport.Export();
    }
    else
    {
      // Import object on the stack
      XPort xport(params);
      xport.Import();
    }
  }
  catch(int ifeof)
  {
    xprintf(false,_T("SQLXport: serious error: %d\n"),ifeof);
  }

  // Printing the result
  if(params.m_errors == 0)
  {
    xprintf(false,_T("SQLXport: %sed with no errors\n"),params.m_direction.GetString());
  }
  else
  {
    xprintf(false,_T("SQLXport: %sed with %d error%c\n")
           ,params.m_direction.GetString()
           ,params.m_errors
           ,params.m_errors > 1 ? _T('s') : ' ');
    print_all_errors();
  }

  // How long it took us
  SQLTimestamp stopping = SQLTimestamp::CurrentTimestamp();
  PrintTotalTime(starting,stopping);

  // Make sure we flush the logfile
  params.CloseLogfile();
}

//////////////////////////////////////////////////////////////////////////
//
// MAIN PROGRAM
//
//////////////////////////////////////////////////////////////////////////

int _tmain(int argc, TCHAR* argv[])
{
  // Initialize the SQL components
  InitSQLComponents(LN_ENGLISH);

  // Print who we are
  PrintCopyright();

  // Get from the command line what we need to do
  params.ProcessCommandLine(argc,argv);
      
  // Perform the export or import
  ExportImport();

  // Return the number of errors to the OS.
  // Needs a direct terminate, because can hang in some virus scanners
  if(params.m_noHang)
  {
    TerminateWithoutCleanup(params.m_errors);
  }
  return params.m_errors;
}
