#include"Snake.h"
#include"Props.h"
extern Props *props;
//==============================================================================================================================
//��ʼ��������������������������ͷ��ģ�ͣ�ָ����Ϸ��ʼʱ��ͷ��λ�ú�
//���߷��򣬲�����ͷ����ڵ�������
//==============================================================================================================================
bool Snake::initSnake()
{
	d3d::BoundingBox boundbox;
	boundbox._min	=	D3DXVECTOR3(-500.0f, 0.0f, -500.0f);
	boundbox._max	=	D3DXVECTOR3(500.0f,  300.0f,  500.0f);
	snow= new Snow(&boundbox , 100);
	snow->init(Device,L"��1.png");

	//��ͷ
	Psnake=new XFileMesh(Device);
	Psnake->InitXfile (L"CAICHONG.x");//��ʼ����ͷģ��
	//����һ������
	D3DXCreateFont (Device,25,12,1000,0,true,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,0,L"����",&PFont);
	//��������
	D3DXCreateTextureFromFile(Device,L"1.jpg",&tex[0]);//��ͷ����

	D3DXVECTOR3 position;
	position.x =0.0f,position.y=200.0f,position.z =0.0f;//��ʼ����ͷ��λ��

	D3DXVECTOR3 direction;
	direction.x =0.0f,direction.y=0.0f,direction.z=1.0f;//��ͷ��z������������
	
	SNAKE snake;//����һ���ڵ�ʵ��
	snake.direction=direction;
	snake.position=position;

	_allSnakeNode.push_back (snake);//����ͷ����ڵ�������

	score =0;//��Ϸ�÷������㡣

	WallRange=500.0f;//Χǽ�Ĵ�С����ΪΧǽ���ĵ�ǽ��Ϊ500.

	timeOfSpeed =20; //���ٵ�ʱ��Ϊ0��

	hadAddScore = false ; //һ��ʼû�гԵ��ӷֵ���

	speedEnable =0;//һ��ʼû�гԵ����ٵ���

	speedTime =0;//����״̬�ĳ���ʱ��Ϊ0

	expandwall=false;

	isalive	= true ;

	return true;
}
//==============================================================================================================================
//���º���
//==============================================================================================================================
void Snake::upDateSanke (float timeDelta,DInputClass* DInput)
{


	DInput->GetInput (); 
	std::list <SNAKE>::iterator node;
	std::list <TURNING>::iterator turning;

	//*********��̬����time��������ͷ�Զ�����*********
	static float time ;
	time+=timeDelta;
	static int signofaddturning=0;

	//********��Ӧ�����ı��ߵ����߷���*****************
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
	//*************����ת�۵�����**********************
	if(signofaddturning ==1)//�����ת��
	{
		if(_allTurning.empty ()) //��������ǿյģ���ֱ�Ӱ�ת�����ӵ�������
				
			{
				if(_allSnakeNode.size ()>1)
				addTurning(node->position,node->direction);
			}	
		else  
			{
				turning=--_allTurning.end ();
				if(node->direction!=turning->direction&&_allSnakeNode.size ()>1) //��������ǿյģ�Ϊ�˱����ظ����ͬһ�����ת�۵㣬��һ��������
				{															//��ֻ����ͷ��ʱ�򣬲������ת�۵�
					addTurning(node->position,node->direction);//��ת�����������һ��Ԫ��
					signofaddturning=0;
				}
			}
	}

	//************������ͷ����ߵ���ײ����***************
	int i=props->Collision(node->position);
	if(i==ADDBODY)  //����Ե������ӳ��ȵĵ���
	{
		node=--_allSnakeNode.end ();//��ȡ���һ������
		addSnake (node->position,node->direction);//����һ������
		if(speedEnable == 1)
			score+=2;
		else
			score++;//�÷ּ�1
	}
	
	//*******************�ж���ײ���������Ӧ�Ĵ���************************
	else if(i==SPEEDUP)
	{
			speedEnable = 1 ; //���ٱ�Ǵ�
			speedTime = 0;
	} 

	else if(i==CUTDOWN) //�жϵ���
	{
		std::list <SNAKE>::iterator node;
		node=--_allSnakeNode.end ();//node��ȡΪ���һ���ڵ�
		if(_allSnakeNode.size ()>3)
		{
			for(int i=1;i<=3;i++)
			{
				node=_allSnakeNode.erase (node);
				node--;
			}
		}
	}
	else if (i==ADDSCORE )//�ӷֵ���
	{
		score+=10;
		hadAddScore=true;
	}
	else if (i==NOADDSCORE)	 //�Բ����ӷֵ���
	{
		if(score>=2)
			score-=2;
	}
	else if (i==EXPAND)
	{
		expandwall=true;
	}
	else if (i==DEAD)//����Ե�֩�������ߣ���̰����Ϊ����״̬
	{
		isalive=false; 
	}

	//**************���ټ�������*************************
	if(DInput->IsKeyDown(DIK_SPACE)&& speedEnable == 0 )
		speed=0.1;
	else if ( speedEnable ==0 ) 
		speed=1.0f/(_allSnakeNode.size ()*5);//�����ߵĽڵ����������ٶ�

	if(time>speed)	//ÿ����λʱ��ǰ��һ��
	{
		time =0; //�Ƚ�ʱ����0��
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

	//************����ÿ���ڵ���ת����ת��********************
	node=_allSnakeNode.begin ();			//��ͷ
	std::list <SNAKE>::iterator lastNode;
	lastNode=--_allSnakeNode.end ();		//��β

	while(node!=_allSnakeNode.end ())
	{
		turning=_allTurning.begin ();
		while (turning!=_allTurning.end ())
		{
			if(node->position==turning->position)
				node->direction=turning->direction;
			if (turning->position==lastNode->position)//����ߵ����һ���ڵ�Ҳ�߹���ת�۵㣬��ô��ת�۵����ϢӦ��ɾ��
				{
					lastNode->direction=turning->direction;
					turning=_allTurning.erase (turning);	
				}	
			else
				turning++;
		}
		node++;
	}

	//**********************************�ж����Ƿ�����***********************************************************
	isDead(WallRange);

}
//=============================================================================================================================
//��Ⱦ����
//=============================================================================================================================
void Snake::Render()
{
	//��Ⱦ��
	RenderSnake();

	//�����ǰΪ����״̬������Ⱦʱ��
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

	//����Ե��ӷֵ��� ,��Ⱦ��Ӧ��Ч��
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
//�ߵ�������������֣�1.ײǽ �жϷ���Ϊȷ����ͷ�������Ƿ񳬹�Χǽ����
//					  2.ײ������ ͨ����ͷ�����������������Ƚ����ж�
//==============================================================================================================================
void Snake::isDead (float range)
{
	std::list <SNAKE>::iterator snakehead,i;
	snakehead=_allSnakeNode.begin ();
	//ײǽ
	if(snakehead->position.x >=range||snakehead->position.x <=-range||snakehead->position.y >=WallRange||snakehead->position.y <=-range||snakehead->position.z >=range||snakehead->position.z <=-range)
		isalive=false;
	//ײ���Լ�
	for (i=++_allSnakeNode.begin ();i!=_allSnakeNode.end ();i++)
	{
		if(i->position==snakehead->position)
			isalive=false ;
	}
} 


//==========================================================================================================================
//�ж����Ƿ���Σ�յĵش�
//==========================================================================================================================
bool Snake::isDangerous (float range)
{
	std::list <SNAKE>::iterator snakehead;
	snakehead=_allSnakeNode.begin ();
	if(snakehead->position.x >=range-50||snakehead->position.x <=-range+50||snakehead->position.y >=range-50||snakehead->position.y <=-range+50||snakehead->position.z >=range-50||snakehead->position.z <=-range+50)
		return true ;	//��Σ��
	return false ;		//��Σ��
}


//============================================================================================================================
//����÷ֺ���
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


//*****************************��Ⱦ������**********************************************************************************
void Snake::RenderSnake ()
{

	delete []a; //���������ͷŵ�getScore����������������ڴ棬��������ڴ�й©

	D3DXMATRIX rotation;
	D3DXMatrixRotationY(&rotation,0);
	std::list <SNAKE>::iterator node;
	node=_allSnakeNode.begin ();


	//��������任����ʹ��ͷ�ĳ�����ȷ
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

	//��������
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

//******************�ڲ����ú���*********����һ������***************************************************************************
void Snake::addSnake (D3DXVECTOR3 position,D3DXVECTOR3 direction)
{
	SNAKE snake;
	snake.direction=direction;
	snake.position.x =position.x-50*direction.x ,snake.position.y =position.y -50*direction.y ,snake.position.z=position.z -50*direction.z;
	_allSnakeNode.push_back (snake);
}

//******************�ڲ����ú���*********����һ��ת�۵�**************************************************************************
void Snake::addTurning(D3DXVECTOR3 position,D3DXVECTOR3 direction)
{
	TURNING turn ;
	turn.position=position;
	turn.direction=direction;
	_allTurning.push_back (turn);//����һ������Ԫ��
}

//********************��ȡ��ͷλ�úͷ���*****************************************************************************************
void Snake::getHeadPos (D3DXVECTOR3 * position)
{
	std::list<SNAKE>::iterator i;
	i=_allSnakeNode.begin ();
	*position=i->position;
	(*position).y +=30;
}
//****************************************��Ⱦʱ�亯��***************************************************************************
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

	//��char* �͵�bת��Ϊwchar_t*��a
	DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, b, -1, NULL, 0);
	a = new wchar_t[dwNum]; 
	MultiByteToWideChar (CP_ACP, 0, b,(strlen(b) + 1), a, dwNum); //��char�͵�bת��Ϊwchar_t ��

	//����һ������
	RECT rect;
	rect.left=400,rect.top=50,rect.right=800,rect.bottom=600;
	//��������
	PFont->DrawText(NULL,a,-1,&rect,DT_LEFT|DT_NOCLIP|DT_SINGLELINE,D3DCOLOR_XRGB(255,255,255));

	delete []a ;
}

//============================================================================================================================
//�޸�Χǽ��С�ĺ���
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
//�ж����Ƿ�������Ԥ�ƻ���������������
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
//���Ʒ�������
//=============================================================================================================================

Snake::~Snake()//��������
{
	delete Psnake;
}