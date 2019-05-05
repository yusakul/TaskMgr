#pragma once
#include "Data.h"
#include "afxcmn.h"

// CDia_Thread �Ի���

class CDia_Thread : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Thread)

public:
	CDia_Thread(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDia_Thread();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_THREAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

	//�����б�
	void UpdataList();

	//�����߳�
	BOOL ListProcessThreads(DWORD dwOwnerPID);

	CListCtrl m_List_Thread;

	//�Ҽ�list�����˵�
	afx_msg void OnRclickListThread(NMHDR *pNMHDR, LRESULT *pResult);
};
