/*
 * 初期設定ファイルクラス
 *  $Id$
 *
 * Copyright (C) 2007-2014, Toshi All rights reserved.
*/
#include "Config.h"

// コンストラクタ
Config::Config(){}

// デストラクタ
Config::~Config(){}

/* 設定ファイル名をフルパスで返す
    TCHAR* 設定ファイル名
    DWORD 設定ファイル名バッファのサイズ
*/
int Config::getIniFileName(TCHAR* szFullPathName, DWORD nSize){

	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];

	// 実行ファイルのフルパスとファイル名を取得
	if(!GetModuleFileName(NULL, szFullPathName, nSize*sizeof(TCHAR)))
		return ERR;

	// フルパスを分解
	_tsplitpath_s(szFullPathName, szDrive, _countof(szDrive), szDir, _countof(szDir), NULL, 0, NULL, 0);
	// szFileName にデータファイル名をフルパスで格納
	_sntprintf_s(szFullPathName, nSize, _TRUNCATE, L"%s%s%s", szDrive, szDir, INIFILENAME);

	return OK;
}

/* 設定ファイルのWindow位置を読み込んで返す
    LPRECT rect ウィンドウの座標値
    戻り値 OK 成功
*/
int Config::readWindowRect(RECT* rect){

	TCHAR szIniFileName[_MAX_PATH];
	TCHAR szBuf[100];

	// INI ファイル名をフルパスで取得
	if(getIniFileName(szIniFileName, _countof(szIniFileName)) != OK)
		return ERR;

	// Top 位置の取得
	if(GetPrivateProfileString(
		TEXT("General"), TEXT("Top"), DEFAULT_WINDOW_TOP,
			szBuf, sizeof(szBuf), szIniFileName) == 0)
		return ERR;

	rect->top = (_tstol(szBuf) == 0)?_tstol(DEFAULT_WINDOW_TOP):_tstol(szBuf);

	// Left 位置の取得
	if(GetPrivateProfileString(
		TEXT("General"), TEXT("Left"), DEFAULT_WINDOW_LEFT,
			szBuf, sizeof(szBuf), szIniFileName) == 0)
		return ERR;

	rect->left = (_tstol(szBuf) == 0)?_tstol(DEFAULT_WINDOW_LEFT):_tstol(szBuf);

	// Width を読み込んで Right に変換して格納
	if(GetPrivateProfileString(
		TEXT("General"), TEXT("Width"), DEFAULT_WINDOW_WIDTH,
			szBuf, sizeof(szBuf), szIniFileName) == 0)
		return ERR;

	rect->right = (_tstol(szBuf) == 0)?_tstol(DEFAULT_WINDOW_WIDTH):_tstol(szBuf);
	rect->right += rect->left;

	// Height を読み込んで Bottom に変換して格納
	if(GetPrivateProfileString(
		TEXT("General"), TEXT("Height"), DEFAULT_WINDOW_HEIGHT,
			szBuf, sizeof(szBuf), szIniFileName) == 0)
		return ERR;

	rect->bottom = (_tstol(szBuf) == 0)?_tstol(DEFAULT_WINDOW_HEIGHT):_tstol(szBuf);
	rect->bottom += rect->top;

	return OK;
}

/* 現在のWindow位置を設定ファイルに書き出す
    LPRECT rect ウィンドウの座標値
    戻り値 OK 成功
*/
int Config::writeWindowRect(RECT* rect){

	TCHAR szIniFileName[_MAX_PATH];
	TCHAR szBuf[100];

	// INI ファイル名をフルパスで取得
	if(getIniFileName(szIniFileName, _countof(szIniFileName)) != OK)
		return ERR;

	// Top 位置の保存
	wsprintf(szBuf, _T("%d"), rect->top);

	if(!WritePrivateProfileString(
		TEXT("General"), TEXT("Top"), szBuf, szIniFileName))
		return ERR;

	// Left 位置の保存
	wsprintf(szBuf, _T("%d"), rect->left);

	if(!WritePrivateProfileString(
		TEXT("General"), TEXT("Left"), szBuf, szIniFileName))
		return ERR;

	// Width の保存
	wsprintf(szBuf, _T("%d"), rect->right - rect->left);

	if(!WritePrivateProfileString(
		TEXT("General"), TEXT("Width"), szBuf, szIniFileName))
		return ERR;

	// Height の保存
	wsprintf(szBuf, _T("%d"), rect->bottom - rect->top);

	if(!WritePrivateProfileString(
		TEXT("General"), TEXT("Height"), szBuf, szIniFileName))
		return ERR;

	return OK;
}

/* 設定ファイルのIntervalを読み込んで返す
    int* interval インターバル
    戻り値 OK 成功
*/
int Config::readInterval(int* interval){

	TCHAR szIniFileName[_MAX_PATH];
	TCHAR szBuf[100];

	// INI ファイル名をフルパスで取得
	if(getIniFileName(szIniFileName, _countof(szIniFileName)) != OK)
		return ERR;

	// Interval の取得
	if(GetPrivateProfileString(
		TEXT("General"), TEXT("Interval"), DEFAULT_INTERVAL,
			szBuf, sizeof(szBuf), szIniFileName) == 0)
		return ERR;

	*interval = (_tstoi(szBuf) == 0)?_tstoi(DEFAULT_INTERVAL):_tstoi(szBuf);

	if(*interval<=0){
		*interval = _tstoi(DEFAULT_INTERVAL);
	}

	return OK;
}

/* Intervalを設定ファイルに書き出す
    int interval インターバル
    戻り値 OK 成功
*/
int Config::writeInterval(int interval){

	TCHAR szIniFileName[_MAX_PATH];
	TCHAR szBuf[100];

	// INI ファイル名をフルパスで取得
	if(getIniFileName(szIniFileName, _countof(szIniFileName)) != OK)
		return ERR;

	// Interval の保存
	wsprintf(szBuf, _T("%d"), interval);

	if(!WritePrivateProfileString(
		TEXT("General"), TEXT("Interval"), szBuf, szIniFileName))
		return ERR;

	return OK;
}
