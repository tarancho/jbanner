# -*- mode: makefile; coding: utf-8; -*-
# Copyright (C) 2003 Tadamegu Furukawa#
# $Id: makefile,v 1.2 2004/01/19 09:16:49 tfuruka1 Exp $
#
# 全般的に MinGWでのBuildに変更しました。以下のコメントは履歴として残
# しておきます。
#
# jbanner の makefile (nmake が必要です) ← make になりました

# (while (re-search-forward "^#!" nil t) (replace-match "!"))
# (while (re-search-forward "^!" nil t) (replace-match "#!"))

#
# この makefaile は概ね以下の構造になっています。
#
#        !if defined(CYGWIN)    ←(1)
#
#        << CYGWIN の定義部>>    ←(2)
#
#        !else                  ←(1)
#
#        << MSVC の定義部 >>     ←(3)
#
#        !endif                 ←(1)
#
#        << 共通の定義 >>
#
# (1)の分部は, nmake でのみ有効な文法ですので, make を使用する場合は,
# コメントアウトして下さい。nmake を使用する場合は, (2), (3)のコメント
# を外して下さい。そして, gcc でコンパイルする場合はコマンドプロンプト
# 等から
#
#     nmake CYGWIN=1
#
# と入力して下さい。MSVC でコンパイルする場合は
#
#     nmake
#
# として下さい。make を使用する場合は, !if 文は使用出来ませんので, (1)
# の分部をコメントアウトして下さい。そして, gcc でコンパイルする場合は,
# (2)分部のコメントを外して, (3)の分部をコメントアウトして下さい。MSVC
# でコンパイルする場合は, (3)分部のコメントを外して, (2)の分部をコメン
# トアウトして下さい。そして, コマンドプロンプト等から
#
#     make
#
# と入力して下さい。
#

# $Log: makefile,v $
# Revision 1.2  2004/01/19 09:16:49  tfuruka1
# clean で　docディレクトリを削除するようにしました。
#
# Revision 1.1  2004/01/19 09:01:28  tfuruka1
# リビジョン管理をRCSからCVSへ変更しました。
#


#######
####### CYGWIN用 → MinGW へ変更しました
#######

# CYGWIN のリソースコンパイラを通すコツ。IDC_STATIC が定義されていない
# 事とMFCのリソース用ヘッダファイルを使用しないようにすれば、大抵は通
# ります。以下は例です

#    #ifdef CYGWIN
#    #   include <windows.h>
#    #   define IDC_STATIC -1
#    #else
#    #   include "afxres.h"
#    #endif

# ソケットを使用する場合は、__USE_W32_SOCKETS を定義したほうが良いよう
# です。特に不具合を発見した事は無いのですが、一応ウォーニングが消えな
# いので入れてます。

# 新しいCygwinを使用している場合は gcc で -mno-cygwin オプションが削除
# されています。その場合は gcc-3 を使用して下さい。
#
# ↑ 現在は MinGWにしました。

O = o
RES = o
CC = i686-w64-mingw32-gcc
CFLAGS = -g -DCYGWIN -Wall -D__USE_W32_SOCKETS --input-charset=utf-8 \
	--exec-charset=cp932

LDFLAGS = -lwsock32 -lgdi32
RC = i686-w64-mingw32-windres
RCFLAGS = --define CYGWIN --define IDC_STATIC=-1 -o

#######
####### 以下はMSVC, gcc共通
#######

.SUFFIXES:	.$(O) .c
.c.$(O):
	$(CC) $(CFLAGS) -c $<

OBJS = main.$(O) gdi_wrap.$(O) comm.$(O) string.$(O)
RES_BASE = resource
OBJ_RES = $(RES_BASE).$(RES)
BASE_NAME = jbanner
EXE = $(BASE_NAME).exe
TAR_GZ = $(BASE_NAME).tar.gz

TEXINFO = $(BASE_NAME).texinfo
INFO = $(BASE_NAME).info
PLAIN_TXT = $(BASE_NAME).txt
PDF = $(BASE_NAME).pdf
HTML = $(BASE_NAME).html

MAKEINFO=/bin/makeinfo
TEXINDEX=/usr/local/bin/texindex
DVIPDF=/usr/local/bin/dvipdfmx
ETAGS=/usr/local/emacs/bin/etags.exe

SHELL=/bin/sh

../$(EXE) : $(OBJS) $(OBJ_RES)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(OBJ_RES) $(LDFLAGS)

# リソースのコンパイル。MSVCのrcとCYGWINのwindresで出力ファイルのオプ
# ションが異なるので、RCFLAGSの一番最後のオプションを出力ファイル名に
# しています。
$(OBJ_RES) : $(RES_BASE).rc
	$(RC) $(RCFLAGS) $@ $(RES_BASE).rc

$(OBJS) : makefile jbanner.h

tags:
	$(ETAGS) *.c *.h

# tar で固める。nmake だとshellが呼び出されないので、強制的にshを呼び
# 出しています。
tar: ../$(EXE) doc clean tags
	$(SHELL) -c "(cd ../..; tar cvzf $(TAR_GZ) jbanner --exclude .git)"

#
# --- ドキュメント
#
doc : $(INFO) $(PLAIN_TXT) $(HTML) $(PDF) $(BASE_NAME).ps 

# --- html
$(HTML) : $(TEXINFO) makefile
	@echo ---
	@echo --- make HTML ---
	@echo ---
	$(MAKEINFO) --html --no-split $(TEXINFO)
	cp $@ ../doc/.
# --- text
$(PLAIN_TXT) : $(TEXINFO) makefile
	@echo ---
	@echo --- make text ---
	@echo ---
	$(MAKEINFO) --no-headers $(TEXINFO) > $@
	cp $@ ../doc/.
# --- info
$(INFO) : $(TEXINFO) makefile
	@echo ---
	@echo --- make info ---
	@echo ---
	$(MAKEINFO) $(TEXINFO)
	cp $@ ../doc/.
# --- pdf
$(PDF) : $(BASE_NAME).dvi
	@echo ---
	@echo --- make pdf ---
	@echo ---
	$(DVIPDF) $(BASE_NAME).dvi
	cp $@ ../doc/.

# --- dvi --- CYGWINのtexindexだと日本語が正しく処理できないので、pTeX
# で配布されているtexindexを使用しています。ところが、pTeXのtexindexだ
# と、ワイルドカードの展開がうまく出来ないようなので、shを強制的に呼び
# 出して処理しています。
$(BASE_NAME).dvi : $(TEXINFO) makefile
	@echo ---
	@echo --- make dvi ---
	@echo ---
	ptex -kanji=utf8 $(TEXINFO)
	$(TEXINDEX) $(BASE_NAME).??
	ptex $(TEXINFO)
	ptex $(TEXINFO)
# --- ps
$(BASE_NAME).ps : $(BASE_NAME).dvi
	@echo ---
	@echo --- make PostScript ---
	@echo ---
	dvipsk -R600 -Pdl $(BASE_NAME).dvi

clean :
	rm -f *.obj
	rm -f *.o
	rm -f *.*~
	rm -f *~ *.tmp
	rm -f *.pdb ../*.pdb
	rm -f *.res
	rm -f *.ilk ../*.ilk
	rm -f *.html
	rm -f *.dvi
	rm -f *.txt
	rm -f *.pdf
	rm -f *.ps
	rm -f *.toc
	rm -f *.log
	rm -f *.aux
	rm -f *.cp
	rm -f *.cps
	rm -f *.fn
	rm -f *.fns
	rm -f *.pg
	rm -f *.pgs
	rm -f *.tp
	rm -f *.tps
	rm -f *.vr
	rm -f *.vrs
	rm -f *.ky
	rm -f *.kys
	rm -f *.info
clean-all : clean
	rm -f ../../$(TAR_GZ)
	rm -f ../$(EXE)
	rm -f ../doc/*.html
	rm -f ../doc/*.pdf
	rm -f ../doc/*.txt
	rm -f ../doc/*.info
	rm -f TAGS
