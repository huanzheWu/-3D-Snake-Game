#include"GroundObject.h"
#include"Explode.h"
const DWORD GroundObject::vertex::FVF =D3DFVF_XYZ|D3DFVF_TEX1;//公告版的灵活顶点格式

extern Explode *explode;//把爆炸实例作用域拓展到这里
//构造函数，获取D3D设备指针
GroundObject::GroundObject(LPDIRECT3DDEVICE9 device)
{
	Device=device;
}

//初始化公告板
void GroundObject::InitObject(wchar_t * filename,wchar_t *wallname,int numofboard,int numwall)
{
	//如果不需要公告板，那么也不用去创建缓存、创建纹理接口
	if(numofboard!=0)
	{
		Device->CreateVertexBuffer (
			4*sizeof(vertex),
			0,
			GroundObject::vertex::FVF,
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
		D3DXCreateTextureFromFile(Device,filename, &Tex);
		Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );  
		Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );  
	
		//随机产生公告板链表中公告板的位置信息
		for(int i=0;i<numofboard ;i++)
		{
			BILLBOARD board;
			board._position.x =d3d::GetRandomFloat(-5000,5000);
			board._position.y=-100;
			board._position.z=d3d::GetRandomFloat(-5000,5000);
			_allBillBoard.push_back (board);//向链表中添加一个公告版；
		}
	}
	//随机产生墙壁的位置
	for(int i=0;i<numwall;i++)
	{
		WALL wall;
		wall._position.x=d3d::GetRandomFloat(-5000,5000);
		wall._position.z=d3d::GetRandomFloat(-5000,5000);
		wall._position.y=0;
		_allWall.push_back (wall);
	}
	InitXfile(wallname);//初始化墙壁的x文件
}

//渲染公告板
void GroundObject::RanderObject(D3DXMATRIX *View)
{
	
	//绘制围墙
	std::list <WALL>::iterator j;
	for(j=_allWall.begin();j!=_allWall.end ();j++)
	{
		D3DXMatrixTranslation(&MatWall,j->_position.x,j->_position.y-160,j->_position.z);
		D3DXMATRIX rotation,scaling;
		D3DXMatrixRotationY(&rotation,D3DX_PI/2.0f);//旋转90度
		D3DXMatrixScaling(&scaling,3.0f,3.0f,3.0f);
		MatWall=scaling*rotation*MatWall;
		Device->SetTransform (D3DTS_WORLD,&MatWall);
		RenderXfile();
	}

	//如果没有公告板，则不必进行下面操作
	if(!_allWall.empty ())
	{
		Device->SetRenderState (D3DRS_ALPHABLENDENABLE,true);//开启alpha融合
		//设置融合系数
		Device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		Device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		D3DXMATRIX matBillboard;  
		D3DXMatrixIdentity(&matBillboard);  
		matBillboard._11 =(*View)._11;  
		matBillboard._13 =(*View)._13;  
		matBillboard._31 =(*View)._31;  
		matBillboard._33 =(*View)._33;
		D3DXMatrixInverse(&matBillboard, NULL, &matBillboard);

		//绘制树木
		std::list <BILLBOARD>::iterator i;
		for(i=_allBillBoard.begin ();i!=_allBillBoard.end ();i++)
		{
			//MatNPC=matNPC; 
			D3DXMatrixTranslation(&MatNPC ,i->_position.x,i->_position.y-20,i->_position.z);
			MatNPC =  matBillboard*MatNPC  ;  
			Device->SetTransform(D3DTS_WORLD, &MatNPC ); 
			//开始绘制
			Device->SetTexture(0, Tex);  
			Device->SetStreamSource(0, Vb , 0, sizeof(vertex) );  
			Device->SetFVF(GroundObject::vertex::FVF);  
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 );  
		}
	}
	Device->SetRenderState (D3DRS_ALPHABLENDENABLE,false);
}

//===================================================================================
//坦克和地面物体（树木、墙壁）碰撞检测
//因为无论是于树木还是墙壁碰撞，碰撞结果为真都是旋转坦克，所以两种碰撞可以写在一起。
//而子弹与墙壁的碰撞结果是，如果碰撞，子弹状态为“死亡”，要另写一个碰撞函数
//====================================================================================
bool GroundObject::BCollisiontest(D3DXVECTOR3 _position)
{
	double distance;
	if(!_allBillBoard.empty())
	{
		//坦克与树木的碰撞检测
		std::list<BILLBOARD>::iterator i;
		for(i=_allBillBoard.begin();i!=_allBillBoard.end ();i++)
		{
			distance=sqrt(pow((i->_position.x-_position.x),2)+pow((i->_position.y-_position.y),2)+pow((i->_position.z-_position.z),2));
			if(distance<5)
				{
					//初始化一个爆炸位置
					EXPLODEB explode;
					explode._position=i->_position;
					explode.sign=0;
					explode._position.y=i->_position.y -50;
					_allbound.push_back (explode);//向爆炸位置列表中加入一个元素
					return true;
				}
		}
	}

	//坦克与墙壁的碰撞检测
	double range;
	if(!_allBillBoard.empty())//如果公告板是空的，那么证明我使用的是房屋x文件，这时它的碰撞范围应该大一点
		range=50.0f;
	else 
		range=20.0f;
	std::list <WALL>::iterator j;
	for(j=_allWall.begin ();j!=_allWall.end ();j++)
	{
		distance=sqrt(pow((j->_position.x-_position.x),2)+pow((j->_position.y-_position.y),2)+pow((j->_position.z-_position.z),2));
		if(distance<range)
			{
					EXPLODEB explode;
					explode._position=j->_position;
					explode._position.y=j->_position.y -50;
					explode.sign=0;
					_allbound.push_back (explode);//向爆炸位置列表中加入一个元素
				return true;
			}	
	}
	return false;
}

//=========================================================================================================================
//子弹与地面物体的碰撞函数
//
//=========================================================================================================================
bool GroundObject::BTCollisiontest(D3DXVECTOR3 _position)
{
	double distance;
	std::list <WALL>::iterator i;
	for(i=_allWall.begin();i!=_allWall.end ();i++)
	{
		distance=sqrt(pow((i->_position.x-_position.x),2)+pow((i->_position.y-_position.y),2)+pow((i->_position.z-_position.z),2));
		if (distance<300)
			return true;
	}
	return false;
}


//==================================================================================================
//渲染子弹与地面物体碰撞时的爆炸效果的函数
//==================================================================================================
void GroundObject::renderbound (D3DXMATRIX *V,float timeDelta)
{
	std::list <EXPLODEB>::iterator i;
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

//析构函数
GroundObject::~GroundObject()
{
	if(!_allBillBoard.empty ())
		{
			Vb->Release ();
			Tex->Release ();
		}
}




