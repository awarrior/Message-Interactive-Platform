/*
Filename    : PIMNavigatorDlg.cpp
Part of     : PIM Navigator
Description : Implementation of PIMNavigator's dialog class
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
#include "atlimage.h"
#include "Shlwapi.h"
#include "PCCSUtils.h"
#include "PIMNavigator.h"
#include "PIMNavigatorDlg.h"
#include "SmsMessageDlg.h"
#include "MainFrm.h"
#include "ColorListCtrl.h"

extern CColorListCtrl g_List;
extern CString g_Phone;
extern _RecordsetPtr g_pRecordset1;
extern _RecordsetPtr g_pRecordset2;
extern int g_Year;	
extern int g_Month;
extern int g_Day;	
extern int g_Hour;
extern int g_Min;
extern int g_Index;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CHINA_TIME 8				// 中国时区

//===================================================================
// Messagemap
//
//===================================================================
BEGIN_MESSAGE_MAP(CPIMNavigatorDlg, CDialog)
	//{{AFX_MSG_MAP(CPIMNavigatorDlg)
//	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_NAVIGATOR, OnSelchangedTreeNavigator)
//	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE_NAVIGATOR, OnTvnItemexpandedTreeNavigator)
//	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//===================================================================
// OnCAOperationNotify
//
// Receives Content Access API operation notifications
//
//===================================================================
DWORD COperationNotify::OnCAOperationNotify(DWORD dwOperation,
											DWORD dwCurrent,
											DWORD dwTotalAmount,
											DWORD dwStatus,
											CA_ITEM_ID* pItemID)
{
	LPWSTR pstrUid = NULL;
	ItemUidToString(pItemID, pstrUid);
	SYSTEMTIME sTime;
	::GetLocalTime(&sTime);
	wchar_t szDate[128] = {0};
	GetFormattedDate(&sTime, szDate, 128);
	CString strStatus = L"";
	strStatus.Format(L"%s OnCAOperationNotify: Operation(%d), progress(%d), total(%d), status(%d), item(%s)",
		szDate, dwOperation, dwCurrent, dwTotalAmount, dwStatus, pstrUid);
	if(pstrUid)
	{
		delete [] pstrUid;
	}
	
	strStatus += L"\n";
	TRACE(strStatus);
	return CONA_OK;
}

//===================================================================
// Constructor
//
//===================================================================
CPIMNavigatorDlg::CPIMNavigatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPIMNavigatorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPIMNavigatorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIsLoadingData = FALSE;
	m_iTargetCount = 0;
	m_iItemCount = 0;
	m_hDM = NULL;
	m_hSMS = NULL;
	m_hCurrentConnection = NULL;
	m_hCurrentItem = NULL;
	m_hCurrentTarget = NULL;
	m_hSMSFolder = NULL;
	m_dwCurrentSMSConnection = 0;

	//m_Pro = "";
	m_Time = "";
	m_PhoNum = "";
	m_Text = "";
}

//===================================================================
// Destructor
//
//===================================================================
CPIMNavigatorDlg::~CPIMNavigatorDlg()
{
	// Close CA connections
	CloseConnection(m_hSMS);

	// Unregister device notification interface
	DWORD dwRet = CONARegisterDMNotifyIF(m_hDM, API_UNREGISTER, this);
	if(dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("CONARegisterDMNotifyIF"), dwRet);
	}

	// Close device management handle
	dwRet = CONACloseDM(m_hDM);
	if(dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("CONACloseDM"), dwRet);
	}

	// Free memory reserved for PIM items
	FreeItemData();
}

//===================================================================
// OnInitDialog
//
// Initializes user interface and register device notify callback
//
//===================================================================
BOOL CPIMNavigatorDlg::OnInitDialog()
{
    // Get device management handle
	DWORD dwRet = CONAOpenDM(&m_hDM);
	if(dwRet != CONA_OK) 
		ErrorMessageDlg(_T("OnInit:CONAOpenDM"), dwRet);

	// Register device notification interface
	dwRet = CONARegisterDMNotifyIF(m_hDM, API_REGISTER, this);
	if(dwRet != CONA_OK) 
		ErrorMessageDlg(_T("CONARegisterDMNotifyIF"), dwRet);
		
	return TRUE;  
}

//===================================================================
// DoDataExchange 
//
//===================================================================
void CPIMNavigatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//===================================================================
// CCONADeviceNotify implementation

void CPIMNavigatorDlg::OnDeviceListUpdated()
{
    TRACE(L"CPIMNavigatorDlg::DeviceListUpdated()\n");
	RefreshTreeView();
}

void CPIMNavigatorDlg::OnDeviceAdded(const WCHAR* pstrSN, DWORD dwStatus)
{
    TRACE(L"CPIMNavigatorDlg::DeviceAdded(%s, %i)\n", pstrSN, dwStatus);
	m_pstrSN=*pstrSN;
	m_dwStatus=dwStatus;
	RefreshTreeView();
}

void CPIMNavigatorDlg::OnDeviceRemoved(const WCHAR* pstrSN, DWORD dwStatus)
{
    TRACE(L"CPIMNavigatorDlg::DeviceRemoved(%s, %i)\n", pstrSN, dwStatus);
	// Get main window and refresh phone list
	RefreshTreeView();
}

void CPIMNavigatorDlg::OnDeviceUpdated(const WCHAR* pstrSN, DWORD dwStatus)
{
    TRACE(L"CPIMNavigatorDlg::DeviceUpdated(%s, %i)\n", pstrSN, dwStatus);
	RefreshTreeView();
}

//===================================================================
// OnCANotify
//
// Receives Content Access API notifications
//
//===================================================================
DWORD CPIMNavigatorDlg::OnCANotify(DWORD dwReason, DWORD dwStatus, CA_ITEM_ID *pItemID)
{
	LPWSTR pstrUid = NULL;
	ItemUidToString(/*pItemID*/NULL, pstrUid);
	SYSTEMTIME sTime;
	::GetLocalTime(&sTime);
	wchar_t szDate[128] = {0};
	GetFormattedDate(&sTime, szDate, 128);
	CString strStatus = L"";
	switch(dwReason)
	{
	case CA_REASON_ENUMERATING:
		strStatus.Format(L"%s OnCANotify(CA_REASON_ENUMERATING)", szDate);
		break;
	case CA_REASON_ITEM_ADDED:
		//strStatus.Format(L"%s OnCANotify(CA_REASON_ITEM_ADDED) %s", szDate, pstrUid);
		break;
	case CA_REASON_ITEM_DELETED:
		strStatus.Format(L"%s OnCANotify(CA_REASON_ITEM_DELETED) %s", szDate, pstrUid);
		break;
	case CA_REASON_ITEM_UPDATED:
		strStatus.Format(L"%s OnCANotify(CA_REASON_ITEM_UPDATED) %s", szDate, pstrUid);
		break;
	case CA_REASON_ITEM_MOVED:
		strStatus.Format(L"%s OnCANotify(CA_REASON_ITEM_MOVED) %s", szDate, pstrUid);
		break;
	case CA_REASON_ITEM_REPLACED:
		strStatus.Format(L"%s OnCANotify(CA_REASON_ITEM_REPLACED) %s", szDate, pstrUid);
		break;
	case CA_REASON_CONNECTION_LOST:
		strStatus.Format(L"%s OnCANotify(CA_REASON_CONNECTION_LOST)", szDate);
		break;
	case CA_REASON_MSG_DELIVERY:
		strStatus = L"OnCANotify(CA_REASON_MSG_DELIVERY)";
		break;
	default:
		strStatus = L"OnCANotify, unknown reason";
		break;
	}
	if(pstrUid)
	{
		delete [] pstrUid;
	}
	
	strStatus += L"\n";
	TRACE(strStatus);
	return CONA_OK;
}

//===================================================================
// FreeItemData
//
// Free memory reserved for PIM items
//
//===================================================================
void CPIMNavigatorDlg::FreeItemData()
{
	// PIM items
	for(int i = 0; i < m_iItemCount; i++)
	{
		if(m_pItemUIDs[i])
		{
			delete m_pItemUIDs[i];
		}
	}
	m_pItemUIDs.RemoveAll();
	m_iItemCount = 0;
	// Target folders
	for(int i = 0; i < m_iTargetCount; i++)
	{
		if(m_pFolderInfos[i])
		{
			delete m_pFolderInfos[i];
		}
	}
	m_pFolderInfos.RemoveAll();
	m_iTargetCount = 0;
}

//===================================================================
// CloseConnection
//
// Close CA connection
//
//===================================================================
DWORD CPIMNavigatorDlg::CloseConnection(CAHANDLE& hCA)
{
	DWORD dwRet = CONA_OK;
	if(hCA)
	{
		dwRet = CARegisterNotifyIF(hCA, API_UNREGISTER, this);
		if(dwRet != CONA_OK)
		{
			ErrorMessageDlg(_T("CloseConnection:CARegisterNotifyIF"), dwRet);
		}
		// Close CA connection
		dwRet = DACloseCA(hCA);
		if(dwRet != CONA_OK)
		{
			ErrorMessageDlg(_T("DACloseCA"), dwRet);
		}
		hCA = NULL;
	}
	return dwRet;
}

//===================================================================
// CheckSMSConnection
//
// Check CA connection to SMS folders and open it if needed
//
//===================================================================
DWORD CPIMNavigatorDlg::CheckSMSConnection(DWORD dwDeviceIndex)
{
	DWORD dwRet = CONA_OK;
	if(m_dwCurrentSMSConnection != dwDeviceIndex)
	{	// Device has changed, close previous connection
		CloseConnection(m_hSMS);
		m_dwCurrentSMSConnection = dwDeviceIndex;
	}
	if(!m_hSMS)
	{	// No CA connection, open it
		DWORD dwMedia = API_MEDIA_ALL;
		dwRet = DAOpenCA(m_strSerialNumberTable[dwDeviceIndex], &dwMedia, CA_TARGET_SMS_MESSAGES, &m_hSMS);
		if( dwRet != CONA_OK )
		{
			ErrorMessageDlg(_T("CheckSMSConnection:DAOpenCA"), dwRet);
		}
		dwRet = CARegisterNotifyIF(m_hSMS, API_REGISTER, this);
		if(dwRet != CONA_OK)
		{
			ErrorMessageDlg(_T("CheckContactsConnection:CARegisterNotifyIF"), dwRet);
		}
	}
	return dwRet;
}

//===================================================================
// AddFolder
//
// Adds folder to tree view and calls recursively itself for subfolders.
//
//===================================================================
HTREEITEM CPIMNavigatorDlg::AddFolder(CString strRootFolder, CA_FOLDER_INFO* pFolderInfo, HTREEITEM hParent, int iIconFolderIndex, int iIconItemIndex)
{
	if(!pFolderInfo)
	{
		return 0;
	}
	// Insert folder item in tree view
	CString strFolderName;
	if(wcscmp(pFolderInfo->pstrName, L"\\"))
	{
		strFolderName = pFolderInfo->pstrName;
	}
	else
	{
		strFolderName = strRootFolder;
	}
	HTREEITEM hItem = m_treeNavigator.InsertItem(strFolderName, iIconFolderIndex, iIconFolderIndex, hParent);
	m_pFolderInfos.SetAtGrow(m_iTargetCount, new CA_FOLDER_INFO);
	memcpy(m_pFolderInfos[m_iTargetCount], pFolderInfo, sizeof(CA_FOLDER_INFO));
	m_treeNavigator.SetItemData(hItem, m_iTargetCount);
	// Add dummy item to get '+' showed.
	HTREEITEM hSubItem = m_treeNavigator.InsertItem(L"", iIconItemIndex, iIconItemIndex, hItem);
	m_treeNavigator.SetItemData(hSubItem, -1);
	m_iTargetCount++;
	for(DWORD i = 0; i < pFolderInfo->dwSubFolderCount; i++)
	{	// Recursive call for adding subfolders.
		AddFolder(strRootFolder, &pFolderInfo->pSubFolders[i], hItem, iIconFolderIndex, iIconItemIndex);
	}
	return hItem;
}

//===================================================================
// GetSMSFolders
//
// Gets SMS folder info and creates folders in tree view
//
//===================================================================
void CPIMNavigatorDlg::GetSMSFolders(DWORD dwDeviceIndex, HTREEITEM hPhone)
{
	// Check CA connection to SMS folders and open it if needed
	DWORD dwRet = CheckSMSConnection(dwDeviceIndex);
	if(dwRet == CONA_OK)
	{	// Get SMS folder target paths
		CA_FOLDER_INFO pFolderInfo = {0};
		pFolderInfo.dwSize = sizeof(pFolderInfo);
		dwRet = CAGetFolderInfo(m_hSMS, &pFolderInfo);
		if(dwRet == CONA_OK)
		{
			m_hSMSFolder = AddFolder(L"SMS Messages", &pFolderInfo, hPhone, m_iIconSMSMessagesIndex, m_iIconSMSIndex);
		}
		else
		{
			ErrorMessageDlg(_T("CAGetFolderInfo"), dwRet);
		}
	}
}

//===================================================================
// RefreshTreeView 
//
// Fill connected devices in tree view
//
//===================================================================
void CPIMNavigatorDlg::RefreshTreeView()
{
	TRACE(_T("CPIMNavigatorDlg::RefreshTreeView()\n"));

	CloseConnection(m_hSMS);
	// Clear all previous data
//	m_hCurrentItem = NULL;
	m_dwCurrentSMSConnection = 0;
	FreeItemData(); // Free memory reserved for PIM items
	m_bIsLoadingData = TRUE;
		
//    CWaitCursor wait;

	DWORD dwDeviceCount = 0;
	
	DWORD dwRet = CONAGetDeviceCount(m_hDM, &dwDeviceCount);
    if(dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("CONAGetDeviceCount"), dwRet);
	}
	if(dwRet == CONA_OK && dwDeviceCount > 0)
	{	// Get list of currently connected devices
		CONAPI_DEVICE* pDevices = new CONAPI_DEVICE[dwDeviceCount];
		dwRet = CONAGetDevices(m_hDM, &dwDeviceCount, pDevices);
        if(dwRet == CONA_OK)
		{
			if(dwDeviceCount > MAX_CONNECTIONS)
			{	// We choose to support only MAX_CONNECTIONS connections
				dwDeviceCount = MAX_CONNECTIONS;
			}
			// Add each device
			for(DWORD dwDeviceIndex = 0; dwDeviceIndex < dwDeviceCount; dwDeviceIndex++)
			{
				// Insert phone item
				CString str = pDevices[dwDeviceIndex].pstrFriendlyName;//手机型号//				
				g_Phone=str;// 读取手机
				wcsncpy_s(m_strSerialNumberTable[dwDeviceIndex], 16, pDevices[dwDeviceIndex].pstrSerialNumber, 16);
			}
			// Free memory allocated by CONAGetDevices
			CONAFreeDeviceStructure(dwDeviceCount, pDevices);
            if( dwRet != CONA_OK ) ErrorMessageDlg(_T("CONAFreeDeviceStructure"), dwRet);

		}
        else
		{
            ErrorMessageDlg(_T("CONAGetDevices"), dwRet);
		}
		// Delete dynamically allocated memory
		if (pDevices) 
        {
            delete[] pDevices;
		    pDevices = NULL;
        }
	}
	m_bIsLoadingData = FALSE;
	if(dwRet == CONA_OK && dwDeviceCount == 0)
	{
		g_Phone=_T("没有连接");// 提示没有连接
	}
}

//===================================================================
// GetSMSMessages
//
// Reads SMS items from phone and adds them into tree view.
//
//===================================================================
void CPIMNavigatorDlg::GetSMSMessages(DWORD dwDeviceIndex, HTREEITEM hTarget)
{
	// Check CA connection to SMS folders and open it if needed
	DWORD dwRet = CheckSMSConnection(dwDeviceIndex);
	if(dwRet == CONA_OK)
	{
		int iTarget = (int)m_treeNavigator.GetItemData(hTarget);
		// Set SMS folder target path
		CA_FOLDER_INFO* pTargetPath = (CA_FOLDER_INFO*)m_pFolderInfos[iTarget];
		// Check folder ID. In user folder root there is no messages
		if(!pTargetPath->dwFolderId || pTargetPath->dwFolderId == CA_MESSAGE_FOLDER_USER_FOLDERS)
		{
			return;
		}
		// Read all the SMS item UIDs from the connected device 
		CA_ID_LIST caIDList = {0};
		caIDList.dwSize = sizeof(caIDList);
		dwRet = CAGetIDList(m_hSMS, pTargetPath->dwFolderId, 0, &caIDList); 
		if(dwRet != CONA_OK)
		{
			ErrorMessageDlg(_T("CAGetIDList"), dwRet);
		}
		CAOPERATIONHANDLE hOperHandle = NULL;
		dwRet = CABeginOperation(m_hSMS, 0, &hOperHandle);
		if(dwRet != CONA_OK)
		{
			ErrorMessageDlg(_T("CABeginOperation"), dwRet);
		}
		for(DWORD k = 0; k < caIDList.dwUIDCount; k++)
		{	// Read SMS item from the connected device
			CA_DATA_MSG dataMsg = {0};;
			dataMsg.dwSize = sizeof(dataMsg);
			dwRet = CAReadItem(hOperHandle, &caIDList.pUIDs[k], 0, CA_DATA_FORMAT_STRUCT, (LPVOID*)&dataMsg);
			if(dwRet == CONA_OK)
			{
				if(dataMsg.dwDataLength > 0)
				{
					if((CA_GET_DATA_FORMAT(dataMsg.dwInfoField)) == CA_DATA_FORMAT_UNICODE)
					{
						if(dataMsg.pbData)
						{	// Insert SMS message item in tree view
							CString strData((LPTSTR)dataMsg.pbData, dataMsg.dwDataLength / sizeof(wchar_t));
							HTREEITEM hSubItem = m_treeNavigator.InsertItem(strData, m_iIconSMSIndex, m_iIconSMSIndex, hTarget);
                			m_pItemUIDs.SetAtGrow(m_iItemCount, new CA_ITEM_ID);
							memcpy(m_pItemUIDs[m_iItemCount], &caIDList.pUIDs[k], sizeof(CA_ITEM_ID));
							m_treeNavigator.SetItemData(hSubItem, m_iItemCount);
							m_iItemCount++;
						}
						else
						{
							// No data, GMS or other kind of message?
						}
					}
					else
					{
						// Message in data format
					}
				}
				// Free memory allocated by CA API
				dwRet = CAFreeItemData(m_hSMS, CA_DATA_FORMAT_STRUCT, (LPVOID)&dataMsg);
				if(dwRet != CONA_OK)
				{
					ErrorMessageDlg(_T("CAFreeItemData"), dwRet);
				}
			}
			else
			{
				ErrorMessageDlg(_T("CAReadItem"), dwRet);
			}
		}
		dwRet = CAEndOperation(hOperHandle);
		if(dwRet != CONA_OK)
		{
			ErrorMessageDlg(_T("CAEndOperation"), dwRet);
		}
	}
}

//===================================================================
// OnTvnItemexpandedTreeNavigator
//
// User has clicked '+' in tree view in order to expand item.
//
//===================================================================
void CPIMNavigatorDlg::OnTvnItemexpandedTreeNavigator(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	HTREEITEM hParent = m_treeNavigator.GetParentItem(hItem);
	if(!m_treeNavigator.ItemHasChildren(hItem))
	{
		return;
	}
	HTREEITEM hChild = m_treeNavigator.GetChildItem(hItem);
	if(m_treeNavigator.GetItemData(hChild) != -1)
	{	// Already expanded
		return;
	}
    CWaitCursor wait;
	// Remove dummy item.
	m_treeNavigator.DeleteItem(hChild);
	DWORD dwDeviceIndex = GetCurrentDevice();
	int iImage = 0;
	int iSelImage = 0;
	m_treeNavigator.GetItemImage(hItem, iImage, iSelImage);
	if(iImage == m_iIconPhoneIndex)
	{		
		GetSMSFolders(dwDeviceIndex, hItem);
	}
	else if(iImage == m_iIconSMSMessagesIndex)
	{	// Check CA connection to SMS folders and open it if needed
		CheckSMSConnection(dwDeviceIndex);
		GetSMSMessages(dwDeviceIndex, hItem);
	}
}

//===================================================================
// GetSMSDetails
//
// Read selected SMS item from phone and show details in list view.
//
//===================================================================
void CPIMNavigatorDlg::GetSMSDetails(CA_ITEM_ID* pUID)
{
	int iItemCount = 0;
	CAOPERATIONHANDLE hOperHandle = NULL;
	m_hCurrentConnection = m_hSMS;
	DWORD dwRet = CABeginOperation(m_hCurrentConnection, 0, &hOperHandle);
	if(dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("CABeginOperation"), dwRet);
	}
	CA_DATA_MSG dataMsg = {0};
	dataMsg.dwSize = sizeof(dataMsg);
	dwRet = CAReadItem(hOperHandle, pUID, 0, CA_DATA_FORMAT_STRUCT, (LPVOID*)&dataMsg);
	if( dwRet == CONA_OK )
	{
		for(UINT i = 0; i < dataMsg.bAddressCount; i++)
		{	// Get addresses
//			m_ItemList.InsertItem(iItemCount, L"Address");
//			m_ItemList.SetItemText(iItemCount, 1, dataMsg.pAddress[i].pstrAddress);//dataMsg.pAddress[i].pstrAddress：电话号码//
			m_PhoNum=dataMsg.pAddress[i].pstrAddress;//获得电话号码
			iItemCount++;
		}
		if(dataMsg.dwDataLength > 0)
		{	// Get actual message data
			if((CA_GET_DATA_FORMAT(dataMsg.dwInfoField)) == CA_DATA_FORMAT_UNICODE)
			{	// Data is in Unicode text format
				if(dataMsg.pbData)
				{	// Data is there, get it
					CString strData((LPTSTR)dataMsg.pbData, dataMsg.dwDataLength / sizeof(wchar_t));
//					m_ItemList.InsertItem(iItemCount, L"Message");
//					m_ItemList.SetItemText(iItemCount, 1, strData);//strData：短信//
					m_Text=strData;//获得短信
					iItemCount++;
				}
				else
				{
					// No data, GMS or other kind of message?
				}
			}
			else
			{
				// Message in data, not text format
			}
		}
		// Get message date and format it
		SYSTEMTIME sTime = {0};
		wchar_t szDate[128] = {0};
		CADataDateToSystemTime(dataMsg.messageDate, sTime);
		GetFormattedDate(&sTime, szDate, 128);
//		m_ItemList.InsertItem(iItemCount, L"Date");
//		m_ItemList.SetItemText(iItemCount, 1, szDate);//szDate：时间//
		m_Time=szDate;//获得时间
		// 时间格式过滤
/*		int length=m_Time.GetLength();
		for (int j=0; j<length; j++)
		{
			if (m_Time[0]!=' ')
			{
				m_Time.Delete(0);
			}
			else
			{
				m_Time.Delete(0);
				break;
			}
		}
*/
		iItemCount++;
		// Get message status
//		m_ItemList.InsertItem(iItemCount, L"Status");
//		m_ItemList.SetItemText(iItemCount, 1, MessageStatusToString(dataMsg.dwInfoField));
		iItemCount++;
		// Free memory allocated by CA API
		dwRet = CAFreeItemData(m_hSMS, CA_DATA_FORMAT_STRUCT, (LPVOID)&dataMsg);
		if(dwRet != CONA_OK)
		{
			ErrorMessageDlg(_T("CAFreeItemData"), dwRet);
		}
	}
	else
	{
		ErrorMessageDlg(_T("CAReadItem"), dwRet);
	}
	dwRet = CAEndOperation(hOperHandle);
	if(dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("CAEndOperation"), dwRet);
	}
}

//===================================================================
// GetInfo
//
// 获得新短信
//
//===================================================================
void CPIMNavigatorDlg::GetInfo()
{
	OnInitDialog();
	RefreshTreeView();

	// Check CA connection to SMS folders and open it if needed
	DWORD dwDeviceIndex = GetCurrentDevice();
	DWORD dwRet = CheckSMSConnection(dwDeviceIndex);

	CA_FOLDER_INFO pFolderInfo = {0};
	pFolderInfo.dwSize = sizeof(pFolderInfo);
	dwRet = CAGetFolderInfo(m_hSMS, &pFolderInfo);

	m_iTargetCount=1;
	m_pFolderInfos.SetAtGrow(m_iTargetCount, new CA_FOLDER_INFO);
	memcpy(m_pFolderInfos[m_iTargetCount], &pFolderInfo, sizeof(CA_FOLDER_INFO));
	pFolderInfo=pFolderInfo.pSubFolders[0];
	m_iTargetCount=2;
	m_pFolderInfos.SetAtGrow(m_iTargetCount, new CA_FOLDER_INFO);
	memcpy(m_pFolderInfos[m_iTargetCount], &pFolderInfo, sizeof(CA_FOLDER_INFO));	
	
/*	for(DWORD i = 0; i < pFolderInfo->dwSubFolderCount; i++)
	{	// Recursive call for adding subfolders.
		AddFolder(strRootFolder, &pFolderInfo->pSubFolders[i], hItem, iIconFolderIndex, iIconItemIndex);
	}
*/
	if(dwRet == CONA_OK)
	{
		int iTarget = 2;//(int)m_treeNavigator.GetItemData(hTarget);
		// Set SMS folder target path
		CA_FOLDER_INFO* pTargetPath = (CA_FOLDER_INFO*)m_pFolderInfos[iTarget];
		// Check folder ID. In user folder root there is no messages
//		if(!pTargetPath->dwFolderId || pTargetPath->dwFolderId == CA_MESSAGE_FOLDER_USER_FOLDERS)
//		{
//			return;
//		}
		// Read all the SMS item UIDs from the connected device 
		CA_ID_LIST caIDList = {0};
		caIDList.dwSize = sizeof(caIDList);
		dwRet = CAGetIDList(m_hSMS, pTargetPath->dwFolderId, CA_OPTION_UPDATE_ITEM_STATUS, &caIDList); 
		if(dwRet != CONA_OK)
		{
			ErrorMessageDlg(_T("CAGetIDList"), dwRet);
		}
		CAOPERATIONHANDLE hOperHandle = NULL;
		dwRet = CABeginOperation(m_hSMS, 0, &hOperHandle);
		if(dwRet != CONA_OK)
		{
			ErrorMessageDlg(_T("CABeginOperation"), dwRet);
		}
		for(DWORD k = 0; k < caIDList.dwUIDCount; k++)
		{	// Read SMS item from the connected device
			CA_DATA_MSG dataMsg = {0};;
			dataMsg.dwSize = sizeof(dataMsg);
			dwRet = CAReadItem(hOperHandle, &caIDList.pUIDs[k], 0, CA_DATA_FORMAT_STRUCT, (LPVOID*)&dataMsg);
			if(dwRet == CONA_OK)
			{
				if(dataMsg.dwDataLength > 0)
				{
					if((CA_GET_DATA_FORMAT(dataMsg.dwInfoField)) == CA_DATA_FORMAT_UNICODE)
					{
						if(dataMsg.pbData)
						{	// Insert SMS message item in tree view
							CString strData((LPTSTR)dataMsg.pbData, dataMsg.dwDataLength / sizeof(wchar_t));
//							HTREEITEM hSubItem = m_treeNavigator.InsertItem(strData, m_iIconSMSIndex, m_iIconSMSIndex, hTarget);
							// m_iItemCount	短信数目，初始为0
							// m_pItemUIDs	创建新UID
							m_pItemUIDs.SetAtGrow(m_iItemCount, new CA_ITEM_ID);//<-
							memcpy(m_pItemUIDs[m_iItemCount], &caIDList.pUIDs[k], sizeof(CA_ITEM_ID));//<-
//							m_treeNavigator.SetItemData(hSubItem, m_iItemCount);
							m_iItemCount++;//<-
						}
						else
						{
							// No data, GMS or other kind of message?
						}
					}
					else
					{
						// Message in data format
					}
				}
				// Free memory allocated by CA API
				dwRet = CAFreeItemData(m_hSMS, CA_DATA_FORMAT_STRUCT, (LPVOID)&dataMsg);
				if(dwRet != CONA_OK)
				{
					ErrorMessageDlg(_T("CAFreeItemData"), dwRet);
				}
			}
			else
			{
				ErrorMessageDlg(_T("CAReadItem"), dwRet);
			}
		}
		dwRet = CAEndOperation(hOperHandle);
		if(dwRet != CONA_OK)
		{
			ErrorMessageDlg(_T("CAEndOperation"), dwRet);
		}
	}

	HandleInfo();
}

//===================================================================
// HandleInfo
//
// 处理新短信
//
//===================================================================
void CPIMNavigatorDlg::HandleInfo()
{
	CArray<CString, CString> arr;
	if (m_Mode == "评论")
	{
		// 获取关键字
		CString key;
		GetPrivateProfileString(_T("CommentSet"),_T("Key"),_T(""),key.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
		key.ReleaseBuffer();
		// 拆分关键字并存储
		int keyLen = key.GetLength();
		CString tmp = "";
		int k;
		for(k=0; k<keyLen; k++)
		{
			TCHAR t = key.GetAt(k);
			if (t == '/')
			{
				if (tmp != "")
				{
					arr.Add(tmp);
					tmp = "";
				}
				continue;
			}
			else
			{
				tmp += t;
			}
		}
		if (tmp != "")
			arr.Add(tmp);
	}
	
	// m_Array置空
	m_Array.RemoveAll();

	// 短信前缀长度
	int prolen = m_Pro.GetLength();

	// 筛选短信
	for (int dwItem=m_iItemCount-1; dwItem>=0; dwItem--)
	{
		// 读取短信信息
		GetSMSDetails((CA_ITEM_ID*)m_pItemUIDs[dwItem]);

		// 短信时间格式化		
		int i;
		// year
		CString iyear_str = "";
		for(i=0; m_Time.GetAt(i)!=_T('/'); i++)
		{
			iyear_str += m_Time.GetAt(i);
		}
		int iyear = _tstoi(LPCTSTR(iyear_str));
		// month
		CString imonth_str = "";
		for(i++; m_Time.GetAt(i)!=_T('/'); i++)
		{
			imonth_str += m_Time.GetAt(i);
		}
		int imonth = _tstoi(LPCTSTR(imonth_str));
		// day
		CString iday_str = "";
		for(i++; m_Time.GetAt(i)!=_T(' '); i++)
		{
			iday_str += m_Time.GetAt(i);
		}
		int iday = _tstoi(LPCTSTR(iday_str));
		// hour
		CString ihour_str = "";
		for(i++; m_Time.GetAt(i)!=_T(':'); i++)
		{
			ihour_str += m_Time.GetAt(i);
		}
		int ihour = _tstoi(LPCTSTR(ihour_str)) + CHINA_TIME;
		// minute
		CString imin_str = "";
		for(i++; i<m_Time.GetLength(); i++)
		{
			imin_str += m_Time.GetAt(i);
		}
		int imin = _tstoi(LPCTSTR(imin_str));
		// m_Time修正重组
		if (imin/10 == 0)
		{
			m_Time.Format(_T("%d/%d/%d %d:0%d"),iyear,imonth,iday,ihour,imin);
		}
		else
		{
			m_Time.Format(_T("%d/%d/%d %d:%d"),iyear,imonth,iday,ihour,imin);
		}

		// 短信时间与参照时间对比
		if (iyear < g_Year)
			break;
		else if (iyear == g_Year && imonth < g_Month)
			break;
		else if (iyear == g_Year && imonth == g_Month && iday < g_Day)
			break;
		else if (iyear == g_Year && imonth == g_Month && iday == g_Day && ihour < g_Hour)
			break;
		else if (iyear == g_Year && imonth == g_Month && iday == g_Day && ihour == g_Hour && imin < g_Min)
			break;

		// 信息初步过滤
		if (m_Mode == "评论")
		{
			// 飞信信息过滤
			if (m_PhoNum.GetLength() == 16 && m_PhoNum.Left(5) == "12520")
			{
				// 前缀
				int pos = 0;
				while(m_Text.GetAt(pos) != ':')
				{
					pos++;
				}
				m_Text.Delete(0,pos+1);

				int nSize = m_Text.GetLength();
				char *pString = new char[3*nSize];
				WideCharToMultiByte(CP_UTF8, 0, m_Text, -1, pString, 3*nSize, NULL, NULL);
				if (*pString == ' ')
				{
					m_Text.Delete(0);
				}

				// 后缀
				// 如【此短信通过手机飞信发送】【飞信提示：如涉及财物往来，请谨慎】等
				CString lbracket = "【";
				CString rbracket = "】";
				int blen = lbracket.GetLength();
				CString key = "飞信";
				int tlen = m_Text.GetLength();
				if (m_Text.Right(blen) == rbracket)
				{
					CString afind = m_Text;
					int bpos = -1;
					int sav = -1;
					do 
					{
						bpos = sav;
						sav = Sunday(afind,lbracket,true);
						if (sav != -1)
						{
							for (int w=0; w<blen; w++)
							{
								afind.SetAt(sav+w,_T('*'));
							}
						}
					} while (sav != -1);
					if (bpos != -1)
					{
						int cut = tlen - bpos;
						CString bck = m_Text.Right(cut);
						if (bck.Find(key) != -1)
							m_Text = m_Text.Left(bpos);
					}
				}
			}

			// 短信前缀过滤
			bool pro_flag = false;
			for(i=0; i<prolen; i++)
			{
				if (m_Pro.GetAt(i) != m_Text.GetAt(i))
				{
					pro_flag = true;
					break;
				}
			}
			if (pro_flag)
				continue;

			// 过滤器
			CString source = m_Text;
			// 关键字屏蔽为*
			for (int m=0; m<arr.GetSize(); m++)
			{
				CString target = arr[m];
				int tSize = target.GetLength();
				int pos = Sunday(source,target,true);
				while(pos != -1)
				{
					for (int n=0; n<tSize; n++)
					{
						source.SetAt(pos+n,_T('*'));
					}
					pos = Sunday(source,target,true);
				}
				m_Text = source;
			}
		}
		if (m_Mode == "投票")
		{
			// 飞信信息过滤
			if (m_PhoNum.GetLength() == 16 && m_PhoNum.Left(5) == "12520")
			{
				// 前缀
				int pos = 0;
				while(m_Text.GetAt(pos) != ':')
				{
					pos++;
				}
				m_Text.Delete(0,pos+1);

				int nSize = m_Text.GetLength();
				char *pString = new char[3*nSize];
				WideCharToMultiByte(CP_UTF8, 0, m_Text, -1, pString, 3*nSize, NULL, NULL);
				if (*pString == ' ')
				{
					m_Text.Delete(0);
				}

				// 后缀
				// 如【此短信通过手机飞信发送】【飞信提示：如涉及财物往来，请谨慎】等
				CString lbracket = "【";
				CString rbracket = "】";
				int blen = lbracket.GetLength();
				CString key = "飞信";
				int tlen = m_Text.GetLength();
				if (m_Text.Right(blen) == rbracket)
				{
					CString afind = m_Text;
					int bpos = -1;
					int sav = -1;
					do 
					{
						bpos = sav;
						sav = Sunday(afind,lbracket,true);
						if (sav != -1)
						{
							for (int w=0; w<blen; w++)
							{
								afind.SetAt(sav+w,_T('*'));
							}
						}
					} while (sav != -1);
					if (bpos != -1)
					{
						int cut = tlen - bpos;
						CString bck = m_Text.Right(cut);
						if (bck.Find(key) != -1)
							m_Text = m_Text.Left(bpos);
					}
				}
			}
		}

		// 已有短信过滤
		if (m_Mode == "评论")
		{
			if(CommentMode(iyear,imonth,iday,ihour,imin))
				break;
		}
		if (m_Mode == "投票")
		{
			if (!g_pRecordset2->BOF)
			{
				g_pRecordset2->MoveFirst();
				for (i=0; i<g_Index; i++)
				{
					g_pRecordset2->MoveNext();
				}
			}
			VoteMode();
		}		
	}

	if (m_Array.GetSize() != 0)
	{
		// 修改参照时间
		const int item = m_iItemCount-1;
		GetSMSDetails((CA_ITEM_ID*)m_pItemUIDs[item]);	
		int j;
		// year
		CString cyear_str = "";
		for(j=0; m_Time.GetAt(j)!=_T('/'); j++)
		{
			cyear_str += m_Time.GetAt(j);
		}
		g_Year = _tstoi(LPCTSTR(cyear_str));
		// month
		CString cmonth_str = "";
		for(j++; m_Time.GetAt(j)!=_T('/'); j++)
		{
			cmonth_str += m_Time.GetAt(j);
		}
		g_Month = _tstoi(LPCTSTR(cmonth_str));
		// day
		CString cday_str = "";
		for(j++; m_Time.GetAt(j)!=_T(' '); j++)
		{
			cday_str += m_Time.GetAt(j);
		}
		g_Day = _tstoi(LPCTSTR(cday_str));
		// hour
		CString chour_str = "";
		for(j++; m_Time.GetAt(j)!=_T(':'); j++)
		{
			chour_str += m_Time.GetAt(j);
		}
		g_Hour = _tstoi(LPCTSTR(chour_str)) + CHINA_TIME;
		// minute
		CString cmin_str = "";
		for(j++; j<m_Time.GetLength(); j++)
		{
			cmin_str += m_Time.GetAt(j);
		}
		g_Min = _tstoi(LPCTSTR(cmin_str));

		// m_Array写入数据库
		try
		{
			if (m_Mode == "评论")
				Recordset(g_pRecordset1);
			if (m_Mode == "投票")
				Recordset(g_pRecordset2);
		}
		catch(...)
		{
			AfxMessageBox(_T("写入数据库错误!"));
		}

		// 短信加入列表
		for (j=0; j<m_Array.GetCount(); j++)
		{
			int index = g_List.GetItemCount();
			g_List.InsertItem(index, m_Mode);
			g_List.SetItemText(index, 1, m_Array[j].time);
			g_List.SetItemText(index, 2, m_Array[j].phonum);
			g_List.SetItemText(index, 3, m_Array[j].text);
		}
	}
}

//===================================================================
// Sunday
//
// 过滤器算法
//
//===================================================================
int CPIMNavigatorDlg::Sunday(LPCTSTR lpctS, LPCTSTR lpctT, BOOL bNoCase)
{
	int i, nSize, nPos=0;
	int nLenS, nLenT;
	CString cstrS, cstrT, cstrSTemp;
	int *nNext;

	cstrS=lpctS;
	cstrT=lpctT;
	nLenS=cstrS.GetLength();
	nLenT=cstrT.GetLength();
	if(sizeof(TCHAR)==sizeof(char))
		nSize=0x100;
	else
		nSize=0x10000;
	nNext=(int*)malloc(nSize*sizeof(int));
	if(NULL==nNext)
		return -2;
	memset(nNext, 0, nSize*sizeof(int));

	for(i=0; i<nSize; i++)
		nNext[i]=nLenT+1;
	for(i=0; i<nLenT; i++)
		nNext[cstrT.GetAt(i)]=nLenT-i;

	while( nPos<(nLenS-nLenT) )
	{
		cstrSTemp=cstrS.Mid(nPos, nLenT);
		if (bNoCase)
		{
			if (cstrSTemp.CompareNoCase(cstrT))
				nPos+=nNext[cstrS.GetAt(nPos+nLenT)];
			else
			{
				free(nNext);
				nNext=NULL;
				return nPos;
			}
		}
		else
		{
			if (cstrSTemp.Compare(cstrT))
				nPos+=nNext[cstrS.GetAt(nPos+nLenT)];
			else
			{
				free(nNext);
				nNext=NULL;
				return nPos;
			}
		}
	}
	if (nLenS-nLenT==nPos)
	{
		cstrSTemp=cstrS.Mid(nPos, nLenT);
		if (bNoCase)
		{
			if (!cstrSTemp.CompareNoCase(cstrT))
			{
				free(nNext);
				nNext=NULL;
				return nPos;
			}
		}
		else
		{
			if (!cstrSTemp.Compare(cstrT))
			{
				free(nNext);
				nNext=NULL;
				return nPos;
			}
		}
	}
	free(nNext);
	nNext=NULL;
	return -1;
}

//===================================================================
// CommentMode
// return false-插入 true-排除
//
// 评论模式下的已有短信过滤
//
//===================================================================
bool CPIMNavigatorDlg::CommentMode(int iyear,int imonth,int iday,int ihour,int imin)
{
	_variant_t var;

	// 过滤前缀
	int prolen = m_Pro.GetLength();
	m_Text.Delete(0,prolen);

	try
	{
		if(g_pRecordset1->BOF)
		{
			m_Array.InsertAt(0,MessageSave(m_Time,m_PhoNum,m_Text));
			return false;
		}
		else
		{
			g_pRecordset1->MoveLast();

			// 获取最后一条记录的时间
			CString time = "";
			var = g_pRecordset1->GetCollect("time");
			if(var.vt != VT_NULL)
				time = (LPCSTR)_bstr_t(var);

			// 时间格式化
			int i;
			// year
			CString year_str = "";
			for(i=0; time.GetAt(i)!=_T('/'); i++)
			{
				year_str += time.GetAt(i);
			}
			int year = _tstoi(LPCTSTR(year_str));
			// month
			CString month_str = "";
			for(i++; time.GetAt(i)!=_T('/'); i++)
			{
				month_str += time.GetAt(i);
			}
			int month = _tstoi(LPCTSTR(month_str));
			// day
			CString day_str = "";
			for(i++; time.GetAt(i)!=_T(' '); i++)
			{
				day_str += time.GetAt(i);
			}
			int day = _tstoi(LPCTSTR(day_str));
			// hour
			CString hour_str = "";
			for(i++; time.GetAt(i)!=_T(':'); i++)
			{
				hour_str += time.GetAt(i);
			}
			int hour = _tstoi(LPCTSTR(hour_str));
			// minute
			CString min_str = "";
			for(i++; i<time.GetLength(); i++)
			{
				min_str += time.GetAt(i);
			}
			int min = _tstoi(LPCTSTR(min_str));

			// 时间对比
			if ((imin > min && ihour == hour && iday == day && imonth == month && iyear == year) ||
				(ihour > hour && iday == day && imonth == month && iyear == year) ||
				(iday > day && imonth == month && iyear == year) ||
				(imonth > month && iyear == year) ||
				(iyear > year)
				)
			{
				m_Array.InsertAt(0,MessageSave(m_Time,m_PhoNum,m_Text));
				return false;
			}
			else if (!(imin == min && ihour == hour && iday == day && imonth == month && iyear == year))
			{
				return true;
			}

			// 同一分钟内的短信过滤
			// 获取最后一条记录的电话号码
			CString number = "";
			var = g_pRecordset1->GetCollect("number");
			if(var.vt != VT_NULL)
				number = (LPCSTR)_bstr_t(var);

			// 电话号码对比
			int numlen = number.GetLength();
			if (m_PhoNum.GetLength() != numlen)
			{
				return false;
			}
			for (i=numlen-1; i>=0; i--)
			{
				if (m_PhoNum.GetAt(i) != number.GetAt(i))
				{
					m_Array.InsertAt(0,MessageSave(m_Time,m_PhoNum,m_Text));
					return false;
				}
			}

			// 获取最后一条记录的短信内容
			CString text = "";
			var = g_pRecordset1->GetCollect("text");
			if(var.vt != VT_NULL)
				text = (LPCSTR)_bstr_t(var);

			// 短信内容对比
			int len1 = m_Text.GetLength();
			int len2 = text.GetLength();
			if (len1 != len2)
			{
				m_Array.InsertAt(0,MessageSave(m_Time,m_PhoNum,m_Text));
				return false;
			}
			else
			{
				for (i=len1-1; i>=0; i--)
				{
					if (m_Text.GetAt(i) != text.GetAt(i))
					{
						m_Array.InsertAt(0,MessageSave(m_Time,m_PhoNum,m_Text));
						return false;
					}
				}
			}
			return true;
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("数据读取错误!"));
		return true;
	}
}

//===================================================================
// VoteMode
// return false-插入 true-排除
//
// 投票模式下的已有短信过滤
//
//===================================================================
bool CPIMNavigatorDlg::VoteMode()
{
	_variant_t var;
	int i;
	bool end_flag;

	// 短信内容对比
	int len = m_Text.GetLength();
	if (len != 1)
	{
		return true;
	}
	CString str = m_Text.GetAt(0);
	int num = _tstoi(LPCTSTR(str));
	if (num <= 0 || num > m_Num)
	{
		return true;
	}

	if (!g_pRecordset2->BOF)
	{
		for (i=0; !g_pRecordset2->adoEOF; g_pRecordset2->MoveNext())
		{
			// 循环结束标记
			end_flag = true;

			// 获取数据表的电话号码
			CString number = "";
			var = g_pRecordset2->GetCollect("number");
			if(var.vt != VT_NULL)
				number = (LPCSTR)_bstr_t(var);

			// 电话号码对比
			int numlen = number.GetLength();
			if (m_PhoNum.GetLength() != numlen)
			{
				end_flag = false;
			}
			else
			{
				for (i=numlen-1; i>=0; i--)
				{
					if (m_PhoNum.GetAt(i) != number.GetAt(i))
					{
						end_flag = false;
						break;
					}
				}
			}
			if (end_flag)
			{
				return true;
			}
		}
	}

	// m_Array内部对比
	for (int j=0; j<m_Array.GetCount(); j++)
	{
		// 循环结束标记
		end_flag = true;

		// 电话号码对比
		int numlen = m_Array[j].phonum.GetLength();
		if (m_PhoNum.GetLength() != numlen)
		{
			end_flag = false;
		}
		else
		{
			for (i=numlen-1; i>=0; i--)
			{
				if (m_PhoNum.GetAt(i) != m_Array[j].phonum.GetAt(i))
				{
					end_flag = false;
				}
			}
		}
		if (end_flag)
		{
			return true;
		}
	}

	m_Array.InsertAt(0,MessageSave(m_Time,m_PhoNum,m_Text));
	return false;
}

//===================================================================
// Recordset
//
// 记录集保存数据
//
//===================================================================
void CPIMNavigatorDlg::Recordset(_RecordsetPtr pRecordset)
{
	for (int i=0; i<m_Array.GetCount(); i++)
	{
		pRecordset->AddNew();
		pRecordset->PutCollect("time", _variant_t(m_Array[i].time));
		pRecordset->PutCollect("number", _variant_t(m_Array[i].phonum));
		pRecordset->PutCollect("text", _variant_t(m_Array[i].text));
		pRecordset->Update();
	}
}

//===================================================================
// ModeSet
//
// 模式设置
//
//===================================================================
void CPIMNavigatorDlg::ModeSet(CString str)
{
	m_Mode = str;
}

//===================================================================
// GetReady
//
// 根据模式设定参数
//
//===================================================================
void CPIMNavigatorDlg::GetReady()
{
	if (m_Mode == "评论")
	{
		GetPrivateProfileString(_T("CommentSet"),_T("Pro"),_T(""),m_Pro.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
		m_Pro.ReleaseBuffer();
		GetPrivateProfileString(_T("CommentSet"),_T("Key"),_T(""),m_Key.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
		m_Key.ReleaseBuffer();
		m_ListTime1 = GetPrivateProfileInt(_T("CommentSet"),_T("LInterval"),10,_T(".\\Data\\Config.ini"));
		m_ShowTime = GetPrivateProfileInt(_T("CommentSet"),_T("SInterval"),10,_T(".\\Data\\Config.ini"));
		return;
	}
	if (m_Mode == "投票")
	{
//		int Index;
		if (!g_pRecordset2->BOF)
		{
			g_pRecordset2->MoveFirst();
			int calc;
			for (calc=0; !g_pRecordset2->adoEOF; g_pRecordset2->MoveNext())
			{
				calc++;
			}
			g_Index = calc-1;
//			Index = g_pRecordset2->RecordCount - 1;
		}
		else
		{
			g_Index = -1;
//			Index = -1;
		}
//		if (g_Index != Index)
//		{
//			AfxMessageBox(_T("Error!"));
//		}
		m_Max = GetPrivateProfileInt(_T("VoteSet"),_T("Max"),0,_T(".\\Data\\Config.ini"));
		m_Num = GetPrivateProfileInt(_T("VoteSet"),_T("Num"),3,_T(".\\Data\\Config.ini"));
		GetPrivateProfileString(_T("VoteSet"),_T("Item"),_T(""),m_Item.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
		m_Item.ReleaseBuffer();
		m_ListTime2 = GetPrivateProfileInt(_T("VoteSet"),_T("LInterval"),10,_T(".\\Data\\Config.ini"));
		return;
	}
	if (m_Mode == "抽奖")
	{
		GetPrivateProfileString(_T("LotterySet"),_T("Participant"),_T("全部"),m_Participant.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
		m_Participant.ReleaseBuffer();
		GetPrivateProfileString(_T("LotterySet"),_T("Chance"),_T("一号多票"),m_Chance.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
		m_Chance.ReleaseBuffer();
		return;
	}
}

//===================================================================
// GetCurrentDevice
//
// Get index of currently selected device in serial number vector.
//
//===================================================================
int CPIMNavigatorDlg::GetCurrentDevice()
{
	return 0;
}

//===================================================================
// ClearListView
//
// Clear previous details in list view
//
//===================================================================
void CPIMNavigatorDlg::ClearListView() 
{
	if(m_StaticPicture.GetBitmap())
	{
		RECT rect, rect2;
		GetWindowRect(&rect);
		m_StaticPicture.GetWindowRect(&rect2);
		MoveWindow(rect.left, rect.top, rect.right - rect.left - rect2.right + rect2.left - 10, rect.bottom - rect.top);
		m_StaticPicture.SetBitmap(0);
	}
	m_ItemList.DeleteAllItems();
}

//===================================================================
// OnSelchangedTreeNavigator 
//
// User has selected new item in tree view, update details list view
//
//===================================================================
void CPIMNavigatorDlg::OnSelchangedTreeNavigator(NMHDR* pNMHDR, LRESULT* pResult) 
{
    CWaitCursor wait;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM newItem = pNMTreeView->itemNew;
	*pResult = 0;

	ClearListView();
	m_hCurrentItem = NULL;
	m_hCurrentTarget = NULL;

	if(m_bIsLoadingData)
	{	// Tree view is being refreshed, wait...
		return;
	}

	m_hCurrentItem = newItem.hItem;

	DWORD dwDeviceIndex = GetCurrentDevice();

	HTREEITEM hParent = m_treeNavigator.GetParentItem(m_hCurrentItem);
	DWORD dwItem = (int)m_treeNavigator.GetItemData(m_hCurrentItem);
	int iImage, iSelImage;
	m_treeNavigator.GetItemImage(m_hCurrentItem, iImage, iSelImage);
    if(iImage == m_iIconPhoneIndex)
	{
        return;	// Phone item is selected
	}
	else if(iImage == m_iIconSMSMessagesIndex)
	{
		m_hCurrentTarget = m_hCurrentItem;
		// Check CA connection to SMS folders and open it if needed
		CheckSMSConnection(dwDeviceIndex);
		// SMS folder selected
		m_hCurrentConnection = m_hSMS;
		int iImageParent, iSelImageParent;
		m_treeNavigator.GetItemImage(hParent, iImageParent, iSelImageParent);
		
		// Check if user created folder
		CA_FOLDER_INFO* pFolderInfo = (CA_FOLDER_INFO*)m_pFolderInfos[dwItem];		
	}
	else if(iImage == m_iIconSMSIndex)
	{	// SMS item is selected
		CheckSMSConnection(dwDeviceIndex);
		m_hCurrentConnection = m_hSMS;
		GetSMSDetails((CA_ITEM_ID*)m_pItemUIDs[dwItem]);
		// Set handle to current folder item
		m_hCurrentTarget = hParent;
	}
}

//===================================================================
// RemoveChildItems 
//
// Removes all child items of current target
//
//===================================================================
void CPIMNavigatorDlg::RemoveChildItems(HTREEITEM hTarget)
{
	for(;;)
	{
		HTREEITEM hChild = m_treeNavigator.GetChildItem(hTarget);
		if(!hChild)
		{
			break;
		}
		DWORD dwItemData = (DWORD)m_treeNavigator.GetItemData(hChild);
		if(dwItemData != -1)
		{
			RemoveChildItems(hChild);
			CA_ITEM_ID* pUID = (CA_ITEM_ID*)m_pItemUIDs[dwItemData];
			if(pUID && pUID->dwSize == sizeof(CA_ITEM_ID))
			{
				delete m_pItemUIDs[dwItemData];
				m_pItemUIDs[dwItemData] = NULL;
			}
			else
			{
				delete m_pFolderInfos[dwItemData];
				m_pFolderInfos[dwItemData] = NULL;
			}
		}
		m_treeNavigator.DeleteItem(hChild);
	}
}

//===================================================================
// WriteItem
//
// Writes new item to phone
//
//===================================================================
void CPIMNavigatorDlg::WriteItem(DWORD dwFolderId, LPVOID pItemData)
{
    CWaitCursor wait;
	CAOPERATIONHANDLE hOperHandle = NULL;
	DWORD dwRet = CABeginOperation(m_hCurrentConnection, 0, &hOperHandle);
	if(dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("CABeginOperation"), dwRet);
	}
	COperationNotify* pOpNotify = new COperationNotify;
	dwRet = CARegisterOperationNotifyIF(hOperHandle, API_REGISTER, pOpNotify);
	if(dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("CARegisterOperationNotifyIF"), dwRet);
	}
	CA_ITEM_ID itemUid = {0};
	itemUid.dwSize = sizeof(itemUid);
	itemUid.dwFolderId = dwFolderId;
	dwRet = CAWriteItem(hOperHandle, &itemUid, 0, CA_DATA_FORMAT_STRUCT, pItemData);
	if(dwRet == CONA_OK)
	{
		dwRet = CACommitOperations(hOperHandle, NULL);	
		if(dwRet != CONA_OK)
		{
			ErrorMessageDlg(_T("CACommitOperations"), dwRet);
		}
	}
	else
	{
		ErrorMessageDlg(_T("DAWriteItem"), dwRet);
	}
	// Free memory allocated by CA API
	dwRet = CAFreeItemData(m_hCurrentConnection, CA_DATA_FORMAT_STRUCT, pItemData);
	if(dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("CAFreeItemData"), dwRet);
	}
	dwRet = CARegisterOperationNotifyIF(hOperHandle, API_UNREGISTER, pOpNotify);
	if(dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("CARegisterOperationNotifyIF"), dwRet);
	}
	delete pOpNotify;
	dwRet = CAEndOperation(hOperHandle);
	if(dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("CAEndOperation"), dwRet);
	}
}

//===================================================================
// ShowNewSMSDlg 
//
// Show "New SMS Message" dialog and create SMS if user clicks OK
//
//===================================================================
void CPIMNavigatorDlg::ShowNewSMSDlg()
{	// Open "New SMS Message" dialog
	/*CSmsMessageDlg dlg(this);
	if(dlg.DoModal() == IDOK)
	{	// User has filled SMS information in dialog and clicked OK
		CA_DATA_MSG dataMsg = {0};
		dataMsg.dwSize = sizeof(dataMsg);
		CA_SET_DATA_FORMAT(dataMsg.dwInfoField, CA_DATA_FORMAT_UNICODE);
		CA_SET_DATA_CODING(dataMsg.dwInfoField, CA_DATA_CODING_UNICODE);
		CA_SET_MESSAGE_STATUS(dataMsg.dwInfoField, CA_MESSAGE_STATUS_DRAFT);
		CA_SET_MESSAGE_TYPE(dataMsg.dwInfoField, CA_SMS_SUBMIT);
		dlg.GetData(dataMsg);
		int iTarget = (int)m_treeNavigator.GetItemData(m_hCurrentTarget);
		CA_FOLDER_INFO* pTargetPath = (CA_FOLDER_INFO*)m_pFolderInfos[iTarget];
		// Set message date
		GetCurrentCADate(dataMsg.messageDate);
		// Write new SMS item to currently connected device
		WriteItem(pTargetPath->dwFolderId, &dataMsg);
		RemoveChildItems(m_hSMSFolder);
		iTarget = (int)m_treeNavigator.GetItemData(m_hSMSFolder);
		pTargetPath = (CA_FOLDER_INFO*)m_pFolderInfos[iTarget];
		for(DWORD i = 0; i < pTargetPath->dwSubFolderCount; i++)
		{
			AddFolder(L"SMS Messages", &pTargetPath->pSubFolders[i], m_hSMSFolder, m_iIconSMSMessagesIndex, m_iIconSMSIndex);
		}
	}*/
}

//===================================================================
// DeleteFolder
//
// Deletes PIM folder from currently connected device
//
//===================================================================
void CPIMNavigatorDlg::DeleteFolder(int iItem)
{
	if(IDYES != MessageBox(_T("Are you sure you want to delete selected folder?"), _T("Confirm Folder Delete"), MB_YESNO|MB_ICONQUESTION))
	{
		return;
	}
	CA_FOLDER_INFO* pFolderInfo = (CA_FOLDER_INFO*)m_pFolderInfos[iItem];
	CAOPERATIONHANDLE hOperHandle = NULL;
	DWORD dwRet = CABeginOperation(m_hCurrentConnection, 0, &hOperHandle);
	if(dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("CABeginOperation"), dwRet);
	}
	CA_ITEM_ID itemId = {0};
	itemId.dwSize = sizeof(itemId);
	itemId.dwFolderId = pFolderInfo->dwFolderId;
	dwRet = CADeleteFolder(hOperHandle, &itemId); 
	if(dwRet == CONA_OK)
	{
		dwRet = CACommitOperations(hOperHandle, NULL);	
		if(dwRet == CONA_OK)
		{	// Delete item from tree view
			m_treeNavigator.DeleteItem(m_hCurrentItem);
			m_hCurrentItem = NULL;
		}
		else
		{
			ErrorMessageDlg(_T("CACommitOperations"), dwRet);
		}
	}
	else
	{
		ErrorMessageDlg(_T("CADeleteFolder"), dwRet);
	}
	dwRet = CAEndOperation(hOperHandle);
	if(dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("CAEndOperation"), dwRet);
	}
}

//===================================================================
// DeleteItem
//
// Deletes PIM item from currently connected device
//
//===================================================================
void CPIMNavigatorDlg::DeleteItem(int iItem)
{
	if(IDYES != MessageBox(_T("Are you sure you want to delete selected item?"), _T("Confirm Item Delete"), MB_YESNO|MB_ICONQUESTION))
	{
		return;
	}
	CA_ITEM_ID* pUID = (CA_ITEM_ID*)m_pItemUIDs[iItem];
	CAOPERATIONHANDLE hOperHandle = NULL;
	DWORD dwRet = CABeginOperation(m_hCurrentConnection, 0, &hOperHandle);
	if(dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("CABeginOperation"), dwRet);
	}
	dwRet = CADeleteItem(hOperHandle, pUID, 0);
	if(dwRet == CONA_OK)
	{
		dwRet = CACommitOperations(hOperHandle, NULL);	
		if(dwRet == CONA_OK)
		{	// Delete item from tree view
			m_treeNavigator.DeleteItem(m_hCurrentItem);
			m_hCurrentItem = NULL;
		}
		else
		{
			ErrorMessageDlg(_T("CACommitOperations"), dwRet);
		}
	}
	else
	{
		ErrorMessageDlg(_T("DADeleteItem"), dwRet);
	}
	dwRet = CAEndOperation(hOperHandle);
	if(dwRet != CONA_OK)
	{
		ErrorMessageDlg(_T("CAEndOperation"), dwRet);
	}
}

//===================================================================
// OnButtonRefresh
//
// User has clicked "Refresh" button
//
//===================================================================
void CPIMNavigatorDlg::OnButtonRefresh() 
{
	// Fill connected in tree view
	RefreshTreeView();
}