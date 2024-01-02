////////////////////////////////////////////////////////////////////////
//
// File: SQLConnectionsDlg.h
//
// Copyright (c) 1998-2024 ir. W.E. Huisman
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
#include "SQLComponents\SQLConnections.h"
#include <SQLDriverManager.h>

// CSQLConnectionsDlg dialog
class SQLConnectionsDlg : public CDialog
{
// Construction
public:
	SQLConnectionsDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SQLCONNECTIONS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	void SetupDynamicLayout();
	// DDX/DDV support
  void LoadDataSources();
  void LoadConnections();
  bool SaveConnections();
  void MakeCurrentConnection();
  void LoadConnection(int p_index);
  void SaveConnection(int p_index);
  bool DropCurrentConnection();
  bool SanityChecks(int p_index);
  void TestTheConnection();
  void FillDatasources(DataSources& sources);

// Implementation
protected:
	HICON     m_hIcon;

  CListCtrl  m_list;
  CComboBox  m_comboDatasource;
  CButton    m_test;
  CButton    m_new;
  CButton    m_validate;
  CButton    m_delete;

  CButton    m_buttonOK;
  CButton    m_buttonCancel;

  XString   m_connectionName;
  XString   m_datasource;
  XString   m_username;
  XString   m_password1;
  XString   m_password2;
  XString   m_options;

  CEdit      m_editConnectionName;
  CEdit      m_editUsername;
  CEdit      m_editPassword1;
  CEdit      m_editPassword2;
  CEdit      m_editOptions;

  SQLConnections m_connections;
  bool           m_changed { false };
  bool           m_selectUpdate  { true };
  int            m_selection { -1 };

	// Generated message map functions
	virtual BOOL OnInitDialog();
  virtual void Cleanup();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

public:
  afx_msg void OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnEnChangeName();
  afx_msg void OnCbnCloseupDatasource();
  afx_msg void OnEnChangeUser();
  afx_msg void OnEnChangePassword1();
  afx_msg void OnEnChangePassword2();
  afx_msg void OnEnChangeOptions();
  afx_msg void OnBnClickedTest();
  afx_msg void OnBnClickedNew();
  afx_msg void OnBnClickedValidate();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedOk();
};
