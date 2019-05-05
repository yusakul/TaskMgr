#pragma once
#include "Data.h"
#include "afxcmn.h"

// CDia_Thread 对话框

class CDia_Thread : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Thread)

public:
	CDia_Thread(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia_Thread();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_THREAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

	//更新列表
	void UpdataList();

	//遍历线程
	BOOL ListProcessThreads(DWORD dwOwnerPID);

	CListCtrl m_List_Thread;

	//右键list呼出菜单
	afx_msg void OnRclickListThread(NMHDR *pNMHDR, LRESULT *pResult);
};
