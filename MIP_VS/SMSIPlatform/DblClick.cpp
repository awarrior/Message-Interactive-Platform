// DblClick.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "DblClick.h"
#include "ColorListCtrl.h"

// ȫ�ֱ���
extern CColorListCtrl g_List;
extern _RecordsetPtr g_pRecordset1;
extern SOCKET g_Socket;

// CDblClick �Ի���

IMPLEMENT_DYNAMIC(CDblClick, CDialog)

CDblClick::CDblClick(CWnd* pParent /*=NULL*/)
	: CDialog(CDblClick::IDD, pParent)
{

}

CDblClick::~CDblClick()
{
}

void CDblClick::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NUMBER, m_Number);
	DDX_Text(pDX, IDC_TIME, m_Time);
	DDX_Text(pDX, IDC_TEXT, m_Text);
	DDX_Control(pDX, IDC_DEL, m_Del);
	DDX_Control(pDX, IDC_HLight, m_hLight);
}


BEGIN_MESSAGE_MAP(CDblClick, CDialog)
	ON_BN_CLICKED(IDC_CANCEL_HLIGHT, &CDblClick::OnBnClickedCancelHlight)
	ON_BN_CLICKED(IDC_OK_HLIGHT, &CDblClick::OnBnClickedOkHlight)
END_MESSAGE_MAP()


// CDblClick ��Ϣ�������
void CDblClick::TransData(CString num, CString time, CString text, int index)
{
	m_Number = num;
	m_Time = time;
	m_Text = text;
	m_Index = index;
}
BOOL CDblClick::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// Ĭ���趨ɾ��
	m_Del.SetCheck(1);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDblClick::OnBnClickedOkHlight()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_Del.GetCheck())
	{
		try
		{
			// ɾ�����ݱ���
			if(!g_pRecordset1->BOF)
			{
				g_pRecordset1->MoveFirst();
			}
			_variant_t var;
			CString stra;
			CString strb;
			// ѭ������
			for(; !g_pRecordset1->adoEOF; g_pRecordset1->MoveNext())
			{
				// time
				var = g_pRecordset1->GetCollect(("time"));
				if(var.vt != VT_NULL)
					stra = (LPCSTR)_bstr_t(var);
				strb = g_List.GetItemText(m_Index, 1);
				if (stra != strb)
					continue;
				// number
				var = g_pRecordset1->GetCollect(("number"));
				if(var.vt != VT_NULL)
					stra = (LPCSTR)_bstr_t(var);
				strb = g_List.GetItemText(m_Index, 2);
				if (stra != strb)
					continue;
				// text
				var = g_pRecordset1->GetCollect(("text"));
				if(var.vt != VT_NULL)
					stra = (LPCSTR)_bstr_t(var);
				strb = g_List.GetItemText(m_Index, 3);
				if (stra != strb)
					continue;
				// �ҵ���ɾ����
				g_pRecordset1->PutCollect(_T("del_flag"), _variant_t(_T("True")));
				g_pRecordset1->Update();
				break;
			}
			if (g_pRecordset1->adoEOF)
			{
				AfxMessageBox(_T("���ݱ��Ҳ���ƥ����!\n�޷����ɾ������!"));
			}
		}
		catch(...)
		{
			AfxMessageBox(_T("���ݱ���Ҵ���!"));
		}
		// ɾ���б���
		g_List.DeleteItem(m_Index);
		// �رնԻ���
		OnOK();
	}
	if (m_hLight.GetCheck())
	{
		GetDlgItem(IDC_OK_HLIGHT)->EnableWindow(FALSE);
		m_Del.ModifyStyle(0, WS_DISABLED);
		// ������Ϣ��flash
		// ��ȡ��Ϣ
		CString phonum = "";
		CString message = "";
		GetDlgItemText(IDC_NUMBER, phonum);
		GetDlgItemText(IDC_TEXT, message);

		// ��Ϣ����
		int len = phonum.GetLength();
		while (len>11)
		{
			phonum.Delete(0);
			len--;
		}
		if (len>7 && len<=11)
		{
			int mlen = len - 7;
			for (int i=0; i<mlen; i++)
			{
				phonum.SetAt(3+i,'*');
			}
		}
		CString msg = _T("cl")+phonum+_T("+")+message;

		// ����ת�� CString to char*
		int nSize = msg.GetLength();
		char *pString = new char[2*nSize];	
		WideCharToMultiByte(CP_UTF8, 0, msg, -1, pString, 2*nSize, NULL, NULL);

		// ��������
		if(send(g_Socket,pString,strlen(pString),0) == SOCKET_ERROR)   
		{   
			// ��������ʧ��
			delete pString; 
		}
		delete pString;
	}
}

void CDblClick::OnBnClickedCancelHlight()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_hLight.GetCheck())
	{
		// ������Ϣ��flash
		// ��ȡ��Ϣ
		CString phonum = "";
		CString message = "";
		GetDlgItemText(IDC_NUMBER, phonum);
		GetDlgItemText(IDC_TEXT, message);

		// ��������
		char *pString = "cc";
		if(send(g_Socket,pString,strlen(pString),0) == SOCKET_ERROR)   
		{   
			// ��������ʧ��
		}
		OnOK();
	}
	OnOK();
}