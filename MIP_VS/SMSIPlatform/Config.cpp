// Config.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "Config.h"


// CConfig �Ի���

IMPLEMENT_DYNAMIC(CConfig, CDialog)

CConfig::CConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CConfig::IDD, pParent)
{

}

CConfig::~CConfig()
{
}

void CConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_Tree);
}


BEGIN_MESSAGE_MAP(CConfig, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CConfig::OnTvnSelchangedTree)
	ON_BN_CLICKED(IDOK, &CConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfig ��Ϣ�������

void CConfig::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ���ؼ���Ӧ�Ի���
	HTREEITEM hitem;   
	hitem = m_Tree.GetSelectedItem();
	CString str = m_Tree.GetItemText(hitem);
	if (!str.Compare(_T("����")))
	{
		m_Set1.ShowWindow(SW_SHOW);
		m_Set2.ShowWindow(SW_HIDE);
		m_Set3.ShowWindow(SW_HIDE);
		m_Tree.SelectDropTarget(m_hSet1);
	}
	else if (!str.Compare(_T("ͶƱ")))
	{
		m_Set1.ShowWindow(SW_HIDE);
		m_Set2.ShowWindow(SW_SHOW);
		m_Set3.ShowWindow(SW_HIDE);
		m_Tree.SelectDropTarget(m_hSet2);
	}
	else if (!str.Compare(_T("�齱")))
	{
		m_Set1.ShowWindow(SW_HIDE);
		m_Set2.ShowWindow(SW_HIDE);
		m_Set3.ShowWindow(SW_SHOW);
		m_Tree.SelectDropTarget(m_hSet3);
	}

	*pResult = 0;
}

BOOL CConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ʼ�����ؼ�
	m_Tree.ModifyStyle(0,TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES);
	m_Tree.DeleteAllItems();
	HTREEITEM hRoot=m_Tree.InsertItem(_T("����"));
	m_hSet1 = m_Tree.InsertItem(_T("����"),hRoot);
	m_hSet2 = m_Tree.InsertItem(_T("ͶƱ"),hRoot);
	m_hSet3 = m_Tree.InsertItem(_T("�齱"),hRoot);
	m_Tree.Expand(hRoot,TVE_EXPAND);

	// ����3�����öԻ���
	m_Set1.Create(IDD_DIALOG1, this);
	m_Set2.Create(IDD_DIALOG2, this);
	m_Set3.Create(IDD_DIALOG3, this);

	// ���öԻ���ָ��λ��
	CRect rec;
	GetDlgItem(IDC_SETPART)->GetWindowRect(rec);
	ScreenToClient(rec);
	GetDlgItem(IDC_SETPART)->DestroyWindow();
	m_Set1.MoveWindow(rec);
	m_Set2.MoveWindow(rec);
	m_Set3.MoveWindow(rec);

	// ��ʼ���Ի���״̬
	m_Set1.ShowWindow(SW_HIDE);
	m_Set2.ShowWindow(SW_HIDE);
	m_Set3.ShowWindow(SW_HIDE);

	// �򿪶�Ӧģʽ�Ի���
	OpenModeDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CConfig::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// �����ò���д�������ļ�
	m_Set1.UpdateData();
	WritePrivateProfileString(_T("CommentSet"),_T("Pro"),m_Set1.ReturnPro(),_T(".\\Data\\Config.ini"));
	WritePrivateProfileString(_T("CommentSet"),_T("Key"),m_Set1.ReturnKey(),_T(".\\Data\\Config.ini"));
	WritePrivateProfileString(_T("CommentSet"),_T("LInterval"),m_Set1.ReturnListTime(),_T(".\\Data\\Config.ini"));
	WritePrivateProfileString(_T("CommentSet"),_T("SInterval"),m_Set1.ReturnShowTime(),_T(".\\Data\\Config.ini"));

	m_Set2.UpdateData();
	WritePrivateProfileString(_T("VoteSet"),_T("Max"),m_Set2.ReturnMax(),_T(".\\Data\\Config.ini"));
	WritePrivateProfileString(_T("VoteSet"),_T("Num"),m_Set2.ReturnNum(),_T(".\\Data\\Config.ini"));
	WritePrivateProfileString(_T("VoteSet"),_T("Item"),m_Set2.ReturnItem(),_T(".\\Data\\Config.ini"));
	WritePrivateProfileString(_T("VoteSet"),_T("LInterval"),m_Set2.ReturnListTime(),_T(".\\Data\\Config.ini"));

	m_Set3.UpdateData();
	WritePrivateProfileString(_T("LotterySet"),_T("Participant"),m_Set3.ReturnParticipant(),_T(".\\Data\\Config.ini"));
	WritePrivateProfileString(_T("LotterySet"),_T("Chance"),m_Set3.ReturnChance(),_T(".\\Data\\Config.ini"));

	OnOK();
}

void CConfig::Mode(CString str)
{
	m_Mode = str;
}

void CConfig::OpenModeDlg()
{
	if (m_Mode == "����")
	{
		m_Tree.SelectDropTarget(m_hSet1);
		m_Set1.ShowWindow(SW_SHOW);
		return;
	}
	if (m_Mode == "ͶƱ")
	{
		m_Tree.SelectDropTarget(m_hSet2);
		m_Set2.ShowWindow(SW_SHOW);
		return;
	}
	if (m_Mode == "�齱")
	{
		m_Tree.SelectDropTarget(m_hSet3);
		m_Set3.ShowWindow(SW_SHOW);
		return;
	}
}
