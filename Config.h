/*
 * �����ݒ�t�@�C���N���X
 *  $Id$
 *
 * Copyright (C) 2007, Toshi All rights reserved.
*/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <windows.h>
#include <tchar.h>

// �����ݒ�t�@�C����
#define INIFILENAME "flashword.ini"

// �E�B���h�E�̃f�t�H���g�ʒu
#define DEFAULT_WINDOW_TOP "10"
#define DEFAULT_WINDOW_LEFT "10"
// �E�B���h�E�̃f�t�H���g�T�C�Y
#define DEFAULT_WINDOW_WIDTH "300"
#define DEFAULT_WINDOW_HEIGHT "70"
// �f�t�H���g�̃C���^�[�o��
#define DEFAULT_INTERVAL "4"

#define OK 0
#define ERR -1

/* �ݒ�t�@�C���N���X */
class Config {

	private:
		int getIniFileName(TCHAR*, DWORD);

	public:
		Config();	// �R���X�g���N�^
		~Config();	// �f�X�g���N�^

		int readWindowRect(RECT*);	// Window�ʒu��ǂݍ���ŕԂ�
		int writeWindowRect(RECT*);	// Window�ʒu�������o��
		int readInterval(int*);		// �C���^�[�o���ǂݍ���
		int writeInterval(int);		// �C���^�[�o���������o��
};

#endif /* __CONFIG_H__ */