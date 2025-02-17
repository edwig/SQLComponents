////////////////////////////////////////////////////////////////////////
//
// File: Xfile.h
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
#include "XFile.h"
#include "XPort.h"
#include "SQLQuery.h"
#include "SQLTransaction.h"
#include "SQLXport.h"
#include "Parameters.h"

//////////////////////////////////////////////////////////////////////////
//
// STATIC READ/WRITE FUNCTIONS
// CLIB WRAPPERS
//
//////////////////////////////////////////////////////////////////////////

static void xfputc(int p_ch,FILE* p_file)
{
  int ch((p_ch & 0xFF) ^ 0xFF);
  if(_fputtc(ch & 0xFF,p_file) != ch)
  {
    xerror(_T("Output file write error\n"));
    throw _TEOF;
  }
}

static void xfwrite(const void* p_buffer,size_t p_size,FILE* p_file)
{
  const _TUCHAR* buffer = (const _TUCHAR*)p_buffer;
  for(size_t ind = 0; ind < p_size; ++ind)
  {
    xfputc(buffer[ind],p_file);
  }
}

static _TUCHAR xfgetc(FILE* p_file)
{
  int ch = _fgettc(p_file);
  if(ch == _TEOF)
  {
    xerror(_T("Input file read error: EOF reached.\n"));
    throw _TEOF;
  }
  return (((_TUCHAR)ch) ^ 0xFF);
}

static void xfread(void* p_buffer,size_t p_size,FILE* p_file)
{
  _TUCHAR* buffer = (_TUCHAR*)p_buffer;
  for(size_t ind = 0; ind < p_size; ++ind)
  {
    buffer[ind] = xfgetc(p_file);
  }
}

// XFILE BINARY FORMAT
// 1: HEADER ("SQLXport 1.0")
// 2: String objects: n * object ( 'T'<n>string of length n'\1' )
// 3: Data   objects: n * object ( 'D'<n><type><N>binary data of length <n>'\2' )
//
// Type of objects
// 'S'  : Section name. To be printed in the logfile
// 'Q'  : Resulting SQL.Run on import
// 'T'  : Table name
// 'C'  : Column name
// 'I'  : Index name
// 'C'  : Constraint name
// 'V'  : View name
// 'N'  : Sequence name
// 'F'  : Function/procedure/type/package/trigger etc
// 'P'  : Public synonym
// 'K'  : Count of number or records in the table (Optional after 1.5)
// 'R'  : Row object. string is empty "n" is number of columns
// 'D'  : Data object. string is binary data
// 'E'  : End marker for recurring groups. Contains string "END"
// 'X'  : End-of file marker. Contains the string 'END-EXPORT'
// 'A'  : Schema name
// 'U'  : User object
// 'Z'  : RDBMS type name

XFile::XFile(Parameters& p_parameters)
      :m_parameters(p_parameters)
{
}

XFile::~XFile()
{
  Close();
}

bool
XFile::OpenCreate(XString p_filename,XString p_databaseType)
{
  Close();
  _tfopen_s(&m_file,p_filename,_T("wb+"));
  if(m_file)
  {
    WriteHeader(p_databaseType);
  }
  else
  {
    xerror(_T("File [%s] could not be opened\n"),p_filename.GetString());
  }
  return (m_file != NULL);
}

bool
XFile::OpenRead(XString p_filename,XString p_databaseType,bool p_listonly)
{
  Close();
  _tfopen_s(&m_file,p_filename,_T("rb+"));
  if(m_file)
  {
    XString type;
    if(ReadHeader(type) == false)
    {
      xerror(_T("File [%s] does not have a valid SQLXport header\n")
             _T("Are you sure it is indeed an SQLXport file?\n")
            ,p_filename.GetString());
      return false;
    }
    if(!p_listonly && type != p_databaseType)
    {
      xerror(_T("File [%s] is for a different RDBMS database platform!\n")
             _T("The export came from a [%s] database.\n")
             _T("While this is a [%s] database!")
            ,type.GetString(),p_databaseType.GetString());
      return false;
    }
  }
  else
  {
    xerror(_T("File [%s] could not be opened\n"),p_filename.GetString());
  }
  return (m_file != NULL);
}

bool
XFile::Close()
{
  if(m_file)
  {
    return fclose(m_file) > 0;
  }
  return true;
}

_TUCHAR
XFile::NextType()
{
  _TUCHAR type = xfgetc(m_file);
  if(type != _TEOF)
  {
    _TUCHAR unget = type ^ 255;
    _ungettc(unget,m_file);
  }
  return type;
}

void
XFile::WriteHeader(XString p_type)
{
  xfwrite(_T("SQLXport "),  9,m_file);
  xfwrite(XPORT_VERSION,3,m_file);
  xfputc(SEPARATOR_STRING,m_file);
  WriteString(_T('Z'),p_type);
}

bool
XFile::ReadHeader(XString& p_type)
{
  TCHAR  buffer[20];
  TCHAR* needed_version = _T("SQLXport ") XPORT_VERSION;
  int   needed_length  = (int) _tcslen(needed_version);

  for(int i = 0;i < needed_length; ++i)
  {
    buffer[i] = (TCHAR)xfgetc(m_file);
  }
  buffer[needed_length] = 0;
  if(_tcscmp(buffer,needed_version) == 0)
  {
    if((TCHAR)xfgetc(m_file) != (TCHAR)SEPARATOR_STRING)
    {
      xerror(_T("File was created with different separator string. Cannot process."));
      return false;
    }
    // Version and separator_string exactly OK!
    TCHAR ch = 0;
    XString type;
    ReadString(ch,type);
    if(ch != 'Z')
    {
      xerror(_T("Missing database type header. Cannot import from this file."));
      return false;
    }
    p_type = type;
    return true;
  }
  if(_tcscmp(buffer,needed_version) < 0)
  {
    _ftprintf(stderr,_T("WARNING:\n"));
    _ftprintf(stderr,_T("Version of export file is smaller than current version!\n")
                     _T("Current version of program is: %s\n")
                     _T("Version of the export file is: %s")
                    ,needed_version
                    ,buffer);
  }
  else
  {
    xerror(_T("This version of the program cannot process the export file!\n")
           _T("Current version of program is : %s\n")
           _T("Version of the export file is : %s"),needed_version,buffer);
  }
  return false;
}

void
XFile::WriteSQL(XString p_sql)
{
  WriteString(_T('Q'),p_sql);
}

XString 
XFile::ReadSQL()
{
  TCHAR type;
  XString sql;

  if(ReadString(type,sql) == false)
  {
    return _T("");
  }
  if(type != 'Q')
  {
    xerror(_T("Error reading SQL string from file.\n"));
    return _T("");
  }
  return sql;
}

void
XFile::WriteSection(XString p_name)
{
  WriteString(_T('S'),p_name);
  xprintf(false,_T("\n"));
  xprintf(false,_T("Exporting section: %s\n"),p_name.GetString());
}

XString
XFile::ReadSection()
{
  TCHAR type;
  XString name;

  if(ReadString(type,name) == false)
  {
    return _T("");
  }
  if(type != 'S')
  {
    xerror(_T("Internal Error: expecting section name.\n"));
    return _T("");
  }
  xprintf(false,_T("\n"));
  xprintf(false,_T("Import section: %s\n"),name.GetString());
  return name;
}

void
XFile::WriteTable(XString p_table)
{
  WriteString(_T('T'),p_table);
  xprintf(false,_T("Exporting table: %-32s "),p_table.GetString());
}

XString
XFile::ReadTable(bool p_doNewline)
{
  TCHAR type;
  XString table;
  if(ReadString(type,table) == false)
  {
    return _T("");
  }
  if(type != 'T')
  {
    xerror(_T("Internal error: Did expect table name.\n"));
    return _T("");
  }
  xprintf(false,_T("Importing table: %-32s "),table.GetString());
  if(p_doNewline)
  {
    xprintf(true,_T("\n"));
  }
  return table;
}

void
XFile::WriteIndex(XString p_index)
{
  WriteString(_T('I'),p_index);
  xprintf(false,_T("Exporting index: %s\n"),p_index.GetString());
}

XString
XFile::ReadIndex()
{
  TCHAR type;
  XString index;
  if(ReadString(type,index) == false)
  {
    return _T("");
  }
  if(type != 'I')
  {
    xerror(_T("Internal error: Did expect index name.\n"));
    return _T("");
  }
  xprintf(false,_T("Importing index: %s\n"),index.GetString());
  return index;
}

void
XFile::WriteColumns(OList* p_columns)
{
  for(unsigned ind = 0;ind < p_columns->size();++ind)
  {
    XString column = (*p_columns)[ind];
    WriteString(_T('C'),column);
  }
  WriteSectionEnd();
}

void
XFile::ReadColumns(OList* p_columns)
{
  // Clear the column list
  p_columns->clear();

  while(true)
  {
    TCHAR type;
    XString name;
    ReadString(type,name);
    if(type == 'E')
    {
      break;
    }
    p_columns->push_back(name);
  }
}

void  
XFile::WriteSynonym(XString p_type,XString p_name)
{
  WriteString(_T('P'),p_type);
  WriteString(_T('P'),p_name);
  xprintf(false,_T("Exporting synonym for %s %s\n"),p_type.GetString(),p_name.GetString());
}

bool    
XFile::ReadSynonym(XString& p_type,XString& p_name)
{
  TCHAR type1;
  TCHAR type2;
  ReadString(type1,p_type);
  ReadString(type2,p_name);
  if(type1 != _T('P') || type2 != 'P')
  {
    xerror(_T("Internal error reading synonym. Not a synonym string\n"));
    return false;
  }
  xprintf(false,_T("Defining public synonym for %s %s\n"),p_type.GetString(),p_name.GetString());
  return true;
}

void
XFile::WriteConstraint(int p_num,XString p_table,XString p_constraint)
{
  WriteString(_T('C'),p_constraint);

  // Special case for Oracle
  if(p_constraint.Left(6).Compare(_T("SYS_C0")) == 0)
  {
    if((p_num % COMMIT_SIZE) == 0)
    {
      xprintf(false,_T("Exporting constraint: %d\n"),p_num);
    }
  }
  else
  {
    xprintf(false,_T("Exporting constraint: %s.%s\n"),p_table.GetString(),p_constraint.GetString());
  }
}

XString
XFile::ReadConstraint()
{
  TCHAR    type;
  XString name;
  ReadString(type,name);
  if(type != 'C')
  {
    xerror(_T("Internal error reading constraint. Not a constraint name\n"));
    return _T("");
  }
  xprintf(false,_T("Enforcing constraint: %s\n"),name.GetString());
  return name;
}

void
XFile::WriteView(XString p_view)
{
  WriteString(_T('V'),p_view);
  xprintf(false,_T("Exporting view: %s\n"),p_view.GetString());
}

XString
XFile::ReadView()
{
  TCHAR type;
  XString name;
  ReadString(type,name);
  if(type != 'V')
  {
    xerror(_T("Internal error reading view. Not a view name\n"));
    return _T("");
  }
  xprintf(false,_T("Importing view: %s\n"),name.GetString());
  return name;
}

void
XFile::WriteSequence(XString p_sequence)
{
  WriteString(_T('N'),p_sequence);
  xprintf(false,_T("Exporting sequence: %s\n"),p_sequence.GetString());
}

XString
XFile::ReadSequence()
{
  TCHAR type;
  XString name;
  ReadString(type,name);
  if(type != 'N')
  {
    xerror(_T("Internal error reading sequence. Not a sequence name.\n"));
    return _T("");
  }
  xprintf(false,_T("Importing sequence: %s\n"),name.GetString());
  return name;
}

void
XFile::WriteProcedure(XString p_procedure)
{
  WriteString(_T('F'),p_procedure);
  xprintf(false,_T("Exporting source %s\n"),p_procedure.GetString());
}

XString
XFile::ReadProcedure()
{
  TCHAR    type;
  XString name;
  ReadString(type,name);
  if(type != 'F')
  {
    xerror(_T("Internal error reading source. Not a function/trigger/procedure name\n"));
    return _T("");
  }
  xprintf(false,_T("Importing source %s\n"),name.GetString());
  return name;
}

bool
XFile::WriteRows(XPort& p_xport,XString& p_table,bool p_export)
{
  long    todo    = 0;
  long    rows    = 0;
  bool    result  = true;
  XString name    = _T("SELECT_for_") + p_table;
  XString select  = p_xport.GetDefineRowSelect  (p_table);
  XString count   = p_xport.GetDefineCountSelect(p_table);
  
  if(p_export)
  {
    try
    {
      SQLQuery qry(p_xport.GetDatabase());

      // Count our records first
      qry.DoSQLStatement(count);
      if(qry.GetRecord())
      {
        todo = qry.GetColumn(1)->GetAsSLong();
      }
      WriteObject(_T('K'),todo);

      // Optimize for empty tables. 
      // Only do export if at least 1 row expected
      if(todo > 0)
      {
        // Get cursor for the records
        qry.DoSQLStatement(select);
        while(qry.GetRecord())
        {
          ++rows;
          int cols = qry.GetNumberOfColumns();

          // Write row object
          WriteObject(_T('R'),cols);
          for(int num = 1; num <= cols; ++num)
          {
            // Get variant of the column
            SQLVariant* var = qry.GetColumn(num);

            if(m_parameters.m_debug)
            {
              XString data; 
              XString colname;
              var->GetAsString(data);
              qry.GetColumnName(num,colname);
              xprintf(false,_T("Row [%d] Column [%3d:%-20s] Data [%s]\n"),rows,num,colname.GetString(),data.GetString());
            }
            // Write data object
            WriteData(var);
          }
          // Write END object
          WriteSectionEnd();

          // Processing...
          if((rows % COMMIT_SIZE) == 0)
          {
            _tprintf(_T("[%10d] %10d rows processed\r"),todo,rows);
            _tprintf(_T("Exporting table: %-32s "),(LPCTSTR)p_table);
          }
        }
      }
      xprintf(false,_T("[%10d] %10d rows exported.\n"),todo,rows);
    }
    catch(StdException& ex)
    {
      xerror(_T("\nError retrieving data from table [%s] : %s\n"),p_table.GetString(),ex.GetErrorMessage().GetString());
      result = false;
    }
  }
  else
  {
    xprintf(false,_T("no rows exported.\n"));
  }
  // Write END object
  WriteSectionEnd();
  return result;
}

bool
XFile::ReadRows(XPort& p_xport,XString& p_table,bool p_object,bool p_list)
{
  int     rows    = 0;
  int     errors  = 0;
  int     todo    = 0;
  XString name    = _T("INSERT_for_") + p_table;
  XString insert  = p_xport.GetDefineRowInsert(p_table);
  OList*  columns = p_xport.GetColumns();
  int     cols    = (int) columns->size();

  try
  {
    // Start transaction
    SQLTransaction trans(p_xport.GetDatabase(),name);
    SQLQuery query(p_xport.GetDatabase());

    // Prepare our query for execution
    if(!p_list)
    {
      query.DoSQLPrepare(insert);
    }

    // Bind the parameters for the query
    for(int ind = 0;ind < cols; ++ind)
    {
      SQLVariant var;
      query.SetParameter(ind + 1,&var);
    }
    
    // Loop over all records
    TCHAR marker = NextType();
    while(marker != 'E')
    {
      int length = 0;

      // Optionally read the number of records in the table
      if(marker == 'K')
      {
        ReadObject(marker,todo);
        marker = NextType();

        // Stop if no objects written to file
        if(marker == 'E')
        {
          break;
        }
      }

      if(marker != 'R')
      {
        xerror(_T("\nInternal error: Expected a row object\n"));
        return false;
      }
      ReadObject(marker,length);
      if(length != cols)
      {
        xerror(_T("\nInternal error: Expected a row object of [%d] columns, but got [%d] columns instead\n"),cols,length);
        return false;
      }
      // row count
      ++rows;

      for(int ind = 0;ind < cols; ++ind)
      {
        SQLVariant* var = query.GetParameter(ind+1);
        ReadData(var);

        if(m_parameters.m_debug)
        {
          XString data; 
          var->GetAsString(data);
          xprintf(false,_T("Row [%d] Column [%3d:%-20s] Data [%s]\n"),rows,ind + 1,(*columns)[ind].GetString(),data.GetString());
        }
      }

      // Do the insert
      if(p_object && !p_list)
      {
        try
        {
          query.DoSQLExecute(true);
        }
        catch(StdException& ex)
        {
           xerror(rows,XString(_T("Error: ")) + ex.GetErrorMessage());
          ++errors;
          ++m_parameters.m_errors;
        }
      }
      // End of the row
      ReadEnd();
      marker = NextType();

      if(!p_list)
      {
        // Processing...
        if((rows % COMMIT_SIZE) == 0)
        {
          _tprintf(_T("[%10d] %10d rows processed\r"),todo,rows);
          _tprintf(_T("Importing table: %-32s "),(LPCTSTR)p_table);
        }
        // Extra commit?
        if((rows % m_parameters.m_commitRange) == 0)
        {
          trans.Commit();
          trans.Start(name);
          _tprintf(_T("[%10d] %10d rows COMMITTED\r"),todo,rows);
          _tprintf(_T("Importing table: %-32s "),(LPCTSTR)p_table);
        }
      }
    }
    // Commit all data in the table
    if(!p_list)
    {
      trans.Commit();
    }
  }
  catch(StdException& ex)
  {
    xerror(_T("\nError inserting data in the table [%s] : %s\n"),p_table.GetString(),ex.GetErrorMessage().GetString());
  }
  catch(...)
  {
    xerror(_T("\nError inserting data in the table: %s\n"),p_table.GetString());
    throw;
  }

  if(p_list)
  {
    xprintf(false,_T("Export contains [%d] rows to import\n"),todo);
  }
  else
  {
    xprintf(false,_T("[%10d] %10d rows imported.\n"),todo,rows - errors);
  }
  if(errors)
  {
    xerror(_T("LOAD ERRORS: Table [%s] is missing [%d] rows.\n"),p_table.GetString(),errors);
    print_all_errors();
    return false;
  }
  return true;
}

// Write section end
void
XFile::WriteSectionEnd()
{
  XString end(_T("END"));
  WriteString(_T('E'),end);
}

bool
XFile::ReadEnd()
{
  TCHAR type;
  XString string;
  ReadString(type,string);
  if(type != 'E')
  {
    xerror(_T("Internal error: Reading 'end' object\n"));
    return false;
  }
  if(string.CompareNoCase(_T("END")))
  {
    xerror(_T("Internal error: Reading END object\n"));
    return false;
  }
  return true;
}

void    
XFile::WriteEndExport()
{
  XString EndExport(_T("END-EXPORT"));
  WriteString(_T('X'),EndExport);
}

void
XFile::ReadEndExport()
{
  TCHAR type;
  XString string;
  ReadString(type,string);
  if(type != 'X')
  {
    xerror(_T("Internal error: reading end-of-export object\n"));
    return;
  }
  if(string != _T("END-EXPORT"))
  {
    xerror(_T("Internal error: Not the END-OF-EXPORT object\n"));
  }
}

void
XFile::Flush()
{
  fflush(m_file);
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE METHODS
//
//////////////////////////////////////////////////////////////////////////

void    
XFile::WriteString(TCHAR p_type,XString& p_string)
{
  int length = p_string.GetLength();

  xfputc(p_type,m_file);
  xfwrite(&length,sizeof(int),m_file);
  xfwrite(p_string.GetString(),length,m_file);
  xfputc(SEPARATOR_STRING,m_file);
}

bool    
XFile::ReadString(TCHAR& p_type,XString& p_string)
{
  p_string.Empty();

  p_type = (TCHAR)xfgetc(m_file);
  if(_istalpha(p_type))
  {
    int length = 0;
    xfread(&length,(size_t)sizeof(int),m_file);

    for(int ind = 0; ind < length; ++ind)
    {
      int character = xfgetc(m_file);
      p_string += (TCHAR)character;
    }

    TCHAR marker = (TCHAR)xfgetc(m_file);
    if(marker != SEPARATOR_STRING)
    {
      xerror(_T("Error reading string from file. Missing delimiter.\n"));
      return false;
    }
  }
  else
  {
    xerror(_T("Error reading string from file\n"));
    return false;
  }
  return true;
}

void
XFile::WriteObject(TCHAR p_type,int p_length)
{
  xfputc(p_type,m_file);
  xfwrite(&p_length,sizeof(int),m_file);
  xfputc(SEPARATOR_STRING,m_file);
}

bool    
XFile::ReadObject(TCHAR& p_type,int& p_length)
{
  p_type = (TCHAR)xfgetc(m_file);
  xfread(&p_length,(size_t)sizeof(int),m_file);
  TCHAR marker = (TCHAR)xfgetc(m_file);
  if(marker != SEPARATOR_STRING)
  {
    xerror(_T("Error reading object from file. Missing delimiter.\n"));
    return false;
  }
  return true;
}

void
XFile::WriteData(SQLComponents::SQLVariant* p_var)
{
  int   length =  p_var->GetDataSize();
  int   type   =  p_var->GetDataType();
  int   sqltyp =  p_var->GetSQLDataType();
  bool  isnull = *p_var->GetIndicatorPointer() == SQL_NULL_DATA;
  const void* buffer = p_var->GetDataPointer();

  // Potentially a shorter string
  if(type == SQL_C_CHAR)
  {
    int altlength = (int) _tcslen((TCHAR*)buffer);
    if(altlength < length)
    {
      length = altlength;
    }
  }

  // Write to XFILE dump
  xfputc(_T('D'),m_file);                            // Writing data
  xfwrite(&length,(size_t)sizeof(int),m_file);   // Binary length of object
  xfwrite(&type,  (size_t)sizeof(int),m_file);   // SQL_C_* datatype
  xfwrite(&sqltyp,(size_t)sizeof(int),m_file);   // SQL_XXX datatype
  xfputc(isnull ? _T('N') : _T('-'),m_file);             // IS NULL ?
  xfwrite(buffer,(size_t)length,m_file);         // Binary data
  xfputc(SEPERATOR_DATA,m_file);                 // End-separator
}

bool    
XFile::ReadData(SQLComponents::SQLVariant* p_var)
{
  int  length = 0;
  int  type   = 0;
  int  sqltyp = 0;
  TCHAR marker = 0;
  TCHAR isnull = 0;

  // Read from XFILE DUMP
  marker = (TCHAR)xfgetc(m_file);
  if(marker != 'D')
  {
    xerror(_T("Error reading row data object.\n"));
    return false;
  }
  xfread(&length,(size_t)sizeof(int),m_file);
  xfread(&type,  (size_t)sizeof(int),m_file);
  xfread(&sqltyp,(size_t)sizeof(int),m_file);
  isnull = (TCHAR)xfgetc(m_file);
  _TUCHAR* buffer = new _TUCHAR[length + 1];
  xfread(buffer,(size_t)length,m_file);
  buffer[length] = 0;

  // Strip European diacritics from character strings?
  if(m_parameters.m_stripDiacs && type == SQL_C_CHAR)
  {
    StripDiacritics(buffer);
  }

  // Construct variant from binary buffer
  p_var->SetFromBinaryStreamData(type,length,buffer,isnull == 'N');
  p_var->SetSQLDataType(sqltyp);
  delete [] buffer;

  // Check end marker
  marker = (TCHAR)xfgetc(m_file);
  if(marker != SEPERATOR_DATA)
  {
    xerror(_T("Error reading row data object from file. Missing delimiter.\n"));
    return false;
  }
  return true;
}

// Strips the diacritics as used in ISO8859P1
// from the WIN1252 character set for West-European use only
void
XFile::StripDiacritics(_TUCHAR* p_buffer)
{
  for(_TUCHAR* pnt = p_buffer; *pnt; ++pnt)
  {
    if(*pnt > 127)  // Optimize for speed
    {
      switch(*pnt)
      {
        case 0xE9: *pnt = _T('e'); break; // é e-aigu
        case 0xE8: *pnt = _T('e'); break; // è e-grave
        case 0xEB: *pnt = _T('e'); break; // ë e-dieresis
        case 0xEA: *pnt = _T('e'); break; // ê e-circumflex

        case 0xE1: *pnt = _T('a'); break; // á a-aigu
        case 0xE0: *pnt = _T('a'); break; // à a-grave
        case 0xE4: *pnt = _T('a'); break; // ä a-dieresis
        case 0xE2: *pnt = _T('a'); break; // â a-circumflex
        case 0xE3: *pnt = _T('a'); break; // ã a-tilde

        case 0xED: *pnt = _T('i'); break; // í i-aigu
        case 0xEC: *pnt = _T('i'); break; // ì i-grave
        case 0xEF: *pnt = _T('i'); break; // ï i-dieresis
        case 0xEE: *pnt = _T('i'); break; // î i-circumflex

        case 0xF3: *pnt = _T('o'); break; // ó o-aigu
        case 0xF2: *pnt = _T('o'); break; // ò o-grave
        case 0xF6: *pnt = _T('o'); break; // ö o-dieresis
        case 0xF4: *pnt = _T('o'); break; // ô o-circumflex
        case 0xF5: *pnt = _T('o'); break; // õ o-tilde

        case 0xFA: *pnt = _T('u'); break; // ú u-aigu
        case 0xF9: *pnt = _T('u'); break; // ù u-grave
        case 0xFC: *pnt = _T('u'); break; // ü u-dieresis
        case 0xFB: *pnt = _T('u'); break; // û u-circumflex

        case 0xE7: *pnt = _T('c'); break; // ç c-cedilla
        case 0xF1: *pnt = _T('n'); break; // ñ n-tilde 
        default:               break; // DO NOTHING!!
      }
    }
  }
}
