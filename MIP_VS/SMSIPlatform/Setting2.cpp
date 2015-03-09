// Setting2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "Setting2.h"


// CSetting2 �Ի���

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


// CSetting2 ��Ϣ�������

BOOL CSetting2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// �������ļ���ȡ���ò���
	GetPrivateProfileString(_T("VoteSet"),_T("Max"),_T("0"),m_Max.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	m_Max.ReleaseBuffer();
	GetPrivateProfileString(_T("VoteSet"),_T("Num"),_T("3"),m_Num.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	m_Num.ReleaseBuffer();
	GetPrivateProfileString(_T("VoteSet"),_T("Item"),_T(""),m_Item.GetBuffer(MAX_PATH),MAX_PATH,_T(".\\Data\\Config.ini"));
	m_Item.ReleaseBuffer();

	// ����ؼ���ʼ��
	m_ListTime.SetRange(2,10);
	m_ListTime.SetTicFreq(2);
	int ilist;
	ilist = GetPrivateProfileInt(_T("VoteSet"),_T("LInterval"),10,_T(".\\Data\\Config.ini"));
	m_ListTime.SetPos(ilist);
	m_nListTime.Format(_T("%d"),ilist);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO: �ڴ����ר�ô����/����û���
	// ����س��ͽ���
	if(GetDlgItem(IDE_ESTIMATE) == GetFocus() ||
		GetDlgItem(IDE_ITEM) == GetFocus() ||
		GetDlgItem(IDC_EDIT_L2) == GetFocus()
		)
		GetParent()->SetFocus();

//	CDialog::OnOK();
}

void CSetting2::OnEnKillfocusEditL2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ʧȥ����ʱ���»���
	UpdateData();
	int pos = _tstoi(LPCTSTR(m_nListTime));
	m_ListTime.SetPos(pos);
}

void CSetting2::OnSliderL2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// ���±༭��
	int pos = m_ListTime.GetPos();
	m_nListTime.Format(_T("%d"),pos);
	UpdateData(FALSE);
}

CString CSetting2::ReturnListTime()
{
	return m_nListTime;
}
