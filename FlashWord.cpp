/*
 * FlashWord
 *  $Id$
 *
 * Copyright (C) 2007-2014, Toshi All rights reserved.
*/
#include	"FlashWord.h"

/*  アプリケーションエントリーポイント  */
int WINAPI WinMain(HINSTANCE    hInstance,
                   HINSTANCE    hPrevInstance,
                   LPSTR        CmdLine,
                   int          CmdShow){

	MSG             msg;    /*  メッセージキューから取得したメッセージ  */
	WNDCLASS        wc;     /*  ウインドウクラス登録用の構造体          */
	Config*			config;
	RECT			Rect;

	// 多重起動防止
	HWND hWnd = FindWindow(IDC_STATIC_WINDOWCLASSNAME, NULL);

	if(hWnd){
		// アイコン化されていれば元のサイズに戻す
		if(IsIconic(hWnd))
			ShowWindow(hWnd, SW_RESTORE);

		// 見つかったウィンドウをフォアグラウンドにする
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

	if(RegisterClass(&wc)==0) // ウインドウクラス登録
		return FALSE;

	/* 設定ファイルから設定を読み込み */
	config = new Config();

	if(config->readWindowRect(&Rect) != OK || config->readInterval(&_interval) != OK){
		MessageBox(NULL, TEXT("INIファイルが異常です"),
					IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
		delete(config);
		return FALSE;
	}

	delete(config);

	// 単語データクラスインスタンス作成と初期化
	_wordData = new WordData();

	if(_wordData->initialize(DATAFILENAME) != TRUE){
		return FALSE;
	}

	// ウインドウ作成
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

	// インターバル 1000ms でタイマーセット
	SetTimer(hWnd, 1, 1000, NULL);

	ShowWindow(hWnd, CmdShow);	// ウインドウの表示
	UpdateWindow(hWnd);			// ウインドウの最初の更新
	deleteMenu(hWnd);			// システムメニュー削除
	addMenu(hWnd);				// メニュー追加

	// メッセージループ
	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

/* 描画 */
void OnPaint(HWND hWnd){

	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hWnd , &ps);

	// 背景色を黒に設定
	SetBkColor(hdc, RGB(0, 0, 0));

	// 領域を黒で塗りつぶす
	RECT Rect;
	GetWindowRect(hWnd, (LPRECT)&Rect);

	SelectObject(hdc , GetStockObject(BLACK_BRUSH));
	Rectangle(hdc, 0, 0, (Rect.right - Rect.left), Rect.bottom - Rect.top);

	// 単語を描画
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

	// 意味を描画
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

/*  ウインドウプロシージャ  */
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam){

	switch(message){

		// 描画イベント
		case WM_PAINT:

			OnPaint(hWnd);
			return 0;

		// タイマーイベント発生時
        case WM_TIMER:

			OnTimer(hWnd);
			return 0;

		// マウス左ボタンクリック時
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

			// ウィンドウサイズの設定を保存
			GetWindowRect(hWnd, (LPRECT)&Rect);

			// 設定ファイルクラス
			Config* config;

			config = new Config();
			config->writeWindowRect(&Rect);
			config->writeInterval(_interval);

			delete(config);

			DestroyWindow(hWnd);

		case WM_DESTROY:

			PostQuitMessage(0);
			return  0;

		// メニュー
		case WM_SYSCOMMAND:

			switch(wparam){
				// About
				case IDM_FLASHWORD_ABOUT:
					MessageBox(hWnd, IDC_STATIC_COPYRIGHT,
								IDC_STATIC_APPNAME, MB_ICONINFORMATION | MB_OK);
					break;
				// データファイルを開く
				case IDM_FLASHWORD_OPENDATAFILE:

					TCHAR szDataFileName[_MAX_PATH];

					// データファイル名取得
					if(_wordData->getDataFileName(szDataFileName, _countof(szDataFileName), DATAFILENAME) != TRUE){
						MessageBox(NULL, TEXT("データファイル名取得失敗"),
									IDC_STATIC_APPNAME, MB_ICONSTOP | MB_OK);
						break;
					}

					// データファイルオープン
					ShellExecute(hWnd, NULL, szDataFileName, NULL, NULL, SW_SHOWNORMAL);

					break;
				// データ再読込
				case IDM_FLASHWORD_RELOAD:
					reload(hWnd);
					MessageBox(hWnd, TEXT("データファイル再読込完了"),
								IDC_STATIC_APPNAME, MB_ICONINFORMATION | MB_OK);
					break;
				default:
					return DefWindowProc(hWnd, message, wparam, lparam);
					break;
			}
	}

	return  DefWindowProc(hWnd, message, wparam, lparam);
}

/* タイマーイベント */
void OnTimer(HWND hWnd){

//char buf[100];
//sprintf(buf, "index=%d tick=%d\n", _index, _tick);
//OutputDebugString(buf);

	// 一時停止時
	if(_pause){
		return;
	}

	if(_tick == 0 || _tick == _interval){
		// 更新領域追加 (WM_PAINT が呼ばれる)
		InvalidateRect(hWnd, NULL, FALSE);
	}

	// カウンター増加
	_tick++;

	// 次の単語
	if(_tick >= _interval*2){
		_tick = 0;
		_index++;
		if(_index >= _wordData->getTotal()){
			_index = 0;
			_wordData->doShuffle();	// 乱数列をシャッフル
		}
	}
}

/* システムメニューを削除
   HWND hWnd ウィンドウハンドル
*/
void deleteMenu(HWND hWnd){

	HMENU hMenu;

	// システムメニューハンドルを取得
	hMenu = GetSystemMenu(hWnd, FALSE);

	// サイズ変更・最大化・最小化・移動・元に戻すメニューを削除
	DeleteMenu(hMenu, SC_SIZE, MF_BYCOMMAND);
//	DeleteMenu(hMenu, SC_MOVE, MF_BYCOMMAND);
	DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
//	DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
	DeleteMenu(hMenu, SC_RESTORE, MF_BYCOMMAND);
}

/* メニューを追加
   HWND hWnd ウィンドウハンドル
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
	menuInfo.dwTypeData = TEXT("FlashWordについて(&A)");

	InsertMenuItem(hMenu, 0, TRUE, &menuInfo);

	// データファイル再読み込み
	menuInfo.wID = IDM_FLASHWORD_RELOAD;
	menuInfo.dwTypeData = TEXT("データ再読込(&R)");
	InsertMenuItem(hMenu, 0, TRUE, &menuInfo);

	// データファイルを開く
	menuInfo.wID = IDM_FLASHWORD_OPENDATAFILE;
	menuInfo.dwTypeData = TEXT("データファイルを開く(&O)");
	InsertMenuItem(hMenu, 0, TRUE, &menuInfo);
}

/* リロード処理
   HWND hWnd ウィンドウハンドル
*/
void reload(HWND hWnd){

	// タイマー停止
	KillTimer(hWnd, 1);

	_tick = -1;

	// メモリ開放
	_wordData->finalize();

	// 再初期化
	if(_wordData->initialize(DATAFILENAME) != TRUE){
		exit(-1);
	}

	_tick = 0;

	// インターバル 1000ms でタイマーセット
	SetTimer(hWnd, 1, 1000, NULL);
}
