/* -*- mode: c++; coding: sjis-dos; -*-
 * Time-stamp: <2003-06-07 21:59:35 tfuruka1>
 * Copyright (C) 2003 Tadamegu Furukawa
 *
 * gdiに関連する関数 - 実際にはフォント部分しか存在していないが・・・
 *
 * $Id: gdi_wrap.c,v 1.1 2004/01/19 09:01:28 tfuruka1 Exp $
 * $Log: gdi_wrap.c,v $
 * Revision 1.1  2004/01/19 09:01:28  tfuruka1
 * リビジョン管理をRCSからCVSへ変更しました。
 *
 * Revision 1.3  2003/06/08 11:22:10  tfuruka1
 * フォントを列挙する関数を追加した。
 *
 * Revision 1.2  2003/06/05 13:50:40  tfuruka1
 * ● -c オプションで文字列をできるように修正した事に伴う修正。
 * ● フォントで斜体、ボールド、中線をそのまま反映するように修正。
 *
 * Revision 1.1  2003/06/01 08:22:11  tfuruka1
 * Initial revision
 *
 */

#include "jbanner.h"

/*
 * SDK32:コンソールウィンドウのハンドル取得
 *
 * 本関数は以下の記事を参考にして作成しました。
 *
 * 最終更新日: 1999/02/09
 * 文書番号: J046738  
 * 
 * この資料は以下について記述したものです。
 * 
 * Microsoft(R) Win32(R) Software Development Kit (SDK)
 * 
 * この資料は、米国 Microsoft Corporation から提供されている Knowledge
 * Base の Article ID Q124103 (最終更新日 1988-12-23) をもとに作成した
 * ものです。
 */
#define MY_BUFSIZE 1024                         // コンソールのタイトル用
HWND GetConsoleHwnd(VOID)
{
    HWND hwndFound;
    char pszNewWindowTitle[MY_BUFSIZE];
    char pszOldWindowTitle[MY_BUFSIZE];
    int i;

    // コンソールタイトルの取得
    GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

    // 独自に、ウィンドウの新規タイトルをフォーマットします
    wsprintf(pszNewWindowTitle,"%d/%d-%s",
             GetTickCount(),
             GetCurrentProcessId(),
             pszOldWindowTitle);

    // 現在のウィンドウタイトルを変更します
    SetConsoleTitle(pszNewWindowTitle);

    for (i = 0; i < 100; i++) {
        // ウィンドウの新規タイトルを探しにいきます
        hwndFound = FindWindow(NULL, pszNewWindowTitle);
        if (hwndFound) {
            Syslogger(FALSE, "GetConsoleHwnd(): 回数=%d, T=%s",
                      i, pszNewWindowTitle);
            break;                              // 見つかった
        }
        Sleep(10);                              // 10m Wait
    }

    // 元のウィンドウタイトルへ戻します
    SetConsoleTitle(pszOldWindowTitle);

    return hwndFound;
}


/*
 * フォント関連の処理（う～ん。汚い処理だにゃ。C++でクラス化した方が良
 * かったな～）
 */
static LOGFONT lft = {-13, 0, 0, 0, 800, 0, 0, 0, SHIFTJIS_CHARSET,
                      3, 2, 1, 49, TEXT("ＭＳ ゴシック")};
static int numCallBack = 0;
static int CALLBACK
EnumFontFamProc(
    ENUMLOGFONT FAR *lpelf,             // pointer to logical-font data
    NEWTEXTMETRIC FAR *lpntm,           // pointer to physical-font data
    int FontType,                       // type of font
    LPARAM lParam                       // address of application-defined data
    )
{
    // データが存在しない場合は処理しない
    if (!lpelf) {
        return 0;
    }

    // ------------------------------
    // フォントの作成要求の場合の処理
    // ------------------------------
    if (0 == lParam) {
        // 最初以外の呼び出しの場合は処理しない
        if (numCallBack) {
            return 0;
        }
        numCallBack++;

        Syslogger(FALSE, "選択フォントファミリ: %s, %s",
                  lpelf->elfFullName,
                  (FontType == DEVICE_FONTTYPE) ? "Device-Font"
                  : ((FontType == RASTER_FONTTYPE) ? "Raster-Font"
                     : "TrueType-Font"));

        memcpy(&lft, &lpelf->elfLogFont, sizeof(LOGFONT));
    }
    else {
        // ------------------------
        // フォントの一覧表示の場合
        // ------------------------

        // フォントの一意な名前, フォントのスタイル, フォントの書体名
        // を表示する
        printf("\"%s\"\t\"%s\"\t\"%s\"\n", lpelf->elfFullName,
               lpelf->elfStyle, lpelf->elfLogFont.lfFaceName);
    }
    return 1;
}

/*
 * フォントの一覧を表示する
 */
VOID WINAPI
LsFont(LPTSTR lpszFamily)
{
    HDC hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
    EnumFontFamilies(hDC, lpszFamily, (FONTENUMPROC)EnumFontFamProc, 1);
    DeleteDC(hDC);
}
/*
 * フォントファミリ名からデフォルトの論理フォントを作成する
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
 * フォントを作成する。フォントファミリが変更になる場合は、予め 
 * SetLogFont関数で論理フォントを初期化し直す必要があります。
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
 * バナーを描画する。なんらかのエラーが発生した場合は、0(FALSE)を返却
 * し、正常に終了した場合は、0以外(TRUE)を返却します。
 */
BOOL WINAPI
DrawBanner(
    LPTSTR lpszUseChar,                         // 表示文字
    int nScreenWidth,                           // 表示幅
    int nFontSize,                              // フォントサイズ
    LPTSTR lpszString,                          // 表示文字列
    BOOL bDebug                                 // T:デバッグ
    )
{
    HWND hWnd;                                  // ウインドウハンドル
    HFONT hFont;                                // フォント
    HFONT hOldFont;                             // 古いフォント
    RECT rc;                                    // 描画粋
    HDC hDC;                                    // デバイスコンテキスト
    HDC hMemDC;                                 // デバイスコンテキスト（仮想）
    HBITMAP hBitMap;                            // ビットマップ
    HBITMAP hOldBitmap;                         // 古いビットマップ

    BYTE szLine[MAX_WIDTH];                     // バナーの一行
    LPTSTR lpszBgChar;                          // 背景の文字

    int x, y, ht;                               // 幅と高さ
    int i;                                      // 汎用

    // 背景の文字を設定する
    if (!(lpszBgChar = malloc(strlen(lpszUseChar + 1)))) {
        fprintf(stderr, "メモリの確保に失敗[%s:%d]", __FILE__, __LINE__);
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

    // ウインドウハンドルの取得
    if (!(hWnd = GetConsoleHwnd())) {
        fprintf(stderr, "ウインドウハンドルの取得に失敗しました\n");
        return FALSE;
    }

    // 描画粋の設定
    rc.top = rc.left = 0;
    rc.right = nScreenWidth;
    rc.bottom = nFontSize * 2;                  // 小文字のgとかの為2倍

    // デバイスコンテキストの取得
    if (!(hDC = GetDC(hWnd))) {
        fprintf(stderr, "デバイスコンテキストの取得に失敗しました。\n");
        return FALSE;
    }

    // 仮想デバイスコンテキストを作成する
    if (!(hMemDC = CreateCompatibleDC(hDC))) {
        fprintf(stderr, "%s\n",
                GetLastErrorMessage("CreateCompatibleDC",
                                    GetLastError()));
        ReleaseDC(hWnd, hDC);
        return FALSE;
    }

    // ビットマップの作成
    if (!(hBitMap = CreateBitmap(nScreenWidth, rc.bottom, 1, 1, NULL))) {
        fprintf(stderr, "ビットマップの作成に失敗しました\n");
        DeleteDC(hMemDC);                       // デバイスコンテキスト削除
        ReleaseDC(hWnd, hDC);                   // デバイスコンテキストの開放
        return FALSE;
    }

    // ビットマップの選択
    if (!(hOldBitmap = SelectObject(hMemDC, hBitMap))) {
        fprintf(stderr, "ビットマップの選択に失敗しました。\n");
        DeleteDC(hMemDC);                       // デバイスコンテキスト削除
        DeleteObject(hBitMap);                  // ビットマップ削除
        ReleaseDC(hWnd, hDC);                   // デバイスコンテキストの開放
        return FALSE;
    }

    // フォントの作成
    if (!(hFont = CreateBannerFont(-nFontSize))) {
        fprintf(stderr, "%s\n",
                GetLastErrorMessage("CreateFont", GetLastError()));
        SelectObject(hMemDC, hOldBitmap);       // ビットマップを戻す
        DeleteDC(hMemDC);                       // デバイスコンテキスト削除
        DeleteObject(hBitMap);                  // ビットマップの削除
        ReleaseDC(hWnd, hDC);                   // デバイスコンテキストの開放
        return FALSE;
    }

    // フォントの選択
    if (!(hOldFont = SelectObject(hMemDC, hFont))) {
        fprintf(stderr, "オブジェクトの選択(FONT)に失敗しました。\n");
        DeleteObject(hFont);                    // フォントの削除
        SelectObject(hMemDC, hOldBitmap);       // ビットマップを戻す
        DeleteDC(hMemDC);                       // デバイスコンテキスト削除
        DeleteObject(hBitMap);                  // ビットマップの削除
        ReleaseDC(hWnd, hDC);                   // デバイスコンテキスト開放
        return FALSE;
    }

    // フォントの色の設定
    SetTextColor(hMemDC, RGB(255, 255, 255));
    SetBkColor(hMemDC, RGB(0, 0, 0));

    // 描画
    ht = DrawText(hMemDC, lpszString, -1, &rc,
                  DT_LEFT | DT_TOP | DT_SINGLELINE);

    // デバッグモードの場合はコンソールに転送する
    if (bDebug) {
        BitBlt(hDC, 0, 0, rc.right, rc.bottom, hMemDC, 0, 0, SRCCOPY);
    }

    // バナーに変換する
    for (y = 0; y < ht; y++) {
        szLine[0] = '\0';
        for (x = 0; x < nScreenWidth; x++) {
            strcat(szLine, (0 == GetPixel(hMemDC, x, y))
                   ? lpszBgChar : lpszUseChar);
        }
        printf("%s\n", TrimRightJa(TrimRight(szLine)));
    }

    // 後始末
    SelectObject(hMemDC, hOldFont);             // フォントを戻す
    SelectObject(hMemDC, hOldBitmap);           // ビットマップを戻す
    ReleaseDC(hWnd, hDC);                       // デバイスコンテキスト開放
    DeleteObject(hFont);                        // フォント削除
    DeleteObject(hBitMap);                      // ビットマップ削除
    DeleteDC(hMemDC);                           // 仮想DC削除

    return TRUE;
}

#if 0
int main(void)
{
    HWND hWnd = GetConsoleHwnd();
    printf("DBG:TEST:GetConsoleHwnd(): hWnd = %x\n", hWnd);
}
#endif
