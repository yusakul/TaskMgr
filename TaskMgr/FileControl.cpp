#include "stdafx.h"
#include "FileControl.h"

CFileControl::CFileControl(void){}
CFileControl::CFileControl(LPCTSTR lpFileName)
{
	m_hFile = CreateFile(
		lpFileName,							// 文件名
		GENERIC_ALL,						// 存取方式
		FILE_SHARE_WRITE | FILE_SHARE_READ, // 共享方式
		NULL,								// 文件句柄继承属性
		CREATE_ALWAYS,						// 操作模式
		FILE_ATTRIBUTE_ARCHIVE,				// 文件属性/标志
		NULL);								// 模板文件句柄，一般为NULL

}

CFileControl::~CFileControl(void){
	if (m_hFile)
	{
		delete(m_hFile);
	}
	

}

DWORD CFileControl::GetMyProcessDir(LPTSTR &lpDirPath)
{
	return GetCurrentDirectory(MAX_PATH, lpDirPath);
}

BOOL  CFileControl::SetMyProcessDir(LPTSTR lpNewDir)
{
	return SetCurrentDirectory(lpNewDir);
}

DWORD CFileControl::GetMyProcessImagePath(LPTSTR &lpDirPath)
{
	return GetModuleFileName(NULL, lpDirPath, MAX_PATH);
}

DWORD CFileControl::GetProcessImagePath(LPTSTR lpName, LPTSTR &lpPath)
{
	return GetModuleFileName(GetModuleHandle(lpName), lpPath, MAX_PATH);
}

BOOL  CFileControl::SetFileIsOnlyRead(LPTSTR lpPath)
{

	// 1. 获取原来的文件属性
	DWORD dwFileAttributes = GetFileAttributes(lpPath);
	// 2. 将只读属性附加到原来的文件属性上
	dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
	// 3. 设置文件属性
	return SetFileAttributes(lpPath, dwFileAttributes);
}

BOOL  CFileControl::SetFileIsHidden(LPTSTR lpPath)
{
	// 1. 获取原来的文件属性
	DWORD dwFileAttributes = GetFileAttributes(lpPath);
	// 2. 将隐藏属性附加到原来的文件属性上
	dwFileAttributes |= FILE_ATTRIBUTE_HIDDEN;
	// 3. 设置文件属性
	return SetFileAttributes(lpPath, dwFileAttributes);
}

BOOL  CFileControl::GetFileAttribute(LPTSTR lpPath, FILE_INFO &stcFileInfo)
{
	// 1. 文件路径
	_tcscpy_s(stcFileInfo.szFilePath, lpPath);

	// 2. 获取时间信息
	WIN32_FILE_ATTRIBUTE_DATA wfad;
	GetFileAttributesEx(lpPath, GetFileExInfoStandard, &wfad);
	// 2.1 获取创建时间
	FILETIME ftLocal;
	FileTimeToLocalFileTime(&wfad.ftCreationTime, &ftLocal);        // 调整为系统所在时区的时间
	FileTimeToSystemTime(&ftLocal, &stcFileInfo.stcCreatTime);      // 转换为SYSTEMTIME格式
	// 2.2 获取最后访问时间
	FileTimeToLocalFileTime(&wfad.ftLastAccessTime, &ftLocal);      // 调整为系统所在时区的时间
	FileTimeToSystemTime(&ftLocal, &stcFileInfo.stcLastAccessTime); // 转换为SYSTEMTIME格式
	// 2.3 获取最后修改时间
	FileTimeToLocalFileTime(&wfad.ftLastWriteTime, &ftLocal);       // 调整为系统所在时区的时间
	FileTimeToSystemTime(&ftLocal, &stcFileInfo.stcLastWriteTime);  // 转换为SYSTEMTIME格式

	// 3. 获取文件大小
	stcFileInfo.qwFileSize = wfad.nFileSizeHigh;
	stcFileInfo.qwFileSize <<= sizeof(DWORD) * 8;
	stcFileInfo.qwFileSize += wfad.nFileSizeLow;

	// 4. 文件属性
	stcFileInfo.dwAttribute = wfad.dwFileAttributes;
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_ARCHIVE)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<ARCHIVE> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_COMPRESSED)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<压缩> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_DIRECTORY)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<目录> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_ENCRYPTED)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<加密> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_HIDDEN)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<隐藏> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_NORMAL)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<NORMAL> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_OFFLINE)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<OFFLINE> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_READONLY)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<只读> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_SPARSE_FILE)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<SPARSE> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_SYSTEM)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<系统文件> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_TEMPORARY)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<临时文件> ");

	return TRUE;
}

BOOL  CFileControl::GetFileList(LPTSTR lpName, list<FILE_INFO> &lstFileInfo)
{
	// 1. 构造代表子目录和文件的字符串，使用通配符“*”
	TCHAR szFilePath[MAX_PATH];
	_tcscpy_s(szFilePath, MAX_PATH, lpName);
	_stprintf_s(szFilePath, MAX_PATH, szFilePath,_T("\\*") );

	// 2. 获取第一个文件/目录，并获得查找句柄
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile = FindFirstFile(szFilePath, &FindFileData);
	int n = GetLastError();
	if (INVALID_HANDLE_VALUE == hListFile)   return FALSE;

	// 3. 开始循环遍历获取文件名
	do
	{
		FILE_INFO stcInfo = { 0 };

		// 3.1 判断是否是本级目录或上级目录的名称，是的话则结束本次循环
		if (!lstrcmp(FindFileData.cFileName, _T("."))
			|| !lstrcmp(FindFileData.cFileName, _T("..")))
		{
			continue;
		}
		// 3.2 将获取到的文件名保存在结构体中
		_tcscpy_s(stcInfo.szFilePath, FindFileData.cFileName);
		// 3.3 获取完整路径名
		TCHAR szFullPath[MAX_PATH];
		
		_stprintf_s(szFullPath, MAX_PATH, _T("%s\\%s"), lpName, FindFileData.cFileName);
		_tcscpy_s(stcInfo.szFullPath, szFullPath);
		// 3.4 如果不是目录，则获取详细信息
		if (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			stcInfo.bIsFolder = TRUE;
		else
			GetFileAttribute(szFullPath, stcInfo);
		// 3.5 将获取到的文件信息压入到链表中
		lstFileInfo.push_back(stcInfo);
	} while (FindNextFile(hListFile, &FindFileData));

	return TRUE;
}

DWORD CFileControl::GetFileTree(LPTSTR lpName, list<FILE_INFO> &lstFileInfo, DWORD dwLeve)
{
	// 1. 如果层级是为0则返回，否则将层级-1后继续
	//if (!dwLeve--)  return 0;

	// 2. 构造代表子目录和文件夹路径的字符串，使用通配符“*”
	TCHAR szFilePath[MAX_PATH];
	_tcscpy_s(szFilePath, MAX_PATH, lpName);
	_stprintf_s(szFilePath, MAX_PATH, _T("%s\\%s"), szFilePath, _T("*"));

	// 3. 获取第一个文件/目录，并获得查找句柄
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile = FindFirstFile(szFilePath, &FindFileData);
	if (INVALID_HANDLE_VALUE == hListFile)   
		return 0;

	// 4. 开始循环遍历获取文件名
	DWORD dwFileCount = 0;
	do
	{
		FILE_INFO stcInfo = { 0 };

		// 4.1 判断是否是本级目录或上级目录的名称，是的话则结束本次循环
		if (!lstrcmp(FindFileData.cFileName, L".")
			|| !lstrcmp(FindFileData.cFileName, L".."))
		{
			continue;
		}
		// 3.2 将获取到的文件名保存在结构体中
		_tcscpy_s(stcInfo.szFilePath, FindFileData.cFileName);

		// 4.3 获取完整路径名
		TCHAR szFullPath[MAX_PATH];
		_stprintf_s(szFullPath, MAX_PATH, L"%s\\%s", lpName, FindFileData.cFileName);
		_tcscpy_s(stcInfo.szFullPath, szFullPath);

		// 4.4 如果是目录，则递归调用，列举下级目录
		if (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			dwFileCount += GetFileTree(szFullPath, lstFileInfo,dwLeve);
		else
			// 3.5 将获取到的文件信息压入到链表中
			lstFileInfo.push_back(stcInfo);
		// 4.4 将文件数量+1
		dwFileCount++;
	} while (FindNextFile(hListFile, &FindFileData));

	// 5. 返回本次遍历到的文件总数
	return dwFileCount;
}