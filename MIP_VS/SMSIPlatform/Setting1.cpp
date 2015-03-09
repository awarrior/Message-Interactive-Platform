// Setting1.cpp : 实现文件
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "Setting1.h"

// CSetting1 对话框

IMPLEMENT_DYNAMIC(CSetting1, CDialog)

CSetting1::CSetting1(CWnd* pParent /*=NULL*/)
	: CDialog(CSetting1::IDD, pParent)
{
	m_Pro="";
	m_Key="";
	m_nListTime="";
	m_nShowTime="";
}

CSetting1::~CSetting1()
{
}

void CSetting1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDE_PRO1,m_Pro);
	DDX_Text(pDX,IDE_KEYWORD,m_Key);
	DDX_Control(pDX,IDC_SLIDER_L1,m_ListTime);
	DDX_Control(pDX,IDC_SLIDER_S,m_ShowTime);
	DDX_Text(pDX,IDC_EDIT_L1,m_nListTime);
	DDX_Text(pDX,IDC_EDIT_S,m_nShowTime);
}


BEGIN_MESSAGE_MAP(CSetting1, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_L1, OnSliderL1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_S, OnSliderS)
	ON_EN_KILLFOCUS(IDC_EDIT_L1, &CSetting1::OnEnKillfocusEditL1)
	ON_EN_KILLFOCUS(IDC_EDIT_S, &CSetting1::OnEnKillfocusEditS)
END_MESSAGE_MAP()


// CSetting1 消息处理程序

BOOL CSetting1::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 从配置文件读取设置参数
	GetPrivateProfileString(_T("CommentSet"),_T("Pro"),_T(""),m_Pro.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	m_Pro.ReleaseBuffer();
	GetPrivateProfileString(_T("CommentSet"),_T("Key"),_T(""),m_Key.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	m_Key.ReleaseBuffer();

	// 滑块控件初始化
	m_ListTime.SetRange(10,20);
	m_ListTime.SetTicFreq(2);
	int ilist;
	ilist = GetPrivateProfileInt(_T("CommentSet"),_T("LInterval"),10,_T(".\\Data\\Config.ini"));
	m_ListTime.SetPos(ilist);
	m_nListTime.Format(_T("%d"),ilist);

	m_ShowTime.SetRange(2,30);
	m_ShowTime.SetTicFreq(4);
	int ishow;
	ishow = GetPrivateProfileInt(_T("CommentSet"),_T("SInterval"),10,_T(".\\Data\\Config.ini"));
	m_ShowTime.SetPos(ishow);
	m_nShowTime.Format(_T("%d"),ishow);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

CString CSetting1::ReturnPro()
{
	return m_Pro;
}

CString CSetting1::ReturnKey()
{
	return m_Key;
}

void CSetting1::OnSliderL1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 更新编辑框
	int pos = m_ListTime.GetPos();
	m_nListTime.Format(_T("%d"),pos);
	UpdateData(FALSE);
}

void CSetting1::OnSliderS(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 更新编辑框
	int pos = m_ShowTime.GetPos();
	m_nShowTime.Format(_T("%d"),pos);
	UpdateData(FALSE);
}

CString CSetting1::ReturnListTime()
{
	return m_nListTime;
}

CString CSetting1::ReturnShowTime()
{
	return m_nShowTime;
}
void CSetting1::OnEnKillfocusEditL1()
{
	// TODO: 在此添加控件通知处理程序代码
	// 失去焦点时更新滑块
	UpdateData();
	int pos = _tstoi(LPCTSTR(m_nListTime));
	m_ListTime.SetPos(pos);
}

void CSetting1::OnEnKillfocusEditS()
{
	// TODO: 在此添加控件通知处理程序代码
	// 失去焦点时更新滑块
	UpdateData();
	int pos = _tstoi(LPCTSTR(m_nShowTime));
	m_ShowTime.SetPos(pos);
}

void CSetting1::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	// 处理回车和焦点
	if(GetDlgItem(IDC_EDIT_L1) == GetFocus() ||
		GetDlgItem(IDC_EDIT_S) == GetFocus() ||
		GetDlgItem(IDE_PRO1) == GetFocus() ||
		GetDlgItem(IDE_KEYWORD) == GetFocus()
		)
		GetParent()->SetFocus();

//	CDialog::OnOK();
}
