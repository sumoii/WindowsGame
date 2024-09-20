//-----------------------------------������˵����----------------------------------------------
//  ��������:��GDIdemo17
//	 2024��9�� Create by sumoii
//  ������С�ͻغ�����Ϸ�������߶�������
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
#define WINDOW_WIDTH	800							//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	600							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE		L"������������Ϩ�����Ϸ�������롿����ϵͳ����֮ �ǹ�����demo"	//Ϊ���ڱ��ⶨ��ĺ�
#define PARTICLE_NUMBER	50							//��ʾ���������ĺ꣬�Է����޸���������


//-----------------------------------��ȫ�ֽṹ�嶨�岿�֡�-------------------------------------
//	������ȫ�ֽṹ�嶨��
//------------------------------------------------------------------------------------------------
struct CHARACTER
{
	int NowHp;       //��ǰ����ֵ
	int MaxHp;       //�������ֵ
	int NowMp;      //��ǰħ��ֵ
	int MaxMp;      //���ħ��ֵ
	int Level;  //��ɫ�ȼ�
	int Strength; //��ɫ����ֵ
	int Intelligence; //��ɫ����ֵ
	int Agility; //��ɫ����ֵ
};

struct SNOW
{
	int x;	//ѩ����X����
	int y;	//ѩ����Y����
	BOOL exist; //ѩ���Ƿ����
};

enum ActionTypes
{
	ACTION_TYPE_NORMAL = 0,
	ACTION_TYPE_CRITICAL = 1,
	ACTION_TYPE_MAGIC = 2,
	ACTION_TYPE_MISS = 3,
	ACTION_TYPE_RECOVER = 4,
};

//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ���������
//------------------------------------------------------------------------------------------------
HDC				g_hdc = NULL, g_mdc = NULL, g_bufdc = NULL;      //ȫ���豸���������ȫ���ڴ�DC���
DWORD		g_tPre = 0, g_tNow = 0;						//����������������¼ʱ��,g_tPre��¼��һ�λ�ͼ��ʱ�䣬g_tNow��¼�˴�׼����ͼ��ʱ��
RECT				g_rect;				//����һ��RECT�ṹ�壬���ڴ����ڲ��������������
int g_iFrameNum, g_iTxtNum;//֡������������
wchar_t text[8][100]; //�洢��������ֵ��ַ�����ֵ
BOOL g_bCanAttack, g_bGameOver;//������Ϸ״̬��ʶ����ɫ�Ƿ���Թ�������Ϸ�Ƿ����
SNOW SnowFlowers[PARTICLE_NUMBER];//ѩ��������ֵ
CHARACTER Hero, Boss;					//������ɫ�ṹ��ʵ�У��ֱ��ʾӢ�ۺ�BOSS
ActionTypes HeroActionType, BossActionType; //������������ö��ʵ��
int					g_SnowNum = 0; //���ڼ���ѩ��
//һϵ��λͼ����Ķ���
HBITMAP		g_hBackGround, g_hGameOver, g_hVictory, g_hSnow;  //��������Ϸ��������Ϸʤ����λͼ���
HBITMAP		g_hMonsterBitmap, g_hHeroBitmap, g_hRecoverSkill;  //BOSS��ɫͼ��Ӣ�۽�ɫͼ���ָ�����ͼ��λͼ���
HBITMAP		g_hSkillButton1, g_hSkillButton2, g_hSkillButton3, g_hSkillButton4;  //4�����ܰ�ť��λͼ���
HBITMAP		g_hHeroSkill1, g_hHeroSkill2, g_hHeroSkill3;  //3��Ӣ�ۼ���Ч����λͼ���
HBITMAP		g_hBossSkill1, g_hBossSkill2, g_hBossSkill3;  //3��BOSS����Ч����λͼ���

//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------
//	������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK	WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//���ڹ��̺���
BOOL						Game_Init(HWND hwnd);			//�ڴ˺����н�����Դ�ĳ�ʼ��
VOID						Game_Main(HWND hwnd);		//�ڴ˺����н��л�ͼ�������д
BOOL						Game_ShutDown(HWND hwnd);	//�ڴ˺����н�����Դ������
VOID						Die_Check(int NowHp, bool isHero); //������麯��
VOID						Message_Insert(wchar_t* str); //������Ϣ������
VOID						HeroAction_Logic(); //Ӣ�۶����߼��жϺ���
VOID						HeroAction_Paint();//Ӣ�۶�����ͼ����
VOID						BossAction_Logic();//���ﶯ���߼��жϺ���
VOID						BossAction_Paint();//���ﶯ����ͼ����
VOID						Snow_Paint();

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
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
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
	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�150,20����
	ShowWindow(hwnd, nShowCmd);    //����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

	//��Ϸ��Դ�ĳ�ʼ��������ʼ��ʧ�ܣ�����һ����Ϣ�򣬲�����FALSE
	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0); //ʹ��MessageBox����������һ����Ϣ����
		return FALSE;
	}
	PlaySound(L"../../../../resource/Chapter9/GDIdemo17/�λ�����ԭ��-ս��1-ɭ��.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); //ѭ�����ű������� 

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
			if (g_tNow - g_tPre >= 60)        //���˴�ѭ���������ϴλ�ͼʱ�����0.06��ʱ�ٽ����ػ����
				Game_Main(hwnd);
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

	switch (message)						//switch��俪ʼ
	{
	case WM_KEYDOWN:					//������Ϣ
		if (wParam == VK_ESCAPE)		//���¡�Esc����
			PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
		if (!g_bCanAttack)
		{
			int x = LOWORD(lParam);	//X����
			int y = HIWORD(lParam);	//Y����
			if (x >= 530 && x <= 570 && y >= 420 && y <= 470)
			{
				g_bCanAttack = true;
				HeroActionType = ACTION_TYPE_NORMAL;
			}
			if (x >= 590 && x < 640 && y >= 420 && y <= 470)
			{
				g_bCanAttack = true;
				HeroActionType = ACTION_TYPE_MAGIC;
			}
			if (x >= 650 && x <= 700 && y >= 420 && y <= 470)
			{
				g_bCanAttack = true;
				HeroActionType = ACTION_TYPE_RECOVER;
			}
		}
		break;

	case WM_DESTROY:					//���Ǵ���������Ϣ
		Game_ShutDown(hwnd);			//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
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

	//��������ϵ�Ĵ���
	g_hdc = GetDC(hwnd);
	g_mdc = CreateCompatibleDC(g_hdc);  //����һ����hdc���ݵ��ڴ�DC
	g_bufdc = CreateCompatibleDC(g_hdc);//�ٴ���һ����hdc���ݵĻ���DC
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT); //��һ���ʹ��ڼ��ݵĿյ�λͼ����

	SelectObject(g_mdc, bmp);//����λͼ����ŵ�g_mdc��

	//����λͼ����
	g_hBackGround = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/bg.bmp", IMAGE_BITMAP, 800, 600, LR_LOADFROMFILE);	//����λͼ
	g_hGameOver = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/gameover.bmp", IMAGE_BITMAP, 1086, 396, LR_LOADFROMFILE); //��Ϸ����λͼ
	g_hVictory = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/victory.bmp", IMAGE_BITMAP, 800, 600, LR_LOADFROMFILE); //��Ϸʤ��λͼ
	g_hMonsterBitmap = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/monster.bmp", IMAGE_BITMAP, 360, 360, LR_LOADFROMFILE);  //�����ɫλͼ
	g_hHeroBitmap = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/hero.bmp", IMAGE_BITMAP, 360, 360, LR_LOADFROMFILE); //Ӣ�۽�ɫλͼ
	g_hHeroSkill1 = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/heroslash.bmp", IMAGE_BITMAP, 364, 140, LR_LOADFROMFILE);  //Ӣ��1����λͼ
	g_hHeroSkill2 = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/heromagic.bmp", IMAGE_BITMAP, 374, 288, LR_LOADFROMFILE);  //Ӣ��2����λͼ
	g_hHeroSkill3 = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/herocritical.bmp", IMAGE_BITMAP, 574, 306, LR_LOADFROMFILE);  //Ӣ��3����λͼ
	g_hSkillButton1 = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/skillbutton1.bmp", IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);  //����1ͼ��λͼ
	g_hSkillButton2 = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/skillbutton2.bmp", IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);   //����2ͼ��λͼ
	g_hSkillButton3 = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/skillbutton3.bmp", IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);   //����3ͼ��λͼ
	g_hSkillButton4 = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/skillbutton4.bmp", IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);   //����4ͼ��λͼ
	g_hBossSkill1 = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/monsterslash.bmp", IMAGE_BITMAP, 234, 188, LR_LOADFROMFILE);   //����1����λͼ
	g_hBossSkill2 = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/monstermagic.bmp", IMAGE_BITMAP, 387, 254, LR_LOADFROMFILE);  //����2����λͼ
	g_hBossSkill3 = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/monstercritical.bmp", IMAGE_BITMAP, 574, 306, LR_LOADFROMFILE); //����3����λͼ
	g_hSnow = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/snow.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);   //ѩ��λͼ
	g_hRecoverSkill = (HBITMAP)LoadImage(NULL, L"../../../../resource/Chapter9/GDIdemo17/recover.bmp", IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);    //�ָ�����λͼ

	GetClientRect(hwnd, &g_rect);		//ȡ���ڲ���������Ĵ�С

	//�趨���ʥ����ʿ�ĸ�������ֵ
	Hero.NowHp = Hero.MaxHp = 1000;	//�趨�������ֵ�ĵ�ǰֵ������
	Hero.Level = 6;					//�趨��ҽ�ɫ�ȼ�
	Hero.NowMp = Hero.MaxMp = 60;    //���ħ��ֵ�ĵ�ǰֵ������
	Hero.Strength = 10;			//�趨Ӣ�۵�����ֵ
	Hero.Agility = 20;				//�趨Ӣ�۵�����ֵ   
	Hero.Intelligence = 10;		//�趨Ӣ�۵�����ֵ

	//�趨BOSS�ƽ�ħ�����ĸ�������ֵ
	Boss.NowHp = Boss.MaxHp = 2000;	//�趨BOSS����ֵ�ĵ�ǰֵ������
	Boss.Level = 10;						//�趨BOSS�ĵȼ�
	Boss.Strength = 10;				//�趨BOSS������ֵ
	Boss.Agility = 10;                   //�趨BOSS������ֵ
	Boss.Intelligence = 10;			//�趨BOSS������ֵ

	g_iTxtNum = 0;		//��ʼ����ʾ��Ϣ��Ŀ

	//��������
	HFONT hFont;
	hFont = CreateFont(20, 0, 0, 0, 700, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("΢���ź�"));
	SelectObject(g_mdc, hFont);
	SetBkMode(g_mdc, TRANSPARENT);    //����������ʾ����͸��

	Game_Main(hwnd);
	return TRUE;
}

//-----------------------------------��Game_Main( )������--------------------------------------
//	��������ϷMain��������Ϸ�����࣬������Ϸ���СС�����ĺ������������
//---------------------------------------------------------------------------------------------
VOID Game_Main(HWND hwnd)
{
	wchar_t str[100];

	//����mdc�����ϱ���ͼ
	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);

	//��ѩЧ���Ļ���
	if (!g_bGameOver)//�����Ϸδ����
	{
		Snow_Paint(); //���÷�װ��ѩ����������Ч���ĺ�����ģ����ѩ����
	}

	//��ʾ��ս��Ϣ
	SetTextColor(g_mdc, RGB(255, 255, 255));
	for (int i = 0; i < g_iTxtNum; i++)
		TextOut(g_mdc, 20, 410 + i * 18, text[i], wcslen(text[i]));

	//���Ϲ���ͼ�Լ���ʾ����Ѫ��
	if (Boss.NowHp > 0)
	{
		//���Ϲ���ͼ
		SelectObject(g_bufdc, g_hMonsterBitmap);
		TransparentBlt(g_mdc, 0, 50, 360, 360, g_bufdc, 0, 0, 360, 360, RGB(0, 0, 0));//����TransparentBlt͸����ͼ����
		//��ʾ����Ѫ��
		swprintf_s(str, L"%d / %d", Boss.NowHp, Boss.MaxHp);
		SetTextColor(g_mdc, RGB(255, 10, 10));
		TextOut(g_mdc, 100, 370, str, wcslen(str));
	}

	//�������ͼ�Լ��������ֵ��ħ��ֵ����ʾ
	if (Hero.NowHp > 0)
	{
		//��������ͼ
		SelectObject(g_bufdc, g_hHeroBitmap);
		TransparentBlt(g_mdc, 400, 50, 360, 360, g_bufdc, 0, 0, 360, 360, RGB(0, 0, 0));//͸��ɫΪRGB(0,0,0)����ɫ
		//��ʾѪ��
		swprintf_s(str, L"%d / %d", Hero.NowHp, Hero.MaxHp);
		SetTextColor(g_mdc, RGB(255, 10, 10));
		TextOut(g_mdc, 600, 350, str, wcslen(str));
		//��ʾħ��ֵ
		swprintf_s(str, L"%d / %d", Hero.NowMp, Hero.MaxMp);
		SetTextColor(g_mdc, RGB(10, 10, 255));
		TextOut(g_mdc, 600, 370, str, wcslen(str));
	}

	//�ж���Ϸ�Ƿ����
	if (g_bGameOver)
	{
		if (Hero.NowHp <= 0)  //�����������������GameOver��ͼƬ
		{
			SelectObject(g_bufdc, g_hGameOver);
			BitBlt(g_mdc, 120, 50, 543, 396, g_bufdc, 543, 0, SRCAND);
			BitBlt(g_mdc, 120, 50, 543, 396, g_bufdc, 0, 0, SRCPAINT);
		}
		else  //������������������ʤ����ͼƬ
		{
			SelectObject(g_bufdc, g_hVictory);
			TransparentBlt(g_mdc, 0, 0, 800, 600, g_bufdc, 0, 0, 800, 600, RGB(0, 0, 0));//͸��ɫΪRGB(0,0,0)
		}
	}
	//�����Ϸû��������Ϊ��ҿɹ���״̬�����������ϼ���ͼ��
	else if (!g_bCanAttack) //���ϼ��ܰ�ť
	{
		SelectObject(g_bufdc, g_hSkillButton1);
		BitBlt(g_mdc, 530, 420, 50, 50, g_bufdc, 0, 0, SRCCOPY);
		SelectObject(g_bufdc, g_hSkillButton2);
		BitBlt(g_mdc, 590, 420, 50, 50, g_bufdc, 0, 0, SRCCOPY);
		SelectObject(g_bufdc, g_hSkillButton3);
		BitBlt(g_mdc, 650, 420, 50, 50, g_bufdc, 0, 0, SRCCOPY);
		SelectObject(g_bufdc, g_hSkillButton4);
		BitBlt(g_mdc, 710, 420, 50, 50, g_bufdc, 0, 0, SRCCOPY);
	}
	//��Ϸû��������Ϊ��Ҳ��ɹ���״̬���㿪ʼ�������Ļ�ͼ
	else
	{
		g_iFrameNum++;

		//��5~10������ʱ��ʾ��ҹ���Ч��ͼ
		if (g_iFrameNum >= 5 && g_iFrameNum <= 10)
		{
			//��5������ʱ����֮ǰ������������Ϸ�߼���������Ϣ��ʾ
			if (g_iFrameNum == 5)
			{
				HeroAction_Logic();
				Die_Check(Boss.NowHp, false);
			}
			//��������Ķ������ͽ�����Ӧ��ͼ
			HeroAction_Paint();
		}

		//��15������ʱ�жϹ�����������
		if (g_iFrameNum == 15)
		{
			BossAction_Logic();
		}

		//��26~30������ʱ��ʾ���﹥��ͼ��
		if (g_iFrameNum >= 26 && g_iFrameNum <= 30)
		{
			BossAction_Paint();
		}

		if (g_iFrameNum == 30) //�غϽ���
		{
			g_bCanAttack = false;
			g_iFrameNum = 0;
			if (!g_bGameOver)
			{
				int MpRecover = 2 * (rand() % Hero.Intelligence) + 6;
				Hero.NowMp += MpRecover;
				if (Hero.NowMp >= Hero.MaxMp)
					Hero.NowMp = Hero.MaxMp;
				swprintf_s(str, L"�غϽ������Զ��ָ��ˡ�%d����ħ��ֵ",MpRecover);
				Message_Insert(str);
			}
		}
	}
	//��mdc�е�ȫ����������hdc��
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);
	g_tPre = GetTickCount();     //��¼�˴λ�ͼʱ��
}

//-----------------------------------��Message_Insert( )������----------------------------------
//	��������ս��Ϣ���������ڲ���������Ϣ
//----------------------------------------------------------------------------------------------
VOID Message_Insert(wchar_t* str)
{
	//���δ��8����Ϣ��ֱ���¿�һ����Ϣ
	if (g_iTxtNum < 8)
	{
		swprintf_s(text[g_iTxtNum], str);
		g_iTxtNum++;
	}
	else 
	{
		for (int i = 0; i < g_iTxtNum; i++)
			swprintf_s(text[i], text[i + 1]);

		swprintf_s(text[7], str);
	}
}

//-----------------------------------��Die_Check( )������----------------------------------------
//	����������ֵ�жϺ���������ɫ�Ƿ���������һ��������ĳ��ɫ��ǰ����ֵ
//-----------------------------------------------------------------------------------------------
VOID Die_Check(int NowHp, bool isHero)
{
	wchar_t str[100];

	if (NowHp < 0)//����ֵС�ڵ���0��
	{
		g_bGameOver = true;
		if (isHero) //�����Ӣ�۵�����ֵС��0,��ʾ��Ϸʧ��
		{
			PlaySound(L"../../../../resource/Chapter9/GDIdemo17/failure.wav", NULL, SND_FILENAME | SND_ASYNC);//����ʧ������
			swprintf_s(str, L":(ʤ���˱��ҳ��£���������������......");//���ʧ����ʾ��
			Message_Insert(str);//���뵽������Ϣ��
		}
		else
		{
			PlaySound(L"../../../../resource/Chapter9/GDIdemo17/victory.wav", NULL, SND_FILENAME | SND_ASYNC);//����ʤ������
			swprintf_s(str, L"���꣬��Ӯ�ˡ��������Ӱ�~~~~~��������");//���ʤ����ʾ��
			Message_Insert(str);//���뵽������Ϣ��
		}
	}
}

//-----------------------------------��HeroAction_Logic( )������------------------------------
//	��������Ҷ����߼��жϺ���
//--------------------------------------------------------------------------------------------
VOID HeroAction_Logic()
{
	int damage = 0;
	wchar_t str[100];

	switch (HeroActionType)
	{
	case ACTION_TYPE_NORMAL:
		if (1 == rand() % 4)	//20%���ʴ�����Ӱ�̿͵Ĵ��У����ͽ��ѣ�4�������˺�
		{
			HeroActionType = ACTION_TYPE_CRITICAL;
			damage = (int)(4.5f * (float)(3 * (rand() % Hero.Agility) + Hero.Level * Hero.Strength + 20));
			Boss.NowHp -= (int)damage;

			swprintf_s(str, L"���ͽ��Ѵ���������ţ���ˣ�4.5������...�Թ����ճ��ˡ�%d�����˺�", damage);
		}
		else
		{
			damage = 3 * (rand() % Hero.Agility) + Hero.Level * Hero.Strength + 20;
			Boss.NowHp -= (int)damage;

			swprintf_s(str, L"���ʥ����ʿʹ������ͨ�������޵�ն�����˺�һ���...�Թ����ճ��ˡ�%d�����˺�", damage);
		}
		Message_Insert(str);
		break;

	case ACTION_TYPE_MAGIC:
		if (Hero.MaxMp > 30)
		{
			damage = 5 * (2 * (rand() % Hero.Agility) + Hero.Level * Hero.Intelligence);
			Boss.NowHp -= (int)damage;
			Hero.NowMp -= 30;
			swprintf_s(str, L"���ʥ����ʿ�ͷ��һ𽣷�...�Թ����ճ��ˡ�%d�����˺�", damage);
		}
		else
		{
			HeroActionType = ACTION_TYPE_MISS;
			swprintf_s(str, L"��ɵ��~!��ħ��ֵ����30�㣬ʩ��ʧ�ܣ���غϰ׷���~��");
		}
		Message_Insert(str);
		break;

	case ACTION_TYPE_RECOVER:
		if (Hero.NowMp >= 40)
		{
			Hero.NowHp -= 40;
			int HpRecover = 5 * (5 * (rand() % Hero.Intelligence) + 40);
			Hero.NowHp += HpRecover;
			if (Hero.NowHp >= Hero.MaxHp)
				Hero.NowHp = Hero.MaxHp;
			swprintf_s(str, L"���ʥ����ʿʹ�������������ָ��ˡ�%d��������ֵ���о��ö��ˡ�", HpRecover);
		}
		else
		{
			HeroActionType = ACTION_TYPE_MISS;
			swprintf_s(str, L"��ɵ��~!��ħ��ֵ����40�㣬ʩ��ʧ�ܣ���غϰ׷���~��");
		}
		Message_Insert(str);
		break;
	}
}

//-----------------------------------��HeroAction_Paint( )������-------------------------------
//	��������Ҷ�����ͼ����
//---------------------------------------------------------------------------------------------
VOID HeroAction_Paint()
{
	switch(HeroActionType)
	{
	case ACTION_TYPE_NORMAL: //��ͨ�������޵�ն
		SelectObject(g_bufdc, g_hHeroSkill1);
		TransparentBlt(g_mdc, 50, 170, 364, 140, g_bufdc, 0, 0, 364, 140, RGB(0, 0, 0));//TransparentBlt������͸����ɫ��ΪRGB(0,0,0)
		break;

	case ACTION_TYPE_CRITICAL:  //���������ͽ���
		SelectObject(g_bufdc, g_hHeroSkill3);
		TransparentBlt(g_mdc, 20, 60, 574, 306, g_bufdc, 0, 0, 574, 306, RGB(0, 0, 0));//TransparentBlt������͸����ɫ��ΪRGB(0,0,0)
		break;

	case ACTION_TYPE_MAGIC:  //ħ���������һ𽣷�
		SelectObject(g_bufdc, g_hHeroSkill2);
		TransparentBlt(g_mdc, 50, 100, 374, 288, g_bufdc, 0, 0, 374, 288, RGB(0, 0, 0));//TransparentBlt������͸����ɫ��ΪRGB(0,0,0)
		break;

	case ACTION_TYPE_RECOVER:   //�ָ���������
		SelectObject(g_bufdc, g_hRecoverSkill);
		TransparentBlt(g_mdc, 560, 170, 150, 150, g_bufdc, 0, 0, 150, 150, RGB(0, 0, 0));//TransparentBlt������͸����ɫ��ΪRGB(0,0,0)
		break;
	}
}

//-----------------------------------��BossAction_Logic( )������-------------------------------
//	������Boss�����߼��жϺ���
//---------------------------------------------------------------------------------------------
VOID BossAction_Logic()
{
	srand((unsigned)time(NULL));      //��ϵͳʱ���ʼ��������� 
	if (Boss.NowHp > (Boss.MaxHp / 2))				//����ֵ����1/2ʱ
	{
		switch (rand() % 3)
		{
		case 0:						//�ͷ���ͨ��������ڤ���
			BossActionType = ACTION_TYPE_NORMAL;
			break;
		case 1:						//�ͷ�����һ��
			BossActionType = ACTION_TYPE_CRITICAL;
			break;
		case 2:						//ʹ����Ѫ��
			BossActionType = ACTION_TYPE_MAGIC;
			break;
		}
	}
	else								//����ֵС��1/2ʱ
	{
		switch (rand() % 3)
		{
		case 0:						//�ͷ���Ѫ��
			BossActionType = ACTION_TYPE_MAGIC;
			break;
		case 1:						//�ͷ�����һ��
			BossActionType = ACTION_TYPE_CRITICAL;
			break;
		case 2:						//ʹ��÷��˹ķ�ظ�
			BossActionType = ACTION_TYPE_RECOVER;
			break;
		}
	}
}

//-----------------------------------��BossAction_Paint( )������-------------------------------
//	������Boss������ͼ����
//---------------------------------------------------------------------------------------------
VOID BossAction_Paint()
{
	int damage = 0, recover = 0;
	wchar_t str[100];

	switch (BossActionType)
	{
	case ACTION_TYPE_NORMAL:							//�ͷ���ͨ��������ڤ���
		SelectObject(g_bufdc, g_hBossSkill1);
		TransparentBlt(g_mdc, 500, 150, 234, 188, g_bufdc, 0, 0, 234, 188, RGB(0, 0, 0));//TransparentBlt������͸����ɫ��ΪRGB(0,0,0)
		//��30������ʱ����������˺��̶Ȳ�������ʾ��Ϣ
		if (g_iFrameNum == 30)
		{
			damage = rand() % Boss.Agility + Boss.Level * Boss.Strength;
			Hero.NowHp -= (int)damage;

			swprintf_s(str, L"�ƽ�ħ�����ͷ���ڤ���...������ճɡ� %d�� ���˺�", damage);
			Message_Insert(str);

			Die_Check(Hero.NowHp, true);
		}
		break;

	case ACTION_TYPE_MAGIC:							//�ͷ���Ѫ��
		SelectObject(g_bufdc, g_hBossSkill2);
		TransparentBlt(g_mdc, 450, 150, 387, 254, g_bufdc, 0, 0, 387, 254, RGB(0, 0, 0));//TransparentBlt������͸����ɫ��ΪRGB(0,0,0)
		//��30������ʱ����������˺��̶Ȳ�������ʾ��Ϣ
		if (g_iFrameNum == 30)
		{
			damage = 2 * (2 * (rand() % Boss.Agility) + Boss.Strength * Boss.Intelligence);  //��Ѫ����˺�ֵ����
			Hero.NowHp -= damage;	   //��Ӣ��Ѫ������һ������������˺�ֵ
			recover = (int)((float)damage * 0.2f);   //��Ѫ��20%
			Boss.NowHp += recover;   //����ָ�������Ѫ��
			swprintf_s(str, L"�ƽ�ħ�����ͷ���Ѫ��...������ճɡ� %d �����˺�,����ָ���%d��������ֵ", damage, recover);   //�����ָ�ʽ����������
			Message_Insert(str);   //����������Ϣ

			Die_Check(Hero.NowHp, true);
		}
		break;

	case ACTION_TYPE_CRITICAL:							//�ͷ�����һ��
		SelectObject(g_bufdc, g_hBossSkill3);
		TransparentBlt(g_mdc, 280, 100, 574, 306, g_bufdc, 0, 0, 574, 306, RGB(0, 0, 0));//TransparentBlt������͸����ɫ��ΪRGB(0,0,0)
		//��30������ʱ����������˺��̶Ȳ�������ʾ��Ϣ
		if (g_iFrameNum == 30)
		{
			damage = 2 * (rand() % Boss.Agility + Boss.Level * Boss.Strength);
			Hero.NowHp -= (int)damage;

			swprintf_s(str, L"�ƽ�ħ��������һ��...������ճɡ�%d�����˺�.", damage);
			Message_Insert(str);

			Die_Check(Hero.NowHp, true);
		}
		break;

	case ACTION_TYPE_RECOVER:							//ʹ��÷��˹ķ��Ѫ
		SelectObject(g_bufdc, g_hRecoverSkill);
		TransparentBlt(g_mdc, 150, 150, 150, 150, g_bufdc, 0, 0, 150, 150, RGB(0, 0, 0));//TransparentBlt������͸����ɫ��ΪRGB(0,0,0)
		//��30������ʱ����ظ�����ֵ��������ʾ��Ϣ
		if (g_iFrameNum == 30)
		{
			recover = 2 * Boss.Intelligence * Boss.Intelligence;
			Boss.NowHp += recover;
			swprintf_s(str, L"�ƽ�ħ����ʹ��÷��˹ķ...�ָ��ˡ�%d��������ֵ", recover);
			Message_Insert(str);
		}
		break;
	}
}

//-----------------------------------��Snow_Paint( )������--------------------------------------
//	��������װ��ѩ����������Ч���ĺ�����ģ����ѩ����
//--------------------------------------------------------------------------------------------------
VOID Snow_Paint()
{
	//��������
	if (g_SnowNum < PARTICLE_NUMBER)  //��������С�ڹ涨��������ʱ��������µ����ӣ��趨ÿ�����ӵ�����ֵ
	{
		SnowFlowers[g_SnowNum].x = rand() % g_rect.right; //�����ӵ�X������Ϊ������ˮƽ�����ϵ�����λ��
		SnowFlowers[g_SnowNum].y = 0;    //��ÿ�����ӵ�Y���궼��Ϊ"0"�����Ӵ�������������
		SnowFlowers[g_SnowNum].exist = true; //�趨���Ӵ���
		g_SnowNum++;   //ÿ����һ�����Ӻ�����ۼӼ���
	}

	//�����ж������Ƿ���ڣ������ڣ�����͸����ͼ����
	for (int i = 0; i < PARTICLE_NUMBER; i++)
	{
		if (SnowFlowers[i].exist)  //���ӻ�����
		{
			//��������ͼ
			SelectObject(g_bufdc, g_hSnow);
			TransparentBlt(g_mdc, SnowFlowers[i].x, SnowFlowers[i].y, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));

			//�������������ƶ������ƫ����
			if (rand() % 2 == 0)
				SnowFlowers[i].x += rand() % 6;  //x�������0~5֮���һ�����ֵ
			else
				SnowFlowers[i].x -= rand() % 6;	 //y�������0~5֮���һ�����ֵ
			//�ݷ������������˶�
			SnowFlowers[i].y += 10;  //���������10
			//����������곬���˴��ڳ��ȣ��������������x��������ڴ��ڶ���
			if (SnowFlowers[i].y > g_rect.bottom)
			{
				SnowFlowers[i].x = rand() % g_rect.right;
				SnowFlowers[i].y = 0;
			}
		}
	}
}

//-----------------------------------��Game_CleanUp( )������--------------------------------
//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
//------------------------------------------------------------------------------------------
BOOL Game_ShutDown(HWND hwnd)
{
	//�ͷ���Դ����
	DeleteObject(g_hBackGround);
	DeleteObject(g_hBackGround);
	DeleteObject(g_hGameOver);
	DeleteObject(g_hVictory);
	DeleteObject(g_hSnow);
	DeleteObject(g_hMonsterBitmap);
	DeleteObject(g_hHeroBitmap);
	DeleteObject(g_hRecoverSkill);
	DeleteObject(g_hSkillButton1);
	DeleteObject(g_hSkillButton2);
	DeleteObject(g_hSkillButton3);
	DeleteObject(g_hSkillButton4);
	DeleteObject(g_hHeroSkill1);
	DeleteObject(g_hHeroSkill2);
	DeleteObject(g_hHeroSkill3);
	DeleteObject(g_hBossSkill1);
	DeleteObject(g_hBossSkill2);
	DeleteObject(g_hBossSkill3);
	DeleteDC(g_bufdc);
	DeleteDC(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}