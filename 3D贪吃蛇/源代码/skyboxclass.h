#pragma once
#include"d3dUtility.h"
//#pragma comment(lib, "strmiids.lib")
//#pragma comment(lib,"dxguid.lib")
struct SKYBOXVERTEX
{
	float	x,y,z;
	float	u,v;
};
#define SkyBoxFVF (D3DFVF_XYZ|D3DFVF_TEX1)

class SkyBox{
	private:
		float m_Length;
		LPDIRECT3DDEVICE9				Device; //D3D设备对象
		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer ;  //顶点缓存
		LPDIRECT3DTEXTURE9			Texture[6];	//6个纹理接口对象
	public:
		SkyBox( LPDIRECT3DDEVICE9  pDevice );      //构造函数  
		virtual ~SkyBox(void);    //析构函数
	public:
		void LoadSkyTextureFromFile(wchar_t *pFrontTextureFile, wchar_t *pBackTextureFile,wchar_t *pLeftTextureFile, wchar_t *pRightTextureFile,wchar_t *pTopTextureFile,wchar_t *pGroundTextureFile);//从文件中载入天空盒贴图
		void InitSkyBox(float length);//初始化天空盒
		void RenderSkyBox();//渲染天空盒
};