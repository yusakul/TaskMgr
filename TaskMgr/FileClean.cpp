// FileClean.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskMgr.h"
#include "FileClean.h"
#include "afxdialogex.h"


// FileClean 对话框

IMPLEMENT_DYNAMIC(CFileClean, CDialogEx)

CFileClean::CFileClean(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_FILECLEAN, pParent)
	, m_cEdit_DelFilePaths(_T(""))
	, m_cEdit_DelFileTypes(_T("\.db\.obj\.tlog\.lastbuildstate\.idb\.pdb\.pch\.res\.ilk\.sdf\.ipch\.log"))
/*	, m_selectPath(_T(""))*/
{

}

CFileClean::~CFileClean()
{
}

void CFileClean::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCSHELLTREE1, m_objTree);
	DDX_Control(pDX, IDC_MFCSHELLLIST1, m_objList);
	DDX_Text(pDX, IDC_EDIT3, m_cEdit_DelFilePaths);
	DDX_Text(pDX, IDC_EDIT1, m_cEdit_DelFileTypes);
	DDX_Control(pDX, IDC_EDIT3, m_cstrEdit_DelFilePaths);
	DDX_Control(pDX, IDC_EDIT3, m_cstrEdit_DelFilePaths);
}


BEGIN_MESSAGE_MAP(CFileClean, CDialogEx)
//	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK, &CFileClean::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_MFCSHELLTREE1, &CFileClean::OnNMDblclkMfcshelltree1)
	ON_BN_CLICKED(IDC_BUTTON1, &CFileClean::OnBnClickedButton_Reset)
	ON_BN_CLICKED(IDCANCEL, &CFileClean::OnBnClickedCancel)
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_NOTIFY(NM_RCLICK, IDC_MFCSHELLTREE1, &CFileClean::OnNMRClickMfcshelltree1)
	ON_NOTIFY(NM_RCLICK, IDC_MFCSHELLLIST1, &CFileClean::OnNMRClickMfcshelllist1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFileClean 消息处理程序


BOOL CFileClean::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_CLEAN);
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);


	//绑定Tree与List
	m_objTree.SetRelatedList(&m_objList);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CFileClean::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateData(TRUE);
	OnBnClickedButton_Reset();
	TCHAR FileName[MAX_PATH];
	int FileNum = DragQueryFile(hDropInfo, -1, FileName, 0); //拖拉的文件个数

	for (int i = 0; i < FileNum; i++) {
		DragQueryFile(hDropInfo, i, FileName, 300); //取得每个文件的文件名		
		FindAllFileTree(FileName, m_cEdit_DelFileTypes);
	}
	if (FileNum >= 1)
	{
		m_objTree.SelectPath(FileName);
		m_selectPath = FileName;

	}

	for (unsigned i = 0; i < m_veDelFilePaths.size(); i++)
	{
		m_cEdit_DelFilePaths = m_cEdit_DelFilePaths + m_veDelFilePaths[i] + L"\r\n";
	}
	UpdateData(FALSE);
	CDialogEx::OnDropFiles(hDropInfo);
}


//确认键
void CFileClean::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	

	int resule = MessageBox(TEXT("删除不可逆，确定删除"), TEXT("提示"), MB_OKCANCEL);
	if (resule == IDOK)
	{
		DeleteALLFile();
		MessageBox(_T("清理完毕"));
	}
}

//检索该目录下所有选中类型文件
void CFileClean::FindAllFileTree(CString path, CString DelFileType)
{
	list<FILE_INFO> listFileInfo;
	m_fileCtrl.GetFileTree(path.GetBuffer(), listFileInfo, 20);


	for (list<FILE_INFO>::iterator it = listFileInfo.begin();
		it != listFileInfo.end(); it++)
	{
		if (it->bIsFolder == TRUE)
		{
			continue;
		}
		CString szFullPath = it->szFullPath;
		int index = szFullPath.ReverseFind(_T('.'));
		szFullPath = szFullPath.Right(szFullPath.GetLength() - index);
		if (DelFileType.Find(szFullPath) != -1)
		{
			m_veDelFilePaths.push_back(it->szFullPath);
		}
	}
}

//删除文件
void CFileClean::DeleteALLFile()
{
	for (unsigned i = 0; i < m_veDelFilePaths.size(); i++)
	{
		DeleteFile(m_veDelFilePaths[i]);
	}
	m_veDelFilePaths.clear();
}

//获取当前目录需要清理的文件
void CFileClean::GetCurrentListFile()
{
	UpdateData(TRUE);
	OnBnClickedButton_Reset();
	//获取选中项	
	m_objTree.GetItemPath(m_selectPath, m_objTree.GetSelectedItem());
	FindAllFileTree(m_selectPath, m_cEdit_DelFileTypes);
	for (unsigned i = 0; i < m_veDelFilePaths.size(); i++)
	{
		m_cEdit_DelFilePaths = m_cEdit_DelFilePaths + m_veDelFilePaths[i] + L"\r\n";
	}
	UpdateData(FALSE);
}

//调用菜单
void CFileClean::CallMenu(LPNMITEMACTIVATE pNMItemActivate)
{
	CMenu menu, *pmenu;
	menu.LoadMenuW(IDR_MENU_Clean);
	pmenu = menu.GetSubMenu(0);//指定包含菜单中的弹出菜单的位置。 位置值从0开始第一个菜单项。 
	ClientToScreen(&pNMItemActivate->ptAction);//在客户端坐标中指示发生事件的位置
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);
	switch (Select)
	{
	case ID_32789:	//添加目录
	{
		//获取当前目录需要清理的文件
		GetCurrentListFile();
	}
	break;
	case ID_32790:	//取消目录
	{
		//重置
		OnBnClickedButton_Reset();
	}
	break;
	default:
		break;
	}
}


//左键双击文件夹添加清理
void CFileClean::OnNMDblclkMfcshelltree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	//获取当前目录需要清理的文件
	GetCurrentListFile();
	*pResult = 0;

}

//重置
void CFileClean::OnBnClickedButton_Reset()
{
	// TODO: 在此添加控件通知处理程序代码
	m_veDelFilePaths.clear();				//待删除的所有文件,用于删除
	m_cEdit_DelFilePaths = _T("");			//待删除的所有文件,用于展示
	m_selectPath = _T("");					//选中的要删除的路径
	UpdateData(FALSE);
}

//退出
void CFileClean::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
	DestroyWindow();
}


HCURSOR CFileClean::OnQueryDragIcon()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return static_cast<HCURSOR>(m_hIcon);
	//return CDialogEx::OnQueryDragIcon();
}

//右键Tree控件添加清理目录
void CFileClean::OnNMRClickMfcshelltree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	
	//获取当前目录需要清理的文件
	GetCurrentListFile();
	*pResult = 0;
}

//右键list控件
void CFileClean::OnNMRClickMfcshelllist1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CallMenu(pNMItemActivate);
	*pResult = 0;
}




void CFileClean::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	EndDialog(IDD_DIALOG_FILECLEAN);
	//DestroyWindow();
	CDialogEx::OnClose();
}
