/*
 * �P��f�[�^�N���X
 *  $Id$
 *
 * Copyright (C) 2007-2014, Toshi All rights reserved.
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
	if(getDataFileName(szDataFileName, _countof(szDataFileName), filename) != TRUE){
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

	_tfopen_s(&fp, filename, _T("rb"));
	if(fp == NULL){
		MessageBox(NULL, TEXT("�f�[�^�t�@�C���ǂݍ��ݎ��s"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return -1;
	}

	// BOM ����
	if(_fgetts(buf, 2, fp) != NULL){

		if(buf[0] != 0xfeff){
			MessageBox(NULL, TEXT("Unicode�t�@�C���ł͂���܂���"),
						_T("MSNChangeMSG"), MB_ICONSTOP | MB_OK);
			return -1;
		}
	}else{
		MessageBox(NULL, TEXT("�f�[�^�t�@�C���ǂݍ��ݎ��s"),
			_T("MSNChangeMSG"), MB_ICONSTOP | MB_OK);
		return -1;
	}

	while(_fgetts(buf, LINE_MAX, fp) != NULL){
		// ��s����
		if(_tcscmp(buf, TEXT("\r\n")) == 0){
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
	TCHAR* next;
	int count=0;

	_tfopen_s(&fp, filename, _T("rb"));
	if(fp == NULL){
		MessageBox(NULL, TEXT("�f�[�^�t�@�C���ǂݍ��ݎ��s"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	// BOM ����
	if(_fgetts(buf, 2, fp) != NULL){
		if(buf[0] != 0xfeff){
			MessageBox(NULL, TEXT("Unicode�t�@�C���ł͂���܂���"),
						_T("MSNChangeMSG"), MB_ICONSTOP | MB_OK);
			return FALSE;
		}
	}else{
		MessageBox(NULL, TEXT("�f�[�^�t�@�C���ǂݍ��ݎ��s"),
			_T("MSNChangeMSG"), MB_ICONSTOP | MB_OK);
			return FALSE;
	}

	while(_fgetts(buf, LINE_MAX, fp) != NULL){

		// ��s����
		if(_tcscmp(buf, TEXT("\r\n")) == 0){
			continue;
		}

		// �R�����g����
		if(buf[0] == '#'){
			continue;
		}

		// ���s�폜
		p = _tcsstr(buf, TEXT("\r\n"));
		if(p != NULL){
			*p = _T('\0');
		}

		p = _tcstok_s(buf, _T("\t"), &next);
		if(p == NULL){
			MessageBox(NULL, TEXT("�f�[�^�t�@�C���t�H�[�}�b�g�G���["),
						IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
			return FALSE;
		}

		worddata[count].word = _tcsdup(p);

		p = _tcstok_s(NULL, _T("\t"), &next);
		if(p == NULL){
			MessageBox(NULL, TEXT("�f�[�^�t�@�C���t�H�[�}�b�g�G���["),
						IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
			return FALSE;
		}

		worddata[count].mean = _tcsdup(p);

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
	if(!GetModuleFileName(NULL, szFullFileName, nSize*sizeof(TCHAR)))
		return FALSE;

	// �t���p�X�𕪉�
	_tsplitpath_s(szFullFileName, szDrive, _countof(szDrive), szDir, _countof(szDir), NULL, 0, NULL, 0);
	// szFileName �Ƀf�[�^�t�@�C�������t���p�X�Ŋi�[
	_sntprintf_s(szFullFileName, nSize, _TRUNCATE, L"%s%s%s", szDrive, szDir, szFileName);

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
