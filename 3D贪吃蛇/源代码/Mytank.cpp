#include"Mytank.h"
Tank::Tank(LPDIRECT3DDEVICE9 _Device)
{
	Device=_Device;
	D3DXMatrixIdentity(&matHero);
}

void Tank::SetTank(CamerClass* Camera,DInputClass * DInput)
{

		static float j=0;
		D3DXMATRIX matscal;
		D3DXMatrixScaling(&matscal,0.01f,0.01f,0.01f);//把模型缩小
		D3DXVECTOR3 tankposition;//坦克的位置
		D3DXMATRIX setrightpos; //旋转正确的方向
		D3DXMatrixRotationY(&setrightpos,D3DX_PI); //旋转180度，和摄像机同向
		(*Camera).getposition (&tankposition);//获取摄像机的位置
		DInput->GetInput();  

		//if(DInput->IsKeyDown(DIK_A))
		//	j+=0.006;
		//if(DInput->IsKeyDown(DIK_D))
		//	j-=0.006;
		j+=DInput->MouseDX()* 0.006f;
		//鼠标移动控制摄像机视觉
		(*Camera).yaw(DInput->MouseDX()* 0.006f); 
	/*	(*Camera).yaw(j);  */


		D3DXMATRIX temp;
		D3DXMatrixRotationY(&temp,j);
		D3DXMatrixTranslation(&matHero,tankposition.x ,tankposition.y-100.0f,tankposition.z);  //移动
		matHero=matscal*temp*setrightpos*matHero;

		Device->SetTransform(D3DTS_WORLD, &matHero);//设置模型的世界矩阵，为绘制做准备 
	
}