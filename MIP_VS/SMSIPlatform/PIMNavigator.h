/*
Filename    : PIMNavigator.h
Part of     : PIMNavigator
Description : Declaration of application class inherited from CWinApp
Version     : 3.2

This example is only to be used with PC Connectivity API version 3.2.
Compability ("as is") with future versions is not quaranteed.

Copyright (c) 2007 Nokia Corporation.
 
This material, including but not limited to documentation and any related 
computer programs, is protected by intellectual property rights of Nokia 
Corporation and/or its licensors.
All rights are reserved. Reproducing, modifying, translating, or 
distributing any or all of this material requires the prior written consent 
of Nokia Corporation. Nokia Corporation retains the right to make changes 
to this material at any time without notice. A copyright license is hereby 
granted to download and print a copy of this material for personal use only.
No other license to any other intellectual property rights is granted. The 
material is provided "as is" without warranty of any kind, either express or 
implied, including without limitation, any warranty of non-infringement, 
merchantability and fitness for a particular purpose. In no event shall 
Nokia Corporation be liable for any direct, indirect, special, incidental, 
or consequential loss or damages, including but not limited to, lost profits 
or revenue,loss of use, cost of substitute program, or loss of data or 
equipment arising out of the use or inability to use the material, even if 
Nokia Corporation has been advised of the likelihood of such damages occurring.
*/ 

#ifndef __NOKIA_PCCS_APPLICATION_PIMNAVIGATOR_EXAMPLE__PIMNAVIGATORAPP__
#define __NOKIA_PCCS_APPLICATION_PIMNAVIGATOR_EXAMPLE__PIMNAVIGATORAPP__

#if _MSC_VER > 1000
#pragma once
#endif 

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CPIMNavigatorApp
/////////////////////////////////////////////////////////////////////////////

class CPIMNavigatorApp : public CWinApp
{

public:

	CPIMNavigatorApp();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPIMNavigatorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPIMNavigatorApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
