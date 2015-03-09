// MainFrm.h : CMainFrame ��Ľӿ�
//
#include "PIMNavigatorDlg.h"

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ����
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

// ����
public:
	static CString m_Control;	// ״̬�ִ�

private:
	int m_nTimer;					// ��ʱ����ʶ
	bool m_pRecord;					// ��¼��������־
	static CString s_Mode;			// ģʽ
	static HANDLE s_Handle_hThread;	// �����߳̾��
	static HANDLE s_Server_hThread;	// �����߳̾��
	static bool s_Stop;				// ֹͣ���
	static SOCKET sockConn;			// ����socket
	static SOCKET sockSrv;			// ����socket
	static bool s_Finish;			// ������ɱ��

	// �˵�
	//-> ����
	bool m_Begin;
	bool m_Pause;
	bool m_Stop;
	//-> �趨
	bool m_Comment;
	bool m_Vote;
	bool m_Lottery;	
	bool m_Setting;
  
	// �ؼ���Ƕ���Ա
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// ���ɵ���Ϣӳ�亯��
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// �趨
	afx_msg void UpdataComment(CCmdUI *pCmdUI);
	afx_msg void UpdataVote(CCmdUI *pCmdUI);
	afx_msg void UpdataLottery(CCmdUI *pCmdUI);
	afx_msg void UpdataConfig(CCmdUI *pCmdUI);
	afx_msg void OnComment();
	afx_msg void OnVote();
	afx_msg void OnLottery();
	afx_msg void OnConfig();
	// ����
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
	// ����
	afx_msg void OnDocument();
public:
	afx_msg void OnClose();
};