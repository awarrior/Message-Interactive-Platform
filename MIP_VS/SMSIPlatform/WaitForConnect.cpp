// WaitForConnect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "WaitForConnect.h"

// ȫ�ֱ���
extern bool g_Connected;

// ��̬����
bool CWaitForConnect::m_ok = false;
SOCKET CWaitForConnect::m_sockConn = NULL;
SOCKET* CWaitForConnect::m_sockSrv = NULL;
SOCKADDR_IN* CWaitForConnect::m_addrClient = NULL;   
int* CWaitForConnect::m_len = NULL;

// CWaitForConnect �Ի���

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


// CWaitForConnect ��Ϣ�������

void CWaitForConnect::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_flag = false;
	m_ok = false;
	m_nTimer = SetTimer(1, 500, 0);
	m_nTimer2 = SetTimer(2, 500, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CWaitForConnect::UpdateSockConn()
{
	//����Ϊ������ģʽ
	unsigned long flg=1;
	if (ioctlsocket(*m_sockSrv,FIONBIO,&flg)!=0)  
	{  
		closesocket(*m_sockSrv);  
		return;
	}

	m_sockConn = accept(*m_sockSrv,(SOCKADDR*)m_addrClient,m_len);

	//����Ϊ����ģʽ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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