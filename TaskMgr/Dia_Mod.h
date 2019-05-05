#pragma once
#include "afxcmn.h"



// CDia_Mod 对话框

class CDia_Mod : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Mod)

public:
	CDia_Mod(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia_Mod();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODULE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:

	void UpdataList();
	BOOL ListProcessModules(DWORD dwPID);
	CListCtrl m_List_Mod;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
