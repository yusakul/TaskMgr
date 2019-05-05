#pragma once

#include "Dia_Details.h"
#include "Dia_WinProcess.h"
#include "Dia_Status.h"
#include "Dia_Ram.h"
#include "Dia_Disk.h"

// CMyTabCtrl

class CMyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CDialogEx* m_Dia[5];

};


