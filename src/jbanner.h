/* -*- mode: c++; coding: sjis-dos; -*-
 * Time-stamp: <2003-06-08 20:18:00 tfuruka1>
 * Copyright (C) 2003 Tadamegu Furukawa
 *
 * jbannerの唯一のヘッダファイル
 *
 * $Id: jbanner.h,v 1.1 2004/01/19 09:01:28 tfuruka1 Exp $
 * $Log: jbanner.h,v $
 * Revision 1.1  2004/01/19 09:01:28  tfuruka1
 * リビジョン管理をRCSからCVSへ変更しました。
 *
 * Revision 1.3  2003/06/08 11:18:45  tfuruka1
 * フォントの列挙関数追加に伴うプロトタイプの追加
 *
 * Revision 1.2  2003/06/05 13:51:35  tfuruka1
 * ● DrawBanner関数の修正に伴い、プロトタイプを修正
 *
 * Revision 1.1  2003/06/01 08:23:29  tfuruka1
 * Initial revision
 *
 */
#ifndef _JBANNER_H_
#define _JBANNER_H_

/*
 * Cygwinのgccで、日本語を通す為に、以下の文字はコードで埋め込んでいま
 * す。
 *
 * 表=\x95\x5c
 * ソ=\x83\x5c
 */
#define VERSION "jbanner version 1.0\n"\
"    Copyright 2002, 2003 Tadamegu Furukawa\n"\
"    日本語を含む文字列を拡大し、バナーとして画面に\x95\x5c示します。\n\n"\
"    この\x83\x5cフトウエアはフリー\x83\x5cフトウエアです。転載、"\
"頒布は入手したアー\n"\
"    カイブのままであれば、自由に行ってください。\n"\
"\n $Id: jbanner.h,v 1.1 2004/01/19 09:01:28 tfuruka1 Exp $\n"

#include <windows.h>
#include <stdio.h>

// 漢字のチェック
#define isKanjiSJIS(c)  (((c) >= 0x81 && (c) <= 0x9f) ||\
                         ((c) >= 0xe0 && (c) <= 0xfc))

#define DEFAULT_WIDTH 80                        // デフォルトの表示幅
#define DEFAULT_CHAR "X"                        // デフォルトの文字
#define DEFAULT_FONT TEXT("ＭＳ ゴシック")      // デフォルトのフォント
#define DEFAULT_FSIZE 16                        // デフォルトのフォントサイズ

#define MAX_WIDTH 1024                          // 最大表示幅

#define Z_SPACE "\x81\x40"                      // 全角スペース
// ----------------
// プロトタイプ宣言
// ----------------

// -- comm.c
VOID WINAPI
Syslogger(BOOL bStdOut,                         // T:stdoutにも出力
          LPCSTR lpstr,                         // 書式printfと同じ
          ...                                   // 引数
    );
LPCTSTR WINAPI GetLongBaseName(LPCTSTR lpszPath);
LPCSTR WINAPI GetLastErrorMessage(LPCSTR lpsz, DWORD dwErr);

// -- gdi_wrap.c
BOOL WINAPI DrawBanner(
    LPTSTR lpszUseChar,                         // 表示文字
    int ScreenWidth,                            // 表示幅
    int FontSize,                               // フォントサイズ
    LPTSTR lpszString,                          // 表示文字列
    BOOL bDebug                                 // T:デバッグ
    );
BOOL WINAPI SetLogFont(LPCTSTR lpszFamily);
VOID WINAPI LsFont(LPTSTR lpszFamily);

// -- string.c
LPTSTR TrimRightJa(LPTSTR lpszStr);
LPTSTR TrimRight(LPTSTR lpszStr);
#endif
