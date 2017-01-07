// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Standard OS versions and headers 
#include "targetver.h"

// Standard libraries
#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // remove support for MFC controls in dialogs

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

// IF We want SQLInfoTree to work, we need the whole shebang from MFC
// IF we want SQLDatasetXLS to work, we need CArhive for oldstyle *.XLS
//    so we also need to include the full set
// So we include the full MFC set, otherwise: we just need CString
//
#ifdef SQL_COMPONENTS_MFC
#include <afx.h>
#include <afxwin.h>             // MFC core and standard components
#include <afxext.h>             // MFC extensions
#include <afxcmn.h>             // MFC support for Windows Common Controls
#else
#include <atlstr.h>             // Just ATLString and CString
#endif

// Headers for unit testing
#include "CppUnitTest.h"

