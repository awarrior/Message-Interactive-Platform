#pragma once


// CDblClick 对话框

class CDblClick : public CDialog
{
	DECLARE_DYNAMIC(CDblClick)

public:
	CDblClick(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDblClick();

// 对话框数据
	enum { IDD = IDD_DBLCLICK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	CString m_Number;	// 手机号码
	CString m_Time;		// 时间
	CString m_Text;		// 短信内容
	CButton m_Del;		// 删除控件
	CButton m_hLight;	// 高亮控件
	int m_Index;		// 记录索引值

public:
	void TransData(CString num, CString time, CString text, int index);

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancelHlight();
	afx_msg void OnBnClickedOkHlight();
};
