#include "d3dUtility.h"

#define  MAX_RANGE_LIGHT  10000.0f; 


bool d3d::InitD3D(
	HINSTANCE hInstance,
	int width,
	int height,
	bool windowed,
	D3DDEVTYPE deviceType,
	IDirect3DDevice9** device,
	HWND* hWnd)
{


	//******************************************************************
	//
	// Create the main application window.
	//创建一个窗口
	//******************************************************************
	WNDCLASS wc;//描述一个窗口
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)d3d::WndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon		 =(HICON)::LoadImage(NULL,L"iconi.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE); 
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"3D贪吃蛇";

	if( !RegisterClass(&wc) )  //注册一个窗口实例
	{
		::MessageBox(0, L"RegisterClass() - FAILED", 0, 0);
		return false;
	}
		
	HWND hwnd = 0;

	//创建一个窗口
	hwnd = ::CreateWindow(
		L"3D贪吃蛇",
		L"3D贪吃蛇",  
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width, 
		height,
		0 /*parent hwnd*/,
		0 /* menu */, 
		hInstance,
		0 /*extra*/); 

	if( !hwnd )
	{
		::MessageBox(0, L"CreateWindow() - FAILED", 0, 0);
		return false;
	}

	*hWnd=hwnd;

	::ShowWindow(hwnd, SW_SHOW);  //显示窗口
	::UpdateWindow(hwnd);		//更新窗口



	//**************************************************************
	// Init D3D: 
	//初始化D3D
	//**************************************************************
	HRESULT hr = 0;

	// Step 1: Create the IDirect3D9 object.获取接口IDirect3D9指针

	IDirect3D9* d3d9 = 0;//
    d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

    if( !d3d9 )
	{
		::MessageBox(0, L"Direct3DCreate9() - FAILED", 0, 0);
		return false;
	}
	// Step 2: Check for hardware vp.

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.
 
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = windowed;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 4: Create the device.

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		deviceType,         // device type
		hwnd,               // window associated with device
		vp,                 // vertex processing
	    &d3dpp,             // present parameters
	    device);            // return created device

	if( FAILED(hr) )
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		
		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			hwnd,
			vp,
			&d3dpp,
			device);

		if( FAILED(hr) )
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, L"CreateDevice() - FAILED", 0, 0);
			return false;
		}
	}

	d3d9->Release(); // done with d3d9 object
	
	return true;
}

//****************************************************
//两个随机函数
//****************************************************
float d3d::GetRandomFloat(float lowBound,float highBound)
{
	if(lowBound>highBound)
		return lowBound;
	float f=(rand()%10000)*0.0001f;
	return (f*(highBound-lowBound))+lowBound;
}
void d3d::GetRandomVector(D3DXVECTOR3* out,D3DXVECTOR3 * min,D3DXVECTOR3 * max)
{
	out->x =GetRandomFloat(min->x ,max->x);
	out->y =GetRandomFloat(min->y ,max->y);
	out->z =GetRandomFloat(min->z,max->z );
}

//**********************************************
//消息循环
//**********************************************
int d3d::EnterMsgLoop( bool (*ptr_display)(float timeDelta) )
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));
	static float lastTime = (float)timeGetTime();  //tiemGetTime()函数返回系统时间
	while(msg.message != WM_QUIT)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))//PM_REMOVE 从消息队列中删除消息
		{
			::TranslateMessage(&msg);//把虚拟按键转化为字符信息并发送至消息队列。
			::DispatchMessage(&msg);//发送一条信息到窗口过程
		}
		else
        {	
			float currTime  = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;
			ptr_display(timeDelta); //调用渲染函数
			lastTime = currTime;
        }
    }
    return msg.wParam;
}

D3DLIGHT9 d3d::InitDirectionalight ( D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 Light;
	::ZeroMemory(&Light,sizeof(Light));
	Light.Type=D3DLIGHT_DIRECTIONAL;
	Light.Direction=*direction;
	Light.Ambient=*color*0.4f;
	Light.Diffuse=*color;
	Light.Specular=*color * 0.6f;
	return Light;
}

D3DLIGHT9 d3d::InitPointlight(D3DXVECTOR3 *Position,D3DXCOLOR *Color)
{
	D3DLIGHT9 Light;
	::ZeroMemory(&Light,sizeof(Light));
	Light.Ambient=*Color*0.4f;
	Light.Diffuse=*Color;
	Light.Specular=*Color*0.6f;
	Light.Falloff=1.0f;
	Light.Range =MAX_RANGE_LIGHT;
	Light.Position=*Position;
	Light.Attenuation0 = 1.0f;
	Light.Attenuation1 = 0.0f;
	Light.Attenuation2 = 0.0f;
	Light.Type=D3DLIGHT_POINT;
	return Light;

}
D3DLIGHT9 d3d::InitSpotlight(D3DXVECTOR3 *position,D3DXVECTOR3 *direction,D3DXCOLOR *color)
{
	D3DLIGHT9 Light;
	::ZeroMemory(&Light,sizeof(Light));
	Light.Ambient=*color*0.4f;
	Light.Specular  = *color * 0.6f;
	Light.Diffuse=*color;
	Light.Direction=*direction;
	Light.Range=MAX_RANGE_LIGHT;
	Light.Position=*position;
	Light.Falloff=1.0f;
	Light.Type=D3DLIGHT_SPOT;
	Light.Attenuation0 = 1.0f;
	Light.Attenuation1 = 0.0f;
	Light.Attenuation2 = 0.0f;
	Light.Theta        = 0.4f;
	Light.Phi          = 0.9f;
	return Light;
}
D3DMATERIAL9 d3d::IintMetirial(D3DXCOLOR *a,D3DXCOLOR *b,D3DXCOLOR *c,D3DXCOLOR *d,float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient=*a;
	mtrl.Diffuse=*b;
	mtrl.Emissive=*c;
	mtrl.Power=p;
	mtrl.Specular=*d;
	return mtrl;
}

d3d::BoundingBox::BoundingBox()
{
	_min.x = d3d::INFINITY;
	_min.y = d3d::INFINITY;
	_min.z = d3d::INFINITY;

	_max.x = -d3d::INFINITY;
	_max.y = -d3d::INFINITY;
	_max.z = -d3d::INFINITY;
}

DWORD d3d::FtoDw(float f)
{
	return *(( DWORD * ) &f ) ;
}

//==================================================================
//将int转换为wchar_t的函数
//==================================================================
wchar_t*  d3d:: IntToWchart(int willchance)
{
	wchar_t * a;
	char b[10]; 
	int i,j=1;
	if(willchance==0)
			b[0]=48;
	else
		{
			for(i=1;willchance/i!=0;i*=10);
			for(j=0,i/=10;i!=0;j++)
			{
				b[j]=willchance/i+48;
				willchance=willchance-(willchance/i)*i;
				i/=10;			
			}
		}
	b[j]='\0';
	DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, b, -1, NULL, 0);
	a = new wchar_t[dwNum]; 
	MultiByteToWideChar (CP_ACP, 0, b,(strlen(b) + 1), a, dwNum);
	return a;
}


bool d3d::BoundingBox::isPointInside(D3DXVECTOR3& p)
{
	if( p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
		p.x <= _max.x && p.y <= _max.y && p.z <= _max.z )
	{
		return true;
	}
	else
	{
		return false;
	}
}