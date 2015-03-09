#pragma once


// CSetting3 对话框

class CSetting3 : public CDialog
{
	DECLARE_DYNAMIC(CSetting3)

public:
	CSetting3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetting3();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CString m_Participant;	// 参与人
	CString m_Chance;		// 机会控制
public:
	// 返回值
	CString ReturnParticipant();
	CString ReturnChance();
protected:
	virtual void OnOK();
};
