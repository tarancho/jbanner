/* -*- mode: c++; coding: sjis-dos; -*-
 * Time-stamp: <2003-06-08 17:35:02 tfuruka1>
 * Copyright (C) 2003 Tadamegu Furukawa
 *
 * jbanner - ���{����܂ޕ�������g�債�A�o�i�[�Ƃ��ĉ�ʂɕ\�����܂��B
 *
 * $Id: main.c,v 1.1 2004/01/19 09:01:28 tfuruka1 Exp $
 * $Log: main.c,v $
 * Revision 1.1  2004/01/19 09:01:28  tfuruka1
 * ���r�W�����Ǘ���RCS����CVS�֕ύX���܂����B
 *
 * Revision 1.4  2003/06/08 11:20:38  tfuruka1
 * �� �t�H���g�̗񋓂��s���I�v�V������ǉ������B
 * �� �\�����̃`�F�b�N�Ɍ�肪�������̂ŏC�������B
 *
 * Revision 1.3  2003/06/05 13:48:57  tfuruka1
 * �� -c �I�v�V�����Łu������v���w��ł���悤�ɏC���B
 *
 * Revision 1.2  2003/06/01 11:17:32  tfuruka1
 * �R�}���h���C����͂̃f�o�b�O�p��Syslog�o�͂�ǉ����܂����B
 *
 * Revision 1.1  2003/06/01 08:19:58  tfuruka1
 * Initial revision
 */
#include "jbanner.h"

/*
 * Cygwin��gcc�ŁA���{���ʂ��ׂɁA�ȉ��̕����̓R�[�h�Ŗ��ߍ���ł���
 * ���B
 *
 * �\=\x95\x5c
 * �\=\x8f\x5c
 * �\=\x83\x5c
 */
#define USAGE_STR "Usage: %s "\
"[-c X] [-w 80] [--usage] [--version] [--help] \x95\x5c�����镶����\n"

#define HELP_STR "Usage: %s \x95\x5c�����镶����\n\n"\
"�S�̓I�ȃI�v�V����\n"\
"  -c, --char=%s       �g�p���镶���� <%s>\n"\
"  -w, --width=%d     \x95\x5c������ݒ肵�܂��B<%d>\n\n"\
"�w���v�I�v�V����\n"\
"  -?, --help         ���̃w���v���b�Z�[�W��\x95\x5c�����܂��B\n"\
"  --usage            �Ȍ��Ȏg�p���@��\x95\x5c�����܂��B\n"\
"  --version          ���@�[�W��������\x95\x5c�����܂��B\n\n"\
"����I�v�V����(WIN32)\n"\
"  -F, --fontname=\"%s\"\n"\
"                     �g�p�t�H���g�����w�肵�܂��B<%s>\n"\
"  -f, --fontsize=%d  �t�H���g�T�C�Y���w�肵�܂��B<%d>\n"\
"  --lsfont[=family]  family�Ŏw�肵���t�H���g�t�@�~���̃t�H���g"\
"��񋓂��܂��B\n"\
"\n���̃o�[�W������Cygwin��banner�Ɠ����悤�ɓ��삷��悤�ɍ쐬���Ă����\n"\
"����ł����A\x83\x5c�[�X�͈�؎Q�Ƃ��Ă��܂���̂ŁA������i�͑S���قȂ�"\
"�Ă�\n"\
"��ł��傤�B�]���āA�ׂ��������ڂ̓���ɂ��Ă��A�قȂ镔��������Ǝv\n"\
"���܂��B\n"\
"\nReport bugs to <tfuruka1@nifty.com>.\n"

/*
 * ���@�[�W��������\������
 */
static void
ShowVersion()
{
  fprintf(stderr, "%s\n", VERSION);
}

/*
 * �Ȍ��Ȏg�p���@��\������
 */
static void ShowUsage(char *lpszCom)
{
    fprintf(stderr, USAGE_STR, GetLongBaseName(lpszCom));
}

/*
 * �g�p���@��\������
 */
static void ShowHelp(char *lpszCom)
{
    ShowVersion();
    fprintf(stderr, HELP_STR, GetLongBaseName(lpszCom),
            DEFAULT_CHAR, DEFAULT_CHAR, DEFAULT_WIDTH, DEFAULT_WIDTH,
            DEFAULT_FONT, DEFAULT_FONT, DEFAULT_FSIZE, DEFAULT_FSIZE);
}

/*
 * �������s�\���ȏꍇ�̂���[���b�Z�[�W��\����A�Ȍ��Ȏg�p���@��\������
 */
static void NotEnoughArguments(char *lpszCom)
{
    fprintf(stderr, "%s: �������s\x8f\x5c���ł��B\n",
            GetLongBaseName(lpszCom));
    ShowUsage(lpszCom);
}

/*
 * �{�R�}���h�̃G���g��
 */ 
// �����`�F�b�N�p�̃}�N���F������argv[i]�ƌ��ߑł����Ă���̂ŗv��
// �ӁI�I�I�s�������Ȃ�̂����������̂ŁE�E�E
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
    BYTE *pNextArg;


    // �R�}���h�I�v�V�����̃f�o�b�O�p�ł�
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
    // �R�}���h�I�v�V�����̃f�o�b�O�p�͂����܂�

    for (i = 1; i < argc; i++) {
        if ('-' != *argv[i]) {
            break;
        }

        // �I�v�V������=���g�p���Ă����ꍇ�̏���
        pNextArg = strchr(argv[i], '=');
        if (pNextArg) {
            if (!*(pNextArg)) {
                fprintf(stderr, "%s: �I�v�V�����̒l�����݂��܂���B(%s)\n",
                        GetLongBaseName(argv[0]), argv[i]);
                return 1;
            }
            *pNextArg = '\0';
            pNextArg++;
        }

        // �����̃`�F�b�N
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
                // ���ɂ��ꂪ�Ō�̈����������Ƃ��Ă��A���̃|�C���^��
                // NULL�Ȃ̂ŁALsFont�̈����Ƃ��Ă͒��낪�����B
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
                fprintf(stderr, "%s: \x95\x5c�������������܂��B<%d>\n",
                        GetLongBaseName(argv[0]), nScreenWidth);
                return 1;
            }
            if (MAX_WIDTH <= nScreenWidth) {
                fprintf(stderr, "%s: \x95\x5c�������傫�����܂��B<%d>\n",
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
                fprintf(stderr, "%s: �t�H���g�T�C�Y�����������܂��B<%d>\n",
                        GetLongBaseName(argv[0]), nFontSize);
                return 1;
            }
        }
        else if (IS_ARG_OK1("-d")) {
            // �f�o�b�O���[�h�ɂ���
            bDebug = TRUE;
        }
        else {
            fprintf(stderr, "%s: �����ȃI�v�V�����ł�(%s)\n",
                    GetLongBaseName(argv[0]), argv[i]);
            return 1;
        }
    }

    if (i >= argc) {
        NotEnoughArguments(argv[0]);
        return 1;
    }

    // �g�p�����񒷂ɍ��킹�ăT�C�Y�𒲐�����
    nScreenWidth /= strlen(lpszUseChar);

    if (strlen(lpszUseChar) >= (ULONG)nScreenWidth) {
            fprintf(stderr, "%s: \x95\x5c�������A"
                    "�g�p�����񒷂�������\x95\x5c���ł��܂���\n",
                    GetLongBaseName(argv[0]));
        return 1;
    }

    Syslogger(FALSE, "DBG: �g�p����=%s, \x95\x5c����=%d, "
              "�t�H���g=%s, �t�H���g�T�C�Y=%d",
              lpszUseChar, nScreenWidth, lpszFontName, nFontSize);

    // �g�p�t�H���g�̏�����
    if (!SetLogFont(lpszFontName)) {
        fprintf(stderr, "%s: �t�H���g�t�@�~�����I���ł��܂���ł����B\n",
                GetLongBaseName(argv[0]));
        return 1;
    }

    for (; i < argc; i++) {
        DrawBanner(lpszUseChar, nScreenWidth, nFontSize, argv[i], bDebug);
    }
    return 0;
}
