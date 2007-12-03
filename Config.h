/*
 * 初期設定ファイルクラス
 *  $Id$
 *
 * Copyright (C) 2007, Toshi All rights reserved.
*/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <windows.h>
#include <tchar.h>

// 初期設定ファイル名
#define INIFILENAME "flashword.ini"

// ウィンドウのデフォルト位置
#define DEFAULT_WINDOW_TOP "10"
#define DEFAULT_WINDOW_LEFT "10"
// ウィンドウのデフォルトサイズ
#define DEFAULT_WINDOW_WIDTH "300"
#define DEFAULT_WINDOW_HEIGHT "70"
// デフォルトのインターバル
#define DEFAULT_INTERVAL "4"

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
