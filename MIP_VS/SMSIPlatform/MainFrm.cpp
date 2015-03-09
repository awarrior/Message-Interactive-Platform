// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "MainFrm.h"
#include "Config.h"
#include "ColorListCtrl.h"
#include "WaitForConnect.h"
#include <winsock2.h>

#define CHINA_TIME 8				// 中国时区

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 全局变量
CWaitForConnect dlg;
bool g_Connected = false;
int g_DClkFlag = 0;			// 双击标志

CPIMNavigatorDlg g_Sms;					// 用于短信读取处理的对象
_RecordsetPtr g_pRecordset1;			// 指向comment表,用于写入记录
_RecordsetPtr g_pRecordset1r;			// 指向comment表,用于读取记录
_RecordsetPtr g_pRecordset2;			// 指向vote表,用于写入记录
_RecordsetPtr g_pRecordset2r;			// 指向vote表,用于读取记录
extern _ConnectionPtr g_pConnection;	// 连接数据库对象
extern CColorListCtrl g_List;			// 列表控件
CArray<CString,CString> g_inArr;		// 索引数组，存储抽奖号码
SOCKET g_Socket;

bool g_PauseFlag = false;	// 选择暂停标志
int g_Year = 0;				// 参照年份
int g_Month = 0;			// 参照月份
int g_Day = 0;				// 参照日期
int g_Hour = 0;				// 参照小时
int g_Min = 0;				// 参照分钟
CString g_Phone = "";		// 手机型号
int g_Index = -1;			// 数据表最后记录索引值
bool g_Stand = false;		// 占用标记

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	// 控制
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
	// 设定
	ON_UPDATE_COMMAND_UI(ID_COMMENT,UpdataComment)
	ON_UPDATE_COMMAND_UI(ID_VOTE,UpdataVote)
	ON_UPDATE_COMMAND_UI(ID_LOTTERY,UpdataLottery)
	ON_UPDATE_COMMAND_UI(ID_CONFIGURATION,UpdataConfig)
	ON_COMMAND(ID_COMMENT,OnComment)
	ON_COMMAND(ID_VOTE,OnVote)
	ON_COMMAND(ID_LOTTERY,OnLottery)
	ON_COMMAND(ID_CONFIGURATION,OnConfig)
	// 帮助
	ON_COMMAND(ID_DOCUMENT,OnDocument)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_PHONETYPE,			// 电话型号
	ID_CONTROL,				// 控制状态
	ID_CONFIG,				// 模式
};

// 静态变量
CString CMainFrame::s_Mode = _T("评论");
HANDLE CMainFrame::s_Handle_hThread = NULL;
HANDLE CMainFrame::s_Server_hThread = NULL;
CString CMainFrame::m_Control = _T("停止");
SOCKET CMainFrame::sockConn = NULL;
SOCKET CMainFrame::sockSrv = NULL;
bool CMainFrame::s_Stop = true;
bool CMainFrame::s_Finish = true;

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	m_nTimer=0;
	m_pRecord=false;

	// 初始化菜单变量
	//-> 控制
	m_Begin=true;
	m_Pause=false;
	m_Stop=false;
	g_PauseFlag=false;
	//-> 设定
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
		TRACE0("未能创建工作区\n");
		return -1;		// 未能创建
	}
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
*/
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
/*
	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
*/
	// 菜单栏
	CMenu menu;
	menu.LoadMenu(IDR_MAINFRAME);
	SetMenu(&menu);
	menu.Detach();

	// 数据库文件连接
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
		AfxMessageBox(_T("数据库连接错误!"));
		::PostQuitMessage(0);
		return -1;
	}

	// 创建ADO记录集
	pRecordsetCreate();

	// 开启服务端线程
	s_Server_hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ServerFunProc,NULL,0,NULL);

	// 状态栏
	SetTimer(1,100,NULL);
	State();
	Configuration();

	// 双击
	SetTimer(3,500,NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.hMenu=NULL;
	cs.style&=~FWS_ADDTOTITLE;
	cs.lpszName=_T("短信互动平台");

	return TRUE;
}


// CMainFrame 诊断

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

// CMainFrame 消息处理程序

//===================================================================
// 创建ADO记录集
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
// 读取手机短信
// -> 线程
//
//===================================================================
DWORD WINAPI CMainFrame::HandleFunProc(LPVOID lpParameter)
{
	g_Stand = true;
	Sleep(10);
	g_Sms.GetInfo();
	g_Stand = false;

	// 关闭线程句柄
	if (s_Handle_hThread)
	{
		CloseHandle(s_Handle_hThread);
	}

	return 0;
}

//===================================================================
// 定时器
//
// 1 == 状态栏的"手机连接"显示
// 2 == 短信定时读取
//
//===================================================================
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
			if (str!=_T("没有连接"))
			{
				flag = true;
			}
			if (flag && str==_T("没有连接"))
			{
				OnStop();
				flag = false;
				AfxMessageBox(_T("手机连接已断开!"));
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
// 更新"评论"状态
//
//===================================================================
void CMainFrame::UpdataComment(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(1);
	pCmdUI->SetCheck(m_Comment);
}

//===================================================================
// 更新"投票"状态
//
//===================================================================
void CMainFrame::UpdataVote(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(1);
	pCmdUI->SetCheck(m_Vote);
}

//===================================================================
// 更新"抽奖"状态
//
//===================================================================
void CMainFrame::UpdataLottery(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(1);
	pCmdUI->SetCheck(m_Lottery);
}

//===================================================================
// 响应"评论"选项
//
//===================================================================
void CMainFrame::OnComment()
{
	m_Comment=true;
	m_Vote=false;
	m_Lottery=false;
	s_Mode=_T("评论");
	// 更新状态栏
	Configuration();
}

//===================================================================
// 响应"投票"选项
//
//===================================================================
void CMainFrame::OnVote()
{
	m_Comment=false;
	m_Vote=true;
	m_Lottery=false;
	s_Mode=_T("投票");
	// 更新状态栏
	Configuration();
}

//===================================================================
// 响应"抽奖"选项
//
//===================================================================
void CMainFrame::OnLottery()
{
	m_Comment=false;
	m_Vote=false;
	m_Lottery=true;
	s_Mode=_T("抽奖");
	// 更新状态栏
	Configuration();
}

//===================================================================
// 响应"配置"选项
//
//===================================================================
void CMainFrame::OnConfig()
{
	CConfig con;
	con.Mode(Mode());
	con.DoModal();
}

//===================================================================
// 打开"说明文档"
//
//===================================================================
void CMainFrame::OnDocument()
{
	ShellExecute(NULL, _T("open"), _T("Document.htm"), NULL, NULL, SW_SHOWNORMAL);	
}

//===================================================================
// 状态栏的"模式"显示
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
// 状态栏的"控制状态"显示
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
// 更新"开始/继续"状态
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
// 更新"暂停"状态
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
// 更新"停止"状态
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
// 返回当前所选模式
//
//===================================================================
CString CMainFrame::Mode()
{
	return s_Mode;
}

//===================================================================
// 服务端线程
//
//===================================================================
DWORD WINAPI CMainFrame::ServerFunProc(LPVOID lpParameter)
{
	WORD wVersionRequested;   
	WSADATA wsaData;   
	int err;

	//初始化套接字库
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

	// 创建套接字
	sockSrv = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,0);
	if (sockSrv == INVALID_SOCKET)
	{
		AfxMessageBox(_T("创建套接字失败!"));
		return 0;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);   
	addrSrv.sin_family=AF_INET;   
	addrSrv.sin_port=htons(6000);   

	// 绑定套接字
	if (bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("绑定失败!"));
	}

	// 将套接字设为监听模式，准备接收客户
	listen(sockSrv,1);  

	SOCKADDR_IN addrClient;   
	int len = sizeof(SOCKADDR); 

	bool cntn = false;
	g_Connected = false;

	while(1)
	{
		// 等待客户请求到来 
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
		// 发送测试数据
		char *pString = "test";
		if(send(sockConn,pString,strlen(pString),0) == SOCKET_ERROR)   
		{   
			// 发送数据失败
			g_Connected = false;
			continue;  
		}

		// 循环标志
		bool flag = false;

		while(1)
		{
			s_Finish = true;
			// 开始演示
			if (m_Control == _T("开始"))
			{
				s_Finish = false;
				// 模式处理
				if (Mode() == "评论")
				{
					if (s_Stop)
					{
						// 发送数据
						char *pString = "m1";
						if(send(sockConn,pString,strlen(pString),0) == SOCKET_ERROR)   
						{   
							// 发送数据失败
							break;   
						}
						s_Stop = false;
					}
					CFunProc(flag);
				}
				else if (Mode() == "投票")
				{
					if (s_Stop)
					{
						// 获取配置
						CString max,num,item;
						GetPrivateProfileString(_T("VoteSet"),_T("Max"),_T("0"),max.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
						max.ReleaseBuffer();
						GetPrivateProfileString(_T("VoteSet"),_T("Num"),_T("3"),num.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
						num.ReleaseBuffer();
						GetPrivateProfileString(_T("VoteSet"),_T("Item"),_T(""),item.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
						item.ReleaseBuffer();

						// 信息整合
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

						// 类型转换 CString to char*
						int nSize = msg.GetLength();
						char *pString = new char[3*nSize];
						WideCharToMultiByte(CP_UTF8, 0, msg, -1, pString, 3*nSize, NULL, NULL);

						// 发送数据
						if(send(sockConn,pString,3*nSize,0) == SOCKET_ERROR)   
						{   
							// 发送数据失败
							break;   
						}

						s_Stop = false;
					}
					VFunProc(flag);
				}
				else if (Mode() == "抽奖")
				{
					if (s_Stop)
					{
						// 发送数据
						char *pString = "m3";
						if(send(sockConn,pString,strlen(pString),0) == SOCKET_ERROR)   
						{   
							// 发送数据失败
							break;   
						}
						// 满足条件索引数组存储
						IndexSave();

						s_Stop = false;
					}
					// 随机抽取
					LFunProc(flag);
				}
				if (flag)
				{
					break;
				}
			}
			// 其他状态
			else
			{
				// 发送心跳包
				char *pString = "dot";
				if(send(sockConn,pString,strlen(pString),0) == SOCKET_ERROR)   
				{   
					// 发送数据失败
					break; 
				}
				Sleep(500);
			}
		}

		// 关闭套接字  
		closesocket(sockConn);
	}
	
	// 关闭套接字
	closesocket(sockSrv);

	// 关闭套接字库
	WSACleanup();

	return 0;
}

//===================================================================
// 睡眠
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
// 服务端线程
// - 评论模式处理
//
//===================================================================
void CMainFrame::CFunProc(bool &flag)
{
	_variant_t var;
	// 间隔时间
	static int ishow = 0;
	static int ilist = 0;
	// 第一次发送数据标记
	static bool firflag = false;

	try
	{
		// 第一次发送数据
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
			// 打开连接
			g_pRecordset1r->Open("SELECT * FROM comment",
				g_pConnection.GetInterfacePtr(),
				adOpenDynamic,
				adLockOptimistic,
				adCmdText);

			// 记录集移动到最后记录
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
			// 判断已读标记
			CString rflg = "";
			var = g_pRecordset1r->GetCollect("read_flag");
			if(var.vt != VT_NULL)
				rflg = (LPCTSTR)_bstr_t(var);

			// 判断删除标记
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
				// 等待
				//aSleep(ilist);
				g_pRecordset1r->MoveNext();
				return;
			}
			else if (dflg == "-1")
			{
				g_pRecordset1r->MoveNext();
				return;
			}

			// 读取手机号码
			CString phonum = "";
			var = g_pRecordset1r->GetCollect("number");
			if(var.vt != VT_NULL)
				phonum = (LPCTSTR)_bstr_t(var);
			CString number = phonum;

			// 读取短信内容
			CString message = "";
			var = g_pRecordset1r->GetCollect("text");
			if(var.vt != VT_NULL)
				message = (LPCTSTR)_bstr_t(var);

			// 信息整合
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

			// 类型转换 CString to char*
			int nSize = msg.GetLength();
			char *pString = new char[3*nSize];	
			memset(pString, 0, 3*nSize);
			WideCharToMultiByte(CP_UTF8, 0, msg, -1, pString, 3*nSize, NULL, NULL);

			// 发送数据
			if(send(sockConn,pString,3*nSize,0) == SOCKET_ERROR)   
			{   
				// 发送数据失败
				flag = true;
				delete pString;
				return;   
			}
			delete pString;

			// 读取短信时间
			CString time = "";
			var = g_pRecordset1r->GetCollect("time");
			if(var.vt != VT_NULL)
				time = (LPCTSTR)_bstr_t(var);

			// 修改已读属性
			g_pRecordset1r->PutCollect("read_flag", _variant_t(_T("True")));
			g_pRecordset1r->Update();

			// 读取下一条数据
			g_pRecordset1r->MoveNext();

			// 已读变绿色
			for (int i=g_List.GetItemCount()-1; i>=0; i--)
			{
				CString cmp = g_List.GetItemText(i,0);
				if (cmp == "评论")
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

			// 等待
			aSleep(ishow);
		}
		else
		{
			// 关闭读取记录集
			if(g_pRecordset1r->State)
				g_pRecordset1r->Close();

			// 等待
			aSleep(ilist);
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("数据库读取错误!"));
		return;
	}
}

//===================================================================
// 服务端线程
// - 投票模式处理
//
//===================================================================
void CMainFrame::VFunProc(bool &flag)
{
	_variant_t var;
	// 间隔时间
	static int ilist = 0;
	// 第一次发送数据标记
	static bool firflag = false;

	try
	{
		// 第一次发送数据
		if (!firflag)
		{
			ilist = GetPrivateProfileInt(_T("VoteSet"),_T("LInterval"),10,_T(".\\Data\\Config.ini"));
			ilist *= 1000;
			firflag = true;
		}

		if(!g_pRecordset2r->State)
		{
			// 打开连接
			g_pRecordset2r->Open("SELECT * FROM vote",
				g_pConnection.GetInterfacePtr(),
				adOpenDynamic,
				adLockOptimistic,
				adCmdText);

			// 记录集移动到最后记录
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
			// 判断已读标记
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
				// 等待
				aSleep(ilist);
				return;
			}

			// 读取短信内容
			CString message = "";
			var = g_pRecordset2r->GetCollect("text");
			if(var.vt != VT_NULL)
				message = (LPCTSTR)_bstr_t(var);

			// 信息整合
			CString msg = "";
			if (message.GetLength() != 1)
			{
				AfxMessageBox(_T("投票数据错误!"));
				return;
			}
			else
			{
				msg = _T("v")+message;
			}

			// 类型转换 CString to char*
			int nSize = msg.GetLength();
			char *pString = new char[3*nSize];
			WideCharToMultiByte(CP_UTF8, 0, msg, -1, pString, 3*nSize, NULL, NULL);

			// 发送数据
			if(send(sockConn,pString,3*nSize,0) == SOCKET_ERROR)   
			{   
				// 发送数据失败
				flag = true;
				delete pString;
				return;   
			}
			delete pString;

			// 读取短信时间
			CString time = "";
			var = g_pRecordset2r->GetCollect("time");
			if(var.vt != VT_NULL)
				time = (LPCTSTR)_bstr_t(var);

			// 读取手机号码
			CString number = "";
			var = g_pRecordset2r->GetCollect("number");
			if(var.vt != VT_NULL)
				number = (LPCTSTR)_bstr_t(var);

			// 修改已读属性
			g_pRecordset2r->PutCollect("read_flag", _variant_t(_T("True")));
			g_pRecordset2r->Update();

			// 读取下一条数据
			g_pRecordset2r->MovePrevious();

			// 已读变绿色
			for (int i=g_List.GetItemCount()-1; i>=0; i--)
			{
				CString cmp = g_List.GetItemText(i,0);
				if (cmp == "投票")
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
			// 关闭读取记录集
			if(g_pRecordset2r->State)
				g_pRecordset2r->Close();

			// 等待
			aSleep(ilist);
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("数据库读取错误!"));
		return;
	}
}

//===================================================================
// 服务端线程
// - 抽奖模式处理
//
//===================================================================
void CMainFrame::LFunProc(bool &flag)
{
	//设置为非阻塞模式
	unsigned long flg=1;  
	if (ioctlsocket(sockConn,FIONBIO,&flg)!=0)  
	{  
		closesocket(sockConn);  
		return;
	}  

	// 接收数据
	char buf[10];
	int bytes = 0;
	if((bytes=recv(sockConn,buf,5/*sizeof(buf)*/,0)) == SOCKET_ERROR)   
	{
		return;
	}
	else
	{
		// 发送消息到Show
		char *pString = "o";
		if(send(sockConn,pString,strlen(pString),0) == SOCKET_ERROR)   
		{   
			// 发送数据失败
		}
	}
	buf[bytes]='\0';

	// char to CString
	int charLen = strlen(buf);
	//计算多字节字符的大小，按字符计算
	int len = MultiByteToWideChar(CP_ACP,0,buf,charLen,NULL,0);
	//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
	TCHAR *buffer = new TCHAR[len + 1];
	//多字节编码转换成宽字节编码
	MultiByteToWideChar(CP_ACP,0,buf,charLen,buffer,len);
	buffer[len] = '\0'; //添加字符串结尾
	//将TCHAR数组转换为CString
	CString recstr = "";
	recstr.Append(buffer);
	//删除缓冲区
	delete []buffer;

	if (recstr == _T("click"))
	{
		// 随机抽取手机号码
		CString res = RandomGet();

		// 信息整合
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

		// 类型转换 CString to char*
		int nSize = msg.GetLength();
		char *pString = new char[3*nSize];
		WideCharToMultiByte(CP_UTF8, 0, msg, -1, pString, 3*nSize, NULL, NULL);

		// 缓冲时间
		aSleep(2000);

		// 发送数据
		if(send(sockConn,pString,3*nSize,0) == SOCKET_ERROR)   
		{   
			// 发送数据失败
			flag = true;
			delete pString;
			return;   
		}
		delete pString;
	}

	//设置为阻塞模式
	flg=0;  
	if (ioctlsocket(sockConn,FIONBIO,&flg)!=0)  
	{  
		closesocket(sockConn);  
		return;
	} 
}

//===================================================================
// 抽奖索引数组存储
// - 返回值为有效计数大小
//
//===================================================================
void CMainFrame::IndexSave()
{
	try
	{
		int i;
		_variant_t var;
		CString flg = "";
		// 评论和投票号码临时串
		CString cStr = "";
		CString vStr = "";
		// 从配置文件读取设置参数
		CString participant,chance;
		GetPrivateProfileString(_T("LotterySet"),_T("Participant"),_T("全部"),participant.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
		participant.ReleaseBuffer();
		GetPrivateProfileString(_T("LotterySet"),_T("Chance"),_T("一号多票"),chance.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
		chance.ReleaseBuffer();
		// 参与者分类
		if(participant == "全部")
		{
			// comment表
			if(!g_pRecordset1r->State)
			{
				// 打开连接
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
					if (chance == _T("一号一票"))
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
					if (chance == _T("一号多票"))
					{
						g_inArr.Add(cStr);
					}
				}
				g_pRecordset1r->MoveNext();
			}
			// 关闭读取记录集
			if(g_pRecordset1r->State)
				g_pRecordset1r->Close();

			// vote表
			if(!g_pRecordset2r->State)
			{
				// 打开连接
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
				if (chance == _T("一号一票"))
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
				if (chance == _T("一号多票"))
				{
					g_inArr.Add(vStr);
				}
				g_pRecordset2r->MoveNext();
			}
			// 关闭读取记录集
			if(g_pRecordset2r->State)
				g_pRecordset2r->Close();

			return;
		}
		if(participant == "评论")
		{
			// comment表
			if(!g_pRecordset1r->State)
			{
				// 打开连接
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
					if (chance == _T("一号一票"))
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
					if (chance == _T("一号多票"))
					{
						g_inArr.Add(cStr);
					}
				}
				g_pRecordset1r->MoveNext();
			}
			// 关闭读取记录集
			if(g_pRecordset1r->State)
				g_pRecordset1r->Close();

			return;
		}
		if(participant == "投票")
		{
			// vote表
			if(!g_pRecordset2r->State)
			{
				// 打开连接
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
				if (chance == _T("一号一票"))
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
				if (chance == _T("一号多票"))
				{
					g_inArr.Add(vStr);
				}
				g_pRecordset2r->MoveNext();
			}
			// 关闭读取记录集
			if(g_pRecordset2r->State)
				g_pRecordset2r->Close();

			return;
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("数据库读取错误!"));
	}
}

//===================================================================
// 随机抽取手机号码
//
//===================================================================
CString CMainFrame::RandomGet()
{
	// 判断是否抽空
	int size = g_inArr.GetSize();
	if (size == 0)
	{
		return "";
	}

	// 从配置文件读取设置参数
	CString participant,chance;
	GetPrivateProfileString(_T("LotterySet"),_T("Participant"),_T("全部"),participant.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	participant.ReleaseBuffer();
	GetPrivateProfileString(_T("LotterySet"),_T("Chance"),_T("一号多票"),chance.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	chance.ReleaseBuffer();

	// 获取随机数并取出索引值
	CTime now = CTime::GetCurrentTime();
	time_t time = now.GetTime();
	srand((unsigned)time);
	int val = rand();
	val %= size;
	CString phone = g_inArr[val];

	// 删除已抽对象
	if (chance == _T("一号一票"))
	{
		g_inArr.RemoveAt(val);
	}
	if (chance == _T("一号多票"))
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

	// 返回手机号码 
	return phone;
}

//===================================================================
// 开始/继续
//
//===================================================================
void CMainFrame::OnBegin()
{
	CString str = Mode();
	// 获取时间间隔参数
	int ilist;
	if (str == _T("评论") || str == _T("投票"))
	{
		if (str == _T("评论"))
		{
			ilist = GetPrivateProfileInt(_T("CommentSet"),_T("LInterval"),10,_T(".\\Data\\Config.ini"));
		}
		if (str == _T("投票"))
		{
			ilist = GetPrivateProfileInt(_T("VoteSet"),_T("LInterval"),10,_T(".\\Data\\Config.ini"));
		}
		ilist *= 1000;
	}

	if (m_Control == _T("停止"))
	{
		// 评论和投票模式
		if (str == _T("评论") || str == _T("投票"))
		{
			// 连接手机提示
			if (g_Phone=="没有连接")
			{
				MessageBox(_T("请先连接手机"),_T("提示"),MB_ICONSTOP);
				return;
			}

			// 菜单状态更变
			m_Begin = false;
			m_Pause = true;
			m_Stop = true;

			// 获取当前时间
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

			// 打开ADO记录集
			if (str == _T("评论"))
			{
				OpenRecord1();
			}
			if (str == _T("投票"))
			{
				OpenRecord2();
			}

			try
			{
				// 未读短信提醒
				if (str == _T("评论"))
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
								if (MessageBox(_T("发现有上次未读数据,\n是否读取?"), _T("未读数据提醒"), MB_ICONQUESTION | MB_YESNO) == IDNO)
								{
									// 数据全部更新为已读
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
				if (str == _T("投票"))
				{
					if (!g_pRecordset2->BOF)
					{
						// 数据全部更新为已读
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
				AfxMessageBox(_T("数据库读取或写入错误!"));
			}

			// 参数初始化
			g_Sms.ModeSet(str);
			g_Sms.GetReady();

			// 从手机读取短信
			m_nTimer = SetTimer(2,ilist,NULL);
		}

		// 抽奖模式
		// 菜单状态更变
		if (str == _T("抽奖"))
		{
			m_Begin = false;
			m_Pause = false;
			m_Stop = true;
		}

		// 更新状态栏
		m_Control = _T("开始");
		State();

		// 菜单属性更变
		GetMenu()->GetSubMenu(0)->EnableMenuItem(5,MF_BYPOSITION | MF_DISABLED);	// 清空数据
		GetMenu()->GetSubMenu(1)->EnableMenuItem(0,MF_BYPOSITION | MF_DISABLED);	// 模式
		m_Setting = false;

		return;
	}
	if (m_Control == _T("暂停"))
	{
		// 菜单状态更变
		m_Begin = false;
		m_Pause = true;
		m_Stop = true;

		// 更新状态栏
		m_Control = _T("开始");
		State();

		// 恢复线程
		ResumeThread(s_Server_hThread);
		if (str == _T("评论") || str == _T("投票"))
		{
			ResumeThread(s_Handle_hThread);
			// 从手机读取短信
			m_nTimer = SetTimer(2,ilist,NULL);
		}
	}
}

//===================================================================
// 暂停演示
//
//===================================================================
void CMainFrame::OnPause()
{
	// 更新状态栏
	m_Control = _T("暂停");
	State();

	// 菜单属性更变
	m_Begin = true;
	m_Pause = false;
	m_Stop = true;

	// 杀掉定时器
	g_PauseFlag = true;
	KillTimer(m_nTimer);

	// 挂起线程
	SuspendThread(s_Handle_hThread);
	SuspendThread(s_Server_hThread);
}

//===================================================================
// 停止演示
//
//===================================================================
void CMainFrame::OnStop()
{
	// 更新状态栏
	m_Control = _T("停止");
	State();

	// 发送消息到Show
	if (Mode() == _T("评论") || Mode() == _T("投票"))
	{
		s_Stop = true;
		char *pString = "s";
		if(send(sockConn,pString,strlen(pString),0) == SOCKET_ERROR)   
		{   
			// 发送数据失败
		}
	}
	else if (Mode() == _T("抽奖"))
	{
		while(1)
		{
			if (s_Finish)
			{
				char *pString = "s";
				if(send(sockConn,pString,strlen(pString),0) == SOCKET_ERROR)   
				{   
					// 发送数据失败
				}
				s_Stop = true;
				break;
			}
		}
	}

	// 数据全设为已读
	try
	{
		if (Mode() == _T("评论"))
		{
			// 打开ADO记录集
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
			// 关闭记录集
			CloseRecord1();
		}
		else if (Mode() == _T("投票"))
		{
			// 打开ADO记录集
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
			// 关闭记录集
			CloseRecord2();
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("数据库修改错误!"));
	}

	// 填充列表颜色
	int listnum = g_List.GetItemCount();
	for (int c=0; c<listnum; c++)
	{
		if (!g_List.ChangedItemColor(c))
		{
			g_List.SetItemColor(c, RGB(0,0,0), RGB(239,255,206));
		}
	}

	// 菜单属性更变
	m_Begin = true;
	m_Pause = false;
	m_Stop = false;
	m_Setting = true;

	GetMenu()->GetSubMenu(0)->EnableMenuItem(5,MF_BYPOSITION | MF_ENABLED);	// 清空数据
	GetMenu()->GetSubMenu(1)->EnableMenuItem(0,MF_BYPOSITION | MF_ENABLED);	// 模式

	// 杀掉定时器
	if (!g_PauseFlag)
	{
		KillTimer(m_nTimer);
	}	
	g_PauseFlag = false;

	// 关闭线程句柄
	if (s_Handle_hThread)
	{
		CloseHandle(s_Handle_hThread);
	}
}

//===================================================================
// 打开记录集1,指向comment数据表
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
		AfxMessageBox(_T("数据表1打开错误!"));
	}
}

//===================================================================
// 打开记录集2,指向vote数据表
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
		AfxMessageBox(_T("数据表2打开错误!"));
	}
}

//===================================================================
// 关闭记录集1
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
		AfxMessageBox(_T("数据表1关闭错误!"));
	}
}

//===================================================================
// 关闭记录集1read
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
		AfxMessageBox(_T("数据表1关闭错误!"));
	}
}

//===================================================================
// 关闭记录集2
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
		AfxMessageBox(_T("数据表2关闭错误!"));
	}
}

//===================================================================
// 关闭记录集2read
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
		AfxMessageBox(_T("数据表2关闭错误!"));
	}
}

//===================================================================
// 关闭记录集合并
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
// 清屏操作
//
//===================================================================
void CMainFrame::OnClear()
{
	g_List.DeleteAllItems();
}

//===================================================================
// 清空数据 - 全部
//
//===================================================================
void CMainFrame::OnDelAll()
{
	// 创建ADO记录集
	pRecordsetCreate();

	// 打开ADO记录集
	OpenRecord1();
	OpenRecord2();

	// 清空操作
	DelComment();
	DelVote();	

	// 关闭ADO记录集
	CloseRecord1();
	CloseRecord2();

	// 清空列表
	g_List.DeleteAllItems();

	// 提示
	AfxMessageBox(_T("数据清空成功!"));
}

//===================================================================
// 清空数据 - 评论
//
//===================================================================
void CMainFrame::OnDelComment()
{
	// 创建ADO记录集
	pRecordsetCreate();

	// 打开ADO记录集
	OpenRecord1();

	// 清空操作
	DelComment();

	// 关闭ADO记录集
	CloseRecord1();

	// 清空列表相应记录
	for (int i=0; i<g_List.GetItemCount();)
	{
		if (g_List.GetItemText(i,0) == _T("评论"))
		{
			g_List.DeleteItem(i);
		}
		else
		{
			i++;
		}
	}

	// 填充列表颜色
	int listnum = g_List.GetItemCount();
	for (int c=0; c<listnum; c++)
	{
		if (!g_List.ChangedItemColor(c))
		{
			g_List.SetItemColor(c, RGB(0,0,0), RGB(239,255,206));
		}
	}

	// 提示
	AfxMessageBox(_T("数据清空成功!"));
}

//===================================================================
// 清空数据 - 投票
//
//===================================================================
void CMainFrame::OnDelVote()
{
	// 创建ADO记录集
	pRecordsetCreate();

	// 打开ADO记录集
	OpenRecord2();

	// 清空操作
	DelVote();	

	// 关闭ADO记录集
	CloseRecord2();

	// 清空列表相应记录
	for (int i=0; i<g_List.GetItemCount();)
	{
		if (g_List.GetItemText(i,0) == _T("投票"))
		{
			g_List.DeleteItem(i);
		}
		else
		{
			i++;
		}
	}

	// 填充列表颜色
	int listnum = g_List.GetItemCount();
	for (int c=0; c<listnum; c++)
	{
		if (!g_List.ChangedItemColor(c))
		{
			g_List.SetItemColor(c, RGB(0,0,0), RGB(239,255,206));
		}
	}

	// 提示
	AfxMessageBox(_T("数据清空成功!"));
}

//===================================================================
// 清空"评论"数据表
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
		AfxMessageBox(_T("清空数据表1错误!"));
	}
}

//===================================================================
// 清空"投票"数据表
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
		AfxMessageBox(_T("清空数据表2错误!"));
	}
}

//===================================================================
// 更新"配置"状态
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
// 程序终止前
//
//===================================================================
void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 关闭资源
	//-> 关闭释放记录集
	if (m_pRecord)
		CloseRecord();
	//-> 关闭服务线程
	if (s_Server_hThread)
		CloseHandle(s_Server_hThread);
	//-> 关闭套接字
	if (sockConn)
		closesocket(sockConn);
	if (sockSrv)
		closesocket(sockSrv);
	//-> 关闭套接字库
	WSACleanup();

	CFrameWnd::OnClose();
}

