# -*- mode: makefile; coding: sjis-dos; -*-
# Time-stamp: <2003-06-09 12:04:33 tfuruka1>
# Copyright (C) 2003 Tadamegu Furukawa#
#
# jbanner の makefile

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
# $Id: makefile,v 1.1 2004/01/19 09:01:28 tfuruka1 Exp $
# $Log: makefile,v $
# Revision 1.1  2004/01/19 09:01:28  tfuruka1
# リビジョン管理をRCSからCVSへ変更しました。
#


# 
# ↓ nmake 以外はコメントアウトして下さい
!if defined(CYGWIN)

#######
####### CYGWIN用
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

O = o
RES = o
CC = gcc
CFLAGS = -g -DCYGWIN -Wall -D__USE_W32_SOCKETS -mno-cygwin
LDFLAGS = -lwsock32 -lgdi32
RC = windres
RCFLAGS = --define CYGWIN --define IDC_STATIC=-1 -o

# ↓ nmake 以外はコメントアウトして下さい
!else

#######
####### MSC(VC)用
#######

O=obj
RES=res
CC=cl
CFLAGS=/W3 /Zi
LDFLAGS=/link user32.lib wsock32.lib gdi32.lib
RC=rc
RCFLAGS= /fo

# ↓ nmake 以外はコメントアウトして下さい
!endif

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

MAKEINFO=/usr/local/bin/makeinfo
TEXINDEX=/usr/local/bin/texindex
DVIPDF=/usr/local/bin/dvipdfmx
ETAGS=/usr/local/Meadow/1.15/bin/etags.exe

SHELL=/bin/sh

$(EXE) : $(OBJS) $(OBJ_RES)
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
tar: clean tags
	$(SHELL) -c "(cd ..; tar cvzf $(TAR_GZ) src)"

#
# --- ドキュメント
#
doc : $(INFO) $(PDF) $(BASE_NAME).ps $(PLAIN_TXT) $(HTML)

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
	ptex $(TEXINFO)
	$(SHELL) -c "$(TEXINDEX) $(BASE_NAME).??"
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
	rm -f *~
	rm -f *.pdb
	rm -f *.res
	rm -f *.ilk
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
	rm -f ../$(TAR_GZ)
	rm -f $(EXE)
