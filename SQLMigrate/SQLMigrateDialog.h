////////////////////////////////////////////////////////////////////////
//
// File: SQLMigrateDialog.h
//
// Copyright (c) 1998-2022 ir. W.E. Huisman
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
#include "MigrateParameters.h"
#include "SQLMigrate.h"
#include "Logging.h"
#include "resource.h"
#include "version.h"

class SQLMigrateDialog : public CDialog
{
public:
  SQLMigrateDialog(CWnd* pParent = NULL);
 ~SQLMigrateDialog();
  BOOL  AddLogLine(XString msg);
  void  SetTableGauge (int num,int maxnum);
  void  SetTablesGauge(int num,int maxnum);
  void  SetSourceType(XString p_type);
  void  SetTargetType(XString p_type);
  void  SetStatus(XString status);
  void  LoadProfile(XString initFile);
  void  SaveProfile(XString initFile);
  void  Closing(bool p_success);

  enum { IDD = IDD_MIGRATE_WIZARD};

  CEdit     m_directory;
  CComboBox m_source_dsn;  
  CEdit     m_source_user;
  CEdit     m_source_password;

  CComboBox m_target_dsn;
  CEdit     m_target_user;
  CEdit     m_target_password;

  CEdit     m_source_schema;
  CEdit     m_target_schema;
  CEdit     m_tablespace;

  CComboBox m_directMigration;
  CEdit     m_dropscript;
  CEdit     m_createscript;

  CProgressCtrl m_table_gauge;
  CProgressCtrl m_tables_gauge;

  clock_t m_start;
  XString m_estimated;

  BOOL m_toLogfile;
  BOOL m_allTables;        //

  CEdit m_log;
  CEdit m_editTable;
  CEdit m_status;
  CEdit m_minOid;
  CEdit m_logPerRow;

  BOOL  m_do_tables;
  BOOL  m_do_views;
  BOOL  m_do_data;
  BOOL  m_do_truncate;
  BOOL  m_do_deletes;
  BOOL  m_do_primarys;
  BOOL  m_do_indices;
  BOOL  m_do_foreigns;
  BOOL  m_do_sequences;
  BOOL  m_do_triggers;
  BOOL  m_do_access;

  // Working objects
  MigrateParameters m_parameters;
  Logging           m_logfile;

  void HandleMessages();

protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  void SetComboBoxes();
  void SetDirectory();
  void EstimateRemainingTime(int p_num,int p_maxnum);

  // Migration
  void GetMigrationParameters();
  void PerformMigration();
  void PostMigration();

  HICON m_hIcon;
  virtual BOOL OnInitDialog();
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();

  afx_msg void OnDirectory();
  afx_msg void OnClosing();
  afx_msg void OnAllTables();
  afx_msg void OnDirectMigration();
  afx_msg void OnToLogfile();
  afx_msg void OnEnKillfocusSourceUser();
  afx_msg void OnEnKillfocusTargetUser();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnMigrate();
  afx_msg void OnCancel();

  DECLARE_MESSAGE_MAP()

private:
  CFont*  m_font;
  XString m_profile;

  bool    m_exportRunning;
  bool    m_commandLineMode;
  bool    m_exportResult;
};
