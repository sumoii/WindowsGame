//-----------------------------------【程序说明】----------------------------------------------
//  程序名称:：D3Ddemo16
//	 2024年9月 Create by sumoii
//  描述：翱翔于三维空间：第一人称摄像机的实现 示例程序
//  图标素材出处： 魔兽争霸3冰封王座 Dota
//  背景音乐素材出处：雅尼-兰花
//  人物模型素材出处：真三国无双6 王元姬  仿制版
//------------------------------------------------------------------------------------------------


//-----------------------------------【宏定义部分】--------------------------------------------
// 描述：定义一些辅助宏
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	932						//为窗口宽度定义的宏，以方便在此处修改窗口宽度
#define WINDOW_HEIGHT	700							//为窗口高度定义的宏，以方便在此处修改窗口高度
#define WINDOW_TITLE	_T("【致我们永不熄灭的游戏开发梦想】 翱翔于三维空间：第一人称摄像机的实现") //为窗口标题定义的宏


//-----------------------------------【头文件包含部分】---------------------------------------
//	描述：包含程序所依赖的头文件
//------------------------------------------------------------------------------------------------                                                                                     
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <time.h> 
#include "DirectInputClass.h"
#include "CameraClass.h"


//-----------------------------------【库文件包含部分】---------------------------------------
//	描述：包含程序所依赖的库文件
//------------------------------------------------------------------------------------------------  
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "dinput8.lib")     // 使用DirectInput必须包含的库文件，注意这里有8
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "winmm.lib") 


//定义顶点结构体
struct CUSTOMVERTEX
{
	FLOAT _x, _y, _z;
	FLOAT _nx, _ny, _nz;
	FLOAT _u, _v;
	CUSTOMVERTEX(FLOAT x, FLOAT y, FLOAT z,
		FLOAT nx, FLOAT ny, FLOAT nz, FLOAT u, FLOAT v)
	{
		_x = x, _y = y, _z = z;
		_nx = nx, _ny = ny, _nz = nz;
		_u = u, _v = v;
	}
};
#define D3DFVF_CUSTOMVERTEX  (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)


//-----------------------------------【全局变量声明部分】-------------------------------------
//	描述：全局变量的声明
//------------------------------------------------------------------------------------------------
LPDIRECT3DDEVICE9					g_pd3dDevice = NULL; //Direct3D设备对象
LPD3DXFONT								g_pTextFPS = NULL;    //字体COM接口
LPD3DXFONT								g_pTextAdaperName = NULL;  // 显卡信息的2D文本
LPD3DXFONT								g_pTextHelper = NULL;  // 帮助信息的2D文本
LPD3DXFONT								g_pTextInfor = NULL;  // 绘制信息的2D文本
float											g_FPS = 0.0f;       //一个浮点型的变量，代表帧速率
wchar_t										g_strFPS[50] = { 0 };    //包含帧速率的字符数组
wchar_t										g_strAdapterName[60] = { 0 };    //包含显卡名称的字符数组
D3DXMATRIX							g_matWorld;   //世界矩阵
DInputClass* g_pDInput = NULL;         //一个DInputClass类的指针
CameraClass* g_pCamera = NULL;
LPD3DXMESH							g_pMesh = NULL; // 网格对象
D3DMATERIAL9* g_pMaterials = NULL; // 网格的材质信息
LPDIRECT3DTEXTURE9* g_pTextures = NULL; // 网格的纹理信息
DWORD									g_dwNumMtrls = 0;    // 材质的数目
LPD3DXMESH							g_cylinder = NULL;     //柱子网格对象
D3DMATERIAL9							g_MaterialCylinder; //材质
LPDIRECT3DVERTEXBUFFER9		g_pVertexBuffer = NULL;  //绘制草地的顶点缓存对象
LPDIRECT3DTEXTURE9				g_pTexture = NULL;  //绘制草地的纹理对象


//-----------------------------------【全局函数声明部分】-------------------------------------
//	描述：全局函数声明，防止“未声明的标识”系列错误
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK		WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT						Direct3D_Init(HWND hwnd, HINSTANCE hInstance);
HRESULT						Objects_Init();
void								Direct3D_Render(HWND hwnd);
void								Direct3D_Update(HWND hwnd);
void								Direct3D_CleanUp();
float								Get_FPS();
void								HelpText_Render(HWND hwnd);

//-----------------------------------【WinMain( )函数】--------------------------------------
//	描述：Windows应用程序的入口函数，我们的程序从这里开始
//------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	//开始设计一个完整的窗口类
	WNDCLASSEX wndClass = { 0 };				//用WINDCLASSEX定义了一个窗口类，即用wndClass实例化了WINDCLASSEX，用于之后窗口的各项初始化    
	wndClass.cbSize = sizeof(WNDCLASSEX);	//设置结构体的字节数大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//设置窗口的样式
	wndClass.lpfnWndProc = WndProc;				//设置指向窗口过程函数的指针
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;				//指定包含窗口过程的程序的实例句柄。
	wndClass.hIcon = (HICON)::LoadImage(NULL, _T("../../../../resource/Chapter21/D3Ddemo16/icon.ico"), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE); //从全局的::LoadImage函数从本地加载自定义ico图标
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //指定窗口类的光标句柄。
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);  //为hbrBackground成员指定一个灰色画刷句柄
	wndClass.lpszMenuName = NULL;						//用一个以空终止的字符串，指定菜单资源的名字。
	wndClass.lpszClassName = _T("ForTheDreamOfGameDevelop");		//用一个以空终止的字符串，指定窗口类的名字。

	if (!RegisterClassEx(&wndClass))				//设计完窗口后，需要对窗口类进行注册，这样才能创建该类型的窗口
		return -1;

	HWND hwnd = CreateWindow(_T("ForTheDreamOfGameDevelop"), WINDOW_TITLE,			//喜闻乐见的创建窗口函数CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);


	//Direct3D资源的初始化，调用失败用messagebox予以显示
	if (!(S_OK == Direct3D_Init(hwnd, hInstance)))
	{
		MessageBox(hwnd, _T("Direct3D初始化失败~！"), _T("浅墨的消息窗口"), 0); //使用MessageBox函数，创建一个消息窗口 
	}
	PlaySound(L"../../../../resource/Chapter21/D3Ddemo16/雅尼 - 兰花.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);   //循环播放背景音乐



	MoveWindow(hwnd, 200, 10, WINDOW_WIDTH, WINDOW_HEIGHT, true);   //调整窗口显示时的位置，窗口左上角位于屏幕坐标（200，10）处
	ShowWindow(hwnd, nShowCmd);    //调用Win32函数ShowWindow来显示窗口
	UpdateWindow(hwnd);  //对窗口进行更新，就像我们买了新房子要装修一样

	//进行DirectInput类的初始化
	g_pDInput = new DInputClass();
	g_pDInput->Init(hwnd, hInstance, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//消息循环过程
	MSG msg = { 0 };  //初始化msg
	while (msg.message != WM_QUIT)			//使用while循环
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //查看应用程序消息队列，有消息时将队列中的消息派发出去。
		{
			TranslateMessage(&msg);		//将虚拟键消息转换为字符消息
			DispatchMessage(&msg);		//该函数分发一个消息给窗口程序。
		}
		else
		{
			Direct3D_Update(hwnd);         //调用更新函数，进行画面的更新
			Direct3D_Render(hwnd);			//调用渲染函数，进行画面的渲染			
		}
	}

	UnregisterClass(_T("ForTheDreamOfGameDevelop"), wndClass.hInstance);
	return 0;
}



//-----------------------------------【WndProc( )函数】--------------------------------------
//	描述：窗口过程函数WndProc,对窗口消息进行处理
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)   //窗口过程函数WndProc
{
	switch (message)				//switch语句开始
	{
	case WM_PAINT:					 // 客户区重绘消息
		Direct3D_Render(hwnd);          //调用Direct3D_Render函数，进行画面的绘制
		ValidateRect(hwnd, NULL);   // 更新客户区的显示
		break;									//跳出该switch语句

	case WM_KEYDOWN:                // 键盘按下消息
		if (wParam == VK_ESCAPE)    // ESC键
			DestroyWindow(hwnd);    // 销毁窗口, 并发送一条WM_DESTROY消息
		break;
	case WM_DESTROY:				//窗口销毁消息
		Direct3D_CleanUp();     //调用Direct3D_CleanUp函数，清理COM接口对象
		PostQuitMessage(0);		//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
		break;						//跳出该switch语句

	default:						//若上述case条件都不符合，则执行该default语句
		return DefWindowProc(hwnd, message, wParam, lParam);		//调用缺省的窗口过程来为应用程序没有处理的窗口消息提供缺省的处理。
	}

	return 0;					//正常退出
}


//-----------------------------------【Direct3D_Init( )函数】----------------------------------
//	描述：Direct3D初始化函数，进行Direct3D的初始化
//------------------------------------------------------------------------------------------------
HRESULT Direct3D_Init(HWND hwnd, HINSTANCE hInstance)
{

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之一，创接口】：创建Direct3D接口对象, 以便用该Direct3D对象创建Direct3D设备对象
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D接口对象的创建
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION))) //初始化Direct3D接口对象，并进行DirectX版本协商
		return E_FAIL;

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之二,取信息】：获取硬件设备信息
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //支持硬件顶点运算，我们就采用硬件顶点运算，妥妥的
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //不支持硬件顶点运算，无奈只好采用软件顶点运算

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之三，填内容】：填充D3DPRESENT_PARAMETERS结构体
	//--------------------------------------------------------------------------------------
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = WINDOW_WIDTH;
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 2;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之四，创设备】：创建Direct3D设备接口
	//--------------------------------------------------------------------------------------
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;

	//获取显卡信息到g_strAdapterName中，并在显卡名称之前加上“当前显卡型号：”字符串
	wchar_t TempName[60] = L"当前显卡型号：";   //定义一个临时字符串，且方便了把"当前显卡型号："字符串引入我们的目的字符串中
	D3DADAPTER_IDENTIFIER9 Adapter;  //定义一个D3DADAPTER_IDENTIFIER9结构体，用于存储显卡信息
	pD3D->GetAdapterIdentifier(0, 0, &Adapter);//调用GetAdapterIdentifier，获取显卡信息
	int len = MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, NULL, 0);//显卡名称现在已经在Adapter.Description中了，但是其为char类型，我们要将其转为wchar_t类型
	MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, g_strAdapterName, len);//这步操作完成后，g_strAdapterName中就为当前我们的显卡类型名的wchar_t型字符串了
	wcscat_s(TempName, g_strAdapterName);//把当前我们的显卡名加到“当前显卡型号：”字符串后面，结果存在TempName中
	wcscpy_s(g_strAdapterName, TempName);//把TempName中的结果拷贝到全局变量g_strAdapterName中，大功告成~

	if (!(S_OK == Objects_Init())) return E_FAIL;

	SAFE_RELEASE(pD3D) //LPDIRECT3D9接口对象的使命完成，我们将其释放掉

	return S_OK;
}

//-----------------------------------【Object_Init( )函数】--------------------------------------
//	描述：渲染资源初始化函数，在此函数中进行要被渲染的物体的资源的初始化
//--------------------------------------------------------------------------------------------------
HRESULT Objects_Init()
{
	//创建字体
	D3DXCreateFont(g_pd3dDevice, 36, 0, 0, 1000, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"), &g_pTextFPS);
	D3DXCreateFont(g_pd3dDevice, 20, 0, 1000, 0, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"华文中宋", &g_pTextAdaperName);
	D3DXCreateFont(g_pd3dDevice, 23, 0, 1000, 0, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"微软雅黑", &g_pTextHelper);
	D3DXCreateFont(g_pd3dDevice, 26, 0, 1000, 0, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"黑体", &g_pTextInfor);

	// 从X文件中加载网格数据
	LPD3DXBUFFER pAdjBuffer = NULL;
	LPD3DXBUFFER pMtrlBuffer = NULL;

	D3DXLoadMeshFromX(L"../../../../resource/Chapter21/D3Ddemo16/WYJ.X", D3DXMESH_MANAGED, g_pd3dDevice,
		&pAdjBuffer, &pMtrlBuffer, NULL, &g_dwNumMtrls, &g_pMesh);

	// 读取材质和纹理数据
	D3DXMATERIAL* pMtrls = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer(); //创建一个D3DXMATERIAL结构体用于读取材质和纹理信息
	g_pMaterials = new D3DMATERIAL9[g_dwNumMtrls];
	g_pTextures = new LPDIRECT3DTEXTURE9[g_dwNumMtrls];

	for (DWORD i = 0; i < g_dwNumMtrls; i++)
	{
		//获取材质，并设置一下环境光的颜色值
		g_pMaterials[i] = pMtrls[i].MatD3D;
		g_pMaterials[i].Ambient = g_pMaterials[i].Diffuse;

		//创建一下纹理对象
		g_pTextures[i] = NULL;
		wchar_t TempName[180] = L"../../../../resource/Chapter21/D3Ddemo16/";
		wchar_t m_strfileName[180] = { 0 };
		int len = MultiByteToWideChar(CP_ACP, 0, pMtrls[i].pTextureFilename, -1, NULL, 0);//图片名称现在已经在中pMtrls[i].pTextureFilename了，但是其为char类型，我们要将其转为wchar_t类型
		MultiByteToWideChar(CP_ACP, 0, pMtrls[i].pTextureFilename, -1, m_strfileName, len);//这步操作完成后，m_strfileName中就为当前我们的纹理图片名的wchar_t型字符串了
		wcscat_s(TempName, m_strfileName);//把当前图片路径，结果存在TempName中
		wcscpy_s(m_strfileName, TempName);//把TempName中的结果拷贝到局部变量m_strfileName中，大功告成~
		D3DXCreateTextureFromFileW(g_pd3dDevice, m_strfileName, &g_pTextures[i]);
	}
	SAFE_RELEASE(pAdjBuffer);
	SAFE_RELEASE(pMtrlBuffer);


	// 创建一片草坪,50X50=250张纹理
	g_pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX), 0,
		D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &g_pVertexBuffer, 0);

	CUSTOMVERTEX* pVertices = NULL;
	g_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = CUSTOMVERTEX(-500.0f, 0.0f, -500.0f, 0.0f, 1.0f, 0.0f, 0.0f, 50.0f);
	pVertices[1] = CUSTOMVERTEX(-500.0f, 0.0f, 500.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	pVertices[2] = CUSTOMVERTEX(500.0f, 0.0f, -500.0f, 0.0f, 1.0f, 0.0f, 50.0f, 50.0f);
	pVertices[3] = CUSTOMVERTEX(500.0f, 0.0f, 500.0f, 0.0f, 1.0f, 0.0f, 50.0f, 0.0f);
	g_pVertexBuffer->Unlock();

	// 创建地板纹理
	D3DXCreateTextureFromFile(g_pd3dDevice, L"../../../../resource/Chapter21/D3Ddemo16/grass.jpg", &g_pTexture);

	//创建柱子
	D3DXCreateCylinder(g_pd3dDevice, 10.0f, 10.0f, 500.0f, 60, 60, &g_cylinder, 0);
	g_MaterialCylinder.Ambient = D3DXCOLOR(0.9f, 0.0f, 0.8f, 1.0f);
	g_MaterialCylinder.Diffuse = D3DXCOLOR(0.9f, 0.0f, 0.8f, 1.0f);
	g_MaterialCylinder.Specular = D3DXCOLOR(0.9f, 0.2f, 0.9f, 0.9f);
	g_MaterialCylinder.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.9f, 1.0f);

	// 设置光照  
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable(0, true);
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	// 创建并初始化虚拟摄像机
	g_pCamera = new CameraClass(g_pd3dDevice);
	D3DXVECTOR3 m_pVecta = D3DXVECTOR3(0.0f, 200.0f, -300.0f);
	D3DXVECTOR3 m_pVectb = D3DXVECTOR3(0.0f, 300.0f, 0.0f);
	g_pCamera->SetCameraPosition(&m_pVecta);  //设置摄像机所在的位置
	g_pCamera->SetTargetPosition(&m_pVectb);  //设置目标观察点所在的位置
	g_pCamera->SetViewMatrix();  //设置取景变换矩阵
	g_pCamera->SetProjMatrix();  //设置投影变换矩阵

	// 设置纹理过滤和纹理寻址方式
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}


//-----------------------------------【Direct3D_Update( )函数】--------------------------------
//	描述：不是即时渲染代码但是需要即时调用的，如按键后的坐标的更改，都放在这里
//--------------------------------------------------------------------------------------------------
void	Direct3D_Update(HWND hwnd)
{
	//使用DirectInput类读取数据
	g_pDInput->GetInput();

	// 沿摄像机各分量移动视角
	if (g_pDInput->IsKeyDown(DIK_A))  g_pCamera->MoveAlongRightVec(-0.3f);
	if (g_pDInput->IsKeyDown(DIK_D))  g_pCamera->MoveAlongRightVec(0.3f);
	if (g_pDInput->IsKeyDown(DIK_W)) g_pCamera->MoveAlongLookVec(0.3f);
	if (g_pDInput->IsKeyDown(DIK_S))  g_pCamera->MoveAlongLookVec(-0.3f);
	if (g_pDInput->IsKeyDown(DIK_R))  g_pCamera->MoveAlongUpVec(0.3f);
	if (g_pDInput->IsKeyDown(DIK_F))  g_pCamera->MoveAlongUpVec(-0.3f);

	//沿摄像机各分量旋转视角
	if (g_pDInput->IsKeyDown(DIK_LEFT))  g_pCamera->RotationUpVec(-0.003f);
	if (g_pDInput->IsKeyDown(DIK_RIGHT))  g_pCamera->RotationUpVec(0.003f);
	if (g_pDInput->IsKeyDown(DIK_UP))  g_pCamera->RotationRightVec(-0.003f);
	if (g_pDInput->IsKeyDown(DIK_DOWN))  g_pCamera->RotationRightVec(0.003f);
	if (g_pDInput->IsKeyDown(DIK_Q)) g_pCamera->RotationLookVec(0.001f);
	if (g_pDInput->IsKeyDown(DIK_E)) g_pCamera->RotationLookVec(-0.001f);


	//鼠标控制右向量和上向量的旋转
	g_pCamera->RotationUpVec(g_pDInput->MouseDX() * 0.001f);
	g_pCamera->RotationRightVec(g_pDInput->MouseDY() * 0.001f);

	//鼠标滚轮控制观察点收缩操作
	static FLOAT fPosZ = 0.0f;
	fPosZ += g_pDInput->MouseDZ() * 0.03f;

	//计算并设置取景变换矩阵
	D3DXMATRIX matView;
	g_pCamera->CalculateViewMatrix(&matView);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	g_pCamera->SetProjMatrix();

	//把正确的世界变换矩阵存到g_matWorld中
	D3DXMatrixTranslation(&g_matWorld, 0.0f, 0.0f, fPosZ);

	//以下这段代码用于限制鼠标光标移动区域
	POINT lt, rb;
	RECT rect;
	GetClientRect(hwnd, &rect);  //取得窗口内部矩形
	//将矩形左上点坐标存入lt中
	lt.x = rect.left;
	lt.y = rect.top;
	//将矩形右下坐标存入rb中
	rb.x = rect.right;
	rb.y = rect.bottom;
	//将lt和rb的窗口坐标转换为屏幕坐标
	ClientToScreen(hwnd, &lt);
	ClientToScreen(hwnd, &rb);
	//以屏幕坐标重新设定矩形区域
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	//限制鼠标光标移动区域
	ClipCursor(&rect);

	ShowCursor(false);		//隐藏鼠标光标
}


//-----------------------------------【Direct3D_Render( )函数】-------------------------------
//	描述：使用Direct3D进行渲染
//--------------------------------------------------------------------------------------------------
void Direct3D_Render(HWND hwnd)
{
	//--------------------------------------------------------------------------------------
	// 【Direct3D渲染五步曲之一】：清屏操作
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(50, 100, 250), 1.0f, 0);

	//--------------------------------------------------------------------------------------
	// 【Direct3D渲染五步曲之二】：开始绘制
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->BeginScene();                     // 开始绘制

	//--------------------------------------------------------------------------------------
	// 【Direct3D渲染五步曲之三】：正式绘制
	//--------------------------------------------------------------------------------------

	//绘制人物
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &g_matWorld);//设置模型的世界矩阵，为绘制做准备
	// 用一个for循环，进行模型的网格各个部分的绘制
	for (DWORD i = 0; i < g_dwNumMtrls; i++)
	{
		g_pd3dDevice->SetMaterial(&g_pMaterials[i]);  //设置此部分的材质
		g_pd3dDevice->SetTexture(0, g_pTextures[i]);//设置此部分的纹理
		g_pMesh->DrawSubset(i);  //绘制此部分
	}


	// 绘制草坪
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pd3dDevice->SetTexture(0, g_pTexture);
	g_pd3dDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//绘制柱子
	D3DXMATRIX TransMatrix, RotMatrix, FinalMatrix;
	D3DXMatrixRotationX(&RotMatrix, -D3DX_PI * 0.5f);
	g_pd3dDevice->SetMaterial(&g_MaterialCylinder);
	for (int i = 0; i < 6; i++)
	{
		D3DXMatrixTranslation(&TransMatrix, -100.0f, 0.0f, -150.0f + (i * 75.0f));
		FinalMatrix = RotMatrix * TransMatrix;
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &FinalMatrix);
		g_cylinder->DrawSubset(0);

		D3DXMatrixTranslation(&TransMatrix, 100.0f, 0.0f, -150.0f + (i * 75.0f));
		FinalMatrix = RotMatrix * TransMatrix;
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &FinalMatrix);
		g_cylinder->DrawSubset(0);
	}

	HelpText_Render(hwnd);


	//--------------------------------------------------------------------------------------
	// 【Direct3D渲染五步曲之四】：结束绘制
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->EndScene();                       // 结束绘制
	//--------------------------------------------------------------------------------------
	// 【Direct3D渲染五步曲之五】：显示翻转
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);  // 翻转与显示

}

//-----------------------------------【HelpText_Render( )函数】-------------------------------
//	描述：封装了帮助信息的函数
//--------------------------------------------------------------------------------------------------
void HelpText_Render(HWND hwnd)
{
	//定义一个矩形，用于获取主窗口矩形
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);

	//在窗口右上角处，显示每秒帧数
	formatRect.top = 5;
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS());
	g_pTextFPS->DrawText(NULL, g_strFPS, charCount, &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_RGBA(0, 239, 136, 255));

	//显示显卡类型名
	g_pTextAdaperName->DrawText(NULL, g_strAdapterName, -1, &formatRect,
		DT_TOP | DT_LEFT, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));

	// 输出帮助信息
	formatRect.left = 0, formatRect.top = 380;
	g_pTextInfor->DrawText(NULL, L"控制说明:", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(235, 123, 230, 255));
	formatRect.top += 35;
	g_pTextHelper->DrawText(NULL, L"    W：向前飞翔     S：向后飞翔 ", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    A：向左飞翔     D：向右飞翔", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    R：垂直向上飞翔     F：垂直向下飞翔", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    Q：向左倾斜       E：向右倾斜", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    上、下、左、右方向键、鼠标移动：视角变化 ", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"     鼠标滚轮：人物模型Y轴方向移动", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    ESC键 : 退出程序", -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 200, 0, 255));
}


//-----------------------------------【Get_FPS( )函数】------------------------------------------
//	描述：用于计算每秒帧速率的一个函数
//--------------------------------------------------------------------------------------------------
float Get_FPS()
{

	//定义四个静态变量
	static float  fps = 0; //我们需要计算的FPS值
	static int    frameCount = 0;//帧数
	static float  currentTime = 0.0f;//当前时间
	static float  lastTime = 0.0f;//持续时间

	frameCount++;//每调用一次Get_FPS()函数，帧数自增1
	currentTime = timeGetTime() * 0.001f;//获取系统时间，其中timeGetTime函数返回的是以毫秒为单位的系统时间，所以需要乘以0.001，得到单位为秒的时间

	//如果当前时间减去持续时间大于了1秒钟，就进行一次FPS的计算和持续时间的更新，并将帧数值清零
	if (currentTime - lastTime > 1.0f) //将时间控制在1秒钟
	{
		fps = (float)frameCount / (currentTime - lastTime);//计算这1秒钟的FPS值
		lastTime = currentTime; //将当前时间currentTime赋给持续时间lastTime，作为下一秒的基准时间
		frameCount = 0;//将本次帧数frameCount值清零
	}

	return fps;
}


//-----------------------------------【Direct3D_CleanUp( )函数】--------------------------------
//	描述：对Direct3D的资源进行清理，释放COM接口对象
//---------------------------------------------------------------------------------------------------
void Direct3D_CleanUp()
{
	//释放COM接口对象
	for (DWORD i = 0; i < g_dwNumMtrls; i++)
		SAFE_RELEASE(g_pTextures[i]);
	SAFE_DELETE(g_pTextures);
	SAFE_DELETE(g_pMaterials);
	SAFE_DELETE(g_pDInput);
	SAFE_RELEASE(g_cylinder);
	SAFE_RELEASE(g_pMesh);
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pTextAdaperName);
	SAFE_RELEASE(g_pTextHelper);
	SAFE_RELEASE(g_pTextInfor);
	SAFE_RELEASE(g_pTextFPS);
	SAFE_RELEASE(g_pd3dDevice);
}