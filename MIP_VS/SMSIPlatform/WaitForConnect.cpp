// WaitForConnect.cpp : 实现文件
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "WaitForConnect.h"

// 全局变量
extern bool g_Connected;

// 静态变量
bool CWaitForConnect::m_ok = false;
SOCKET CWaitForConnect::m_sockConn = NULL;
SOCKET* CWaitForConnect::m_sockSrv = NULL;
SOCKADDR_IN* CWaitForConnect::m_addrClient = NULL;   
int* CWaitForConnect::m_len = NULL;

// CWaitForConnect 对话框

IMPLEMENT_DYNAMIC(CWaitForConnect, CDialog)

CWaitForConnect::CWaitForConnect(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitForConnect::IDD, pParent)
{

}

CWaitForConnect::~CWaitForConnect()
{
}

void CWaitForConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaitForConnect, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWaitForConnect 消息处理程序

void CWaitForConnect::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}

void CWaitForConnect::ParamInit(SOCKET* sockSrv, SOCKADDR_IN* addrClient, int* len)
{
	m_sockSrv = sockSrv;
	m_addrClient = addrClient;
	m_len = len;
}

SOCKET CWaitForConnect::ReturnConn()
{
	return m_sockConn;
}

BOOL CWaitForConnect::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_flag = false;
	m_ok = false;
	m_nTimer = SetTimer(1, 500, 0);
	m_nTimer2 = SetTimer(2, 500, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CWaitForConnect::UpdateSockConn()
{
	//设置为非阻塞模式
	unsigned long flg=1;
	if (ioctlsocket(*m_sockSrv,FIONBIO,&flg)!=0)  
	{  
		closesocket(*m_sockSrv);  
		return;
	}

	m_sockConn = accept(*m_sockSrv,(SOCKADDR*)m_addrClient,m_len);

	//设置为阻塞模式
	flg=0;  
	if (ioctlsocket(*m_sockSrv,FIONBIO,&flg)!=0)  
	{  
		closesocket(*m_sockSrv);  
		return;
	}
}

DWORD WINAPI CWaitForConnect::Proc(LPVOID lpParameter)
{
	UpdateSockConn();

	if (m_sockConn != SOCKET_ERROR)
	{
		m_ok = true;
	}

	return 0;
}

void CWaitForConnect::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		if (!m_ok)
		{
			m_hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Proc,NULL,0,NULL);
			CloseHandle(m_hThread);
			//m_flag = true;
		}
		else
		{
			g_Connected = true;
			KillTimer(m_nTimer);
			CDialog::OnOK();
		}
	}
	if (nIDEvent == 2)
	{
		SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		KillTimer(m_nTimer2);
	}

	CDialog::OnTimer(nIDEvent);
}