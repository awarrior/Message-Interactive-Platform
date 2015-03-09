#pragma once


// CWaitForConnect 对话框

class CWaitForConnect : public CDialog
{
	DECLARE_DYNAMIC(CWaitForConnect)

public:
	CWaitForConnect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWaitForConnect();
	void ParamInit(SOCKET* sockSrv, SOCKADDR_IN* addrClient, int* len);
	SOCKET ReturnConn();

// 对话框数据
	enum { IDD = IDD_CONNECT };

private:
	UINT m_nTimer;
	UINT m_nTimer2;
	HANDLE m_hThread;
	bool m_flag;
	
	static SOCKET m_sockConn;
	static bool m_ok;
	static SOCKET* m_sockSrv;
	static SOCKADDR_IN* m_addrClient;   
	static int* m_len;
	static DWORD WINAPI Proc(LPVOID lpParameter);
	static void UpdateSockConn();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
