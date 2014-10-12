/*
 * �����ݒ�t�@�C���N���X
 *  $Id$
 *
 * Copyright (C) 2007-2014, Toshi All rights reserved.
*/
#include "Config.h"

// �R���X�g���N�^
Config::Config(){}

// �f�X�g���N�^
Config::~Config(){}

/* �ݒ�t�@�C�������t���p�X�ŕԂ�
    TCHAR* �ݒ�t�@�C����
    DWORD �ݒ�t�@�C�����o�b�t�@�̃T�C�Y
*/
int Config::getIniFileName(TCHAR* szFullPathName, DWORD nSize){

	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];

	// ���s�t�@�C���̃t���p�X�ƃt�@�C�������擾
	if(!GetModuleFileName(NULL, szFullPathName, nSize*sizeof(TCHAR)))
		return ERR;

	// �t���p�X�𕪉�
	_tsplitpath_s(szFullPathName, szDrive, _countof(szDrive), szDir, _countof(szDir), NULL, 0, NULL, 0);
	// szFileName �Ƀf�[�^�t�@�C�������t���p�X�Ŋi�[
	_sntprintf_s(szFullPathName, nSize, _TRUNCATE, L"%s%s%s", szDrive, szDir, INIFILENAME);

	return OK;
}

/* �ݒ�t�@�C����Window�ʒu��ǂݍ���ŕԂ�
    LPRECT rect �E�B���h�E�̍��W�l
    �߂�l OK ����
*/
int Config::readWindowRect(RECT* rect){

	TCHAR szIniFileName[_MAX_PATH];
	TCHAR szBuf[100];

	// INI �t�@�C�������t���p�X�Ŏ擾
	if(getIniFileName(szIniFileName, _countof(szIniFileName)) != OK)
		return ERR;

	// Top �ʒu�̎擾
	if(GetPrivateProfileString(
		TEXT("General"), TEXT("Top"), DEFAULT_WINDOW_TOP,
			szBuf, sizeof(szBuf), szIniFileName) == 0)
		return ERR;

	rect->top = (_tstol(szBuf) == 0)?_tstol(DEFAULT_WINDOW_TOP):_tstol(szBuf);

	// Left �ʒu�̎擾
	if(GetPrivateProfileString(
		TEXT("General"), TEXT("Left"), DEFAULT_WINDOW_LEFT,
			szBuf, sizeof(szBuf), szIniFileName) == 0)
		return ERR;

	rect->left = (_tstol(szBuf) == 0)?_tstol(DEFAULT_WINDOW_LEFT):_tstol(szBuf);

	// Width ��ǂݍ���� Right �ɕϊ����Ċi�[
	if(GetPrivateProfileString(
		TEXT("General"), TEXT("Width"), DEFAULT_WINDOW_WIDTH,
			szBuf, sizeof(szBuf), szIniFileName) == 0)
		return ERR;

	rect->right = (_tstol(szBuf) == 0)?_tstol(DEFAULT_WINDOW_WIDTH):_tstol(szBuf);
	rect->right += rect->left;

	// Height ��ǂݍ���� Bottom �ɕϊ����Ċi�[
	if(GetPrivateProfileString(
		TEXT("General"), TEXT("Height"), DEFAULT_WINDOW_HEIGHT,
			szBuf, sizeof(szBuf), szIniFileName) == 0)
		return ERR;

	rect->bottom = (_tstol(szBuf) == 0)?_tstol(DEFAULT_WINDOW_HEIGHT):_tstol(szBuf);
	rect->bottom += rect->top;

	return OK;
}

/* ���݂�Window�ʒu��ݒ�t�@�C���ɏ����o��
    LPRECT rect �E�B���h�E�̍��W�l
    �߂�l OK ����
*/
int Config::writeWindowRect(RECT* rect){

	TCHAR szIniFileName[_MAX_PATH];
	TCHAR szBuf[100];

	// INI �t�@�C�������t���p�X�Ŏ擾
	if(getIniFileName(szIniFileName, _countof(szIniFileName)) != OK)
		return ERR;

	// Top �ʒu�̕ۑ�
	wsprintf(szBuf, _T("%d"), rect->top);

	if(!WritePrivateProfileString(
		TEXT("General"), TEXT("Top"), szBuf, szIniFileName))
		return ERR;

	// Left �ʒu�̕ۑ�
	wsprintf(szBuf, _T("%d"), rect->left);

	if(!WritePrivateProfileString(
		TEXT("General"), TEXT("Left"), szBuf, szIniFileName))
		return ERR;

	// Width �̕ۑ�
	wsprintf(szBuf, _T("%d"), rect->right - rect->left);

	if(!WritePrivateProfileString(
		TEXT("General"), TEXT("Width"), szBuf, szIniFileName))
		return ERR;

	// Height �̕ۑ�
	wsprintf(szBuf, _T("%d"), rect->bottom - rect->top);

	if(!WritePrivateProfileString(
		TEXT("General"), TEXT("Height"), szBuf, szIniFileName))
		return ERR;

	return OK;
}

/* �ݒ�t�@�C����Interval��ǂݍ���ŕԂ�
    int* interval �C���^�[�o��
    �߂�l OK ����
*/
int Config::readInterval(int* interval){

	TCHAR szIniFileName[_MAX_PATH];
	TCHAR szBuf[100];

	// INI �t�@�C�������t���p�X�Ŏ擾
	if(getIniFileName(szIniFileName, _countof(szIniFileName)) != OK)
		return ERR;

	// Interval �̎擾
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

/* Interval��ݒ�t�@�C���ɏ����o��
    int interval �C���^�[�o��
    �߂�l OK ����
*/
int Config::writeInterval(int interval){

	TCHAR szIniFileName[_MAX_PATH];
	TCHAR szBuf[100];

	// INI �t�@�C�������t���p�X�Ŏ擾
	if(getIniFileName(szIniFileName, _countof(szIniFileName)) != OK)
		return ERR;

	// Interval �̕ۑ�
	wsprintf(szBuf, _T("%d"), interval);

	if(!WritePrivateProfileString(
		TEXT("General"), TEXT("Interval"), szBuf, szIniFileName))
		return ERR;

	return OK;
}
