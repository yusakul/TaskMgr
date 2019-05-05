
// TaskMgrDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "MyTabCtrl.h"
#include "FileClean.h"

/*afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);*/

// CTaskMgrDlg 对话框
class CTaskMgrDlg : public CDialogEx
{
	// 构造
public:
	CTaskMgrDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASKMGR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMyTabCtrl m_MyTab;




	afx_msg void OnMenu_Exit();
	afx_msg void OnMenu_LogOut();

	//提权函数
	BOOL AdjustPrivilege();
	afx_msg void OnMenu_ShutDown();
	afx_msg void OnMenu_Reboot();
	afx_msg void OnMenu_FileClean();

	

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	

};






