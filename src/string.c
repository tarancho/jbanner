/* -*- mode: c++; coding: sjis-dos; -*-
 * Time-stamp: <2003-06-01 15:28:34 tfuruka1>
 * Copyright (C) 2003 Tadamegu Furukawa
 *
 *  文字列操作関数
 *
 * $Id: string.c,v 1.1 2004/01/19 09:01:27 tfuruka1 Exp $
 * $Log: string.c,v $
 * Revision 1.1  2004/01/19 09:01:27  tfuruka1
 * リビジョン管理をRCSからCVSへ変更しました。
 *
 */
// (replace-regexp "/\\*\\(.+\\)\\*/" "//\\1")
// (replace-regexp "[ \t]+$" "")

#include <windows.h>
#include <stdio.h>

#define Z_SPACE "\x81\x40"                      // 全角スペース

/* -------------------------------------------------------------------
 * 文字列の右側のスペース文字を取り除く
 * *-----------------------------------------------------------------*/
LPTSTR TrimRight(LPTSTR lpszStr)
{
    size_t len = strlen(lpszStr);

    for (--len; len; len--) {
        if ('\r' == *(lpszStr + len)
            || '\n' == *(lpszStr + len)
            || '\t' == *(lpszStr + len)
            || '\f' == *(lpszStr + len)
            || ' ' == *(lpszStr + len)) {
            *(lpszStr + len) = '\0';
        }
        else {
            break;
        }
    }
    return lpszStr;
}

/* -------------------------------------------------------------------
 * 文字列の右側の全角スペース文字を取り除く
 * *-----------------------------------------------------------------*/
LPTSTR TrimRightJa(LPTSTR lpszStr)
{
    size_t len = strlen(lpszStr) - 2;

    for (; len; len -= 2) {
        if (0 == strcmp(Z_SPACE, lpszStr + len)) {
            *(lpszStr + len) = '\0';
        }
        else {
            break;
        }
    }
    return lpszStr;
}

/* -------------------------------------------------------------------
 * 文字列の左側のスペース文字を取り除く
 * *-----------------------------------------------------------------*/
LPTSTR TrimLeft(LPTSTR lpszStr)
{
    LPTSTR pFrom = lpszStr, pTo = lpszStr;

    for (; *pFrom; pFrom++) {
        if ('\r' != *pFrom
            && '\n' != *pFrom
            && '\t' != *pFrom
            && '\f' != *pFrom
            && ' ' != *pFrom) {
            break;
        }
    }

    if (pFrom == pTo) {                         // 余分な文字は無い
        return lpszStr;
    }
    for (; *pFrom; pFrom++, pTo++) {
        *pTo = *pFrom;
    }
    *pTo = '\0';
    return lpszStr;
}

/* -------------------------------------------------------------------
 * 文字列の左右のスペース文字を取り除く
 * *-----------------------------------------------------------------*/
LPTSTR TrimString(LPSTR lpszStr)
{
    return TrimLeft(TrimRight(lpszStr));
}

/* -------------------------------------------------------------------
 * fgetsと同様の処理を行うが、タブをスペース文字列へ展開する。但し、タ
 * ブの展開を途中で止める事は無いので、最大文字列長はタブストップを考
 * 慮する必要がある。
 * *-----------------------------------------------------------------*/
LPTSTR
ufgets(
    LPTSTR szBuf,                               // 格納バッファ
    int cbMax,                                  // 最大文字列長
    FILE *fp,                                   // ファイルポインタ
    int nTab                                    // タブストップ
    )
{
    int i, j, c;

    for (i = 0; i < cbMax - 1; i++) {
        if (EOF == (c = getc(fp))) break;
        if ('\t' == c) {
            for (j = 0; j < (nTab - (i % nTab)); j++) {
                *szBuf++ = ' ';
            }
            i += (nTab - (i % nTab));
            i--;
            continue;
        }
        *szBuf++ = c;
        if ('\n' == c) {
            i++;
            break;
        }
    }
    *szBuf = '\0';

    if (!i) {
        return NULL;
    }
    return szBuf;
}

/* -------------------------------------------------------------------
 * strの文字列を区切り文字cで区切り、区切られた文字列の先頭アドレスを
 * それぞれ*intoに格納する。但しmaxで指定された数より多くは区切らない。
 * 連続した区切り文字は1つと見る。
 * *-----------------------------------------------------------------*/
int unstr(char *str, int c, char *into[], int max)
{
    int intomax = 0;

    into[intomax++] = str;
    while (*str) {
        if((int)*str == c) {
            if(intomax < max) {
                *str++ = '\0';
                while((int)*str == c) {
                    str++;
                }
                into[intomax++] = str;
            }
            else {
                return 0;
            }
        }
        else {
            str++;
        }
    }
    return intomax;
}
