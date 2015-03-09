// Setting3.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "Setting3.h"


// CSetting3 �Ի���

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


// CSetting3 ��Ϣ�������

BOOL CSetting3::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// �������ļ���ȡ���ò���
	GetPrivateProfileString(_T("LotterySet"),_T("Participant"),_T("ȫ��"),m_Participant.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	m_Participant.ReleaseBuffer();
	GetPrivateProfileString(_T("LotterySet"),_T("Chance"),_T("һ�Ŷ�Ʊ"),m_Chance.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	m_Chance.ReleaseBuffer();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO: �ڴ����ר�ô����/����û���
	// ����س��ͽ���
	if(GetDlgItem(IDC_PARTICIPANT) == GetFocus() ||
		GetDlgItem(IDC_CHANCE) == GetFocus()
		)
		GetParent()->SetFocus();

//	CDialog::OnOK();
}
