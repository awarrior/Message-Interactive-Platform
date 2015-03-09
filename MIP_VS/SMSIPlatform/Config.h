#pragma once
#include "Setting1.h"
#include "Setting2.h"
#include "Setting3.h"

// CConfig 对话框

class CConfig : public CDialog
{
	DECLARE_DYNAMIC(CConfig)

public:
	CConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConfig();

// 对话框数据
	enum { IDD = IDD_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	CSetting1 m_Set1;	// 评论设置
	CSetting2 m_Set2;	// 投票设置
	CSetting3 m_Set3;	// 抽奖设置
	CTreeCtrl m_Tree;	// 树控件
	HTREEITEM m_hSet1;	// 评论设置句柄
	HTREEITEM m_hSet2;	// 投票设置句柄
	HTREEITEM m_hSet3;	// 抽奖设置句柄
	CString m_Mode;		// 模式
	
	void OpenModeDlg();	// 打开对应模式对话框
public:
	void Mode(CString str);	// 返回模式

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
