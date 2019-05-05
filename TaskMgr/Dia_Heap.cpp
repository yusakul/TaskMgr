// Dia_Heap.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskMgr.h"
#include "Dia_Heap.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// CDia_Heap �Ի���

IMPLEMENT_DYNAMIC(CDia_Heap, CDialogEx)

CDia_Heap::CDia_Heap(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_HEAP, pParent)
{

}

CDia_Heap::~CDia_Heap()
{
}

void CDia_Heap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Heap, m_List_Heap);
}


BEGIN_MESSAGE_MAP(CDia_Heap, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDia_Heap ��Ϣ�������


BOOL CDia_Heap::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_List_Heap.SetExtendedStyle(m_List_Heap.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
	m_List_Heap.InsertColumn(0, _T("��ID"), 0, 200);
	m_List_Heap.InsertColumn(1, _T("���С"), 0, 100);
	m_List_Heap.InsertColumn(2, _T("�Ѵ�С"), 0, 200);




	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDia_Heap::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DestroyWindow();
	CDialogEx::OnClose();
}

BOOL CDia_Heap::ListProcessHeap(DWORD dwOwnerPID)
{

	HEAPLIST32 hl;

	HANDLE hHeapSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, dwOwnerPID);

	hl.dwSize = sizeof(HEAPLIST32);

	if (hHeapSnap == INVALID_HANDLE_VALUE)
	{
		win32pError();
		return FALSE;
	}

	if (Heap32ListFirst(hHeapSnap, &hl))
	{
		do
		{
			int i = 0;
			TCHAR temp[MAX_PATH] = {};

			HEAPENTRY32 he;
			ZeroMemory(&he, sizeof(HEAPENTRY32));
			he.dwSize = sizeof(HEAPENTRY32);

			if (Heap32First(&he, dwOwnerPID, hl.th32HeapID))
			{
				//��ID
				wsprintf(temp, L"%d", hl.th32HeapID);
				m_List_Heap.InsertItem(i, temp);
				do
				{
					//���С
					wsprintf(temp, L"%d", he.dwBlockSize/1024);
					wcscat_s(temp, MAX_PATH, L"K");
					m_List_Heap.SetItemText(i, 1, temp);
					
					//�Ѵ�С
					he.dwSize = sizeof(HEAPENTRY32);
					wsprintf(temp, L"%d", he.dwSize);
					m_List_Heap.SetItemText(i, 2, temp);

				} while (Heap32Next(&he));
			}
			hl.dwSize = sizeof(HEAPLIST32);

			i++;
			if (i > 500)break;
		} while (Heap32ListNext(hHeapSnap, &hl));
	}
	else
	{
		win32pError();
		CloseHandle(hHeapSnap);
		return FALSE;
	}
	CloseHandle(hHeapSnap);
	return TRUE;
}
