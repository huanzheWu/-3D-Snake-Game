#include"GameProps.h"
#include"XFileMesh.h"
#include"Explode.h"

#define D3DFVF_TIME (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)//时间的灵活顶点格式

extern Tank *mytank;//将在melon中的坦克实例作用域拓展到这里。（我方坦克和道具碰撞测试所需）
extern EnemyTank *enetank;//将在melon中的敌人坦克实例作用域拓展到这里。（飞机子弹与敌方坦克碰撞测试所需）
extern Explode *explode;//把爆炸实例作用域拓展到这里

GameProps::GameProps (LPDIRECT3DDEVICE9 _Device)
{
	Device=_Device;
	time=60;
}
//====================================================================
//初始化游戏道具函数
//三种道具的x模型都应该在这一步中初始化
//====================================================================

void GameProps::InitProps()
{
	//为3个道具创建3个XFileMesh接口
	PPops1 =new XFileMesh (Device);
	PPops2	=new XFileMesh (Device);
	PPops3 =new XFileMesh(Device);
	//创建飞机x文件接口
	PEffect1=new XFileMesh(Device);
	PBullet=new XFileMesh(Device);

	//初始化3个道具的x文件
	PPops1->InitXfile(L"m18nade.x"); //载入时间有点长
	PPops2->InitXfile(L"shose.x");
	PPops3->InitXfile(L"medkit.x");
	PBullet->InitXfile (L"444.x"); //载入时间有点长

	//初始化飞机x文件
	PEffect1->InitXfile(L"dropship.x");


	//创建道具链表
	for(int i=1;i<4;i++)
	{
		PROPS props;
		props.ID=i; 
		props._isalive=false;
		props._happenTime=0; 
		props._timeDelta=3;
		_allProps.push_back (props);
	}
	//
	//
	//
	//
	wchar_t a[61][10]={{},
		{L"1.png"},{L"2.png"},{L"3.png"},{L"4.png"},{L"5.png"},{L"6.png"},{L"7.png"},{L"8.png"},{L"9.png"},
		{L"10.png"},{L"11.png"},{L"12.png"},{L"13.png"},{L"14.png"},{L"15.png"},{L"16.png"},{L"17.png"},{L"18.png"},{L"19.png"},
		{L"20.png"},{L"21.png"},{L"22.png"},{L"23.png"},{L"24.png"},{L"25.png"},{L"26.png"},{L"27.png"},{L"28.png"},
		{L"29.png"},{L"30.png"},{L"31.png"},{L"32.png"},{L"33.png"},{L"34.png"},{L"35.png"},{L"36.png"},{L"37.png"},{L"38.png"},
		{L"39.png"},{L"40.png"},{L"41.png"},{L"42.png"},{L"43.png"},{L"44.png"},{L"45.png"},{L"46.png"},{L"47.png"},{L"48.png"},
		{L"49.png"},{L"50.png"},{L"51.png"},{L"52.png"},{L"53.png"},{L"54.png"},{L"55.png"},{L"56.png"},{L"57.png"},{L"58.png"},
		{L"59.png"},{L"60.png"}
	};
	for(int i=1;i<=60;i++)
	{
		D3DXCreateTextureFromFile(Device,a[i], &TimeTex[i]);//创建纹理
	}
		unsigned long white = D3DCOLOR_XRGB(255,255,255);
		//获取血条的长宽高
		D3DSURFACE_DESC desc;
		TimeTex[10]->GetLevelDesc(0, &desc);
		float w = (float)desc.Width;
		float h = (float)desc.Height;

		Device->CreateVertexBuffer (
		4*sizeof(TIME),
		0,
		D3DFVF_TIME,
		D3DPOOL_MANAGED,
		&Vb,
		0
		);

		TIME * Vertex;
		Vb->Lock(0,0,(void **)&Vertex,0);
		Vertex[0]=TIME(w+600,10,0,1,white,1,0);
		Vertex[1]=TIME(w+600,10+h,0,1,white,1,1);
		Vertex[2]=TIME(600,10,0,1,white,0,0);
		Vertex[3]=TIME(600,10+h,0,1,white,0,1);
		Vb->Unlock ();

		sign=0;//加速道具初始化为0，表示还没被吃到
		sign3=0; 
		speeduptime=10;//加速时间为10s；
}

//================================================================================
//更新道具函数
//现在只想到更新道具的生存状态、出现位置
//
//================================================================================
void GameProps::upDateProps(float timeDelta)
{
	std::list <PROPS>::iterator i;
	for(i=_allProps.begin();i!=_allProps.end ();i++)
	{
		i->_happenTime+=timeDelta;
		if(i->_happenTime>i->_timeDelta&&i->_isalive==false) //只有当时间到了，而且道具是死的时候才去更新它。否则会出现一段时间后道具会移动的现象
				{
					i->_isalive=true; //道具出现时间到了
					//随机给道具一个出现的位置
					i->_position.x=d3d::GetRandomFloat (-1000,1000);
					i->_position.y =-100.0f;
					i->_position.z=d3d::GetRandomFloat (-1000,1000);
				}
		
	}
}

//==========================================================================
//渲染道具函数
//这里关系到一个问题：什么时候才要渲染，渲染后道具状态如何定
//如何将道具和它的x文件联系起来
//==========================================================================
void GameProps::RenderProps ()
{
	D3DXMATRIX position;//道具的出现位置
	D3DXMATRIX scaling;
	D3DXMatrixScaling(&scaling,1,1,1);
	std::list <PROPS>::iterator i;
	for(i=_allProps.begin();i!=_allProps.end ();i++)
	{
		if(i->_isalive==true)
		{
			//设置世界矩阵
			D3DXMatrixTranslation(&position,i->_position.x,i->_position.y,i->_position.z);
			position=scaling*position;
			Device->SetTransform (D3DTS_WORLD,&position);
			
			if(i->ID==1)
				PPops1->RenderXfile ();
			if(i->ID==2)
				PPops2->RenderXfile();
			if(i->ID==3)
				PPops3->RenderXfile();
		}
	}
}
//===============================================================================
//道具碰撞检测以及调用相关的效果函数
//===============================================================================
void GameProps::Effect(float timeDelta)
{
	std::list <PROPS>::iterator i;
	for(i=_allProps.begin();i!=_allProps.end ();i++)
	{
		if(mytank->collisiontest(i->_position)&&i->_isalive==true)
		{
			i->_happenTime =0.0f;//道具出现时间置零。//这一句要移到吃完东西的时候才使用
			i->_isalive=false;

			if(i->ID==1)//道具一
				{
					addplane (10);//向飞机链表添加10架飞机
				}
			if (i->ID==2)
				{
					sign=1;//加速道具标记，标记是否被吃到
					if(time>=50)
						time=60;
					else
					time+=10;
				}
			if(i->ID==3)
				sign3=1;
		}
	}
}
//==============================================================================
//道具一效果渲染函数
//==============================================================================
void GameProps::effect1(D3DXMATRIX *V,float timeDalte)
{
	//添加了10个飞机，那么现在飞机链表里面就有10个元素
	std::list <PLANE>::iterator i;
	D3DXMATRIX matposition;
	D3DXMATRIX matscaling;
	D3DXMatrixScaling(&matscaling,3.0f,3.0f,3.0f);
	if(!_allplane.empty ())
	{
		for(i=_allplane.begin();i!=_allplane.end ();i++)
		{
			if(i->_isalive==true) //如果飞机活着
			{
				D3DXMatrixTranslation(&matposition,i->_position.x,i->_position.y,i->_position.z); //设置飞机出现的位置
				matposition=matscaling*matposition;
				Device->SetTransform (D3DTS_WORLD,&matposition);
				PEffect1->RenderXfile ();//渲染飞机
			}
		}
	}

	//
	//
	//渲染子弹
	std::list <PROBULLET> ::iterator  j;
	if(!_allbullet.empty())
	{
		for(j=_allbullet.begin();j!=_allbullet.end ();j++)
		{
			if (j->_isalive==true)
			{
				D3DXMATRIX setposition,scaling,rotation;
				D3DXMatrixScaling(&scaling,0.01,0.01,0.01);
				D3DXMatrixTranslation(&setposition,j->_position.x,j->_position.y,j->_position.z);
				D3DXMatrixRotationZ(&rotation,D3DX_PI/2.0f);
				setposition=scaling*rotation*setposition;
				Device->SetTransform (D3DTS_WORLD,&setposition);
				PBullet->RenderXfile();//渲染子弹
			}
		}
	}
	//
	//
	//
	updateplane (V,timeDalte);//更新飞机和子弹状态
	removedead ();//移除死亡飞机和子弹
}

//=======================================================================
//移除已经死亡的飞机
//=======================================================================
void GameProps::removedead ()
{
	//从列表中移除死亡的飞机
	std::list <PLANE>::iterator i;
	i=_allplane.begin();
	while(i!=_allplane.end ())
	{
		if(i->_isalive==false)
		i=_allplane.erase (i);//如果飞机死亡，从列表中移除
		else
			i++;
	}

	//从列表中移除死亡的子弹
	std::list <PROBULLET>::iterator j;
	j=_allbullet.begin();
	while(j!=_allbullet.end ())
	{
		if(j->_isalive==false)
			j=_allbullet.erase (j);
		else 
			j++;
	}
}

//========================================================================
//更新飞机和子弹的状态函数
//========================================================================
void GameProps::updateplane (D3DXMATRIX * V,float timeDalte)
{
	//更新飞机状态
	std::list <PLANE>::iterator i;
	for(i=_allplane.begin();i!=_allplane.end ();i++)
	{
		i->_keeptime+=timeDalte;

		//更新飞机位置（实际上只需要更新z就可以了）
		i->_position.x+=500*timeDalte*(i->_look.x);
		i->_position.y+=500*timeDalte*(i->_look.y);
		i->_position.z+=500*timeDalte*(i->_look.z);

		//
		static float time=1.0f;
		time+=timeDalte;
		if((int)time%2==0)
		{
			addbullet(i->_position);
			time=1;
		}
		//

		//飞机飞的时间够长了，状态设为死亡
		if(i->_keeptime>i->_lifetime)
			i->_isalive=false;
	}

	//更新子弹函数
	std::list<PROBULLET> ::iterator j;
	for(j=_allbullet.begin();j!=_allbullet.end ();j++)
	{
		if(j->_isalive=true)
		{
			//更新子弹的位置
			j->_position.x+=300*timeDalte*(j->_speed.x);
			j->_position.y +=300*timeDalte*(j->_speed.y);
			j->_position.z +=300*timeDalte*(j->_speed.z);
		}
		j->_happentime+=timeDalte;
		if(j->_happentime>j->_lifetime)
			{
				j->_isalive=false;
				
				//初始化一个位置并添加至爆炸链表中
				EXPLODE explode;
				explode._position=j->_position;
				explode.sign=0;
				_allbound.push_back(explode);

			}
		if(enetank->Collisiontest (j->_position))
			{
				j->_isalive=false;
				//初始化一个位置并添加至爆炸链表中
				EXPLODE explode;
				explode._position=j->_position;
				explode.sign=0;
				_allbound.push_back(explode);
			}
	}
}

//==========================================================================
//这个函数是用来初始化一架新飞机
//==========================================================================
void GameProps::resetplane (PLANE *plane)
{
	plane->_position.x=d3d::GetRandomFloat (-2000,2000);
	plane->_position.y=300;
	plane->_position.z=d3d::GetRandomFloat (2000,2500);
	plane->_isalive=true;
	plane->_keeptime=0;
	plane->_look.x=0.0f,plane->_look.y =0.0f,plane->_look.z=-1.0f;
	plane->_lifetime=10;
}
//============================================================================
//添加飞机函数
//============================================================================
void GameProps::addplane (int numofplane)
{
	for(int i=0;i<numofplane;i++)
	{
		PLANE plane;
		resetplane(&plane);//初始化一架飞机
		_allplane.push_back (plane);//把这架新的飞机加入飞机链表
	}
}

//============================================================================
//初始化子弹函数
//============================================================================
void GameProps::resetbullet(D3DXVECTOR3 position,PROBULLET* bullet)
{
	bullet->_happentime=0;
	bullet->_isalive=true;
	bullet->_lifetime=1.5;
	bullet->_speed.x=0.0f,bullet->_speed.y=-1,bullet->_speed.z=0.0f;
	bullet->_position.x=position.x,bullet->_position.y=position.y ,bullet->_position.z =position.z;
}
//===========================================================================
//添加子弹函数
//===========================================================================
void GameProps::addbullet(D3DXVECTOR3 position)
{
	PROBULLET bullet;
	resetbullet (position,&bullet);
	_allbullet.push_back(bullet);
}

//=======================================================================
//加速道具函数
//=======================================================================
bool GameProps::Effect2(float timeDelta)
{
	static float overtime=0;
	overtime+=timeDelta;
	if(overtime>speeduptime)
	{
		sign=0;
		overtime=0;
	}
	if(sign==1)
	{
		return true;
	}
	return false;
}

//==========================================================================
//道具爆炸函数
//==========================================================================
void GameProps::renderbound (D3DXMATRIX *V,float timeDelta)
{
	std::list <EXPLODE>::iterator i;
	D3DXMATRIX matposition;
	if(!_allbound.empty ())
	{
		i=_allbound.begin();
		while(i!=_allbound.end ())
		{
			D3DXMatrixTranslation(&matposition,i->_position.x,i->_position.y,i->_position.z);
			Device->SetTransform (D3DTS_WORLD,&matposition);//指定爆炸地点
			explode->RanderExplpde(V,matposition,timeDelta,&(i->sign));
			if(i->sign>24)
				i=_allbound.erase (i);
			else 
				i++;
		}
	}
}

//=====================================================
//检测第三个道具是否被吃到
//=====================================================
bool GameProps::Effect3()
{
	if(sign3==1)
		{
			sign3=0;
			return true;
	}
	return false;
}

//=======================================================
//绘制时间函数
//这个函数除了绘制时间外，其返回值代表了时间是否用光（游戏是不是应该结束）
//
//=======================================================

bool GameProps::rendertime (float timeDelta)
{

	time-=timeDelta;
	if(time<=1)
	{
		return true;//表示游戏应该结束了
	}
	else
	{
	//开启融合
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	Device->SetTexture(0,TimeTex[(int)time]);
	Device->SetStreamSource(0, Vb, 0, sizeof(TIME));
	Device->SetFVF(D3DFVF_TIME);
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	
	//关闭融合
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	return false ;//表示时间还剩余
	}
	
}