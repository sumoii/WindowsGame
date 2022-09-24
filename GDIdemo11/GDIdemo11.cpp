//-----------------------------------【程序说明】----------------------------------------------
//  程序名称:：GDIdemoCore
//	 2013年3月 Create by 浅墨
//  描述：实现GDI游戏开发所需的核心程序
//------------------------------------------------------------------------------------------------

//-----------------------------------【头文件包含部分】---------------------------------------
//	描述：包含程序所依赖的头文件
//------------------------------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>//使用swprintf_s函数所需的头文件

//-----------------------------------【库文件包含部分】---------------------------------------
//	描述：包含程序所依赖的库文件
//------------------------------------------------------------------------------------------------
#pragma comment(lib,"winmm.lib")			//调用PlaySound函数所需库文件
#pragma  comment(lib,"Msimg32.lib")		//添加使用TransparentBlt函数所需的库文件

//-----------------------------------【宏定义部分】--------------------------------------------
//	描述：定义一些辅助宏
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	800							//为窗口宽度定义的宏，以方便在此处修改窗口宽度
#define WINDOW_HEIGHT	600							//为窗口高度定义的宏，以方便在此处修改窗口高度
#define WINDOW_TITLE	L"【致我们永不熄灭的游戏开发梦想】GDI程序核心框架"	//为窗口标题定义的宏

struct SwordBullets //SwordBullets 结构体代表剑气（子弹）
{
	int x, y; //光剑气(子弹坐标)
	bool exist; //光剑气 是否存在
};

//-----------------------------------【全局变量声明部分】-------------------------------------
//	描述：全局变量的声明
//------------------------------------------------------------------------------------------------
HDC		g_hdc = NULL,g_mdc = NULL,g_bufdc=NULL;       //全局设备环境句柄与全局内存DC句柄
HBITMAP g_hSwordMan = NULL, g_hSwordBlade = NULL, g_hBackGround = NULL;
DWORD g_tPre = 0, g_tNow = 0;//声明两个函数来记录时间
int g_iX = 0, g_iY = 0, g_iXnow = 0, g_iYnow = 0; //g_iX,g_iY代表鼠标光标所在位置，g_iXnow,g_iYnow,代表当前人物所在坐标
int g_iBGOffset = 0, g_iBulletNum = 0;//g_iBGOffset为滚动背景所要裁剪的区域宽度，

SwordBullets Bullet[30];//用来存储人物发出的子弹

//-----------------------------------【全局函数声明部分】-------------------------------------
//	描述：全局函数声明，防止“未声明的标识”系列错误
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK	WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//窗口过程函数
BOOL						Game_Init(HWND hwnd);			//在此函数中进行资源的初始化
VOID							Game_Paint(HWND hwnd);		//在此函数中进行绘图代码的书写
BOOL						Game_CleanUp(HWND hwnd);	//在此函数中进行资源的清理

//-----------------------------------【WinMain( )函数】--------------------------------------
//	描述：Windows应用程序的入口函数，我们的程序从这里开始
//------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//【1】窗口创建四步曲之一：开始设计一个完整的窗口类
	WNDCLASSEX wndClass = { 0 };							//用WINDCLASSEX定义了一个窗口类
	wndClass.cbSize = sizeof(WNDCLASSEX);			//设置结构体的字节数大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//设置窗口的样式
	wndClass.lpfnWndProc = WndProc;					//设置指向窗口过程函数的指针
	wndClass.cbClsExtra = 0;								//窗口类的附加内存，取0就可以了
	wndClass.cbWndExtra = 0;							//窗口的附加内存，依然取0就行了
	wndClass.hInstance = hInstance;						//指定包含窗口过程的程序的实例句柄。
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //本地加载自定义ico图标
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //指定窗口类的光标句柄。
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);  //为hbrBackground成员指定一个灰色画刷句柄	
	wndClass.lpszMenuName = NULL;						//用一个以空终止的字符串，指定菜单资源的名字。
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		//用一个以空终止的字符串，指定窗口类的名字。

	//【2】窗口创建四步曲之二：注册窗口类
	if (!RegisterClassEx(&wndClass))				//设计完窗口后，需要对窗口类进行注册，这样才能创建该类型的窗口
		return -1;

	//【3】窗口创建四步曲之三：正式创建窗口
	HWND hwnd = CreateWindow(L"ForTheDreamOfGameDevelop", WINDOW_TITLE,				//喜闻乐见的创建窗口函数CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//【4】窗口创建四步曲之四：窗口的移动、显示与更新
	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);		//调整窗口显示时的位置，使窗口左上角位于（250,80）处
	ShowWindow(hwnd, nShowCmd);    //调用ShowWindow函数来显示窗口
	UpdateWindow(hwnd);						//对窗口进行更新，就像我们买了新房子要装修一样

	//游戏资源的初始化，若初始化失败，弹出一个消息框，并返回FALSE
	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"资源初始化失败", L"消息窗口", 0); //使用MessageBox函数，创建一个消息窗口
		return FALSE;
	}

	PlaySound(L"仙剑三·原版战斗3.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); //循环播放背景音乐 

	//【5】消息循环过程
	MSG msg = { 0 };				//定义并初始化msg
	while (msg.message != WM_QUIT)		//使用while循环，如果消息不是WM_QUIT消息，就继续循环
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //查看应用程序消息队列，有消息时将队列中的消息派发出去。
		{
			TranslateMessage(&msg);		//将虚拟键消息转换为字符消息
			DispatchMessage(&msg);			//分发一个消息给窗口程序。
		}
		else
		{
			g_tNow = GetTickCount();
			if (g_tNow - g_tPre >= 5)
				Game_Paint(hwnd);
		}
	}

	//【6】窗口类的注销
	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);  //程序准备结束，注销窗口类
	return 0;
}

//-----------------------------------【WndProc( )函数】--------------------------------------
//	描述：窗口过程函数WndProc,对窗口消息进行处理
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;			//定义一个PAINTSTRUCT结构体来记录一些绘制信息

	switch (message)						//switch语句开始
	{
	case WM_PAINT:						// 若是客户区重绘消息
		g_hdc = BeginPaint(hwnd, &paintStruct);  //指定窗口进行绘图工作的准备，并用将和绘图有关的信息填充到paintStruct结构体中。
		Game_Paint(hwnd);
		EndPaint(hwnd, &paintStruct);			//EndPaint函数标记指定窗口的绘画过程结束
		ValidateRect(hwnd, NULL);		// 更新客户区的显示
		break;									//跳出该switch语句

	case WM_KEYDOWN:					// 若是键盘按下消息
		if (wParam == VK_ESCAPE)    // 如果被按下的键是ESC
			DestroyWindow(hwnd);		// 销毁窗口, 并发送一条WM_DESTROY消息
			PostQuitMessage(0); //结束程序
		break;									//跳出该switch语句

	case WM_DESTROY:					//若是窗口销毁消息
		Game_CleanUp(hwnd);			//调用自定义的资源清理函数Game_CleanUp（）进行退出前的资源清理
		PostQuitMessage(0);			//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
		break;									//跳出该switch语句

	case WM_LBUTTONDOWN:
		for (int i = 0; i < 30; i++)
		{
			if(!Bullet[i].exist)
			{
				Bullet[i].x = g_iXnow;
				Bullet[i].y = g_iYnow + 30;
				Bullet[i].exist = true;
				g_iBulletNum++;
				break;
			}
		}

	case WM_MOUSEMOVE:	
		g_iX = LOWORD(lParam);//取得鼠标X坐标
		if (g_iX > WINDOW_WIDTH - 317)
			g_iX = WINDOW_WIDTH - 317;
		else if (g_iX < 0)
			g_iX = 0;
		g_iY = HIWORD(lParam);
		if (g_iY > WINDOW_HEIGHT - 283)
			g_iY = WINDOW_HEIGHT - 283;
		else if (g_iY < -200)
			g_iY = -200;
		break;


	default:										//若上述case条件都不符合，则执行该default语句
		return DefWindowProc(hwnd, message, wParam, lParam);		//调用缺省的窗口过程
	}

	return 0;									//正常退出
}

//-----------------------------------【Game_Init( )函数】--------------------------------------
//	描述：初始化函数，进行一些简单的初始化
//------------------------------------------------------------------------------------------------
BOOL Game_Init(HWND hwnd)
{
	HBITMAP bmp;
	g_hdc = GetDC(hwnd);
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufdc = CreateCompatibleDC(g_hdc);
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	g_iX = 300;
	g_iY = 100;
	g_iXnow = 300;
	g_iYnow = 100;
	SelectObject(g_mdc, bmp);
	g_hSwordMan = (HBITMAP)LoadImage(NULL, L"swordman.bmp", IMAGE_BITMAP, 317, 283, LR_LOADFROMFILE);
	g_hSwordBlade = (HBITMAP)LoadImage(NULL, L"swordblade.bmp", IMAGE_BITMAP, 100, 26, LR_LOADFROMFILE);
	g_hBackGround = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	POINT pt, lt, rb;
	RECT rect;
	pt.x = 300;
	pt.y = 100;
	ClientToScreen(hwnd, &pt);
	SetCursorPos(pt.x, pt.y);
	ShowCursor(false);
	//限制鼠标光标移动区域
	GetClientRect(hwnd, &rect);//取得窗口内部矩形
	lt.x = rect.left;
	lt.y = rect.top;
	rb.x = rect.right;
	rb.y = rect.bottom;
	ClientToScreen(hwnd, &lt);
	ClientToScreen(hwnd, &rb);
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	ClipCursor(&rect);
	Game_Paint(hwnd);
	return TRUE;
}

//-----------------------------------【Game_Paint( )函数】--------------------------------------
//	描述：绘制函数，在此函数中进行绘制操作
//--------------------------------------------------------------------------------------------------
VOID Game_Paint(HWND hwnd)
{
	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, g_iBGOffset, WINDOW_HEIGHT, g_bufdc, WINDOW_WIDTH - g_iBGOffset, 0, SRCCOPY);
	BitBlt(g_mdc,g_iBGOffset, 0,WINDOW_WIDTH - g_iBGOffset,WINDOW_HEIGHT,g_bufdc, 0,0, SRCCOPY);

	wchar_t str[20] = {};

	if (g_iXnow < g_iX)
	{
		g_iXnow += 10;
		if (g_iXnow > g_iX)
			g_iXnow = g_iX;
	}
	else
	{
		g_iXnow -= 10;
		if (g_iXnow < g_iX)
			g_iXnow = g_iX;
	}

	if (g_iYnow < g_iY)
	{
		g_iYnow += 10;
		if (g_iYnow > g_iY)
			g_iYnow = g_iY;
	}
	else
	{
		g_iYnow -= 10;
		if (g_iYnow < g_iY)
			g_iYnow = g_iY;
	}

	SelectObject(g_bufdc, g_hSwordMan);
	TransparentBlt(g_mdc, g_iXnow, g_iYnow, 317, 283, g_bufdc, 0, 0, 317, 283,RGB(0,0,0));
	SelectObject(g_bufdc, g_hSwordBlade);
	if(g_iBulletNum!=0)
		for(int i=0;i<30;i++)
			if (Bullet[i].exist)
			{
				TransparentBlt(g_mdc, Bullet[i].x - 70,Bullet[i].y + 100, 100, 33, g_bufdc, 0, 0, 100, 26, RGB(0, 0, 0));
				Bullet[i].x -= 10;
				if (Bullet[i].x < 0)
				{
					g_iBulletNum--;
					Bullet[i].exist = false;

				}
			}
			HFONT hFont;
			hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("微软雅黑"));
			SelectObject(g_mdc, hFont);
			SetBkMode(g_mdc, TRANSPARENT);
			SetTextColor(g_mdc, RGB(255, 255, 0));
			swprintf_s(str, L"鼠标X坐标为%d", g_iX);
			TextOut(g_mdc, 0, 0, str, wcslen(str));
			swprintf_s(str, L"鼠标Y坐标为%d", g_iY);
			TextOut(g_mdc, 0, 20, str,wcslen(str));

			BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);

			g_tPre = GetTickCount();
			g_iBGOffset += 5;
			if (g_iBGOffset == WINDOW_WIDTH)
				g_iBGOffset = 0;

}

//-----------------------------------【Game_CleanUp( )函数】--------------------------------
//	描述：资源清理函数，在此函数中进行程序退出前资源的清理工作
//---------------------------------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hwnd)
{
	DeleteObject(g_hBackGround);
	DeleteObject(g_bufdc);
	DeleteObject(g_mdc);
	ReleaseDC(hwnd, g_hdc);

	return TRUE;
}