////////////////////////////////////////////////////////////////////////
//
// File: Parameters.h
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
// Version number: See SQLXport.h
//
#pragma once
#include <map>

using namespace std;
using errormap = std::map<XString, int>;

// Data commit per x rows for inserting tables
#define DATA_COMMIT_RANGE_DEFAULT    10000
#define DATA_COMMIT_RANGE_MINIMUM      100
#define DATA_COMMIT_RANGE_MAXIMUM  1000000

class Parameters
{
public:
  Parameters();
 ~Parameters();

  // Print all the arguments
  void PrintArguments();
  // Checking the correctness of all arguments
  void CheckArguments();
  // Print a help page
  void Usage();
  // Processing the options on the command line
  void ProcessCommandLine(int argc,TCHAR* argv[]);
  // Process parameters from a parameter file
  void ReadParameterFile(XString p_filename);
  // Create the logfile
  void MakeLogfile();
  // Close the logfile
  void CloseLogfile();

  // ALL DATA IS PUBLIC ACCESSABLE

  // GLOBAL ARGUMENTS FOR THE IMPORT/EXPORT
  XString m_database;
  XString m_user;
  XString m_password;
  XString m_schema;
  XString m_object;
  XString m_filter;
  XString m_file;
  // Direction as word and as boolean
  XString m_direction;
  bool    m_export     { true  };
  // Import options
  bool    m_listOnly   { false };
  bool    m_debug      { false };
  bool    m_drop       { false };
  bool    m_statistics { false };
  bool    m_recompile  { false };
  bool    m_stripDiacs { false };
  int     m_commitRange{ DATA_COMMIT_RANGE_DEFAULT };
  // Export options
  bool    m_rows       { true  };
  bool    m_constraints{ true  };
  bool    m_source     { true  };
  bool    m_grants     { true  };
  bool    m_createSql  { false };

  // Locking of tables
  bool    m_consistent { false };
  int     m_waitTime   { 999999999 };
  // Error counting and logfile
  int     m_errors     { 0 };
  FILE*   m_logfile    { nullptr };
  // Sometimes left hanging in virus scanner at the end
  bool    m_noHang     { false   };

private:
  void FindDirection();
  void ProcessOneParameter(XString p_parameter);
};

