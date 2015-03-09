// Config.cpp : 实现文件
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "Config.h"


// CConfig 对话框

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


// CConfig 消息处理程序

void CConfig::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	// 树控件响应对话框
	HTREEITEM hitem;   
	hitem = m_Tree.GetSelectedItem();
	CString str = m_Tree.GetItemText(hitem);
	if (!str.Compare(_T("评论")))
	{
		m_Set1.ShowWindow(SW_SHOW);
		m_Set2.ShowWindow(SW_HIDE);
		m_Set3.ShowWindow(SW_HIDE);
		m_Tree.SelectDropTarget(m_hSet1);
	}
	else if (!str.Compare(_T("投票")))
	{
		m_Set1.ShowWindow(SW_HIDE);
		m_Set2.ShowWindow(SW_SHOW);
		m_Set3.ShowWindow(SW_HIDE);
		m_Tree.SelectDropTarget(m_hSet2);
	}
	else if (!str.Compare(_T("抽奖")))
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

	// TODO:  在此添加额外的初始化
	// 初始化树控件
	m_Tree.ModifyStyle(0,TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES);
	m_Tree.DeleteAllItems();
	HTREEITEM hRoot=m_Tree.InsertItem(_T("配置"));
	m_hSet1 = m_Tree.InsertItem(_T("评论"),hRoot);
	m_hSet2 = m_Tree.InsertItem(_T("投票"),hRoot);
	m_hSet3 = m_Tree.InsertItem(_T("抽奖"),hRoot);
	m_Tree.Expand(hRoot,TVE_EXPAND);

	// 创建3个设置对话框
	m_Set1.Create(IDD_DIALOG1, this);
	m_Set2.Create(IDD_DIALOG2, this);
	m_Set3.Create(IDD_DIALOG3, this);

	// 放置对话框到指定位置
	CRect rec;
	GetDlgItem(IDC_SETPART)->GetWindowRect(rec);
	ScreenToClient(rec);
	GetDlgItem(IDC_SETPART)->DestroyWindow();
	m_Set1.MoveWindow(rec);
	m_Set2.MoveWindow(rec);
	m_Set3.MoveWindow(rec);

	// 初始化对话框状态
	m_Set1.ShowWindow(SW_HIDE);
	m_Set2.ShowWindow(SW_HIDE);
	m_Set3.ShowWindow(SW_HIDE);

	// 打开对应模式对话框
	OpenModeDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CConfig::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// 将设置参数写入配置文件
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
	if (m_Mode == "评论")
	{
		m_Tree.SelectDropTarget(m_hSet1);
		m_Set1.ShowWindow(SW_SHOW);
		return;
	}
	if (m_Mode == "投票")
	{
		m_Tree.SelectDropTarget(m_hSet2);
		m_Set2.ShowWindow(SW_SHOW);
		return;
	}
	if (m_Mode == "抽奖")
	{
		m_Tree.SelectDropTarget(m_hSet3);
		m_Set3.ShowWindow(SW_SHOW);
		return;
	}
}
