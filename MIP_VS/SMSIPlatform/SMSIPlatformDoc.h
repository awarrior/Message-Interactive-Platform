// SMSIPlatformDoc.h : CSMSIPlatformDoc ��Ľӿ�
//


#pragma once


class CSMSIPlatformDoc : public CDocument
{
protected: // �������л�����
	CSMSIPlatformDoc();
	DECLARE_DYNCREATE(CSMSIPlatformDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CSMSIPlatformDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


