#pragma once


// CDblClick �Ի���

class CDblClick : public CDialog
{
	DECLARE_DYNAMIC(CDblClick)

public:
	CDblClick(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDblClick();

// �Ի�������
	enum { IDD = IDD_DBLCLICK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:
	CString m_Number;	// �ֻ�����
	CString m_Time;		// ʱ��
	CString m_Text;		// ��������
	CButton m_Del;		// ɾ���ؼ�
	CButton m_hLight;	// �����ؼ�
	int m_Index;		// ��¼����ֵ

public:
	void TransData(CString num, CString time, CString text, int index);

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancelHlight();
	afx_msg void OnBnClickedOkHlight();
};
