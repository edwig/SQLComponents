////////////////////////////////////////////////////////////////////////
//
// File: Parameters.h
//
// Copyright (c) 1998-2023 ir. W.E. Huisman
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

Parameters::Parameters()
{
}

Parameters::~Parameters()
{
}

void
Parameters::PrintArguments()
{
  xprintf(false,"Operation of this run of SQLXport\n");
  xprintf(false,"---------------------------------\n");
  xprintf(false,"Direction of operation  : %s\n",m_direction.GetString());
  xprintf(false,"Database connected to   : %s\n",m_database.GetString());
  xprintf(false,"Logged in as user       : %s\n",m_user.GetString());
  xprintf(false,"Schema to act upon      : %s\n",m_schema.GetString());
  xprintf(false,"Object to export/import : %s\n",m_object.IsEmpty() ? "*" : m_object.GetString());
  xprintf(false,"File for export dump    : %s\n",m_file.GetString());
  if(m_export)
  {
    xprintf(false,"Extra filter for data   : %s\n",m_filter.GetString());
    xprintf(false,"Export table data rows  : %s\n",m_rows        ? "true" : "false");
    xprintf(false,"Export table constraints: %s\n",m_constraints ? "true" : "false");
    xprintf(false,"Export rights           : %s\n",m_grants      ? "true" : "false");
    xprintf(false,"Export source code      : %s\n",m_source      ? "true" : "false");
    xprintf(false,"Use consistent export   : %s\n",m_consistent  ? "true" : "false");
    xprintf(false,"Use lock wait time      : %d\n",m_waitTime);
  }
  else
  {
    xprintf(false,"Drop schema contents    : %s\n",m_drop        ? "true" : "false");
    xprintf(false,"Do statistics on import : %s\n",m_statistics  ? "true" : "false");
    xprintf(false,"Do recompile  on import : %s\n",m_recompile   ? "true" : "false");
    xprintf(false,"Listing SQL only        : %s\n",m_listOnly    ? "true" : "false");
    xprintf(false,"Strip diacritics        : %s\n",m_stripDiacs  ? "true" : "false");
  }
  xprintf(false,"Debugging row data      : %s\n",m_debug       ? "true" : "false");
  xprintf(false,"\n");
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
    xerror("SQLXport: Must specify a import/export direction with /DIR:\n");
  }
  if(m_direction.CompareNoCase("export") && m_direction.CompareNoCase("import"))
  {
    xerror("SQLXport: you must either specify \"import\" or \"export\" as a working direction\n");
  }
  // Really do an import
  if(m_direction.CompareNoCase("import") == 0)
  {
    m_export = false;
  }
  if(m_database.IsEmpty() && m_listOnly == false)
  {
    xerror("SQLXport: No database name given with /DBASE:\n");
  }
  if(m_user.IsEmpty() && m_listOnly == false)
  {
    xerror("SQLXport: No database user given with /USER:\n");
  }
  if(m_password.IsEmpty() && m_listOnly == false)
  {
    xerror("SQLXPort: No login password given with /PASSWD:\n");
  }
  if(m_export && m_schema.IsEmpty() && m_listOnly == false)
  {
    xerror("SQLXport: Need a schema name upon exporting a database\n");
  }
  if(m_export && m_statistics)
  {
    xerror("SQLXport: Can only perform statistics upon import\n");
  }
  if(m_export && m_recompile)
  {
    xerror("SQLXport: Can only perform full recompile upon export\n");
  }
  if(m_file.IsEmpty())
  {
    m_file = "ExportDump.dmp";
    xprintf(false,"SQLXport: WARNING: Using default dump file name: %s\n",m_file.GetString());
  }
  if(m_export && m_user.CompareNoCase(m_schema) != 0 )
  {
    xerror("SQLXport: WARNING: not logged in, in the schema to export/import.\n");
    xerror("Correct functioning of this program is **NOT** garantueed.\n");
    xerror("You have been warned!!\n\n");
  }
  if(!m_export)
  {
    if(m_commitRange < DATA_COMMIT_RANGE_MINIMUM || m_commitRange > DATA_COMMIT_RANGE_MAXIMUM)
    {
      xerror("SQLXport: Data commit out of range. Must be between %d and %d\n"
            ,DATA_COMMIT_RANGE_MINIMUM,DATA_COMMIT_RANGE_MAXIMUM);
    }
  }

  if(!m_object.IsEmpty() && m_object.Find(',') > 0)
  {
    m_multiple = true;
    MakeMultipleObjectQuery();
  }

//   if(!m_object.IsEmpty() && m_object.Find('%') > 0)
//   {
//     m_multiple = true;
//     MakeMultipleObjectSelect();
//   }

  if(m_export)
  {
    if(m_drop)
    {
      xerror("SQLXport can only drop the schema content on import\n");
    }
    if(m_statistics)
    {
      xerror("SQLXport can only recompute statistics on import\n");
    }
    if(m_recompile)
    {
      xerror("SQLXport can only do a full recompile on import\n");
    }
    if(m_commitRange != DATA_COMMIT_RANGE_DEFAULT)
    {
      xerror("SQLXport can only set commit range on import\n");
    }
  }
  else // m_export == false
  {
    if(m_rows == false)
    {
      xerror("SQLXport can only use /NOROWS upon export mode\n");
    }
    if(m_constraints == false)
    {
      xerror("OraXport can only use /NOCONSTRAINTS upon export mode\n");
    }
    if(m_source == false)
    {
      xerror("SQLXport can only use /NOSOURCE upon export mode\n");
    }
    if(m_grants == false)
    {
      xerror("SQLXport can only use /NOGRANTS upon export mode\n");
    }
    if(m_createSql)
    {
      xerror("SQLXport can only use /CREATESQL upon export mode\n");
    }
  }

  // Do error
  if(m_errors)
  {
    xputs("\n");
    Usage();

    xputs("\n");
    xputs("Program aborted\n");
    exit(2);
  }
}

void
Parameters::Usage()
{
  xprintf(false, "Usage:\n");
  xprintf(false, "SQLXport /DIR:{export | import} options\n");
  xprintf(false, "The following are valid options:\n");
  xprintf(false, "\n");
  xprintf(false, "/DBASE:<database>    Name of the database to export from/import to\n");
  xprintf(false, "/USER:<username>     Name of the user logging in to the database\n");
  xprintf(false, "/PASSWD:<password>   Password of the user logging in\n");
  xprintf(false, "/FILE:<filename>     Name of the file the export goes into/import comes from\n");
  xprintf(false, "/SCHEMA:<schemaname> Name of the schema that's been exported (export only)\n");
  xprintf(false, "/OBJECT:<objectname> Name of the object to be exported or imported\n");
  xprintf(false, "                     You can use multiple objects delimited by a ',' char\n");
  xprintf(false, "/FILTER:<filter>     Extra filter to select data rows (export only)\n");
  xprintf(false, "/PARAMS:<filename>   File with these parameters\n");
  xprintf(false, "/DROP                Drop schema contents (import only)\n");
  xprintf(false, "/CONSISTENT          Use share table lock (export only)\n");
  xprintf(false, "/WAIT:<seconds>      Time to wait for a share table lock (export only)\n");
  xprintf(false, "/NOROWS              Do not export the tables data rows\n");
  xprintf(false, "/NOCONSTRAINTS       Do not export indices/primary/foreign/check constraints\n");
  xprintf(false, "/NOSOURCE            Do not export functions/procedures/types/packages/triggers\n");
  xprintf(false, "/NOGRANTS            Do not export the object's access rights\n");
  xprintf(false, "/COMMIT:<nnn>        Do commit after <nnn> records (import only)\n");
  xprintf(false, "/STATISTICS          Start full statistics after import\n");
  xprintf(false, "/RECOMPILE           Start full recompile after import\n");
  xprintf(false, "/LIST                Only list resulting SQL. (import only)\n");
  xprintf(false, "/STRIPDIAC           Strip European diacritics (import only)\n");
  xprintf(false, "/CREATESQL           Create scripts (export only)\n");
  xprintf(false, "/DEBUG               Debug reading/writing of row data\n");
  xprintf(false, "/NOHANG              Stop hanging in virus scanner at the end\n");
  xprintf(false, "/H(ELP) or /?        This help page\n");
  xprintf(false, "\n");
}

// Read all parameters from a parameter file
void
Parameters::ReadParameterFile(XString p_filename)
{
  FILE* file = nullptr;
  errno_t error = fopen_s(&file, p_filename, "r");
  if(error == 0)
  {
    char buffer[MAX_PATH];
    while(fgets(buffer,MAX_PATH,file))
    {
      XString line(buffer);
      line.Trim("\r\n");
      if(line.GetAt(0) != '#')
      {
        ProcessOneParameter(line);
      }
    }
    fclose(file);
  }
  else
  {
    xprintf(false,"ERROR: [%d] Cannot open parameter file: %s\n",(int)error,p_filename.GetString());
    TerminateWithoutCleanup(3);
  }
}

// Processing the options on the command line
void
Parameters::ProcessCommandLine(int argc,char* argv[])
{
  // Find the direction from the executable name
  FindDirection();

  // Find the options from the command line
  // /DIR: overrides the executable name
  for (int i=1; i < argc; i++)
  {
    LPCSTR param = argv[i];

    if (param[0] == '-' || param[0] == '/')
    {
      if(tolower(param[1]) =='h' || param[1] == '?')
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
  filename += "_";
  filename += m_direction;
  filename += ".txt";

  // Open in write mode
  fopen_s(&m_logfile,filename,"w");
  if(m_logfile == NULL)
  {
    printf("WARNING: Could not open logfile: %s\n",(LPCTSTR) filename);
  }
}



// Close the logfile
void 
Parameters::CloseLogfile()
{
  if(m_logfile)
  {
    if(fclose(m_logfile))
    {
      printf("WARNING: Logfile not correctly flushed to disk.\n");
    }
    m_logfile = nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
Parameters::MakeMultipleObjectQuery()
{
  XString objects("IN (");
  int pos = m_object.Find(',');
  int count = 0;

  while(pos >= 0)
  {
    ++count;

    // Find next object
    XString first = m_object.Left(pos);
    m_object = m_object.Mid(pos + 1);

    // Put object in objects string
    if(count > 1)
    {
      objects += ",";
    }
    objects += "\'";
    objects += first;
    objects += "\'";

    // Find next delimiter
    pos = m_object.Find(',');
    if(pos < 0)
    {
      objects += ",\'" + m_object + "\'";
    }
  }
  // Finalize
  m_object = objects + ")";
}

void
Parameters::MakeMultipleObjectSelect()
{
  XString objects("LIKE '");
  objects += m_object;
  objects += "'\n";

  m_object = objects;
}

// See if the executable is renamed "import" or "export"
// So we do not have to specify the direction
void
Parameters::FindDirection()
{
  XString name = GetExeFile();
  if(name.CompareNoCase("export") == 0 ||
     name.CompareNoCase("import") == 0)
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
       if(p_parameter.Left(4) .CompareNoCase("dir:")          == 0) m_direction    = p_parameter.Mid(4);
  else if(p_parameter.Left(6) .CompareNoCase("dbase:")        == 0) m_database     = p_parameter.Mid(6);
  else if(p_parameter.Left(5) .CompareNoCase("user:")         == 0) m_user         = p_parameter.Mid(5);
  else if(p_parameter.Left(7) .CompareNoCase("passwd:")       == 0) m_password     = p_parameter.Mid(7);
  else if(p_parameter.Left(7) .CompareNoCase("schema:")       == 0) m_schema       = p_parameter.Mid(7);
  else if(p_parameter.Left(5) .CompareNoCase("file:")         == 0) m_file         = p_parameter.Mid(5);
  else if(p_parameter.Left(7) .CompareNoCase("object:")       == 0) m_object       = p_parameter.Mid(7);
  else if(p_parameter.Left(7) .CompareNoCase("filter:")       == 0) m_filter       = p_parameter.Mid(7);
  else if(p_parameter.Left(5) .CompareNoCase("wait:")         == 0) m_waitTime     = atoi(p_parameter.Mid(5));
  else if(p_parameter.Left(7) .CompareNoCase("commit:")       == 0) m_commitRange  = atoi(p_parameter.Mid(7));
  else if(p_parameter.Left(10).CompareNoCase("consistent")    == 0) m_consistent   = true;
  else if(p_parameter.Left(4) .CompareNoCase("drop")          == 0) m_drop         = true;
  else if(p_parameter.Left(4) .CompareNoCase("list")          == 0) m_listOnly     = true;
  else if(p_parameter.Left(5) .CompareNoCase("debug")         == 0) m_debug        = true;
  else if(p_parameter.Left(10).CompareNoCase("statistics")    == 0) m_statistics   = true;
  else if(p_parameter.Left(9) .CompareNoCase("recompile")     == 0) m_recompile    = true;
  else if(p_parameter.Left(8) .CompareNoCase("nogrants")      == 0) m_grants       = false;
  else if(p_parameter.Left(6) .CompareNoCase("norows")        == 0) m_rows         = false;
  else if(p_parameter.Left(13).CompareNoCase("noconstraints") == 0) m_constraints  = false;
  else if(p_parameter.Left(8) .CompareNoCase("nosource")      == 0) m_source       = false;
  else if(p_parameter.Left(9) .CompareNoCase("createsql")     == 0) m_createSql    = true;
  else if(p_parameter.Left(6) .CompareNoCase("nohang")        == 0) m_noHang       = true;
  else if(p_parameter.Left(9) .CompareNoCase("stripdiac")     == 0) m_stripDiacs   = true;
  else if(p_parameter.Left(6) .CompareNoCase("params")        == 0)
  {
    XString filename = p_parameter.Mid(7);
    ReadParameterFile(filename);
  }
  else
  {
    // No known parameter
    xprintf(false,"ERROR: Unknown startup parameter: %s\n",p_parameter.GetString());
    TerminateWithoutCleanup(3);
  }
}
