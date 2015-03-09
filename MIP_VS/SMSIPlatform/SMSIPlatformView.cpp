// SMSIPlatformView.cpp : CSMSIPlatformView 类的实现
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "MainFrm.h"
#include "SMSIPlatformDoc.h"
#include "SMSIPlatformView.h"
#include "PIMNavigatorDlg.h"
#include "DblClick.h"
#include "ColorListCtrl.h"

// 全局变量
// 定义ADO连接、命令、记录集变量指针
_RecordsetPtr	m_pRecordset;
extern _ConnectionPtr	m_pConnection;

extern CPIMNavigatorDlg sms;
CColorListCtrl g_List;
extern int g_DClkFlag;
extern bool g_PauseFlag;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSMSIPlatformView

IMPLEMENT_DYNCREATE(CSMSIPlatformView, CView)

BEGIN_MESSAGE_MAP(CSMSIPlatformView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCTRL, &CSMSIPlatformView::OnNMDblclkList)
ON_WM_PAINT()
END_MESSAGE_MAP()

// CSMSIPlatformView 构造/析构

CSMSIPlatformView::CSMSIPlatformView()
{
	// TODO: 在此处添加构造代码

}

CSMSIPlatformView::~CSMSIPlatformView()
{
}

BOOL CSMSIPlatformView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CSMSIPlatformView 绘制

void CSMSIPlatformView::OnDraw(CDC* pDC)
{
	CSMSIPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

// CSMSIPlatformView 诊断

#ifdef _DEBUG
void CSMSIPlatformView::AssertValid() const
{
	CView::AssertValid();
}

void CSMSIPlatformView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSMSIPlatformDoc* CSMSIPlatformView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSMSIPlatformDoc)));
	return (CSMSIPlatformDoc*)m_pDocument;
}
#endif //_DEBUG


// CSMSIPlatformView 消息处理程序

void CSMSIPlatformView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	// 短信列表
	CRect rect;
	this->GetClientRect(&rect);
	g_List.Create( LVS_REPORT,CRect(0,0,rect.Width(),rect.Height()),this,IDC_LISTCTRL);
	g_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	g_List.ShowWindow(SW_SHOW);
	g_List.InsertColumn(0,_T("模式"),LVCFMT_LEFT,50);
	g_List.InsertColumn(1,_T("时间"),LVCFMT_LEFT,120);
	g_List.InsertColumn(2,_T("电话号码"),LVCFMT_LEFT,135);
	g_List.InsertColumn(3,_T("短信"),LVCFMT_LEFT,1200);
}

BOOL CSMSIPlatformView::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CView::DestroyWindow();
}

void CSMSIPlatformView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	this->GetClientRect(&rect);
	g_List.MoveWindow(rect);

	// 不为绘图消息调用 CView::OnPaint()
}

void CSMSIPlatformView::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此处添加消息处理程序代码
	CString state = CMainFrame::m_Control;
	if (state != _T("开始"))
	{
		return;
	}
	CString mstr = CMainFrame::Mode();
	if (mstr != _T("评论"))
	{
		return;
	}
	POSITION pos = g_List.GetFirstSelectedItemPosition();
	int tIndex = g_List.GetNextSelectedItem(pos);
	CString tstr = g_List.GetItemText(tIndex, 0);
	if (tIndex >= 0 && mstr == tstr)
	{
		CDblClick dlg;
		CString num = g_List.GetItemText(tIndex, 2);
		CString time = g_List.GetItemText(tIndex, 1);
		CString text = g_List.GetItemText(tIndex, 3);
		dlg.TransData(num, time, text, tIndex);
		g_DClkFlag = 1;
		dlg.DoModal();
		g_DClkFlag = 2;
		g_PauseFlag = false;
	}
	
	*pResult = 0;
}

