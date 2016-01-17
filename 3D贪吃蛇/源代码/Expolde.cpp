#include"Explode.h"
const DWORD Explode::vertex::FVF =D3DFVF_XYZ|D3DFVF_TEX1;

//构造函数
 Explode::Explode(LPDIRECT3DDEVICE9 _Device)
{
	Device=_Device;
}
 //析构函数
 Explode::~Explode()
 {
	 //释放顶点缓存
	 Vb->Release ();
	 for (int i=0;i<25;i++)
		 Tex[i]->Release ();
 }
//==========================================================================
//初始化函数，包括创建顶点缓存，加载纹理贴图、设置纹理过滤、Alpha融合系数等
//==========================================================================
void Explode::InitExplode()
{
	//创建顶点缓存
	Device->CreateVertexBuffer (
		4*sizeof(vertex),
		0,
		Explode::vertex::FVF,
		D3DPOOL_MANAGED,
		&Vb,
		0
		);
	vertex *BoardVertex=NULL;
	Vb->Lock (0,0,(void **)&BoardVertex,0);
	BoardVertex[0]=vertex(-100.0f,300.0f,0.0f,0.0f,0.0f);//a
	BoardVertex[1]=vertex(100.0f,300.0f,0.0f,1.0f,0.0f);//b
	BoardVertex[2]=vertex(-100.0f,0.0f,0.0f,0.0f,1.0f);//c
	BoardVertex[3]=vertex(100.0f,0.0f,0.0f,1.0f,1.0f);//d
	Vb->Unlock ();
	//从文件中加载25张连续纹理贴图
	D3DXCreateTextureFromFile(Device,L"explode00.dds", &Tex[0]);
	D3DXCreateTextureFromFile(Device,L"explode01.dds", &Tex[1]);
	D3DXCreateTextureFromFile(Device,L"explode02.dds", &Tex[2]);
	D3DXCreateTextureFromFile(Device,L"explode03.dds", &Tex[3]);
	D3DXCreateTextureFromFile(Device,L"explode04.dds", &Tex[4]);
	D3DXCreateTextureFromFile(Device,L"explode05.dds", &Tex[5]);
	D3DXCreateTextureFromFile(Device,L"explode06.dds", &Tex[6]);
	D3DXCreateTextureFromFile(Device,L"explode07.dds", &Tex[7]);
	D3DXCreateTextureFromFile(Device,L"explode08.dds", &Tex[8]);
	D3DXCreateTextureFromFile(Device,L"explode09.dds", &Tex[9]);
	D3DXCreateTextureFromFile(Device,L"explode10.dds", &Tex[10]);
	D3DXCreateTextureFromFile(Device,L"explode11.dds", &Tex[11]);
	D3DXCreateTextureFromFile(Device,L"explode12.dds", &Tex[12]);
	D3DXCreateTextureFromFile(Device,L"explode13.dds", &Tex[13]);
	D3DXCreateTextureFromFile(Device,L"explode14.dds", &Tex[14]);
	D3DXCreateTextureFromFile(Device,L"explode15.dds", &Tex[15]);
	D3DXCreateTextureFromFile(Device,L"explode16.dds", &Tex[16]);
	D3DXCreateTextureFromFile(Device,L"explode17.dds", &Tex[17]);
	D3DXCreateTextureFromFile(Device,L"explode18.dds", &Tex[18]);
	D3DXCreateTextureFromFile(Device,L"explode19.dds", &Tex[19]);
	D3DXCreateTextureFromFile(Device,L"explode20.dds", &Tex[20]);
	D3DXCreateTextureFromFile(Device,L"explode21.dds", &Tex[21]);
	D3DXCreateTextureFromFile(Device,L"explode22.dds", &Tex[22]);
	D3DXCreateTextureFromFile(Device,L"explode23.dds", &Tex[23]);
	D3DXCreateTextureFromFile(Device,L"explode24.dds", &Tex[24]);
	//设置纹理过滤方式
	Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );  
	Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	//	//设置融合系数
	Device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	Device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}



//渲染爆炸效果的函数
bool Explode::RanderExplpde (D3DXMATRIX *View,D3DXMATRIX matNPC,float timeDelta,int *sign)
{
	
	Device->SetRenderState (D3DRS_ALPHABLENDENABLE,true);//开启alpha融合

	D3DXMATRIX matBillboard;  
	D3DXMatrixIdentity(&matBillboard);  
	matBillboard._11 =(*View)._11;  
	matBillboard._13 =(*View)._13;  
	matBillboard._31 =(*View)._31;  
	matBillboard._33 =(*View)._33;

	D3DXMatrixInverse(&matBillboard, NULL, &matBillboard);  

	MatNPC=matNPC; 
	MatNPC =  matBillboard*MatNPC  ;  
	Device->SetTransform(D3DTS_WORLD, &MatNPC ); 

	//开始绘制

	static float timebreak=0;
	timebreak+=timeDelta;
    if( timebreak > 0.05f )
	   {
			timebreak=0;
			(*sign)++;
		    if( *sign > 24 )
                  {
					  /*sign = 0;*/
					  return false;
				  }
	    }

		//设置当前纹理
		Device->SetTexture( 0, Tex[*sign] );

		//渲染图形
		Device->SetStreamSource( 0, Vb, 0, sizeof(vertex) );
		Device->SetFVF( vertex::FVF );
		Device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		return true;

}

