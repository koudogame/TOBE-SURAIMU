#include "direct3d.h"
#include "sprite.h"
#include "commonstates.h"
#include "key.h"

//#include "summary.h"
#include "pad.h"

//  プロトタイプ宣言
LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef  _DEBUG
	//メモリリークのチェック
	//_CrtSetBreakAlloc(164);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//多重起動を防ぐ
	//ミューテックスの作成
	HANDLE mutex = CreateMutex(NULL, false, "Creator_abyabyabyabya_Spring");

	//多重起動のチェック
	//起動している場合
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return 0;

	srand((unsigned)time(NULL));

	//  ウィンドウクラスの作成
	WNDCLASSEX wnd;
	ZeroMemory(&wnd, sizeof(WNDCLASSEX));

	wnd.cbSize = sizeof(WNDCLASSEX);                                      //  構造体のサイズ
	wnd.style = CS_VREDRAW;                                                 //  スタイル
	wnd.hInstance = hInstance;                                          //  アプリケーションインスタンス
	wnd.lpszClassName = "ClassName";                                        //  クラス名
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);                            //  カーソル形状
	wnd.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);         //  デフォルトの背景色
	wnd.lpfnWndProc = WinProc;                                              //  ウィンドウプロシージャへの関数ポインタ

	//  登録
	if (!RegisterClassEx(&wnd))
	{
		//  エラー
		return 1;
	}

	//  ウィンドウサイズを決定
	RECT rect;
	rect.left = rect.top = 0L;
	rect.right = getWindowWidth<long>();
	rect.bottom = getWindowHeight<long>();

	const DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
	const DWORD ex_style = WS_EX_OVERLAPPEDWINDOW;
	AdjustWindowRectEx(&rect, style, false, ex_style);            //  ウィンドウの枠を考慮した画面を表示

	//  ウィンドウの作成
	const HWND hWnd = CreateWindowEx(
		ex_style,                   //  拡張ウィンドウスタイル
		"ClassName",                //  クラス名
		"StarProject",              //  ウィンドウタイトル
		style,                      //  ウィンドウスタイル
		CW_USEDEFAULT,              //  表示座標_x
		CW_USEDEFAULT,              //  表示座標_y
		rect.right - rect.left,     //  ウィンドウ横幅
		rect.bottom - rect.top,     //  ウィンドウ縦幅
		NULL,                       //  親ウィンドウへのハンドル
		NULL,                       //  メニューハンドル( つけると重くなる )
		hInstance,                  //  アプリケーションインスタンス
		NULL);                     //  ウィンドウ作成データ
		// NULLチェック
	if (hWnd == NULL)
	{
		// エラー
		return 0;
	}

	// COMライブラリの初期化
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		// エラー
		return 0;
	}

	// Direct3D初期化
	if (!Direct3D::getInstance()->init(hWnd))
	{  // エラー
		return 0;
	}

	// Spriteクラス初期化
	if (!Sprite::getInstance()->init())
	{   // エラー
		return 0;
	}

	// Commonクラス初期化
	if (!Common::getInstance()->init())
		// エラー
		return 0;

	// ウィンドウの表示
	ShowWindow(hWnd, SW_SHOWNORMAL);

	// メインループ
	MSG msg = { NULL };

	// 時間計測
	DWORD t1, t2, t3 = 0L, dt;
	t1 = timeGetTime();
	t2 = timeGetTime();

	/***********************************/

	/*Summary game;
	if (!game.init())
		return 0;

	/***************************************/

	while (msg.message != WM_QUIT)
	{
		// メッセージ処理
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);    // コールバック関数へ
		}
		else
		{
			// フレームレート制御
			t1 = timeGetTime();     // 現在の時間
			dt = (t1 - t2) + t3;    // 前回の更新からの差分を計算

			// 約16ミリ秒経過していたら処理を行う
			if (dt > 16L)
			{
				t2 = t1;            // 今の時刻を前回の時間とする
				t3 = dt % 16L;      // 誤差( オーバー )分を保存

				Key::getInstance()->update();
				Pad::getInstance()->update();

				// ゲーム処理
				Direct3D::getInstance()->clear();

				/*if (!game.update())
				{
					break;
				}*/
				// スプライト描画開始
				Sprite::getInstance()->begin();

				// ゲーム画面描画
				//game.draw();

				// スプライト描画終了
				Sprite::getInstance()->end();

				// 描画更新
				Direct3D::getInstance()->present();
			}
		}
	}

	// COMライブラリの開放.
	CoUninitialize();
	// インターフェイスの開放
	//game.destroy();
	Direct3D::getInstance()->destroy();

	return 0;
}

// コールバック関数
LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (Msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYUP:
		Keyboard::ProcessMessage(Msg, wParam, lParam);
		break;

	case WM_KEYDOWN:
		Keyboard::ProcessMessage(Msg, wParam, lParam);
		switch (wParam)
		{
		case VK_ESCAPE:
			// Escが押されたらウィンドウを閉じる
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}