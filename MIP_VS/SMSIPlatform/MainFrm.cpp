// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "MainFrm.h"
#include "Config.h"
#include "ColorListCtrl.h"
#include "WaitForConnect.h"
#include <winsock2.h>

#define CHINA_TIME 8				// �й�ʱ��

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ȫ�ֱ���
CWaitForConnect dlg;
bool g_Connected = false;
int g_DClkFlag = 0;			// ˫����־

CPIMNavigatorDlg g_Sms;					// ���ڶ��Ŷ�ȡ����Ķ���
_RecordsetPtr g_pRecordset1;			// ָ��comment��,����д���¼
_RecordsetPtr g_pRecordset1r;			// ָ��comment��,���ڶ�ȡ��¼
_RecordsetPtr g_pRecordset2;			// ָ��vote��,����д���¼
_RecordsetPtr g_pRecordset2r;			// ָ��vote��,���ڶ�ȡ��¼
extern _ConnectionPtr g_pConnection;	// �������ݿ����
extern CColorListCtrl g_List;			// �б�ؼ�
CArray<CString,CString> g_inArr;		// �������飬�洢�齱����
SOCKET g_Socket;

bool g_PauseFlag = false;	// ѡ����ͣ��־
int g_Year = 0;				// �������
int g_Month = 0;			// �����·�
int g_Day = 0;				// ��������
int g_Hour = 0;				// ����Сʱ
int g_Min = 0;				// ���շ���
CString g_Phone = "";		// �ֻ��ͺ�
int g_Index = -1;			// ���ݱ�����¼����ֵ
bool g_Stand = false;		// ռ�ñ��

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	// ����
	ON_UPDATE_COMMAND_UI(ID_BEGIN,UpdataBegin)
	ON_UPDATE_COMMAND_UI(ID_PAUSE,UpdataPause)
	ON_UPDATE_COMMAND_UI(ID_STOP,UpdataStop)
	ON_COMMAND(ID_BEGIN,OnBegin)
	ON_COMMAND(ID_PAUSE,OnPause)
	ON_COMMAND(ID_STOP,OnStop)
	ON_COMMAND(ID_CLEAR,OnClear)
	ON_COMMAND(ID_DEL_ALL,OnDelAll)
	ON_COMMAND(ID_DEL_COMMENT,OnDelComment)
	ON_COMMAND(ID_DEL_VOTE,OnDelVote)
	// �趨
	ON_UPDATE_COMMAND_UI(ID_COMMENT,UpdataComment)
	ON_UPDATE_COMMAND_UI(ID_VOTE,UpdataVote)
	ON_UPDATE_COMMAND_UI(ID_LOTTERY,UpdataLottery)
	ON_UPDATE_COMMAND_UI(ID_CONFIGURATION,UpdataConfig)
	ON_COMMAND(ID_COMMENT,OnComment)
	ON_COMMAND(ID_VOTE,OnVote)
	ON_COMMAND(ID_LOTTERY,OnLottery)
	ON_COMMAND(ID_CONFIGURATION,OnConfig)
	// ����
	ON_COMMAND(ID_DOCUMENT,OnDocument)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_PHONETYPE,			// �绰�ͺ�
	ID_CONTROL,				// ����״̬
	ID_CONFIG,				// ģʽ
};

// ��̬����
CString CMainFrame::s_Mode = _T("����");
HANDLE CMainFrame::s_Handle_hThread = NULL;
HANDLE CMainFrame::s_Server_hThread = NULL;
CString CMainFrame::m_Control = _T("ֹͣ");
SOCKET CMainFrame::sockConn = NULL;
SOCKET CMainFrame::sockSrv = NULL;
bool CMainFrame::s_Stop = true;
bool CMainFrame::s_Finish = true;

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	m_nTimer=0;
	m_pRecord=false;

	// ��ʼ���˵�����
	//-> ����
	m_Begin=true;
	m_Pause=false;
	m_Stop=false;
	g_PauseFlag=false;
	//-> �趨
	m_Comment=true;
	m_Vote=false;
	m_Lottery=false;
	m_Setting=true;
}

CMainFrame::~CMainFrame()
{
	
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
/*
	if (!m_Menu.Create(this, IDD_DIALOGBAR,
		CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
		IDD_DIALOGBAR))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;		// δ�ܴ���
	}
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}
*/
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
/*
	// TODO: �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
*/
	// �˵���
	CMenu menu;
	menu.LoadMenu(IDR_MAINFRAME);
	SetMenu(&menu);
	menu.Detach();

	// ���ݿ��ļ�����
	CString filepath=".\\Data\\Messages.mdb";
	try                 
	{	
		if(g_pConnection->State)
			g_pConnection->Close();
		g_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + (_bstr_t)filepath + 
			";User ID=admin;Password=;Jet OLEDB:Database Password=2091123116","","",adModeUnknown);
	}
	catch(...)
	{
		AfxMessageBox(_T("���ݿ����Ӵ���!"));
		::PostQuitMessage(0);
		return -1;
	}

	// ����ADO��¼��
	pRecordsetCreate();

	// ����������߳�
	s_Server_hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ServerFunProc,NULL,0,NULL);

	// ״̬��
	SetTimer(1,100,NULL);
	State();
	Configuration();

	// ˫��
	SetTimer(3,500,NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.hMenu=NULL;
	cs.style&=~FWS_ADDTOTITLE;
	cs.lpszName=_T("���Ż���ƽ̨");

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

// CMainFrame ��Ϣ�������

//===================================================================
// ����ADO��¼��
//
//===================================================================
void CMainFrame::pRecordsetCreate()
{
	if (!m_pRecord)
	{
		g_pRecordset1.CreateInstance(__uuidof(Recordset));
		g_pRecordset1r.CreateInstance(__uuidof(Recordset));
		g_pRecordset2.CreateInstance(__uuidof(Recordset));
		g_pRecordset2r.CreateInstance(__uuidof(Recordset));
	}
	m_pRecord = true;
}

//===================================================================
// ��ȡ�ֻ�����
// -> �߳�
//
//===================================================================
DWORD WINAPI CMainFrame::HandleFunProc(LPVOID lpParameter)
{
	g_Stand = true;
	Sleep(10);
	g_Sms.GetInfo();
	g_Stand = false;

	// �ر��߳̾��
	if (s_Handle_hThread)
	{
		CloseHandle(s_Handle_hThread);
	}

	return 0;
}

//===================================================================
// ��ʱ��
//
// 1 == ״̬����"�ֻ�����"��ʾ
// 2 == ���Ŷ�ʱ��ȡ
//
//===================================================================
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{
		if (!g_Stand)
		{
			static bool flag=false;
			g_Sms.OnInitDialog();
			g_Sms.RefreshTreeView();
			CString str=g_Phone;
			CClientDC dc(this);
			CSize sz=dc.GetTextExtent(str);
			m_wndStatusBar.SetPaneInfo(1,ID_PHONETYPE,SBPS_NORMAL,sz.cx);
			m_wndStatusBar.SetPaneText(1,str);
			if (str!=_T("û������"))
			{
				flag = true;
			}
			if (flag && str==_T("û������"))
			{
				OnStop();
				flag = false;
				AfxMessageBox(_T("�ֻ������ѶϿ�!"));
			}
		}
	}

	if (nIDEvent == 2)
	{
		s_Handle_hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)HandleFunProc,NULL,0,NULL);
	}

	if (nIDEvent == 3)
	{
		if (g_DClkFlag == 1)
		{
			g_DClkFlag = 0;
			OnPause();
		}
		else if (g_DClkFlag == 2)
		{
			g_DClkFlag = 0;
			OnBegin();
		}
	}

	CFrameWnd::OnTimer(nIDEvent);
}

//===================================================================
// ����"����"״̬
//
//===================================================================
void CMainFrame::UpdataComment(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(1);
	pCmdUI->SetCheck(m_Comment);
}

//===================================================================
// ����"ͶƱ"״̬
//
//===================================================================
void CMainFrame::UpdataVote(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(1);
	pCmdUI->SetCheck(m_Vote);
}

//===================================================================
// ����"�齱"״̬
//
//===================================================================
void CMainFrame::UpdataLottery(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(1);
	pCmdUI->SetCheck(m_Lottery);
}

//===================================================================
// ��Ӧ"����"ѡ��
//
//===================================================================
void CMainFrame::OnComment()
{
	m_Comment=true;
	m_Vote=false;
	m_Lottery=false;
	s_Mode=_T("����");
	// ����״̬��
	Configuration();
}

//===================================================================
// ��Ӧ"ͶƱ"ѡ��
//
//===================================================================
void CMainFrame::OnVote()
{
	m_Comment=false;
	m_Vote=true;
	m_Lottery=false;
	s_Mode=_T("ͶƱ");
	// ����״̬��
	Configuration();
}

//===================================================================
// ��Ӧ"�齱"ѡ��
//
//===================================================================
void CMainFrame::OnLottery()
{
	m_Comment=false;
	m_Vote=false;
	m_Lottery=true;
	s_Mode=_T("�齱");
	// ����״̬��
	Configuration();
}

//===================================================================
// ��Ӧ"����"ѡ��
//
//===================================================================
void CMainFrame::OnConfig()
{
	CConfig con;
	con.Mode(Mode());
	con.DoModal();
}

//===================================================================
// ��"˵���ĵ�"
//
//===================================================================
void CMainFrame::OnDocument()
{
	ShellExecute(NULL, _T("open"), _T("Document.htm"), NULL, NULL, SW_SHOWNORMAL);	
}

//===================================================================
// ״̬����"ģʽ"��ʾ
//
//===================================================================
void CMainFrame::Configuration()
{
	CString str;
	CSize sz;
	CClientDC dc(this);
	str=Mode();
	sz=dc.GetTextExtent(str);
	m_wndStatusBar.SetPaneInfo(3,ID_CONFIG,SBPS_NORMAL,sz.cx);
	m_wndStatusBar.SetPaneText(3,str);
}

//===================================================================
// ״̬����"����״̬"��ʾ
//
//===================================================================
void CMainFrame::State()
{
	CString str = m_Control;
	CSize sz;
	CClientDC dc(this);
	sz=dc.GetTextExtent(str);
	m_wndStatusBar.SetPaneInfo(2,ID_CONTROL,SBPS_NORMAL,sz.cx);
	m_wndStatusBar.SetPaneText(2,str);
}

//===================================================================
// ����"��ʼ/����"״̬
//
//===================================================================
void CMainFrame::UpdataBegin(CCmdUI *pCmdUI)
{
	if (m_Begin && g_Connected)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

//===================================================================
// ����"��ͣ"״̬
//
//===================================================================
void CMainFrame::UpdataPause(CCmdUI *pCmdUI)
{
	if (m_Pause)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

//===================================================================
// ����"ֹͣ"״̬
//
//===================================================================
void CMainFrame::UpdataStop(CCmdUI *pCmdUI)
{
	if (m_Stop)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

//===================================================================
// ���ص�ǰ��ѡģʽ
//
//===================================================================
CString CMainFrame::Mode()
{
	return s_Mode;
}

//===================================================================
// ������߳�
//
//===================================================================
DWORD WINAPI CMainFrame::ServerFunProc(LPVOID lpParameter)
{
	WORD wVersionRequested;   
	WSADATA wsaData;   
	int err;

	//��ʼ���׽��ֿ�
	wVersionRequested = MAKEWORD(2, 2);   
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 )   
	{   
		return 0;   
	}   
	if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2)   
	{   
		WSACleanup( );   
		return 0;   
	}   

	// �����׽���
	sockSrv = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,0);
	if (sockSrv == INVALID_SOCKET)
	{
		AfxMessageBox(_T("�����׽���ʧ��!"));
		return 0;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);   
	addrSrv.sin_family=AF_INET;   
	addrSrv.sin_port=htons(6000);   

	// ���׽���
	if (bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("��ʧ��!"));
	}

	// ���׽�����Ϊ����ģʽ��׼�����տͻ�
	listen(sockSrv,1);  

	SOCKADDR_IN addrClient;   
	int len = sizeof(SOCKADDR); 

	bool cntn = false;
	g_Connected = false;

	while(1)
	{
		// �ȴ��ͻ������� 
		Sleep(200);
		dlg.ParamInit(&sockSrv,&addrClient,&len);
		if (!cntn && !g_Connected)
		{
			dlg.DoModal();
			cntn = true;
		}
		if (!g_Connected)
		{
			continue;
		}
		cntn = false;
		sockConn = dlg.ReturnConn();
		g_Socket = sockConn;
		// ���Ͳ�������
		char *pString = "test";
		if(send(sockConn,pString,strlen(pString),0) == SOCKET_ERROR)   
		{   
			// ��������ʧ��
			g_Connected = false;
			continue;  
		}

		// ѭ����־
		bool flag = false;

		while(1)
		{
			s_Finish = true;
			// ��ʼ��ʾ
			if (m_Control == _T("��ʼ"))
			{
				s_Finish = false;
				// ģʽ����
				if (Mode() == "����")
				{
					if (s_Stop)
					{
						// ��������
						char *pString = "m1";
						if(send(sockConn,pString,strlen(pString),0) == SOCKET_ERROR)   
						{   
							// ��������ʧ��
							break;   
						}
						s_Stop = false;
					}
					CFunProc(flag);
				}
				else if (Mode() == "ͶƱ")
				{
					if (s_Stop)
					{
						// ��ȡ����
						CString max,num,item;
						GetPrivateProfileString(_T("VoteSet"),_T("Max"),_T("0"),max.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
						max.ReleaseBuffer();
						GetPrivateProfileString(_T("VoteSet"),_T("Num"),_T("3"),num.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
						num.ReleaseBuffer();
						GetPrivateProfileString(_T("VoteSet"),_T("Item"),_T(""),item.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
						item.ReleaseBuffer();

						// ��Ϣ����
						// item
						int i = 0;
						int len = item.GetLength();
						CString tmp;
						CArray<CString,CString> arr;
						for(i=0; i<len; i++)
						{
							TCHAR t = item.GetAt(i);
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
						// msg
						if(num == "10")
							num = "0";
						CString msg = "m2"+num+max;
						if(num == "0")
							num = "10";
						int n = _tstoi(LPCTSTR(num));
						while (arr.GetSize()<n)
						{
							arr.Add("");
						}
						for (i=0; i<n; i++)
						{
							msg += "+";
							msg += arr[i];
						}

						// ����ת�� CString to char*
						int nSize = msg.GetLength();
						char *pString = new char[3*nSize];
						WideCharToMultiByte(CP_UTF8, 0, msg, -1, pString, 3*nSize, NULL, NULL);

						// ��������
						if(send(sockConn,pString,3*nSize,0) == SOCKET_ERROR)   
						{   
							// ��������ʧ��
							break;   
						}

						s_Stop = false;
					}
					VFunProc(flag);
				}
				else if (Mode() == "�齱")
				{
					if (s_Stop)
					{
						// ��������
						char *pString = "m3";
						if(send(sockConn,pString,strlen(pString),0) == SOCKET_ERROR)   
						{   
							// ��������ʧ��
							break;   
						}
						// ����������������洢
						IndexSave();

						s_Stop = false;
					}
					// �����ȡ
					LFunProc(flag);
				}
				if (flag)
				{
					break;
				}
			}
			// ����״̬
			else
			{
				// ����������
				char *pString = "dot";
				if(send(sockConn,pString,strlen(pString),0) == SOCKET_ERROR)   
				{   
					// ��������ʧ��
					break; 
				}
				Sleep(500);
			}
		}

		// �ر��׽���  
		closesocket(sockConn);
	}
	
	// �ر��׽���
	closesocket(sockSrv);

	// �ر��׽��ֿ�
	WSACleanup();

	return 0;
}

//===================================================================
// ˯��
//
//===================================================================
void CMainFrame::aSleep(int time)
{
	time /= 100;
	for (int i=0; i<time; i++)
	{
		if (s_Stop)
		{
			break;
		}
		Sleep(100);
	}
}

//===================================================================
// ������߳�
// - ����ģʽ����
//
//===================================================================
void CMainFrame::CFunProc(bool &flag)
{
	_variant_t var;
	// ���ʱ��
	static int ishow = 0;
	static int ilist = 0;
	// ��һ�η������ݱ��
	static bool firflag = false;

	try
	{
		// ��һ�η�������
		if (!firflag)
		{
			ishow = GetPrivateProfileInt(_T("CommentSet"),_T("SInterval"),10,_T(".\\Data\\Config.ini"));
			ishow *= 1000;
			ilist = GetPrivateProfileInt(_T("CommentSet"),_T("LInterval"),10,_T(".\\Data\\Config.ini"));
			ilist *= 1000;
			firflag = true;
		}

		if(!g_pRecordset1r->State)
		{
			// ������
			g_pRecordset1r->Open("SELECT * FROM comment",
				g_pConnection.GetInterfacePtr(),
				adOpenDynamic,
				adLockOptimistic,
				adCmdText);

			// ��¼���ƶ�������¼
			if(!g_pRecordset1r->BOF)
			{
				g_pRecordset1r->MoveFirst();
			}
			else
			{
				return;
			}
		}

		if (!g_pRecordset1r->adoEOF)
		{
			// �ж��Ѷ����
			CString rflg = "";
			var = g_pRecordset1r->GetCollect("read_flag");
			if(var.vt != VT_NULL)
				rflg = (LPCTSTR)_bstr_t(var);

			// �ж�ɾ�����
			CString dflg = "";
			var = g_pRecordset1r->GetCollect("del_flag");
			if(var.vt != VT_NULL)
				dflg = (LPCTSTR)_bstr_t(var);

			// read_flag || del_flag ==
			// true:-1
			// false:0
			if(rflg == "-1")
			{
				//if(g_pRecordset1r->State)
				//	g_pRecordset1r->Close();
				// �ȴ�
				//aSleep(ilist);
				g_pRecordset1r->MoveNext();
				return;
			}
			else if (dflg == "-1")
			{
				g_pRecordset1r->MoveNext();
				return;
			}

			// ��ȡ�ֻ�����
			CString phonum = "";
			var = g_pRecordset1r->GetCollect("number");
			if(var.vt != VT_NULL)
				phonum = (LPCTSTR)_bstr_t(var);
			CString number = phonum;

			// ��ȡ��������
			CString message = "";
			var = g_pRecordset1r->GetCollect("text");
			if(var.vt != VT_NULL)
				message = (LPCTSTR)_bstr_t(var);

			// ��Ϣ����
			int len = phonum.GetLength();
			while (len>11)
			{
				phonum.Delete(0);
				len--;
			}
			if (len>7 && len<=11)
			{
				int mlen = len - 7;
				for (int i=0; i<mlen; i++)
				{
					phonum.SetAt(3+i,'*');
				}
			}
			CString msg = _T("c")+phonum+_T("+")+message;

			// ����ת�� CString to char*
			int nSize = msg.GetLength();
			char *pString = new char[3*nSize];	
			memset(pString, 0, 3*nSize);
			WideCharToMultiByte(CP_UTF8, 0, msg, -1, pString, 3*nSize, NULL, NULL);

			// ��������
			if(send(sockConn,pString,3*nSize,0) == SOCKET_ERROR)   
			{   
				// ��������ʧ��
				flag = true;
				delete pString;
				return;   
			}
			delete pString;

			// ��ȡ����ʱ��
			CString time = "";
			var = g_pRecordset1r->GetCollect("time");
			if(var.vt != VT_NULL)
				time = (LPCTSTR)_bstr_t(var);

			// �޸��Ѷ�����
			g_pRecordset1r->PutCollect("read_flag", _variant_t(_T("True")));
			g_pRecordset1r->Update();

			// ��ȡ��һ������
			g_pRecordset1r->MoveNext();

			// �Ѷ�����ɫ
			for (int i=g_List.GetItemCount()-1; i>=0; i--)
			{
				CString cmp = g_List.GetItemText(i,0);
				if (cmp == "����")
				{
					cmp = g_List.GetItemText(i,1);
					if (cmp == time)
					{
						cmp = g_List.GetItemText(i,2);
						if (cmp == number)
						{
							cmp = g_List.GetItemText(i,3);
							if (cmp == message)
							{
								if (!g_List.ChangedItemColor(i))
								{
									g_List.SetItemColor(i, RGB(0,0,0), RGB(239,255,206));
									break;
								}
								else
								{
									continue;
								}
							}
							else
							{
								continue;
							}
						}
						else
						{
							continue;
						}
					}
					else
					{
						continue;
					}
				}
				else
				{
					break;
				}
			}

			// �ȴ�
			aSleep(ishow);
		}
		else
		{
			// �رն�ȡ��¼��
			if(g_pRecordset1r->State)
				g_pRecordset1r->Close();

			// �ȴ�
			aSleep(ilist);
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("���ݿ��ȡ����!"));
		return;
	}
}

//===================================================================
// ������߳�
// - ͶƱģʽ����
//
//===================================================================
void CMainFrame::VFunProc(bool &flag)
{
	_variant_t var;
	// ���ʱ��
	static int ilist = 0;
	// ��һ�η������ݱ��
	static bool firflag = false;

	try
	{
		// ��һ�η�������
		if (!firflag)
		{
			ilist = GetPrivateProfileInt(_T("VoteSet"),_T("LInterval"),10,_T(".\\Data\\Config.ini"));
			ilist *= 1000;
			firflag = true;
		}

		if(!g_pRecordset2r->State)
		{
			// ������
			g_pRecordset2r->Open("SELECT * FROM vote",
				g_pConnection.GetInterfacePtr(),
				adOpenDynamic,
				adLockOptimistic,
				adCmdText);

			// ��¼���ƶ�������¼
			if(!g_pRecordset2r->adoEOF)
			{
				g_pRecordset2r->MoveLast();
			}
			else
			{
				return;
			}
		}
	
		if (!g_pRecordset2r->BOF)
		{
			// �ж��Ѷ����
			CString rflg = "";
			var = g_pRecordset2r->GetCollect("read_flag");
			if(var.vt != VT_NULL)
				rflg = (LPCTSTR)_bstr_t(var);

			// read_flag ==
			// true:-1
			// false:0
			if(rflg == "-1")
			{
				if(g_pRecordset2r->State)
					g_pRecordset2r->Close();
				// �ȴ�
				aSleep(ilist);
				return;
			}

			// ��ȡ��������
			CString message = "";
			var = g_pRecordset2r->GetCollect("text");
			if(var.vt != VT_NULL)
				message = (LPCTSTR)_bstr_t(var);

			// ��Ϣ����
			CString msg = "";
			if (message.GetLength() != 1)
			{
				AfxMessageBox(_T("ͶƱ���ݴ���!"));
				return;
			}
			else
			{
				msg = _T("v")+message;
			}

			// ����ת�� CString to char*
			int nSize = msg.GetLength();
			char *pString = new char[3*nSize];
			WideCharToMultiByte(CP_UTF8, 0, msg, -1, pString, 3*nSize, NULL, NULL);

			// ��������
			if(send(sockConn,pString,3*nSize,0) == SOCKET_ERROR)   
			{   
				// ��������ʧ��
				flag = true;
				delete pString;
				return;   
			}
			delete pString;

			// ��ȡ����ʱ��
			CString time = "";
			var = g_pRecordset2r->GetCollect("time");
			if(var.vt != VT_NULL)
				time = (LPCTSTR)_bstr_t(var);

			// ��ȡ�ֻ�����
			CString number = "";
			var = g_pRecordset2r->GetCollect("number");
			if(var.vt != VT_NULL)
				number = (LPCTSTR)_bstr_t(var);

			// �޸��Ѷ�����
			g_pRecordset2r->PutCollect("read_flag", _variant_t(_T("True")));
			g_pRecordset2r->Update();

			// ��ȡ��һ������
			g_pRecordset2r->MovePrevious();

			// �Ѷ�����ɫ
			for (int i=g_List.GetItemCount()-1; i>=0; i--)
			{
				CString cmp = g_List.GetItemText(i,0);
				if (cmp == "ͶƱ")
				{
					cmp = g_List.GetItemText(i,1);
					if (cmp == time)
					{
						cmp = g_List.GetItemText(i,2);
						if (cmp == number)
						{
							cmp = g_List.GetItemText(i,3);
							if (cmp == message)
							{
								if (!g_List.ChangedItemColor(i))
								{
									g_List.SetItemColor(i, RGB(0,0,0), RGB(239,255,206));
									break;
								}
								else
								{
									continue;
								}
							}
							else
							{
								continue;
							}
						}
						else
						{
							continue;
						}
					}
					else
					{
						continue;
					}
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			// �رն�ȡ��¼��
			if(g_pRecordset2r->State)
				g_pRecordset2r->Close();

			// �ȴ�
			aSleep(ilist);
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("���ݿ��ȡ����!"));
		return;
	}
}

//===================================================================
// ������߳�
// - �齱ģʽ����
//
//===================================================================
void CMainFrame::LFunProc(bool &flag)
{
	//����Ϊ������ģʽ
	unsigned long flg=1;  
	if (ioctlsocket(sockConn,FIONBIO,&flg)!=0)  
	{  
		closesocket(sockConn);  
		return;
	}  

	// ��������
	char buf[10];
	int bytes = 0;
	if((bytes=recv(sockConn,buf,5/*sizeof(buf)*/,0)) == SOCKET_ERROR)   
	{
		return;
	}
	else
	{
		// ������Ϣ��Show
		char *pString = "o";
		if(send(sockConn,pString,strlen(pString),0) == SOCKET_ERROR)   
		{   
			// ��������ʧ��
		}
	}
	buf[bytes]='\0';

	// char to CString
	int charLen = strlen(buf);
	//������ֽ��ַ��Ĵ�С�����ַ�����
	int len = MultiByteToWideChar(CP_ACP,0,buf,charLen,NULL,0);
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ķ��ֽ��ַ���С
	TCHAR *buffer = new TCHAR[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���
	MultiByteToWideChar(CP_ACP,0,buf,charLen,buffer,len);
	buffer[len] = '\0'; //����ַ�����β
	//��TCHAR����ת��ΪCString
	CString recstr = "";
	recstr.Append(buffer);
	//ɾ��������
	delete []buffer;

	if (recstr == _T("click"))
	{
		// �����ȡ�ֻ�����
		CString res = RandomGet();

		// ��Ϣ����
		int len = res.GetLength();
		while (len>11)
		{
			res.Delete(0);
			len--;
		}
		while (len<11)
		{
			res.Insert(0,'0');
			len++;
		}
		CString msg = "l"+res;

		// ����ת�� CString to char*
		int nSize = msg.GetLength();
		char *pString = new char[3*nSize];
		WideCharToMultiByte(CP_UTF8, 0, msg, -1, pString, 3*nSize, NULL, NULL);

		// ����ʱ��
		aSleep(2000);

		// ��������
		if(send(sockConn,pString,3*nSize,0) == SOCKET_ERROR)   
		{   
			// ��������ʧ��
			flag = true;
			delete pString;
			return;   
		}
		delete pString;
	}

	//����Ϊ����ģʽ
	flg=0;  
	if (ioctlsocket(sockConn,FIONBIO,&flg)!=0)  
	{  
		closesocket(sockConn);  
		return;
	} 
}

//===================================================================
// �齱��������洢
// - ����ֵΪ��Ч������С
//
//===================================================================
void CMainFrame::IndexSave()
{
	try
	{
		int i;
		_variant_t var;
		CString flg = "";
		// ���ۺ�ͶƱ������ʱ��
		CString cStr = "";
		CString vStr = "";
		// �������ļ���ȡ���ò���
		CString participant,chance;
		GetPrivateProfileString(_T("LotterySet"),_T("Participant"),_T("ȫ��"),participant.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
		participant.ReleaseBuffer();
		GetPrivateProfileString(_T("LotterySet"),_T("Chance"),_T("һ�Ŷ�Ʊ"),chance.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
		chance.ReleaseBuffer();
		// �����߷���
		if(participant == "ȫ��")
		{
			// comment��
			if(!g_pRecordset1r->State)
			{
				// ������
				g_pRecordset1r->Open("SELECT * FROM comment",
					g_pConnection.GetInterfacePtr(),
					adOpenDynamic,
					adLockOptimistic,
					adCmdText);
			}
			if(!g_pRecordset1r->BOF)
			{
				g_pRecordset1r->MoveFirst();
			}
			while(!g_pRecordset1r->adoEOF)
			{
				var = g_pRecordset1r->GetCollect("del_flag");
				if(var.vt != VT_NULL)
					flg = (LPCTSTR)_bstr_t(var);
				// del_flag ==
				// true:-1
				// false:0
				if(flg == "0")
				{
					var = g_pRecordset1r->GetCollect("number");
					if(var.vt != VT_NULL)
						cStr = (LPCTSTR)_bstr_t(var);
					if (chance == _T("һ��һƱ"))
					{
						bool flg = false;
						for (i=0; i<g_inArr.GetSize(); i++)
						{
							if (g_inArr[i] == cStr)
							{
								flg = true;
								break;
							}
						}
						if (!flg)
						{
							g_inArr.Add(cStr);
						}
					}
					if (chance == _T("һ�Ŷ�Ʊ"))
					{
						g_inArr.Add(cStr);
					}
				}
				g_pRecordset1r->MoveNext();
			}
			// �رն�ȡ��¼��
			if(g_pRecordset1r->State)
				g_pRecordset1r->Close();

			// vote��
			if(!g_pRecordset2r->State)
			{
				// ������
				g_pRecordset2r->Open("SELECT * FROM vote",
					g_pConnection.GetInterfacePtr(),
					adOpenDynamic,
					adLockOptimistic,
					adCmdText);
			}
			if(!g_pRecordset2r->BOF)
			{
				g_pRecordset2r->MoveFirst();
			}
			while(!g_pRecordset2r->adoEOF)
			{
				var = g_pRecordset2r->GetCollect("number");
				if(var.vt != VT_NULL)
					vStr = (LPCTSTR)_bstr_t(var);
				if (chance == _T("һ��һƱ"))
				{
					bool flg = false;
					for (i=0; i<g_inArr.GetSize(); i++)
					{
						if (g_inArr[i] == vStr)
						{
							flg = true;
							break;
						}
					}
					if (!flg)
					{
						g_inArr.Add(vStr);
					}
				}
				if (chance == _T("һ�Ŷ�Ʊ"))
				{
					g_inArr.Add(vStr);
				}
				g_pRecordset2r->MoveNext();
			}
			// �رն�ȡ��¼��
			if(g_pRecordset2r->State)
				g_pRecordset2r->Close();

			return;
		}
		if(participant == "����")
		{
			// comment��
			if(!g_pRecordset1r->State)
			{
				// ������
				g_pRecordset1r->Open("SELECT * FROM comment",
					g_pConnection.GetInterfacePtr(),
					adOpenDynamic,
					adLockOptimistic,
					adCmdText);
			}
			if(!g_pRecordset1r->BOF)
			{
				g_pRecordset1r->MoveFirst();
			}
			while(!g_pRecordset1r->adoEOF)
			{
				var = g_pRecordset1r->GetCollect("del_flag");
				if(var.vt != VT_NULL)
					flg = (LPCTSTR)_bstr_t(var);
				// del_flag ==
				// true:-1
				// false:0
				if(flg == "0")
				{
					var = g_pRecordset1r->GetCollect("number");
					if(var.vt != VT_NULL)
						cStr = (LPCTSTR)_bstr_t(var);
					if (chance == _T("һ��һƱ"))
					{
						bool flg = false;
						for (i=0; i<g_inArr.GetSize(); i++)
						{
							if (g_inArr[i] == cStr)
							{
								flg = true;
								break;
							}
						}
						if (!flg)
						{
							g_inArr.Add(cStr);
						}
					}
					if (chance == _T("һ�Ŷ�Ʊ"))
					{
						g_inArr.Add(cStr);
					}
				}
				g_pRecordset1r->MoveNext();
			}
			// �رն�ȡ��¼��
			if(g_pRecordset1r->State)
				g_pRecordset1r->Close();

			return;
		}
		if(participant == "ͶƱ")
		{
			// vote��
			if(!g_pRecordset2r->State)
			{
				// ������
				g_pRecordset2r->Open("SELECT * FROM vote",
					g_pConnection.GetInterfacePtr(),
					adOpenDynamic,
					adLockOptimistic,
					adCmdText);
			}
			if(!g_pRecordset2r->BOF)
			{
				g_pRecordset2r->MoveFirst();
			}
			while(!g_pRecordset2r->adoEOF)
			{
				var = g_pRecordset2r->GetCollect("number");
				if(var.vt != VT_NULL)
					vStr = (LPCTSTR)_bstr_t(var);
				if (chance == _T("һ��һƱ"))
				{
					bool flg = false;
					for (i=0; i<g_inArr.GetSize(); i++)
					{
						if (g_inArr[i] == vStr)
						{
							flg = true;
							break;
						}
					}
					if (!flg)
					{
						g_inArr.Add(vStr);
					}
				}
				if (chance == _T("һ�Ŷ�Ʊ"))
				{
					g_inArr.Add(vStr);
				}
				g_pRecordset2r->MoveNext();
			}
			// �رն�ȡ��¼��
			if(g_pRecordset2r->State)
				g_pRecordset2r->Close();

			return;
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("���ݿ��ȡ����!"));
	}
}

//===================================================================
// �����ȡ�ֻ�����
//
//===================================================================
CString CMainFrame::RandomGet()
{
	// �ж��Ƿ���
	int size = g_inArr.GetSize();
	if (size == 0)
	{
		return "";
	}

	// �������ļ���ȡ���ò���
	CString participant,chance;
	GetPrivateProfileString(_T("LotterySet"),_T("Participant"),_T("ȫ��"),participant.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	participant.ReleaseBuffer();
	GetPrivateProfileString(_T("LotterySet"),_T("Chance"),_T("һ�Ŷ�Ʊ"),chance.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	chance.ReleaseBuffer();

	// ��ȡ�������ȡ������ֵ
	CTime now = CTime::GetCurrentTime();
	time_t time = now.GetTime();
	srand((unsigned)time);
	int val = rand();
	val %= size;
	CString phone = g_inArr[val];

	// ɾ���ѳ����
	if (chance == _T("һ��һƱ"))
	{
		g_inArr.RemoveAt(val);
	}
	if (chance == _T("һ�Ŷ�Ʊ"))
	{
		for (int i=0; i<g_inArr.GetSize(); i++)
		{
			if (g_inArr[i] == phone)
			{
				g_inArr.RemoveAt(i);
				i--;
			}
		}
	}

	// �����ֻ����� 
	return phone;
}

//===================================================================
// ��ʼ/����
//
//===================================================================
void CMainFrame::OnBegin()
{
	CString str = Mode();
	// ��ȡʱ��������
	int ilist;
	if (str == _T("����") || str == _T("ͶƱ"))
	{
		if (str == _T("����"))
		{
			ilist = GetPrivateProfileInt(_T("CommentSet"),_T("LInterval"),10,_T(".\\Data\\Config.ini"));
		}
		if (str == _T("ͶƱ"))
		{
			ilist = GetPrivateProfileInt(_T("VoteSet"),_T("LInterval"),10,_T(".\\Data\\Config.ini"));
		}
		ilist *= 1000;
	}

	if (m_Control == _T("ֹͣ"))
	{
		// ���ۺ�ͶƱģʽ
		if (str == _T("����") || str == _T("ͶƱ"))
		{
			// �����ֻ���ʾ
			if (g_Phone=="û������")
			{
				MessageBox(_T("���������ֻ�"),_T("��ʾ"),MB_ICONSTOP);
				return;
			}

			// �˵�״̬����
			m_Begin = false;
			m_Pause = true;
			m_Stop = true;

			// ��ȡ��ǰʱ��
			CTime t = CTime::GetCurrentTime();
			// year
			CString cyear_str = t.Format("%Y");
			g_Year = _tstoi(LPCTSTR(cyear_str));
			// month
			CString cmonth_str = t.Format("%m");
			g_Month = _tstoi(LPCTSTR(cmonth_str));
			// day
			CString cday_str = t.Format("%d");
			g_Day = _tstoi(LPCTSTR(cday_str));
			// hour
			CString chour_str = t.Format("%H");
			g_Hour = _tstoi(LPCTSTR(chour_str));
			// minute
			CString cmin_str = t.Format("%M");
			g_Min = _tstoi(LPCTSTR(cmin_str));

			// ��ADO��¼��
			if (str == _T("����"))
			{
				OpenRecord1();
			}
			if (str == _T("ͶƱ"))
			{
				OpenRecord2();
			}

			try
			{
				// δ����������
				if (str == _T("����"))
				{
					if (!g_pRecordset1->adoEOF)
					{
						g_pRecordset1->MoveLast();
						while(!g_pRecordset1->BOF)
						{
							_variant_t var;
							CString rflg = "";
							var = g_pRecordset1->GetCollect("read_flag");
							if(var.vt != VT_NULL)
								rflg = (LPCSTR)_bstr_t(var);

							// read_flag ==
							// true:-1
							// false:0
							if (rflg == "0")
							{
								if (MessageBox(_T("�������ϴ�δ������,\n�Ƿ��ȡ?"), _T("δ����������"), MB_ICONQUESTION | MB_YESNO) == IDNO)
								{
									// ����ȫ������Ϊ�Ѷ�
									g_pRecordset1->MoveFirst();
									while(!g_pRecordset1->adoEOF)
									{
										g_pRecordset1->PutCollect("read_flag", _variant_t(_T("True")));
										g_pRecordset1->Update();
										g_pRecordset1->MoveNext();
									}
								}
								break;
							}

							g_pRecordset1->MovePrevious();
						}
					}
				}
				if (str == _T("ͶƱ"))
				{
					if (!g_pRecordset2->BOF)
					{
						// ����ȫ������Ϊ�Ѷ�
						g_pRecordset2->MoveFirst();
						while(!g_pRecordset2->adoEOF)
						{
							g_pRecordset2->PutCollect("read_flag", _variant_t(_T("True")));
							g_pRecordset2->Update();
							g_pRecordset2->MoveNext();
						}
					}
				}
			}
			catch(...)
			{
				AfxMessageBox(_T("���ݿ��ȡ��д�����!"));
			}

			// ������ʼ��
			g_Sms.ModeSet(str);
			g_Sms.GetReady();

			// ���ֻ���ȡ����
			m_nTimer = SetTimer(2,ilist,NULL);
		}

		// �齱ģʽ
		// �˵�״̬����
		if (str == _T("�齱"))
		{
			m_Begin = false;
			m_Pause = false;
			m_Stop = true;
		}

		// ����״̬��
		m_Control = _T("��ʼ");
		State();

		// �˵����Ը���
		GetMenu()->GetSubMenu(0)->EnableMenuItem(5,MF_BYPOSITION | MF_DISABLED);	// �������
		GetMenu()->GetSubMenu(1)->EnableMenuItem(0,MF_BYPOSITION | MF_DISABLED);	// ģʽ
		m_Setting = false;

		return;
	}
	if (m_Control == _T("��ͣ"))
	{
		// �˵�״̬����
		m_Begin = false;
		m_Pause = true;
		m_Stop = true;

		// ����״̬��
		m_Control = _T("��ʼ");
		State();

		// �ָ��߳�
		ResumeThread(s_Server_hThread);
		if (str == _T("����") || str == _T("ͶƱ"))
		{
			ResumeThread(s_Handle_hThread);
			// ���ֻ���ȡ����
			m_nTimer = SetTimer(2,ilist,NULL);
		}
	}
}

//===================================================================
// ��ͣ��ʾ
//
//===================================================================
void CMainFrame::OnPause()
{
	// ����״̬��
	m_Control = _T("��ͣ");
	State();

	// �˵����Ը���
	m_Begin = true;
	m_Pause = false;
	m_Stop = true;

	// ɱ����ʱ��
	g_PauseFlag = true;
	KillTimer(m_nTimer);

	// �����߳�
	SuspendThread(s_Handle_hThread);
	SuspendThread(s_Server_hThread);
}

//===================================================================
// ֹͣ��ʾ
//
//===================================================================
void CMainFrame::OnStop()
{
	// ����״̬��
	m_Control = _T("ֹͣ");
	State();

	// ������Ϣ��Show
	if (Mode() == _T("����") || Mode() == _T("ͶƱ"))
	{
		s_Stop = true;
		char *pString = "s";
		if(send(sockConn,pString,strlen(pString),0) == SOCKET_ERROR)   
		{   
			// ��������ʧ��
		}
	}
	else if (Mode() == _T("�齱"))
	{
		while(1)
		{
			if (s_Finish)
			{
				char *pString = "s";
				if(send(sockConn,pString,strlen(pString),0) == SOCKET_ERROR)   
				{   
					// ��������ʧ��
				}
				s_Stop = true;
				break;
			}
		}
	}

	// ����ȫ��Ϊ�Ѷ�
	try
	{
		if (Mode() == _T("����"))
		{
			// ��ADO��¼��
			OpenRecord1();

			if (!g_pRecordset1->BOF)
			{
				g_pRecordset1->MoveFirst();
				while(!g_pRecordset1->adoEOF)
				{
					g_pRecordset1->PutCollect("read_flag", _variant_t(_T("True")));
					g_pRecordset1->Update();
					g_pRecordset1->MoveNext();
				}
			}
			// �رռ�¼��
			CloseRecord1();
		}
		else if (Mode() == _T("ͶƱ"))
		{
			// ��ADO��¼��
			OpenRecord2();

			if (!g_pRecordset2->BOF)
			{
				g_pRecordset2->MoveFirst();
				while(!g_pRecordset2->adoEOF)
				{
					g_pRecordset2->PutCollect("read_flag", _variant_t(_T("True")));
					g_pRecordset2->Update();
					g_pRecordset2->MoveNext();
				}
			}
			// �رռ�¼��
			CloseRecord2();
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("���ݿ��޸Ĵ���!"));
	}

	// ����б���ɫ
	int listnum = g_List.GetItemCount();
	for (int c=0; c<listnum; c++)
	{
		if (!g_List.ChangedItemColor(c))
		{
			g_List.SetItemColor(c, RGB(0,0,0), RGB(239,255,206));
		}
	}

	// �˵����Ը���
	m_Begin = true;
	m_Pause = false;
	m_Stop = false;
	m_Setting = true;

	GetMenu()->GetSubMenu(0)->EnableMenuItem(5,MF_BYPOSITION | MF_ENABLED);	// �������
	GetMenu()->GetSubMenu(1)->EnableMenuItem(0,MF_BYPOSITION | MF_ENABLED);	// ģʽ

	// ɱ����ʱ��
	if (!g_PauseFlag)
	{
		KillTimer(m_nTimer);
	}	
	g_PauseFlag = false;

	// �ر��߳̾��
	if (s_Handle_hThread)
	{
		CloseHandle(s_Handle_hThread);
	}
}

//===================================================================
// �򿪼�¼��1,ָ��comment���ݱ�
//
//===================================================================
void CMainFrame::OpenRecord1()
{
	try
	{
		if(!g_pRecordset1->State)
		{
			g_pRecordset1->Open("SELECT * FROM comment",
				g_pConnection.GetInterfacePtr(),
				adOpenDynamic,
				adLockOptimistic,
				adCmdText);
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("���ݱ�1�򿪴���!"));
	}
}

//===================================================================
// �򿪼�¼��2,ָ��vote���ݱ�
//
//===================================================================
void CMainFrame::OpenRecord2()
{
	try
	{
		if(!g_pRecordset2->State)
		{
			g_pRecordset2->Open("SELECT * FROM vote",
				g_pConnection.GetInterfacePtr(),
				adOpenDynamic,
				adLockOptimistic,
				adCmdText);
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("���ݱ�2�򿪴���!"));
	}
}

//===================================================================
// �رռ�¼��1
//
//===================================================================
void CMainFrame::CloseRecord1()
{
	try
	{
		if(g_pRecordset1->State)
			g_pRecordset1->Close();
	}
	catch(...)
	{
		AfxMessageBox(_T("���ݱ�1�رմ���!"));
	}
}

//===================================================================
// �رռ�¼��1read
//
//===================================================================
void CMainFrame::CloseRecord1r()
{
	try
	{
		if(g_pRecordset1r->State)
			g_pRecordset1r->Close();
	}
	catch(...)
	{
		AfxMessageBox(_T("���ݱ�1�رմ���!"));
	}
}

//===================================================================
// �رռ�¼��2
//
//===================================================================
void CMainFrame::CloseRecord2()
{
	try
	{
		if(g_pRecordset2->State)
			g_pRecordset2->Close();
	}
	catch(...)
	{
		AfxMessageBox(_T("���ݱ�2�رմ���!"));
	}
}

//===================================================================
// �رռ�¼��2read
//
//===================================================================
void CMainFrame::CloseRecord2r()
{
	try
	{
		if(g_pRecordset2r->State)
			g_pRecordset2r->Close();
	}
	catch(...)
	{
		AfxMessageBox(_T("���ݱ�2�رմ���!"));
	}
}

//===================================================================
// �رռ�¼���ϲ�
//
//===================================================================
void CMainFrame::CloseRecord()
{
	CloseRecord1();
	CloseRecord2();
	CloseRecord1r();
	CloseRecord2r();

	g_pRecordset1.Release();
	g_pRecordset1 = NULL;
	g_pRecordset2.Release();
	g_pRecordset2 = NULL;
	g_pRecordset1r.Release();
	g_pRecordset1r = NULL;
	g_pRecordset2r.Release();
	g_pRecordset2r = NULL;

	m_pRecord = false;
}

//===================================================================
// ��������
//
//===================================================================
void CMainFrame::OnClear()
{
	g_List.DeleteAllItems();
}

//===================================================================
// ������� - ȫ��
//
//===================================================================
void CMainFrame::OnDelAll()
{
	// ����ADO��¼��
	pRecordsetCreate();

	// ��ADO��¼��
	OpenRecord1();
	OpenRecord2();

	// ��ղ���
	DelComment();
	DelVote();	

	// �ر�ADO��¼��
	CloseRecord1();
	CloseRecord2();

	// ����б�
	g_List.DeleteAllItems();

	// ��ʾ
	AfxMessageBox(_T("������ճɹ�!"));
}

//===================================================================
// ������� - ����
//
//===================================================================
void CMainFrame::OnDelComment()
{
	// ����ADO��¼��
	pRecordsetCreate();

	// ��ADO��¼��
	OpenRecord1();

	// ��ղ���
	DelComment();

	// �ر�ADO��¼��
	CloseRecord1();

	// ����б���Ӧ��¼
	for (int i=0; i<g_List.GetItemCount();)
	{
		if (g_List.GetItemText(i,0) == _T("����"))
		{
			g_List.DeleteItem(i);
		}
		else
		{
			i++;
		}
	}

	// ����б���ɫ
	int listnum = g_List.GetItemCount();
	for (int c=0; c<listnum; c++)
	{
		if (!g_List.ChangedItemColor(c))
		{
			g_List.SetItemColor(c, RGB(0,0,0), RGB(239,255,206));
		}
	}

	// ��ʾ
	AfxMessageBox(_T("������ճɹ�!"));
}

//===================================================================
// ������� - ͶƱ
//
//===================================================================
void CMainFrame::OnDelVote()
{
	// ����ADO��¼��
	pRecordsetCreate();

	// ��ADO��¼��
	OpenRecord2();

	// ��ղ���
	DelVote();	

	// �ر�ADO��¼��
	CloseRecord2();

	// ����б���Ӧ��¼
	for (int i=0; i<g_List.GetItemCount();)
	{
		if (g_List.GetItemText(i,0) == _T("ͶƱ"))
		{
			g_List.DeleteItem(i);
		}
		else
		{
			i++;
		}
	}

	// ����б���ɫ
	int listnum = g_List.GetItemCount();
	for (int c=0; c<listnum; c++)
	{
		if (!g_List.ChangedItemColor(c))
		{
			g_List.SetItemColor(c, RGB(0,0,0), RGB(239,255,206));
		}
	}

	// ��ʾ
	AfxMessageBox(_T("������ճɹ�!"));
}

//===================================================================
// ���"����"���ݱ�
//
//===================================================================
void CMainFrame::DelComment()
{
	try
	{
		if (!g_pRecordset1->BOF)
		{
			g_pRecordset1->MoveFirst();
			while(!g_pRecordset1->adoEOF)
			{
				g_pRecordset1->Delete(adAffectCurrent);
				g_pRecordset1->Update();
				g_pRecordset1->MoveNext();
			}
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("������ݱ�1����!"));
	}
}

//===================================================================
// ���"ͶƱ"���ݱ�
//
//===================================================================
void CMainFrame::DelVote()
{
	try
	{
		if (!g_pRecordset2->BOF)
		{
			g_pRecordset2->MoveFirst();
			while(!g_pRecordset2->adoEOF)
			{
				g_pRecordset2->Delete(adAffectCurrent);
				g_pRecordset2->Update();
				g_pRecordset2->MoveNext();
			}
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("������ݱ�2����!"));
	}
}

//===================================================================
// ����"����"״̬
//
//===================================================================
void CMainFrame::UpdataConfig(CCmdUI *pCmdUI)
{
	if (m_Setting)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

//===================================================================
// ������ֹǰ
//
//===================================================================
void CMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// �ر���Դ
	//-> �ر��ͷż�¼��
	if (m_pRecord)
		CloseRecord();
	//-> �رշ����߳�
	if (s_Server_hThread)
		CloseHandle(s_Server_hThread);
	//-> �ر��׽���
	if (sockConn)
		closesocket(sockConn);
	if (sockSrv)
		closesocket(sockSrv);
	//-> �ر��׽��ֿ�
	WSACleanup();

	CFrameWnd::OnClose();
}

