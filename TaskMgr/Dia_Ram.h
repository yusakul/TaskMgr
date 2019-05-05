#pragma once
#include "HistogramCtrl.h"

// CDia_Ram 对话框

class CDia_Ram : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Ram)

public:
	CDia_Ram(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia_Ram();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STATUS_RAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Edit_RamRate;
	CString m_Edit_UserRam;
	CString m_Edit_freeRam;
	CString m_Edit_totalRam;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//更新内存信息
	int UpdateMemroyInfo();

	//创建瀑布图对象
	CHistogramCtrl m_ctrlHistogram;
};
