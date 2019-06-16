////////////////////////////////////////////////////////////////////////
//
// File: SQLConnectionsDlg.cpp
//
// Copyright (c) 1998-2018 ir. W.E. Huisman
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
// Last Revision:  15-06-2019
// Version number: 1.5.5
//
#include "pch.h"
#include "framework.h"
#include "SQLConnections.h"
#include "SQLConnectionsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CSQLConnectionsDlg dialog

SQLConnectionsDlg::SQLConnectionsDlg(CWnd* pParent /*=nullptr*/)
	                 :CDialog(IDD_SQLCONNECTIONS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void SQLConnectionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_LIST,      m_list);
  DDX_Text   (pDX, IDC_NAME,      m_connectionName);
  DDX_Text   (pDX, IDC_DATASOURCE,m_datasource);
  DDX_Text   (pDX, IDC_USER,      m_username);
  DDX_Text   (pDX, IDC_PASSWORD1, m_password1);
  DDX_Text   (pDX, IDC_PASSWORD2, m_password2);
  DDX_Text   (pDX, IDC_OPTIONS,   m_options);
  DDX_Control(pDX, IDC_NEW,       m_new);
  DDX_Control(pDX, IDC_SAVE,      m_save);
  DDX_Control(pDX, IDC_DELETE,    m_delete);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    SQLConnection* conn = m_connections.GetConnection(m_connectionName);

    m_new.EnableWindow(conn == nullptr);
    m_save.EnableWindow(conn != nullptr);
    m_delete.EnableWindow(conn != nullptr);
  }
}

BEGIN_MESSAGE_MAP(SQLConnectionsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST,  &SQLConnectionsDlg::OnLvnItemchangedList)
  ON_EN_CHANGE(IDC_NAME,                &SQLConnectionsDlg::OnEnChangeName)
  ON_EN_CHANGE(IDC_DATASOURCE,          &SQLConnectionsDlg::OnEnChangeDatasource)
  ON_EN_CHANGE(IDC_USER,                &SQLConnectionsDlg::OnEnChangeUser)
  ON_EN_CHANGE(IDC_PASSWORD1,           &SQLConnectionsDlg::OnEnChangePassword1)
  ON_EN_CHANGE(IDC_PASSWORD2,           &SQLConnectionsDlg::OnEnChangePassword2)
  ON_EN_CHANGE(IDC_OPTIONS,             &SQLConnectionsDlg::OnEnChangeOptions)
  ON_BN_CLICKED(IDC_NEW,                &SQLConnectionsDlg::OnBnClickedNew)
  ON_BN_CLICKED(IDC_SAVE,               &SQLConnectionsDlg::OnBnClickedSave)
  ON_BN_CLICKED(IDC_DELETE,             &SQLConnectionsDlg::OnBnClickedDelete)
  ON_BN_CLICKED(IDCANCEL,               &SQLConnectionsDlg::OnBnClickedCancel)
  ON_BN_CLICKED(IDOK,                   &SQLConnectionsDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CSQLConnectionsDlg message handlers

BOOL SQLConnectionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

  // Init the connections list
  m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EDITLABELS);
  m_list.InsertColumn(0, "Connection", LVCFMT_LEFT, 190);

  // Read in the complete list
  LoadConnections();

  // Set focus to the list, don't automatically set the focus
  m_list.SetFocus();
  return FALSE;
}

void
SQLConnectionsDlg::LoadConnections()
{
  m_connections.LoadConnectionsFile();
  unsigned index = 0;

  SQLConnection* conn = m_connections.GetConnection(index);
  while(conn)
  {
    m_list.InsertItem(LVIF_TEXT|LVIF_STATE,index+1,conn->m_name,0,0,0,0);

    // Next connection
    conn = m_connections.GetConnection(++index);
  }

  if(index)
  {
    // Set focus on first line of the list
    m_list.SetSelectionMark(0);
    m_list.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);

    // Display all fields
    LoadCurrentConnection();
  }
}

bool
SQLConnectionsDlg::SaveConnections()
{
  if(!m_connections.SaveConnectionsFile())
  {
    AfxMessageBox("Could **NOT** save the 'database.xml' file",MB_OK|MB_ICONERROR);
    return false;
  }
  m_changed = false;
  return true;
}

void 
SQLConnectionsDlg::LoadCurrentConnection(int p_index /*=-1*/)
{
  if(p_index < 0)
  {
    p_index = m_list.GetSelectionMark();
  }
  CString name = m_list.GetItemText(p_index,0);

  SQLConnection* conn = m_connections.GetConnection(name);
  if (conn)
  {
    m_connectionName  = conn->m_name;
    m_datasource      = conn->m_datasource;
    m_username        = conn->m_username;
    m_password1       = conn->m_password;
    m_password2       = conn->m_password;
    m_options         = conn->m_options;

    UpdateData(FALSE);
  }
}

void
SQLConnectionsDlg::SaveCurrentConnection()
{
  SQLConnection* conn = m_connections.GetConnection(m_connectionName);
  if(conn)
  {
    conn->m_datasource = m_datasource;
    conn->m_username   = m_username;
    conn->m_password   = m_password1;
    conn->m_options    = m_options;
  }
}

bool
SQLConnectionsDlg::DropCurrentConnection()
{
  SQLConnection* conn = m_connections.GetConnection(m_connectionName);
  if(conn)
  {
    // Remove from the connections registration
    m_connections.DelConnection(conn->m_name);

    // Remove from the list;
    for(int index = 0; index < m_list.GetItemCount(); ++index)
    {
      CString name = m_list.GetItemText(index,0);
      if(name == m_connectionName)
      {
        m_list.DeleteItem(index);
        break;
      }
    }

    // Clean up all the fields
    m_connectionName.Empty();
    m_datasource.Empty();
    m_username.Empty();
    m_password1.Empty();
    m_password2.Empty();
    m_options.Empty();

    // Tell it the dialog
    UpdateData(FALSE);
    return true;
  }
  return false;
}

bool
SQLConnectionsDlg::MakeCurrentConnection()
{
  bool result = m_connections.AddConnection(m_connectionName,m_datasource,m_username,m_password1,m_options);
  if(result)
  {
    int index = m_list.GetItemCount();
    m_list.InsertItem(LVIF_TEXT|LVIF_STATE,index,m_connectionName,0,0,0,0);
    m_list.SetSelectionMark(index);
  }
  return result;
}

bool
SQLConnectionsDlg::SanityChecks()
{
  CString errors;

  if(m_datasource.IsEmpty()) errors += "You must provide a datasource name.\n";
  if(m_username  .IsEmpty()) errors += "You must provide a user name.\n";
  if(m_password1.IsEmpty())  errors += "You must provide a password.\n";
  if(m_password1.Compare(m_password2)) errors += "The passwords do **NOT** match.\n";

  if(!errors.IsEmpty())
  {
    AfxMessageBox(errors,MB_OK|MB_ICONERROR);
    return false;
  }
  return true;
}

void 
SQLConnectionsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void SQLConnectionsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width()  - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR SQLConnectionsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void 
SQLConnectionsDlg::OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

  if(pNMLV->iItem >= 0 && pNMLV->uNewState == 3)
  {
    LoadCurrentConnection(pNMLV->iItem);
  }
  *pResult = 0;
}

void 
SQLConnectionsDlg::OnEnChangeName()
{
  UpdateData();
  UpdateData(FALSE);
}

void 
SQLConnectionsDlg::OnEnChangeDatasource()
{
  UpdateData();
  UpdateData(FALSE);
}

void 
SQLConnectionsDlg::OnEnChangeUser()
{
  UpdateData();
  UpdateData(FALSE);
}

void 
SQLConnectionsDlg::OnEnChangePassword1()
{
  UpdateData();
  UpdateData(FALSE);
}

void 
SQLConnectionsDlg::OnEnChangePassword2()
{
  UpdateData();
  UpdateData(FALSE);
}

void 
SQLConnectionsDlg::OnEnChangeOptions()
{
  UpdateData();
  UpdateData(FALSE);
}

void 
SQLConnectionsDlg::OnBnClickedNew()
{
  if(SanityChecks() && MakeCurrentConnection())
  {
    m_changed = true;
  }
}

void 
SQLConnectionsDlg::OnBnClickedSave()
{
  if(SanityChecks())
  {
    SaveCurrentConnection();
    m_changed = true;
  }
}

void 
SQLConnectionsDlg::OnBnClickedDelete()
{
  if(DropCurrentConnection())
  {
    m_changed = true;
  }
}

void 
SQLConnectionsDlg::OnBnClickedCancel()
{
  if(m_changed && AfxMessageBox("CANCEL: Changes where made. Are you sure?", MB_YESNO | MB_DEFBUTTON2) == IDNO)
  {
    return;
  }
  CDialog::OnCancel();
}

void 
SQLConnectionsDlg::OnBnClickedOk()
{
  if(m_changed && SaveConnections() == false)
  {
    return;
  }
  CDialog::OnOK();
}
