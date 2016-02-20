#include"Props.h"
#include<math.h>
#include<stdlib.h>
#include"CameraClass.h"
extern CamerClass Camera;

//纹理动画公告板的顶点灵活格式
const DWORD Props::vertex::FVF =D3DFVF_XYZ|D3DFVF_TEX1;

//*****************初始化道具函数*********************
bool Props::initProps()
{
	//创建字体
	D3DXCreateFont (Device,25,12,1000,0,true,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,0,L"宋体",&PFont);

	//初始化公告板道具一
	prop1=new BillBoard(Device);
	prop1->InitBoard(L"RedApple_archigraphs.png");
//
	//初始化公告板道具二
	prop2=new BillBoard(Device);
	prop2->InitBoard (L"RedDaisy_archigraphs.png");

	//初始化公告板道具三
	prop3=new BillBoard(Device);
	prop3->InitBoard (L"WhiteDaisy_archigraphs.png");

	//初始化公告板道具四
	prop4=new BillBoard(Device);
	prop4->InitBoard (L"cock.png");

	//道具五的纹理贴图文件名数组
	wchar_t Nameprop5tex[8][13]=
	    {
			{ L"prop51.bmp" } ,{ L"prop52.bmp" },{ L"prop53.bmp" },{ L"prop54.bmp "},
			{ L"prop55.bmp" } ,{ L"prop56.bmp" },{ L"prop57.bmp" },{ L"prop58.bmp" },
	    };

	//道具六的纹理贴图文件名数组
	wchar_t Nameprop6tex[8][13]=
		{
			{ L"prop71.bmp" } ,{ L"prop72.bmp" },{ L"prop73.bmp" },{ L"prop74.bmp "},
			{ L"prop75.bmp" } ,{ L"prop76.bmp" },{ L"prop77.bmp" },{ L"prop78.bmp" },
		};

	wchar_t Nameprop7tex[8][13]=
	    {
			{ L"prop61.bmp" } ,{ L"prop62.bmp" },{ L"prop63.bmp" },{ L"prop64.bmp "},
			{ L"prop65.bmp" } ,{ L"prop66.bmp" },{ L"prop67.bmp" },{ L"prop68.bmp" },
		};

	//载入道具5/6的动态纹理贴图文件
	for(int i=0 ; i < 8 ; i++)
	{
		D3DXCreateTextureFromFile( Device, Nameprop5tex[i], &prop5tex[i]);
		D3DXCreateTextureFromFile( Device ,Nameprop6tex[i],&prop6tex[i]);
		D3DXCreateTextureFromFile( Device,Nameprop7tex[i],&prop7tex[i]);//道具7的纹理贴图
	}
	
	
	//动态纹理道具的顶点缓存创建
	Device->CreateVertexBuffer (
		4*sizeof(vertex),
		0,
		Props::vertex::FVF,
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

	for(int i=1;i<=7;i++)  //向道具链表中加入道具 
	{
		PROPS prop;
		prop._isalive=true;
		prop._id=i; 
		if(i==1)
			prop._timeDelta=2;//长身体的道具没2秒出现一次
		if(i==2)
			prop._timeDelta=d3d::GetRandomFloat (10,20); //加速道具每10—20秒出现一次
		if(i==3)
			prop._timeDelta=d3d::GetRandomFloat (10,20); //减速道具每10-20秒出现一次
		if(i==4)
			prop._timeDelta=d3d::GetRandomFloat (90,100); //减少节点道具每25-30秒出现一次
		if(i==5)
			prop._timeDelta=d3d::GetRandomFloat	(50,60);
		if(i==6)
			prop._timeDelta=d3d::GetRandomFloat (100,120);
		if(i==7)
			prop._timeDelta=d3d::GetRandomFloat (12,15);//捕捉道具每30秒出现一次

		prop._timeHappen=0;
		prop._position.x =d3d::GetRandomFloat (-400,400);
		prop._position.y =d3d::GetRandomFloat (50,400);
		prop._position.z =d3d::GetRandomFloat (0,400);
		_allprops.push_back (prop); 
	}

	prop5TimeLeft=30;  //道具5限时
	prop5TimeExit=0;   //道具5出现的时间	

	prop7TimeLeft = 10;
	prop7TimeExit = 0;

	isaddscore=true;

	propscore  =0; //道具得分为0；

	return true ;
}

//****************更新道具函数******************************
void Props::upDateProps (float timeDelta)
{
	std::list <PROPS>::iterator i;
	for (i=_allprops.begin ();i!=_allprops.end ();i++)
	{
		i->_timeHappen+=timeDelta;
		//如果道具到了该出现的时间，重新设置它的状态和随机位置
		if(i->_timeHappen>i->_timeDelta&&i->_isalive==false)
		{
			i->_position.x=d3d::GetRandomFloat (-400,400);
			i->_position.y=d3d::GetRandomFloat (50,400);
			i->_position.z=d3d::GetRandomFloat (-200,400);
			i->_isalive=true;
			if(i->_id==5)
				isaddscore=false;
		}
	}
}

//**************渲染道具函数*************************
void Props::RenderProps ()
{
	std ::list <PROPS >::iterator i;
	for(i=_allprops.begin ();i!=_allprops.end ();i++)
	{
		if(i->_isalive==true)
		{
			//设置世界变换
			D3DXMATRIX position,scaling,V;
			D3DXMatrixScaling(&scaling,0.5f,0.3f,0.5f);
			D3DXMatrixTranslation(&position,i->_position.x ,i->_position.y ,i->_position.z);
			position=scaling*position;
			Camera.SetViewMatrix(&V);
			if(i->_id==1)
			{
				prop1->RanderBorad ( &V,position);
			}
			if(i->_id==2)
			{
				prop2->RanderBorad (&V,position);
			}
			if(i->_id==3)
			{
				prop3->RanderBorad(&V,position);
			}
			if(i->_id==4)
			{
				D3DXMatrixScaling(&scaling,1.0f,1.0f,1.0f);
				position=scaling*position;
				prop4->RanderBorad (&V,position);
			}
			switch (i->_id)
			{
				case 5:
					{
						prop5TimeExit +=0.1;
						if(prop5TimeLeft-(int)prop5TimeExit >0)
						{
							//指定时间出现的位置
							RECT rect;
							rect.left=400,rect.top=50,rect.right=800,rect.bottom=600;
							//渲染时间
							RenderTime(prop5TimeLeft-(int) prop5TimeExit,rect );
							//渲染动态纹理
							RenderDynaminc(prop5tex,position,V);
						}
						else //这里表示时间到了 没吃到那个道具
						{
							isaddscore=false;	//表示没有吃到那个道具
							i->_isalive=false;	//道具消失
							i->_timeHappen=0;	//道具出现时间置零
							prop5TimeExit=0;	//道具5时间绘制置零
						}
						break;
					}
				case 6:
					{
						RenderDynaminc(prop6tex,position,V);
						break;
					}
				case 7:
					{
						if(prop7TimeLeft-prop7TimeExit>0)
						{
							RenderDynaminc (prop7tex,position,V);
							prop7TimeExit += 0.1;
						}
						else 
						{
							prop7TimeExit =0;
							i->_isalive=false;
							i->_timeHappen=0;
						}
						break;
					}
			}

		}
	}
}


//******************************渲染动态纹理的函数***********************************************************************************************
void Props::RenderDynaminc (IDirect3DTexture9 * proptex[8],D3DXMATRIX position,D3DXMATRIX V)
{
	D3DXMATRIX scaling;
	Device->SetRenderState (D3DRS_ALPHABLENDENABLE,true);//开启alpha融合
	//设置融合系数
	Device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_ONE);
	Device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_ONE);
	D3DXMATRIX matBillboard;  
	D3DXMatrixIdentity(&matBillboard);  
	matBillboard._11 =	V._11;  
	matBillboard._13 =	V._13;  
	matBillboard._31 =	V._31;  
	matBillboard._33 =	V._33;
	D3DXMatrixInverse(&matBillboard, NULL, &matBillboard);  
	/*D3DXMatrixScaling(&scaling,10.0f,10.0f,10.0f);*/
	position =  /*scaling*/matBillboard*position  ;  
	Device->SetTransform(D3DTS_WORLD, &position ); 
	//开始绘制
	static float time =0;
	static float timechance=0.3;
	static int j=0;
	time+=0.1;
	if(time >timechance)
	{
		time = 0;
		j ++;
	}
	Device->SetTexture(0,proptex[j]);
	Device->SetStreamSource(0, Vb , 0, sizeof(vertex) );  
	Device->SetFVF(Props::vertex::FVF);  
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 );  
	Device->SetRenderState (D3DRS_ALPHABLENDENABLE,false);
	if(j==7)
	{
		j=0;
		//break;
	}
}


//**********碰撞函数******蛇与道具的碰撞*********************
int Props::Collision (D3DXVECTOR3 position)
{
	std ::list <PROPS>::iterator  i;
	double distance=50.0f;
	for(i=_allprops.begin ();i!=_allprops.end ();i++)
	{
		if(sqrt(    pow(i->_position.x-position.x,2)   +  pow(i->_position.y-position.y,2)
			+pow(i->_position.z-position.z,2)  )  <distance&&i->_isalive==true)
		{
			propscore ++;//道具得分加1

			i->_isalive=false;
			i->_timeHappen=0;
			if(i->_id==1)
				return ADDBODY;		//增加节点
			if(i->_id==2)
				return SPEEDUP;		//加速
			if(i->_id==3)
				return SPEEDDOWN;	//降速
			if(i->_id==4)
				return CUTDOWN;		//切断身体长度
			if(i->_id==5)			//增加分数
			{
				prop5TimeExit=0;
				return ADDSCORE;
			}
			if(i->_id==6)
			{
				return EXPAND;
			}
			if(i->_id==7)
			{
				return DEAD;
				//蛇的状态判为死亡
			}
		}
		if(isaddscore==false)
		{
			isaddscore=true;
			return NOADDSCORE;
		}

	}

	return 0;
}

//---------------------------------------------------------------------------
//作用：提供道具5倒计时的渲染函数
//类型：Props类的内部调用函数
//备注：被RenderProps()函数调用
//时间：2014.8.26.
//---------------------------------------------------------------------------
void Props::RenderTime(int time,RECT rect )
{
	char b[20]={"Time left :"};
	char c[5];
	wchar_t * a;
	itoa(time,c,10);    //把int转换成为char*
	for(int i=0,j=11;;i++)
	{
		b[j++]=c[i];
		if(c[i]=='\0')
			{
				b[j]='\0';
				break;
			}	
	}
	//将char* 型的b转换为wchar_t*的a
	DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, b, -1, NULL, 0);
	a = new wchar_t[dwNum]; 
	MultiByteToWideChar (CP_ACP, 0, b,(strlen(b) + 1), a, dwNum); //把char型的b转换为wchar_t 型

		//定义一个矩形
		//绘制文字
	PFont->DrawText(NULL,a,-1,&rect,DT_LEFT|DT_NOCLIP|DT_SINGLELINE,D3DCOLOR_XRGB(255,255,255));

	delete []a ;
}