#pragma once
#include "afxcmn.h"


// CDia_Heap �Ի���

class CDia_Heap : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Heap)

public:
	CDia_Heap(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDia_Heap();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HEAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List_Heap;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//������
	BOOL ListProcessHeap(DWORD dwOwnerPID);

};
