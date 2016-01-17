#include"Mirror.h"
const DWORD Mirror::MirrorVertex::FVF_Mirror = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

void Mirror::InitMirror(int length)
{
	
	Device->CreateVertexBuffer (
		6*sizeof(MirrorVertex),
		0,
		MirrorVertex::FVF_Mirror,
		D3DPOOL_MANAGED,
		&Vb,
		0
		);
	MirrorVertex *mirrorVertex=NULL;
	Vb->Lock (0,0,(void **)&mirrorVertex,0);
	mirrorVertex[0]=MirrorVertex(0,0.0f,1000.0f,-1.0f,0.0f,0.0f,0.0f,0.0f);//a
	mirrorVertex[1]=MirrorVertex(0,1000,1000,-1.0f,0.0f,0.0f,1.0f,1.0f);//c
	mirrorVertex[2]=MirrorVertex(0,1000,-1000,-1.0f,0.0f,0.0f,1.0f,0.0f);//b

	mirrorVertex[3]=MirrorVertex(0,0.0f,1000,-1.0f,0.0f,0.0f,1.0f,1.0f);//a
	mirrorVertex[4]=MirrorVertex(0,1000,-1000,-1.0f,0.0f,0.0f,0.0f,1.0f);//b
	mirrorVertex[5]=MirrorVertex(0,0.0f,-1000,-1.0f,0.0f,0.0f,1.0f,0.0f);//d

	Vb->Unlock ();
	D3DXCreateTextureFromFile(Device, L"ice.bmp", &Mirrortex);
}
D3DXMATRIX Mirror::PreSet(D3DXMATRIX moddleplace)
{
	
		ModdlePlace=moddleplace;
		//渲染镜子
		D3DXMATRIX I;
		D3DXMatrixTranslation(&I,101.0f,-200.0f,0.0f);
		Device->SetTransform(D3DTS_WORLD, &I);
		Device->SetStreamSource (0,Vb,0,sizeof(MirrorVertex));
		Device->SetFVF (MirrorVertex::FVF_Mirror);
		Device->SetTexture  (0,Mirrortex);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		Device->SetRenderState (D3DRS_STENCILENABLE,true);//启用模版缓存
		Device->SetRenderState (D3DRS_STENCILFUNC,D3DCMP_ALWAYS);//测试总是成功
		Device->SetRenderState (D3DRS_STENCILREF,0x1);//参考值
		Device->SetRenderState (D3DRS_STENCILMASK,0xffffffff);
		Device->SetRenderState (D3DRS_STENCILWRITEMASK,0xffffffff);
		Device->SetRenderState (D3DRS_STENCILZFAIL,D3DSTENCILOP_KEEP);
		Device->SetRenderState (D3DRS_STENCILFAIL,D3DSTENCILOP_KEEP);
		Device->SetRenderState (D3DRS_STENCILPASS,D3DSTENCILOP_REPLACE);//用模版参考值替代模版缓存中对应的值

		Device->SetRenderState (D3DRS_ZWRITEENABLE,false);//关闭深度缓存，阻止对z缓存进行写操作

		Device->SetRenderState (D3DRS_ALPHABLENDENABLE,true);//开启融合运算，阻止引起后台缓存中的变化
		//设置融合因子，这样设置使得后台缓存不发生变化
		Device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_ZERO);
		Device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_ONE);
		
		//然后开始画出镜子
		Device->SetTexture  (0,Mirrortex);
		D3DXMatrixTranslation(&I,100.0f,-200.0f,0.0f);
		Device->SetTransform(D3DTS_WORLD, &I);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		//绘制映像只有匹配才能写入（只有在镜面区才能显示）
		Device->SetRenderState (D3DRS_STENCILFUNC,D3DCMP_EQUAL);
		Device->SetRenderState (D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);

		//清理深度缓存，因为镜像深度大于镜面深度
		Device->Clear (0,0,D3DCLEAR_ZBUFFER,0,1.0f,0);
		//开启融合，使得映像在镜子中出现
		Device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
		Device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_ZERO);
		Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		//定位映像位置
		D3DXMATRIX W,R;
		D3DXPLANE planeXZ(1.0f,0.0f,0.0f,-100.0f);//xz平面
		D3DXMatrixReflect(&R,&planeXZ);
		W=R*ModdlePlace;

		return W;

}
void Mirror::LastSet ()
{

		Device->SetRenderState (D3DRS_ALPHABLENDENABLE,false);
		Device->SetRenderState (D3DRS_STENCILENABLE,false);
		Device->SetRenderState (D3DRS_ZWRITEENABLE,true);
		Device->SetRenderState (D3DRS_CULLMODE,D3DCULL_CCW);
}
Mirror::~Mirror()
{
	Vb->Release ();
	Mirrortex->Release ();
}