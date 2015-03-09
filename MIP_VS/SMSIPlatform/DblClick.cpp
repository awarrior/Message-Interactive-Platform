// DblClick.cpp : 实现文件
//

#include "stdafx.h"
#include "SMSIPlatform.h"
#include "DblClick.h"
#include "ColorListCtrl.h"

// 全局变量
extern CColorListCtrl g_List;
extern _RecordsetPtr g_pRecordset1;
extern SOCKET g_Socket;

// CDblClick 对话框

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


// CDblClick 消息处理程序
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

	// TODO:  在此添加额外的初始化
	// 默认设定删除
	m_Del.SetCheck(1);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDblClick::OnBnClickedOkHlight()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Del.GetCheck())
	{
		try
		{
			// 删除数据表项
			if(!g_pRecordset1->BOF)
			{
				g_pRecordset1->MoveFirst();
			}
			_variant_t var;
			CString stra;
			CString strb;
			// 循环查找
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
				// 找到被删除项
				g_pRecordset1->PutCollect(_T("del_flag"), _variant_t(_T("True")));
				g_pRecordset1->Update();
				break;
			}
			if (g_pRecordset1->adoEOF)
			{
				AfxMessageBox(_T("数据表找不到匹配项!\n无法完成删除操作!"));
			}
		}
		catch(...)
		{
			AfxMessageBox(_T("数据表查找错误!"));
		}
		// 删除列表项
		g_List.DeleteItem(m_Index);
		// 关闭对话框
		OnOK();
	}
	if (m_hLight.GetCheck())
	{
		GetDlgItem(IDC_OK_HLIGHT)->EnableWindow(FALSE);
		m_Del.ModifyStyle(0, WS_DISABLED);
		// 发送信息到flash
		// 读取信息
		CString phonum = "";
		CString message = "";
		GetDlgItemText(IDC_NUMBER, phonum);
		GetDlgItemText(IDC_TEXT, message);

		// 信息整合
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

		// 类型转换 CString to char*
		int nSize = msg.GetLength();
		char *pString = new char[2*nSize];	
		WideCharToMultiByte(CP_UTF8, 0, msg, -1, pString, 2*nSize, NULL, NULL);

		// 发送数据
		if(send(g_Socket,pString,strlen(pString),0) == SOCKET_ERROR)   
		{   
			// 发送数据失败
			delete pString; 
		}
		delete pString;
	}
}

void CDblClick::OnBnClickedCancelHlight()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_hLight.GetCheck())
	{
		// 发送信息到flash
		// 读取信息
		CString phonum = "";
		CString message = "";
		GetDlgItemText(IDC_NUMBER, phonum);
		GetDlgItemText(IDC_TEXT, message);

		// 发送数据
		char *pString = "cc";
		if(send(g_Socket,pString,strlen(pString),0) == SOCKET_ERROR)   
		{   
			// 发送数据失败
		}
		OnOK();
	}
	OnOK();
}