#pragma once
#include"d3dUtility.h"
class XFileMesh{
public:
	XFileMesh(LPDIRECT3DDEVICE9 _Device);
	XFileMesh();
	~XFileMesh();

	private:

	LPDIRECT3DDEVICE9 Device;
	LPD3DXBUFFER  ppAdjacency;
	LPD3DXBUFFER  ppMaterials;
	DWORD pNumMaterials;
	LPD3DXMESH SourceMesh;
	LPD3DXPMESH PMesh;//进行渐进网格
	D3DMATERIAL9*       g_pMaterials   ; // 网格的材质信息  
	LPDIRECT3DTEXTURE9* g_pTextures    ; // 网格的纹理信息 

	public:
		void InitXfile(LPCTSTR pFilename);//初始化网格函数
		void RenderXfile();	//渲染函数
		
};