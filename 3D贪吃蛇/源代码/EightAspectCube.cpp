#include"d3dUtility.h"
IDirect3DVertexBuffer9 *vb;  //顶点缓存
IDirect3DIndexBuffer9 *ib;	//索引缓存
IDirect3DTexture9 *texture[8];//纹理接口
struct MyVertex
{
	MyVertex(){}
	MyVertex(D3DXVECTOR3 _position,D3DXVECTOR3 NORMAL,float n,float v)
	{		
		position=_position;
		normal=NORMAL;
		_n=n,_v=v;	
		
	}
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	float _n,_v;
};

#define FVF_MYVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)  //定义灵活顶点格式

//这个函数是用来计算一个面的法相量的
void ComputeNormal(D3DXVECTOR3 *P0,D3DXVECTOR3 *P1,D3DXVECTOR3 *P2,D3DXVECTOR3 *out)
{
		D3DXVECTOR3 U=*P1-*P0;
		D3DXVECTOR3 V=*P2-*P0;
		D3DXVec3Cross(out,&U,&V);
		D3DXVec3Normalize(out,out);

}
//这个函数用来计算顶点法线
void CalcuVertexVec(D3DXVECTOR3 *a,D3DXVECTOR3 *b,D3DXVECTOR3 *c,D3DXVECTOR3 *d,D3DXVECTOR3 *Res)
{
	*Res=(1.0/4.0)*(*a+*b+*c+*d); 
}

void InitCube(LPDIRECT3DDEVICE9 Device)
{
		//八面体顶点法相量计算
	//顶点分量
	D3DXVECTOR3 A(0.0f,0.0f,-1.0f); 
	D3DXVECTOR3 B(0.0f,1.0f,0.0f);
	D3DXVECTOR3 C(1.0f,0.0f,0.0f);
	D3DXVECTOR3 D(0.0f,0.0f,1.0f);
	D3DXVECTOR3 E(-1.0f,0.0f,0.0f);
	D3DXVECTOR3 F(0.0f,-1.0f,0.0f);

	//八个面的法相量
	D3DXVECTOR3 NA,NB,NC,ND,NE,NF,NG,NH;
	ComputeNormal(&A,&B,&C,&NA);
	ComputeNormal(&B,&D,&C,&NB);
	ComputeNormal(&B,&E,&D,&NC);
	ComputeNormal(&E,&B,&A,&ND);
	ComputeNormal(&A,&C,&F,&NE);
	ComputeNormal(&C,&D,&F,&NF);
	ComputeNormal(&F,&D,&E,&NG);
	ComputeNormal(&E,&A,&F,&NH);

	//六个顶点的顶点向量
	D3DXVECTOR3 DA,DB,DC,DD,DE,DF;
	CalcuVertexVec(&NA,&ND,&NE,&NH,&DA);
	CalcuVertexVec(&NA,&NB,&NC,&ND,&DB);
	CalcuVertexVec(&NA,&NB,&NE,&NF,&DC);
	CalcuVertexVec(&NA,&NC,&NF,&NG,&DD);
	CalcuVertexVec(&NC,&ND,&NG,&NH,&DE);
	CalcuVertexVec(&NE,&NF,&NG,&NH,&DF);

	//创建顶点缓存
	Device->CreateVertexBuffer (
	24*sizeof(MyVertex),
		0,
		FVF_MYVERTEX,
		D3DPOOL_MANAGED,
		&vb,
		0);
	//创建索引缓存
	Device->CreateIndexBuffer (
		24*sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&ib,
		0
		);
	MyVertex *vertices=NULL;
	vb->Lock (0,0,(void**)&vertices,0);
	//前面
    vertices[0]=MyVertex(A, DA,  0.0f,0.0f);
	vertices[1]=MyVertex(B, DB,  1.0f,0.0f);
	vertices[2]=MyVertex(C, DC,  0.5f,1.0f); 
	vertices[3]=MyVertex(A ,DA,  1.0f,0.0f);
	vertices[4]=MyVertex(C, DC,  0.0f,0.0f);
	vertices[5]=MyVertex(F, DF,  0.5f,1.0f);



	//背面
	vertices[6]=MyVertex(B,DB,  0.0f,0.0f);
	vertices[7]=MyVertex(E,DE,  1.0f,0.0f);
	vertices[8]=MyVertex(D,DD,  0.5f,1.0f);

	vertices[9]=MyVertex(E,DE,  1.0f,0.0f);
	vertices[10]=MyVertex(F,DF,  0.0f,0.0f);
	vertices[11]=MyVertex(D,DD,  0.5f,1.0f);
    //左侧面
	vertices[12]=MyVertex(E,DE,  0.0f,0.0f);
	vertices[13]=MyVertex(B,DB,  1.0f,0.0f);
	vertices[14]=MyVertex(A,DA,  0.5f,1.0f);

	vertices[15]=MyVertex(E,DE,  0.0f,0.0f);
	vertices[16]=MyVertex(A,DA,  0.5f,1.0f);
	vertices[17]=MyVertex(F,DF,  1.0f,0.0f);
	//右侧面
	vertices[18]=MyVertex(C,DC,  0.0f,0.0f);
	vertices[19]=MyVertex(B,DB,  1.0f,0.0f);
	vertices[20]=MyVertex(D,DD,  0.5f,1.0f);

	vertices[21]=MyVertex(C,DC,  0.0f,0.0f);
	vertices[22]=MyVertex(D,DD,  0.5f,1.0f);
	vertices[23]=MyVertex(F,DF,  1.0f,0.0f);

	vb->Unlock() ;
	WORD *indices=NULL;
	ib->Lock (0,0,(void**)&indices,0);
	for(int i=0;i<24;i++)
		indices[i]=i;
	ib->Unlock ();

	//从文件中载入纹理
	D3DXCreateTextureFromFile(Device,L"a.jpg",&texture[0]);
	D3DXCreateTextureFromFile(Device,L"b.jpg",&texture[1]);
	D3DXCreateTextureFromFile(Device,L"c.jpg",&texture[2]);
	D3DXCreateTextureFromFile(Device,L"d.jpg",&texture[3]);
	D3DXCreateTextureFromFile(Device,L"e.jpg",&texture[4]);
	D3DXCreateTextureFromFile(Device,L"f.jpg",&texture[5]);
	D3DXCreateTextureFromFile(Device,L"g.jpg",&texture[6]);
	D3DXCreateTextureFromFile(Device,L"h.jpg",&texture[7]);
}

void RenderCube(LPDIRECT3DDEVICE9 Device)
{
		D3DXMATRIX World;
		D3DXMatrixTranslation(&World,0.0f,0.0f,0.0f);
		Device->SetTransform (D3DTS_WORLD,&World);
		Device->SetStreamSource (0,vb,0,sizeof(MyVertex));
		Device->SetIndices (ib);
		Device->SetFVF (FVF_MYVERTEX);
		for(int i=0;i<8;i++)
		{
			Device->SetTexture (0,texture[i]);
			Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,3,3*i,1);
		}
}
void Realesecube()
{
	d3d::Release<IDirect3DVertexBuffer9*>(vb);
	d3d::Release <IDirect3DIndexBuffer9*>(ib);
	for(int i=0;i<8;i++)
	{
		d3d::Release <IDirect3DTexture9*>(texture[i]);
	}
}