#pragma once
#include "afxcmn.h"
#include <vector>

// CDia_WinProc �Ի���

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
	CDia_WinProc(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDia_WinProc();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WINPROCESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl_winproc;	
	CImageList* pImageList = new CImageList;

	//ˢ��list
	void UpdataList();
	std::vector<MyWinProcStruct> m_WinProcVec;
	//void InitList();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnNMRClickListWinproc(NMHDR *pNMHDR, LRESULT *pResult);
};
