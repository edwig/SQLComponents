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
#pragma once
#include "Parameters.h"
#include <SQLComponents.h>
#include <SQLVariant.h>
#include <vector>

using namespace SQLComponents;

#define SEPARATOR_STRING  1
#define SEPERATOR_DATA    2

class XPort;

// List of objects (strings)
typedef std::vector<XString> OList;

class XFile
{
public:
  XFile(Parameters& p_parameters);
 ~XFile();

  bool    OpenCreate(XString p_filename,XString p_databaseType);
  bool    OpenRead  (XString p_filename,XString p_databaseType,bool p_listonly);
  bool    Close();

  // Writing to the export file
  void    WriteSection(XString p_name);
  void    WriteTable(XString p_table);
  void    WriteColumns(OList* p_columns);
  void    WriteIndex(XString p_index);
  void    WriteConstraint(int p_num,XString p_table,XString p_constraint);
  void    WriteView(XString p_view);
  void    WriteSequence(XString p_sequence);
  void    WriteProcedure(XString p_procedure);
  void    WriteSQL(XString p_sql);
  void    WriteSynonym(XString p_type,XString p_name);
  bool    WriteRows(XPort& p_xport,XString& p_table,bool p_export);
  void    WriteSectionEnd();
  void    WriteEndExport();
  void    Flush();

  // Reading from the export file
  _TUCHAR NextType();
  XString ReadSection();
  XString ReadTable(bool p_doNewline);
  void    ReadColumns(OList* p_columns);
  XString ReadIndex();
  XString ReadConstraint();
  XString ReadView();
  XString ReadSequence();
  XString ReadProcedure();
  bool    ReadSynonym(XString& p_type,XString& p_name);
  XString ReadSQL();
  bool    ReadRows(XPort& p_xport,XString& p_table,bool p_object,bool p_list);
  bool    ReadEnd();
  void    ReadEndExport();

private:
  // Private objects occurring multiple times
  void    WriteHeader(XString p_type);
  bool    ReadHeader(XString& p_type);
  void    WriteString(TCHAR p_type,XString& p_string);
  bool    ReadString(TCHAR& p_type,XString& p_string);
  void    WriteObject(TCHAR p_type,int  p_length);
  bool    ReadObject(TCHAR& p_type,int& p_length);
  void    WriteData(SQLComponents::SQLVariant* p_var);
  bool    ReadData (SQLComponents::SQLVariant* p_var);
  void    StripDiacritics(_TUCHAR* p_buffer);

  // Data
  FILE*       m_file { nullptr };
  Parameters& m_parameters;
};