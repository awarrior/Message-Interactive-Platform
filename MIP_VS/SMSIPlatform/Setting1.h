#pragma once


// CSetting1 �Ի���

class CSetting1 : public CDialog
{
	DECLARE_DYNAMIC(CSetting1)

public:
	CSetting1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetting1();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_Pro;			// ����ǰ׺
	CString m_Key;			// �ؼ���
	CSliderCtrl m_ListTime;	// �б�ˢ�¼��
	CSliderCtrl m_ShowTime;	// ��ʾˢ�¼��
	CString m_nListTime;	// �б�ˢ�¼������
	CString m_nShowTime;	// ��ʾˢ�¼������
public:
	// ����ֵ
	CString ReturnPro();
	CString ReturnKey();
	CString ReturnListTime();
	CString ReturnShowTime();
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSliderL1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSliderS(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditL1();
	afx_msg void OnEnKillfocusEditS();
};
