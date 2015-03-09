// SMSIPlatformView.cpp : CSMSIPlatformView ���ʵ��
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "MainFrm.h"
#include "SMSIPlatformDoc.h"
#include "SMSIPlatformView.h"
#include "PIMNavigatorDlg.h"
#include "DblClick.h"
#include "ColorListCtrl.h"

// ȫ�ֱ���
// ����ADO���ӡ������¼������ָ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCTRL, &CSMSIPlatformView::OnNMDblclkList)
ON_WM_PAINT()
END_MESSAGE_MAP()

// CSMSIPlatformView ����/����

CSMSIPlatformView::CSMSIPlatformView()
{
	// TODO: �ڴ˴���ӹ������

}

CSMSIPlatformView::~CSMSIPlatformView()
{
}

BOOL CSMSIPlatformView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CSMSIPlatformView ����

void CSMSIPlatformView::OnDraw(CDC* pDC)
{
	CSMSIPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

// CSMSIPlatformView ���

#ifdef _DEBUG
void CSMSIPlatformView::AssertValid() const
{
	CView::AssertValid();
}

void CSMSIPlatformView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSMSIPlatformDoc* CSMSIPlatformView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSMSIPlatformDoc)));
	return (CSMSIPlatformDoc*)m_pDocument;
}
#endif //_DEBUG


// CSMSIPlatformView ��Ϣ�������

void CSMSIPlatformView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	// �����б�
	CRect rect;
	this->GetClientRect(&rect);
	g_List.Create( LVS_REPORT,CRect(0,0,rect.Width(),rect.Height()),this,IDC_LISTCTRL);
	g_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	g_List.ShowWindow(SW_SHOW);
	g_List.InsertColumn(0,_T("ģʽ"),LVCFMT_LEFT,50);
	g_List.InsertColumn(1,_T("ʱ��"),LVCFMT_LEFT,120);
	g_List.InsertColumn(2,_T("�绰����"),LVCFMT_LEFT,135);
	g_List.InsertColumn(3,_T("����"),LVCFMT_LEFT,1200);
}

BOOL CSMSIPlatformView::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���

	return CView::DestroyWindow();
}

void CSMSIPlatformView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	this->GetClientRect(&rect);
	g_List.MoveWindow(rect);

	// ��Ϊ��ͼ��Ϣ���� CView::OnPaint()
}

void CSMSIPlatformView::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ˴������Ϣ����������
	CString state = CMainFrame::m_Control;
	if (state != _T("��ʼ"))
	{
		return;
	}
	CString mstr = CMainFrame::Mode();
	if (mstr != _T("����"))
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

