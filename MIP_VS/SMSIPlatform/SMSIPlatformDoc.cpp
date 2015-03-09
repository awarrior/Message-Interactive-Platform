// SMSIPlatformDoc.cpp : CSMSIPlatformDoc 类的实现
//

#include "stdafx.h"
#include "SMSIPlatform.h"

#include "SMSIPlatformDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSMSIPlatformDoc

IMPLEMENT_DYNCREATE(CSMSIPlatformDoc, CDocument)

BEGIN_MESSAGE_MAP(CSMSIPlatformDoc, CDocument)
END_MESSAGE_MAP()


// CSMSIPlatformDoc 构造/析构

CSMSIPlatformDoc::CSMSIPlatformDoc()
{
	// TODO: 在此添加一次性构造代码

}

CSMSIPlatformDoc::~CSMSIPlatformDoc()
{
}

BOOL CSMSIPlatformDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CSMSIPlatformDoc 序列化

void CSMSIPlatformDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CSMSIPlatformDoc 诊断

#ifdef _DEBUG
void CSMSIPlatformDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSMSIPlatformDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSMSIPlatformDoc 命令
