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
//==========================================================================
#pragma once
#include"d3dUtility.h"
class BillBoard
{
	private :
		LPDIRECT3DVERTEXBUFFER9 Vb;
		LPDIRECT3DTEXTURE9 Tex;
		LPDIRECT3DDEVICE9 Device;
		D3DXMATRIX MatNPC;
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

		BillBoard(LPDIRECT3DDEVICE9 dvice);
		~BillBoard();
		void InitBoard(wchar_t *filename);
		void RanderBorad(D3DXMATRIX *view,D3DXMATRIX matNPC);
};