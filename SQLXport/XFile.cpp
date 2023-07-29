////////////////////////////////////////////////////////////////////////
//
// File: Xfile.h
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
  if(fputc(ch & 0xFF,p_file) != ch)
  {
    xerror("Output file write error\n");
    throw EOF;
  }
}

static void xfwrite(const void* p_buffer,size_t p_size,FILE* p_file)
{
  const unsigned char* buffer = (unsigned char*)p_buffer;
  for(size_t ind = 0; ind < p_size; ++ind)
  {
    xfputc(buffer[ind],p_file);
  }
}

static unsigned char xfgetc(FILE* p_file)
{
  int ch = fgetc(p_file);
  if(ch == EOF)
  {
    xerror("Input file read error: EOF reached.\n");
    throw EOF;
  }
  return (((unsigned char)ch) ^ 0xFF);
}

static void xfread(void* p_buffer,size_t p_size,FILE* p_file)
{
  unsigned char* buffer = (unsigned char*)p_buffer;
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
  fopen_s(&m_file,p_filename,"wb+");
  if(m_file)
  {
    WriteHeader(p_databaseType);
  }
  else
  {
    xerror("File [%s] could not be opened\n",p_filename.GetString());
  }
  return (m_file != NULL);
}

bool
XFile::OpenRead(XString p_filename,XString p_databaseType,bool p_listonly)
{
  Close();
  fopen_s(&m_file,p_filename,"rb+");
  if(m_file)
  {
    XString type;
    if(ReadHeader(type) == false)
    {
      xerror("File [%s] does not have a valid SQLXport header\n"
             "Are you sure it is indeed an SQLXport file?\n"
            ,p_filename.GetString());
      return false;
    }
    if(!p_listonly && type != p_databaseType)
    {
      xerror("File [%s] is for a different RDBMS database platform!\n"
             "The export came from a [%s] database.\n"
             "While this is a [%s] database!"
            ,type.GetString(),p_databaseType.GetString());
      return false;
    }
  }
  else
  {
    xerror("File [%s] could not be opened\n",p_filename.GetString());
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

char
XFile::NextType()
{
  unsigned char type = xfgetc(m_file);
  if(type != EOF)
  {
    unsigned char unget = type ^ 255;
    ungetc(unget,m_file);
  }
  return type;
}

void
XFile::WriteHeader(XString p_type)
{
  xfwrite("SQLXport ",  9,m_file);
  xfwrite(XPORT_VERSION,3,m_file);
  xfputc(SEPARATOR_STRING,m_file);
  WriteString('Z',p_type);
}

bool
XFile::ReadHeader(XString& p_type)
{
  char  buffer[20];
  char* needed_version = "SQLXport " XPORT_VERSION;
  int   needed_length  = (int) strlen(needed_version);

  for(int i = 0;i < needed_length; ++i)
  {
    buffer[i] = (char)xfgetc(m_file);
  }
  buffer[needed_length] = 0;
  if(strcmp(buffer,needed_version) == 0)
  {
    if((char)xfgetc(m_file) != (char)SEPARATOR_STRING)
    {
      xerror("File was created with different separator string. Cannot process.");
      return false;
    }
    // Version and separator_string exactly OK!
    char ch = 0;
    XString type;
    ReadString(ch,type);
    if(ch != 'Z')
    {
      xerror("Missing database type header. Cannot import from this file.");
      return false;
    }
    p_type = type;
    return true;
  }
  if(strcmp(buffer,needed_version) < 0)
  {
    fprintf(stderr,"WARNING:\n");
    fprintf(stderr,"Version of export file is smaller than current version!\n"
                   "Current version of program is: %s\n"
                   "Version of the export file is: %s"
                  ,needed_version
                  ,buffer);
  }
  else
  {
    xerror("This version of the program cannot process the export file!\n"
           "Current version of program is : %s\n"
           "Version of the export file is : %s",needed_version,buffer);
  }
  return false;
}

void
XFile::WriteSQL(XString p_sql)
{
  WriteString('Q',p_sql);
}

XString 
XFile::ReadSQL()
{
  char type;
  XString sql;

  if(ReadString(type,sql) == false)
  {
    return "";
  }
  if(type != 'Q')
  {
    xerror("Error reading SQL string from file.\n");
    return "";
  }
  return sql;
}

void
XFile::WriteSection(XString p_name)
{
  WriteString('S',p_name);
  xprintf(false,"\n");
  xprintf(false,"Exporting section: %s\n",p_name.GetString());
}

XString
XFile::ReadSection()
{
  char type;
  XString name;

  if(ReadString(type,name) == false)
  {
    return "";
  }
  if(type != 'S')
  {
    xerror("Internal Error: expecting section name.\n");
    return "";
  }
  xprintf(false,"\n");
  xprintf(false,"Import section: %s\n",name.GetString());
  return name;
}

void
XFile::WriteTable(XString p_table)
{
  WriteString('T',p_table);
  xprintf(false,"Exporting table: %-32s ",p_table.GetString());
}

XString
XFile::ReadTable(bool p_doNewline)
{
  char type;
  XString table;
  if(ReadString(type,table) == false)
  {
    return "";
  }
  if(type != 'T')
  {
    xerror("Internal error: Did expect table name.\n");
    return "";
  }
  xprintf(false,"Importing table: %-32s ",table.GetString());
  if(p_doNewline)
  {
    xprintf(true,"\n");
  }
  return table;
}

void
XFile::WriteIndex(XString p_index)
{
  WriteString('I',p_index);
  xprintf(false,"Exporting index: %s\n",p_index.GetString());
}

XString
XFile::ReadIndex()
{
  char type;
  XString index;
  if(ReadString(type,index) == false)
  {
    return "";
  }
  if(type != 'I')
  {
    xerror("Internal error: Did expect index name.\n");
    return "";
  }
  xprintf(false,"Importing index: %s\n",index.GetString());
  return index;
}

void
XFile::WriteColumns(OList* p_columns)
{
  for(unsigned ind = 0;ind < p_columns->size();++ind)
  {
    XString column = (*p_columns)[ind];
    WriteString('C',column);
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
    char type;
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
  WriteString('P',p_type);
  WriteString('P',p_name);
  xprintf(false,"Exporting synonym for %s %s\n",p_type.GetString(),p_name.GetString());
}

bool    
XFile::ReadSynonym(XString& p_type,XString& p_name)
{
  char type1;
  char type2;
  ReadString(type1,p_type);
  ReadString(type2,p_name);
  if(type1 != 'P' || type2 != 'P')
  {
    xerror("Internal error reading synonym. Not a synonym string\n");
    return false;
  }
  xprintf(false,"Defining public synonym for %s %s\n",p_type.GetString(),p_name.GetString());
  return true;
}

void
XFile::WriteConstraint(int p_num,XString p_table,XString p_constraint)
{
  WriteString('C',p_constraint);

  // Special case for Oracle
  if(p_constraint.Left(6).Compare("SYS_C0") == 0)
  {
    if((p_num % COMMIT_SIZE) == 0)
    {
      xprintf(false,"Exporting constraint: %d\n",p_num);
    }
  }
  else
  {
    xprintf(false,"Exporting constraint: %s.%s\n",p_table.GetString(),p_constraint.GetString());
  }
}

XString
XFile::ReadConstraint()
{
  char    type;
  XString name;
  ReadString(type,name);
  if(type != 'C')
  {
    xerror("Internal error reading constraint. Not a constraint name\n");
    return "";
  }
  xprintf(false,"Enforcing constraint: %s\n",name.GetString());
  return name;
}

void
XFile::WriteView(XString p_view)
{
  WriteString('V',p_view);
  xprintf(false,"Exporting view: %s\n",p_view.GetString());
}

XString
XFile::ReadView()
{
  char type;
  XString name;
  ReadString(type,name);
  if(type != 'V')
  {
    xerror("Internal error reading view. Not a view name\n");
    return "";
  }
  xprintf(false,"Importing view: %s\n",name.GetString());
  return name;
}

void
XFile::WriteSequence(XString p_sequence)
{
  WriteString('N',p_sequence);
  xprintf(false,"Exporting sequence: %s\n",p_sequence.GetString());
}

XString
XFile::ReadSequence()
{
  char type;
  XString name;
  ReadString(type,name);
  if(type != 'N')
  {
    xerror("Internal error reading sequence. Not a sequence name.\n");
    return "";
  }
  xprintf(false,"Importing sequence: %s\n",name.GetString());
  return name;
}

void
XFile::WriteProcedure(XString p_procedure)
{
  WriteString('F',p_procedure);
  xprintf(false,"Exporting source %s\n",p_procedure.GetString());
}

XString
XFile::ReadProcedure()
{
  char    type;
  XString name;
  ReadString(type,name);
  if(type != 'F')
  {
    xerror("Internal error reading source. Not a function/trigger/procedure name\n");
    return "";
  }
  xprintf(false,"Importing source %s\n",name.GetString());
  return name;
}

bool
XFile::WriteRows(XPort& p_xport,XString& p_table,bool p_export)
{
  long    todo    = 0;
  long    rows    = 0;
  bool    result  = true;
  XString name    = "SELECT_for_" + p_table;
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
      WriteObject('K',todo);

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
          WriteObject('R',cols);
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
              xprintf(false,"Row [%d] Column [%3d:%-20s] Data [%s]\n",rows,num,colname.GetString(),data.GetString());
            }
            // Write data object
            WriteData(var);
          }
          // Write END object
          WriteSectionEnd();

          // Processing...
          if((rows % COMMIT_SIZE) == 0)
          {
            printf("[%10d] %10d rows processed\r",todo,rows);
            printf("Exporting table: %-32s ",(LPCTSTR)p_table);
          }
        }
      }
      xprintf(false,"[%10d] %10d rows exported.\n",todo,rows);
    }
    catch(StdException& ex)
    {
      xerror("\nError retrieving data from table [%s] : %s\n",p_table.GetString(),ex.GetErrorMessage().GetString());
      result = false;
    }
  }
  else
  {
    xprintf(false,"no rows exported.\n");
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
  XString name    = "INSERT_for_" + p_table;
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
    char marker = NextType();
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
        xerror("\nInternal error: Expected a row object\n");
        return false;
      }
      ReadObject(marker,length);
      if(length != cols)
      {
        xerror("\nInternal error: Expected a row object of [%d] columns, but got [%d] columns instead\n",cols,length);
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
          xprintf(false,"Row [%d] Column [%3d:%-20s] Data [%s]\n",rows,ind + 1,(*columns)[ind].GetString(),data.GetString());
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
           xerror(rows,XString("Error: ") + ex.GetErrorMessage());
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
          printf("[%10d] %10d rows processed\r",todo,rows);
          printf("Importing table: %-32s ",(LPCTSTR)p_table);
        }
        // Extra commit?
        if((rows % m_parameters.m_commitRange) == 0)
        {
          trans.Commit();
          trans.Start(name);
          printf("[%10d] %10d rows COMMITTED\r",todo,rows);
          printf("Importing table: %-32s ",(LPCTSTR)p_table);
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
    xerror("\nError inserting data in the table [%s] : %s\n",p_table.GetString(),ex.GetErrorMessage().GetString());
  }
  catch(...)
  {
    xerror("\nError inserting data in the table: %s\n",p_table.GetString());
    throw;
  }

  if(p_list)
  {
    xprintf(false,"Export contains [%d] rows to import\n",todo);
  }
  else
  {
    xprintf(false,"[%10d] %10d rows imported.\n",todo,rows - errors);
  }
  if(errors)
  {
    xerror("LOAD ERRORS: Table [%s] is missing [%d] rows.\n",p_table.GetString(),errors);
    print_all_errors();
    return false;
  }
  return true;
}

// Write section end
void
XFile::WriteSectionEnd()
{
  XString end("END");
  WriteString('E',end);
}

bool
XFile::ReadEnd()
{
  char type;
  XString string;
  ReadString(type,string);
  if(type != 'E')
  {
    xerror("Internal error: Reading 'end' object\n");
    return false;
  }
  if(string.CompareNoCase("END"))
  {
    xerror("Internal error: Reading END object\n");
    return false;
  }
  return true;
}

void    
XFile::WriteEndExport()
{
  XString EndExport("END-EXPORT");
  WriteString('X',EndExport);
}

void
XFile::ReadEndExport()
{
  char type;
  XString string;
  ReadString(type,string);
  if(type != 'X')
  {
    xerror("Internal error: reading end-of-export object\n");
    return;
  }
  if(string != "END-EXPORT")
  {
    xerror("Internal error: Not the END-OF-EXPORT object\n");
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
XFile::WriteString(char p_type,XString& p_string)
{
  int length = p_string.GetLength();

  xfputc(p_type,m_file);
  xfwrite(&length,sizeof(int),m_file);
  xfwrite(p_string.GetString(),length,m_file);
  xfputc(SEPARATOR_STRING,m_file);
}

bool    
XFile::ReadString(char& p_type,XString& p_string)
{
  p_string.Empty();

  p_type = (char)xfgetc(m_file);
  if(isalpha(p_type))
  {
    int length = 0;
    xfread(&length,(size_t)sizeof(int),m_file);

    for(int ind = 0; ind < length; ++ind)
    {
      int character = xfgetc(m_file);
      p_string += (char)character;
    }

    char marker = (char)xfgetc(m_file);
    if(marker != SEPARATOR_STRING)
    {
      xerror("Error reading string from file. Missing delimiter.\n");
      return false;
    }
  }
  else
  {
    xerror("Error reading string from file\n");
    return false;
  }
  return true;
}

void
XFile::WriteObject(char p_type,int p_length)
{
  xfputc(p_type,m_file);
  xfwrite(&p_length,sizeof(int),m_file);
  xfputc(SEPARATOR_STRING,m_file);
}

bool    
XFile::ReadObject(char& p_type,int& p_length)
{
  p_type = (char)xfgetc(m_file);
  xfread(&p_length,(size_t)sizeof(int),m_file);
  char marker = (char)xfgetc(m_file);
  if(marker != SEPARATOR_STRING)
  {
    xerror("Error reading object from file. Missing delimiter.\n");
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
    int altlength = (int) strlen((char*)buffer);
    if(altlength < length)
    {
      length = altlength;
    }
  }

  // Write to XFILE dump
  xfputc('D',m_file);                            // Writing data
  xfwrite(&length,(size_t)sizeof(int),m_file);   // Binary length of object
  xfwrite(&type,  (size_t)sizeof(int),m_file);   // SQL_C_* datatype
  xfwrite(&sqltyp,(size_t)sizeof(int),m_file);   // SQL_XXX datatype
  xfputc(isnull ? 'N' : '-',m_file);             // IS NULL ?
  xfwrite(buffer,(size_t)length,m_file);         // Binary data
  xfputc(SEPERATOR_DATA,m_file);                 // End-separator
}

bool    
XFile::ReadData(SQLComponents::SQLVariant* p_var)
{
  int  length = 0;
  int  type   = 0;
  int  sqltyp = 0;
  char marker = 0;
  char isnull = 0;

  // Read from XFILE DUMP
  marker = (char)xfgetc(m_file);
  if(marker != 'D')
  {
    xerror("Error reading row data object.\n");
    return false;
  }
  xfread(&length,(size_t)sizeof(int),m_file);
  xfread(&type,  (size_t)sizeof(int),m_file);
  xfread(&sqltyp,(size_t)sizeof(int),m_file);
  isnull = (char)xfgetc(m_file);
  unsigned char* buffer = new unsigned char[length + 1];
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
  marker = (char)xfgetc(m_file);
  if(marker != SEPERATOR_DATA)
  {
    xerror("Error reading row data object from file. Missing delimiter.\n");
    return false;
  }
  return true;
}

// Strips the diacritics as used in ISO8859P1
// from the WIN1252 character set for West-European use only
void
XFile::StripDiacritics(unsigned char* p_buffer)
{
  for(unsigned char* pnt = p_buffer; *pnt; ++pnt)
  {
    if(*pnt > 127)  // Optimize for speed
    {
      switch(*pnt)
      {
        case 0xE9: *pnt = 'e'; break; // � e-aigu
        case 0xE8: *pnt = 'e'; break; // � e-grave
        case 0xEB: *pnt = 'e'; break; // � e-dieresis
        case 0xEA: *pnt = 'e'; break; // � e-circumflex

        case 0xE1: *pnt = 'a'; break; // � a-aigu
        case 0xE0: *pnt = 'a'; break; // � a-grave
        case 0xE4: *pnt = 'a'; break; // � a-dieresis
        case 0xE2: *pnt = 'a'; break; // � a-circumflex
        case 0xE3: *pnt = 'a'; break; // � a-tilde

        case 0xED: *pnt = 'i'; break; // � i-aigu
        case 0xEC: *pnt = 'i'; break; // � i-grave
        case 0xEF: *pnt = 'i'; break; // � i-dieresis
        case 0xEE: *pnt = 'i'; break; // � i-circumflex

        case 0xF3: *pnt = 'o'; break; // � o-aigu
        case 0xF2: *pnt = 'o'; break; // � o-grave
        case 0xF6: *pnt = 'o'; break; // � o-dieresis
        case 0xF4: *pnt = 'o'; break; // � o-circumflex
        case 0xF5: *pnt = 'o'; break; // � o-tilde

        case 0xFA: *pnt = 'u'; break; // � u-aigu
        case 0xF9: *pnt = 'u'; break; // � u-grave
        case 0xFC: *pnt = 'u'; break; // � u-dieresis
        case 0xFB: *pnt = 'u'; break; // � u-circumflex

        case 0xE7: *pnt = 'c'; break; // � c-cedilla
        case 0xF1: *pnt = 'n'; break; // � n-tilde 
        default:               break; // DO NOTHING!!
      }
    }
  }
}
