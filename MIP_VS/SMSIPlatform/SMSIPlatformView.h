// SMSIPlatformView.h : CSMSIPlatformView 类的接口
//


#pragma once
#include "afxcmn.h"


class CSMSIPlatformView : public CView
{
protected: // 仅从序列化创建
	CSMSIPlatformView();
	DECLARE_DYNCREATE(CSMSIPlatformView)

// 属性
public:
	CSMSIPlatformDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CSMSIPlatformView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	virtual BOOL DestroyWindow();
	afx_msg void OnPaint();
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // SMSIPlatformView.cpp 中的调试版本
inline CSMSIPlatformDoc* CSMSIPlatformView::GetDocument() const
   { return reinterpret_cast<CSMSIPlatformDoc*>(m_pDocument); }
#endif

