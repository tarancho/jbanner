/* -*- mode: c++; coding: sjis-dos; -*-
 * Time-stamp: <2003-06-01 15:29:26 tfuruka1>
 * Copyright (C) 2003 Tadamegu Furukawa
 *
 * 共通モジュール
 *
 * $Id: comm.c,v 1.1 2004/01/19 09:01:28 tfuruka1 Exp $
 * $Log: comm.c,v $
 * Revision 1.1  2004/01/19 09:01:28  tfuruka1
 * リビジョン管理をRCSからCVSへ変更しました。
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
 * Unixのbasenameと同じ。返却するポインタは一時的なポインタなので、必
 * ずコピーして使用する必要がある。
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

    if (a == b) {                               // 等しい時は NULLしかない
        return lpszPath;
    }

    return ((ULONG)a > (ULONG)b ? a : b) + 1;
}
/*--------------------------------------------------------------------
 * Unixのbasenameと同じ。返却するポインタは一時的なポインタなので、必
 * ずコピーして使用する必要がある。ショートファイル名が指定されても、
 * ロングファイル名へ変換します。
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
 * dwErrに対応するエラーメッセージを返却する。dwErrはGetLastError()で
 * 取得した値を指定する。
 * *-------------------------------------------------------------------*/
LPCSTR WINAPI
GetLastErrorMessage(LPCSTR lpsz, DWORD dwErr)
{
#ifdef _WIN32_WCE
    static LPTSTR sz = "WindowsCE では GetLastErrorMessage は未サポートです";
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
 * UNIXのSyslogの簡易版。常にdebug.local7しか出力しません。
 * *-------------------------------------------------------------------*/
VOID WINAPI
Syslogger(BOOL bStdOut,                         // T:stdoutにも出力
          LPCSTR lpstr,                         // 書式printfと同じ
          ...                                   // 引数
    )
{
    WSADATA wsaData;
    SOCKET finet;
    PSERVENT sp;
    SOCKADDR_IN sin;
    LPHOSTENT lpHost;
    va_list args;                               // 引数展開用
    char szLine[1024 * 64], szBuf[1024], *p;

    // モジュール名のフルパスを得る
    if (!GetModuleFileName(GetModuleHandle(NULL), szBuf, 1024)) {
        return;
    }
    // ファイル名部分だけ切り出す
    if (NULL == (p = strrchr(szBuf, '\\'))) {
        return;
    }
    // FACILITY = 23, LEVEL = 7
    sprintf(szLine, "<%d>[%s] ", 7 + 23 * 8, p + 1);

    // 文字列を書式に合せて整形する
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
