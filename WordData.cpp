/*
 * 単語データクラス
 *  $Id$
 *
 * Copyright (C) 2007-2014, Toshi All rights reserved.
*/
#include "WordData.h"

/* コンストラクタ */
WordData::WordData(){}

/* デストラクタ */
WordData::~WordData(){}

/* 初期化 */
int WordData::initialize(TCHAR* filename){

	TCHAR szDataFileName[_MAX_PATH];

	total=0;

	// データファイル名取得
	if(getDataFileName(szDataFileName, _countof(szDataFileName), filename) != TRUE){
		MessageBox(NULL, TEXT("データファイル名取得失敗"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	// データ数取得
	total = countData(szDataFileName);
	if(total < 0){
		return FALSE;
	}

	// データ用メモリ確保
	worddata = (word_data*)malloc(sizeof(struct word_data) * total);
	if(worddata == NULL){
		MessageBox(NULL, TEXT("メモリが確保できませんでした"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	// 乱数用メモリ確保
	randomdata = (int*)malloc(sizeof(int) * total);
	if(randomdata == NULL){
		MessageBox(NULL, TEXT("メモリが確保できませんでした"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	// 乱数初期化
	srand((unsigned) time(NULL));

	// 乱数初期値格納
	for(int i=0; i<total; i++){
		randomdata[i] = i;
	}

	// シャッフル
	doShuffle();

	// データ取得
	if(readData(szDataFileName) != TRUE){
		return FALSE;
	}

	return TRUE;
}

/* メモリ開放 */
void WordData::finalize(){

	// 乱数用メモリ開放
	if(randomdata != NULL){
		free(randomdata);
	}

	// 単語データ用メモリ開放
	for(int i=0; i<total; i++){
		free(worddata[i].word);
		free(worddata[i].mean);
	}

	if(worddata != NULL){
		free(worddata);
	}
}

/* データファイル数取得 */
int WordData::countData(TCHAR* filename){

	FILE* fp;
	TCHAR buf[LINE_MAX];
	int count=0;

	_tfopen_s(&fp, filename, _T("rb"));
	if(fp == NULL){
		MessageBox(NULL, TEXT("データファイル読み込み失敗"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return -1;
	}

	// BOM 判定
	if(_fgetts(buf, 2, fp) != NULL){

		if(buf[0] != 0xfeff){
			MessageBox(NULL, TEXT("Unicodeファイルではありません"),
						_T("MSNChangeMSG"), MB_ICONSTOP | MB_OK);
			return -1;
		}
	}else{
		MessageBox(NULL, TEXT("データファイル読み込み失敗"),
			_T("MSNChangeMSG"), MB_ICONSTOP | MB_OK);
		return -1;
	}

	while(_fgetts(buf, LINE_MAX, fp) != NULL){
		// 空行無視
		if(_tcscmp(buf, TEXT("\r\n")) == 0){
			continue;
		}

		// コメント無視
		if(buf[0] == '#'){
			continue;
		}

		count++;
	}

	fclose(fp);

	if(count < 1){
		MessageBox(NULL, TEXT("データがありません"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return -1;
	}

	return count;
}

/* データ読み込み */
int WordData::readData(TCHAR* filename){

	FILE* fp;
	TCHAR buf[LINE_MAX];
	TCHAR* p;
	TCHAR* next;
	int count=0;

	_tfopen_s(&fp, filename, _T("rb"));
	if(fp == NULL){
		MessageBox(NULL, TEXT("データファイル読み込み失敗"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	// BOM 判定
	if(_fgetts(buf, 2, fp) != NULL){
		if(buf[0] != 0xfeff){
			MessageBox(NULL, TEXT("Unicodeファイルではありません"),
						_T("MSNChangeMSG"), MB_ICONSTOP | MB_OK);
			return FALSE;
		}
	}else{
		MessageBox(NULL, TEXT("データファイル読み込み失敗"),
			_T("MSNChangeMSG"), MB_ICONSTOP | MB_OK);
			return FALSE;
	}

	while(_fgetts(buf, LINE_MAX, fp) != NULL){

		// 空行無視
		if(_tcscmp(buf, TEXT("\r\n")) == 0){
			continue;
		}

		// コメント無視
		if(buf[0] == '#'){
			continue;
		}

		// 改行削除
		p = _tcsstr(buf, TEXT("\r\n"));
		if(p != NULL){
			*p = _T('\0');
		}

		p = _tcstok_s(buf, _T("\t"), &next);
		if(p == NULL){
			MessageBox(NULL, TEXT("データファイルフォーマットエラー"),
						IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
			return FALSE;
		}

		worddata[count].word = _tcsdup(p);

		p = _tcstok_s(NULL, _T("\t"), &next);
		if(p == NULL){
			MessageBox(NULL, TEXT("データファイルフォーマットエラー"),
						IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
			return FALSE;
		}

		worddata[count].mean = _tcsdup(p);

		count++;
	}

	fclose(fp);

	return TRUE;
}

/* データファイル名をフルパスで取得 */
int WordData::getDataFileName(TCHAR* szFullFileName, DWORD nSize, TCHAR* szFileName){

	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];

	// 実行ファイルのフルパスとファイル名を取得
	if(!GetModuleFileName(NULL, szFullFileName, nSize*sizeof(TCHAR)))
		return FALSE;

	// フルパスを分解
	_tsplitpath_s(szFullFileName, szDrive, _countof(szDrive), szDir, _countof(szDir), NULL, 0, NULL, 0);
	// szFileName にデータファイル名をフルパスで格納
	_sntprintf_s(szFullFileName, nSize, _TRUNCATE, L"%s%s%s", szDrive, szDir, szFileName);

	return TRUE;
}

/* 乱数配列をシャッフル */
void WordData::doShuffle(){
	for(int i=0; i<total-1; i++){
		int r = rand()%(total-i) + i;
		int m = randomdata[i];
		randomdata[i] = randomdata[r];
		randomdata[r] = m;
	}
}

/* 総数を返す */
int WordData::getTotal(){
	return total;
}

/* 単語を返す */
TCHAR* WordData::getWord(int index){
	return worddata[randomdata[index]].word;
}

/* 単語の長さを返す */
DWORD WordData::getWordLen(int index){
	return lstrlen(worddata[randomdata[index]].word);
}

/* 意味を返す */
TCHAR* WordData::getMean(int index){
	return worddata[randomdata[index]].mean;
}

/* 意味の長さを返す */
DWORD WordData::getMeanLen(int index){
	return lstrlen(worddata[randomdata[index]].mean);
}
