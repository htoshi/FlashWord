/*
 * �P��f�[�^�N���X
 *  $Id$
 *
 * Copyright (C) 2007, Toshi All rights reserved.
*/
#include "WordData.h"

/* �R���X�g���N�^ */
WordData::WordData(){}

/* �f�X�g���N�^ */
WordData::~WordData(){}

/* ������ */
int WordData::initialize(TCHAR* filename){

	TCHAR szDataFileName[_MAX_PATH];

	total=0;

	// �f�[�^�t�@�C�����擾
	if(getDataFileName(szDataFileName, sizeof(szDataFileName), filename) != TRUE){
		MessageBox(NULL, TEXT("�f�[�^�t�@�C�����擾���s"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	// �f�[�^���擾
	total = countData(szDataFileName);
	if(total < 0){
		return FALSE;
	}

	// �f�[�^�p�������m��
	worddata = (word_data*)malloc(sizeof(struct word_data) * total);
	if(worddata == NULL){
		MessageBox(NULL, TEXT("���������m�ۂł��܂���ł���"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	// �����p�������m��
	randomdata = (int*)malloc(sizeof(int) * total);
	if(randomdata == NULL){
		MessageBox(NULL, TEXT("���������m�ۂł��܂���ł���"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	// ����������
	srand((unsigned) time(NULL));

	// ���������l�i�[
	for(int i=0; i<total; i++){
		randomdata[i] = i;
	}

	// �V���b�t��
	doShuffle();

	// �f�[�^�擾
	if(readData(szDataFileName) != TRUE){
		return FALSE;
	}

	return TRUE;
}

/* �������J�� */
void WordData::finalize(){

	// �����p�������J��
	if(randomdata != NULL){
		free(randomdata);
	}

	// �P��f�[�^�p�������J��
	for(int i=0; i<total; i++){
		free(worddata[i].word);
		free(worddata[i].mean);
	}

	if(worddata != NULL){
		free(worddata);
	}
}

/* �f�[�^�t�@�C�����擾 */
int WordData::countData(TCHAR* filename){

	FILE* fp;
	TCHAR buf[LINE_MAX];
	int count=0;

	if((fp = fopen(filename, "r"))==NULL){
		MessageBox(NULL, TEXT("�f�[�^�t�@�C���ǂݍ��ݎ��s"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return -1;
	}

	while(fgets(buf, LINE_MAX, fp) != NULL){
		// ��s����
		if(_tcslen(buf) <= 1){
			continue;
		}

		// �R�����g����
		if(buf[0] == '#'){
			continue;
		}

		count++;
	}

	fclose(fp);

	if(count < 1){
		MessageBox(NULL, TEXT("�f�[�^������܂���"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return -1;
	}

	return count;
}

/* �f�[�^�ǂݍ��� */
int WordData::readData(TCHAR* filename){

	FILE* fp;
	TCHAR buf[LINE_MAX];
	TCHAR* p;
	TCHAR* q;
	int count=0;

	if((fp = fopen(filename, "r"))==NULL){
		MessageBox(NULL, TEXT("�f�[�^�t�@�C���ǂݍ��ݎ��s"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	while(fgets(buf, LINE_MAX, fp) != NULL){

		// ��s����
		if(_tcslen(buf) <= 1){
			continue;
		}

		// �R�����g����
		if(buf[0] == '#'){
			continue;
		}

		// ���s�폜
		if((p=strchr(buf, '\n')) != NULL){
			*p='\0';
		}

		if((p=strtok(buf, "\t")) == NULL){
			MessageBox(NULL, TEXT("�f�[�^�t�@�C���t�H�[�}�b�g�G���["),
						IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
			return FALSE;
		}

		// Allocate word memory
		if((q=(TCHAR*)malloc(strlen(p) + 1)) == NULL){
			MessageBox(NULL, TEXT("���������m�ۂł��܂���ł���"),
						IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
			return FALSE;
		}

		worddata[count].word = q;
		_tcsncpy(worddata[count].word, p, strlen(p) + 1);

		if((p=strtok(NULL, "\t")) == NULL){
			MessageBox(NULL, TEXT("�f�[�^�t�@�C���t�H�[�}�b�g�G���["),
						IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
			return FALSE;
		}

		// Allocate meaning memory
		if((q=(TCHAR*)malloc(strlen(p) + 1)) == NULL){
			MessageBox(NULL, TEXT("���������m�ۂł��܂���ł���"),
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

/* �f�[�^�t�@�C�������t���p�X�Ŏ擾 */
int WordData::getDataFileName(TCHAR* szFullFileName, DWORD nSize, TCHAR* szFileName){

	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];

	// ���s�t�@�C���̃t���p�X�ƃt�@�C�������擾
	if(!GetModuleFileName(NULL, szFullFileName, nSize))
		return FALSE;

	// �t���p�X�𕪉�
	_tsplitpath(szFullFileName, szDrive, szDir, NULL, NULL);

	// szFileName �Ƀf�[�^�t�@�C�������t���p�X�Ŋi�[
	_tcscpy(szFullFileName, szDrive);
	_tcscat(szFullFileName, szDir);
	_tcscat(szFullFileName, szFileName);

	return TRUE;
}

/* �����z����V���b�t�� */
void WordData::doShuffle(){
	for(int i=0; i<total-1; i++){
		int r = rand()%(total-i) + i;
		int m = randomdata[i];
		randomdata[i] = randomdata[r];
		randomdata[r] = m;
	}
}

/* ������Ԃ� */
int WordData::getTotal(){
	return total;
}

/* �P���Ԃ� */
TCHAR* WordData::getWord(int index){
	return worddata[randomdata[index]].word;
}

/* �P��̒�����Ԃ� */
DWORD WordData::getWordLen(int index){
	return lstrlen(worddata[randomdata[index]].word);
}

/* �Ӗ���Ԃ� */
TCHAR* WordData::getMean(int index){
	return worddata[randomdata[index]].mean;
}

/* �Ӗ��̒�����Ԃ� */
DWORD WordData::getMeanLen(int index){
	return lstrlen(worddata[randomdata[index]].mean);
}
