#pragma once


// CSetting3 �Ի���

class CSetting3 : public CDialog
{
	DECLARE_DYNAMIC(CSetting3)

public:
	CSetting3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetting3();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CString m_Participant;	// ������
	CString m_Chance;		// �������
public:
	// ����ֵ
	CString ReturnParticipant();
	CString ReturnChance();
protected:
	virtual void OnOK();
};
