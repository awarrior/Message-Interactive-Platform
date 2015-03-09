// SMSIPlatform.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "MainFrm.h"

#include "SMSIPlatformDoc.h"
#include "SMSIPlatformView.h"

#include "PCCSUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����ADO���ӱ���ָ��
_ConnectionPtr g_pConnection;

// CSMSIPlatformApp

BEGIN_MESSAGE_MAP(CSMSIPlatformApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CSMSIPlatformApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CSMSIPlatformApp ����

CSMSIPlatformApp::CSMSIPlatformApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

CSMSIPlatformApp::~CSMSIPlatformApp()
{
	// Terminate Common Functions API
//	CFAPI_Terminate(NULL);
	// Terminate Content Access API
//	CAAPI_Terminate(NULL);
	// Terminate Device Management API
//	DMAPI_Terminate(NULL);
//		return FALSE;
}


// Ψһ��һ�� CSMSIPlatformApp ����

CSMSIPlatformApp theApp;


// CSMSIPlatformApp ��ʼ��

BOOL CSMSIPlatformApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSMSIPlatformDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CSMSIPlatformView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//-> NOKIA API ��ʼ��
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
	//<- NOKIA API ��ʼ��

	//-> ��ʼ��COM,����ADO���Ӳ���
	//AfxOleInit();
	g_pConnection.CreateInstance(__uuidof(Connection));
	//<- ��ʼ��COM,����ADO���Ӳ���

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����


	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CSMSIPlatformApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CSMSIPlatformApp ��Ϣ�������


int CSMSIPlatformApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	// �ر�ADO����״̬
	if(g_pConnection->State)
		g_pConnection->Close();
	g_pConnection= NULL;

	return CWinApp::ExitInstance();
}
