#pragma once


// CSetting1 对话框

class CSetting1 : public CDialog
{
	DECLARE_DYNAMIC(CSetting1)

public:
	CSetting1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetting1();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_Pro;			// 短信前缀
	CString m_Key;			// 关键字
	CSliderCtrl m_ListTime;	// 列表刷新间隔
	CSliderCtrl m_ShowTime;	// 演示刷新间隔
	CString m_nListTime;	// 列表刷新间隔数字
	CString m_nShowTime;	// 演示刷新间隔数字
public:
	// 返回值
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
