#pragma once


// 所支持的控件类型宏
#define UGP_GUI_STATICTEXT		1
#define UGP_GUI_BUTTON			2
#define UGP_GUI_Background		3

// 鼠标按键状态宏
#define UGP_BUTTON_UP			1
#define UGP_BUTTON_OVER		2
#define UGP_BUTTON_DOWN		3


// 菜单页面的宏定义
#define GUI_MAIN_SCREEN       1
#define GUI_START_SCREEN      2
#define GUI_LOAD_SCREEN			3
#define GUI_OPTION_SCREEN    4

// 设置一些GUI中用到的控件ID
#define STATIC_TEXT_ID     1
#define BUTTON_START_ID    2
#define BUTTON_LOAD_ID  3
#define BUTTON_OPTION_ID  4
#define BUTTON_QUIT_ID     5
#define BUTTON_BACK_ID     6
#define BUTTON_LEVEL_1_ID  7





// FVF灵活顶点类型的结构体
struct GUIVERTEX
{
	float x, y, z, rhw;
	unsigned long color;
	float tu, tv;
};
#define D3DFVF_GUI (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)


//控件属性结构体
struct GUICONTROL
{
	//操作类型，ID和颜色
	int m_type;  //控件类型
	int m_id;  //控件ID
	unsigned long m_color; //控件颜色
	int m_listID;   //如果是文字的话，这个变量就表示它使用的字体，否则就表示顶点缓存
	float m_xPos, m_yPos;   //控件的起始位置
	float m_width, m_height;   // 控件的宽度和高度
	wchar_t *m_text;   // 文字内容
	LPDIRECT3DTEXTURE9 m_Background;   // 控件背景的填充图像
	LPDIRECT3DTEXTURE9 m_upTex, m_downTex, m_overTex;   // 存放按钮弹起，按下和鼠标经过时的3张纹理图
};



class D3DGUIClass
{

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;  //D3D设备对象	
	LPD3DXFONT *m_pFonts;  //D3D字体对象
	GUICONTROL *m_pControls;  //控件对象
	LPDIRECT3DVERTEXBUFFER9 *m_pVertexBuffer;   //顶点缓存对象指针
	GUICONTROL m_Background;  //背景图对象
	LPDIRECT3DVERTEXBUFFER9 m_BackgroundBuffer;   //背景图缓冲区对象

	bool m_bIsBackgroundUsed;  //一个标识，用于标识是否已经用了背景
	int m_nTotalFontNum;		//字体数目计数器
	int m_nTotalControlNum;	//控件数目计数器
	int m_nTotalBufferNum;	//缓冲区数目计数器

	int m_nWindowWidth;	//窗口宽度
	int m_nWindowHeight;	//窗口高度



public:
	D3DGUIClass(LPDIRECT3DDEVICE9 device, int w, int h);
	~D3DGUIClass() { ClearUp(); }

	LPDIRECT3DDEVICE9 GetD3dDevice() { return m_pd3dDevice; }  //返回D3D设备对象的函数
	GUICONTROL *GetBackground() { return &m_Background; } //返回背景的函数
	LPDIRECT3DVERTEXBUFFER9 GetBackgroundBuffer() { return m_BackgroundBuffer; }  //返回背景缓冲区对象的函数


	int GetTotalFontNum() { return m_nTotalFontNum; }  //返回所有字体数目的函数
	int GetTotalControlNum() { return m_nTotalControlNum; }  //返回所有控件数目的函数
	int GetTotalBufferNum() { return m_nTotalBufferNum; } //返回总的缓冲区数目的函数
	int GetWindowWidth() { return m_nWindowWidth; }  //返回窗口宽度的函数
	int GetWindowHeight() { return m_nWindowHeight; }  //返回窗口高度的函数
	bool IsBackgroundUsed() { return m_bIsBackgroundUsed; }     //返回背景是否在使用的bool值的函数
	void SetWindowSize(int w, int h) { m_nWindowWidth = w; m_nWindowHeight = h; }  //设置窗口宽度和高度的函数

	LPD3DXFONT GetFont(int id)  //返回字体ID函数
	{
		if(id < 0 || id >= m_nTotalFontNum) return NULL;
		return m_pFonts[id];
	}

	GUICONTROL *GetGUIControl(int id)  //返回GUI控件ID函数
	{
		if(id < 0 || id >= m_nTotalControlNum) return NULL;
		return &m_pControls[id];
	}

	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer(int id) //返回顶点缓存ID函数
	{
		if(id < 0 || id >= m_nTotalBufferNum) return NULL;
		return m_pVertexBuffer[id];
	}


	bool CreateTextFont(wchar_t *fontName, int size, int *fontID);  //字体创建函数
	bool AddBackground(wchar_t *fileName);  //GUI背景添加函数
	bool AddStaticText(int id, wchar_t *text, float x, float y, unsigned long color, int fontID); //添加静态文本函数
	bool AddButton(int id, float x, float y, wchar_t *up, wchar_t *over, wchar_t *down); //添加按钮函数
	void ClearUp( ); //资源清理函数


};

void ProcessGUI(D3DGUIClass *gui, bool LMBDown, int mouseX, int mouseY,
	void(*funcPtr)(int id, int state));  //回调函数

