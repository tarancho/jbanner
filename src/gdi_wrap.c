/* -*- mode: c++; coding: sjis-dos; -*-
 * Time-stamp: <2003-06-07 21:59:35 tfuruka1>
 * Copyright (C) 2003 Tadamegu Furukawa
 *
 * gdi�Ɋ֘A����֐� - ���ۂɂ̓t�H���g�����������݂��Ă��Ȃ����E�E�E
 *
 * $Id: gdi_wrap.c,v 1.1 2004/01/19 09:01:28 tfuruka1 Exp $
 * $Log: gdi_wrap.c,v $
 * Revision 1.1  2004/01/19 09:01:28  tfuruka1
 * ���r�W�����Ǘ���RCS����CVS�֕ύX���܂����B
 *
 * Revision 1.3  2003/06/08 11:22:10  tfuruka1
 * �t�H���g��񋓂���֐���ǉ������B
 *
 * Revision 1.2  2003/06/05 13:50:40  tfuruka1
 * �� -c �I�v�V�����ŕ�������ł���悤�ɏC���������ɔ����C���B
 * �� �t�H���g�ŎΆA�{�[���h�A���������̂܂ܔ��f����悤�ɏC���B
 *
 * Revision 1.1  2003/06/01 08:22:11  tfuruka1
 * Initial revision
 *
 */

#include "jbanner.h"

/*
 * SDK32:�R���\�[���E�B���h�E�̃n���h���擾
 *
 * �{�֐��͈ȉ��̋L�����Q�l�ɂ��č쐬���܂����B
 *
 * �ŏI�X�V��: 1999/02/09
 * �����ԍ�: J046738  
 * 
 * ���̎����͈ȉ��ɂ��ċL�q�������̂ł��B
 * 
 * Microsoft(R) Win32(R) Software Development Kit (SDK)
 * 
 * ���̎����́A�č� Microsoft Corporation ����񋟂���Ă��� Knowledge
 * Base �� Article ID Q124103 (�ŏI�X�V�� 1988-12-23) �����Ƃɍ쐬����
 * ���̂ł��B
 */
#define MY_BUFSIZE 1024                         // �R���\�[���̃^�C�g���p
HWND GetConsoleHwnd(VOID)
{
    HWND hwndFound;
    char pszNewWindowTitle[MY_BUFSIZE];
    char pszOldWindowTitle[MY_BUFSIZE];
    int i;

    // �R���\�[���^�C�g���̎擾
    GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

    // �Ǝ��ɁA�E�B���h�E�̐V�K�^�C�g�����t�H�[�}�b�g���܂�
    wsprintf(pszNewWindowTitle,"%d/%d-%s",
             GetTickCount(),
             GetCurrentProcessId(),
             pszOldWindowTitle);

    // ���݂̃E�B���h�E�^�C�g����ύX���܂�
    SetConsoleTitle(pszNewWindowTitle);

    for (i = 0; i < 100; i++) {
        // �E�B���h�E�̐V�K�^�C�g����T���ɂ����܂�
        hwndFound = FindWindow(NULL, pszNewWindowTitle);
        if (hwndFound) {
            Syslogger(FALSE, "GetConsoleHwnd(): ��=%d, T=%s",
                      i, pszNewWindowTitle);
            break;                              // ��������
        }
        Sleep(10);                              // 10m Wait
    }

    // ���̃E�B���h�E�^�C�g���֖߂��܂�
    SetConsoleTitle(pszOldWindowTitle);

    return hwndFound;
}


/*
 * �t�H���g�֘A�̏����i���`��B�����������ɂ�BC++�ŃN���X������������
 * �������ȁ`�j
 */
static LOGFONT lft = {-13, 0, 0, 0, 800, 0, 0, 0, SHIFTJIS_CHARSET,
                      3, 2, 1, 49, TEXT("�l�r �S�V�b�N")};
static int numCallBack = 0;
static int CALLBACK
EnumFontFamProc(
    ENUMLOGFONT FAR *lpelf,             // pointer to logical-font data
    NEWTEXTMETRIC FAR *lpntm,           // pointer to physical-font data
    int FontType,                       // type of font
    LPARAM lParam                       // address of application-defined data
    )
{
    // �f�[�^�����݂��Ȃ��ꍇ�͏������Ȃ�
    if (!lpelf) {
        return 0;
    }

    // ------------------------------
    // �t�H���g�̍쐬�v���̏ꍇ�̏���
    // ------------------------------
    if (0 == lParam) {
        // �ŏ��ȊO�̌Ăяo���̏ꍇ�͏������Ȃ�
        if (numCallBack) {
            return 0;
        }
        numCallBack++;

        Syslogger(FALSE, "�I���t�H���g�t�@�~��: %s, %s",
                  lpelf->elfFullName,
                  (FontType == DEVICE_FONTTYPE) ? "Device-Font"
                  : ((FontType == RASTER_FONTTYPE) ? "Raster-Font"
                     : "TrueType-Font"));

        memcpy(&lft, &lpelf->elfLogFont, sizeof(LOGFONT));
    }
    else {
        // ------------------------
        // �t�H���g�̈ꗗ�\���̏ꍇ
        // ------------------------

        // �t�H���g�̈�ӂȖ��O, �t�H���g�̃X�^�C��, �t�H���g�̏��̖�
        // ��\������
        printf("\"%s\"\t\"%s\"\t\"%s\"\n", lpelf->elfFullName,
               lpelf->elfStyle, lpelf->elfLogFont.lfFaceName);
    }
    return 1;
}

/*
 * �t�H���g�̈ꗗ��\������
 */
VOID WINAPI
LsFont(LPTSTR lpszFamily)
{
    HDC hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
    EnumFontFamilies(hDC, lpszFamily, (FONTENUMPROC)EnumFontFamProc, 1);
    DeleteDC(hDC);
}
/*
 * �t�H���g�t�@�~��������f�t�H���g�̘_���t�H���g���쐬����
 */
BOOL WINAPI SetLogFont(LPCTSTR lpszFamily)
{
    HDC hDC = CreateDC("DISPLAY", NULL, NULL, NULL);

    numCallBack = 0;
    EnumFontFamilies(hDC, lpszFamily, (FONTENUMPROC)EnumFontFamProc, 0);
    DeleteDC(hDC);
    return numCallBack;
}

/* 
 * �t�H���g���쐬����B�t�H���g�t�@�~�����ύX�ɂȂ�ꍇ�́A�\�� 
 * SetLogFont�֐��Ř_���t�H���g���������������K�v������܂��B
 */
static HFONT WINAPI
CreateBannerFont(int nHeight)
{

    LOGFONT lf;

    memcpy(&lf, &lft, sizeof(LOGFONT));
    lf.lfHeight = nHeight;
    lf.lfWidth = 0;

    return CreateFontIndirect(&lf);
}

/*
 * �o�i�[��`�悷��B�Ȃ�炩�̃G���[�����������ꍇ�́A0(FALSE)��ԋp
 * ���A����ɏI�������ꍇ�́A0�ȊO(TRUE)��ԋp���܂��B
 */
BOOL WINAPI
DrawBanner(
    LPTSTR lpszUseChar,                         // �\������
    int nScreenWidth,                           // �\����
    int nFontSize,                              // �t�H���g�T�C�Y
    LPTSTR lpszString,                          // �\��������
    BOOL bDebug                                 // T:�f�o�b�O
    )
{
    HWND hWnd;                                  // �E�C���h�E�n���h��
    HFONT hFont;                                // �t�H���g
    HFONT hOldFont;                             // �Â��t�H���g
    RECT rc;                                    // �`�搈
    HDC hDC;                                    // �f�o�C�X�R���e�L�X�g
    HDC hMemDC;                                 // �f�o�C�X�R���e�L�X�g�i���z�j
    HBITMAP hBitMap;                            // �r�b�g�}�b�v
    HBITMAP hOldBitmap;                         // �Â��r�b�g�}�b�v

    BYTE szLine[MAX_WIDTH];                     // �o�i�[�̈�s
    LPTSTR lpszBgChar;                          // �w�i�̕���

    int x, y, ht;                               // ���ƍ���
    int i;                                      // �ėp

    // �w�i�̕�����ݒ肷��
    if (!(lpszBgChar = malloc(strlen(lpszUseChar + 1)))) {
        fprintf(stderr, "�������̊m�ۂɎ��s[%s:%d]", __FILE__, __LINE__);
        return FALSE;
    }

    for (i = 0; (ULONG)i < strlen(lpszUseChar); i++) {
        if (isKanjiSJIS(*((LPBYTE)lpszUseChar + i))) {
            strcpy(lpszBgChar + i, Z_SPACE);
            i++;
        }
        else {
            strcpy(lpszBgChar + i, " ");
        }
    }

    // �E�C���h�E�n���h���̎擾
    if (!(hWnd = GetConsoleHwnd())) {
        fprintf(stderr, "�E�C���h�E�n���h���̎擾�Ɏ��s���܂���\n");
        return FALSE;
    }

    // �`�搈�̐ݒ�
    rc.top = rc.left = 0;
    rc.right = nScreenWidth;
    rc.bottom = nFontSize * 2;                  // ��������g�Ƃ��̈�2�{

    // �f�o�C�X�R���e�L�X�g�̎擾
    if (!(hDC = GetDC(hWnd))) {
        fprintf(stderr, "�f�o�C�X�R���e�L�X�g�̎擾�Ɏ��s���܂����B\n");
        return FALSE;
    }

    // ���z�f�o�C�X�R���e�L�X�g���쐬����
    if (!(hMemDC = CreateCompatibleDC(hDC))) {
        fprintf(stderr, "%s\n",
                GetLastErrorMessage("CreateCompatibleDC",
                                    GetLastError()));
        ReleaseDC(hWnd, hDC);
        return FALSE;
    }

    // �r�b�g�}�b�v�̍쐬
    if (!(hBitMap = CreateBitmap(nScreenWidth, rc.bottom, 1, 1, NULL))) {
        fprintf(stderr, "�r�b�g�}�b�v�̍쐬�Ɏ��s���܂���\n");
        DeleteDC(hMemDC);                       // �f�o�C�X�R���e�L�X�g�폜
        ReleaseDC(hWnd, hDC);                   // �f�o�C�X�R���e�L�X�g�̊J��
        return FALSE;
    }

    // �r�b�g�}�b�v�̑I��
    if (!(hOldBitmap = SelectObject(hMemDC, hBitMap))) {
        fprintf(stderr, "�r�b�g�}�b�v�̑I���Ɏ��s���܂����B\n");
        DeleteDC(hMemDC);                       // �f�o�C�X�R���e�L�X�g�폜
        DeleteObject(hBitMap);                  // �r�b�g�}�b�v�폜
        ReleaseDC(hWnd, hDC);                   // �f�o�C�X�R���e�L�X�g�̊J��
        return FALSE;
    }

    // �t�H���g�̍쐬
    if (!(hFont = CreateBannerFont(-nFontSize))) {
        fprintf(stderr, "%s\n",
                GetLastErrorMessage("CreateFont", GetLastError()));
        SelectObject(hMemDC, hOldBitmap);       // �r�b�g�}�b�v��߂�
        DeleteDC(hMemDC);                       // �f�o�C�X�R���e�L�X�g�폜
        DeleteObject(hBitMap);                  // �r�b�g�}�b�v�̍폜
        ReleaseDC(hWnd, hDC);                   // �f�o�C�X�R���e�L�X�g�̊J��
        return FALSE;
    }

    // �t�H���g�̑I��
    if (!(hOldFont = SelectObject(hMemDC, hFont))) {
        fprintf(stderr, "�I�u�W�F�N�g�̑I��(FONT)�Ɏ��s���܂����B\n");
        DeleteObject(hFont);                    // �t�H���g�̍폜
        SelectObject(hMemDC, hOldBitmap);       // �r�b�g�}�b�v��߂�
        DeleteDC(hMemDC);                       // �f�o�C�X�R���e�L�X�g�폜
        DeleteObject(hBitMap);                  // �r�b�g�}�b�v�̍폜
        ReleaseDC(hWnd, hDC);                   // �f�o�C�X�R���e�L�X�g�J��
        return FALSE;
    }

    // �t�H���g�̐F�̐ݒ�
    SetTextColor(hMemDC, RGB(255, 255, 255));
    SetBkColor(hMemDC, RGB(0, 0, 0));

    // �`��
    ht = DrawText(hMemDC, lpszString, -1, &rc,
                  DT_LEFT | DT_TOP | DT_SINGLELINE);

    // �f�o�b�O���[�h�̏ꍇ�̓R���\�[���ɓ]������
    if (bDebug) {
        BitBlt(hDC, 0, 0, rc.right, rc.bottom, hMemDC, 0, 0, SRCCOPY);
    }

    // �o�i�[�ɕϊ�����
    for (y = 0; y < ht; y++) {
        szLine[0] = '\0';
        for (x = 0; x < nScreenWidth; x++) {
            strcat(szLine, (0 == GetPixel(hMemDC, x, y))
                   ? lpszBgChar : lpszUseChar);
        }
        printf("%s\n", TrimRightJa(TrimRight(szLine)));
    }

    // ��n��
    SelectObject(hMemDC, hOldFont);             // �t�H���g��߂�
    SelectObject(hMemDC, hOldBitmap);           // �r�b�g�}�b�v��߂�
    ReleaseDC(hWnd, hDC);                       // �f�o�C�X�R���e�L�X�g�J��
    DeleteObject(hFont);                        // �t�H���g�폜
    DeleteObject(hBitMap);                      // �r�b�g�}�b�v�폜
    DeleteDC(hMemDC);                           // ���zDC�폜

    return TRUE;
}

#if 0
int main(void)
{
    HWND hWnd = GetConsoleHwnd();
    printf("DBG:TEST:GetConsoleHwnd(): hWnd = %x\n", hWnd);
}
#endif
