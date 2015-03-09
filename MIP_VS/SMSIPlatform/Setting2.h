#pragma once


// CSetting2 对话框

class CSetting2 : public CDialog
{
	DECLARE_DYNAMIC(CSetting2)

public:
	CSetting2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetting2();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CString m_Max;		// 票数最大值
	CString m_Num;		// 项数
	CString m_Item;		// 项目内容
	CSliderCtrl m_ListTime;	// 列表刷新间隔
	CString m_nListTime;	// 列表刷新间隔数字
public:
	// 返回值
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
