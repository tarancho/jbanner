/* -*- mode: c++; coding: sjis-dos; -*-
 * Time-stamp: <2003-06-01 15:28:34 tfuruka1>
 * Copyright (C) 2003 Tadamegu Furukawa
 *
 *  �����񑀍�֐�
 *
 * $Id: string.c,v 1.1 2004/01/19 09:01:27 tfuruka1 Exp $
 * $Log: string.c,v $
 * Revision 1.1  2004/01/19 09:01:27  tfuruka1
 * ���r�W�����Ǘ���RCS����CVS�֕ύX���܂����B
 *
 */
// (replace-regexp "/\\*\\(.+\\)\\*/" "//\\1")
// (replace-regexp "[ \t]+$" "")

#include <windows.h>
#include <stdio.h>

#define Z_SPACE "\x81\x40"                      // �S�p�X�y�[�X

/* -------------------------------------------------------------------
 * ������̉E���̃X�y�[�X��������菜��
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
 * ������̉E���̑S�p�X�y�[�X��������菜��
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
 * ������̍����̃X�y�[�X��������菜��
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

    if (pFrom == pTo) {                         // �]���ȕ����͖���
        return lpszStr;
    }
    for (; *pFrom; pFrom++, pTo++) {
        *pTo = *pFrom;
    }
    *pTo = '\0';
    return lpszStr;
}

/* -------------------------------------------------------------------
 * ������̍��E�̃X�y�[�X��������菜��
 * *-----------------------------------------------------------------*/
LPTSTR TrimString(LPSTR lpszStr)
{
    return TrimLeft(TrimRight(lpszStr));
}

/* -------------------------------------------------------------------
 * fgets�Ɠ��l�̏������s�����A�^�u���X�y�[�X������֓W�J����B�A���A�^
 * �u�̓W�J��r���Ŏ~�߂鎖�͖����̂ŁA�ő啶���񒷂̓^�u�X�g�b�v���l
 * ������K�v������B
 * *-----------------------------------------------------------------*/
LPTSTR
ufgets(
    LPTSTR szBuf,                               // �i�[�o�b�t�@
    int cbMax,                                  // �ő啶����
    FILE *fp,                                   // �t�@�C���|�C���^
    int nTab                                    // �^�u�X�g�b�v
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
 * str�̕��������؂蕶��c�ŋ�؂�A��؂�ꂽ������̐擪�A�h���X��
 * ���ꂼ��*into�Ɋi�[����B�A��max�Ŏw�肳�ꂽ����葽���͋�؂�Ȃ��B
 * �A��������؂蕶����1�ƌ���B
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
