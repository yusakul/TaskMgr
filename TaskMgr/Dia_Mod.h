#pragma once
#include "afxcmn.h"



// CDia_Mod �Ի���

class CDia_Mod : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Mod)

public:
	CDia_Mod(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDia_Mod();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODULE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:

	void UpdataList();
	BOOL ListProcessModules(DWORD dwPID);
	CListCtrl m_List_Mod;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
