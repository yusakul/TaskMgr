// MyTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskMgr.h"
#include "MyTabCtrl.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{

}

CMyTabCtrl::~CMyTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()



// CMyTabCtrl 消息处理程序




void CMyTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	switch (GetCurSel())
	{
	case 0:
	{
		m_Dia[0]->ShowWindow(SW_SHOW);
		m_Dia[1]->ShowWindow(SW_HIDE);
		m_Dia[2]->ShowWindow(SW_HIDE);
		m_Dia[3]->ShowWindow(SW_HIDE);
		m_Dia[4]->ShowWindow(SW_HIDE);
	}
	break;
	case 1:
	{
		m_Dia[0]->ShowWindow(SW_HIDE);
		m_Dia[1]->ShowWindow(SW_SHOW);
		m_Dia[2]->ShowWindow(SW_HIDE);
		m_Dia[3]->ShowWindow(SW_HIDE);
		m_Dia[4]->ShowWindow(SW_HIDE);
	}
	break;
	case 2:
	{
		m_Dia[0]->ShowWindow(SW_HIDE);
		m_Dia[1]->ShowWindow(SW_HIDE);
		m_Dia[2]->ShowWindow(SW_SHOW);
		m_Dia[3]->ShowWindow(SW_HIDE);
		m_Dia[4]->ShowWindow(SW_HIDE);
	}
	break;
	case 3:
	{
		m_Dia[0]->ShowWindow(SW_HIDE);
		m_Dia[1]->ShowWindow(SW_HIDE);
		m_Dia[2]->ShowWindow(SW_HIDE);
		m_Dia[3]->ShowWindow(SW_SHOW);
		m_Dia[4]->ShowWindow(SW_HIDE);
	}
	break;
	case 4:
	{
		m_Dia[0]->ShowWindow(SW_HIDE);
		m_Dia[1]->ShowWindow(SW_HIDE);
		m_Dia[2]->ShowWindow(SW_HIDE);
		m_Dia[3]->ShowWindow(SW_HIDE);
		m_Dia[4]->ShowWindow(SW_SHOW);
	}
	break;
	default:
		break;
	}
}