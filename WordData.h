/*
 * �P��f�[�^�N���X�w�b�_
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

/* �P��ƈӖ��̍\���� */
struct word_data {
	TCHAR* word;
	TCHAR* mean;
};

/* �P��f�[�^�N���X */
class WordData {

	private:
		word_data* worddata;	// �P��ƈӖ��̍\����
		int* randomdata;		// �����z��
		int total;				// �P�ꑍ��
		int countData(TCHAR*);	// �f�[�^�t�@�C�����擾
		int readData(TCHAR*);	// �f�[�^�ǂݍ���

	public:
		WordData();				// �R���X�g���N�^
		~WordData();			// �f�X�g���N�^
		int initialize(TCHAR*);	// ������
		void finalize();		// �������J��
		void doShuffle();		// �����z����V���b�t��
		int getTotal();			// ������Ԃ�
		int getDataFileName(TCHAR*, DWORD, TCHAR*);

		TCHAR* getWord(int);	// �P���Ԃ�
		DWORD getWordLen(int);	// �P��̒�����Ԃ�
		TCHAR* getMean(int);	// �Ӗ���Ԃ�
		DWORD getMeanLen(int);	// �Ӗ��̒�����Ԃ�
};

#endif /* __WORDDATA_H__ */
