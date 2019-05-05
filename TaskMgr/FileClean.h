#pragma once
#include "afxshelltreectrl.h"
#include "afxshelllistctrl.h"
#include <vector>
#include "FileControl.h"
#include "afxwin.h"


// FileClean 对话框

class CFileClean : public CDialogEx
{
	DECLARE_DYNAMIC(CFileClean)

public:
	CFileClean(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileClean();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILECLEAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();

	CMFCShellTreeCtrl m_objTree;
	CMFCShellListCtrl m_objList;
	// 待删除的所有文件，用于展示
	CString m_cEdit_DelFilePaths;
	// 待删除的文件后缀集合
	CString m_cEdit_DelFileTypes;
	// 选中的要删除的路径
	CString m_selectPath;
	

	void FindAllFileTree(CString path, CString DelFileType);
	void DeleteALLFile();
	void GetCurrentListFile();
	void CallMenu(LPNMITEMACTIVATE pNMItemActivate);
	CEdit m_cstrEdit_DelFilePaths;
	CFileControl m_fileCtrl;
	HICON m_hIcon;

	//待删除的所有文件
	std::vector<CString> m_veDelFilePaths;

	//双击Tree控件内文件夹选定目录
	afx_msg void OnNMDblclkMfcshelltree1(NMHDR *pNMHDR, LRESULT *pResult);

	//重置键
	afx_msg void OnBnClickedButton_Reset();

	//取消键
	afx_msg void OnBnClickedCancel();

	//拖入文件夹
	afx_msg void OnDropFiles(HDROP hDropInfo);

	//确认键
	afx_msg void OnBnClickedOk();

	//最小化
	afx_msg HCURSOR OnQueryDragIcon();

	//系统命令
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnNMRClickMfcshelltree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickMfcshelllist1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
};

