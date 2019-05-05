#pragma once
#include "afxshelltreectrl.h"
#include "afxshelllistctrl.h"
#include <vector>
#include "FileControl.h"
#include "afxwin.h"


// FileClean �Ի���

class CFileClean : public CDialogEx
{
	DECLARE_DYNAMIC(CFileClean)

public:
	CFileClean(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileClean();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILECLEAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();

	CMFCShellTreeCtrl m_objTree;
	CMFCShellListCtrl m_objList;
	// ��ɾ���������ļ�������չʾ
	CString m_cEdit_DelFilePaths;
	// ��ɾ�����ļ���׺����
	CString m_cEdit_DelFileTypes;
	// ѡ�е�Ҫɾ����·��
	CString m_selectPath;
	

	void FindAllFileTree(CString path, CString DelFileType);
	void DeleteALLFile();
	void GetCurrentListFile();
	void CallMenu(LPNMITEMACTIVATE pNMItemActivate);
	CEdit m_cstrEdit_DelFilePaths;
	CFileControl m_fileCtrl;
	HICON m_hIcon;

	//��ɾ���������ļ�
	std::vector<CString> m_veDelFilePaths;

	//˫��Tree�ؼ����ļ���ѡ��Ŀ¼
	afx_msg void OnNMDblclkMfcshelltree1(NMHDR *pNMHDR, LRESULT *pResult);

	//���ü�
	afx_msg void OnBnClickedButton_Reset();

	//ȡ����
	afx_msg void OnBnClickedCancel();

	//�����ļ���
	afx_msg void OnDropFiles(HDROP hDropInfo);

	//ȷ�ϼ�
	afx_msg void OnBnClickedOk();

	//��С��
	afx_msg HCURSOR OnQueryDragIcon();

	//ϵͳ����
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnNMRClickMfcshelltree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickMfcshelllist1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
};

