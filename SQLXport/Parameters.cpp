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
#include "stdafx.h"
#include "parameters.h"
#include "SQLXport.h"
#include <GetExePath.h>
#include <WinFile.h>

Parameters::Parameters()
{
}

Parameters::~Parameters()
{
}

void
Parameters::PrintArguments()
{
  xprintf(false,_T("Operation of this run of SQLXport\n"));
  xprintf(false,_T("---------------------------------\n"));
  xprintf(false,_T("Direction of operation  : %s\n"),m_direction.GetString());
  xprintf(false,_T("Database connected to   : %s\n"),m_database.GetString());
  xprintf(false,_T("Logged in as user       : %s\n"),m_user.GetString());
  xprintf(false,_T("Schema to act upon      : %s\n"),m_schema.GetString());
  xprintf(false,_T("Object to export/import : %s\n"),m_object.IsEmpty() ? _T("*") : m_object.GetString());
  xprintf(false,_T("File for export dump    : %s\n"),m_file.GetString());
  if(m_export)
  {
    xprintf(false,_T("Extra filter for data   : %s\n"),m_filter.GetString());
    xprintf(false,_T("Export table data rows  : %s\n"),m_rows        ? _T("true") : _T("false"));
    xprintf(false,_T("Export table constraints: %s\n"),m_constraints ? _T("true") : _T("false"));
    xprintf(false,_T("Export rights           : %s\n"),m_grants      ? _T("true") : _T("false"));
    xprintf(false,_T("Export source code      : %s\n"),m_source      ? _T("true") : _T("false"));
    xprintf(false,_T("Use consistent export   : %s\n"),m_consistent  ? _T("true") : _T("false"));
    xprintf(false,_T("Use lock wait time      : %d\n"),m_waitTime);
  }
  else
  {
    xprintf(false,_T("Drop schema contents    : %s\n"),m_drop        ? _T("true") : _T("false"));
    xprintf(false,_T("Do statistics on import : %s\n"),m_statistics  ? _T("true") : _T("false"));
    xprintf(false,_T("Do recompile  on import : %s\n"),m_recompile   ? _T("true") : _T("false"));
    xprintf(false,_T("Listing SQL only        : %s\n"),m_listOnly    ? _T("true") : _T("false"));
    xprintf(false,_T("Strip diacritics        : %s\n"),m_stripDiacs  ? _T("true") : _T("false"));
  }
  xprintf(false,_T("Debugging row data      : %s\n"),m_debug       ? _T("true") : _T("false"));
  xprintf(false,_T("\n"));
}

void
Parameters::CheckArguments()
{
  m_direction = m_direction.Trim();
  m_database  = m_database.Trim();
  m_user      = m_user.Trim();
  m_password  = m_password.Trim();
  m_schema    = m_schema.Trim();
  m_file      = m_file.Trim();
  m_object    = m_object.Trim();
  m_filter    = m_filter.Trim();

  if(m_direction.IsEmpty())
  {
    xerror(_T("SQLXport: Must specify a import/export direction with /DIR:\n"));
  }
  if(m_direction.CompareNoCase(_T("export")) && m_direction.CompareNoCase(_T("import")))
  {
    xerror(_T("SQLXport: you must either specify \"import\" or \"export\" as a working direction\n"));
  }
  // Really do an import
  if(m_direction.CompareNoCase(_T("import")) == 0)
  {
    m_export = false;
  }
  if(m_database.IsEmpty() && m_listOnly == false)
  {
    xerror(_T("SQLXport: No database name given with /DBASE:\n"));
  }
  if(m_user.IsEmpty() && m_listOnly == false)
  {
    xerror(_T("SQLXport: No database user given with /USER:\n"));
  }
  if(m_password.IsEmpty() && m_listOnly == false)
  {
    xerror(_T("SQLXPort: No login password given with /PASSWD:\n"));
  }
  if(m_export && m_schema.IsEmpty() && m_listOnly == false)
  {
    xerror(_T("SQLXport: Need a schema name upon exporting a database\n"));
  }
  if(m_export && m_statistics)
  {
    xerror(_T("SQLXport: Can only perform statistics upon import\n"));
  }
  if(m_export && m_recompile)
  {
    xerror(_T("SQLXport: Can only perform full recompile upon export\n"));
  }
  if(m_file.IsEmpty())
  {
    m_file = _T("ExportDump.dmp");
    xprintf(false,_T("SQLXport: WARNING: Using default dump file name: %s\n"),m_file.GetString());
  }
  if(m_export && m_user.CompareNoCase(m_schema) != 0 )
  {
    xerror(_T("SQLXport: WARNING: not logged in, in the schema to export/import.\n"));
    xerror(_T("Correct functioning of this program is **NOT** garantueed.\n"));
    xerror(_T("You have been warned!!\n\n"));
  }
  if(!m_export)
  {
    if(m_commitRange < DATA_COMMIT_RANGE_MINIMUM || m_commitRange > DATA_COMMIT_RANGE_MAXIMUM)
    {
      xerror(_T("SQLXport: Data commit out of range. Must be between %d and %d\n")
            ,DATA_COMMIT_RANGE_MINIMUM,DATA_COMMIT_RANGE_MAXIMUM);
    }
  }

  if(m_export)
  {
    if(m_drop)
    {
      xerror(_T("SQLXport can only drop the schema content on import\n"));
    }
    if(m_statistics)
    {
      xerror(_T("SQLXport can only recompute statistics on import\n"));
    }
    if(m_recompile)
    {
      xerror(_T("SQLXport can only do a full recompile on import\n"));
    }
    if(m_commitRange != DATA_COMMIT_RANGE_DEFAULT)
    {
      xerror(_T("SQLXport can only set commit range on import\n"));
    }
  }
  else // m_export == false
  {
    if(m_rows == false)
    {
      xerror(_T("SQLXport can only use /NOROWS upon export mode\n"));
    }
    if(m_constraints == false)
    {
      xerror(_T("OraXport can only use /NOCONSTRAINTS upon export mode\n"));
    }
    if(m_source == false)
    {
      xerror(_T("SQLXport can only use /NOSOURCE upon export mode\n"));
    }
    if(m_grants == false)
    {
      xerror(_T("SQLXport can only use /NOGRANTS upon export mode\n"));
    }
    if(m_createSql)
    {
      xerror(_T("SQLXport can only use /CREATESQL upon export mode\n"));
    }
  }

  // Do error
  if(m_errors)
  {
    xputs(_T("\n"));
    Usage();

    xputs(_T("\n"));
    xputs(_T("Program aborted\n"));
    exit(2);
  }
}

void
Parameters::Usage()
{
  xprintf(false, _T("Usage:\n"));
  xprintf(false, _T("SQLXport /DIR:{export | import} options\n"));
  xprintf(false, _T("The following are valid options:\n"));
  xprintf(false, _T("\n"));
  xprintf(false, _T("/DBASE:<database>    Name of the database to export from/import to\n"));
  xprintf(false, _T("/USER:<username>     Name of the user logging in to the database\n"));
  xprintf(false, _T("/PASSWD:<password>   Password of the user logging in\n"));
  xprintf(false, _T("/FILE:<filename>     Name of the file the export goes into/import comes from\n"));
  xprintf(false, _T("/SCHEMA:<schemaname> Name of the schema that's been exported (export only)\n"));
  xprintf(false, _T("/OBJECT:<objectname> Name of the object to be exported or imported\n"));
  xprintf(false, _T("/FILTER:<filter>     Extra filter to select data rows (export only)\n"));
  xprintf(false, _T("/PARAMS:<filename>   File with these parameters\n"));
  xprintf(false, _T("/DROP                Drop schema contents (import only)\n"));
  xprintf(false, _T("/CONSISTENT          Use share table lock (export only)\n"));
  xprintf(false, _T("/WAIT:<seconds>      Time to wait for a share table lock (export only)\n"));
  xprintf(false, _T("/NOROWS              Do not export the tables data rows\n"));
  xprintf(false, _T("/NOCONSTRAINTS       Do not export indices/primary/foreign/check constraints\n"));
  xprintf(false, _T("/NOSOURCE            Do not export functions/procedures/types/packages/triggers\n"));
  xprintf(false, _T("/NOGRANTS            Do not export the object's access rights\n"));
  xprintf(false, _T("/COMMIT:<nnn>        Do commit after <nnn> records (import only)\n"));
  xprintf(false, _T("/STATISTICS          Start full statistics after import\n"));
  xprintf(false, _T("/RECOMPILE           Start full recompile after import\n"));
  xprintf(false, _T("/LIST                Only list resulting SQL. (import only)\n"));
  xprintf(false, _T("/STRIPDIAC           Strip European diacritics (import only)\n"));
  xprintf(false, _T("/CREATESQL           Create scripts (export only)\n"));
  xprintf(false, _T("/DEBUG               Debug reading/writing of row data\n"));
  xprintf(false, _T("/NOHANG              Stop hanging in virus scanner at the end\n"));
  xprintf(false, _T("/H(ELP) or /?        This help page\n"));
  xprintf(false, _T("\n"));
}

// Read all parameters from a parameter file
void
Parameters::ReadParameterFile(XString p_filename)
{
  WinFile file(p_filename);
  m_readParams = true;

  if(file.Open(winfile_read | open_trans_text))
  {
    XString line;
    while(file.Read(line))
    {
      line.Trim(_T("\n"));
      if(line.GetAt(0) != '#')
      {
        ProcessOneParameter(line);
      }
    }
    file.Close();
  }
  else
  {
    xprintf(false,_T("ERROR: [%d] Cannot open parameter file: %s\n"),(int)file.GetLastError(),p_filename.GetString());
    TerminateWithoutCleanup(3);
  }
  m_readParams = false;
}

// Processing the options on the command line
void
Parameters::ProcessCommandLine(int argc,TCHAR* argv[])
{
  // Find the direction from the executable name
  FindDirection();

  // Find the options from the command line
  // /DIR: overrides the executable name
  for (int i=1; i < argc; i++)
  {
    LPCTSTR param = argv[i];

    if (param[0] == _T('-') || param[0] == '/')
    {
      if(_totlower(param[1]) ==_T('h') || param[1] == '?')
      {
        Usage();
        TerminateWithoutCleanup(3);
      }
      ProcessOneParameter(&param[1]);
    }
  }

  // Check the arguments for correctness
  // Does NOT return here on error
  CheckArguments();
}

void
Parameters::MakeLogfile()
{
  XString filename = m_file;

  // Make a nice logging filename
  // <basename>_<direction>.txt
  int pos = filename.ReverseFind('.');
  if(pos > 0)
  {
    filename = filename.Left(pos);
  }
  filename += _T("_");
  filename += m_direction;
  filename += _T(".txt");

  // Open in write mode
  m_logfile.SetFilename(filename);
  if(m_logfile.Open(winfile_write | open_trans_text | open_shared_read
                   ,FAttributes::attrib_none,Encoding::UTF8) == false)
  {
    _tprintf(_T("WARNING: Could not open logfile: %s\n"),(LPCTSTR) filename);
  }
}

// Close the logfile
void 
Parameters::CloseLogfile()
{
  if(m_logfile.GetIsOpen())
  {
    if(m_logfile.Close(true) == false)
    {
      _tprintf(_T("WARNING: Logfile not correctly flushed to disk.\n"));
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

// See if the executable is renamed "import" or "export"
// So we do not have to specify the direction
void
Parameters::FindDirection()
{
  XString name = GetExeFile();
  if(name.CompareNoCase(_T("export")) == 0 ||
     name.CompareNoCase(_T("import")) == 0)
  {
    m_direction = name;
  }
}

// STILL TO BE DONE
// /NOVIEWS
// /NOSEQUENCES
// /NOSYNONYMS

void
Parameters::ProcessOneParameter(XString p_parameter)
{
       if(p_parameter.Left(4) .CompareNoCase(_T("dir:"))          == 0) m_direction    = p_parameter.Mid(4);
  else if(p_parameter.Left(6) .CompareNoCase(_T("dbase:"))        == 0) m_database     = p_parameter.Mid(6);
  else if(p_parameter.Left(5) .CompareNoCase(_T("user:"))         == 0) m_user         = p_parameter.Mid(5);
  else if(p_parameter.Left(7) .CompareNoCase(_T("passwd:"))       == 0) m_password     = p_parameter.Mid(7);
  else if(p_parameter.Left(7) .CompareNoCase(_T("schema:"))       == 0) m_schema       = p_parameter.Mid(7);
  else if(p_parameter.Left(5) .CompareNoCase(_T("file:"))         == 0) m_file         = p_parameter.Mid(5);
  else if(p_parameter.Left(7) .CompareNoCase(_T("object:"))       == 0) m_object       = p_parameter.Mid(7);
  else if(p_parameter.Left(7) .CompareNoCase(_T("filter:"))       == 0) m_filter       = p_parameter.Mid(7);
  else if(p_parameter.Left(5) .CompareNoCase(_T("wait:"))         == 0) m_waitTime     = _ttoi(p_parameter.Mid(5));
  else if(p_parameter.Left(7) .CompareNoCase(_T("commit:"))       == 0) m_commitRange  = _ttoi(p_parameter.Mid(7));
  else if(p_parameter.Left(10).CompareNoCase(_T("consistent"))    == 0) m_consistent   = true;
  else if(p_parameter.Left(4) .CompareNoCase(_T("drop"))          == 0) m_drop         = true;
  else if(p_parameter.Left(4) .CompareNoCase(_T("list"))          == 0) m_listOnly     = true;
  else if(p_parameter.Left(5) .CompareNoCase(_T("debug"))         == 0) m_debug        = true;
  else if(p_parameter.Left(10).CompareNoCase(_T("statistics"))    == 0) m_statistics   = true;
  else if(p_parameter.Left(9) .CompareNoCase(_T("recompile"))     == 0) m_recompile    = true;
  else if(p_parameter.Left(8) .CompareNoCase(_T("nogrants"))      == 0) m_grants       = false;
  else if(p_parameter.Left(6) .CompareNoCase(_T("norows"))        == 0) m_rows         = false;
  else if(p_parameter.Left(13).CompareNoCase(_T("noconstraints")) == 0) m_constraints  = false;
  else if(p_parameter.Left(8) .CompareNoCase(_T("nosource"))      == 0) m_source       = false;
  else if(p_parameter.Left(9) .CompareNoCase(_T("createsql"))     == 0) m_createSql    = true;
  else if(p_parameter.Left(6) .CompareNoCase(_T("nohang"))        == 0) m_noHang       = true;
  else if(p_parameter.Left(9) .CompareNoCase(_T("stripdiac"))     == 0) m_stripDiacs   = true;
  else if(p_parameter.Left(6) .CompareNoCase(_T("params"))        == 0)
  {
    if(!m_readParams)
    {
      XString filename = p_parameter.Mid(7);
      ReadParameterFile(filename);
    }
  }
  else
  {
    // No known parameter
    xprintf(false,_T("ERROR: Unknown startup parameter: %s\n"),p_parameter.GetString());
    TerminateWithoutCleanup(3);
  }
}
