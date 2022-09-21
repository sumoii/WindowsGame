#include<Windows.h>
#include<time.h>
#pragma comment (lib,"winmm.lib")

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600	

HDC				g_hdc = NULL, g_mdc = NULL;
HBITMAP		g_hBitmap = NULL;

BOOL Game_Init(HWND hwnd);
VOID Game_Paint(HWND hwnd);
BOOL Game_CleanUp(HWND hwnd);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";

	if (!RegisterClassEx(&wndClass))
		return -1;

	HWND hWnd = CreateWindow(L"ForTheDreamOfGameDevelop", L"致我们永不熄灭的游戏开发梦想！",WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
	
	MoveWindow(hWnd, 200, 50, 800, 600, true);
	ShowWindow( hWnd, 1 );
	UpdateWindow(hWnd);

	if (!Game_Init(hWnd)) 
	{
		MessageBox(hWnd, L"资源初始化失败",L"消息窗口",0);
		return FALSE;
	}
	
	PlaySound(L"緊迫.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	MSG msg = { 0 };
	while ( msg.message != WM_QUIT ) 
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;

	switch (message)
	{
	case WM_PAINT:
		g_hdc = BeginPaint(hwnd, &paintStruct);
		Game_Paint(hwnd);
		EndPaint(hwnd, &paintStruct);
		ValidateRect(hwnd, NULL);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		Game_CleanUp(hwnd);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	
	return 0;
}

BOOL Game_Init(HWND hwnd) 
{
	g_hdc = GetDC(hwnd);
	g_hBitmap = (HBITMAP)LoadImage(NULL, L"Naruto.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_mdc = CreateCompatibleDC(g_hdc);
	Game_Paint(hwnd);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}

VOID Game_Paint( HWND hwnd)
{
	SelectObject(g_mdc, g_hBitmap);
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);
}

BOOL Game_CleanUp(HWND hwnd)
{
	DeleteObject(g_hBitmap);
	DeleteDC(g_mdc);
	return TRUE;
}