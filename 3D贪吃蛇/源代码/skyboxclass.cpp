#include"skyboxclass.h"
#include"d3dUtility.h"

//===========================================================================
//构造函数：初始化
//===========================================================================
SkyBox::SkyBox( LPDIRECT3DDEVICE9  pDevice )
{
	//给各个参数赋初值
	m_pVertexBuffer=NULL;
	Device=pDevice;
	for(int i=0; i<6; i++)
		Texture[i] = NULL; 
	m_Length = 0.0f;
}

void SkyBox::LoadSkyTextureFromFile(wchar_t *pFrontTextureFile, wchar_t *pBackTextureFile,wchar_t *pLeftTextureFile, wchar_t *pRightTextureFile,wchar_t *pTopTextureFile,wchar_t *pGroundTextureFile)
{
	//从文件加载六张纹理
	D3DXCreateTextureFromFile( Device , pFrontTextureFile, &Texture[0] );  //前面
	D3DXCreateTextureFromFile( Device , pBackTextureFile,  &Texture[1] );  //后面
	D3DXCreateTextureFromFile( Device , pLeftTextureFile,  &Texture[2] );  //左面
	D3DXCreateTextureFromFile( Device , pRightTextureFile, &Texture[3] );  //右面
	D3DXCreateTextureFromFile( Device , pTopTextureFile,   &Texture[4] );  //上面
	D3DXCreateTextureFromFile( Device , pGroundTextureFile,   &Texture[5] );  //底面


}

//===============================================================================
//初始化天空盒函数
//================================================================================
void SkyBox::InitSkyBox(float length)
{

	m_Length=length;
	Device->CreateVertexBuffer (
	24*sizeof(SKYBOXVERTEX),
		0,
		SkyBoxFVF,
		D3DPOOL_MANAGED,
		&m_pVertexBuffer,
		0);
	//前面
	SKYBOXVERTEX vertices[] =
    {
		//前面的四个顶点
		{ -m_Length/2, 0.0f,    m_Length/2, 0.0f, 1.0f, },//A
		{ -m_Length/2, m_Length/2,   m_Length/2, 0.0f, 0.0f, },//B
		{  m_Length/2, 0.0f,    m_Length/2, 1.0f, 1.0f, },//C
		{  m_Length/2, m_Length/2,   m_Length/2, 1.0f, 0.0f, },//D

		//背面的四个顶点
		{  m_Length/2, 0.0f,   -m_Length/2, 0.0f, 1.0f, },//E
		{  m_Length/2, m_Length/2,  -m_Length/2, 0.0f, 0.0f, },//F
		{ -m_Length/2, 0.0f,   -m_Length/2, 1.0f, 1.0f, },//G
		{ -m_Length/2, m_Length/2,  -m_Length/2, 1.0f, 0.0f, },//H

		//左面的四个顶点
		{ -m_Length/2, 0.0f,   -m_Length/2, 0.0f, 1.0f, },
		{ -m_Length/2, m_Length/2,  -m_Length/2, 0.0f, 0.0f, },
		{ -m_Length/2, 0.0f,    m_Length/2, 1.0f, 1.0f, },
		{ -m_Length/2, m_Length/2,   m_Length/2, 1.0f, 0.0f, },

		//右面的四个顶点
		{ m_Length/2, 0.0f,   m_Length/2, 0.0f, 1.0f, },
		{ m_Length/2, m_Length/2,  m_Length/2, 0.0f, 0.0f, },
		{ m_Length/2, 0.0f,  -m_Length/2, 1.0f, 1.0f, },
		{ m_Length/2, m_Length/2, -m_Length/2, 1.0f, 0.0f, },

		//上面的四个顶点
		{  m_Length/2, m_Length/2, -m_Length/2, 1.0f, 0.0f, },//f
		{  m_Length/2, m_Length/2,  m_Length/2, 1.0f, 1.0f, },//d
		{ -m_Length/2, m_Length/2, -m_Length/2, 0.0f, 0.0f, },//h
		{ -m_Length/2, m_Length/2,  m_Length/2, 0.0f, 1.0f, }, //b

		

		//下面的四个顶点
		{ -m_Length/2, 0.0f,    -m_Length/2, 0.0f, 1.0f, },
		{ -m_Length/2, 0.0f,   m_Length/2, 0.0f, 0.0f, },
		{ m_Length/2, 0.0f, - m_Length/2, 1.0f, 1.0f, },
		{ m_Length/2, 0.0f,   m_Length/2, 1.0f, 0.0f, }

    };
	//准备填充顶点数据
    void* pVertices;
	//2.加锁
    m_pVertexBuffer->Lock( 0, 0, (void**)&pVertices, 0 );
	//3.访问。把结构体中的数据直接拷到顶点缓冲区中
    memcpy( pVertices, vertices, sizeof(vertices) );
	//4.解锁
	m_pVertexBuffer->Unlock();
}
//============================================================================================
//渲染天空盒函数
//============================================================================================
void SkyBox::RenderSkyBox()
{
	Device->SetRenderState(D3DRS_LIGHTING,false);
	Device->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);  //将纹理颜色混合的第一个参数的颜色值用于输出  
    Device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );   //纹理颜色混合的第一个参数的值就取纹理颜色值  


	//Device->SetRenderState (D3DRS_ALPHABLENDENABLE,true);//开启融合
	//Device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_ZERO);
	//Device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_ONE);
	//设置纹理夹取寻址模式
	Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	D3DXMATRIX matSky,matTransSky,matRotSky;  
	D3DXMatrixTranslation(&matTransSky,0.0f,-5000.0f,0.0f);  
    D3DXMatrixRotationY(&matRotSky, -0.000005f*timeGetTime());   //旋转天空网格, 简单模拟云彩运动效果  
    matSky=matTransSky*matRotSky;  

	Device->SetTransform (D3DTS_WORLD,&matSky);

	Device->SetStreamSource(0,m_pVertexBuffer, 0, sizeof(SKYBOXVERTEX));    //把包含的几何体信息的顶点缓存和渲染流水线相关联

	Device->SetFVF(SkyBoxFVF);  //设置FVF灵活顶点格式  
    //一个for循环，将5个面绘制出来  
    for(int i =0; i<6; i++)  
    {  
        Device->SetTexture(0, Texture[i]);  
       Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, i*4, 2);  
    }  
	Device->SetRenderState (D3DRS_LIGHTING,true);
	//Device->SetRenderState (D3DRS_ALPHABLENDENABLE,false);
}

//=======================================================================
//析构函数
//=======================================================================
SkyBox::~SkyBox(void)  
{  
	m_pVertexBuffer->Release ();
    for(int i=0; i<5; i++)  
    {  
		Texture[i]->Release ();
    }  
}