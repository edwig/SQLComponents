// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define NO_WARN_MBCS_MFC_DEPRECATION

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

// IF We want SQLInfoTree to work, we need the whole shebang from MFC
// IF we want SQLDatasetXLS to work, we need CArhive for oldstyle *.XLS
//    so we also need to include the full set
// So we include the full MFC set, otherwise: we just need CString
//
#include <afx.h>
#include <afxwin.h>             // MFC core and standard components
#include <afxext.h>             // MFC extensions
#include <afxcmn.h>             // MFC support for Windows Common Controls

// Include our library
#include "SQLComponents.h"




