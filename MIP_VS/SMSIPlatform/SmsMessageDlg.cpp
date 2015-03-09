/*
Filename    : SmsMessageDlg.cpp
Part of     : PIM Navigator
Description : Implementation of "New SMS Message" dialog
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
#include "pimnavigator.h"
#include "SmsMessageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmsMessageDlg dialog


CSmsMessageDlg::CSmsMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSmsMessageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSmsMessageDlg)
	m_strMessage = _T("");
	m_strNumber = _T("");
	//}}AFX_DATA_INIT
}


void CSmsMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmsMessageDlg)
//	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strMessage);
//	DDX_Text(pDX, IDC_EDIT_NUMBER, m_strNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSmsMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CSmsMessageDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//===================================================================
// GetData
//
// Fills CA_DATA_MSG struct with information that use has given
//
//===================================================================
void CSmsMessageDlg::GetData(CA_DATA_MSG& dataMsg)
{
	// Phone number
	int iLength = m_strNumber.GetLength();
	WCHAR* pStr = NULL;

    if(iLength > 0)
	{	
		pStr = CONAAllocString((LPAPIWCHAR)(LPCTSTR)m_strNumber);
		CA_DATA_ADDRESS* pDataAddress = (CA_DATA_ADDRESS*)CoTaskMemAlloc(sizeof(CA_DATA_ADDRESS));
		pDataAddress->dwSize = sizeof(CA_DATA_ADDRESS);
		pDataAddress->dwAddressInfo = CA_MSG_ADDRESS_TYPE_NUMBER;
		pDataAddress->pstrAddress = pStr;
		dataMsg.bAddressCount = 1;
		dataMsg.pAddress = pDataAddress;
	}
	// Message body text
	iLength = m_strMessage.GetLength();
	if(iLength > 0)
	{
		pStr = (LPTSTR)CoTaskMemAlloc((iLength + 1) * sizeof(WCHAR));
		wcsncpy_s(pStr, iLength + 1, (LPCTSTR)m_strMessage, iLength + 1);
		dataMsg.dwDataLength = (iLength + 1) * sizeof(WCHAR);
		dataMsg.pbData = (BYTE*)pStr;
	}
}
