/*
Filename    : PIMNavigatorDlg.h
Part of     : PIM Navigator
Description : Declaration of PIMNavigator's dialog class
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

#ifndef __NOKIA_PCCS_APPLICATION_PIMNAVIGATOR_EXAMPLE__PIMNAVIGATORDLG__
#define __NOKIA_PCCS_APPLICATION_PIMNAVIGATOR_EXAMPLE__PIMNAVIGATORDLG__

#if _MSC_VER > 1000
#pragma once
#endif 

#include "afxwin.h"
#include "MessageSave.h"

/////////////////////////////////////////////////////////////////////////////
// Defines
/////////////////////////////////////////////////////////////////////////////

#define MAX_CONNECTIONS			10

/////////////////////////////////////////////////////////////////////////////
// COperationNotify class
/////////////////////////////////////////////////////////////////////////////

class COperationNotify : public ICAOperationNotify
{
	// ICAOperationNotify
	virtual DWORD OnCAOperationNotify(DWORD dwOperation, DWORD dwCurrent, DWORD dwTotalAmount, DWORD dwStatus, CA_ITEM_ID * pItemID);
};

/////////////////////////////////////////////////////////////////////////////
// CPIMNavigatorDlg dialog
/////////////////////////////////////////////////////////////////////////////

class CPIMNavigatorDlg : public CDialog, IDMAPIDeviceNotify, ICANotify
{

public:

	CPIMNavigatorDlg(CWnd* pParent = NULL);
	virtual ~CPIMNavigatorDlg();
	
    // IDMAPIDeviceNotify methods
	void OnDeviceListUpdated();
    void OnDeviceAdded(const WCHAR* pstrSN, DWORD dwStatus);
    void OnDeviceRemoved(const WCHAR* pstrSN, DWORD dwStatus);
    void OnDeviceUpdated(const WCHAR* pstrSN, DWORD dwStatus);

	// ICANotify
	virtual DWORD OnCANotify(DWORD dwReason, DWORD dwStatus, CA_ITEM_ID *pItemID);

	//{{AFX_DATA(CPIMNavigatorDlg)
	enum { IDD };//= IDD_PIMNAVIGATOR_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPIMNavigatorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	
public:

	//{{AFX_MSG(CPIMNavigatorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreeNavigator(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnItemexpandedTreeNavigator(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnButtonRefresh();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CStatic m_StaticPicture;
	CButton	m_btnNewItem;
	CButton	m_btnNewFolder;
	CButton	m_btnDelete;
	CButton m_btnSave;
	CListCtrl m_ItemList;
	CTreeCtrl m_treeNavigator;
	int m_iTargetCount;
	int m_iItemCount;
	int m_iIconPhoneIndex;
	int m_iIconNoPhoneIndex;
	int m_iIconContactsIndex;
	int m_iIconSMSMessagesIndex;
	int m_iIconSMSIndex;
	CImageList m_ImageList;
	CPtrArray m_pFolderInfos;
	CPtrArray m_pItemUIDs;
	DWORD		m_dwCurrentSMSConnection;
	wchar_t		m_strSerialNumberTable[16][MAX_CONNECTIONS];
	DMHANDLE	m_hDM;
	BOOL		m_bIsLoadingData;
	CAHANDLE	m_hSMS;
	CAHANDLE	m_hCurrentConnection;
	HTREEITEM	m_hCurrentItem;
	HTREEITEM	m_hCurrentTarget;
	HTREEITEM	m_hSMSFolder;
	// 接收手机连接变量
	WCHAR m_pstrSN;
	DWORD m_dwStatus;

	void ShowNoPicture();
	DWORD CloseConnection(CAHANDLE& hCA);
	DWORD CheckSMSConnection(DWORD dwDeviceIndex);
	int GetCurrentDevice();
	void FreeItemData();
	void ClearListView();
	void RemoveChildItems(HTREEITEM hTarget);
	void GetSMSFolders(DWORD dwDeviceIndex, HTREEITEM hPhone);
	HTREEITEM AddFolder(CString strRootFolder, CA_FOLDER_INFO* pFolderInfo, HTREEITEM hParent, int iIconFolderIndex, int iIconItemIndex);
	void GetSMSMessages(DWORD dwDeviceIndex, HTREEITEM hTarget);
	void GetSMSDetails(CA_ITEM_ID* pUID);
	void ShowNewSMSDlg();
	void WriteItem(DWORD dwFolderId, LPVOID pItemData);
	void DeleteFolder(int iItem);
	void DeleteItem(int iItem);
	int Sunday(LPCTSTR lpctS, LPCTSTR lpctT, BOOL bNoCase);
	
private:
	CString m_Time;			// 短信时间
	CString m_PhoNum;		// 短信号码
	CString m_Text;			// 短信内容
	CString m_Mode;			// 模式
	CArray<MessageSave,MessageSave&> m_Array;	// 待插入数组
	
	CString m_Pro;			// 短信前缀
	CString m_Key;			// 关键字
	int m_ListTime1;		// 列表刷新间隔
	int m_ShowTime;			// 演示刷新间隔

	int m_Max;				// 票数最大值
	int m_Num;				// 项数
	CString m_Item;			// 项目内容
	int m_ListTime2;		// 列表刷新间隔

	CString m_Participant;	// 参与人
	CString m_Chance;		// 机会控制

public:
	void RefreshTreeView();
	void GetInfo();
	void HandleInfo();
	bool CommentMode(int iyear,int imonth,int iday,int ihour,int imin);
	bool VoteMode();
	void Recordset(_RecordsetPtr pRecordset);
	void ModeSet(CString str);
	void GetReady();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
