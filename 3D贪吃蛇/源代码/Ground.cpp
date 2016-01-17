#include"Ground.h"

//********��ʼ������***************************
bool Ground::initObject ()
{
	//����Χǽ������
	D3DXCreateTextureFromFile (Device,L"ice.bmp",&Tex[0]);
	Wall=new XFileMesh(Device);
	//��ʼ��Χǽģ��
	Wall->InitXfile (L"shu.x");

	//�����������
	D3DXCreateTextureFromFile(Device,L"dmentia_bluebird.bmp",&Tex[1]);
	Bird=new XFileMesh(Device);
	//��ʼ����ģ��
	Bird->InitXfile (L"niao.x");

	//����ǽ�ڵ����ž���
	D3DXMatrixScaling(&WallSca,1000.0f,1000.0f,1000.0f);
	//����������ž���
	D3DXMatrixScaling(&BirdSca,  10.0f,  10.0f,   10.0f);

	//��ķ��з���1
	BirdDir1.x =0.0f,BirdDir1.y=0.0f,BirdDir1.z =1.0f;	//���з���1
	BirdDir2.x =0.0f,BirdDir2.y=0.0f,BirdDir2.z =-1.0f;	//���з���2

	wallRang=500.0f;

	return true;
}

//******************************************����Χǽ����**********************************************************
bool Ground ::createWall(D3DXVECTOR3 centre)
{
	float i;
	D3DXVECTOR3 position;
	_allWalls.clear ();
	//ָ��Χǽ��λ��
	for (i=centre.x -wallRang;i<=centre.x+wallRang;i+=100)  //ǰ
	{
		position.y =0.0f,position.z =wallRang,position.x=i;
		_allWalls.push_back (position);
	}
	for (i=centre.z-wallRang;i<=centre.z +wallRang;i+=100)//��
	{
		position.x=-wallRang,position.y=0.0f,position.z =i;
		_allWalls.push_back (position);
	}
	for(i=centre.z -wallRang;i<=centre.z+wallRang;i+=100 )//��
	{
		position.x =wallRang,position.y =0.0f,position.z =i;
		_allWalls.push_back (position);
	}
	for (i=centre.x -wallRang;i<=centre.x+wallRang;i+=100)//��
	{
		position.y =0.0f,position.z =-wallRang,position.x=i;
		_allWalls.push_back (position);
	}
	return true;
}

//******************************************************************������ĺ���********************************************************
bool Ground ::createBird (D3DXVECTOR3 centre)
{
	//ָ�����λ��

	BIRD bird;
	float i;
	static int j=10,k=10;
	for (i=centre.x -500;i<=centre.x +500;i +=100)
	{
		bird._direction=BirdDir2; //ָ����ķ��з���
		bird._position.x =i;bird._position.z = 500+j;bird._position.y =360;//ָ����ĳ�ʼλ��
		_allBird.push_back (bird);
		if(i<0)
			j+=50;
		else 
			j-=50;
	}
	for(i=centre.x-450;i<=centre.x + 500;i+=100)
	{
		bird._direction=BirdDir1;//ָ����ķ��з���
		bird._position.x =i;bird._position.z =-500+k;bird._position.y =360;//ָ����ĳ�ʼλ��
		_allBird.push_back (bird);
		if(i<0)
			k-=50;
		else 
			k+=50;
	}
	D3DXMatrixRotationY(&BirdRota,0);
	return true;
}

//********************************************************������״̬�ĺ���**************************************************************

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
				ifrota=true;//ÿ10��תһ����
			else 
				ifrota=false;
			rota++;
		}
	}
}



//***********��Ⱦ�������庯��*******************
void Ground::renderGround ()
{
	//����Χǽ
	std::list <D3DXVECTOR3 >::iterator i;
	for(i=_allWalls.begin ();i!=_allWalls.end ();i++)
	{
		//����������󣬰�Χǽ�������ʵ���λ��
		D3DXMatrixTranslation(&WallPos,i->x ,i->y ,i->z);
		WallPos=WallSca*WallPos;
		Device->SetTransform (D3DTS_WORLD,&WallPos);
		//��������
		Device->SetTexture (0,Tex[0]);
		//����Χǽx�ļ�
		Wall->RenderXfile ();
	}

	//������
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

//**************************************************��������************************************
Ground::~Ground()
{

}