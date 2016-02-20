//*******************************************************
//头文件
//*******************************************************
#include "d3dUtility.h"
#include"Terrain.h"
#include"CameraClass.h"
#include"skyboxclass.h"
#include <tchar.h>
#include <Mmsystem.h>
#include"XFileMesh.h"
#include"Snake.h"
#include"Props.h"
#include"DirectInputClass.h"
#include"Ground.h"
#include"Snow.h"
#include"D3DGUI.h"
#include"GameUI.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
//====================================================================
//全局变量声明部分
//====================================================================
LPDIRECT3DDEVICE9 Device = 0;			//		D3D设备接口
D3DXMATRIX World;						//定义了一个矩阵
CamerClass Camera(CamerClass::air);
SkyBox* skybox=NULL;
Terrain *terrain=NULL;
Snake *snake=NULL;
Snow * snow=NULL;
Props *props=NULL;						//道具类指针
DInputClass *DInput=NULL;				//Dinput类指针
IDirect3DTexture9 * SnakeBodyTex;
D3DMATERIAL9 mtrl;
Ground *groundobject=NULL;
GameUI *gameui=NULL;
GameUI *gameui2=NULL;
float gametime =0;


bool g_LMBDown = false;      // GUI中的鼠标状态信息，鼠标左键是否按下的标识
int	 g_MouseX = 0, g_MouseY = 0;      //存储鼠标坐标的两个变量

float timedelta;
//
 //创建四个GUI类对象，分别代表四个页面
D3DGUIClass		*g_MainGUI		= NULL;//主窗口
D3DGUIClass		*g_StartGUI		= NULL; //游戏开始窗口
D3DGUIClass		*g_ControlGUI	= NULL; //游戏载入窗口
D3DGUIClass		*g_StoreGUI		= NULL; //游戏设置窗口

int		g_MainGUIFontID		= -1;						//  GUI中字体对象的ID
int		g_StartGUIFontID	= -1;						//  GUI中字体对象的ID
int		g_ControlGUIFontID	= -1;						//  GUI中字体对象的ID
int		g_StoreGUIFontID	= -1;					//  GUI中字体对象的ID
int		g_currentGUI		= GUI_MAIN_SCREEN;    //一个当前的GUI标识

//=====================================
//外部函数声明
//=====================================
extern void KeyboardAndMouse( CamerClass *Camera,float  );
//===========================================
//全局函数声明部分
//===========================================
bool Setup();
void Cleanup();
bool Display(float timeDelta);
void ComputeNormal(D3DXVECTOR3 *P0,D3DXVECTOR3 *P1,D3DXVECTOR3 *P2,D3DXVECTOR3 *OUT);//计算面法向量
void CalcuVertexVec(D3DXVECTOR3 *a,D3DXVECTOR3 *b,D3DXVECTOR3 *c,D3DXVECTOR3 *d,D3DXVECTOR3 *Res);
void	GUICallback(int id, int state); //GUI的回调函数
//===========================================
// WinMain 程序入口函数
//===========================================
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	HWND hwnd = 0;

	if(!d3d::InitD3D(hinstance,800, 600, true, D3DDEVTYPE_HAL, &Device,&hwnd))
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}

	DInput=new DInputClass ();
	DInput->Init(hwnd,hinstance,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


	if(!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}
	d3d::EnterMsgLoop( Display );
	Cleanup();
	Device->Release();
	return 0;
}

// WndProc
// 窗口消息处理函数

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	 
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		break;
	case WM_LBUTTONDOWN:
			g_LMBDown = true;
			break;

	case WM_LBUTTONUP:
			g_LMBDown = false;
			break;

	case WM_MOUSEMOVE:
			g_MouseX = LOWORD (lParam);
			g_MouseY = HIWORD (lParam);
			break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}
//
//设置函数
//
bool Setup()                                                                                                                                                                                                                                                                                                                                                             
{
	PlaySound(L"3.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	//======================================================================================================================
	//GUI系统的一些设置
	//======================================================================================================================

	// 创建一些GUI系统
	g_MainGUI = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //主菜单页面
	g_StartGUI = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //开始新游戏页面
	g_ControlGUI  = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //载入游戏页面
	g_StoreGUI  = new D3DGUIClass(Device, WINDOW_WIDTH, WINDOW_HEIGHT); //设置页面

	// 给四个页面分别添加背景图
	if(!g_MainGUI->AddBackground(L"maingui.jpg")) return false;  
	if(!g_StartGUI->AddBackground (L"gameover.jpg")) return false ;
	if(!g_ControlGUI ->AddBackground(L"controlgui.jpg")) return false;
	if(!g_StoreGUI ->AddBackground(L"storegui.jpg")) return false;


	// 分别给四个页面添加字体
	if(!g_MainGUI->CreateTextFont(L"微软雅黑", 28, &g_MainGUIFontID)) return false;
	if(!g_StartGUI->CreateTextFont(L"微软雅黑", 38, &g_StartGUIFontID)) return false;
	if(!g_ControlGUI ->CreateTextFont(L"微软雅黑", 38, &g_ControlGUIFontID)) return false;
	if(!g_StoreGUI ->CreateTextFont(L"微软雅黑", 38, &g_StoreGUIFontID)) return false;

	//---------------------------------【主菜单main页面相关的页面布局代码】----------------------

	// 添加4个按钮，分别是开始游戏，载入进度，选项和退出游戏，每个按钮对应3幅图
	if(!g_MainGUI->AddButton(BUTTON_START_ID, 0, 0, L"START_UP.png",
		L"START_OVER.png", L"START_DOWN.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_LOAD_ID, 0, 150, L"CONTROL_UP.png",
		L"CONTROL_OVER.png", L"CONTROL_DOWN.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_OPTION_ID,0, 330, L"STORE_UP.png",
		L"STORE_OVER.png", L"STORE_DOWN.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_QUIT_ID, 0, 475, L"THE_END_UP.png",
		L"THE_END_OVER.png", L"THE_END_DOWN.png"))return false ;

	//------------------------【开始新游戏start页面相关的页面布局代码】------------------------

	if(!g_StartGUI->AddButton(BUTTON_LEVEL_1_ID,550, 1380,  L"level1Up.png", L"level1Over.png",
		L"level1Down.png"))return false;

	if(!g_StartGUI->AddButton(BUTTON_BACK_ID, 650, 500, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;

	//------------------------【游戏操作介绍页面页面相关的页面布局代码】-----------------------

	// 添加按钮到页面中
	if(!g_ControlGUI ->AddButton(BUTTON_BACK_ID, 650, 500, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;

	//------------------------【游戏设置option页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	if(!g_StoreGUI ->AddButton(BUTTON_BACK_ID,650, 500, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;

	//设定融合因子
	Device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	Device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//指定Alpha来源
	Device->SetTextureStageState (0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	Device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);

	Device->SetRenderState(D3DRS_LIGHTING,true);
	Device->SetRenderState(D3DRS_NORMALIZENORMALS,true);

	d3d::BoundingBox boundingBox;
	boundingBox._min = D3DXVECTOR3(-500.0f, 0.0f, -500.0f);
	boundingBox._max = D3DXVECTOR3( 500.0f,  300.0f,  500.0f);
	snow = new Snow(&boundingBox, 20);
	snow->init(Device, L"花4.png");

	skybox= new SkyBox(Device);
	skybox->LoadSkyTextureFromFile (L"frontsnow1.jpg",L"backsnow1.jpg",L"leftsnow1.jpg",L"rightsnow1.jpg",L"topsnow1.jpg",L"groundsnow1.jpg");//加载天空盒贴图

	terrain=new Terrain(Device,"heighmap.raw", 200, 200, 60,0.0f);//创建一个地形类
	terrain->loadTexture(L"ground.jpg");

	snake=new Snake(Device);
	if(!snake->initSnake ())
		::MessageBox(0, L"InitSnake() - FAILED", 0, 0);

	props=new Props(Device);
	if(!props->initProps ())
		::MessageBox(0,L"InitProps()_FAILED" ,0,0);


	groundobject=new Ground (Device);
	groundobject->initObject ();
	D3DXVECTOR3 position;
	position.x =0.0f;position.y =0.0f;position.z =0.0f;//指定围墙中心位置
	groundobject->createBird (position);

	gameui=new GameUI(Device);//游戏界面的UI
	if(!gameui->initPicture  (L"分数.png",10.0f,10.0f,5.0f))
		::MessageBox(0,L"InitUI()_FAILED" ,0,0);
	if(!gameui->initTex(L"宋体"))
		::MessageBox(0,L"InitFont()_FAILED" ,0,0);

	gameui2=new GameUI(Device);//游戏界面的UI
	if(!gameui2->initPicture  (L"EventObjectIcon4.dds",600.0f,10.0f,1.5f)) 
		::MessageBox(0,L"InitUI()_FAILED" ,0,0);

	//设置投影变换
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
	D3DX_PI*0.5f,
	800.0/600.0,
	1.0f,
	30000.0f
		);
	Device->SetTransform (D3DTS_PROJECTION,&proj);

	D3DVIEWPORT9 VP={0,0,800,600,0,1};
	Device->SetViewport (&VP);


	//设置材质
	
	::ZeroMemory(&mtrl,sizeof(mtrl));
	D3DXCOLOR colorA=d3d::BLACK;
	D3DXCOLOR colorB=d3d::GREEN;
	D3DXCOLOR colorC=d3d::RED;
	D3DXCOLOR colorD=d3d::YELLOW;
	mtrl=d3d::IintMetirial(&colorA,&colorB,&colorC,&colorD,1.0f);
	Device->SetMaterial (&mtrl);

	//设置灯光
	D3DLIGHT9 light;
	::ZeroMemory(&light,sizeof(light));
	D3DXCOLOR LightColor=d3d::GREEN  ;
	D3DXVECTOR3 LightPosition (0.0f,0.0f,0.0f);
	light=d3d::InitPointlight(&LightPosition,&LightColor);
	Device->SetLight (0,&light);	
	Device->LightEnable (0,true);
	Device->SetRenderState (D3DRS_CULLMODE,D3DCULL_NONE);
	skybox->InitSkyBox(20000.f);//初始化天空盒
	//设置纹理过滤方式
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);  
   	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);  
    Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);  

	D3DXCreateTextureFromFile(Device,L"ground.jpg",&SnakeBodyTex);

	return true;
}

//渲染函数
//
//
bool Display(float timeDelta)
{
	timedelta=timeDelta;
	if( Device ) 
	{ 
		
		//*************绘制区域*********************
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
		Device->BeginScene ();
		
		if(g_currentGUI == GUI_MAIN_SCREEN)
			ProcessGUI(g_MainGUI, g_LMBDown, g_MouseX,g_MouseY, GUICallback);
		else if(g_currentGUI == GUI_START_SCREEN)
			ProcessGUI(g_StartGUI, g_LMBDown, g_MouseX,g_MouseY, GUICallback);
		else if(g_currentGUI == GUI_LOAD_SCREEN)
			ProcessGUI(g_ControlGUI , g_LMBDown, g_MouseX,g_MouseY, GUICallback);
		else if(g_currentGUI == GUI_OPTION_SCREEN)
			ProcessGUI(g_StoreGUI , g_LMBDown, g_MouseX,g_MouseY, GUICallback);
		else
			ProcessGUI(g_MainGUI, g_LMBDown, g_MouseX,g_MouseY, GUICallback);

		Device->EndScene ();
		Device->Present(0, 0, 0, 0);//提交后台缓存示翻转）

	}
	return true;
}


void  render (float timeDelta)
{
		//设置取景变换
	if (snake->ifDead ()==false)
	{
		gametime += timeDelta ;//计算游戏时间

		D3DXMATRIX V;
		Camera.SetViewMatrix(&V);
		Device->SetTransform  (D3DTS_VIEW,&V);
		KeyboardAndMouse(&Camera ,timeDelta);	
		D3DXMATRIX TerrainWorld;
		D3DXMatrixTranslation(&TerrainWorld,0.0f,00.0f,0.0f);

		//*************更新区域*********************
		
		props->upDateProps (timeDelta);
		groundobject->updateBird (timeDelta);
		snow->update(timeDelta);//掉落蓝色的花
		snake->changeWallRange (groundobject);
		D3DXVECTOR3 position;
		position.x =0.0f,position.y =0.0f,position.z =0.0f;
		groundobject->createWall (position);

		//为啥要把蛇的更新函数放在这里呢？因为里面包含了渲染字体的操作...
		snake->upDateSanke (timeDelta,DInput); 
		
		//开启深度缓存
		//开启深度缓存
		Device->SetRenderState (D3DRS_ZENABLE,true);
		Device->SetRenderState (D3DRS_ZFUNC,D3DCMP_LESS);
		Device->SetRenderState (D3DRS_ZWRITEENABLE,true);

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100,155,255), 1.0f, 0);
		
		//开始绘制
		Device->BeginScene ();

		//渲染地形
		terrain->draw (&TerrainWorld,false);

		//渲染天空盒
		skybox->RenderSkyBox();
	
	
		//渲染地面物体
		groundobject->renderGround ();
		
		//绘制雪花
		snow->render();
		gameui->RenderUI ();
		RECT rect;
		rect.left=120,rect.top=50,rect.right=800,rect.bottom=600;
		gameui->RenderFont(rect,snake->getScore());
		snake->Render();
		float i;
		groundobject->getWallRange(&i);
		if(snake->isDangerous (i))
			gameui2->RenderUI();

		//绘制道具
		props->RenderProps ();

		Device->EndScene ();
		Device->Present(0, 0, 0, 0);//提交后台缓存示翻转）
	}
	else 
	{
		//绘制游戏得分
		RECT rect;
		rect.left=280,rect.top=195,rect.right=800,rect.bottom=600;
		gameui->RenderFont (rect,snake->getScore());

		//绘制时间
		rect.left=280,rect.top=140,rect.right=800,rect.bottom=600;
		gameui->RenderFont (rect,d3d::IntToWchart(gametime));

		//绘制道具得分
		rect.left=280,rect.top=240,rect.right=800,rect.bottom=600;
		int propscore =props->getpropscore();
		gameui->RenderFont(rect ,d3d::IntToWchart(propscore));

		rect.left = 280,rect.top=422,rect.right=800,rect.bottom=600;
		int finnalscore=snake->getscore()+gametime +propscore*10;
		gameui->RenderFont (rect,d3d::IntToWchart(finnalscore));
		
	}
		//关闭深度缓存
		Device->SetRenderState (D3DRS_ZENABLE,false);
		Device->SetRenderState (D3DRS_ZFUNC,D3DCMP_LESS);
		Device->SetRenderState (D3DRS_ZWRITEENABLE,false);
}

void Cleanup()
{
		delete snake;
}

//-----------------------------------【GUICallback( )函数】---------------------------------------
//	描述：GUI系统的回调函数，填写按钮按下后的相关处理代码
//--------------------------------------------------------------------------------------------------
void GUICallback(int id, int state)
{
	switch(id)
	{
	case BUTTON_START_ID:   //start game开始游戏按钮
		if(state == UGP_BUTTON_DOWN)
			{
				g_currentGUI = GUI_START_SCREEN;
			}
		break;

	case BUTTON_LOAD_ID:  //load game载入游戏按钮
		if(state == UGP_BUTTON_DOWN)
			{
				g_currentGUI = GUI_LOAD_SCREEN;
			}	
		break;

	case BUTTON_OPTION_ID: //option设置按钮
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_OPTION_SCREEN;
		break;
	case BUTTON_BACK_ID: //back返回按钮
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_MAIN_SCREEN;
		break;

	case BUTTON_QUIT_ID://quit退出按钮
		if(state == UGP_BUTTON_DOWN)
			PostQuitMessage(0);
		break;
	case BUTTON_LEVEL_1_ID:
		render (timedelta); //start game开始游戏页面中，Level1按钮
		break;
	}
}
