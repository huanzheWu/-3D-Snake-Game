#include<math.h>
#include"EnemyTank.h"

extern GroundObject * groundobject ;//把地面物体实例的作用域拓展到改类中，因为需要访问该实例中的成员

EnemyTank::EnemyTank (LPDIRECT3DDEVICE9 _Device)
{
	Device=_Device;
	deadline=200.0;//设定坦克的被攻击范围
}
bool EnemyTank::InitTank ()
{
	InitXfile(L"M3FRVT1.x");//指定敌方坦克x文件模型
	return true;
}
//============================================================
//这个函数用来设置坦克的初始信息
//============================================================
void EnemyTank::resetTank (ENEMYTANK *etank)
{
	//位置随机
	etank->_position.x=d3d::GetRandomFloat(-1000,1000);
	etank->_position.y =00.0f;
	etank->_position.z=d3d::GetRandomFloat (2000,4000);
	etank->_isalive=true;
	D3DXMatrixIdentity(&(etank->_rotation));
	//坦克的look向量需要载入模型，观察一开始坦克是朝向哪边，再来设置
	//获得坦克的look向量后，用crood函数来更改旋转后的这个向量，这里先假设它的look为（0，0，-1）
	etank->_look.x=0.0f,etank->_look.y=0.0f,etank->_look .z=-1.0f;
	//在2-5秒之间，随机产生坦克的行走时间
	etank->_walktime=d3d::GetRandomFloat (3.0f,6.0f);
	//状态保存时间为0	
	etank->_keeptime =0;
	//子弹发射时间间隔为1-3秒之间随机
	etank->_launchtime=d3d::GetRandomFloat (1.0f,3.0f);
	//距上一次发射的时间间隔先设为零
	etank->_unlaunchtime =0;
	//一开始不发射子弹
	etank->_launch =false;
}
void EnemyTank::addTank()
{
	ENEMYTANK enemytank;
	resetTank(&enemytank);//设置新坦克的信息
	_allenetank.push_back (enemytank);//向链表中加入新的坦克
}



void EnemyTank::upDate (float timeDelta)
{
	//可以定义一个时间，让坦克走行走那段时间后转弯。
	std::list <ENEMYTANK>::iterator i;
	if(!_allenetank.empty())
	{
		for(i=_allenetank.begin();i!=_allenetank.end ();i++)
		{
			//朝着观察向量行走
			i->_position.x+=200*timeDelta*(i->_look.x);
			i->_position.y+=200*timeDelta*(i->_look.y);
			i->_position.z+=200*timeDelta*(i->_look.z);
			i->_keeptime+=timeDelta;
			i->_unlaunchtime+=timeDelta;

			static int enble1=0;

			if(i->_keeptime>i->_walktime||i->_position.z<=-10000||i->_position .z>=10000)
			{
				if(i->_keeptime>i->_walktime)
					enble1=1;
				//旋转，这个旋转也需要随机方向,角度是否要随机？（暂时先定为随机试一试）
				//旋转代码	
				angle=d3d::GetRandomFloat (-0.25,0.25);//先随机给0.1-1（18度到PI）
				D3DXMatrixRotationY(&(i->_rotation),D3DX_PI*angle);
				i->_look .x=0.0f,i->_look .y =0.0f,i->_look .z=-1.0f;

				if(i->_position.z<=-10000||i->_position .z>=10000)
				{
					i->_position.x=d3d::GetRandomFloat(-10000,10000);
					i->_position.y =00.0f;
					i->_position.z=d3d::GetRandomFloat (4000,8000);
				}

				//改变坦克的观察向量的方向
				D3DXVec3TransformCoord(&(i->_look),&(i->_look),&(i->_rotation));
 				i->_keeptime =0;//状态时间置零
			}


			if(groundobject->BCollisiontest(i->_position)&&enble1==1)//如果坦克和公告版碰撞
			{
				D3DXMatrixRotationY(&(i->_rotation),D3DX_PI*angle);
				enble1=0;	
				D3DXVec3TransformCoord(&(i->_look),&(i->_look),&(i->_rotation));
			}

		

			if(i->_unlaunchtime>i->_launchtime)
			{
				//发射子弹间隔时间足够长，可以发射子弹了
				i->_launch =true;
			}
		}	
	}
	removedeadtank();
}



void EnemyTank::removedeadtank()
{
	std::list <ENEMYTANK>::iterator i;
	i=_allenetank.begin();
	while (i!=_allenetank.end ())
	{
		if(i->_isalive==false)
			i=_allenetank.erase (i);
		else 
			i++;
	}
}

void EnemyTank::Render ()
{
	std::list <ENEMYTANK>::iterator i;
	for(i=_allenetank.begin();i!=_allenetank.end ();i++)
	{
		if(i->_isalive)
		{
			//取得当前坦克位置
			D3DXMATRIX move;
			D3DXMatrixTranslation(&move,i->_position.x,i->_position.y-100,i->_position.z);
			//缩放坦克大小
			D3DXMATRIX matscal;
			D3DXMatrixScaling(&matscal,0.015f,0.015f,0.015f);
			move=matscal*(i->_rotation)*move;
			Device->SetTransform (D3DTS_WORLD,&move);
			RenderXfile ();
		
			if(i->_launch)
			{
				//发射子弹？
				//这样不行，子弹只会被渲染一次
				//应该是回传这个时间的坦克的位置和方向，而在display函数里渲染这些已经发射出去的子弹
				//在这里添加代码addbullet？是的
				addEbullet (i->_position ,i->_look );
				i->_launch=false ;
				i->_unlaunchtime=0;
			}
		}
	}
}


//========================================================================
//初始化子弹函数（指定文件）
//========================================================================

void EnemyTank::InitEbullet()
{
	/*三步曲之一：通过.X文件加载网格模型*/
	D3DXLoadMeshFromX(
		L"airplane.x",
		D3DXMESH_MANAGED,
		Device,
		&eppAdjacency,
		&eppMaterials,
		NULL,
		&epNumMaterials,//返回网格中的材质数目
		&Bulletmesh//返回所创建的并已填充好XFile几何数据的ID3DXMesh对象
		);
	//检测载入的XFile中是否含有顶点法线信息。
	if(!Bulletmesh->GetFVF ()&D3DFVF_NORMAL)
	{
		ID3DXMesh *temp=0;
		Bulletmesh->CloneMeshFVF(
			D3DXMESH_MANAGED,
			Bulletmesh->GetFVF ()|D3DFVF_NORMAL,
			Device,
			&temp);
		Bulletmesh->Release ();
		Bulletmesh=temp;
	}

	//三步曲之二：载入材质和纹理
	D3DXMATERIAL *pMtrls=(D3DXMATERIAL *)eppMaterials->GetBufferPointer(); 

	eg_pMaterials= new D3DMATERIAL9[epNumMaterials];  
    eg_pTextures  = new LPDIRECT3DTEXTURE9[ epNumMaterials];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         

	//获取纹理和材质数据
	for(DWORD i=0; i<epNumMaterials; i++)  
      {  
           //获取材质，并设置一下环境光的颜色值  
           eg_pMaterials[i]= pMtrls[i].MatD3D;  
           eg_pMaterials[i].Ambient= eg_pMaterials[i].Diffuse;  
           //创建一下纹理对象  
           eg_pTextures[i]  = NULL;  
           D3DXCreateTextureFromFileA(Device,pMtrls[i].pTextureFilename, &eg_pTextures[i]);  
      }  
	//进行网格优化
		Bulletmesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT |D3DXMESHOPT_VERTEXCACHE|D3DXMESHOPT_ATTRSORT ,
		(DWORD *)eppAdjacency->GetBufferPointer (),
		(DWORD *)eppAdjacency->GetBufferPointer (),
		NULL,
		NULL
		);
		eppAdjacency->Release ();
		eppMaterials->Release ();
}
//========================================================================
//初始化一颗新子弹的函数,作为addEbullet函数的调用函数
//========================================================================
void EnemyTank::resetEbullet (Ebullet *ebullet,D3DXVECTOR3 position,D3DXVECTOR3 look)
{
	ebullet->_age=0;//新生的子弹
	ebullet->_alive=true;//活的
	ebullet->_lifetime=10;//寿命1秒钟
	ebullet->_position =position; //这里到时候需要来修改参数 ，使得子弹的方向正确
	ebullet->_speed=look;

}

//========================================================================
//增加子弹函数，作为render的调用函数
//========================================================================
void EnemyTank::addEbullet (D3DXVECTOR3 position,D3DXVECTOR3 look)
{
	Ebullet ebullet;//新子弹
	resetEbullet(&ebullet,position,look);//设置子弹的位置和方向
	_Ebullet.push_back (ebullet);//加入子弹链表
}

//=========================================================================
//更新子弹函数
//=========================================================================
void EnemyTank::UpDateBullet(float timeDelta)
{
	std::list <Ebullet>::iterator i;
	for(i=_Ebullet.begin();i!=_Ebullet.end ();i++)
	{
		//更新子弹的位置
		i->_position .x +=1000*timeDelta*(i->_speed .x);
		i->_position .y +=1000*timeDelta*(i->_speed .y);
		i->_position .z +=1000*timeDelta*(i->_speed .z);
		i->_age +=timeDelta;
		if(i->_age >i->_lifetime )
			i->_alive=false;
	}
	removedead ();//调用移除死亡子弹的函数
}


void EnemyTank::removedead ()
{
	std::list <Ebullet>::iterator i;
	i=_Ebullet.begin();
	while (i!=_Ebullet.end ())
	{
		if(i->_alive==false)
			i=_Ebullet.erase (i);
		else 
			i++;
	}
}


void EnemyTank::RenderBullet()
{
	std::list <Ebullet>::iterator j;
	if(!_Ebullet.empty ())//判断是否有子弹，有的话进入渲染
	{
		for(j=_Ebullet.begin();j!=_Ebullet.end ();j++)
		{
			if(j->_alive=true)
			{
				D3DXMATRIX MaxLarght;
				D3DXMatrixScaling (&MaxLarght,10.0f,10.0f,10.0f);
					D3DXMATRIX Move;
					D3DXMatrixTranslation(&Move,j->_position.x,j->_position.y -30,j->_position.z);//指定子弹的初始位置 
					Move=MaxLarght*Move;
					Device->SetTransform (D3DTS_WORLD,&Move);
				//RenderXfile
				for(DWORD i = 0; i < epNumMaterials; i++)  
				{  
					Device->SetMaterial(&eg_pMaterials[i]);  
					Device->SetTexture(0,eg_pTextures[i]);  
					Bulletmesh->DrawSubset(i);  
				 } 
			}
		}
	}
}
//==============================================================================================
//碰撞测试函数
//==============================================================================================

bool  EnemyTank::Collisiontest(D3DXVECTOR3 _position)
{
	double distance;
	std::list<ENEMYTANK>::iterator i;
	for(i=_allenetank.begin();i!=_allenetank.end ();i++)
	{
		distance=sqrt(pow((i->_position.x-_position.x),2)+pow((i->_position.y-_position.y),2)+pow((i->_position.z-_position.z),2));
		if(distance<deadline)
			{
				i->_isalive =false;
				return true;
			}
	}
	return false;
}




EnemyTank::~EnemyTank()
{

}
