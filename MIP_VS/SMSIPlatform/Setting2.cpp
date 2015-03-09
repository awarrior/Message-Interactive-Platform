// Setting2.cpp : 实现文件
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "Setting2.h"


// CSetting2 对话框

IMPLEMENT_DYNAMIC(CSetting2, CDialog)

CSetting2::CSetting2(CWnd* pParent /*=NULL*/)
	: CDialog(CSetting2::IDD, pParent)
{	
	m_Max="";	
	m_Num="";		
	m_Item="";
	m_nListTime="";
}

CSetting2::~CSetting2()
{
}

void CSetting2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDE_ESTIMATE,m_Max);
	DDX_CBString(pDX,IDC_NUM,m_Num);
	DDX_Text(pDX,IDE_ITEM,m_Item);
	DDX_Control(pDX,IDC_SLIDER_L2,m_ListTime);
	DDX_Text(pDX,IDC_EDIT_L2,m_nListTime);
}


BEGIN_MESSAGE_MAP(CSetting2, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_L2, OnSliderL2)
	ON_EN_KILLFOCUS(IDC_EDIT_L2, &CSetting2::OnEnKillfocusEditL2)
END_MESSAGE_MAP()


// CSetting2 消息处理程序

BOOL CSetting2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 从配置文件读取设置参数
	GetPrivateProfileString(_T("VoteSet"),_T("Max"),_T("0"),m_Max.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	m_Max.ReleaseBuffer();
	GetPrivateProfileString(_T("VoteSet"),_T("Num"),_T("3"),m_Num.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	m_Num.ReleaseBuffer();
	GetPrivateProfileString(_T("VoteSet"),_T("Item"),_T(""),m_Item.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	m_Item.ReleaseBuffer();

	// 滑块控件初始化
	m_ListTime.SetRange(2,10);
	m_ListTime.SetTicFreq(2);
	int ilist;
	ilist = GetPrivateProfileInt(_T("VoteSet"),_T("LInterval"),10,_T(".\\Data\\Config.ini"));
	m_ListTime.SetPos(ilist);
	m_nListTime.Format(_T("%d"),ilist);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

CString CSetting2::ReturnMax()
{
	return m_Max;
}

CString CSetting2::ReturnNum()
{
	return m_Num;
}

CString CSetting2::ReturnItem()
{
	return m_Item;
}

void CSetting2::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	// 处理回车和焦点
	if(GetDlgItem(IDE_ESTIMATE) == GetFocus() ||
		GetDlgItem(IDE_ITEM) == GetFocus() ||
		GetDlgItem(IDC_EDIT_L2) == GetFocus()
		)
		GetParent()->SetFocus();

//	CDialog::OnOK();
}

void CSetting2::OnEnKillfocusEditL2()
{
	// TODO: 在此添加控件通知处理程序代码
	// 失去焦点时更新滑块
	UpdateData();
	int pos = _tstoi(LPCTSTR(m_nListTime));
	m_ListTime.SetPos(pos);
}

void CSetting2::OnSliderL2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 更新编辑框
	int pos = m_ListTime.GetPos();
	m_nListTime.Format(_T("%d"),pos);
	UpdateData(FALSE);
}

CString CSetting2::ReturnListTime()
{
	return m_nListTime;
}
