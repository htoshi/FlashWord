/*
 * FlashWord
 *  $Id$
 *
 * Copyright (C) 2007-2014, Toshi All rights reserved.
*/
#define		STRICT
#define		WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <shellapi.h>
#include <stdlib.h>
#include "resource.h"
#include "WordData.h"
#include "Config.h"

// �f�[�^�t�@�C����
#define	DATAFILENAME _T("data.txt")

/* �O���[�o���ϐ� */
static int _tick=0;
static int _index=0;
static int _interval=0;
static bool _pause=false;
static WordData* _wordData;

/* �v���g�^�C�v�錾 */
LRESULT CALLBACK WindowProc(HWND,UINT,WPARAM,LPARAM);
void OnPaint(HWND);
void OnTimer(HWND);
void deleteMenu(HWND);
void addMenu(HWND);
void reload(HWND);
