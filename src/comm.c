/* -*- mode: c++; coding: sjis-dos; -*-
 * Time-stamp: <2003-06-01 15:29:26 tfuruka1>
 * Copyright (C) 2003 Tadamegu Furukawa
 *
 * ���ʃ��W���[��
 *
 * $Id: comm.c,v 1.1 2004/01/19 09:01:28 tfuruka1 Exp $
 * $Log: comm.c,v $
 * Revision 1.1  2004/01/19 09:01:28  tfuruka1
 * ���r�W�����Ǘ���RCS����CVS�֕ύX���܂����B
 *
 * Revision 1.1  2003/06/01 08:23:11  tfuruka1
 * Initial revision
 *
 */
#include <windows.h>
#include <stdio.h>

#ifndef CYGWIN
#   include <mbstring.h>
#else
#   include <sys/types.h>
#   include <winsock.h>
#endif

/*--------------------------------------------------------------------
 * Unix��basename�Ɠ����B�ԋp����|�C���^�͈ꎞ�I�ȃ|�C���^�Ȃ̂ŁA�K
 * ���R�s�[���Ďg�p����K�v������B
 * *-------------------------------------------------------------------*/
LPCTSTR WINAPI
BaseName(LPCTSTR lpszPath)
{
    LPCTSTR a, b;
#ifdef _MSC_VER
    a = _mbsrchr(lpszPath, '\\');
    b = _mbsrchr(lpszPath, '/');
#else
    a = strchr(lpszPath, '\\');
    b = strchr(lpszPath, '/');
#endif

    if (a == b) {                               // ���������� NULL�����Ȃ�
        return lpszPath;
    }

    return ((ULONG)a > (ULONG)b ? a : b) + 1;
}
/*--------------------------------------------------------------------
 * Unix��basename�Ɠ����B�ԋp����|�C���^�͈ꎞ�I�ȃ|�C���^�Ȃ̂ŁA�K
 * ���R�s�[���Ďg�p����K�v������B�V���[�g�t�@�C�������w�肳��Ă��A
 * �����O�t�@�C�����֕ϊ����܂��B
 * *-------------------------------------------------------------------*/
LPCTSTR WINAPI
GetLongBaseName(LPCTSTR lpszPath)
{
    HANDLE hFile;
    static WIN32_FIND_DATA wfd;

    hFile = FindFirstFile(lpszPath, &wfd);
    if (INVALID_HANDLE_VALUE == hFile) {
        return BaseName(lpszPath);
    }
    FindClose(hFile);
    return wfd.cFileName;
}

/*--------------------------------------------------------------------
 * dwErr�ɑΉ�����G���[���b�Z�[�W��ԋp����BdwErr��GetLastError()��
 * �擾�����l���w�肷��B
 * *-------------------------------------------------------------------*/
LPCSTR WINAPI
GetLastErrorMessage(LPCSTR lpsz, DWORD dwErr)
{
#ifdef _WIN32_WCE
    static LPTSTR sz = "WindowsCE �ł� GetLastErrorMessage �͖��T�|�[�g�ł�";
    return sz;
#else
    static TCHAR sz[1024];
    TCHAR szTmp[256];
    int i;

    if (!(i = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
                            FORMAT_MESSAGE_IGNORE_INSERTS,
                            NULL, dwErr, 0, szTmp, sizeof(szTmp), NULL))) {
        strcpy(szTmp, "---");
    }
    else {
        szTmp[i] = '\0';
        for (i--; 0 <= i; i--) {
            if ('\n' == szTmp[i] || '\r' == szTmp[i]) {
                szTmp[i] = '\0';
            }
        }
    }
    wsprintf(sz, "[WIN32] %s: Error Code = %d(%#02x): %s",
             lpsz, dwErr, dwErr, szTmp);
    return (LPCTSTR)sz;
#endif
}

/*--------------------------------------------------------------------
 * UNIX��Syslog�̊ȈՔŁB���debug.local7�����o�͂��܂���B
 * *-------------------------------------------------------------------*/
VOID WINAPI
Syslogger(BOOL bStdOut,                         // T:stdout�ɂ��o��
          LPCSTR lpstr,                         // ����printf�Ɠ���
          ...                                   // ����
    )
{
    WSADATA wsaData;
    SOCKET finet;
    PSERVENT sp;
    SOCKADDR_IN sin;
    LPHOSTENT lpHost;
    va_list args;                               // �����W�J�p
    char szLine[1024 * 64], szBuf[1024], *p;

    // ���W���[�����̃t���p�X�𓾂�
    if (!GetModuleFileName(GetModuleHandle(NULL), szBuf, 1024)) {
        return;
    }
    // �t�@�C�������������؂�o��
    if (NULL == (p = strrchr(szBuf, '\\'))) {
        return;
    }
    // FACILITY = 23, LEVEL = 7
    sprintf(szLine, "<%d>[%s] ", 7 + 23 * 8, p + 1);

    // ������������ɍ����Đ��`����
    va_start(args, lpstr);
    vsprintf(szLine + strlen(szLine), lpstr, args);
    va_end(args);

    if (bStdOut) {
        printf("%s\n", szLine);
    }

    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		return;
	}

    if (0 > (finet = socket(AF_INET, SOCK_DGRAM, 0))) {
        return;
    }

    lpHost = gethostbyname("localhost");

    if (NULL == (sp = getservbyname("syslog", "udp"))) {
        return;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(sp->s_port);
    sin.sin_addr = *((LPIN_ADDR)*lpHost->h_addr_list);

    sendto(finet, szLine, strlen(szLine), 0, (PSOCKADDR)&sin,
           sizeof(SOCKADDR));
    closesocket(finet);
    WSACleanup();
}
