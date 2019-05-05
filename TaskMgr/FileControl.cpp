#include "stdafx.h"
#include "FileControl.h"

CFileControl::CFileControl(void){}
CFileControl::CFileControl(LPCTSTR lpFileName)
{
	m_hFile = CreateFile(
		lpFileName,							// �ļ���
		GENERIC_ALL,						// ��ȡ��ʽ
		FILE_SHARE_WRITE | FILE_SHARE_READ, // ����ʽ
		NULL,								// �ļ�����̳�����
		CREATE_ALWAYS,						// ����ģʽ
		FILE_ATTRIBUTE_ARCHIVE,				// �ļ�����/��־
		NULL);								// ģ���ļ������һ��ΪNULL

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

	// 1. ��ȡԭ�����ļ�����
	DWORD dwFileAttributes = GetFileAttributes(lpPath);
	// 2. ��ֻ�����Ը��ӵ�ԭ�����ļ�������
	dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
	// 3. �����ļ�����
	return SetFileAttributes(lpPath, dwFileAttributes);
}

BOOL  CFileControl::SetFileIsHidden(LPTSTR lpPath)
{
	// 1. ��ȡԭ�����ļ�����
	DWORD dwFileAttributes = GetFileAttributes(lpPath);
	// 2. ���������Ը��ӵ�ԭ�����ļ�������
	dwFileAttributes |= FILE_ATTRIBUTE_HIDDEN;
	// 3. �����ļ�����
	return SetFileAttributes(lpPath, dwFileAttributes);
}

BOOL  CFileControl::GetFileAttribute(LPTSTR lpPath, FILE_INFO &stcFileInfo)
{
	// 1. �ļ�·��
	_tcscpy_s(stcFileInfo.szFilePath, lpPath);

	// 2. ��ȡʱ����Ϣ
	WIN32_FILE_ATTRIBUTE_DATA wfad;
	GetFileAttributesEx(lpPath, GetFileExInfoStandard, &wfad);
	// 2.1 ��ȡ����ʱ��
	FILETIME ftLocal;
	FileTimeToLocalFileTime(&wfad.ftCreationTime, &ftLocal);        // ����Ϊϵͳ����ʱ����ʱ��
	FileTimeToSystemTime(&ftLocal, &stcFileInfo.stcCreatTime);      // ת��ΪSYSTEMTIME��ʽ
	// 2.2 ��ȡ������ʱ��
	FileTimeToLocalFileTime(&wfad.ftLastAccessTime, &ftLocal);      // ����Ϊϵͳ����ʱ����ʱ��
	FileTimeToSystemTime(&ftLocal, &stcFileInfo.stcLastAccessTime); // ת��ΪSYSTEMTIME��ʽ
	// 2.3 ��ȡ����޸�ʱ��
	FileTimeToLocalFileTime(&wfad.ftLastWriteTime, &ftLocal);       // ����Ϊϵͳ����ʱ����ʱ��
	FileTimeToSystemTime(&ftLocal, &stcFileInfo.stcLastWriteTime);  // ת��ΪSYSTEMTIME��ʽ

	// 3. ��ȡ�ļ���С
	stcFileInfo.qwFileSize = wfad.nFileSizeHigh;
	stcFileInfo.qwFileSize <<= sizeof(DWORD) * 8;
	stcFileInfo.qwFileSize += wfad.nFileSizeLow;

	// 4. �ļ�����
	stcFileInfo.dwAttribute = wfad.dwFileAttributes;
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_ARCHIVE)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<ARCHIVE> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_COMPRESSED)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<ѹ��> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_DIRECTORY)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<Ŀ¼> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_ENCRYPTED)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<����> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_HIDDEN)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<����> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_NORMAL)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<NORMAL> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_OFFLINE)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<OFFLINE> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_READONLY)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<ֻ��> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_SPARSE_FILE)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<SPARSE> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_SYSTEM)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<ϵͳ�ļ�> ");
	if (stcFileInfo.dwAttribute&FILE_ATTRIBUTE_TEMPORARY)
		_tcscpy_s(stcFileInfo.szAttributeDescription, L"<��ʱ�ļ�> ");

	return TRUE;
}

BOOL  CFileControl::GetFileList(LPTSTR lpName, list<FILE_INFO> &lstFileInfo)
{
	// 1. ���������Ŀ¼���ļ����ַ�����ʹ��ͨ�����*��
	TCHAR szFilePath[MAX_PATH];
	_tcscpy_s(szFilePath, MAX_PATH, lpName);
	_stprintf_s(szFilePath, MAX_PATH, szFilePath,_T("\\*") );

	// 2. ��ȡ��һ���ļ�/Ŀ¼������ò��Ҿ��
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile = FindFirstFile(szFilePath, &FindFileData);
	int n = GetLastError();
	if (INVALID_HANDLE_VALUE == hListFile)   return FALSE;

	// 3. ��ʼѭ��������ȡ�ļ���
	do
	{
		FILE_INFO stcInfo = { 0 };

		// 3.1 �ж��Ƿ��Ǳ���Ŀ¼���ϼ�Ŀ¼�����ƣ��ǵĻ����������ѭ��
		if (!lstrcmp(FindFileData.cFileName, _T("."))
			|| !lstrcmp(FindFileData.cFileName, _T("..")))
		{
			continue;
		}
		// 3.2 ����ȡ�����ļ��������ڽṹ����
		_tcscpy_s(stcInfo.szFilePath, FindFileData.cFileName);
		// 3.3 ��ȡ����·����
		TCHAR szFullPath[MAX_PATH];
		
		_stprintf_s(szFullPath, MAX_PATH, _T("%s\\%s"), lpName, FindFileData.cFileName);
		_tcscpy_s(stcInfo.szFullPath, szFullPath);
		// 3.4 �������Ŀ¼�����ȡ��ϸ��Ϣ
		if (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			stcInfo.bIsFolder = TRUE;
		else
			GetFileAttribute(szFullPath, stcInfo);
		// 3.5 ����ȡ�����ļ���Ϣѹ�뵽������
		lstFileInfo.push_back(stcInfo);
	} while (FindNextFile(hListFile, &FindFileData));

	return TRUE;
}

DWORD CFileControl::GetFileTree(LPTSTR lpName, list<FILE_INFO> &lstFileInfo, DWORD dwLeve)
{
	// 1. ����㼶��Ϊ0�򷵻أ����򽫲㼶-1�����
	//if (!dwLeve--)  return 0;

	// 2. ���������Ŀ¼���ļ���·�����ַ�����ʹ��ͨ�����*��
	TCHAR szFilePath[MAX_PATH];
	_tcscpy_s(szFilePath, MAX_PATH, lpName);
	_stprintf_s(szFilePath, MAX_PATH, _T("%s\\%s"), szFilePath, _T("*"));

	// 3. ��ȡ��һ���ļ�/Ŀ¼������ò��Ҿ��
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile = FindFirstFile(szFilePath, &FindFileData);
	if (INVALID_HANDLE_VALUE == hListFile)   
		return 0;

	// 4. ��ʼѭ��������ȡ�ļ���
	DWORD dwFileCount = 0;
	do
	{
		FILE_INFO stcInfo = { 0 };

		// 4.1 �ж��Ƿ��Ǳ���Ŀ¼���ϼ�Ŀ¼�����ƣ��ǵĻ����������ѭ��
		if (!lstrcmp(FindFileData.cFileName, L".")
			|| !lstrcmp(FindFileData.cFileName, L".."))
		{
			continue;
		}
		// 3.2 ����ȡ�����ļ��������ڽṹ����
		_tcscpy_s(stcInfo.szFilePath, FindFileData.cFileName);

		// 4.3 ��ȡ����·����
		TCHAR szFullPath[MAX_PATH];
		_stprintf_s(szFullPath, MAX_PATH, L"%s\\%s", lpName, FindFileData.cFileName);
		_tcscpy_s(stcInfo.szFullPath, szFullPath);

		// 4.4 �����Ŀ¼����ݹ���ã��о��¼�Ŀ¼
		if (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			dwFileCount += GetFileTree(szFullPath, lstFileInfo,dwLeve);
		else
			// 3.5 ����ȡ�����ļ���Ϣѹ�뵽������
			lstFileInfo.push_back(stcInfo);
		// 4.4 ���ļ�����+1
		dwFileCount++;
	} while (FindNextFile(hListFile, &FindFileData));

	// 5. ���ر��α��������ļ�����
	return dwFileCount;
}