//-----------------------------------������˵����----------------------------------------------
//  ��������:��GDIdemo11
//	2024��9�� Create by sumoii
//  ������Windows��Ϣ����֮ �����Ϣ����  ʾ������
//---------------------------------------------------------------------------------------------

//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//	����������������������ͷ�ļ�
//--------------------------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>//ʹ��swprintf_s���������ͷ�ļ�
#include  <time.h> //ʹ�û�ȡϵͳʱ��time()������Ҫ������ͷ�ļ�

//-----------------------------------�����ļ��������֡�---------------------------------------
//	���������������������Ŀ��ļ�
//--------------------------------------------------------------------------------------------
#pragma comment(lib,"winmm.lib")  //����PlaySound����������ļ�
#pragma comment(lib,"Msimg32.lib")//���ʹ��TransparentBlt��������Ŀ��ļ�

//-----------------------------------���궨�岿�֡�--------------------------------------------
//	����������һЩ������
//---------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	800							//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	600							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define SPRITE_NUMBER 300							//������������
#define WINDOW_TITLE	L"������������Ϩ�����Ϸ�������롿��Ϸ��������֮ ��ʱ��������ʾ"	//Ϊ���ڱ��ⶨ��ĺ�

//-----------------------------------��ȫ�ֽṹ���������֡�-----------------------------------
//	������ȫ�ֽṹ��Ķ���
//--------------------------------------------------------------------------------------------
struct SwordBullets	//SwordBullets�ṹ����������ӵ���
{
	int x, y;        //�������ӵ�������
	bool exist;     //�������ӵ����Ƿ����
};

//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ���������
//--------------------------------------------------------------------------------------------
HDC			g_hdc = NULL, g_mdc = NULL, g_bufdc = NULL;//ȫ���豸�������
HBITMAP	g_hSwordMan = NULL , g_hSwordBlade = NULL, g_hBackGround = NULL;//��������λͼ���������3��ͼƬ���
DWORD		g_tPre = 0, g_tNow = 0;					//����l������������¼ʱ��,g_tPre��¼��һ�λ�ͼ��ʱ�䣬g_tNow��¼�˴�׼����ͼ��ʱ��
int g_iX = 0, g_iY = 0, g_iXnow = 0, g_iYnow = 0;   //g_iX,g_iY�������������λ�ã�g_iXnow��g_iYnow����ǰ�������꣬Ҳ������ͼ��λ��
int g_iBGOffset = 0, g_iBulletNum = 0;       //g_iBGOffsetΪ����������Ҫ�ü��������ȣ�g_iBulletNum��¼�������н������ӵ�����Ŀ
SwordBullets Bullet[30];           //����һ����SwordBullets�����͵����飬�����洢���������Ľ������ӵ���


//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------
//	������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
//--------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//���ڹ��̺���
BOOL Game_Init(HWND hwnd);			//�ڴ˺����н�����Դ�ĳ�ʼ��
VOID Game_Paint(HWND hwnd);		//�ڴ˺����н��л�ͼ�������д
BOOL Game_CleanUp(HWND hwnd);	//�ڴ˺����н�����Դ������

//-----------------------------------��WinMain( )������--------------------------------------
//	������WindowsӦ�ó������ں��������ǵĳ�������￪ʼ
//-------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����
	WNDCLASSEX wndClass = { 0 };							//��WINDCLASSEX������һ��������
	wndClass.cbSize = sizeof(WNDCLASSEX);			//���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;					//����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra = 0;								//������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra = 0;							//���ڵĸ����ڴ棬��Ȼȡ0������
	wndClass.hInstance = hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"../../../../resource/Chapter7/GDIdemo10/icon.ico",
		IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //ָ��������Ĺ������
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
	wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		//��һ���Կ���ֹ���ַ�����ָ������������֡�

	//��2�����ڴ����Ĳ���֮����ע�ᴰ����
	if (!RegisterClassEx(&wndClass))				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;

	//��3�����ڴ����Ĳ���֮������ʽ��������
	HWND hwnd = CreateWindow(L"ForTheDreamOfGameDevelop", WINDOW_TITLE,				//ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	MoveWindow(hwnd, 200, 20, WINDOW_WIDTH, WINDOW_HEIGHT, true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�200,20����
	ShowWindow(hwnd, nShowCmd);    //����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

	//��Ϸ��Դ�ĳ�ʼ��������ʼ��ʧ�ܣ�����һ����Ϣ�򣬲�����FALSE
	if (!Game_Init(hwnd))
	{
		//ʹ��MessageBox����������һ����Ϣ����
		MessageBox(hwnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0);
		return FALSE;
	}
	PlaySound(L"../../../../resource/Chapter7/GDIdem11/�ɽ�����ԭ��ս��3.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); //ѭ�����ű������� 

	//��5����Ϣѭ������
	MSG msg = { 0 };				//���岢��ʼ��msg
	while (msg.message != WM_QUIT)		//ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage(&msg);		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);			//�ַ�һ����Ϣ�����ڳ���
		}
		else
		{
			g_tNow = GetTickCount();   //��ȡ��ǰϵͳʱ��
			if (g_tNow - g_tPre >= 100)        //���˴�ѭ���������ϴλ�ͼʱ�����0.1��ʱ�ٽ����ػ����
				Game_Paint(hwnd);
		}
	}

	//��6���������ע��
	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);  //����׼��������ע��������
	return 0;
}

//-----------------------------------��WndProc( )������--------------------------------------
//	���������ڹ��̺���WndProc,�Դ�����Ϣ���д���
//-------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;			//����һ��PAINTSTRUCT�ṹ������¼һЩ������Ϣ

	switch (message)						//switch��俪ʼ
	{
	case WM_KEYDOWN:					// ���Ǽ��̰�����Ϣ
		switch (wParam)
		{
		case VK_ESCAPE://����Esc��
			DestroyWindow(hwnd); //���ٴ��ڣ�������һ��WM_DESTROY��Ϣ
			PostQuitMessage(0); //��������
			break;
		}
		break;							//������switch���

	case WM_LBUTTONDOWN: //������������Ϣ
		for (int i = 0; i < 30; i++)
		{
			if (!Bullet[i].exist)
			{
				Bullet[i].x = g_iXnow;//�������ӵ���x����
				Bullet[i].y = g_iYnow + 30; //�������ӵ���y����
				Bullet[i].exist = true;
				g_iBulletNum++; //�ۼӽ������ӵ�����Ŀ
				break;
			}
		}

	case WM_MOUSEMOVE: //����ƶ���Ϣ
		//��X����Ĵ���
		g_iX = LOWORD(lParam); //ȡ�����X����
		if (g_iX > WINDOW_WIDTH - 317)
			g_iX = WINDOW_WIDTH - 317;
		else if (g_iX < 0)
			g_iX = 0;
		//��Y����Ĵ���
		g_iY = HIWORD(lParam);
		if (g_iY > WINDOW_HEIGHT - 283)
			g_iY = WINDOW_HEIGHT - 283;
		else if (g_iY < -200)
			g_iY = -200;
		break;

	case WM_DESTROY:		
		//���Ǵ���������Ϣ
		Game_CleanUp(hwnd);			//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
		PostQuitMessage(0);			//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;									//������switch���

	default:										//������case�����������ϣ���ִ�и�default���
		return DefWindowProc(hwnd, message, wParam, lParam);		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;									//�����˳�
}


//-----------------------------------��Game_Init( )������--------------------------------------
//	��������ʼ������������һЩ�򵥵ĳ�ʼ��
//---------------------------------------------------------------------------------------------
BOOL Game_Init(HWND hwnd)
{
	srand((unsigned)time(NULL));  //��ϵͳʱ���ʼ���������
	HBITMAP bmp;

	g_hdc = GetDC(hwnd);  //��ȡ�豸�������
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufdc = CreateCompatibleDC(g_hdc);
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);//��һ���ʹ��ڼ��ݵĿյ�λͼ����

	//�趨������ͼ��ʼλ�ã����λ�ó�ʼֵ
	g_iX = 300;
	g_iY = 100;
	g_iXnow = 300;
	g_iYnow = 100;

	SelectObject(g_mdc, bmp);//����λͼ����ŵ�mdc��
	g_hBackGround = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter7/GDIdemo11/bg.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hSwordMan = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter7/GDIdemo11/swordman.bmp", IMAGE_BITMAP, 317, 283, LR_LOADFROMFILE);
	g_hSwordBlade = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter7/GDIdemo11/swordblade.bmp", IMAGE_BITMAP, 100, 26, LR_LOADFROMFILE);

	POINT pt, lt, rb;
	RECT rect;
	//�趨���λ��
	pt.x = 300;
	pt.y = 100;
	ClientToScreen(hwnd, &pt);
	SetCursorPos(pt.x, pt.y);

	ShowCursor(FALSE);

	//����������ƶ�����
	GetClientRect(hwnd, &rect);//ȡ�ô����ڲ�����
	lt.x = rect.left;
	lt.y = rect.top;
	rb.x = rect.right;
	rb.y = rect.bottom;
	//��lt �� rb�Ĵ�������ת��Ϊ��������
	ClientToScreen(hwnd, &lt);
	ClientToScreen(hwnd, &rb);

	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	//����������ƶ�����
	ClipCursor(&rect);

	Game_Paint(hwnd);
	return TRUE;
}

//-----------------------------------��Game_Paint( )������--------------------------------------
//	���������ƺ������ڴ˺����н��л��Ʋ���
//----------------------------------------------------------------------------------------------
VOID Game_Paint(HWND hwnd)
{
	//��mdc �����ϱ���ͼ
	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, g_iBGOffset, WINDOW_HEIGHT, g_bufdc, WINDOW_WIDTH-g_iBGOffset, 0, SRCCOPY);
	BitBlt(g_mdc, g_iBGOffset, 0, WINDOW_WIDTH - g_iBGOffset, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);

	wchar_t str[20] = {};
	
	//���㽣������ͼ����
	if (g_iXnow < g_iX)//����ǰ��ͼX����С��������X����
	{
		g_iXnow += 10;
		if (g_iXnow > g_iX)
			g_iXnow = g_iX;
	}
	else   //����ǰ��ͼX�������������X����
	{
		g_iXnow -= 10;
		if (g_iXnow < g_iX)
			g_iXnow = g_iX;
	}

	if (g_iYnow < g_iY)  //����ǰ��ͼY����С��������Y����
	{
		g_iYnow += 10;
		if (g_iYnow > g_iY)
			g_iYnow = g_iY;
	}
	else  //����ǰ��ͼY���������������Y����
	{
		g_iYnow -= 10;
		if (g_iYnow < g_iY)
			g_iYnow = g_iY;
	}
	//���Ͻ���ͼ
	SelectObject(g_bufdc, g_hSwordMan);
	TransparentBlt(g_mdc, g_iXnow,g_iYnow,317,283,g_bufdc,0,0,317,283,RGB(0,0,0));


	SelectObject(g_bufdc, g_hSwordBlade);
	if(g_iBulletNum!=0)
		for (int i = 0; i < 30; i++)
			if (Bullet[i].exist)
			{
				//���Ͻ������ӵ���ͼ
				TransparentBlt(g_mdc, Bullet[i].x - 70, Bullet[i].y + 100, 100, 33, g_bufdc, 0, 0, 100, 26, RGB(0, 0, 0));

				//������һ���������ӵ��������ꡣ�������ӵ����Ǵ���������ģ���ˣ�ÿ����X���ϵ�����ֵ�ݼ�10����λ��������ͼ����������ƶ���Ч����
				//������������ӵ����´ε������ѳ������ڵĿɼ���Χ��h[i].x<0��,
				//��ô�������ӵ�����Ϊ�����ڣ������������ӵ�������g_iBulletNum����ֵ��1.
				Bullet[i].x -= 10;
				if (Bullet[i].x < 0)
				{
					g_iBulletNum--;
					Bullet[i].exist = false;
				}
			}

	HFONT hFont;
	hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("΢���ź�"));  //��������
	SelectObject(g_mdc, hFont);  //ѡ�����嵽g_mdc��
	SetBkMode(g_mdc, TRANSPARENT);    //�������ֱ���͸��
	SetTextColor(g_mdc, RGB(255, 255, 0));  //����������ɫ

	//�����Ͻǽ����������
	swprintf_s(str, L"���X����Ϊ%d    ", g_iX);
	TextOut(g_mdc, 0, 0, str, wcslen(str));
	swprintf_s(str, L"���Y����Ϊ%d    ", g_iY);
	TextOut(g_mdc, 0, 20, str, wcslen(str));

	//�����Ļ�����ʾ�ڴ�����
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	g_tPre = GetTickCount();     //��¼�˴λ�ͼʱ�䣬���´���Ϸѭ�����ж��Ƿ��Ѿ��ﵽ������²����趨��ʱ������

	g_iBGOffset += 5; //�ñ���������+5
	if (g_iBGOffset == WINDOW_WIDTH)//��������������ﵽ�˱������ֵ��������
		g_iBGOffset = 0;
}

//-----------------------------------��Game_CleanUp( )������--------------------------------
//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
//------------------------------------------------------------------------------------------d
BOOL Game_CleanUp(HWND hwnd)

{
	//�ͷ���Դ����
	DeleteObject(g_hBackGround);
	DeleteObject(g_hSwordBlade);
	DeleteObject(g_hSwordMan);
	DeleteDC(g_bufdc);
	DeleteDC(g_mdc);
	ReleaseDC(hwnd, g_hdc);  //�ͷ��豸����
	return TRUE;
}