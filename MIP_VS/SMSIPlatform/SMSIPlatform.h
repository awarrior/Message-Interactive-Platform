// SMSIPlatform.h : SMSIPlatform Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CSMSIPlatformApp:
// �йش����ʵ�֣������ SMSIPlatform.cpp
//

class CSMSIPlatformApp : public CWinApp
{
public:
	CSMSIPlatformApp();
	~CSMSIPlatformApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CSMSIPlatformApp theApp;