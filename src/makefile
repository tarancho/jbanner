# -*- mode: makefile; coding: sjis-dos; -*-
# Time-stamp: <2003-06-09 12:04:33 tfuruka1>
# Copyright (C) 2003 Tadamegu Furukawa#
#
# jbanner �� makefile

# (while (re-search-forward "^#!" nil t) (replace-match "!"))
# (while (re-search-forward "^!" nil t) (replace-match "#!"))

#
# ���� makefaile �͊T�ˈȉ��̍\���ɂȂ��Ă��܂��B
#
#        !if defined(CYGWIN)    ��(1)
#        
#        << CYGWIN �̒�`��>>    ��(2)
#
#        !else                  ��(1)
#
#        << MSVC �̒�`�� >>     ��(3)
#
#        !endif                 ��(1)
#
#        << ���ʂ̒�` >>
#
# (1)�̕�����, nmake �ł̂ݗL���ȕ��@�ł��̂�, make ���g�p����ꍇ��, 
# �R�����g�A�E�g���ĉ������Bnmake ���g�p����ꍇ��, (2), (3)�̃R�����g
# ���O���ĉ������B������, gcc �ŃR���p�C������ꍇ�̓R�}���h�v�����v�g
# ������
#
#     nmake CYGWIN=1
#
# �Ɠ��͂��ĉ������BMSVC �ŃR���p�C������ꍇ��
#
#     nmake
#
# �Ƃ��ĉ������Bmake ���g�p����ꍇ��, !if ���͎g�p�o���܂���̂�, (1)
# �̕������R�����g�A�E�g���ĉ������B������, gcc �ŃR���p�C������ꍇ��,
# (2)�����̃R�����g���O����, (3)�̕������R�����g�A�E�g���ĉ������BMSVC 
# �ŃR���p�C������ꍇ��, (3)�����̃R�����g���O����, (2)�̕������R����
# �g�A�E�g���ĉ������B������, �R�}���h�v�����v�g������
#
#     make
#
# �Ɠ��͂��ĉ������B
#
# $Id: makefile,v 1.1 2004/01/19 09:01:28 tfuruka1 Exp $
# $Log: makefile,v $
# Revision 1.1  2004/01/19 09:01:28  tfuruka1
# ���r�W�����Ǘ���RCS����CVS�֕ύX���܂����B
#


# 
# �� nmake �ȊO�̓R�����g�A�E�g���ĉ�����
!if defined(CYGWIN)

#######
####### CYGWIN�p
#######

# CYGWIN �̃��\�[�X�R���p�C����ʂ��R�c�BIDC_STATIC ����`����Ă��Ȃ�
# ����MFC�̃��\�[�X�p�w�b�_�t�@�C�����g�p���Ȃ��悤�ɂ���΁A���͒�
# ��܂��B�ȉ��͗�ł�

#    #ifdef CYGWIN
#    #   include <windows.h>
#    #   define IDC_STATIC -1
#    #else
#    #   include "afxres.h"
#    #endif

# �\�P�b�g���g�p����ꍇ�́A__USE_W32_SOCKETS ���`�����ق����ǂ��悤
# �ł��B���ɕs��𔭌��������͖����̂ł����A�ꉞ�E�H�[�j���O��������
# ���̂œ���Ă܂��B

O = o
RES = o
CC = gcc
CFLAGS = -g -DCYGWIN -Wall -D__USE_W32_SOCKETS -mno-cygwin
LDFLAGS = -lwsock32 -lgdi32
RC = windres
RCFLAGS = --define CYGWIN --define IDC_STATIC=-1 -o

# �� nmake �ȊO�̓R�����g�A�E�g���ĉ�����
!else

#######
####### MSC(VC)�p
#######

O=obj
RES=res
CC=cl
CFLAGS=/W3 /Zi
LDFLAGS=/link user32.lib wsock32.lib gdi32.lib
RC=rc
RCFLAGS= /fo

# �� nmake �ȊO�̓R�����g�A�E�g���ĉ�����
!endif

#######
####### �ȉ���MSVC, gcc����
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

# ���\�[�X�̃R���p�C���BMSVC��rc��CYGWIN��windres�ŏo�̓t�@�C���̃I�v
# �V�������قȂ�̂ŁARCFLAGS�̈�ԍŌ�̃I�v�V�������o�̓t�@�C������
# ���Ă��܂��B
$(OBJ_RES) : $(RES_BASE).rc
	$(RC) $(RCFLAGS) $@ $(RES_BASE).rc

$(OBJS) : makefile jbanner.h

tags:
	$(ETAGS) *.c *.h

# tar �Ōł߂�Bnmake ����shell���Ăяo����Ȃ��̂ŁA�����I��sh���Ă�
# �o���Ă��܂��B
tar: clean tags
	$(SHELL) -c "(cd ..; tar cvzf $(TAR_GZ) src)"

#
# --- �h�L�������g
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

# --- dvi --- CYGWIN��texindex���Ɠ��{�ꂪ�����������ł��Ȃ��̂ŁApTeX
# �Ŕz�z����Ă���texindex���g�p���Ă��܂��B�Ƃ��낪�ApTeX��texindex��
# �ƁA���C���h�J�[�h�̓W�J�����܂��o���Ȃ��悤�Ȃ̂ŁAsh�������I�ɌĂ�
# �o���ď������Ă��܂��B
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