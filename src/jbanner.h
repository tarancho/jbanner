/* -*- mode: c++; coding: sjis-dos; -*-
 * Time-stamp: <2003-06-08 20:18:00 tfuruka1>
 * Copyright (C) 2003 Tadamegu Furukawa
 *
 * jbanner�̗B��̃w�b�_�t�@�C��
 *
 * $Id: jbanner.h,v 1.1 2004/01/19 09:01:28 tfuruka1 Exp $
 * $Log: jbanner.h,v $
 * Revision 1.1  2004/01/19 09:01:28  tfuruka1
 * ���r�W�����Ǘ���RCS����CVS�֕ύX���܂����B
 *
 * Revision 1.3  2003/06/08 11:18:45  tfuruka1
 * �t�H���g�̗񋓊֐��ǉ��ɔ����v���g�^�C�v�̒ǉ�
 *
 * Revision 1.2  2003/06/05 13:51:35  tfuruka1
 * �� DrawBanner�֐��̏C���ɔ����A�v���g�^�C�v���C��
 *
 * Revision 1.1  2003/06/01 08:23:29  tfuruka1
 * Initial revision
 *
 */
#ifndef _JBANNER_H_
#define _JBANNER_H_

/*
 * Cygwin��gcc�ŁA���{���ʂ��ׂɁA�ȉ��̕����̓R�[�h�Ŗ��ߍ���ł���
 * ���B
 *
 * �\=\x95\x5c
 * �\=\x83\x5c
 */
#define VERSION "jbanner version 1.0\n"\
"    Copyright 2002, 2003 Tadamegu Furukawa\n"\
"    ���{����܂ޕ�������g�債�A�o�i�[�Ƃ��ĉ�ʂ�\x95\x5c�����܂��B\n\n"\
"    ����\x83\x5c�t�g�E�G�A�̓t���[\x83\x5c�t�g�E�G�A�ł��B�]�ځA"\
"�Еz�͓��肵���A�[\n"\
"    �J�C�u�̂܂܂ł���΁A���R�ɍs���Ă��������B\n"\
"\n $Id: jbanner.h,v 1.1 2004/01/19 09:01:28 tfuruka1 Exp $\n"

#include <windows.h>
#include <stdio.h>

// �����̃`�F�b�N
#define isKanjiSJIS(c)  (((c) >= 0x81 && (c) <= 0x9f) ||\
                         ((c) >= 0xe0 && (c) <= 0xfc))

#define DEFAULT_WIDTH 80                        // �f�t�H���g�̕\����
#define DEFAULT_CHAR "X"                        // �f�t�H���g�̕���
#define DEFAULT_FONT TEXT("�l�r �S�V�b�N")      // �f�t�H���g�̃t�H���g
#define DEFAULT_FSIZE 16                        // �f�t�H���g�̃t�H���g�T�C�Y

#define MAX_WIDTH 1024                          // �ő�\����

#define Z_SPACE "\x81\x40"                      // �S�p�X�y�[�X
// ----------------
// �v���g�^�C�v�錾
// ----------------

// -- comm.c
VOID WINAPI
Syslogger(BOOL bStdOut,                         // T:stdout�ɂ��o��
          LPCSTR lpstr,                         // ����printf�Ɠ���
          ...                                   // ����
    );
LPCTSTR WINAPI GetLongBaseName(LPCTSTR lpszPath);
LPCSTR WINAPI GetLastErrorMessage(LPCSTR lpsz, DWORD dwErr);

// -- gdi_wrap.c
BOOL WINAPI DrawBanner(
    LPTSTR lpszUseChar,                         // �\������
    int ScreenWidth,                            // �\����
    int FontSize,                               // �t�H���g�T�C�Y
    LPTSTR lpszString,                          // �\��������
    BOOL bDebug                                 // T:�f�o�b�O
    );
BOOL WINAPI SetLogFont(LPCTSTR lpszFamily);
VOID WINAPI LsFont(LPTSTR lpszFamily);

// -- string.c
LPTSTR TrimRightJa(LPTSTR lpszStr);
LPTSTR TrimRight(LPTSTR lpszStr);
#endif
