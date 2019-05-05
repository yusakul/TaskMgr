
// TaskMgrDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "MyTabCtrl.h"
#include "FileClean.h"

/*afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);*/

// CTaskMgrDlg �Ի���
class CTaskMgrDlg : public CDialogEx
{
	// ����
public:
	CTaskMgrDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASKMGR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMyTabCtrl m_MyTab;




	afx_msg void OnMenu_Exit();
	afx_msg void OnMenu_LogOut();

	//��Ȩ����
	BOOL AdjustPrivilege();
	afx_msg void OnMenu_ShutDown();
	afx_msg void OnMenu_Reboot();
	afx_msg void OnMenu_FileClean();

	

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	

};






