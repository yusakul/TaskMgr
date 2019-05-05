#pragma once
#include "afxcmn.h"


// CDia_Heap 对话框

class CDia_Heap : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Heap)

public:
	CDia_Heap(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia_Heap();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HEAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List_Heap;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//遍历堆
	BOOL ListProcessHeap(DWORD dwOwnerPID);

};
