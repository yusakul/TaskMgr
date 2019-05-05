#pragma once
#include "HistogramCtrl.h"

// CDia_Ram �Ի���

class CDia_Ram : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Ram)

public:
	CDia_Ram(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDia_Ram();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STATUS_RAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_Edit_RamRate;
	CString m_Edit_UserRam;
	CString m_Edit_freeRam;
	CString m_Edit_totalRam;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//�����ڴ���Ϣ
	int UpdateMemroyInfo();

	//�����ٲ�ͼ����
	CHistogramCtrl m_ctrlHistogram;
};
