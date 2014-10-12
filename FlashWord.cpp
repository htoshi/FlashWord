/*
 * FlashWord
 *  $Id$
 *
 * Copyright (C) 2007-2014, Toshi All rights reserved.
*/
#include	"FlashWord.h"

/*  �A�v���P�[�V�����G���g���[�|�C���g  */
int WINAPI WinMain(HINSTANCE    hInstance,
                   HINSTANCE    hPrevInstance,
                   LPSTR        CmdLine,
                   int          CmdShow){

	MSG             msg;    /*  ���b�Z�[�W�L���[����擾�������b�Z�[�W  */
	WNDCLASS        wc;     /*  �E�C���h�E�N���X�o�^�p�̍\����          */
	Config*			config;
	RECT			Rect;

	// ���d�N���h�~
	HWND hWnd = FindWindow(IDC_STATIC_WINDOWCLASSNAME, NULL);

	if(hWnd){
		// �A�C�R��������Ă���Ό��̃T�C�Y�ɖ߂�
		if(IsIconic(hWnd))
			ShowWindow(hWnd, SW_RESTORE);

		// ���������E�B���h�E���t�H�A�O���E���h�ɂ���
		SetForegroundWindow(::GetLastActivePopup(hWnd));

		return FALSE;
	}

	wc.style        =0;
	wc.lpfnWndProc  =WindowProc;
	wc.cbClsExtra   =0;
	wc.cbWndExtra   =0;
	wc.hInstance    =hInstance;
	wc.hIcon        =LoadIcon(hInstance, TEXT("APP_ICON"));
	wc.hCursor      =LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName =NULL;
	wc.lpszClassName=IDC_STATIC_WINDOWCLASSNAME;

	if(RegisterClass(&wc)==0) // �E�C���h�E�N���X�o�^
		return FALSE;

	/* �ݒ�t�@�C������ݒ��ǂݍ��� */
	config = new Config();

	if(config->readWindowRect(&Rect) != OK || config->readInterval(&_interval) != OK){
		MessageBox(NULL, TEXT("INI�t�@�C�����ُ�ł�"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		delete(config);
		return FALSE;
	}

	delete(config);

	// �P��f�[�^�N���X�C���X�^���X�쐬�Ə�����
	_wordData = new WordData();

	if(_wordData->initialize(DATAFILENAME) != TRUE){
		return FALSE;
	}

	// �E�C���h�E�쐬
	hWnd = CreateWindow(IDC_STATIC_WINDOWCLASSNAME,
						IDC_STATIC_APPNAME,
						WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
						Rect.left,
						Rect.top,
						(Rect.right - Rect.left),
						(Rect.bottom - Rect.top),
						NULL,
						(HMENU)NULL,
						hInstance,
						0);
	if(hWnd == NULL)
		return FALSE;

	// �C���^�[�o�� 1000ms �Ń^�C�}�[�Z�b�g
	SetTimer(hWnd, 1, 1000, NULL);

	ShowWindow(hWnd, CmdShow);	// �E�C���h�E�̕\��
	UpdateWindow(hWnd);			// �E�C���h�E�̍ŏ��̍X�V
	deleteMenu(hWnd);			// �V�X�e�����j���[�폜
	addMenu(hWnd);				// ���j���[�ǉ�

	// ���b�Z�[�W���[�v
	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

/* �`�� */
void OnPaint(HWND hWnd){

	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hWnd , &ps);

	// �w�i�F�����ɐݒ�
	SetBkColor(hdc, RGB(0, 0, 0));

	// �̈�����œh��Ԃ�
	RECT Rect;
	GetWindowRect(hWnd, (LPRECT)&Rect);

	SelectObject(hdc , GetStockObject(BLACK_BRUSH));
	Rectangle(hdc, 0, 0, (Rect.right - Rect.left), Rect.bottom - Rect.top);

	// �P���`��
	if(_tick >= 0){
		HFONT hFont = CreateFont(
				26, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
				SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				VARIABLE_PITCH | FF_ROMAN , _T("Segoe UI Semibold"));

		SelectObject(hdc, hFont);
		SetTextColor(hdc, RGB(255, 255, 0));
		TextOut(hdc, 5, 0, _wordData->getWord(_index), _wordData->getWordLen(_index));

		DeleteObject(hFont);
	}

	// �Ӗ���`��
	if(_tick >= _interval){
		HFONT hFont = CreateFont(
				22, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
				SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				VARIABLE_PITCH | FF_ROMAN , _T("Meiryo UI"));

		SelectObject(hdc, hFont);
		SetTextColor(hdc, RGB(255, 255, 255));
		TextOut(hdc, 5, 24, _wordData->getMean(_index), _wordData->getMeanLen(_index));

		DeleteObject(hFont);
	}

	EndPaint(hWnd , &ps);
}

/*  �E�C���h�E�v���V�[�W��  */
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam){

	switch(message){

		// �`��C�x���g
		case WM_PAINT:

			OnPaint(hWnd);
			return 0;

		// �^�C�}�[�C�x���g������
        case WM_TIMER:

			OnTimer(hWnd);
			return 0;

		// �}�E�X���{�^���N���b�N��
		case WM_LBUTTONDOWN:

			if(_pause){
				_pause = false;
				SetWindowText(hWnd, IDC_STATIC_APPNAME);
			}else{
				_pause = true;
				SetWindowText(hWnd, IDC_STATIC_APPNAME IDC_STATIC_PAUSESTR);
			}

			return 0;

		case WM_CLOSE:
			RECT Rect;

			ShowWindow(hWnd, SW_HIDE);
			ShowWindow(hWnd, SW_RESTORE);

			// �E�B���h�E�T�C�Y�̐ݒ��ۑ�
			GetWindowRect(hWnd, (LPRECT)&Rect);

			// �ݒ�t�@�C���N���X
			Config* config;

			config = new Config();
			config->writeWindowRect(&Rect);
			config->writeInterval(_interval);

			delete(config);

			DestroyWindow(hWnd);

		case WM_DESTROY:

			PostQuitMessage(0);
			return  0;

		// ���j���[
		case WM_SYSCOMMAND:

			switch(wparam){
				// About
				case IDM_FLASHWORD_ABOUT:
					MessageBox(hWnd, IDC_STATIC_COPYRIGHT,
								IDC_STATIC_APPNAME, MB_ICONINFORMATION | MB_OK);
					break;
				// �f�[�^�t�@�C�����J��
				case IDM_FLASHWORD_OPENDATAFILE:

					TCHAR szDataFileName[_MAX_PATH];

					// �f�[�^�t�@�C�����擾
					if(_wordData->getDataFileName(szDataFileName, _countof(szDataFileName), DATAFILENAME) != TRUE){
						MessageBox(NULL, TEXT("�f�[�^�t�@�C�����擾���s"),
									IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
						break;
					}

					// �f�[�^�t�@�C���I�[�v��
					ShellExecute(hWnd, NULL, szDataFileName, NULL, NULL, SW_SHOWNORMAL);

					break;
				// �f�[�^�ēǍ�
				case IDM_FLASHWORD_RELOAD:
					reload(hWnd);
					MessageBox(hWnd, TEXT("�f�[�^�t�@�C���ēǍ�����"),
								IDC_STATIC_APPNAME, MB_ICONINFORMATION | MB_OK);
					break;
				default:
					return DefWindowProc(hWnd, message, wparam, lparam);
					break;
			}
	}

	return  DefWindowProc(hWnd, message, wparam, lparam);
}

/* �^�C�}�[�C�x���g */
void OnTimer(HWND hWnd){

//char buf[100];
//sprintf(buf, "index=%d tick=%d\n", _index, _tick);
//OutputDebugString(buf);

	// �ꎞ��~��
	if(_pause){
		return;
	}

	if(_tick == 0 || _tick == _interval){
		// �X�V�̈�ǉ� (WM_PAINT ���Ă΂��)
		InvalidateRect(hWnd, NULL, FALSE);
	}

	// �J�E���^�[����
	_tick++;

	// ���̒P��
	if(_tick >= _interval*2){
		_tick = 0;
		_index++;
		if(_index >= _wordData->getTotal()){
			_index = 0;
			_wordData->doShuffle();	// ��������V���b�t��
		}
	}
}

/* �V�X�e�����j���[���폜
   HWND hWnd �E�B���h�E�n���h��
*/
void deleteMenu(HWND hWnd){

	HMENU hMenu;

	// �V�X�e�����j���[�n���h�����擾
	hMenu = GetSystemMenu(hWnd, FALSE);

	// �T�C�Y�ύX�E�ő剻�E�ŏ����E�ړ��E���ɖ߂����j���[���폜
	DeleteMenu(hMenu, SC_SIZE, MF_BYCOMMAND);
//	DeleteMenu(hMenu, SC_MOVE, MF_BYCOMMAND);
	DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
//	DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
	DeleteMenu(hMenu, SC_RESTORE, MF_BYCOMMAND);
}

/* ���j���[��ǉ�
   HWND hWnd �E�B���h�E�n���h��
*/
void addMenu(HWND hWnd){

	HMENU hMenu;
	MENUITEMINFO menuInfo;

	hMenu = GetSystemMenu(hWnd, FALSE);

	menuInfo.cbSize = sizeof(MENUITEMINFO);
	menuInfo.fMask = MIIM_TYPE | MIIM_ID;

	// About
	menuInfo.wID = IDM_FLASHWORD_ABOUT;
	menuInfo.fType = MFT_STRING;
	menuInfo.dwTypeData = TEXT("FlashWord�ɂ���(&A)");

	InsertMenuItem(hMenu, 0, TRUE, &menuInfo);

	// �f�[�^�t�@�C���ēǂݍ���
	menuInfo.wID = IDM_FLASHWORD_RELOAD;
	menuInfo.dwTypeData = TEXT("�f�[�^�ēǍ�(&R)");
	InsertMenuItem(hMenu, 0, TRUE, &menuInfo);

	// �f�[�^�t�@�C�����J��
	menuInfo.wID = IDM_FLASHWORD_OPENDATAFILE;
	menuInfo.dwTypeData = TEXT("�f�[�^�t�@�C�����J��(&O)");
	InsertMenuItem(hMenu, 0, TRUE, &menuInfo);
}

/* �����[�h����
   HWND hWnd �E�B���h�E�n���h��
*/
void reload(HWND hWnd){

	// �^�C�}�[��~
	KillTimer(hWnd, 1);

	_tick = -1;

	// �������J��
	_wordData->finalize();

	// �ď�����
	if(_wordData->initialize(DATAFILENAME) != TRUE){
		exit(-1);
	}

	_tick = 0;

	// �C���^�[�o�� 1000ms �Ń^�C�}�[�Z�b�g
	SetTimer(hWnd, 1, 1000, NULL);
}
