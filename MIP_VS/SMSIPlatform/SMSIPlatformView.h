// SMSIPlatformView.h : CSMSIPlatformView ��Ľӿ�
//


#pragma once
#include "afxcmn.h"


class CSMSIPlatformView : public CView
{
protected: // �������л�����
	CSMSIPlatformView();
	DECLARE_DYNCREATE(CSMSIPlatformView)

// ����
public:
	CSMSIPlatformDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CSMSIPlatformView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	virtual BOOL DestroyWindow();
	afx_msg void OnPaint();
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // SMSIPlatformView.cpp �еĵ��԰汾
inline CSMSIPlatformDoc* CSMSIPlatformView::GetDocument() const
   { return reinterpret_cast<CSMSIPlatformDoc*>(m_pDocument); }
#endif

