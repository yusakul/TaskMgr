
// TaskMgrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskMgr.h"
#include "TaskMgrDlg.h"
#include "afxdialogex.h"
#include <WinUser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//清理文件线程回调函数
DWORD WINAPI ThreadProc_FileClean(LPVOID lpParam);
bool CreateChildThread_FileClean();



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTaskMgrDlg 对话框



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


// CTaskMgrDlg 消息处理程序

BOOL CTaskMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	m_MyTab.InsertItem(0, L"窗口进程");
	m_MyTab.InsertItem(1, L"详细信息");
	m_MyTab.InsertItem(2, L"CPU");
	m_MyTab.InsertItem(3, L"RAM");
	m_MyTab.InsertItem(4, L"Disk");

	//给子窗口指针赋值
	m_MyTab.m_Dia[0] = new CDia_WinProc;
	m_MyTab.m_Dia[1] = new CDia_Details;
	m_MyTab.m_Dia[2] = new CDia_Status;
	m_MyTab.m_Dia[3] = new CDia_Ram;
	m_MyTab.m_Dia[4] = new CDia_Disk;

	//创建子窗口
	m_MyTab.m_Dia[0]->Create(IDD_DIALOG_WINPROCESS, &m_MyTab);
	m_MyTab.m_Dia[1]->Create(IDD_DIALOG_DETAILS, &m_MyTab);
	m_MyTab.m_Dia[2]->Create(IDD_DIALOG_STATUS, &m_MyTab);
	m_MyTab.m_Dia[3]->Create(IDD_DIALOG_STATUS_RAM, &m_MyTab);
	m_MyTab.m_Dia[4]->Create(IDD_DIALOG_DISK, &m_MyTab);

	//控制两个子窗口大小
	CRect rc;
	m_MyTab.GetClientRect(rc);
	rc.DeflateRect(2, 25, 2, 2);
	m_MyTab.m_Dia[0]->MoveWindow(rc);
	m_MyTab.m_Dia[1]->MoveWindow(rc);
	m_MyTab.m_Dia[2]->MoveWindow(rc);
	m_MyTab.m_Dia[3]->MoveWindow(rc);
	m_MyTab.m_Dia[4]->MoveWindow(rc);


	//显示第一个子窗口
	m_MyTab.m_Dia[0]->ShowWindow(SW_SHOW);
	m_MyTab.m_Dia[1]->ShowWindow(SW_HIDE);
	m_MyTab.m_Dia[2]->ShowWindow(SW_HIDE);
	m_MyTab.m_Dia[3]->ShowWindow(SW_HIDE);
	m_MyTab.m_Dia[4]->ShowWindow(SW_HIDE);


	//菜单
	CMenu m_menu;
	m_menu.LoadMenu(IDR_MENU_MainWindow);
	SetMenu(&m_menu);

	::RegisterHotKey(m_hWnd, 1234, MOD_CONTROL| MOD_ALT, 'H');

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTaskMgrDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CTaskMgrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//提权函数
BOOL CTaskMgrDlg::AdjustPrivilege()
{
	BOOL bRet = FALSE;
	TOKEN_PRIVILEGES tp = { 0 };//令牌权限结构
	HANDLE hToken = nullptr;//令牌句柄

	do
	{
		//打开当前进程令牌,并且获取它              //令牌权限修改和查询
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES |
			TOKEN_QUERY, &hToken))
			break;
		//获取关机注销重启的LUID(Locally Unique Identifier),局部唯一标识
		if (!LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid))
			break;
		tp.PrivilegeCount = 1;//修改权限的个数
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;//激活SE_SHUTDOWN_NAME这个权限
														   //提升权限//FALSE表示可以修改权限//把需要修改的权限传进来
		if (!AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)nullptr, 0))
			break;
		bRet = TRUE;
	} while (FALSE);
	if (hToken)
		CloseHandle(hToken);
	return bRet;
}

//退出任务管理器
void CTaskMgrDlg::OnMenu_Exit()	
{
	// TODO: 在此添加命令处理程序代码
	DestroyWindow();
}

//注销计算机
void CTaskMgrDlg::OnMenu_LogOut()		
{
	// TODO: 在此添加命令处理程序代码
	//提权
	AdjustPrivilege();

	int resule = MessageBox(TEXT("确认注销"), TEXT("提示"), MB_OKCANCEL);
	if (resule == IDOK)
	{
		ExitWindowsEx(EWX_LOGOFF, 0);
	}
}

//关机
void CTaskMgrDlg::OnMenu_ShutDown()		//关机
{
	// TODO: 在此添加命令处理程序代码
	//提权
	AdjustPrivilege();
	int resule = MessageBox(TEXT("确认关机"), TEXT("提示"), MB_OKCANCEL);
	if (resule == IDOK)
	{
		ExitWindowsEx(EWX_SHUTDOWN, 0);
	}
}

//重启
void CTaskMgrDlg::OnMenu_Reboot()
{
	// TODO: 在此添加命令处理程序代码

	//提权
	AdjustPrivilege();
	int resule = MessageBox(TEXT("确认重启"), TEXT("提示"), MB_OKCANCEL);
	if (resule == IDOK)
	{
		ExitWindowsEx(EWX_REBOOT, 0);
	}
}

//VS文件清理
void CTaskMgrDlg::OnMenu_FileClean()
{
	// TODO: 在此添加命令处理程序代码
	CreateChildThread_FileClean();
}


//清理文件线程回调函数
DWORD WINAPI ThreadProc_FileClean(LPVOID lpParam)
{
	CFileClean m_Flieclean;

// 	m_Flieclean->Create(IDD_DIALOG_FILECLEAN);
// 	m_Flieclean->ShowWindow(SW_SHOW);

	m_Flieclean.DoModal();
	
	return 0;
}

//创建线程清理文件
bool CreateChildThread_FileClean()
{
	DWORD dwThreadID = 0;
	HANDLE hThread = CreateThread(
		NULL,				//默认安全属性
		0,					//默认堆栈大小
		ThreadProc_FileClean,//线程函数
		L"CreateThread_FileClean",	//参数
		0,					//默认创建标识
		&dwThreadID);		//返回TID
	if (hThread == NULL)
	{
		return false;
	}
	//WaitForSingleObject();
	return true;
}


//快捷键
BOOL CTaskMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DestroyWindow();
	CDialogEx::OnClose();
}


