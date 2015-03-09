// MainFrm.h : CMainFrame 类的接口
//
#include "PIMNavigatorDlg.h"

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 函数
private:
	void Configuration();
	void State();
	void OpenRecord1();
	void OpenRecord2();
	void CloseRecord1();
	void CloseRecord2();
	void CloseRecord1r();
	void CloseRecord2r();
	void CloseRecord();
	void pRecordsetCreate();
	void DelComment();
	void DelVote();
	static DWORD WINAPI HandleFunProc(LPVOID lpParameter);
	static DWORD WINAPI ServerFunProc(LPVOID lpParameter);
	static void CFunProc(bool &flag);
	static void VFunProc(bool &flag);
	static void LFunProc(bool &flag);
	static void IndexSave();
	static CString RandomGet();
	static void aSleep(int time);
public:
	static CString Mode();

// 变量
public:
	static CString m_Control;	// 状态字串

private:
	int m_nTimer;					// 定时器标识
	bool m_pRecord;					// 记录集创建标志
	static CString s_Mode;			// 模式
	static HANDLE s_Handle_hThread;	// 处理线程句柄
	static HANDLE s_Server_hThread;	// 服务线程句柄
	static bool s_Stop;				// 停止标记
	static SOCKET sockConn;			// 连接socket
	static SOCKET sockSrv;			// 服务socket
	static bool s_Finish;			// 服务完成标记

	// 菜单
	//-> 控制
	bool m_Begin;
	bool m_Pause;
	bool m_Stop;
	//-> 设定
	bool m_Comment;
	bool m_Vote;
	bool m_Lottery;	
	bool m_Setting;
  
	// 控件条嵌入成员
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// 生成的消息映射函数
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 设定
	afx_msg void UpdataComment(CCmdUI *pCmdUI);
	afx_msg void UpdataVote(CCmdUI *pCmdUI);
	afx_msg void UpdataLottery(CCmdUI *pCmdUI);
	afx_msg void UpdataConfig(CCmdUI *pCmdUI);
	afx_msg void OnComment();
	afx_msg void OnVote();
	afx_msg void OnLottery();
	afx_msg void OnConfig();
	// 控制
	afx_msg void UpdataBegin(CCmdUI *pCmdUI);
	afx_msg void UpdataPause(CCmdUI *pCmdUI);
	afx_msg void UpdataStop(CCmdUI *pCmdUI);
	afx_msg void OnBegin();
	afx_msg void OnPause();
	afx_msg void OnStop();
	afx_msg void OnClear();
	afx_msg void OnDelAll();
	afx_msg void OnDelComment();
	afx_msg void OnDelVote();
	// 帮助
	afx_msg void OnDocument();
public:
	afx_msg void OnClose();
};