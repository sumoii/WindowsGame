//-----------------------------------������˵����----------------------------------------------
//  ��������:��GDIdemo16
//	 2024��9�� Create by sumoii
//  ����������ϵͳ����֮ �ǹ����� ʾ������
//------------------------------------------------------------------------------------------------

//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//	����������������������ͷ�ļ�
//------------------------------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>//ʹ��swprintf_s���������ͷ�ļ�
#include  <time.h> //ʹ�û�ȡϵͳʱ��time������Ҫ������ͷ�ļ�

//-----------------------------------�����ļ��������֡�---------------------------------------
//	���������������������Ŀ��ļ�
//------------------------------------------------------------------------------------------------
#pragma comment(lib,"winmm.lib")			//����PlaySound����������ļ�
#pragma  comment(lib,"Msimg32.lib")		//���ʹ��TransparentBlt��������Ŀ��ļ�

//-----------------------------------���궨�岿�֡�--------------------------------------------
//	����������һЩ������
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	932							//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	700							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE		L"������������Ϩ�����Ϸ�������롿����ϵͳ����֮ �ǹ�����demo"	//Ϊ���ڱ��ⶨ��ĺ�
#define FLYSTAR_NUMBER	100							//��ʾ���������ĺ꣬�Է����޸���������
#define FLYSTAR_LASTED_FRAME 60                	//��ʾ���ӳ���֡���ĺ꣬�Է����޸�ÿ���ǹ����ų�����ʱ��           


//-----------------------------------��ȫ�ֽṹ�嶨�岿�֡�-------------------------------------
//	������ȫ�ֽṹ�嶨��
//------------------------------------------------------------------------------------------------
struct FLYSTAR
{
	int x;       //�ǹ����ڵ�x����
	int y;       //�ǹ����ڵ�y����
	int vx;      //�ǹ�x������ٶ�
	int vy;      //�ǹ�y������ٶ�
	int lasted;  //�ǹ���ڵ�ʱ��
	BOOL exist;  //�ǹ��Ƿ����
};


//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ���������
//------------------------------------------------------------------------------------------------
HDC				g_hdc = NULL, g_mdc = NULL, g_bufdc = NULL;      //ȫ���豸���������ȫ���ڴ�DC���
HBITMAP		g_hStar = NULL, g_hBackGround = NULL;  		 //����λͼ������ڴ洢λͼ��Դ
DWORD		g_tPre = 0, g_tNow = 0;						//����l������������¼ʱ��,g_tPre��¼��һ�λ�ͼ��ʱ�䣬g_tNow��¼�˴�׼����ͼ��ʱ��
RECT				g_rect;				//����һ��RECT�ṹ�壬���ڴ����ڲ��������������
FLYSTAR		FlyStars[FLYSTAR_NUMBER];  //��������
int					g_StarNum = 0; //����g_StarNum���ڼ���

//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------
//	������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK	WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//���ڹ��̺���
BOOL						Game_Init(HWND hwnd);			//�ڴ˺����н�����Դ�ĳ�ʼ��
VOID							Game_Paint(HWND hwnd);		//�ڴ˺����н��л�ͼ�������д
BOOL						Game_CleanUp(HWND hwnd);	//�ڴ˺����н�����Դ������

//-----------------------------------��WinMain( )������--------------------------------------
//	������WindowsӦ�ó������ں��������ǵĳ�������￪ʼ
//------------------------------------------------------------------------------------------------
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
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"../../../../resource/Chapter8/GDIdemo16/icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
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
	MoveWindow(hwnd, 150, 20, WINDOW_WIDTH, WINDOW_HEIGHT, true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�150,20����
	ShowWindow(hwnd, nShowCmd);    //����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

	//��Ϸ��Դ�ĳ�ʼ��������ʼ��ʧ�ܣ�����һ����Ϣ�򣬲�����FALSE
	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0); //ʹ��MessageBox����������һ����Ϣ����
		return FALSE;
	}
	PlaySound(L"../../../../resource/Chapter8/GDIdemo16/�������뤫�� (Long Version).wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); //ѭ�����ű������� 

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
			if (g_tNow - g_tPre >= 30)        //���˴�ѭ���������ϴλ�ͼʱ�����0.03��ʱ�ٽ����ػ����
				Game_Paint(hwnd);
		}

	}

	//��6���������ע��
	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);  //����׼��������ע��������
	return 0;
}

//-----------------------------------��WndProc( )������--------------------------------------
//	���������ڹ��̺���WndProc,�Դ�����Ϣ���д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)						//switch��俪ʼ
	{
	case WM_KEYDOWN:					//������Ϣ
		if (wParam == VK_ESCAPE)		//���¡�Esc����
			PostQuitMessage(0);
		break;

	case WM_DESTROY:					//���Ǵ���������Ϣ
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
//------------------------------------------------------------------------------------------------
BOOL Game_Init(HWND hwnd)
{

	srand((unsigned)time(NULL));      //��ϵͳʱ���ʼ��������� 
	HBITMAP bmp;

	g_hdc = GetDC(hwnd);
	g_mdc = CreateCompatibleDC(g_hdc);  //����һ����hdc���ݵ��ڴ�DC
	g_bufdc = CreateCompatibleDC(g_hdc);//�ٴ���һ����hdc���ݵĻ���DC
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT); //��һ���ʹ��ڼ��ݵĿյ�λͼ����

	SelectObject(g_mdc, bmp);//����λͼ����ŵ�g_mdc��

	//����λͼ����
	g_hBackGround = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter8/GDIdemo16/bg.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hStar = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter8/GDIdemo16/star.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);

	GetClientRect(hwnd, &g_rect);		//ȡ���ڲ���������Ĵ�С

	Game_Paint(hwnd);
	return TRUE;
}

//-----------------------------------��Game_Paint( )������--------------------------------------
//	���������ƺ������ڴ˺����н��л��Ʋ���
//--------------------------------------------------------------------------------------------------
VOID Game_Paint(HWND hwnd)
{

	//����mdc�����ϱ���ͼ
	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);

	//��������
	if (g_StarNum == 0)              //������ñ�ը��
	{
		int	x = rand() % g_rect.right;
		int	y = rand() % g_rect.bottom;

		for (int i = 0; i < FLYSTAR_NUMBER; i++)       //�����ǹ�����
		{
			FlyStars[i].x = x;
			FlyStars[i].y = y;
			FlyStars[i].lasted = 0;  //�趨�����Ӵ��ڵ�ʱ��Ϊ��
			//�����ӱ��i�������������ĸ������˶�����x��y������ƶ��ٶ����Ϊ1��15֮���һ��ֵ����1+rand()%15����ɡ�
			if (i % 4 == 0)
			{
				FlyStars[i].vx = -(1 + rand() % 15);
				FlyStars[i].vy = -(1 + rand() % 15);
			}
			if (i % 4 == 1)
			{
				FlyStars[i].vx = 1 + rand() % 15;
				FlyStars[i].vy = 1 + rand() % 15;
			}
			if (i % 4 == 2)
			{
				FlyStars[i].vx = -(1 + rand() % 15);
				FlyStars[i].vy = 1 + rand() % 15;
			}
			if (i % 4 == 3)
			{
				FlyStars[i].vx = 1 + rand() % 15;
				FlyStars[i].vy = -(1 + rand() % 15);
			}
			FlyStars[i].exist = true;  //�趨���Ӵ���
		}
		g_StarNum = FLYSTAR_NUMBER;   //ȫ��������forѭ��������ɺ����ǽ�����������ΪFLYSTAR_NUMBER������Ŀǰ��FLYSTAR_NUMBER���ǹ�
	}


	//��ʾ���Ӳ�������һ֡����������
	for (int i = 0; i < FLYSTAR_NUMBER; i++)
	{
		if (FlyStars[i].exist)   //�ж������Ƿ񻹴���,�����ڣ�����������꣨FlyStars[i].x,FlyStars[i].y��������ͼ����
		{
			SelectObject(g_bufdc, g_hStar);
			TransparentBlt(g_mdc, FlyStars[i].x, FlyStars[i].y, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));

			//������һ����ͼ������
			FlyStars[i].x += FlyStars[i].vx;
			FlyStars[i].y += FlyStars[i].vy;

			//��ÿ����һ����ͼ�󣬽����ӵĴ���ʱ���ۼ�1.
			FlyStars[i].lasted++;
			//���������жϣ���ĳ�����ܳ���������һ���ķ�Χ���򽫸�������Ϊ�����ڣ�����������֮�ݼ�
			if (FlyStars[i].x <= -10 || FlyStars[i].x > g_rect.right || FlyStars[i].y <= -10 || FlyStars[i].y > g_rect.bottom || FlyStars[i].lasted > FLYSTAR_LASTED_FRAME)
			{
				FlyStars[i].exist = false;  //ɾ���ǹ����� 
				g_StarNum--;                    //�ݼ��ǹ�����
			}
		}
	}

	//��mdc�е�ȫ����������hdc��
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	g_tPre = GetTickCount();     //��¼�˴λ�ͼʱ��
}

//-----------------------------------��Game_CleanUp( )������--------------------------------
//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
//---------------------------------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hwnd)
{
	//�ͷ���Դ����
	DeleteObject(g_hBackGround);
	DeleteObject(g_hStar);
	DeleteDC(g_bufdc);
	DeleteDC(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}