/*
 * 単語データクラスヘッダ
 *  $Id$
 *
 * Copyright (C) 2007-2015, Toshi All rights reserved.
*/
#ifndef __WORDDATA_H__
#define __WORDDATA_H__

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include "resource.h"

#define	LINE_MAX 256

/* 単語と意味の構造体 */
struct word_data {
	TCHAR* word;
	TCHAR* mean;
};

/* 単語データクラス */
class WordData {

	private:
		word_data* worddata;	// 単語と意味の構造体
		int* randomdata;		// 乱数配列
		int total;				// 単語総数
		int countData(TCHAR*);	// データファイル数取得
		int readData(TCHAR*);	// データ読み込み

	public:
		WordData();				// コンストラクタ
		~WordData();			// デストラクタ
		int initialize(TCHAR*);	// 初期化
		void finalize();		// メモリ開放
		void doShuffle();		// 乱数配列をシャッフル
		int getTotal();			// 総数を返す
		int getDataFileName(TCHAR*, DWORD, TCHAR*);

		TCHAR* getWord(int);	// 単語を返す
		DWORD getWordLen(int);	// 単語の長さを返す
		TCHAR* getMean(int);	// 意味を返す
		DWORD getMeanLen(int);	// 意味の長さを返す
};

#endif /* __WORDDATA_H__ */
