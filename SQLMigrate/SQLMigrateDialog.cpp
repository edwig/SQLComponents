////////////////////////////////////////////////////////////////////////
//
// File: SQLMigrateDialog.cpp
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
#include "stdafx.h"
#include "SQLMigrate.h"
#include "SQLMigrateApp.h"
#include "SQLMigrateDialog.h"
#include "MapDialog.h"
#include "AboutDlg.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SQLMigrateDialog::SQLMigrateDialog(CWnd* pParent)
                 :CDialog(SQLMigrateDialog::IDD, pParent)
                 ,m_migrate(nullptr)
                 ,m_font(nullptr)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_toLogfile       = false;
  m_allTables       = false;
  m_exportRunning   = false;
  m_commandLineMode = false;
  m_exportResult    = false;
  m_do_truncate     = false;
  m_do_deletes      = false;
  m_do_primarys     = false;
  m_do_indices      = false;
  m_do_sequences    = false;
  m_do_triggers     = false;
  m_do_access       = false;
}

SQLMigrateDialog::~SQLMigrateDialog()
{
  if(m_font)
  {
    delete m_font;
  }
}

void 
SQLMigrateDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_DIRECTORY,       m_directory);
  // Source database
  DDX_Control(pDX, IDC_SOURCE         , m_source_dsn);
  DDX_Control(pDX, IDC_SOURCE_USER    , m_source_user);
  DDX_Control(pDX, IDC_SOURCE_PASSWORD, m_source_password);
  // Target database
  DDX_Control(pDX, IDC_TARGET         , m_target_dsn);
  DDX_Control(pDX, IDC_TARGET_USER    , m_target_user);
  DDX_Control(pDX, IDC_TARGET_PASSWORD, m_target_password);
  // Schemata
  DDX_Control(pDX, IDC_SOURCE_SCHEMA,   m_source_schema);
  DDX_Control(pDX, IDC_TARGET_SCHEMA,   m_target_schema);
  // Migration
  DDX_Control(pDX, IDC_DIRECT_MIGRATION,m_directMigration);
  DDX_Check  (pDX, IDC_ALLTABLES,       m_allTables);
  DDX_Check  (pDX, IDC_TOLOGFILE,       m_toLogfile);
  // Convert to file
  DDX_Control(pDX, IDC_DROP_FILE,     m_dropscript);
  DDX_Control(pDX, IDC_CREATE_FILE,   m_createscript);
  DDX_Control(pDX, IDC_LOGLINES,      m_logPerRow);
  // If only one table migration
  DDX_Control(pDX, IDC_TABLE,         m_editTable);
  DDX_Control(pDX, IDC_MINOID,        m_minOid);
  // Progress and status
  DDX_Control(pDX, IDC_STATUS,        m_status);
  DDX_Control(pDX, IDC_PROGRESS1,     m_table_gauge);
  DDX_Control(pDX, IDC_PROGRESS2,     m_tables_gauge);
  // Options
  DDX_Check(pDX,IDC_DO_TABLES,        m_do_tables);
  DDX_Check(pDX,IDC_DO_DATA,          m_do_data);
  DDX_Check(pDX,IDC_DO_TRUNCATE,      m_do_truncate);
  DDX_Check(pDX,IDC_DO_DELETES,       m_do_deletes);
  DDX_Check(pDX,IDC_DO_PRIMARYS,      m_do_primarys);
  DDX_Check(pDX,IDC_DO_INDICES,       m_do_indices);
  DDX_Check(pDX,IDC_DO_FOREIGN,       m_do_foreigns);
  DDX_Check(pDX,IDC_DO_SEQUENCES,     m_do_sequences);
  DDX_Check(pDX,IDC_DO_TRIGGERS,      m_do_triggers);
  DDX_Check(pDX,IDC_DO_ACCESS,        m_do_access);

  DDX_Control(pDX, IDC_LOG,           m_log);
}

BEGIN_MESSAGE_MAP(SQLMigrateDialog, CDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED   (IDC_BUTT_DIR,        OnDirectory)
  ON_EN_KILLFOCUS (IDC_SOURCE_USER,     OnEnKillfocusSourceUser)
  ON_EN_KILLFOCUS (IDC_TARGET_USER,     OnEnKillfocusTargetUser)
  ON_CBN_CLOSEUP  (IDC_DIRECT_MIGRATION,OnDirectMigration)
  ON_BN_CLICKED   (IDC_ALLTABLES,       OnAllTables)
  ON_BN_CLICKED   (IDC_TOLOGFILE,       OnToLogfile)
  ON_BN_CLICKED   (IDC_EXPORT,          OnMigrate) 
  ON_BN_CLICKED   (IDC_CLOSE,           OnClosing)
END_MESSAGE_MAP()

BOOL 
SQLMigrateDialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT (IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL)
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
  // when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);			// Set big icon
  SetIcon(m_hIcon, FALSE);		// Set small icon

  SetDirectory();
  SetComboBoxes();

  // Reset gauges
  SetTableGauge(0,100);
  SetTablesGauge(0,100);

  // Action not edittable
  GetDlgItem(IDC_STATUS)->EnableWindow(FALSE);

  // Set fonttype of the log window
  m_font = new CFont();
  m_font->CreatePointFont(10,"Courier");
  m_log.SetFont(m_font);
  m_log.SetLimitText(0);

  m_toLogfile           = 0;
  m_allTables           = 0;
  m_parameters.v_direct = 0;

  m_do_tables    = 1;
  m_do_data      = 1;
  m_do_truncate  = 1;
  m_do_deletes   = 1;

  LPWSTR* argv;
  int argc;
  argv = CommandLineToArgvW(GetCommandLineW(), &argc);
  CString ini_file = "SQLMigrate.ini";
  if (argc == 2)
  {
    m_commandLineMode = true;
    ini_file = argv[1];
    HANDLE file_handle;
    file_handle = CreateFile(ini_file, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file_handle == INVALID_HANDLE_VALUE)
    {
      m_toLogfile = true;
      Logging log;
      log.Open();
      log.WriteLog("Absolute or relative pathname to SQLMigrate.ini is NOT correct!");
      log.WriteLog("End of SQLMigrate.");
      log.Close();
      Closing(false);
      return FALSE;
    }
  }
  GlobalFree(argv);

  if (argc > 2)
  {
    MessageBox("Wrong number of arguments on the command line");
  }
  // Load standard profile
  LoadProfile(ini_file);
  if (m_commandLineMode)
  {
    OnMigrate();
    Closing(m_exportResult);
  }

  // Disable for now
  m_minOid.EnableWindow(FALSE);

  // Name of the program + version
  SetWindowText(SQL_MIGRATE " " SQL_COMPONENTS_VERSION);

  return TRUE;  // return TRUE  unless you set the focus to a control
}

void 
SQLMigrateDialog::SetDirectory()
{
  char buffer[MAX_PATH + 1];
  _getcwd(buffer,MAX_PATH);
  SetDlgItemText(IDC_DIRECTORY,buffer);
}

void
SQLMigrateDialog::SetComboBoxes()
{
  // Getting datasources and adding to the combo boxes
  DSMap sources;
  SQLDatabase database;
  database.GetDataSources(sources);

  for(auto& datasource : sources)
  {
    m_source_dsn.AddString(datasource.m_datasource);
    m_target_dsn.AddString(datasource.m_datasource);
  }

  m_directMigration.AddString("Direct datapump migration");
  m_directMigration.AddString("SELECT/INSERT migration");
  m_directMigration.AddString("Write to scripts");
}

void 
SQLMigrateDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID & 0xFFF0) == IDM_ABOUTBOX)
  {
    AboutDlg dlgAbout;
    dlgAbout.DoModal();
  }
  else
  {
    CDialog::OnSysCommand(nID, lParam);
  }
}

void
SQLMigrateDialog::LoadProfile(CString initFile)
{
  char buffer[MAX_PATH+1];
  CString source_database;
  CString source_user;
  CString source_password;
  CString source_schema;
  CString target_database;
  CString target_user;
  CString target_password;
  CString target_schema;
  CString type_migration;
  CString type_level;
  CString type_space;
  CString file_create;
  CString file_drop;
  CString table_table;
  CString table_min;
  CString log_loglines;
  CString directory;
  int     file_direct;
  int     table_all;
  int     table_minoid;
  int     log_logging;
  int     log_log;

  CString workingDir("");
  if (initFile.Left(2).Right(1) == ":" || initFile.Left(1) == "\\")
  {
    // absolute pathname
    LPCTSTR path = initFile;
    char drive[_MAX_DRIVE];
    char dir  [_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext  [_MAX_EXT];
    _splitpath_s(path, drive, _MAX_DRIVE,dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);
    workingDir  = (CString)drive + (CString)dir;
    initFile = (CString)fname + (CString)ext;
  }
  else
  {
    // Find in the current working directory
    char curdir[MAX_PATH];
    _getcwd(curdir,MAX_PATH);
    workingDir = curdir;
    if(workingDir.Right(1) != '\\')
    {
      workingDir += "\\";
    }
  }
  m_profile = workingDir + initFile;

  // Source Database
  GetPrivateProfileString("SOURCE","database","<source-database>",buffer,MAX_PATH,m_profile); source_database = buffer;
  GetPrivateProfileString("SOURCE","user"    ,"user",             buffer,MAX_PATH,m_profile); source_user     = buffer;
  GetPrivateProfileString("SOURCE","password","password",         buffer,MAX_PATH,m_profile); source_password = buffer;
  GetPrivateProfileString("SOURCE","schema",  "",                 buffer,MAX_PATH,m_profile); source_schema   = buffer;

  SetDlgItemText(IDC_SOURCE,         source_database);
  SetDlgItemText(IDC_SOURCE_USER,    source_user);
  SetDlgItemText(IDC_SOURCE_PASSWORD,source_password);
  SetDlgItemText(IDC_SOURCE_SCHEMA,  source_schema);

  // Target Database
  GetPrivateProfileString("TARGET","database","<target-database>",buffer,MAX_PATH,m_profile); target_database  = buffer;
  GetPrivateProfileString("TARGET","user",    "user",             buffer,MAX_PATH,m_profile); target_user      = buffer;
  GetPrivateProfileString("TARGET","password","password",         buffer,MAX_PATH,m_profile); target_password  = buffer;
  GetPrivateProfileString("TARGET","schema",  "",                 buffer,MAX_PATH,m_profile); target_schema    = buffer;

  SetDlgItemText(IDC_TARGET,          target_database);
  SetDlgItemText(IDC_TARGET_USER,     target_user);
  SetDlgItemText(IDC_TARGET_PASSWORD, target_password);
  SetDlgItemText(IDC_TARGET_SCHEMA,   target_schema);

  // Type migration
  GetPrivateProfileString("TABLE","tablespace","",               buffer,MAX_PATH,m_profile); type_space     = buffer;
  SetDlgItemText(IDC_TABLESPACE,  type_space);

  // File
  GetPrivateProfileString("FILE","Directory","",              buffer,MAX_PATH,m_profile); directory   = buffer;
  GetPrivateProfileString("FILE","create",   "script.sql",    buffer,MAX_PATH,m_profile); file_create = buffer;
  GetPrivateProfileString("FILE","drop",     "dropscript.sql",buffer,MAX_PATH,m_profile); file_drop   = buffer;
  file_direct = GetPrivateProfileInt("FILE", "direct",1,m_profile);

  m_directMigration.SetCurSel(file_direct);
  SetDlgItemText(IDC_CREATE_FILE,file_create);
  SetDlgItemText(IDC_DROP_FILE,  file_drop);  
  OnDirectMigration();

  // Tables
  GetPrivateProfileString("TABLE","table","",buffer,MAX_PATH,m_profile); 
  table_table  = buffer;
  table_all    = GetPrivateProfileInt("TABLE","all",        1,   m_profile);
  table_minoid = GetPrivateProfileInt("TABLE","minoid",     0,   m_profile);
  if(table_minoid > 0)
  {
    table_min.Format("%d",table_minoid);
  }
  CheckDlgButton(IDC_ALLTABLES,   table_all);
  SetDlgItemText(IDC_TABLE,       table_table);
  SetDlgItemText(IDC_MINOID,      table_min);
  OnAllTables();

  // Log and log lines
  log_logging = GetPrivateProfileInt("LOG","logging",1,m_profile);
  log_log     = GetPrivateProfileInt("LOG","lines",1000,m_profile);
  if(log_log > 0)
  {
    log_loglines.Format("%d",log_log);
  }
  CheckDlgButton(IDC_TOLOGFILE,log_logging);
  SetDlgItemText(IDC_LOGLINES,log_loglines);

  // Options
  m_do_tables    = GetPrivateProfileInt("OPTION","tables",   1,m_profile);
  m_do_data      = GetPrivateProfileInt("OPTION","data",     1,m_profile);
  m_do_truncate  = GetPrivateProfileInt("OPTION","truncate", 1,m_profile);
  m_do_deletes   = GetPrivateProfileInt("OPTION","deletes",  1,m_profile);
  m_do_primarys  = GetPrivateProfileInt("OPTION","primarys", 0,m_profile);
  m_do_indices   = GetPrivateProfileInt("OPTION","indices",  0,m_profile);
  m_do_foreigns  = GetPrivateProfileInt("OPTION","foreigns", 0,m_profile);
  m_do_sequences = GetPrivateProfileInt("OPTION","sequences",0,m_profile);
  m_do_triggers  = GetPrivateProfileInt("OPTION","triggers", 0,m_profile);
  m_do_access    = GetPrivateProfileInt("OPTION","access",   0,m_profile);

  CheckDlgButton(IDC_DO_TABLES,   m_do_tables);
  CheckDlgButton(IDC_DO_DATA,     m_do_data);
  CheckDlgButton(IDC_DO_TRUNCATE, m_do_truncate);
  CheckDlgButton(IDC_DO_DELETES,  m_do_deletes);
  CheckDlgButton(IDC_DO_PRIMARYS, m_do_primarys);
  CheckDlgButton(IDC_DO_INDICES,  m_do_indices);
  CheckDlgButton(IDC_DO_FOREIGN,  m_do_foreigns);
  CheckDlgButton(IDC_DO_SEQUENCES,m_do_sequences);
  CheckDlgButton(IDC_DO_TRIGGERS, m_do_triggers);
  CheckDlgButton(IDC_DO_ACCESS,   m_do_access);

  // Post processing in the interface
  OnToLogfile();
}

void
SQLMigrateDialog::SaveProfile(CString initFile)
{
  CString source_database;
  CString source_user;
  CString source_password;
  CString source_schema;
  CString target_database;
  CString target_user;
  CString target_password;
  CString target_schema;
  CString type_migration;
  CString type_level;
  CString type_space;
  CString type_directstring;
  CString file_create;
  CString file_drop;
  CString file_directstring;
  CString table_table;
  CString table_min;
  CString table_allestr;
  CString log_loglines;
  CString log_logstr;
  CString directory;
  int     file_direct;
  int     table_all;
  int     log_logging;
  int     option_tables;
  int     option_data;
  int     option_truncate;
  int     option_deletes;
  int     option_primarys;
  int     option_indices;
  int     option_foreigns;
  int     option_sequences;
  int     option_triggers;
  int     option_m_access;


  if(m_profile == "")
  {
    return;
  }

  // Source Database
  GetDlgItemText(IDC_SOURCE,         source_database);
  GetDlgItemText(IDC_SOURCE_USER,    source_user);
  GetDlgItemText(IDC_SOURCE_PASSWORD,source_password);
  GetDlgItemText(IDC_SOURCE_SCHEMA,  source_schema);
  WritePrivateProfileString("SOURCE","database",source_database,m_profile);
  WritePrivateProfileString("SOURCE","user",    source_user,    m_profile);
  WritePrivateProfileString("SOURCE","password",source_password,m_profile);
  WritePrivateProfileString("SOURCE","schema",  source_schema,  m_profile);

  // Target Database
  GetDlgItemText(IDC_TARGET,          target_database);
  GetDlgItemText(IDC_TARGET_USER,     target_user);
  GetDlgItemText(IDC_TARGET_PASSWORD, target_password);
  GetDlgItemText(IDC_TARGET_SCHEMA,   target_schema);

  WritePrivateProfileString("TARGET","database", target_database, m_profile);
  WritePrivateProfileString("TARGET","user",     target_user,     m_profile);
  WritePrivateProfileString("TARGET","password", target_password, m_profile);
  WritePrivateProfileString("TARGET","schema",   target_schema,   m_profile);

  // Type Migration
  GetDlgItemText(IDC_TABLESPACE,    type_space);
  WritePrivateProfileString("TABLE","tablespace",type_space,    m_profile);

  // files
  GetDlgItemText(IDC_CREATE_FILE,file_create);
  GetDlgItemText(IDC_DROP_FILE,  file_drop);  
  file_direct = m_directMigration.GetCurSel();
  file_directstring.Format("%d",file_direct);

  // Directory and files
  GetDlgItemText(IDC_DIRECTORY,directory);
  WritePrivateProfileString("FILE","Directory",directory,     m_profile);
  WritePrivateProfileString("FILE","create",file_create,      m_profile);
  WritePrivateProfileString("FILE","drop",  file_drop,        m_profile);
  WritePrivateProfileString("FILE","direct",file_directstring,m_profile);

  // Tables
  table_all = IsDlgButtonChecked(IDC_ALLTABLES);
  GetDlgItemText(IDC_TABLE,      table_table);
  GetDlgItemText(IDC_MINOID,     table_min);

  table_allestr = table_all ? "1" : "0";
  WritePrivateProfileString("TABLE","all",        table_allestr,    m_profile);
  WritePrivateProfileString("TABLE","table",      table_table,      m_profile);
  WritePrivateProfileString("TABLE","minoid",     table_min,        m_profile);

  // Log en log lines
  log_logging = IsDlgButtonChecked(IDC_TOLOGFILE);
  GetDlgItemText(IDC_LOGLINES,log_loglines);
  log_logstr = log_logging ? "1" : "0";
  WritePrivateProfileString("LOG","logging",log_logstr  ,m_profile);
  WritePrivateProfileString("LOG","lines",  log_loglines,m_profile);

  // Options
  option_tables    = IsDlgButtonChecked(IDC_DO_TABLES);
  option_data      = IsDlgButtonChecked(IDC_DO_DATA);
  option_truncate  = IsDlgButtonChecked(IDC_DO_TRUNCATE);
  option_deletes   = IsDlgButtonChecked(IDC_DO_DELETES);
  option_primarys  = IsDlgButtonChecked(IDC_DO_PRIMARYS);
  option_indices   = IsDlgButtonChecked(IDC_DO_INDICES);
  option_foreigns  = IsDlgButtonChecked(IDC_DO_FOREIGN);
  option_sequences = IsDlgButtonChecked(IDC_DO_SEQUENCES);
  option_triggers  = IsDlgButtonChecked(IDC_DO_TRIGGERS);
  option_m_access  = IsDlgButtonChecked(IDC_DO_ACCESS);

  WritePrivateProfileString("OPTION","tables",   option_tables    ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","data",     option_data      ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","truncate", option_truncate  ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","deletes",  option_deletes   ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","primarys", option_primarys  ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","indices",  option_indices   ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","foreigns", option_foreigns  ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","sequences",option_sequences ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","triggers", option_triggers  ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","access",   option_m_access  ? "1" : "0",m_profile);
}

void SQLMigrateDialog::OnPaint() 
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialog::OnPaint();
  }
}

HCURSOR 
SQLMigrateDialog::OnQueryDragIcon()
{
  return (HCURSOR) m_hIcon;
}

void 
SQLMigrateDialog::OnClosing() 
{
  if(m_exportRunning)
  {
    return;
  }
  // Close program
  SaveProfile(m_profile);
  Closing(true);
}

void
SQLMigrateDialog::OnDirectory()
{
  MapDialog dlg;
  if (dlg.Browse(GetSafeHwnd(),"Find working directory"))
  {
    CString directory = dlg.GetPath();
    SetDlgItemText(IDC_DIRECTORY,directory);
  }
}

// Preset schema with user
void 
SQLMigrateDialog::OnEnKillfocusSourceUser()
{
  CString user;
  GetDlgItemText(IDC_SOURCE_USER,  user);
  SetDlgItemText(IDC_SOURCE_SCHEMA,user);
}

// Preset schema with user
void
SQLMigrateDialog::OnEnKillfocusTargetUser()
{
  CString user;
  GetDlgItemText(IDC_TARGET_USER,  user);
  SetDlgItemText(IDC_TARGET_SCHEMA,user);
}

void
SQLMigrateDialog::Closing(bool p_success)
{
  if (m_commandLineMode)
  {
    ExitProcess(p_success ? EXIT_SUCCESS : EXIT_FAILURE);
  }
  else
  {
    OnOK();
  }
}

void 
SQLMigrateDialog::OnDirectMigration() 
{
  int ind = m_directMigration.GetCurSel();

  if (ind == 0)
  {
    m_parameters.v_direct = 0;

    GetDlgItem(IDC_DROP_FILE)->EnableWindow(FALSE);
    GetDlgItem(IDC_CREATE_FILE)->EnableWindow(FALSE);
    // enable target database
    GetDlgItem(IDC_TARGET)->EnableWindow(TRUE);
  }
  else if(ind == 1)
  {
    m_parameters.v_direct = 1;
    GetDlgItem(IDC_DROP_FILE)->EnableWindow(FALSE);
    GetDlgItem(IDC_CREATE_FILE)->EnableWindow(FALSE);
    // enable target database
    GetDlgItem(IDC_TARGET)->EnableWindow(TRUE);
  } 
  else if (ind == 2)
  {
    m_parameters.v_direct = 2;
    GetDlgItem(IDC_DROP_FILE)->EnableWindow(TRUE);
    GetDlgItem(IDC_CREATE_FILE)->EnableWindow(TRUE);
    // disable target database
    GetDlgItem(IDC_TARGET)->EnableWindow(FALSE);
  }
}

void 
SQLMigrateDialog::OnAllTables() 
{
  UpdateData(TRUE);
  if (m_allTables == TRUE)
  {
    GetDlgItem(IDC_TABLE )->EnableWindow(FALSE);
    GetDlgItem(IDC_MINOID)->EnableWindow(FALSE);
    m_parameters.v_table = "";
    m_parameters.v_minOid = 0;
    SetDlgItemText(IDC_TABLE,"");
    SetDlgItemText(IDC_MINOID,"");
  }
  else
  {
    GetDlgItem(IDC_TABLE )->EnableWindow(TRUE);
    GetDlgItem(IDC_MINOID)->EnableWindow(TRUE);
    GetDlgItemText(IDC_TABLE, m_parameters.v_table); 
    CString getal;
    GetDlgItemText(IDC_MINOID,getal);
    m_parameters.v_minOid = atoi(getal);
  }  
}

void 
SQLMigrateDialog::OnToLogfile() 
{
  UpdateData(TRUE);
  GetDlgItem(IDC_LOGLINES)->EnableWindow(m_toLogfile);
}

void
SQLMigrateDialog::SetTableGauge(int num,int maxnum)
{
  // The MFC documentation states that the range is in the 32 bit range
  // @#! This is NOT the case. The maximum is 32768
  if(maxnum > 32000)
  {
    int max = (100 * maxnum) / 32000;
    int pos = (max * num) / maxnum;
    m_table_gauge.SetRange(0,(short)max);
    m_table_gauge.SetPos(pos);
  }
  else
  {
    m_table_gauge.SetRange(0,(short)maxnum);
    m_table_gauge.SetPos(num);
  }
}

void
SQLMigrateDialog::SetTablesGauge(int num,int maxnum)
{
  m_tables_gauge.SetRange(0,(short)maxnum);
  m_tables_gauge.SetPos(num);
}


void  
SQLMigrateDialog::SetSourceType(CString p_type)
{
  SetDlgItemText(IDC_SOURCE_TYPE,p_type);
}

void
SQLMigrateDialog::SetTargetType(CString p_type)
{
  SetDlgItemText(IDC_TARGET_TYPE,p_type);
}

void
SQLMigrateDialog::SetStatus(CString status)
{
  GetDlgItem(IDC_STATUS)->EnableWindow(status != "");
  m_status.SetWindowText(status); 
}

void 
SQLMigrateDialog::OnMigrate()
{
  m_log.SetWindowText(""); // Empty the log window
  GetDlgItemText(IDC_SOURCE,           m_parameters.v_source_dsn);  
  GetDlgItemText(IDC_SOURCE_USER,      m_parameters.v_source_user);  
  GetDlgItemText(IDC_SOURCE_PASSWORD,  m_parameters.v_source_password);  

  GetDlgItemText(IDC_TARGET,           m_parameters.v_target_dsn);  
  GetDlgItemText(IDC_TARGET_USER,      m_parameters.v_target_user);  
  GetDlgItemText(IDC_TARGET_PASSWORD,  m_parameters.v_target_password); 

  GetDlgItemText(IDC_SOURCE_SCHEMA,    m_parameters.v_source_schema);
  GetDlgItemText(IDC_TARGET_SCHEMA,    m_parameters.v_target_schema);

  GetDlgItemText(IDC_DIRECTORY,        m_parameters.v_directory);
  GetDlgItemText(IDC_DROP_FILE,        m_parameters.v_dropscript); 
  GetDlgItemText(IDC_CREATE_FILE,      m_parameters.v_createscript); 

  GetDlgItemText(IDC_TABLE,            m_parameters.v_table); 
  GetDlgItemText(IDC_TABLESPACE,       m_parameters.v_tablespace); 

  CString create   = m_parameters.v_directory + "\\" + m_parameters.v_createscript;
  CString dropping = m_parameters.v_directory + "\\" + m_parameters.v_dropscript;
  CString logging  = m_parameters.v_directory + "\\" + FILENAME_LOGFILE;

  m_logfile.SetScript(create);
  m_logfile.SetDropScript(dropping);
  m_logfile.SetLogfile(logging);

  if (!m_logfile.Open())
  {
    MessageBox("Cannot open a *.sql or *.txt file. File is read-only or no space left on file system!",SQL_MIGRATE,MB_OK);
    return;
  }

  // Options
  m_parameters.v_do_tables = IsDlgButtonChecked(IDC_DO_TABLES);
  m_parameters.v_do_data   = IsDlgButtonChecked(IDC_DO_DATA);
  m_parameters.v_truncate  = IsDlgButtonChecked(IDC_DO_TRUNCATE);
  m_parameters.v_deletes   = IsDlgButtonChecked(IDC_DO_DELETES);
  m_parameters.v_primarys  = IsDlgButtonChecked(IDC_DO_PRIMARYS);
  m_parameters.v_indices   = IsDlgButtonChecked(IDC_DO_INDICES);
  m_parameters.v_foreigns  = IsDlgButtonChecked(IDC_DO_FOREIGN);
  m_parameters.v_sequences = IsDlgButtonChecked(IDC_DO_SEQUENCES);
  m_parameters.v_triggers  = IsDlgButtonChecked(IDC_DO_TRIGGERS);
  m_parameters.v_access    = IsDlgButtonChecked(IDC_DO_ACCESS);

  CString getal;
  GetDlgItemText(IDC_MINOID,getal);
  m_parameters.v_minOid = atoi(getal);
  GetDlgItemText(IDC_LOGLINES,getal);
  m_parameters.v_logLines = atoi(getal);

  //////////////////////////////////////////////////////////////
  //
  // Here are all the checks on the logic sanity of all
  // filled in options. Does everything belong together?
  // In the mean time we build the logfile header
  //
  //////////////////////////////////////////////////////////////

  m_logfile.WriteLog("MIGRATION");
  // Extra empty line
  m_logfile.WriteLog(" ");

  // check bron data m_gegevens
  if (m_parameters.v_source_dsn =="")
  {
    MessageBox("No source database found. Enter a source database DataSourceName (DSN)",SQL_MIGRATE,MB_OK);
    m_source_dsn.SetFocus();
    return;
  }
  else
  {
    // Ruler           "------------------- : "
    m_logfile.WriteLog("Source database     : " + m_parameters.v_source_dsn);
  }
  if (m_parameters.v_source_user =="")
  {
    MessageBox("No source user found for the source database. Enter a source user name",SQL_MIGRATE,MB_OK);
    m_source_user.SetFocus();
    return;
  }
  else
  {
    // Ruler           "------------------- : "
    m_logfile.WriteLog("Source user name    : " + m_parameters.v_source_user);
  }

  if (m_parameters.v_source_password =="")
  {
    MessageBox("No source user password found. Enter a source password",SQL_MIGRATE,MB_OK);
    m_source_password.SetFocus();
    return;
  }
  m_logfile.WriteLog(" ");


  int direct = m_directMigration.GetCurSel();
  if(direct < 2) 
  {  
    // Check target
    if(m_parameters.v_target_dsn =="")
    {
      MessageBox("No target database found. Enter a target database DataSourceName (DSN)",SQL_MIGRATE,MB_OK);
      m_target_dsn.SetFocus();
      return;
    }
    else
    {
      // Ruler           "------------------- : "
      m_logfile.WriteLog("Target database     : " + m_parameters.v_target_dsn);
    }
    // Check target user
    if (m_parameters.v_target_user =="")
    {
      MessageBox("No target database user found. Enter a target database user",SQL_MIGRATE,MB_OK);
      m_target_user.SetFocus();
      return;
    }
    else
    {
      // Ruler           "------------------- : "
      m_logfile.WriteLog("Target user name    : " + m_parameters.v_target_user);
    }
    // Check target password
    if (m_parameters.v_target_password =="")
    {
      MessageBox("No target user password found. Enter a target password",SQL_MIGRATE,MB_OK);
      m_target_password.SetFocus();
      return;
    }
  }
  m_logfile.WriteLog("");

  if(direct == 2)
  {
    // Ruler           "-------------------- : "
    m_logfile.WriteLog("Direct migration     : No");
    if ( m_parameters.v_dropscript =="")
    {
      MessageBox("No file name for the dropscript given. Enter a file name...",SQL_MIGRATE,MB_OK);
      m_dropscript.SetFocus();
      return;
    }
    else
    {
      // Ruler           "------------------- : "
      m_logfile.WriteLog("Dropscript          : " + m_parameters.v_dropscript);
    }
    if (m_parameters.v_createscript == "")
    {
      MessageBox("No file name for a create-script given. Enter a file name...",SQL_MIGRATE,MB_OK);
      m_createscript.SetFocus();
      return;
    }
    else
    {
      // Ruler           "------------------ : "
      m_logfile.WriteLog("Create script      : " + m_parameters.v_createscript);
    }
  }
  else
  {
    // Ruler           "------------------- : "
    m_logfile.WriteLog(CString("Direct migration    : ") + ((direct == 0) ? "Datapump" : "SELECT/INSERT"));
  }
  if (m_allTables == FALSE && m_parameters.v_table == "")
  {
    MessageBox("No tablename given for a table migration. Enter a table name or table pattern...",SQL_MIGRATE,MB_OK);
    m_editTable.SetFocus();
    return;
  }
  else
  {
    if (m_parameters.v_table =="")
    {
      // Ruler           "------------------- : "
      m_logfile.WriteLog("Tables              : All");
    }
    else
    {
      // Ruler           "------------------- : "
      m_logfile.WriteLog("Table(s)            : " + m_parameters.v_table);  
    }
  }
  // Ruler           "------------------- : "
  m_logfile.WriteLog("Tablespace          : " + m_parameters.v_tablespace);


  if(m_parameters.v_minOid < 0)
  {
    MessageBox("Not a valid number for a primary identity key",SQL_MIGRATE,MB_OK);
    m_minOid.SetFocus();
    return;
  }
  GetDlgItemText(IDC_MINOID,getal);
  m_logfile.WriteLog("Start with identity : " + getal);

  // Controleer aantal regels in het logfile
  if(m_parameters.v_logLines)
  {
    if(m_parameters.v_logLines < 10 || m_parameters.v_logLines > 100000)
    {
      MessageBox("Number of rows per line in the logfile in an invalid range.\n"
                 "Fill in a number between 10 and 100.000",SQL_MIGRATE,MB_OK);
      m_logPerRow.SetFocus();
      return;
    }
  }
  GetDlgItemText(IDC_LOGLINES,getal);
  m_logfile.WriteLog("Logline after rows  : " + getal);

  // Check if we have something to do!
  if(m_parameters.v_do_tables + m_parameters.v_do_data   +
     m_parameters.v_primarys  + m_parameters.v_indices   + 
     m_parameters.v_foreigns  + m_parameters.v_sequences + 
     m_parameters.v_triggers  + m_parameters.v_access == 0)
  {
    MessageBox("Nothing will be migrated because all options are in the OFF position.\n",SQL_MIGRATE,MB_OK);
    return;
  }

  ///////////////////////////////////////////////////////////////////////////////////////
  //
  //   MIGRATION
  //   Perform the real migration now!
  //
  ///////////////////////////////////////////////////////////////////////////////////////

  m_migrate = new SQLMigrate(m_parameters, m_logfile);

  try
  {
    // block all buttons
    m_exportRunning = true;
    m_exportResult  = m_migrate->Migrate();
  }
  catch(...)
  {
    CString text("Migration is stopped with an error");
    if(m_commandLineMode)
    {
      m_logfile.WriteLog("");
      m_logfile.WriteLog(text);
    }
    else
    {
      MessageBox(text,SQL_MIGRATE,MB_OK);
    }
  }
  m_exportRunning = false;
  delete m_migrate;
  m_migrate = nullptr;

  // Closing the logfile now. we are done.
  m_logfile.Close();

  // Reset the status box on the window
  SetTableGauge (0,100);
  SetTablesGauge(0,100);
  SetDlgItemText(IDC_SOURCE_TYPE,"");
  SetDlgItemText(IDC_TARGET_TYPE,"");
  SetStatus("");

  if(m_exportResult)
  {
    CString text("The migration is complete");
    if (m_commandLineMode)
    {
      m_logfile.WriteLog("");
      m_logfile.WriteLog(text);
    }
    else
    {
      MessageBox(text,SQL_MIGRATE,MB_OK);
    }
  }
}

BOOL
SQLMigrateDialog::AddLogLine(CString msg)
{
  m_log.SetRedraw(FALSE);

  int cLines = m_log.GetLineCount();
  int nLastChar = (cLines ? m_log.LineIndex(cLines-1) : 0) + m_log.LineLength(cLines);
  cLines = m_log.GetLineCount();
  while(m_log.LineIndex(cLines-1) > 28000)
  {
    // If the number of lines in the edit box becomes to large (30.000 lines)
    // we must clear a few lines at the beginning
    int nFirstLine  = m_log.LineIndex(0);
    int nSecondLine = m_log.LineIndex(10);
    m_log.SetSel(nFirstLine, nSecondLine, TRUE /*NoScroll*/);
    m_log.ReplaceSel("");
    cLines = m_log.GetLineCount();
  }
  nLastChar = (cLines ? m_log.LineIndex(cLines-1) : 0) + m_log.LineLength(cLines);
  m_log.SetSel(nLastChar, nLastChar, TRUE);

  m_log.SetRedraw(TRUE);

  msg.Replace("\n","\r\n");

  m_log.ReplaceSel(msg);
  m_log.ReplaceSel("\r\n");

  return TRUE;
}

void 
SQLMigrateDialog::HandleMessages()
{
  // Handle only the PAINT messages for larger processes
  // So we can see the application
  // Potentially we can have an endless loop here
  // so we limit te loop to a certain time frame
  MSG msg;
  UINT ticks = GetTickCount();
  while (GetTickCount() - ticks < 100 && (PeekMessage(&msg,NULL,WM_MOVE,WM_USER,PM_REMOVE)))
  {
    try
    {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
    catch(StdException& er)
    {
      // How now brown cow?
      UNREFERENCED_PARAMETER(er);
    }
  }
}
