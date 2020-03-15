////////////////////////////////////////////////////////////////////////
//
// File: SQLConnectionsDlg.h
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
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
          void LoadDataSources();
          void LoadConnections();
          bool SaveConnections();
          bool MakeCurrentConnection();
          void LoadCurrentConnection(int p_index = -1);
          void SaveCurrentConnection();
          bool DropCurrentConnection();
          bool SanityChecks();
          void TestTheConnection();
          void FillDatasources(DataSources& sources);

// Implementation
protected:
	HICON     m_hIcon;
  CListCtrl m_list;
  CComboBox m_comboDatasource;
  CButton   m_test;
  CButton   m_new;
  CButton   m_save;
  CButton   m_delete;


  CString   m_connectionName;
  CString   m_datasource;
  CString   m_username;
  CString   m_password1;
  CString   m_password2;
  CString   m_options;

  SQLConnections m_connections;
  bool           m_changed { false };

	// Generated message map functions
	virtual BOOL OnInitDialog();
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
  afx_msg void OnBnClickedSave();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedOk();
};
