////////////////////////////////////////////////////////////////////////
//
// File: SQLConnectionsDlg.cpp
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
#include "pch.h"
#include "framework.h"
#include "SQLConnections.h"
#include "SQLConnectionsDlg.h"
#include <SQLDatabase.h>
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
	CButton m_buttonOK;

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

	DDX_Control(pDX, IDOK, m_buttonOK);
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

  DDX_Control(pDX, IDC_DATASOURCE, m_comboDatasource);
  DDX_Control(pDX, IDC_LIST,       m_list);
  DDX_Control(pDX, IDC_NAME,       m_editConnectionName);
  DDX_Control(pDX, IDC_USER,       m_editUsername);
  DDX_Control(pDX, IDC_PASSWORD1,  m_editPassword1);
  DDX_Control(pDX, IDC_PASSWORD2,  m_editPassword2);
  DDX_Control(pDX, IDC_OPTIONS,    m_editOptions);
  DDX_Control(pDX, IDC_TEST,       m_test);
  DDX_Control(pDX, IDC_NEW,        m_new);
  DDX_Control(pDX, IDC_VALIDATE,   m_validate);
  DDX_Control(pDX, IDC_DELETE,     m_delete);
  DDX_Control(pDX, IDOK,           m_buttonOK);
  DDX_Control(pDX, IDCANCEL,       m_buttonCancel);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    m_editConnectionName.SetWindowText(m_connectionName);
    m_editUsername. SetWindowText(m_username);
    m_editPassword1.SetWindowText(m_password1);
    m_editPassword2.SetWindowText(m_password2);
    m_editOptions  .SetWindowText(m_options);

    // Enable buttons
    m_validate.EnableWindow(m_selection >= 0);
    m_delete  .EnableWindow(m_selection >= 0);
  }
  else
  {
    m_editConnectionName.GetWindowText(m_connectionName);
    m_editUsername. GetWindowText(m_username);
    m_editPassword1.GetWindowText(m_password1);
    m_editPassword2.GetWindowText(m_password2);
    m_editOptions  .GetWindowText(m_options);
  }
}

BEGIN_MESSAGE_MAP(SQLConnectionsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST,  &SQLConnectionsDlg::OnLvnItemchangedList)
  ON_EN_CHANGE(IDC_NAME,                &SQLConnectionsDlg::OnEnChangeName)
  ON_CBN_SELCHANGE(IDC_DATASOURCE,      &SQLConnectionsDlg::OnCbnCloseupDatasource)
  ON_CBN_CLOSEUP(IDC_DATASOURCE,        &SQLConnectionsDlg::OnCbnCloseupDatasource)
  ON_EN_CHANGE(IDC_USER,                &SQLConnectionsDlg::OnEnChangeUser)
  ON_EN_CHANGE(IDC_PASSWORD1,           &SQLConnectionsDlg::OnEnChangePassword1)
  ON_EN_CHANGE(IDC_PASSWORD2,           &SQLConnectionsDlg::OnEnChangePassword2)
  ON_EN_CHANGE(IDC_OPTIONS,             &SQLConnectionsDlg::OnEnChangeOptions)
  ON_BN_CLICKED(IDC_TEST,               &SQLConnectionsDlg::OnBnClickedTest)
  ON_BN_CLICKED(IDC_NEW,                &SQLConnectionsDlg::OnBnClickedNew)
	ON_BN_CLICKED(IDC_VALIDATE,           &SQLConnectionsDlg::OnBnClickedValidate)
  ON_BN_CLICKED(IDC_DELETE,             &SQLConnectionsDlg::OnBnClickedDelete)
  ON_BN_CLICKED(IDCANCEL,               &SQLConnectionsDlg::OnBnClickedCancel)
  ON_BN_CLICKED(IDOK,                   &SQLConnectionsDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CSQLConnectionsDlg message handlers

BOOL SQLConnectionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(_T("SQL Connections"));

  // Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		XString strAboutMenu;
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
  m_list.InsertColumn(0, _T("Connection"),LVCFMT_LEFT,180);

  // Load Datasource names
  LoadDataSources();

  // Read in the complete list
  LoadConnections();

  // Set focus to the list, don't automatically set the focus
  m_list.SetFocus();
	UpdateData(FALSE);
  return FALSE;
}

void SQLConnectionsDlg::Cleanup()
{
	// Cleanup itemdata
	for (int i = 0; i < m_list.GetItemCount(); ++i)
	{
		SQLConnection* conn = (SQLConnection*)m_list.GetItemData(i);
		delete conn;
	}
}

// Triggered by SetCanResize
void
SQLConnectionsDlg::SetupDynamicLayout()
{
  auto manager = GetDynamicLayout();
  if(manager == nullptr)
  {
    EnableDynamicLayout();
    manager = GetDynamicLayout();
  }
  if(manager != nullptr)
  {
    manager->Create(this);
    LoadDynamicLayoutResource(m_lpszTemplateName);

    manager->AddItem(IDCANCEL,CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
		manager->AddItem(IDOK,    CMFCDynamicLayout::MoveHorizontal(100), CMFCDynamicLayout::SizeNone());
	}
}

void
SQLConnectionsDlg::LoadDataSources()
{
  SQLDriverManager manager;
  DataSources      sources;

  m_comboDatasource.ResetContent();

  // First: Get all the system data sources
  manager.GetDataSources(sources,SQL_FETCH_FIRST_SYSTEM);
  FillDatasources(sources);

  // clear the list
  sources.clear();

  // Secondly: get all the user data sources
  manager.GetDataSources(sources,SQL_FETCH_FIRST_USER);
  FillDatasources(sources);

  // Show first datasource found
  m_comboDatasource.SetCurSel(0);
  m_comboDatasource.GetLBText(0,m_datasource);
}

void 
SQLConnectionsDlg::FillDatasources(DataSources& sources)
{
  for(auto& source : sources)
  {
    m_comboDatasource.AddString(source.m_datasource);
  }
}

void
SQLConnectionsDlg::LoadConnections()
{
  m_connections.LoadConnectionsFile();
  unsigned index = 0;

	m_selectUpdate = false;
  SQLConnection* conn = m_connections.GetConnection(index);
  while(conn)
  {
		int item = m_list.InsertItem(LVIF_TEXT | LVIF_STATE, index++, conn->m_name, 0, 0, 0, 0);
		if (item >= 0)
		{
			SQLConnection* data = new SQLConnection;
			data->m_datasource = conn->m_datasource;
			data->m_name       = conn->m_name;
			data->m_options    = conn->m_options;
			data->m_password   = conn->m_password;
			data->m_username   = conn->m_username;
			m_list.SetItemData(item, (DWORD_PTR)data);
		}

    // Next connection
		conn = m_connections.GetConnection(index);
  }
	m_selectUpdate = true;
	if (index > 0)
  {
		// Display all fields
		LoadConnection(0);

    // Set focus on first line of the list
    m_list.SetSelectionMark(0);
    m_list.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
  }
}

bool
SQLConnectionsDlg::SaveConnections()
{
	SaveConnection(m_selection);
	
	// Validate all connections
	for (int i = 0; i < m_list.GetItemCount(); ++i)
	{
		// Display all fields
		LoadConnection(i);

		if (!SanityChecks(i))
		{
			// Set focus on first line of the list
			m_list.SetSelectionMark(i);
			m_list.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			return false;
		}
	}
	
	// Refill m_connections
	m_connections.Reset();
	for (int i = 0; i < m_list.GetItemCount(); ++i)
	{
		SQLConnection* conn = (SQLConnection*)m_list.GetItemData(i);
		m_connections.AddConnection(conn->m_name, conn->m_datasource, conn->m_username, conn->m_password, conn->m_options);
	}

	// Save m_connections
  if(!m_connections.SaveConnectionsFile())
  {
		::MessageBox(GetSafeHwnd(),_T("Could **NOT** save the 'database.xml' file"),_T("Error"),MB_OK | MB_ICONERROR);
    return false;
  }
  m_changed = false;
  return true;
}

void 
SQLConnectionsDlg::LoadConnection(int p_index)
{
	SQLConnection* conn = (SQLConnection*)m_list.GetItemData(p_index);

    m_connectionName  = conn->m_name;
    m_datasource      = conn->m_datasource;
    m_username        = conn->m_username;
    m_password1       = conn->m_password;
    m_password2       = conn->m_password;
    m_options         = conn->m_options;

    int source = m_comboDatasource.FindStringExact(0,m_datasource);
    if(source >= 0)
    {
      m_comboDatasource.SetCurSel(source);
    }
    UpdateData(FALSE);
	m_selection = p_index;
}

void
SQLConnectionsDlg::SaveConnection(int p_index)
{
	if (p_index >= 0 && p_index < m_list.GetItemCount())
  {
		m_selectUpdate = false;
		m_list.SetItemText(p_index, 0, m_connectionName);
		m_selectUpdate = true;
		SQLConnection* conn = (SQLConnection*)m_list.GetItemData(p_index);

		conn->m_name       = m_connectionName;
    conn->m_datasource = m_datasource;
    conn->m_username   = m_username;
    conn->m_password   = m_password1;
    conn->m_options    = m_options;
  }
}

bool
SQLConnectionsDlg::DropCurrentConnection()
{
	if (m_selection >= 0)
  {
		m_selectUpdate = false;
	  SQLConnection* conn = (SQLConnection*)m_list.GetItemData(m_selection);
		delete conn;

    // Clean up all the fields
    m_connectionName.Empty();
    m_username.Empty();
    m_password1.Empty();
    m_password2.Empty();
    m_options.Empty();

    // Tell it the dialog
    UpdateData(FALSE);

		// Remove from the list;
		m_list.DeleteItem(m_selection);
		if (m_list.GetItemCount() > 0)
		{
			m_list.SetSelectionMark(0);
			m_list.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			LoadConnection(0);
		}
		else
		{
			m_selection = -1;
		}
		m_selectUpdate = true;
    return true;
  }
  return false;
}

void
SQLConnectionsDlg::MakeCurrentConnection()
{
	m_selectUpdate = false;
	int index = m_list.InsertItem(LVIF_TEXT | LVIF_STATE, m_list.GetItemCount(), m_connectionName, 0, 0, 0, 0);
	if (index >= 0)
	{
		SQLConnection* data = new SQLConnection;
		data->m_datasource = m_datasource;
		data->m_name = m_connectionName;
		data->m_options = m_options;
		data->m_password = m_password1;
		data->m_username = m_username;
		m_list.SetItemData(index, (DWORD_PTR)data);
		m_changed = true;
    m_list.SetSelectionMark(index);
		m_list.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_selection = index;
  }
	UpdateData(FALSE);
	m_selectUpdate = true;
}

bool
SQLConnectionsDlg::SanityChecks(int p_index)
{
  XString errors;

	for (int i = 0; i < m_list.GetItemCount(); ++i)
	{
		if (i != p_index)
		{
			SQLConnection* conn = (SQLConnection*)m_list.GetItemData(i);
			if (m_connectionName.CompareNoCase(conn->m_name) == 0)
			{
				errors += m_connectionName + _T(" already exists.\n");
				break;
			}
		}
	}

	if(m_connectionName.IsEmpty()) errors += _T("You must provide a connection name.\n");
  if(m_datasource.IsEmpty())     errors += _T("You must provide a datasource name.\n");
  if(m_username  .IsEmpty())     errors += _T("You must provide a user name.\n");
  if(m_password1.IsEmpty())      errors += _T("You must provide a password.\n");
	if(m_password1.Compare(m_password2)) errors += _T("The passwords do **NOT** match.\n");

  if(!errors.IsEmpty())
  {
		::MessageBox(GetSafeHwnd(),errors,_T("Error"), MB_OK | MB_ICONERROR);
    return false;
  }
  return true;
}

void
SQLConnectionsDlg::TestTheConnection()
{
  XString error;

  try
  {
    SQLDatabase database;
    if(database.Open(m_datasource,m_username,m_password1))
    {
      XString message;
      message.Format(_T("The connection with [%s] as user [%s] has succeeded!\n\n")
                     _T("The 'true' database name is: %s\n")
                     _T("This database is a [%s:%s] database\n")
                     _T("Vendors ODBC driver version is: %s")
                    ,m_datasource
                    ,m_username
                    ,database.GetDatabaseName()
                    ,database.GetDBVendorName()
                    ,database.GetDBVendorVersion()
                    ,database.GetDBVendorDriverVersion());
			::MessageBox(GetSafeHwnd(),message,_T("Melding"),MB_OK);
      database.Close();
    }
    else
    {
			::MessageBox(GetSafeHwnd(),_T("We are sorry: **NO** connection made!!"),_T("No Connection"), MB_OK | MB_ICONERROR);
    }
  }
  catch(StdException& ex)
  {
    error = ex.GetErrorMessage();
  }
  catch(XString& err)
  {
    error = err;
  }

  // Error handling needed?
  if(!error.IsEmpty())
  {
    ::MessageBox(GetSafeHwnd(),_T("ERROR while opening database: ") + error,_T("Error"), MB_OK | MB_ICONERROR);
  }
}

void 
SQLConnectionsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if((nID & 0xFFF0) == IDM_ABOUTBOX)
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
	if (m_selectUpdate)
	{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		if (pNMLV->iItem >= 0)
		{
			//if (pNMLV->uNewState == 0 && pNMLV->uOldState == LVIS_SELECTED)
			//{
			//	SaveConnection(pNMLV->iItem);
			//}
			if (pNMLV->uNewState == (LVIS_FOCUSED | LVIS_SELECTED))
  {
				SaveConnection(m_selection);
				LoadConnection(pNMLV->iItem);
			}
		}
  }
  *pResult = 0;
}

void 
SQLConnectionsDlg::OnEnChangeName()
{
  XString name = m_connectionName;
  UpdateData();
  if(name.Compare(m_connectionName))
  {
    m_changed = true;
  }
}

void 
SQLConnectionsDlg::OnCbnCloseupDatasource()
{
  int ind = m_comboDatasource.GetCurSel();
  if(ind >= 0)
  {
    XString datasource;
    m_comboDatasource.GetLBText(ind,datasource);
    if(datasource.Compare(m_datasource))
    {
      m_datasource = datasource;
      m_changed = true;
    }
  }
  UpdateData(FALSE);
}

void 
SQLConnectionsDlg::OnEnChangeUser()
{
  XString user = m_username;
  UpdateData();
  if(user.Compare(m_username))
  {
    m_changed = true;
  }
}

void 
SQLConnectionsDlg::OnEnChangePassword1()
{
  XString password = m_password1;
  UpdateData();
  if (password.Compare(m_password1))
  {
    m_changed = true;
  }
}

void 
SQLConnectionsDlg::OnEnChangePassword2()
{
  XString password = m_password2;
  UpdateData();
  if (password.Compare(m_password2))
  {
    m_changed = true;
  }
}

void 
SQLConnectionsDlg::OnEnChangeOptions()
{
  XString options = m_options;
  UpdateData();
  if (options.Compare(m_options))
  {
    m_changed = true;
  }
}

void
SQLConnectionsDlg::OnBnClickedTest()
{
	if (SanityChecks(m_selection))
  {
    TestTheConnection();
  }
}

void 
SQLConnectionsDlg::OnBnClickedNew()
{
	SaveConnection(m_selection);

	// Clean up all the fields
	m_connectionName = _T("New connection");
	m_username.Empty();
	m_password1.Empty();
	m_password2.Empty();
	m_options.Empty();
	MakeCurrentConnection();

	// Tell it the dialog
	UpdateData(FALSE);
}

void 
SQLConnectionsDlg::OnBnClickedValidate()
  {
	SanityChecks(m_selection);
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
	if (m_changed && ::MessageBox(GetSafeHwnd(),_T("CANCEL: Changes where made. Are you sure?"),_T("Changes were made"), MB_YESNO | MB_DEFBUTTON2) == IDNO)
  {
    return;
  }
	Cleanup();
	CDialog::OnCancel();
}

void 
SQLConnectionsDlg::OnBnClickedOk()
{
  if(m_changed && SaveConnections() == false)
  {
    return;
  }
	Cleanup();
	CDialog::OnOK();
}
