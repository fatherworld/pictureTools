
// ComputeH_VisualTools.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CComputeH_VisualToolsApp:
// See ComputeH_VisualTools.cpp for the implementation of this class
//

class CComputeH_VisualToolsApp : public CWinApp
{
public:
	CComputeH_VisualToolsApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CComputeH_VisualToolsApp theApp;