//////////////////////////////////////////////////////////////////////////
//
// Getting the framework settings for this library/program
// Uses the $(SolutionDir)Framework.h file for configuration of 
// the MFC XString or the std::string SMX_String
//
#pragma once

#define WIN32_LEAN_AND_MEAN                     // Exclude rarely-used stuff from Windows headers

#include "..\Framework.h"

//////////////////////////////////////////////////////////////////////////
//
// Can be extended beyond this point with extra MFC requirements
//
//////////////////////////////////////////////////////////////////////////

// Extras needed for MFC in this program
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h>         // MFC support for Windows Common Controls
#include <afxcontrolbars.h> // MFC support for ribbons and control bars
