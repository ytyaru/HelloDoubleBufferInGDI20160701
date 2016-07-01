#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include <sti.h>
#include <gdiplus.h>
using namespace Gdiplus;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR szClassNme[] = _T("HelloDoubleBufferInGDI20160701");

HWND ghWnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst,
                   LPSTR lpszCmdLine, int nCmdShow)
{
    MSG msg;
    WNDCLASS myProg;
    if (!hPreInst) {
        myProg.style            = CS_HREDRAW | CS_VREDRAW;
        myProg.lpfnWndProc      = WndProc;
        myProg.cbClsExtra       = 0;
        myProg.cbWndExtra       = 0;
        myProg.hInstance        = hInstance;
        myProg.hIcon            = NULL;
        myProg.hCursor          = LoadCursor(NULL, IDC_ARROW);
		myProg.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
        myProg.lpszMenuName     = NULL;
        myProg.lpszClassName    = szClassNme;
        if (!RegisterClass(&myProg))
            return FALSE;
    }
    ghWnd = CreateWindow(szClassNme,	// class名
		_T("Hello GDI DoubleBuffer !! こんにちはＧＤＩダブルバッファ！"),		// タイトル
        WS_OVERLAPPEDWINDOW,		// Style
        CW_USEDEFAULT,				// X
        CW_USEDEFAULT,				// Y
        CW_USEDEFAULT,				// Width
        CW_USEDEFAULT,				// Height
        NULL,						// 親ウィンドウまたはオーナーウィンドウのハンドル
        NULL,						// メニューハンドルまたは子ウィンドウ ID
        hInstance,					// アプリケーションインスタンスのハンドル
        NULL);						// ウィンドウ作成データ
    ShowWindow(ghWnd, nCmdShow);
    UpdateWindow(ghWnd);
	
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    static HDC memoryHDC;
    static HBITMAP memoryHBMP;

	LPTSTR lptStr = _T("Hello GDI DoubleBuffer. こんにちはGDIダブルバッファ。");

    switch (msg) {		
		case WM_TIMER:
			// 背景を消去する
			SelectObject(memoryHDC, GetStockObject(WHITE_BRUSH));
			PatBlt(memoryHDC, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), PATCOPY);

			// メモリデバイスコンテキストに対して描画する
			TextOut(memoryHDC , 10 , 10 , lptStr , lstrlen(lptStr));
			
			// 再描画
			InvalidateRect(ghWnd, NULL, FALSE);
			break;
		case WM_PAINT:
			hdc = BeginPaint(ghWnd , &ps);

			// メモリデバイスコンテキストから、実際のデバイスコンテキストへコピーする
			BitBlt(hdc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), memoryHDC, 0, 0, SRCCOPY);

			EndPaint(ghWnd , &ps);
			break;
		case WM_ERASEBKGND:
			return TRUE;
		case WM_CREATE:
			hdc = GetDC(ghWnd);

			// メモリデバイスコンテキストを作成する
			memoryHDC = CreateCompatibleDC(hdc);

			memoryHBMP = CreateCompatibleBitmap(hdc, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			SelectObject(memoryHDC, memoryHBMP);

			// ちらつきがないことを確認するため10ミリ秒ごとに再描画させる
			SetTimer(hWnd, 1, 10, (TIMERPROC)NULL);
			break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return(DefWindowProc(hWnd, msg, wParam, lParam));
    }
    return (0L);
}
