// SMSIPlatformDoc.cpp : CSMSIPlatformDoc ���ʵ��
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


// CSMSIPlatformDoc ����/����

CSMSIPlatformDoc::CSMSIPlatformDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CSMSIPlatformDoc::~CSMSIPlatformDoc()
{
}

BOOL CSMSIPlatformDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CSMSIPlatformDoc ���л�

void CSMSIPlatformDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CSMSIPlatformDoc ���

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


// CSMSIPlatformDoc ����
