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
    ghWnd = CreateWindow(szClassNme,	// class��
		_T("Hello GDI DoubleBuffer !! ����ɂ��͂f�c�h�_�u���o�b�t�@�I"),		// �^�C�g��
        WS_OVERLAPPEDWINDOW,		// Style
        CW_USEDEFAULT,				// X
        CW_USEDEFAULT,				// Y
        CW_USEDEFAULT,				// Width
        CW_USEDEFAULT,				// Height
        NULL,						// �e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
        NULL,						// ���j���[�n���h���܂��͎q�E�B���h�E ID
        hInstance,					// �A�v���P�[�V�����C���X�^���X�̃n���h��
        NULL);						// �E�B���h�E�쐬�f�[�^
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

	LPTSTR lptStr = _T("Hello GDI DoubleBuffer. ����ɂ���GDI�_�u���o�b�t�@�B");

    switch (msg) {		
		case WM_TIMER:
			// �w�i����������
			SelectObject(memoryHDC, GetStockObject(WHITE_BRUSH));
			PatBlt(memoryHDC, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), PATCOPY);

			// �������f�o�C�X�R���e�L�X�g�ɑ΂��ĕ`�悷��
			TextOut(memoryHDC , 10 , 10 , lptStr , lstrlen(lptStr));
			
			// �ĕ`��
			InvalidateRect(ghWnd, NULL, FALSE);
			break;
		case WM_PAINT:
			hdc = BeginPaint(ghWnd , &ps);

			// �������f�o�C�X�R���e�L�X�g����A���ۂ̃f�o�C�X�R���e�L�X�g�փR�s�[����
			BitBlt(hdc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), memoryHDC, 0, 0, SRCCOPY);

			EndPaint(ghWnd , &ps);
			break;
		case WM_ERASEBKGND:
			return TRUE;
		case WM_CREATE:
			hdc = GetDC(ghWnd);

			// �������f�o�C�X�R���e�L�X�g���쐬����
			memoryHDC = CreateCompatibleDC(hdc);

			memoryHBMP = CreateCompatibleBitmap(hdc, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			SelectObject(memoryHDC, memoryHBMP);

			// ��������Ȃ����Ƃ��m�F���邽��10�~���b���Ƃɍĕ`�悳����
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
