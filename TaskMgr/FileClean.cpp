// FileClean.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskMgr.h"
#include "FileClean.h"
#include "afxdialogex.h"


// FileClean �Ի���

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


// CFileClean ��Ϣ�������


BOOL CFileClean::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_CLEAN);
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);


	//��Tree��List
	m_objTree.SetRelatedList(&m_objList);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CFileClean::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UpdateData(TRUE);
	OnBnClickedButton_Reset();
	TCHAR FileName[MAX_PATH];
	int FileNum = DragQueryFile(hDropInfo, -1, FileName, 0); //�������ļ�����

	for (int i = 0; i < FileNum; i++) {
		DragQueryFile(hDropInfo, i, FileName, 300); //ȡ��ÿ���ļ����ļ���		
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


//ȷ�ϼ�
void CFileClean::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	

	int resule = MessageBox(TEXT("ɾ�������棬ȷ��ɾ��"), TEXT("��ʾ"), MB_OKCANCEL);
	if (resule == IDOK)
	{
		DeleteALLFile();
		MessageBox(_T("�������"));
	}
}

//������Ŀ¼������ѡ�������ļ�
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

//ɾ���ļ�
void CFileClean::DeleteALLFile()
{
	for (unsigned i = 0; i < m_veDelFilePaths.size(); i++)
	{
		DeleteFile(m_veDelFilePaths[i]);
	}
	m_veDelFilePaths.clear();
}

//��ȡ��ǰĿ¼��Ҫ������ļ�
void CFileClean::GetCurrentListFile()
{
	UpdateData(TRUE);
	OnBnClickedButton_Reset();
	//��ȡѡ����	
	m_objTree.GetItemPath(m_selectPath, m_objTree.GetSelectedItem());
	FindAllFileTree(m_selectPath, m_cEdit_DelFileTypes);
	for (unsigned i = 0; i < m_veDelFilePaths.size(); i++)
	{
		m_cEdit_DelFilePaths = m_cEdit_DelFilePaths + m_veDelFilePaths[i] + L"\r\n";
	}
	UpdateData(FALSE);
}

//���ò˵�
void CFileClean::CallMenu(LPNMITEMACTIVATE pNMItemActivate)
{
	CMenu menu, *pmenu;
	menu.LoadMenuW(IDR_MENU_Clean);
	pmenu = menu.GetSubMenu(0);//ָ�������˵��еĵ����˵���λ�á� λ��ֵ��0��ʼ��һ���˵�� 
	ClientToScreen(&pNMItemActivate->ptAction);//�ڿͻ���������ָʾ�����¼���λ��
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);
	switch (Select)
	{
	case ID_32789:	//���Ŀ¼
	{
		//��ȡ��ǰĿ¼��Ҫ������ļ�
		GetCurrentListFile();
	}
	break;
	case ID_32790:	//ȡ��Ŀ¼
	{
		//����
		OnBnClickedButton_Reset();
	}
	break;
	default:
		break;
	}
}


//���˫���ļ����������
void CFileClean::OnNMDblclkMfcshelltree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ��ǰĿ¼��Ҫ������ļ�
	GetCurrentListFile();
	*pResult = 0;

}

//����
void CFileClean::OnBnClickedButton_Reset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_veDelFilePaths.clear();				//��ɾ���������ļ�,����ɾ��
	m_cEdit_DelFilePaths = _T("");			//��ɾ���������ļ�,����չʾ
	m_selectPath = _T("");					//ѡ�е�Ҫɾ����·��
	UpdateData(FALSE);
}

//�˳�
void CFileClean::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
	DestroyWindow();
}


HCURSOR CFileClean::OnQueryDragIcon()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return static_cast<HCURSOR>(m_hIcon);
	//return CDialogEx::OnQueryDragIcon();
}

//�Ҽ�Tree�ؼ��������Ŀ¼
void CFileClean::OnNMRClickMfcshelltree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//��ȡ��ǰĿ¼��Ҫ������ļ�
	GetCurrentListFile();
	*pResult = 0;
}

//�Ҽ�list�ؼ�
void CFileClean::OnNMRClickMfcshelllist1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CallMenu(pNMItemActivate);
	*pResult = 0;
}




void CFileClean::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	EndDialog(IDD_DIALOG_FILECLEAN);
	//DestroyWindow();
	CDialogEx::OnClose();
}
