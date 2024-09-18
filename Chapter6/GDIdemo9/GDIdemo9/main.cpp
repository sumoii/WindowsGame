//-----------------------------------������˵����----------------------------------------------
//  ��������:��GDIdemo9
//	2024��9�� Create by sumoii
//  ��������Ϸ��������֮ ��ʱ��������ʾ
//----------------------------------------------------------------------------------------------

//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//	����������������������ͷ�ļ�
//------------------------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	800							//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	600							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define SPRITE_NUMBER 300							//������������
#define WINDOW_TITLE	L"������������Ϩ�����Ϸ�������롿��Ϸ��������֮ ��ʱ��������ʾ"	//Ϊ���ڱ��ⶨ��ĺ�

//-----------------------------------��ȫ�ֽṹ���������֡�-------------------------------------
//	������ȫ�ֽṹ��Ķ���
//--------------------------------------------------------------------------------------------
struct Sprites	//����sprite�ṹ �������ϵ������ṹ��Աx��yΪ��ͼ���꣬directionΪĿǰ�����ƶ��ķ���
{
	int x, y;	//x��yΪ��ֽ����
	int direction; //direction ΪĿǰ������ƶ�����
};

//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ���������
//--------------------------------------------------------------------------------------------
HDC			g_hdc = NULL, g_mdc = NULL, g_bufdc = NULL;//ȫ���豸�������
HBITMAP	g_hSprite[4] , g_hBackGround = NULL;//��������λͼ���������3��ͼƬ���
DWORD		g_tPre = 0, g_tNow = 0;					//����l������������¼ʱ��,g_tPre��¼��һ�λ�ͼ��ʱ�䣬g_tNow��¼�˴�׼����ͼ��ʱ��
int g_iPicNum = 0, g_iX = 0, g_iY = 0;//"g_iNum"����������¼Ŀǰ��ʾ��ͼ��
Sprites Sprite[SPRITE_NUMBER]; //����SPRITE_NUMBER ��ֵ��������Sprite[]

//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------
//	������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//���ڹ��̺���
BOOL Game_Init(HWND hwnd);			//�ڴ˺����н�����Դ�ĳ�ʼ��
VOID Game_Paint(HWND hwnd);		//�ڴ˺����н��л�ͼ�������д
BOOL Game_CleanUp(HWND hwnd);	//�ڴ˺����н�����Դ������
VOID		BubSort(int n);		

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
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"../../../../resource/Chapter6/GDIdemo9/icon.ico",
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
	PlaySound(L"../../../../resource/Chapter6/GDIdem9/OrcTheme.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); //ѭ�����ű������� 

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
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;			//����һ��PAINTSTRUCT�ṹ������¼һЩ������Ϣ

	switch (message)						//switch��俪ʼ
	{
	case WM_KEYDOWN:					// ���Ǽ��̰�����Ϣ
		if (wParam == VK_ESCAPE)    // ��������µļ���ESC
			DestroyWindow(hwnd);		// ���ٴ���, ������һ��WM_DESTROY��Ϣ
		break;									//������switch���

	case WM_DESTROY:					//���Ǵ���������Ϣ
		Game_CleanUp(hwnd);			//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
		PostQuitMessage(0);			//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;									//������switch���

	default:										//������case�����������ϣ���ִ�и�default���
		return DefWindowProc(hwnd, message, wParam, lParam);		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;									//�����˳�
}


//-----------------------------------��BubSort( )������--------------------------------------
//	�������������ݷ�����
//-------------------------------------------------------------------------------------------
VOID		BubSort(int n)
{
	int i, j;
	bool f;
	Sprites tmp;

	for (i = 0; i < n - 1; i++)
	{
		f = false;
		for (j = 0; j < n - i - 1; j++)
		{
			if (Sprite[j + 1].y < Sprite[j].y)
			{
				tmp = Sprite[j + 1];
				Sprite[j + 1] = Sprite[j];
				Sprite[j] = tmp;
				f = true;
			}
		}
		if (!f)
			break;
	}
}


//-----------------------------------��Game_Init( )������--------------------------------------
//	��������ʼ������������һЩ�򵥵ĳ�ʼ��
//------------------------------------------------------------------------------------------------
BOOL Game_Init(HWND hwnd)
{
	srand((unsigned)time(NULL));  //��ϵͳʱ���ʼ���������
	HBITMAP bmp;

	g_hdc = GetDC(hwnd);  //��ȡ�豸�������
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufdc = CreateCompatibleDC(g_hdc);
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);//��һ���ʹ��ڼ��ݵĿյ�λͼ����

	SelectObject(g_mdc, bmp);//����λͼ����ŵ�mdc��
	g_hBackGround = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter6/GDIdemo9/bg.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hSprite[0] = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter6/GDIdemo9/11.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);
	g_hSprite[1] = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter6/GDIdemo9/22.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);
	g_hSprite[2] = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter6/GDIdemo9/33.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);
	g_hSprite[3] = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter6/GDIdemo9/44.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);

	//�趨��ʼ����ͼ���궼Ϊ�����ڵ��������꣬��ʼ���ƶ�����Ϊ����
	for (int i = 0; i < SPRITE_NUMBER; i++)
	{
		Sprite[i].direction = 3;//��ʼ����
		Sprite[i].x = rand() % WINDOW_WIDTH; //��ͼ����ʼX����
		Sprite[i].y = rand() % WINDOW_HEIGHT; //��ͼ����ʼY����
	}
	
	Game_Paint(hwnd);
	return TRUE;
}

//-----------------------------------��Game_Paint( )������--------------------------------------
//	���������ƺ������ڴ˺����н��л��Ʋ���
//--------------------------------------------------------------------------------------------------
VOID Game_Paint(HWND hwnd)
{
	//����ͼ��
	if (g_iPicNum == 4)               //�ж��Ƿ񳬹����ͼ�ţ����������ͼ�š�12��������ʾͼ������Ϊ"0"��
		g_iPicNum = 0;

	//��mdc �����ϱ���ͼ
	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);         //��Ŀǰͼ�Ž��д�����ͼ

	BubSort(SPRITE_NUMBER);    //��������ͼ֮ǰ����BubSort����������������

	//��mdc�н���͸������
	for (int i =  0; i<SPRITE_NUMBER;i++)
	{	
		SelectObject(g_bufdc, g_hSprite[Sprite[i].direction]);
		TransparentBlt(g_mdc, Sprite[i].x, Sprite[i].y, 96, 96, g_bufdc, g_iPicNum * 96, 0, 96, 96, RGB(0, 0, 0));//����TransparentBlt͸��ɫ�ʷ�
	}

	//�����Ļ�����ʾ�ڴ�����
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	g_tPre = GetTickCount();     //��¼�˴λ�ͼʱ�䣬���´���Ϸѭ�����ж��Ƿ��Ѿ��ﵽ������²����趨��ʱ������
	g_iPicNum++;                    //����g_iNum��ֵ��1��Ϊ��һ��Ҫ��ʾ��ͼ��

	g_iX += 10;					   //�����´���ͼ������
	//�����ͼ���곬�����ڱ�Ե������������Ϊ-60
	//�������forѭ��������ÿһֻ������һ�ε��ƶ�������ͼ����
	for (int i = 0; i < SPRITE_NUMBER; i++)
	{
		switch (rand() % 4)          //���������4�������������´��ƶ���������0,1,2,3�ֱ�����ϣ��£�����
		{
		case 0:					     //��
			Sprite[i].y -= 20;
			//�ڼ�����µ���ͼ����֮�󣬻����жϴ��µ�����᲻��ʹ��������ͼ�������ڱ߽磬���������򽫸÷����ϵ������趨Ϊ�պõ����ٽ�ֵ
			if (Sprite[i].y < 0)
				Sprite[i].y = 0;
			Sprite[i].direction = 0;
			break;
			//���������պ�������ͬ�ķ�������
		case 1:		//��
			Sprite[i].y += 20;
			if (Sprite[i].y > WINDOW_HEIGHT - 100)
				Sprite[i].y = WINDOW_HEIGHT - 100;
			Sprite[i].direction = 1;
			break;
		case 2:				    	//��
			Sprite[i].x -= 20;
			if (Sprite[i].x < 0)
				Sprite[i].x = 0;
			Sprite[i].direction = 2;
			break;
		case 3:				    	//��
			Sprite[i].x += 20;

			if (Sprite[i].x > WINDOW_WIDTH - 100)
				Sprite[i].x = WINDOW_WIDTH - 100;
			Sprite[i].direction = 3;
			break;
		}
	}
}

//-----------------------------------��Game_CleanUp( )������--------------------------------
//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
//---------------------------------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hwnd)

{ 
	//�ͷ���Դ����
	DeleteObject(g_hBackGround);
	for (int i = 0; i < 4; i++) {
		DeleteObject(g_hSprite[i]);
	}
	DeleteDC(g_bufdc);
	DeleteDC(g_mdc);
	ReleaseDC(hwnd, g_hdc);  //�ͷ��豸����
	return TRUE;
}