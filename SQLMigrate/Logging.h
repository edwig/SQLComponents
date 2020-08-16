////////////////////////////////////////////////////////////////////////
//
// File: Logging.h
//
// Copyright (c) 1998-2020 ir. W.E. Huisman
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
// Version number: See SQLComponents.h
//
#pragma once

#define FILENAME_LOGFILE    "logfile.txt"
#define FILENAME_OUTPUT     "script.sql"
#define FILENAME_DROPFILE   "dropscript.sql"

//class applicatieDlg;

typedef enum log
{
  nothing = 0,
  create,
  drop,
}
LogType;

class Logging
{
public:
  Logging();
 ~Logging();
  int  Open();
  void Close();
  void WriteLog (CString boodschap);
  void WriteOut (CString statement,bool p_delim = false);
  void WriteDrop(CString statement,bool p_delim = false);
  void SetLogStatus(LogType status);
  void SetTables(int num);
  void SetTableGauge(int num,int maxnum);
  void SetTablesGauge(int num);
  void SetStatus(CString status);
  void SetScript(CString p_script);
  void SetDropScript(CString p_script);
  void SetLogfile(CString p_logfile);
  void SetDBType(bool p_source, CString p_type);
private:
  int     m_tables;
  LogType m_status;

  CString m_logfile;
  CString m_script;
  CString m_dropscript;

  FILE* m_flog;
  FILE* m_fout;
  FILE* m_fdrop;
};
