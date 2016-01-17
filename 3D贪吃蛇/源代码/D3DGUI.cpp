#include<d3d9.h>
#include<d3dx9.h>
#include"D3DGUI.h"
//-----------------------------------------------------------------------------
// Desc: 构造函数
//-----------------------------------------------------------------------------
D3DGUIClass::D3DGUIClass(LPDIRECT3DDEVICE9 device, int w, int h)
{
	m_pFonts = NULL;
	m_pControls = NULL;
	m_pVertexBuffer = NULL;

	m_nTotalFontNum = m_nTotalControlNum = m_nTotalBufferNum = 0;
	m_nWindowWidth = m_nWindowHeight = 0;
	m_bIsBackgroundUsed = false;

	m_pd3dDevice = device;
	m_nWindowWidth = w; m_nWindowHeight = h;

	memset(&m_Background, 0, sizeof(GUICONTROL));
}



//-----------------------------------------------------------------------------
// Name： D3DGUIClass::CreateTextFont( )
// Desc:	 字体创建函数
//-----------------------------------------------------------------------------
bool D3DGUIClass::CreateTextFont(wchar_t *fontName, int size, int *fontID)
{
	if(!m_pd3dDevice) return false;

	if(!m_pFonts)
	{
		m_pFonts = new LPD3DXFONT[1];
		if(!m_pFonts) return false;
	}
	else
	{
		LPD3DXFONT *temp;
		temp = new LPD3DXFONT[m_nTotalFontNum + 1];
		if(!temp) return false;

		memcpy(temp, m_pFonts,
			sizeof(LPD3DXFONT) * m_nTotalFontNum);

		delete[] m_pFonts;
		m_pFonts = temp;
	}


	// 调用D3DXCreateTextFont函数创建字体
	D3DXCreateFont(m_pd3dDevice, size, 0, 0, 1000, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		0, fontName,
		&m_pFonts[m_nTotalFontNum]);

	if(!m_pFonts[m_nTotalFontNum]) return false;

	// 存下字体ID和ID计数。
	if(fontID) *fontID = m_nTotalFontNum;
	m_nTotalFontNum++;

	return true;
}


//-----------------------------------------------------------------------------
// Name： D3DGUIClass::AddBackground( )
// Desc:	创建保存背景图的缓存函数
//-----------------------------------------------------------------------------
bool D3DGUIClass::AddBackground(wchar_t *fileName)
{
	if(!fileName) return false;

	m_Background.m_type = UGP_GUI_Background;

	//从文件载入纹理图片 
	if(D3DXCreateTextureFromFile(m_pd3dDevice, fileName, &m_Background.m_Background) != D3D_OK)
		return false;

	// 获取窗口宽度和高度，以便稍后的背景铺满窗口
	float w = (float)m_nWindowWidth;
	float h = (float)m_nWindowHeight;

	GUIVERTEX obj[] =
	{
		{w, 0, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 1.0f, 0.0f},
		{w, h, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 1.0f, 1.0f},
		{0, 0, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 0.0f, 0.0f},
		{0, h, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 0.0f, 1.0f},
	};

	// 创建顶点缓存
	if(FAILED(m_pd3dDevice->CreateVertexBuffer(sizeof(obj), 0, D3DFVF_GUI,
		D3DPOOL_DEFAULT, &m_BackgroundBuffer, NULL))) return false;

	// 填充顶点缓存
	void *ptr;
	if(FAILED(m_BackgroundBuffer->Lock(0, sizeof(obj), (void**)&ptr, 0))) return false;
	memcpy(ptr, obj, sizeof(obj));
	m_BackgroundBuffer->Unlock();

	m_bIsBackgroundUsed = true;

	return true;
}
//-----------------------------------------------------------------------------
// Name： D3DGUIClass::AddStaticText( )
// Desc:	创建静态文本控件
//-----------------------------------------------------------------------------
bool D3DGUIClass::AddStaticText(int id, wchar_t *text, float x, float y, unsigned long color, int fontID)
{
	if(!text || fontID < 0 || fontID >= m_nTotalFontNum) return false;

	//下面的代码用于创建一个控件
	if(!m_pControls)
	{
		m_pControls = new GUICONTROL[1];
		if(!m_pControls) return false;
		memset(&m_pControls[0], 0, sizeof(GUICONTROL));
	}
	else
	{
		GUICONTROL *temp;
		temp = new GUICONTROL[m_nTotalControlNum + 1];
		if(!temp) return false;
		memset(temp, 0, sizeof(GUICONTROL) *
			(m_nTotalControlNum + 1));

		memcpy(temp, m_pControls,
			sizeof(GUICONTROL) * m_nTotalControlNum);

		delete[] m_pControls;
		m_pControls = temp;
	}

	// 填充我们需要的静态文本信息
	m_pControls[m_nTotalControlNum].m_type = UGP_GUI_STATICTEXT;
	m_pControls[m_nTotalControlNum].m_id = id;
	m_pControls[m_nTotalControlNum].m_color = color;
	m_pControls[m_nTotalControlNum].m_xPos = x;
	m_pControls[m_nTotalControlNum].m_yPos = y;
	m_pControls[m_nTotalControlNum].m_listID = fontID;

	// 复制文本数据 
	int len = wcslen(text);
	m_pControls[m_nTotalControlNum].m_text = new wchar_t[len+1];
	if(!m_pControls[m_nTotalControlNum].m_text) return false;
	wcscpy(m_pControls[m_nTotalControlNum].m_text, text);
	//m_pControls[m_nTotalControlNum].m_text[len] = '\0';

	//增量总数的计算
	m_nTotalControlNum++;

	return true;
}


//-----------------------------------------------------------------------------
// Name： D3DGUIClass::AddButton( )
// Desc:	创建按钮控件的函数
//-----------------------------------------------------------------------------
bool D3DGUIClass::AddButton(int id, float x, float y, wchar_t *up, wchar_t *over, wchar_t *down)
{
	if(!up || !over || !down) return false;

	if(!m_pControls)
	{
		m_pControls = new GUICONTROL[1];
		if(!m_pControls) return false;
		memset(&m_pControls[0], 0, sizeof(GUICONTROL));
	}
	else
	{
		GUICONTROL *temp;
		temp = new GUICONTROL[m_nTotalControlNum + 1];
		if(!temp) return false;
		memset(temp, 0, sizeof(GUICONTROL) *
			(m_nTotalControlNum + 1));

		memcpy(temp, m_pControls,
			sizeof(GUICONTROL) * m_nTotalControlNum);

		delete[] m_pControls;
		m_pControls = temp;
	}

	// Set all the data needed to render/process a button.
	m_pControls[m_nTotalControlNum].m_type = UGP_GUI_BUTTON;
	m_pControls[m_nTotalControlNum].m_id = id;
	m_pControls[m_nTotalControlNum].m_xPos = x;
	m_pControls[m_nTotalControlNum].m_yPos = y;
	m_pControls[m_nTotalControlNum].m_listID = m_nTotalBufferNum;

	// 从文件加载纹理
	if(D3DXCreateTextureFromFile(m_pd3dDevice, up, &m_pControls[m_nTotalControlNum].m_upTex) != D3D_OK)
	{
		return false;
	}

	if(D3DXCreateTextureFromFile(m_pd3dDevice, over, &m_pControls[m_nTotalControlNum].m_overTex) != D3D_OK)
	{
		return false;
	}

	if(D3DXCreateTextureFromFile(m_pd3dDevice, down, &m_pControls[m_nTotalControlNum].m_downTex) != D3D_OK)
	{
		return false;
	}

	unsigned long white = D3DCOLOR_XRGB(255,255,255);

	//获取一下图形的宽度和高度 
	D3DSURFACE_DESC desc;
	m_pControls[m_nTotalControlNum].m_upTex->GetLevelDesc(0, &desc);

	float w = (float)desc.Width;
	float h = (float)desc.Height;

	m_pControls[m_nTotalControlNum].m_width = w;
	m_pControls[m_nTotalControlNum].m_height = h;

	GUIVERTEX obj[] =
	{
		{w + x, 0 + y, 0.0f, 1, white, 1.0f, 0.0f},
		{w + x, h + y, 0.0f, 1, white, 1.0f, 1.0f},
		{0 + x, 0 + y, 0.0f, 1, white, 0.0f, 0.0f},
		{0 + x, h + y, 0.0f, 1, white, 0.0f, 1.0f},
	};

	//创建顶点缓存
	if(!m_pVertexBuffer)
	{
		m_pVertexBuffer = new LPDIRECT3DVERTEXBUFFER9[1];
		if(!m_pVertexBuffer) return false;
	}
	else
	{
		LPDIRECT3DVERTEXBUFFER9 *temp;
		temp = new LPDIRECT3DVERTEXBUFFER9[m_nTotalBufferNum + 1];
		if(!temp) return false;

		memcpy(temp, m_pVertexBuffer,sizeof(LPDIRECT3DVERTEXBUFFER9) * m_nTotalBufferNum);

		delete[] m_pVertexBuffer;
		m_pVertexBuffer = temp;
	}

	if(FAILED(m_pd3dDevice->CreateVertexBuffer(sizeof(obj), 0,
		D3DFVF_GUI, D3DPOOL_DEFAULT, &m_pVertexBuffer[m_nTotalBufferNum], NULL)))
	{
		return false;
	}

	//填充顶点缓存
	void *ptr;

	if(FAILED(m_pVertexBuffer[m_nTotalBufferNum]->Lock(0, sizeof(obj), (void**)&ptr, 0)))
	{
		return false;
	}

	memcpy(ptr, obj, sizeof(obj));
	m_pVertexBuffer[m_nTotalBufferNum]->Unlock();

	// 自增
	m_nTotalBufferNum++;

	// 自增
	m_nTotalControlNum++;

	return true;
}

//-----------------------------------------------------------------------------
// Name： D3DGUIClass::ClearUp( )
// Desc:	清理门户释放资源的函数
//-----------------------------------------------------------------------------
void D3DGUIClass::ClearUp()
{
	//清理门户，释放所有的资源
	if(m_bIsBackgroundUsed)
	{
		if(m_Background.m_Background)
			m_Background.m_Background->Release();

		if(m_BackgroundBuffer)
			m_BackgroundBuffer->Release();
	}

	m_Background.m_Background = NULL;
	m_BackgroundBuffer = NULL;

	for(int i = 0; i < m_nTotalFontNum; i++)
	{
		if(m_pFonts[i])
		{
			m_pFonts[i]->Release();
			m_pFonts[i] = NULL;
		}
	}

	if(m_pFonts) delete[] m_pFonts;
	m_pFonts = NULL;
	m_nTotalFontNum = 0;

	for(int i = 0; i < m_nTotalBufferNum; i++)
	{
		if(m_pVertexBuffer[i])
		{
			m_pVertexBuffer[i]->Release();
			m_pVertexBuffer[i] = NULL;
		}
	}

	if(m_pVertexBuffer) delete[] m_pVertexBuffer;
	m_pVertexBuffer = NULL;
	m_nTotalBufferNum = 0;

	for(int i = 0; i < m_nTotalControlNum; i++)
	{
		if(m_pControls[i].m_Background)
		{
			m_pControls[i].m_Background->Release();
			m_pControls[i].m_Background = NULL;
		}

		if(m_pControls[i].m_upTex)
		{
			m_pControls[i].m_upTex->Release();
			m_pControls[i].m_upTex = NULL;
		}

		if(m_pControls[i].m_downTex)
		{
			m_pControls[i].m_downTex->Release();
			m_pControls[i].m_downTex = NULL;
		}

		if(m_pControls[i].m_overTex)
		{
			m_pControls[i].m_overTex->Release();
			m_pControls[i].m_overTex = NULL;
		}

		if(m_pControls[i].m_text)
		{
			delete[] m_pControls[i].m_text;
			m_pControls[i].m_text = NULL;
		}
	}

	if(m_pControls) delete[] m_pControls;
	m_pControls = NULL;
	m_nTotalControlNum = 0;
}


//-----------------------------------------------------------------------------
// Name：ProcessGUI
// Desc: 全局的函数，封装渲染整个GUI系统，同样还为控件调用回调函数
//-----------------------------------------------------------------------------
void ProcessGUI(D3DGUIClass *gui, bool LMBDown, int mouseX, int mouseY, void(*funcPtr)(int id, int state))
{
	if(!gui) return;

	LPDIRECT3DDEVICE9 device = gui->GetD3dDevice();
	if(!device) return;

	// 绘制背景
	GUICONTROL *Background = gui->GetBackground();
	LPDIRECT3DVERTEXBUFFER9 bdBuffer = gui->GetBackgroundBuffer();

	//已经创建出的东西才绘制，所以来个if
	if(gui->IsBackgroundUsed() && Background && bdBuffer)
	{
		device->SetTexture(0, Background->m_Background);
		device->SetStreamSource(0, bdBuffer, 0, sizeof(GUIVERTEX));
		device->SetFVF(D3DFVF_GUI);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		device->SetTexture(0, NULL);
	}

	//用来显示文本的对象
	LPD3DXFONT pFont = NULL;
	RECT fontPosition = {0, 0, (long)gui->GetWindowWidth(),
		(long)gui->GetWindowHeight()};

	// 创建一个顶点缓存对象用于按钮的渲染
	LPDIRECT3DVERTEXBUFFER9 pBuffer = NULL;
	int status = UGP_BUTTON_UP;

	// 一个循环，用于各种控件的渲染
	for(int i = 0; i < gui->GetTotalControlNum(); i++)
	{
		// 获取当前控件
		GUICONTROL *pControl = gui->GetGUIControl(i);
		if(!pControl) continue;

		// 根据不同的类型做不同的操作
		switch(pControl->m_type)
		{
		case UGP_GUI_STATICTEXT:
			// 这种情况下获取字体对象
			pFont = gui->GetFont(pControl->m_listID);
			if(!pFont) continue;

			// 设置文字位置
			fontPosition.left = pControl->m_xPos;
			fontPosition.top = pControl->m_yPos;

			// 显示文字
			pFont->DrawText(NULL, pControl->m_text, -1, &fontPosition,
				DT_LEFT, pControl->m_color);
			break;

		case UGP_GUI_BUTTON:
			status = UGP_BUTTON_UP;

			//获取按钮所对应的顶点缓存对象
			pBuffer = gui->GetVertexBuffer(pControl->m_listID);
			if(!pBuffer) continue;

			// 设置纹理的alpha透明选项
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			//检查鼠标是否悬停或者点击了按钮
			if(mouseX > pControl->m_xPos && mouseX < pControl->m_xPos + pControl->m_width &&
				mouseY > pControl->m_yPos && mouseY < pControl->m_yPos + pControl->m_height)
			{
				if(LMBDown) status = UGP_BUTTON_DOWN;
				else status = UGP_BUTTON_OVER;
			}

			//根据不同的鼠标和按钮之间缠绵悱恻的状态来准备不同的纹理图 
			if(status == UGP_BUTTON_UP) device->SetTexture(0, pControl->m_upTex);
			if(status == UGP_BUTTON_OVER) device->SetTexture(0, pControl->m_overTex);
			if(status == UGP_BUTTON_DOWN) device->SetTexture(0, pControl->m_downTex);

			// 万事俱备，开始渲染按钮
			device->SetStreamSource(0, pBuffer, 0, sizeof(GUIVERTEX));
			device->SetFVF(D3DFVF_GUI);
			device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			// 关闭alpha混合.
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			break;
		}

		//调用回调函数处理控件消息
		if(funcPtr) funcPtr(pControl->m_id, status);
	}

}