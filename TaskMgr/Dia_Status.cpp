// Dia_Status.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskMgr.h"
#include "Dia_Status.h"
#include "afxdialogex.h"
#include "Data.h"

//void WINAPI GlobalMemoryStatus(_Out_ LPMEMORYSTATUS lpBuffer);//创建MEMRORYSTATUS结构体指针


// CDia_Status 对话框

IMPLEMENT_DYNAMIC(CDia_Status, CDialogEx)

CDia_Status::CDia_Status(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_STATUS, pParent)
, m_Edit_CPUrate(_T(""))
, m_Edit_CPUspeed(_T(""))
, m_CPU_CurrentIdle(_T(""))
, m_CPU_CurrentMhz(_T(""))
, m_CPU_MaxIdleState(_T(""))
, m_CPU_MaxMhz(_T(""))
, m_CPU_MhzLimit(_T(""))
, m_CPU_Number(_T(""))
{

}

CDia_Status::~CDia_Status()
{
}

void CDia_Status::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_CPU1, m_Edit_CPUrate);
	DDX_Text(pDX, IDC_EDIT_CPU_CurrentIdleState, m_CPU_CurrentIdle);
	DDX_Text(pDX, IDC_EDIT_CPU_CurrentMhz, m_CPU_CurrentMhz);
	DDX_Text(pDX, IDC_EDIT_CPU_MaxIdleState, m_CPU_MaxIdleState);
	DDX_Text(pDX, IDC_EDIT_CPU_MaxMhz, m_CPU_MaxMhz);
	//  DDX_Control(pDX, IDC_EDIT_CPU_MhzLimit, m_CPU_MhzLimit);
	DDX_Text(pDX, IDC_EDIT_CPU_MhzLimit, m_CPU_MhzLimit);
	DDX_Text(pDX, IDC_EDIT_CPU_Number, m_CPU_Number);
}


BEGIN_MESSAGE_MAP(CDia_Status, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//ON_MESSAGE(UpdataCPUrate, &CDia_Status::OnUpdatacpurate)
	ON_MESSAGE(WM_MYMSG, OnUpdatacpurate)
END_MESSAGE_MAP()


// CDia_Status 消息处理程序


BOOL CDia_Status::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	SetTimer(5675, 1000, NULL);
	//SetTimer(IDC_STATIC_HISTOGRAM_CPU, 1000, NULL);

	CRect rect;

	// 获取静态控件的举行

	GetDlgItem(IDC_STATIC_HISTOGRAM_CPU)->GetWindowRect(rect);

	// 将该矩形转化为客户窗口

	ScreenToClient(rect);

	// 调用Create函数，并将上面的矩形传入

	m_ctrlHistogram.Create(WS_VISIBLE | WS_CHILD

		| WS_TABSTOP, rect, this, IDC_STATIC_HISTOGRAM_CPU);





	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDia_Status::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	DestroyWindow();
	CDialogEx::OnClose();
}


//更新CPU信息
void CDia_Status::UpdataCPUInfo()
{
	NTSTATUS Processor_information;
	PROCESSOR_POWER_INFORMATION* ppInfo = new PROCESSOR_POWER_INFORMATION[GetNumCPUs()];

	Processor_information = CallNtPowerInformation(ProcessorInformation
		, NULL, 0, ppInfo, GetNumCPUs() * sizeof(PROCESSOR_POWER_INFORMATION));

	ppInfo->Number;	//核心数
	ppInfo->MaxMhz;	//最大频率
	ppInfo->CurrentMhz;	//当前频率
	ppInfo->MhzLimit;	//限制频率
	ppInfo->MaxIdleState;	//最大空闲状态
	ppInfo->CurrentIdleState;//当前空闲状态


	UpdateData(TRUE);
	m_Edit_CPUrate.Format(_T("%d"), GetCPUuseRate());
	m_CPU_MaxMhz = "4000.0";
	m_CPU_CurrentMhz.Format(_T("%d"), ppInfo->CurrentMhz);
	m_CPU_MhzLimit.Format(_T("%d"), ppInfo->MhzLimit);
	m_CPU_MaxIdleState.Format(_T("%d"), ppInfo->MaxIdleState);
	m_CPU_CurrentIdle.Format(_T("%d"), ppInfo->CurrentIdleState);
	m_CPU_Number.Format(_T("%d"), GetNumCPUs());
	UpdateData(FALSE);
	
}

//处理器数量
int  CDia_Status::GetNumCPUs()
{
	SYSTEM_INFO system_info;
	memset(&system_info, 0x0, sizeof(system_info));//重置
	GetSystemInfo(&system_info);
// 	if (system_info.dwNumberOfProcessors == 0)//处理器数量
// 	{
// 		return 1;
// 	}
// 	else
	return system_info.dwNumberOfProcessors;
}

//CPU空闲时间
double CDia_Status::FILETIME2DOUBLE(const _FILETIME& fileTime)
{
	return double(fileTime.dwHighDateTime*4.294967296e9)
		+ double(fileTime.dwLowDateTime);
}

//CPU使用率
int CDia_Status::GetCPUuseRate()
{
	_FILETIME idleTime, kernelTime, userTime;//空闲时间，内核时间，用户时间
	//获取时间
	GetSystemTimes(&idleTime, &kernelTime, &userTime);

	//等待1000毫秒
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 50);
	/*PostMessage()*/
	::PostMessage(this->m_hWnd, WM_MYMSG, 0, 0);

	//获取新的时间
	_FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);

	//将各个时间转换
	double dOldIdleTime = FILETIME2DOUBLE(idleTime);
	double dNewIdleTime = FILETIME2DOUBLE(newIdleTime);
	double dOldKernelTime = FILETIME2DOUBLE(kernelTime);
	double dNewKernelTime = FILETIME2DOUBLE(newKernelTime);
	double dOldUserTime = FILETIME2DOUBLE(userTime);
	double dNewUserTime = FILETIME2DOUBLE(newUserTime);

	//计算出使用率
	int lResult = (int)((100.0 - 100.0*(dNewIdleTime - dOldIdleTime) / (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)));

	//int lResult = 100*((dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)/(dNewIdleTime - dOldIdleTime));

	return lResult;
}


//定时执行程序
void CDia_Status::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//更新CPU信息
	UpdataCPUInfo();

	m_ctrlHistogram.SetPos((UINT)GetCPUuseRate());

	CDialogEx::OnTimer(nIDEvent);
}


afx_msg LRESULT CDia_Status::OnUpdatacpurate(WPARAM wParam, LPARAM lParam)
{
// 	UpdateData(TRUE);
// 	m_Edit_CPUrate.Format(_T("%d"), GetCPUuseRate());
// 	UpdateData(FALSE);
	
	return 0;
}
