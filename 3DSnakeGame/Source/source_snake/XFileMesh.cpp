#include"XFileMesh.h"
#include<d3dx9.h>
//构造函数
XFileMesh::XFileMesh(LPDIRECT3DDEVICE9 _Device)
{
	Device=_Device;

}
//初始化函数
void XFileMesh::InitXfile(LPCTSTR pFilename)
{
	/*三步曲之一：通过.X文件加载网格模型*/
	D3DXLoadMeshFromX(
		pFilename,
		D3DXMESH_MANAGED,
		Device,
		&ppAdjacency,
		&ppMaterials,
		NULL,
		&pNumMaterials,//返回网格中的材质数目
		&SourceMesh//返回所创建的并已填充好XFile几何数据的ID3DXMesh对象
		);

	//检测载入的XFile中是否含有顶点法线信息。
	if(!SourceMesh->GetFVF ()&D3DFVF_NORMAL)
	{
		ID3DXMesh *temp=0;
		SourceMesh->CloneMeshFVF(
			D3DXMESH_MANAGED,
			SourceMesh->GetFVF ()|D3DFVF_NORMAL,
			Device,
			&temp);
		SourceMesh->Release ();
		SourceMesh=temp;
	}

	//三步曲之二：载入材质和纹理
	D3DXMATERIAL *pMtrls=(D3DXMATERIAL *)  ppMaterials->GetBufferPointer(); 

	g_pMaterials= new D3DMATERIAL9[ pNumMaterials];  
    g_pTextures  = new LPDIRECT3DTEXTURE9[ pNumMaterials];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         

	//获取纹理和材质数据
	for(DWORD i=0; i<pNumMaterials; i++)  
      {  
           //获取材质，并设置一下环境光的颜色值  
           g_pMaterials[i]= pMtrls[i].MatD3D;  
           g_pMaterials[i].Ambient= g_pMaterials[i].Diffuse;  
           //创建一下纹理对象  
           g_pTextures[i]  = NULL;  
           D3DXCreateTextureFromFileA(Device,pMtrls[i].pTextureFilename, &g_pTextures[i]);  
      }  

	//进行网格优化
		SourceMesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT |D3DXMESHOPT_VERTEXCACHE|D3DXMESHOPT_ATTRSORT ,
		(DWORD *)ppAdjacency->GetBufferPointer (),
		(DWORD *)ppAdjacency->GetBufferPointer (),
		NULL,
		NULL
		);
	ppAdjacency->Release ();
    ppMaterials->Release ();
}

//渲染函数
void XFileMesh::RenderXfile()
{
	 for(DWORD i = 0; i < pNumMaterials; i++)  
      {  
			Device->SetMaterial(&g_pMaterials[i]);  
		///*	Device->SetTexture(0,g_pTextures[i]);  */
			SourceMesh->DrawSubset(i);  
      }  
}
//析构函数
XFileMesh::~XFileMesh()
{
	SourceMesh->Release ();
}
