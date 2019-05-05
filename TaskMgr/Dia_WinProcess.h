#pragma once
#include "afxcmn.h"
#include <vector>

// CDia_WinProc 对话框

typedef struct _MyWinProcStruct
{
	TCHAR lpWinTitle[MAX_PATH];
	TCHAR lpPaths[MAX_PATH];
	DWORD hPID;
	HICON hIcon;
	DWORD ListID;
	HANDLE hWnd;
}MyWinProcStruct, *PMyWinProcStruct;

class CDia_WinProc : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_WinProc)

public:
	CDia_WinProc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia_WinProc();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WINPROCESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl_winproc;	
	CImageList* pImageList = new CImageList;

	//刷新list
	void UpdataList();
	std::vector<MyWinProcStruct> m_WinProcVec;
	//void InitList();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnNMRClickListWinproc(NMHDR *pNMHDR, LRESULT *pResult);
};
