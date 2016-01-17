#include"Snake.h"
#include"Props.h"
extern Props *props;
//==============================================================================================================================
//初始化函数，在这个函数里，创建了蛇头的模型，指定游戏开始时蛇头的位置和
//行走方向，并将蛇头加入节点链表中
//==============================================================================================================================
bool Snake::initSnake()
{
	d3d::BoundingBox boundbox;
	boundbox._min	=	D3DXVECTOR3(-500.0f, 0.0f, -500.0f);
	boundbox._max	=	D3DXVECTOR3(500.0f,  300.0f,  500.0f);
	snow= new Snow(&boundbox , 100);
	snow->init(Device,L"花1.png");

	//蛇头
	Psnake=new XFileMesh(Device);
	Psnake->InitXfile (L"CAICHONG.x");//初始化蛇头模型
	//创建一种字体
	D3DXCreateFont (Device,25,12,1000,0,true,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,0,L"宋体",&PFont);
	//创建纹理
	D3DXCreateTextureFromFile(Device,L"1.jpg",&tex[0]);//蛇头纹理

	D3DXVECTOR3 position;
	position.x =0.0f,position.y=200.0f,position.z =0.0f;//初始化蛇头的位置

	D3DXVECTOR3 direction;
	direction.x =0.0f,direction.y=0.0f,direction.z=1.0f;//蛇头向z轴正方向行走
	
	SNAKE snake;//定义一个节点实例
	snake.direction=direction;
	snake.position=position;

	_allSnakeNode.push_back (snake);//将蛇头加入节点链表中

	score =0;//游戏得分先置零。

	WallRange=500.0f;//围墙的大小设置为围墙中心到墙壁为500.

	timeOfSpeed =20; //加速的时间为0；

	hadAddScore = false ; //一开始没有吃到加分道具

	speedEnable =0;//一开始没有吃到加速道具

	speedTime =0;//加速状态的持续时间为0

	expandwall=false;

	isalive	= true ;

	return true;
}
//==============================================================================================================================
//更新函数
//==============================================================================================================================
void Snake::upDateSanke (float timeDelta,DInputClass* DInput)
{


	DInput->GetInput (); 
	std::list <SNAKE>::iterator node;
	std::list <TURNING>::iterator turning;

	//*********静态变量time，控制蛇头自动行走*********
	static float time ;
	time+=timeDelta;
	static int signofaddturning=0;

	//********对应按键改变蛇的行走方向*****************
	node=_allSnakeNode.begin ();

	if(DInput->IsKeyDown(DIK_A))
		{node->direction.x=-1.0f;node->direction.y =0.0f;node->direction.z=0.0f;signofaddturning=1;}
	if(DInput->IsKeyDown(DIK_D))
		{node->direction.x=1.0f;node->direction.y =0.0f;node->direction.z=0.0f;signofaddturning=1;}
	if(DInput->IsKeyDown(DIK_S))
		{node->direction.x=0.0f;node->direction.y =-1.0f;node->direction.z=0.0f;signofaddturning=1;}
	if(DInput->IsKeyDown(DIK_W))
		{node->direction.x=0.0f;node->direction.y =1.0f;node->direction.z=0.0f;signofaddturning=1;}
	if(DInput->IsKeyDown (DIK_J))
		{node->direction.x =0.0f;node->direction .y=0.0f;node->direction.z =1.0f;signofaddturning=1;}
	if(DInput->IsKeyDown (DIK_K))
		{node->direction .x=0.0f;node->direction.y =0.0f;node->direction.z=-1.0f;signofaddturning=1;}
	//*************更新转折点链表**********************
	if(signofaddturning ==1)//如果有转向
	{
		if(_allTurning.empty ()) //如果链表是空的，则直接把转向点添加到链表中
				
			{
				if(_allSnakeNode.size ()>1)
				addTurning(node->position,node->direction);
			}	
		else  
			{
				turning=--_allTurning.end ();
				if(node->direction!=turning->direction&&_allSnakeNode.size ()>1) //如果链表不是空的，为了避免重复添加同一方向的转折点，用一个条件来
				{															//且只有蛇头的时候，不必添加转折点
					addTurning(node->position,node->direction);//向转向链表中添加一个元素
					signofaddturning=0;
				}
			}
	}

	//************进行蛇头与道具的碰撞函数***************
	int i=props->Collision(node->position);
	if(i==ADDBODY)  //如果吃到了增加长度的道具
	{
		node=--_allSnakeNode.end ();//获取最后一颗身体
		addSnake (node->position,node->direction);//增加一个虫身
		if(speedEnable == 1)
			score+=2;
		else
			score++;//得分加1
	}
	
	//*******************判断碰撞结果并作相应的处理************************
	else if(i==SPEEDUP)
	{
			speedEnable = 1 ; //加速标记打开
			speedTime = 0;
	} 

	else if(i==CUTDOWN) //切断道具
	{
		std::list <SNAKE>::iterator node;
		node=--_allSnakeNode.end ();//node获取为最后一个节点
		if(_allSnakeNode.size ()>3)
		{
			for(int i=1;i<=3;i++)
			{
				node=_allSnakeNode.erase (node);
				node--;
			}
		}
	}
	else if (i==ADDSCORE )//加分道具
	{
		score+=10;
		hadAddScore=true;
	}
	else if (i==NOADDSCORE)	 //吃不到加分道具
	{
		if(score>=2)
			score-=2;
	}
	else if (i==EXPAND)
	{
		expandwall=true;
	}
	else if (i==DEAD)//如果吃到蜘蛛网道具，则贪吃蛇为死亡状态
	{
		isalive=false; 
	}

	//**************加速键的设置*************************
	if(DInput->IsKeyDown(DIK_SPACE)&& speedEnable == 0 )
		speed=0.1;
	else if ( speedEnable ==0 ) 
		speed=1.0f/(_allSnakeNode.size ()*5);//根据蛇的节点数来决定速度

	if(time>speed)	//每个单位时间前进一次
	{
		time =0; //先将时间置0；
		for(node=_allSnakeNode.begin();node!=_allSnakeNode.end ();node++)
		{
			node->position.x +=10*node->direction.x;
			node->position.y +=10*node->direction.y;
			node->position.z +=10*node->direction.z;
		}
	}	

	if(hadAddScore==true )
	{
		snow->update (timeDelta);
	}

	//************更新每个节点在转向点的转向********************
	node=_allSnakeNode.begin ();			//蛇头
	std::list <SNAKE>::iterator lastNode;
	lastNode=--_allSnakeNode.end ();		//蛇尾

	while(node!=_allSnakeNode.end ())
	{
		turning=_allTurning.begin ();
		while (turning!=_allTurning.end ())
		{
			if(node->position==turning->position)
				node->direction=turning->direction;
			if (turning->position==lastNode->position)//如果蛇的最后一个节点也走过了转折点，那么该转折点的信息应该删除
				{
					lastNode->direction=turning->direction;
					turning=_allTurning.erase (turning);	
				}	
			else
				turning++;
		}
		node++;
	}

	//**********************************判断蛇是否死亡***********************************************************
	isDead(WallRange);

}
//=============================================================================================================================
//渲染函数
//=============================================================================================================================
void Snake::Render()
{
	//渲染蛇
	RenderSnake();

	//如果当前为加速状态，则渲染时间
	if(speedEnable == 1)
	{
		if(speedTime < timeOfSpeed )
		{
			speed	=	0.01f;
			renderTime((int)timeOfSpeed-(int)speedTime );
		}
		else 
		{
			speedEnable = 0;
			speedTime = 0;
		}
		speedTime += 0.1 ;
	}

	//如果吃到加分道具 ,渲染相应的效果
	if(hadAddScore==true)
	{
		static float time = 0;
		static float timeAddScore = 10;
		if(time < timeAddScore )
		{
			snow->render ();
		}
		else 
		{
			hadAddScore	=	false ;
			time =0;
		}

		time += 0.1;
	}
}

//==============================================================================================================================
//蛇的死亡情况有两种：1.撞墙 判断方法为确定蛇头的坐标是否超过围墙坐标
//					  2.撞到自身 通过蛇头坐标与蛇身坐标作比较来判断
//==============================================================================================================================
void Snake::isDead (float range)
{
	std::list <SNAKE>::iterator snakehead,i;
	snakehead=_allSnakeNode.begin ();
	//撞墙
	if(snakehead->position.x >=range||snakehead->position.x <=-range||snakehead->position.y >=WallRange||snakehead->position.y <=-range||snakehead->position.z >=range||snakehead->position.z <=-range)
		isalive=false;
	//撞到自己
	for (i=++_allSnakeNode.begin ();i!=_allSnakeNode.end ();i++)
	{
		if(i->position==snakehead->position)
			isalive=false ;
	}
} 


//==========================================================================================================================
//判断蛇是否处于危险的地带
//==========================================================================================================================
bool Snake::isDangerous (float range)
{
	std::list <SNAKE>::iterator snakehead;
	snakehead=_allSnakeNode.begin ();
	if(snakehead->position.x >=range-50||snakehead->position.x <=-range+50||snakehead->position.y >=range-50||snakehead->position.y <=-range+50||snakehead->position.z >=range-50||snakehead->position.z <=-range+50)
		return true ;	//有危险
	return false ;		//无危险
}


//============================================================================================================================
//计算得分函数
//============================================================================================================================
wchar_t*  Snake::getScore()
{
	char b[10]; 
	int Score=score*100;
	int i,j=1;
	if(Score==0)
			b[0]=48;
	else
		{
			for(i=1;Score/i!=0;i*=10);
			for(j=0,i/=10;i!=0;j++)
			{
				b[j]=Score/i+48;
				Score=Score-(Score/i)*i;
				i/=10;			
			}
		}
	b[j]='\0';
	DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, b, -1, NULL, 0);
	a = new wchar_t[dwNum]; 
	MultiByteToWideChar (CP_ACP, 0, b,(strlen(b) + 1), a, dwNum);
	return a;
}


//*****************************渲染蛇身函数**********************************************************************************
void Snake::RenderSnake ()
{

	delete []a; //在这里先释放掉getScore（）函数中申请的内存，以免造成内存泄漏

	D3DXMATRIX rotation;
	D3DXMatrixRotationY(&rotation,0);
	std::list <SNAKE>::iterator node;
	node=_allSnakeNode.begin ();


	//设置世界变换矩阵，使蛇头的朝向正确
	if(node->direction.x ==-1)
		D3DXMatrixRotationY(&rotation,D3DX_PI/2.0f);
	else if(node->direction.x ==1)
		D3DXMatrixRotationY(&rotation,-D3DX_PI/2.0f);
	else if(node->direction.y ==1)
		D3DXMatrixRotationX(&rotation,D3DX_PI/2.0f);
	else if(node->direction .y ==-1)
		D3DXMatrixRotationX(&rotation,-D3DX_PI/2.0f);
	else if (node->direction.z==1)
		D3DXMatrixRotationY(&rotation,-D3DX_PI);

	//绘制蛇身
	std::list <TURNING >::iterator  turning;
	for(node=_allSnakeNode.begin ();node!=_allSnakeNode.end ();node++)
	{
		D3DXMATRIX position;
		D3DXMATRIX scaling;
		D3DXMatrixTranslation(&position,node->position.x,node->position.y,node->position.z);
		position=rotation*position;
		Device->SetTransform (D3DTS_WORLD,&position);
		Device->SetTexture (0,tex[0]);
		Psnake->RenderXfile ();
	}

}

//******************内部调用函数*********增加一节身体***************************************************************************
void Snake::addSnake (D3DXVECTOR3 position,D3DXVECTOR3 direction)
{
	SNAKE snake;
	snake.direction=direction;
	snake.position.x =position.x-50*direction.x ,snake.position.y =position.y -50*direction.y ,snake.position.z=position.z -50*direction.z;
	_allSnakeNode.push_back (snake);
}

//******************内部调用函数*********增加一个转折点**************************************************************************
void Snake::addTurning(D3DXVECTOR3 position,D3DXVECTOR3 direction)
{
	TURNING turn ;
	turn.position=position;
	turn.direction=direction;
	_allTurning.push_back (turn);//增加一个链表元素
}

//********************获取蛇头位置和方向*****************************************************************************************
void Snake::getHeadPos (D3DXVECTOR3 * position)
{
	std::list<SNAKE>::iterator i;
	i=_allSnakeNode.begin ();
	*position=i->position;
	(*position).y +=30;
}
//****************************************渲染时间函数***************************************************************************
void Snake::renderTime (int time )
{
	wchar_t * a=NULL;
	char b[20]={"SpeedUpTime:"};
	int i,j=1;
	if(time==0)
			b[0]=48;
	else
		{
			for(i=1;time/i!=0;i*=10);
			for(j=12,i/=10;i!=0;j++)
			{
				b[j]=time/i+48;
				time=time-(time/i)*i;
				i/=10;			
			}
		}
	b[j]='\0';

	//将char* 型的b转换为wchar_t*的a
	DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, b, -1, NULL, 0);
	a = new wchar_t[dwNum]; 
	MultiByteToWideChar (CP_ACP, 0, b,(strlen(b) + 1), a, dwNum); //把char型的b转换为wchar_t 型

	//定义一个矩形
	RECT rect;
	rect.left=400,rect.top=50,rect.right=800,rect.bottom=600;
	//绘制文字
	PFont->DrawText(NULL,a,-1,&rect,DT_LEFT|DT_NOCLIP|DT_SINGLELINE,D3DCOLOR_XRGB(255,255,255));

	delete []a ;
}

//============================================================================================================================
//修改围墙大小的函数
//============================================================================================================================
void Snake ::changeWallRange  (Ground *ground)
{
	if(expandwall==true)
	{
		ground->wallRang=1000.0f;
		expandwall=false;
	}
	if(ground->wallRang >500)
	{
		ground->wallRang -=0.1;
	}
}
//========================================================================================================================
//判断蛇是否死亡，预计会在主函数被调用
//========================================================================================================================
bool Snake ::ifDead ()
{
	if(isalive==true)
		return false;
	else 
		return true ;
}
//=============================================================================================================================
//
//绘制分数函数
//=============================================================================================================================

Snake::~Snake()//析构函数
{
	delete Psnake;
}