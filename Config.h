/*
 * 初期設定ファイルクラス
 *  $Id$
 *
 * Copyright (C) 2007-2015, Toshi All rights reserved.
*/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <windows.h>
#include <tchar.h>

// 初期設定ファイル名
#define INIFILENAME _T("flashword.ini")

// ウィンドウのデフォルト位置
#define DEFAULT_WINDOW_TOP _T("10")
#define DEFAULT_WINDOW_LEFT _T("10")
// ウィンドウのデフォルトサイズ
#define DEFAULT_WINDOW_WIDTH _T("320")
#define DEFAULT_WINDOW_HEIGHT _T("86")
// デフォルトのインターバル
#define DEFAULT_INTERVAL _T("4")

#define OK 0
#define ERR -1

/* 設定ファイルクラス */
class Config {

	private:
		int getIniFileName(TCHAR*, DWORD);

	public:
		Config();	// コンストラクタ
		~Config();	// デストラクタ

		int readWindowRect(RECT*);	// Window位置を読み込んで返す
		int writeWindowRect(RECT*);	// Window位置を書き出す
		int readInterval(int*);		// インターバル読み込む
		int writeInterval(int);		// インターバルを書き出す
};

#endif /* __CONFIG_H__ */
