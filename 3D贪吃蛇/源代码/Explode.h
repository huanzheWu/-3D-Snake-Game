//==========================================================================
//广告版类
//有两个成员函数：InitBoard(wchar_t *filename)和RanderBorad(D3DXMATRIX *view,D3DXMATRIX matNPC)
//使用RanderBoard时应该传入有摄像机类计算得出的取景变换矩阵以及一个矩阵，指定广告板的位置
//==========================================================================
//数学方面：关于旋转矩阵为什么要取逆:
//由摄像机的观察向量和广告版平面的法向量可知
//这两个向量的旋转方向是相反的
//==========================================================================
//关于一开始广告版人物背景无法消除的问题：
//需要进行融合的物体应该先绘制出来，由融合公式只，DestPixel是当前处于后台缓存
//中的像素颜色值
//==========================================================================
//2014.8.6
//版本1.0
//===========================================================================

//============================================================================
//版本2.0  将公告板技术应用于爆炸效果
//=============================================================================
#pragma once
#include"d3dUtility.h"
#include<list>
class Explode
{
	private :
		LPDIRECT3DVERTEXBUFFER9 Vb;//顶点缓存
		LPDIRECT3DTEXTURE9 Tex[25];//纹理接口（24个）
		LPDIRECT3DDEVICE9 Device;//D3D设备
		D3DXMATRIX MatNPC; //产生爆炸的位置
		//爆炸图顶点结构
		struct vertex
		{
			vertex(){}
			vertex(float x,float y,float z,float u,float v)
			{
				_x=x;_y=y,_z=z;
				_u=u;_v=v;
			}
			float _x,_y, _z;
			float _u,_v;
			static const DWORD FVF;
		};
	public:
		Explode(LPDIRECT3DDEVICE9 dvice);
		~Explode();
		void InitExplode();
		bool RanderExplpde(D3DXMATRIX *view,D3DXMATRIX matNPC,float timeDelta,int* sign);

};