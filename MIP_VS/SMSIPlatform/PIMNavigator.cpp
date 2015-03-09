/*
Filename    : PIMNavigator.cpp
Part of     : PIM Navigator
Description : Implementation of application class inherited from CWinApp
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

#include "stdafx.h"
#include "PCCSUtils.h"
#include "PIMNavigator.h"
#include "PIMNavigatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//===================================================================
// The one and only CPIMNavigatorApp object
//
//===================================================================
CPIMNavigatorApp theApp;


//===================================================================
// Messagemap
//
//===================================================================
BEGIN_MESSAGE_MAP(CPIMNavigatorApp, CWinApp)
	//{{AFX_MSG_MAP(CPIMNavigatorApp)
	//}}AFX_MSG
END_MESSAGE_MAP()


//===================================================================
// Constructor
//
//===================================================================
CPIMNavigatorApp::CPIMNavigatorApp()
{
}


//===================================================================
// InitInstance()
//
// Initializes PC Suite Connectivity API and starts user interface dialog
//
//===================================================================
BOOL CPIMNavigatorApp::InitInstance()
{
	DWORD dwRet	= CONA_OK;
	// Initialize Device Management API
	dwRet = DMAPI_Initialize(DMAPI_VERSION_32, NULL);
	if (dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("Error initializing DMAPI"), dwRet);
		return false;
	}

	// Initialize Content Access API
	dwRet = CAAPI_Initialize(CAAPI_VERSION_31, NULL);
	if (dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("Error initializing CAAPI"), dwRet);
		DMAPI_Terminate(NULL);
		return false;
	}

	// Initialize Common Functions API
	dwRet = CFAPI_Initialize(CFAPI_VERSION_10, NULL);
	if (dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("Error initializing CAAPI"), dwRet);
		DMAPI_Terminate(NULL);
		CAAPI_Terminate(NULL);
		return false;
	}

	// Open the main dialog
	CPIMNavigatorDlg* dlg = new CPIMNavigatorDlg;
	m_pMainWnd = dlg;
	dlg->DoModal();
    delete dlg;

	// Terminate Common Functions API
	CFAPI_Terminate(NULL);
	// Terminate Content Access API
	CAAPI_Terminate(NULL);
	// Terminate Device Management API
	DMAPI_Terminate(NULL);
	return FALSE;
}

