#include"d3dUtility.h"
#include "DirectInputClass.h"
//-----------------------------------------------------------------------------
// 功能: 构造函数
//-----------------------------------------------------------------------------
DInputClass::DInputClass()
{
	m_pDirectInput = NULL;
	m_KeyboardDevice = NULL;
	ZeroMemory(m_keyBuffer,sizeof(char)*256);
	m_MouseDevice= NULL;
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
}

//-----------------------------------------------------------------------------
// 函数名：DInputClass::Init(）
// 功能: 初始化DirectInput键盘及鼠标输入设备
//-----------------------------------------------------------------------------
HRESULT DInputClass::Init( HWND hWnd,HINSTANCE hInstance,DWORD keyboardCoopFlags, DWORD mouseCoopFlags )
{
	//初始化一个IDirectInput8接口对象
	DirectInput8Create( hInstance, DIRECTINPUT_VERSION, 
		IID_IDirectInput8,(void**)&m_pDirectInput,NULL );

	//进行键盘设备的初始化
	 m_pDirectInput->CreateDevice( GUID_SysKeyboard, &m_KeyboardDevice, NULL ); //创建设备
	 m_KeyboardDevice->SetCooperativeLevel( hWnd, keyboardCoopFlags);//设置设备的协作级别
	 m_KeyboardDevice->SetDataFormat( &c_dfDIKeyboard);//设置数据格式
	 m_KeyboardDevice->Acquire( );//获取权限
	 m_KeyboardDevice->Poll( );//获取轮询

	//进行鼠标设备的初始化
	 m_pDirectInput->CreateDevice( GUID_SysMouse, &m_MouseDevice, NULL );
	 m_MouseDevice->SetCooperativeLevel( hWnd,mouseCoopFlags);
	 m_MouseDevice->SetDataFormat( &c_dfDIMouse );
	 m_MouseDevice->Acquire( );
	 m_KeyboardDevice->Poll( );

	return S_OK;
}

//-----------------------------------------------------------------------------
// 函数名：DInputClass::GetInput()
// 功能: 用于获取输入信息的函数
//-----------------------------------------------------------------------------
void DInputClass::GetInput()
{
	HRESULT hr = m_KeyboardDevice->GetDeviceState(sizeof(m_keyBuffer), (void*)&m_keyBuffer); 

	//获取键盘输入消息
	if(hr)
	{
		m_KeyboardDevice->Acquire();  
		m_KeyboardDevice->GetDeviceState( sizeof(m_keyBuffer),(LPVOID)m_keyBuffer );
	}

	hr = m_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&m_MouseState); 
	//获取鼠标输入消息
	if(hr)
	{
		m_MouseDevice->Acquire();
		m_MouseDevice->GetDeviceState( sizeof(DIMOUSESTATE), (void*)&m_MouseState);
	}
}

//-----------------------------------------------------------------------------
// 函数名：DInputClass::IsKeyDown()
// 功能: 判断键盘上某个键是否按下
//-----------------------------------------------------------------------------
bool DInputClass::IsKeyDown(int iKey)
{
	if(m_keyBuffer[iKey] & 0x80)
		return true;
	else
		return false;
}


//-----------------------------------------------------------------------------
// 函数名：DInputClass::IsMouseButtonDown()
// 功能: 判断鼠标上某键是否按下
//-----------------------------------------------------------------------------
bool DInputClass::IsMouseButtonDown(int button)
{
	return (m_MouseState.rgbButtons[button] & 0x80) != 0;
}

//-----------------------------------------------------------------------------
// 函数名：DInputClass::MouseDX
// 功能: 返回鼠标指针的X轴坐标值
//-----------------------------------------------------------------------------
float DInputClass::MouseDX()
{
	return (float)m_MouseState.lX;
}

//-----------------------------------------------------------------------------
// 函数名：DInputClass::MouseDY
// 功能: 返回鼠标指针的Y轴坐标值
//-----------------------------------------------------------------------------
float DInputClass::MouseDY()
{
	return (float)m_MouseState.lY;
}

//-----------------------------------------------------------------------------
// 函数名：DInputClass::MouseDZ
// 功能: 返回鼠标指针的Z轴坐标值（滚轮）
//-----------------------------------------------------------------------------
float DInputClass::MouseDZ()
{
	return (float)m_MouseState.lZ;
}




//-----------------------------------------------------------------------------
// 功能: 析构函数
//-----------------------------------------------------------------------------
DInputClass::~DInputClass(void)
{
	if(m_KeyboardDevice != NULL)
		m_KeyboardDevice->Unacquire();
	if(m_MouseDevice != NULL)
		m_MouseDevice->Unacquire();
	m_KeyboardDevice->Release ();
	m_MouseDevice->Release ();
	m_pDirectInput->Release ();
}
