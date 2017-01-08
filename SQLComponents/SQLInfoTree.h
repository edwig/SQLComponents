////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoTree.h
//
// Copyright (c) 1998-2017 ir. W.E. Huisman
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
// Last Revision:   08-01-2017
// Version number:  1.4.0
//
#pragma once
#include "SQLInfo.h"

namespace SQLComponents
{

class SQLInfoTree : public SQLInfo
{
public:
   SQLInfoTree(SQLDatabase* p_database);
  ~SQLInfoTree();

#ifdef SQL_COMPONENTS_MFC
  // All ODBC Functions to the tree
  void MakeTreeInfo (CTreeCtrl* tree);
  // All table info to a tree control
  void MakeTableInfo(CTreeCtrl* tree,CString& table);
  // All procedure info to a tree control
  bool MakeProcedureInfo(CTreeCtrl* tree,CString& procedure);
  // Report all capabilities to a system file
  virtual void ReportAllCapabilities(CTreeCtrl* p_tree,CString& filename);

private:
  CString   InfoNumberToString(SQLINTEGER num);
  HTREEITEM DisplayBitfield(CTreeCtrl*  tree
                           ,HTREEITEM   item
                           ,SQLUINTEGER var
                           ,SQLUINTEGER macro
                           ,char*       text);
  // Helper for displaying SQLGetInfo information
  void DisplayConversion(CTreeCtrl* tree 
                        ,HTREEITEM  item
                        ,CString    type
                        ,SQLINTEGER totype);
  // Make ODBC Extensions in a tree control
  void MakeTreeODBCExtensions(CTreeCtrl* tree,HTREEITEM item);
  // All ODBC Info to a tree control
  void MakeTreeInfoFunctions(CTreeCtrl* tree,HTREEITEM item);
  void WordListToTree(WordList& p_list,CTreeCtrl* p_tree,HTREEITEM p_item);

  // Printing the DBInfo tree to a report
  void ReportCapabilities(CTreeCtrl* p_tree
                         ,HTREEITEM  p_branch
                         ,int        p_level
                         ,FILE*      p_file);
#endif
};

// End of namespace
}