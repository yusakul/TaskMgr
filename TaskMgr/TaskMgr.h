
// TaskMgr.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTaskMgrApp: 
// �йش����ʵ�֣������ TaskMgr.cpp
//

class CTaskMgrApp : public CWinApp
{
public:
	CTaskMgrApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

public:
	//������Ϣ
	
};

void win32pError();
extern CTaskMgrApp theApp;