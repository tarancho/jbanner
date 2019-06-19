/* -*- mode: c++; coding: utf-8; -*-
 * Copyright (C) 2003 Tadamegu Furukawa
 *
 * jbanner - 日本語を含む文字列を拡大し、バナーとして画面に表示します。
 *
 * $Id: main.c,v 1.1 2004/01/19 09:01:28 tfuruka1 Exp $
 * $Log: main.c,v $
 * Revision 1.1  2004/01/19 09:01:28  tfuruka1
 * リビジョン管理をRCSからCVSへ変更しました。
 *
 * Revision 1.4  2003/06/08 11:20:38  tfuruka1
 * ● フォントの列挙を行うオプションを追加した。
 * ● 表示幅のチェックに誤りがあったので修正した。
 *
 * Revision 1.3  2003/06/05 13:48:57  tfuruka1
 * ● -c オプションで「文字列」を指定できるように修正。
 *
 * Revision 1.2  2003/06/01 11:17:32  tfuruka1
 * コマンドライン解析のデバッグ用にSyslog出力を追加しました。
 *
 * Revision 1.1  2003/06/01 08:19:58  tfuruka1
 * Initial revision
 */
#include "jbanner.h"

/*
 * Cygwinのgccで、日本語を通す為に、以下の文字はコードで埋め込んでいま
 * す。
 *
 * 表=\x95\x5c
 * 十=\x8f\x5c
 * ソ=\x83\x5c
 *
 * ↑ 現在はソースをutf-8で記述し、実行時のエンコーディングを指定でき
 * るようになったので、上記の対応は不要になりました。
 */
#define USAGE_STR "Usage: %s "\
"[-c X] [-w 80] [--usage] [--version] [--help] 表示する文字列\n"

#define HELP_STR "Usage: %s 表示する文字列\n\n"\
"全体的なオプション\n"\
"  -c, --char=%s       使用する文字列 <%s>\n"\
"  -w, --width=%d     表示幅を設定します。<%d>\n\n"\
"ヘルプオプション\n"\
"  -?, --help         このヘルプメッセージを表示します。\n"\
"  --usage            簡潔な使用方法を表示します。\n"\
"  --version          ヴァージョン情報を表示します。\n\n"\
"特殊オプション(WIN32)\n"\
"  -F, --fontname=\"%s\"\n"\
"                     使用フォント名を指定します。<%s>\n"\
"  -f, --fontsize=%d  フォントサイズを指定します。<%d>\n"\
"  --lsfont[=family]  familyで指定したフォントファミリのフォント"\
"を列挙します。\n"\
"\nこのバージョンはCygwinのbannerと同じように動作するように作成しているつ\n"\
"もりですが、ソースは一切参照していませんので、実装手段は全く異なっ"\
"てい\n"\
"るでしょう。従って、細かい見た目の動作についても、異なる部分があると思\n"\
"われます。\n"\
"\nReport bugs to <tfuruka1@nifty.com>.\n"

/*
 * ヴァージョン情報を表示する
 */
static void
ShowVersion()
{
  fprintf(stderr, "%s\n", VERSION);
}

/*
 * 簡潔な使用方法を表示する
 */
static void ShowUsage(char *lpszCom)
{
    fprintf(stderr, USAGE_STR, GetLongBaseName(lpszCom));
}

/*
 * 使用方法を表示する
 */
static void ShowHelp(char *lpszCom)
{
    ShowVersion();
    fprintf(stderr, HELP_STR, GetLongBaseName(lpszCom),
            DEFAULT_CHAR, DEFAULT_CHAR, DEFAULT_WIDTH, DEFAULT_WIDTH,
            DEFAULT_FONT, DEFAULT_FONT, DEFAULT_FSIZE, DEFAULT_FSIZE);
}

/*
 * 引数が不十分な場合のえらーメッセージを表示後、簡潔な使用方法を表示する
 */
static void NotEnoughArguments(char *lpszCom)
{
    fprintf(stderr, "%s: 引数が不十分です。\n",
            GetLongBaseName(lpszCom));
    ShowUsage(lpszCom);
}

/*
 * 本コマンドのエントリ
 */
// 引数チェック用のマクロ：引数はargv[i]と決め打ちしているので要注
// 意！！！行が長くなるのが嫌だったので・・・
#define IS_ARG_OK1(Expected) (0 == strcmp(Expected, argv[i]))
#define IS_ARG_OK2(Exp1, Exp2) IS_ARG_OK1(Exp1) || IS_ARG_OK1(Exp2)
int main(int argc, char *argv[])
{
    int i;
    int nScreenWidth = DEFAULT_WIDTH;
    int nFontSize = DEFAULT_FSIZE;
    BOOL bDebug = FALSE;
    LPTSTR lpszUseChar = DEFAULT_CHAR;
    LPTSTR lpszFontName = DEFAULT_FONT;
    char *pNextArg;


    // コマンドオプションのデバッグ用です
    {
        Syslogger(FALSE, ">>>>>"
#if defined(CYGWIN)
                  "CYGWIN-VERSION"
#else
                  "MSVC-VERSION"
#endif
                  ": START");
        for (i = 0; i < argc; i++) {
            Syslogger(FALSE, "ARGV[%d]=<%s>", i, argv[i]);
        }
        Syslogger(FALSE, "<<<<<");
    }
    // コマンドオプションのデバッグ用はここまで

    for (i = 1; i < argc; i++) {
        if ('-' != *argv[i]) {
            break;
        }

        // オプションに=を使用していた場合の処理
        pNextArg = strchr(argv[i], '=');
        if (pNextArg) {
            if (!*(pNextArg)) {
                fprintf(stderr, "%s: オプションの値が存在しません。(%s)\n",
                        GetLongBaseName(argv[0]), argv[i]);
                return 1;
            }
            *pNextArg = '\0';
            pNextArg++;
        }

        // 引数のチェック
        if (IS_ARG_OK2("-?", "--help")) {
            ShowHelp(argv[0]);
            return 0;
        }
        else if (IS_ARG_OK1("--usage")) {
            ShowUsage(argv[0]);
            return 0;
        }
        else if (IS_ARG_OK1("--version")) {
            ShowVersion();
            return 0;
        }
        else if (IS_ARG_OK1("--lsfont")) {
            if (!pNextArg) {
                // 仮にこれが最後の引数だったとしても、次のポインタは
                // NULLなので、LsFontの引数としては辻褄があう。
                pNextArg = argv[i + 1];
            }
            LsFont(pNextArg);
            return 0;
        }
        else if (IS_ARG_OK2("-c", "--char")) {
            if (!pNextArg) {
                if (NULL == argv[i + 1]) {
                    NotEnoughArguments(argv[0]);
                    return 1;
                }
                i++;
                pNextArg = argv[i];
            }
            lpszUseChar = pNextArg;
        }
        else if (IS_ARG_OK2("-w", "--width")) {
            if (!pNextArg) {
                if (NULL == argv[i + 1]) {
                    NotEnoughArguments(argv[0]);
                    return 1;
                }
                i++;
                pNextArg = argv[i];
            }
            nScreenWidth = (int)strtol(pNextArg, NULL, 10);
            if (0 >= nScreenWidth) {
                fprintf(stderr, "%s: 表示幅が狭すぎます。<%d>\n",
                        GetLongBaseName(argv[0]), nScreenWidth);
                return 1;
            }
            if (MAX_WIDTH <= nScreenWidth) {
                fprintf(stderr, "%s: 表示幅が大きすぎます。<%d>\n",
                        GetLongBaseName(argv[0]), nScreenWidth);
                return 1;
            }
        }
        else if (IS_ARG_OK2("-F", "--fontname")) {
            if (!pNextArg) {
                if (NULL == argv[i + 1]) {
                    NotEnoughArguments(argv[0]);
                    return 1;
                }
                i++;
                pNextArg = argv[i];
            }
            lpszFontName = pNextArg;
        }
        else if (IS_ARG_OK2("-f", "--fontsize")) {
            if (!pNextArg) {
                if (NULL == argv[i + 1]) {
                    NotEnoughArguments(argv[0]);
                    return 1;
                }
                i++;
                pNextArg = argv[i];
            }
            nFontSize = (int)strtol(pNextArg, NULL, 10);
            if (0 >= nFontSize) {
                fprintf(stderr, "%s: フォントサイズが小さすぎます。<%d>\n",
                        GetLongBaseName(argv[0]), nFontSize);
                return 1;
            }
        }
        else if (IS_ARG_OK1("-d")) {
            // デバッグモードにする
            bDebug = TRUE;
        }
        else {
            fprintf(stderr, "%s: 無効なオプションです(%s)\n",
                    GetLongBaseName(argv[0]), argv[i]);
            return 1;
        }
    }

    if (i >= argc) {
        NotEnoughArguments(argv[0]);
        return 1;
    }

    // 使用文字列長に合わせてサイズを調整する
    nScreenWidth /= strlen(lpszUseChar);

    if (strlen(lpszUseChar) >= (ULONG)nScreenWidth) {
            fprintf(stderr, "%s: 表示幅より、"
                    "使用文字列長が長い為表示できません\n",
                    GetLongBaseName(argv[0]));
        return 1;
    }

    Syslogger(FALSE, "DBG: 使用文字=%s, 表示幅=%d, "
              "フォント=%s, フォントサイズ=%d",
              lpszUseChar, nScreenWidth, lpszFontName, nFontSize);

    // 使用フォントの初期化
    if (!SetLogFont(lpszFontName)) {
        fprintf(stderr, "%s: フォントファミリが選択できませんでした。\n",
                GetLongBaseName(argv[0]));
        return 1;
    }

    for (; i < argc; i++) {
        DrawBanner(lpszUseChar, nScreenWidth, nFontSize, argv[i], bDebug);
    }
    return 0;
}
