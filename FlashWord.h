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

// データファイル名
#define	DATAFILENAME _T("data.txt")

/* グローバル変数 */
static int _tick=0;
static int _index=0;
static int _interval=0;
static bool _pause=false;
static WordData* _wordData;

/* プロトタイプ宣言 */
LRESULT CALLBACK WindowProc(HWND,UINT,WPARAM,LPARAM);
void OnPaint(HWND);
void OnTimer(HWND);
void deleteMenu(HWND);
void addMenu(HWND);
void reload(HWND);
