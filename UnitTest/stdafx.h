// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // remove support for MFC controls in dialogs

// Standard OS versions and headers 
#include "targetver.h"
#include "framework.h"

// Standard libraries
#include <stdio.h>

// IF We want SQLInfoTree to work, we need the whole shebang from MFC
// IF we want SQLDatasetXLS to work, we need CArhive for oldstyle *.XLS
//    so we also need to include the full set
// So we include the full MFC set, otherwise: we just need XString
//
#include <afxwin.h>             // MFC core and standard components
#include <afxext.h>             // MFC extensions
#include <afxcmn.h>             // MFC support for Windows Common Controls

// Headers for our libraries
#include <BaseLibrary.h>
#include <SQLComponents.h>
