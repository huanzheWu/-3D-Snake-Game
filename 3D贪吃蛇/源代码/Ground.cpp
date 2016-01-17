#include"Ground.h"

//********初始化函数***************************
bool Ground::initObject ()
{
	//创建围墙的纹理
	D3DXCreateTextureFromFile (Device,L"ice.bmp",&Tex[0]);
	Wall=new XFileMesh(Device);
	//初始化围墙模型
	Wall->InitXfile (L"shu.x");

	//创建鸟的纹理
	D3DXCreateTextureFromFile(Device,L"dmentia_bluebird.bmp",&Tex[1]);
	Bird=new XFileMesh(Device);
	//初始化鸟模型
	Bird->InitXfile (L"niao.x");

	//设置墙壁的缩放矩阵
	D3DXMatrixScaling(&WallSca,1000.0f,1000.0f,1000.0f);
	//设置鸟的缩放矩阵
	D3DXMatrixScaling(&BirdSca,  10.0f,  10.0f,   10.0f);

	//鸟的飞行方向1
	BirdDir1.x =0.0f,BirdDir1.y=0.0f,BirdDir1.z =1.0f;	//飞行方向1
	BirdDir2.x =0.0f,BirdDir2.y=0.0f,BirdDir2.z =-1.0f;	//飞行方向2

	wallRang=500.0f;

	return true;
}

//******************************************创建围墙函数**********************************************************
bool Ground ::createWall(D3DXVECTOR3 centre)
{
	float i;
	D3DXVECTOR3 position;
	_allWalls.clear ();
	//指定围墙的位置
	for (i=centre.x -wallRang;i<=centre.x+wallRang;i+=100)  //前
	{
		position.y =0.0f,position.z =wallRang,position.x=i;
		_allWalls.push_back (position);
	}
	for (i=centre.z-wallRang;i<=centre.z +wallRang;i+=100)//左
	{
		position.x=-wallRang,position.y=0.0f,position.z =i;
		_allWalls.push_back (position);
	}
	for(i=centre.z -wallRang;i<=centre.z+wallRang;i+=100 )//右
	{
		position.x =wallRang,position.y =0.0f,position.z =i;
		_allWalls.push_back (position);
	}
	for (i=centre.x -wallRang;i<=centre.x+wallRang;i+=100)//后
	{
		position.y =0.0f,position.z =-wallRang,position.x=i;
		_allWalls.push_back (position);
	}
	return true;
}

//******************************************************************创建鸟的函数********************************************************
bool Ground ::createBird (D3DXVECTOR3 centre)
{
	//指定鸟的位置

	BIRD bird;
	float i;
	static int j=10,k=10;
	for (i=centre.x -500;i<=centre.x +500;i +=100)
	{
		bird._direction=BirdDir2; //指定鸟的飞行方向
		bird._position.x =i;bird._position.z = 500+j;bird._position.y =360;//指定鸟的初始位置
		_allBird.push_back (bird);
		if(i<0)
			j+=50;
		else 
			j-=50;
	}
	for(i=centre.x-450;i<=centre.x + 500;i+=100)
	{
		bird._direction=BirdDir1;//指定鸟的飞行方向
		bird._position.x =i;bird._position.z =-500+k;bird._position.y =360;//指定鸟的初始位置
		_allBird.push_back (bird);
		if(i<0)
			k-=50;
		else 
			k+=50;
	}
	D3DXMatrixRotationY(&BirdRota,0);
	return true;
}

//********************************************************更新鸟状态的函数**************************************************************

void Ground::updateBird(float timeDelta)
{
	std::list <D3DXVECTOR3 >::iterator wall;
	for(wall=_allWalls.begin();wall!=_allWalls.end (); wall++)
	{
		if(wall->x ==500.0f)
			wall->x =wallRang;
		else if(wall->y==500.0f)
			wall->y =wallRang;
		else if(wall->z ==500.0f)
			wall->z ==wallRang;
		else if(wall->x ==-500.0f)
			wall->x =-wallRang;
		else if(wall->y==-500.0f)
			wall->y =-wallRang;
		else if(wall->z ==-500.0f)
			wall->z ==-wallRang;
	}

	std::list <BIRD>::iterator i;
	static float time=0;
	static int rota=0;
	time+=timeDelta;
	for(i=_allBird.begin();i!=_allBird.end ();i++)
	{
		i->_position.z=i->_position.z+100*timeDelta*i->_direction.z;
		if(time>10)
		{
			i->_direction.z =-(i->_direction.z);
			if(i==--_allBird.end ())
				time=0;
			if(rota%2==0)
				ifrota=true;//每10秒转一次向
			else 
				ifrota=false;
			rota++;
		}
	}
}



//***********渲染地面物体函数*******************
void Ground::renderGround ()
{
	//绘制围墙
	std::list <D3DXVECTOR3 >::iterator i;
	for(i=_allWalls.begin ();i!=_allWalls.end ();i++)
	{
		//设置世界矩阵，把围墙放置在适当的位置
		D3DXMatrixTranslation(&WallPos,i->x ,i->y ,i->z);
		WallPos=WallSca*WallPos;
		Device->SetTransform (D3DTS_WORLD,&WallPos);
		//设置纹理
		Device->SetTexture (0,Tex[0]);
		//绘制围墙x文件
		Wall->RenderXfile ();
	}

	//绘制鸟
	std::list <BIRD>::iterator j;
	for(j=_allBird.begin ();j!=_allBird.end ();j++)
	{
		D3DXMATRIX BirdPos;
		D3DXMatrixTranslation(&BirdPos,j->_position.x,j->_position.y ,j->_position.z  );
		if(ifrota==true)
		{
			D3DXMatrixRotationY(&BirdRota,D3DX_PI);
		}
		BirdPos=BirdSca*BirdRota*BirdPos;
		Device->SetTransform (D3DTS_WORLD ,&BirdPos);
		Device->SetTexture (0,Tex[1]);
		Bird->RenderXfile ();
	}
}

//**************************************************析构函数************************************
Ground::~Ground()
{

}