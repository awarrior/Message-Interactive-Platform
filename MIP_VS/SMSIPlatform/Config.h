#pragma once
#include "Setting1.h"
#include "Setting2.h"
#include "Setting3.h"

// CConfig �Ի���

class CConfig : public CDialog
{
	DECLARE_DYNAMIC(CConfig)

public:
	CConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConfig();

// �Ի�������
	enum { IDD = IDD_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:
	CSetting1 m_Set1;	// ��������
	CSetting2 m_Set2;	// ͶƱ����
	CSetting3 m_Set3;	// �齱����
	CTreeCtrl m_Tree;	// ���ؼ�
	HTREEITEM m_hSet1;	// �������þ��
	HTREEITEM m_hSet2;	// ͶƱ���þ��
	HTREEITEM m_hSet3;	// �齱���þ��
	CString m_Mode;		// ģʽ
	
	void OpenModeDlg();	// �򿪶�Ӧģʽ�Ի���
public:
	void Mode(CString str);	// ����ģʽ

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
