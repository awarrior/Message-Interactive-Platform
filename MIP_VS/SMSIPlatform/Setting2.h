#pragma once


// CSetting2 �Ի���

class CSetting2 : public CDialog
{
	DECLARE_DYNAMIC(CSetting2)

public:
	CSetting2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetting2();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CString m_Max;		// Ʊ�����ֵ
	CString m_Num;		// ����
	CString m_Item;		// ��Ŀ����
	CSliderCtrl m_ListTime;	// �б�ˢ�¼��
	CString m_nListTime;	// �б�ˢ�¼������
public:
	// ����ֵ
	CString ReturnMax();
	CString ReturnNum();
	CString ReturnItem();
	CString ReturnListTime();
protected:
	virtual void OnOK();
public:
	afx_msg void OnEnKillfocusEditL2();
	afx_msg void OnSliderL2(NMHDR *pNMHDR, LRESULT *pResult);
};
