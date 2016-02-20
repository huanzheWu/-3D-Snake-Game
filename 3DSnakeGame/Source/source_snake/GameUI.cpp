#include"GameUI.h"
#define D3DFVF_SCORE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)//分数的灵活顶点格式

//初始化函数
bool  GameUI::initPicture (wchar_t* FileName ,float x,float y,float pictureScal)
{
	D3DXCreateTextureFromFile(Device,FileName, &Tex[0]);//载入纹理
	unsigned long white = D3DCOLOR_XRGB(255,255,255);
		//获取分数贴图的长宽高
		D3DSURFACE_DESC desc;
		Tex[0]->GetLevelDesc(0, &desc); 
		float w = (float)desc.Width;
		float h = (float)desc.Height;

		Device->CreateVertexBuffer (
		4*sizeof(SCORE),
		0,
		D3DFVF_SCORE,
		D3DPOOL_MANAGED,
		&Vb,
		0
		);

		SCORE * Vertex;
		Vb->Lock(0,0,(void **)&Vertex,0);
		Vertex[0]=SCORE(w/pictureScal+x,	y,					0,	1,	white,	1,	0);
		Vertex[1]=SCORE(w/pictureScal+x,	y+h/pictureScal,	0,	1,	white,	1,	1);
		Vertex[2]=SCORE(x,					y,					0,	1,	white,	0,	0);
		Vertex[3]=SCORE(x,					y+h/pictureScal,	0,	1,	white,	0,	1);
		Vb->Unlock ();
		return true;
}

//渲染函数
void GameUI::RenderUI ()
{
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	Device->SetTexture(0,Tex[0]);
	Device->SetStreamSource(0, Vb, 0, sizeof(SCORE));
	Device->SetFVF(D3DFVF_SCORE);
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	
	//关闭融合
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
}

bool GameUI::initTex(wchar_t *Font)
{
	//创建字体
	D3DXCreateFont (Device,25,12,1000,0,true,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,0,Font ,&pFont);
	return true;
}

void GameUI::RenderFont(RECT rect ,wchar_t *content)
{
	pFont->DrawText(NULL,content,-1,&rect,DT_LEFT|DT_NOCLIP|DT_SINGLELINE,D3DCOLOR_XRGB(255,255,255));
}

