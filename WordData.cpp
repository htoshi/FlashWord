/*
 * 単語データクラス
 *  $Id$
 *
 * Copyright (C) 2007, Toshi All rights reserved.
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
	if(getDataFileName(szDataFileName, sizeof(szDataFileName), filename) != TRUE){
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

	if((fp = fopen(filename, "r"))==NULL){
		MessageBox(NULL, TEXT("データファイル読み込み失敗"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return -1;
	}

	while(fgets(buf, LINE_MAX, fp) != NULL){
		// 空行無視
		if(_tcslen(buf) <= 1){
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
	TCHAR* q;
	int count=0;

	if((fp = fopen(filename, "r"))==NULL){
		MessageBox(NULL, TEXT("データファイル読み込み失敗"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	while(fgets(buf, LINE_MAX, fp) != NULL){

		// 空行無視
		if(_tcslen(buf) <= 1){
			continue;
		}

		// コメント無視
		if(buf[0] == '#'){
			continue;
		}

		// 改行削除
		if((p=strchr(buf, '\n')) != NULL){
			*p='\0';
		}

		if((p=strtok(buf, "\t")) == NULL){
			MessageBox(NULL, TEXT("データファイルフォーマットエラー"),
						IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
			return FALSE;
		}

		// Allocate word memory
		if((q=(TCHAR*)malloc(strlen(p) + 1)) == NULL){
			MessageBox(NULL, TEXT("メモリが確保できませんでした"),
						IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
			return FALSE;
		}

		worddata[count].word = q;
		_tcsncpy(worddata[count].word, p, strlen(p) + 1);

		if((p=strtok(NULL, "\t")) == NULL){
			MessageBox(NULL, TEXT("データファイルフォーマットエラー"),
						IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
			return FALSE;
		}

		// Allocate meaning memory
		if((q=(TCHAR*)malloc(strlen(p) + 1)) == NULL){
			MessageBox(NULL, TEXT("メモリが確保できませんでした"),
						IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
			return FALSE;
		}

		worddata[count].mean = q;
		_tcsncpy(worddata[count].mean, p, strlen(p) + 1);

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
	if(!GetModuleFileName(NULL, szFullFileName, nSize))
		return FALSE;

	// フルパスを分解
	_tsplitpath(szFullFileName, szDrive, szDir, NULL, NULL);

	// szFileName にデータファイル名をフルパスで格納
	_tcscpy(szFullFileName, szDrive);
	_tcscat(szFullFileName, szDir);
	_tcscat(szFullFileName, szFileName);

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
