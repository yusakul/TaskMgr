#pragma once
#include <Strsafe.h>
#include <list>
using std::list;

typedef struct _FILE_INFO
{
	TCHAR      szFilePath[MAX_PATH];       // 文件名
	TCHAR      szFullPath[MAX_PATH];       // 文件全路径
	BOOL       bIsFolder;                  // 是否为文件夹
	DWORD64    qwFileSize;                 // 文件大小
	DWORD      dwAttribute;                // 文件属性
	TCHAR      szAttributeDescription[16]; // 文件属性描述
	SYSTEMTIME stcCreatTime;               // 创建时间
	SYSTEMTIME stcLastAccessTime;          // 最后访问时间
	SYSTEMTIME stcLastWriteTime;           // 最后写入时间
	
}FILE_INFO, *PFILE_INFO;


class CFileControl
{
public:
	CFileControl(void);
	CFileControl(LPCTSTR lpFileName);
	~CFileControl(void);
public:
	DWORD GetMyProcessDir(LPTSTR &lpDirPath);						// 获取本进程当前目录
	BOOL  SetMyProcessDir(LPTSTR lpNewDir);							// 设置程序的当前目录
	DWORD GetMyProcessImagePath(LPTSTR &lpPath);					// 获取本进程映像路径
	DWORD GetProcessImagePath(LPTSTR lpName, LPTSTR &lpPath);		// 根据进程名获取映像路径

	BOOL  SetFileIsOnlyRead(LPTSTR lpPath);							// 设置文件为只读
	BOOL  SetFileIsHidden(LPTSTR lpPath);							// 设置文件为隐藏

	BOOL  GetFileAttribute(LPTSTR lpPath, FILE_INFO &stcFileInfo);  // 将文件属性压入队列
	BOOL  GetFileList(LPTSTR lpName, list<FILE_INFO> &lstFileInfo); // 获取当前目录下文件列表
	DWORD GetFileTree(LPTSTR lpName, list<FILE_INFO> &lstFileInfo,
											    DWORD dwLeve = 1);  // 获取文件树
private:
	HANDLE          m_hFile;
	list<FILE_INFO> m_lstFileInfo;
};

