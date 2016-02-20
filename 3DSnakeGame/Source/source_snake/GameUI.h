//
//
//这个类主要是用来绘制游戏进行过程中的界面的HUD 
//
//

#pragma once
#include"d3dUtility.h"
#include<list>

struct SCORE
{
	SCORE(){}
	SCORE(float _x,float _y,float _z,float _rhw,unsigned long _color,float _tu,float _tv)
	{
		x=_x,y=_y,z=_z,rhw=_rhw,color=_color,tu=_tu,tv=_tv;
	}
	float x, y, z, rhw;
	unsigned long color;
	float tu, tv;
};

class  GameUI
{
	public:
		GameUI(LPDIRECT3DDEVICE9 _Device){Device=_Device;}//构造函数
		~GameUI();//析构函数
	private :
		LPDIRECT3DDEVICE9 Device;//D3D设备指针
		LPDIRECT3DVERTEXBUFFER9 Vb;//顶点缓存
		LPDIRECT3DTEXTURE9 Tex[1];//纹理接口
		ID3DXFont *pFont ; //字体接口指针
		float pictureScal; //图片的放大倍数
	public :
		bool initPicture(wchar_t *FileName ,float x , float y,float pictureScal );//初始化函数
		bool initTex(wchar_t *Fontz);
		void RenderFont(RECT rect ,wchar_t *content);
		void RenderUI();//渲染函数
};