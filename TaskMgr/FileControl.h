#pragma once
#include <Strsafe.h>
#include <list>
using std::list;

typedef struct _FILE_INFO
{
	TCHAR      szFilePath[MAX_PATH];       // �ļ���
	TCHAR      szFullPath[MAX_PATH];       // �ļ�ȫ·��
	BOOL       bIsFolder;                  // �Ƿ�Ϊ�ļ���
	DWORD64    qwFileSize;                 // �ļ���С
	DWORD      dwAttribute;                // �ļ�����
	TCHAR      szAttributeDescription[16]; // �ļ���������
	SYSTEMTIME stcCreatTime;               // ����ʱ��
	SYSTEMTIME stcLastAccessTime;          // ������ʱ��
	SYSTEMTIME stcLastWriteTime;           // ���д��ʱ��
	
}FILE_INFO, *PFILE_INFO;


class CFileControl
{
public:
	CFileControl(void);
	CFileControl(LPCTSTR lpFileName);
	~CFileControl(void);
public:
	DWORD GetMyProcessDir(LPTSTR &lpDirPath);						// ��ȡ�����̵�ǰĿ¼
	BOOL  SetMyProcessDir(LPTSTR lpNewDir);							// ���ó���ĵ�ǰĿ¼
	DWORD GetMyProcessImagePath(LPTSTR &lpPath);					// ��ȡ������ӳ��·��
	DWORD GetProcessImagePath(LPTSTR lpName, LPTSTR &lpPath);		// ���ݽ�������ȡӳ��·��

	BOOL  SetFileIsOnlyRead(LPTSTR lpPath);							// �����ļ�Ϊֻ��
	BOOL  SetFileIsHidden(LPTSTR lpPath);							// �����ļ�Ϊ����

	BOOL  GetFileAttribute(LPTSTR lpPath, FILE_INFO &stcFileInfo);  // ���ļ�����ѹ�����
	BOOL  GetFileList(LPTSTR lpName, list<FILE_INFO> &lstFileInfo); // ��ȡ��ǰĿ¼���ļ��б�
	DWORD GetFileTree(LPTSTR lpName, list<FILE_INFO> &lstFileInfo,
											    DWORD dwLeve = 1);  // ��ȡ�ļ���
private:
	HANDLE          m_hFile;
	list<FILE_INFO> m_lstFileInfo;
};

