#pragma once
#include "afxcmn.h"
#include "TaskMgr.h"
#include "Dia_Mod.h"
#include "Dia_Thread.h"
#include "Data.h"
#include "Dia_Heap.h"

// CDia_Details 对话框


class CDia_Details : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Details)

public:
	CDia_Details(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia_Details();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DETAILS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl_De;
	CImageList* pImageList;
	CDia_Mod m_Mod;
	CDia_Thread m_Thread;
	CDia_Heap m_Heap;
	void UpdataList();

	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLvnItemchangedListWinproc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListDetails(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickListDetails(NMHDR *pNMHDR, LRESULT *pResult);

	
};
