
// TaskMgrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskMgr.h"
#include "TaskMgrDlg.h"
#include "afxdialogex.h"
#include <WinUser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//�����ļ��̻߳ص�����
DWORD WINAPI ThreadProc_FileClean(LPVOID lpParam);
bool CreateChildThread_FileClean();



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	/*virtual BOOL OnInitDialog();*/
	afx_msg void OnClose();
//	afx_msg void OnDestroy();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CLOSE()
//	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTaskMgrDlg �Ի���



CTaskMgrDlg::CTaskMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TASKMGR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTaskMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_MyTab);
}

BEGIN_MESSAGE_MAP(CTaskMgrDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32783, &CTaskMgrDlg::OnMenu_Exit)
	ON_COMMAND(ID_Menu, &CTaskMgrDlg::OnMenu_LogOut)
	ON_COMMAND(ID_32784, &CTaskMgrDlg::OnMenu_ShutDown)
	ON_COMMAND(ID_32785, &CTaskMgrDlg::OnMenu_Reboot)
	ON_COMMAND(ID_32786, &CTaskMgrDlg::OnMenu_FileClean)

END_MESSAGE_MAP()


// CTaskMgrDlg ��Ϣ�������

BOOL CTaskMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������


	m_MyTab.InsertItem(0, L"���ڽ���");
	m_MyTab.InsertItem(1, L"��ϸ��Ϣ");
	m_MyTab.InsertItem(2, L"CPU");
	m_MyTab.InsertItem(3, L"RAM");
	m_MyTab.InsertItem(4, L"Disk");

	//���Ӵ���ָ�븳ֵ
	m_MyTab.m_Dia[0] = new CDia_WinProc;
	m_MyTab.m_Dia[1] = new CDia_Details;
	m_MyTab.m_Dia[2] = new CDia_Status;
	m_MyTab.m_Dia[3] = new CDia_Ram;
	m_MyTab.m_Dia[4] = new CDia_Disk;

	//�����Ӵ���
	m_MyTab.m_Dia[0]->Create(IDD_DIALOG_WINPROCESS, &m_MyTab);
	m_MyTab.m_Dia[1]->Create(IDD_DIALOG_DETAILS, &m_MyTab);
	m_MyTab.m_Dia[2]->Create(IDD_DIALOG_STATUS, &m_MyTab);
	m_MyTab.m_Dia[3]->Create(IDD_DIALOG_STATUS_RAM, &m_MyTab);
	m_MyTab.m_Dia[4]->Create(IDD_DIALOG_DISK, &m_MyTab);

	//���������Ӵ��ڴ�С
	CRect rc;
	m_MyTab.GetClientRect(rc);
	rc.DeflateRect(2, 25, 2, 2);
	m_MyTab.m_Dia[0]->MoveWindow(rc);
	m_MyTab.m_Dia[1]->MoveWindow(rc);
	m_MyTab.m_Dia[2]->MoveWindow(rc);
	m_MyTab.m_Dia[3]->MoveWindow(rc);
	m_MyTab.m_Dia[4]->MoveWindow(rc);


	//��ʾ��һ���Ӵ���
	m_MyTab.m_Dia[0]->ShowWindow(SW_SHOW);
	m_MyTab.m_Dia[1]->ShowWindow(SW_HIDE);
	m_MyTab.m_Dia[2]->ShowWindow(SW_HIDE);
	m_MyTab.m_Dia[3]->ShowWindow(SW_HIDE);
	m_MyTab.m_Dia[4]->ShowWindow(SW_HIDE);


	//�˵�
	CMenu m_menu;
	m_menu.LoadMenu(IDR_MENU_MainWindow);
	SetMenu(&m_menu);

	::RegisterHotKey(m_hWnd, 1234, MOD_CONTROL| MOD_ALT, 'H');

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTaskMgrDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTaskMgrDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CTaskMgrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��Ȩ����
BOOL CTaskMgrDlg::AdjustPrivilege()
{
	BOOL bRet = FALSE;
	TOKEN_PRIVILEGES tp = { 0 };//����Ȩ�޽ṹ
	HANDLE hToken = nullptr;//���ƾ��

	do
	{
		//�򿪵�ǰ��������,���һ�ȡ��              //����Ȩ���޸ĺͲ�ѯ
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES |
			TOKEN_QUERY, &hToken))
			break;
		//��ȡ�ػ�ע��������LUID(Locally Unique Identifier),�ֲ�Ψһ��ʶ
		if (!LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid))
			break;
		tp.PrivilegeCount = 1;//�޸�Ȩ�޵ĸ���
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;//����SE_SHUTDOWN_NAME���Ȩ��
														   //����Ȩ��//FALSE��ʾ�����޸�Ȩ��//����Ҫ�޸ĵ�Ȩ�޴�����
		if (!AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)nullptr, 0))
			break;
		bRet = TRUE;
	} while (FALSE);
	if (hToken)
		CloseHandle(hToken);
	return bRet;
}

//�˳����������
void CTaskMgrDlg::OnMenu_Exit()	
{
	// TODO: �ڴ���������������
	DestroyWindow();
}

//ע�������
void CTaskMgrDlg::OnMenu_LogOut()		
{
	// TODO: �ڴ���������������
	//��Ȩ
	AdjustPrivilege();

	int resule = MessageBox(TEXT("ȷ��ע��"), TEXT("��ʾ"), MB_OKCANCEL);
	if (resule == IDOK)
	{
		ExitWindowsEx(EWX_LOGOFF, 0);
	}
}

//�ػ�
void CTaskMgrDlg::OnMenu_ShutDown()		//�ػ�
{
	// TODO: �ڴ���������������
	//��Ȩ
	AdjustPrivilege();
	int resule = MessageBox(TEXT("ȷ�Ϲػ�"), TEXT("��ʾ"), MB_OKCANCEL);
	if (resule == IDOK)
	{
		ExitWindowsEx(EWX_SHUTDOWN, 0);
	}
}

//����
void CTaskMgrDlg::OnMenu_Reboot()
{
	// TODO: �ڴ���������������

	//��Ȩ
	AdjustPrivilege();
	int resule = MessageBox(TEXT("ȷ������"), TEXT("��ʾ"), MB_OKCANCEL);
	if (resule == IDOK)
	{
		ExitWindowsEx(EWX_REBOOT, 0);
	}
}

//VS�ļ�����
void CTaskMgrDlg::OnMenu_FileClean()
{
	// TODO: �ڴ���������������
	CreateChildThread_FileClean();
}


//�����ļ��̻߳ص�����
DWORD WINAPI ThreadProc_FileClean(LPVOID lpParam)
{
	CFileClean m_Flieclean;

// 	m_Flieclean->Create(IDD_DIALOG_FILECLEAN);
// 	m_Flieclean->ShowWindow(SW_SHOW);

	m_Flieclean.DoModal();
	
	return 0;
}

//�����߳������ļ�
bool CreateChildThread_FileClean()
{
	DWORD dwThreadID = 0;
	HANDLE hThread = CreateThread(
		NULL,				//Ĭ�ϰ�ȫ����
		0,					//Ĭ�϶�ջ��С
		ThreadProc_FileClean,//�̺߳���
		L"CreateThread_FileClean",	//����
		0,					//Ĭ�ϴ�����ʶ
		&dwThreadID);		//����TID
	if (hThread == NULL)
	{
		return false;
	}
	//WaitForSingleObject();
	return true;
}


//��ݼ�
BOOL CTaskMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if ((pMsg->message == WM_HOTKEY) && (pMsg->wParam == 1234))
	{
		if (IsWindowVisible() == TRUE)
			ShowWindow(SW_HIDE);
		else
			ShowWindow(SW_SHOW);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAboutDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DestroyWindow();
	CDialogEx::OnClose();
}


