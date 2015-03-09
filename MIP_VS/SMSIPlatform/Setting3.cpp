// Setting3.cpp : 实现文件
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "Setting3.h"


// CSetting3 对话框

IMPLEMENT_DYNAMIC(CSetting3, CDialog)

CSetting3::CSetting3(CWnd* pParent /*=NULL*/)
	: CDialog(CSetting3::IDD, pParent)
{
	m_Participant="";
	m_Chance="";
}

CSetting3::~CSetting3()
{
}

void CSetting3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX,IDC_PARTICIPANT,m_Participant);
	DDX_CBString(pDX,IDC_TICKET,m_Chance);
}


BEGIN_MESSAGE_MAP(CSetting3, CDialog)
END_MESSAGE_MAP()


// CSetting3 消息处理程序

BOOL CSetting3::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 从配置文件读取设置参数
	GetPrivateProfileString(_T("LotterySet"),_T("Participant"),_T("全部"),m_Participant.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	m_Participant.ReleaseBuffer();
	GetPrivateProfileString(_T("LotterySet"),_T("Chance"),_T("一号多票"),m_Chance.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	m_Chance.ReleaseBuffer();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

CString CSetting3::ReturnParticipant()
{
	return m_Participant;
}

CString CSetting3::ReturnChance()
{
	return m_Chance;
}

void CSetting3::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	// 处理回车和焦点
	if(GetDlgItem(IDC_PARTICIPANT) == GetFocus() ||
		GetDlgItem(IDC_CHANCE) == GetFocus()
		)
		GetParent()->SetFocus();

//	CDialog::OnOK();
}
