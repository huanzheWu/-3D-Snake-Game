#pragma once
#include<list>
#include"XFileMesh.h"
#include"d3dUtility.h"
#include"DirectInputClass.h"
#include"Bullet.h"
#include"GroundObject.h"


//敌方坦克的子弹
struct Ebullet
{
		float  _age;//年龄
		float _lifetime;//寿命
		bool _alive;//是否活着
		D3DXVECTOR3 _position;//子弹位置
		D3DXVECTOR3 _speed;//子弹的速度（方向）
		enum Whos{my,enemy};//谁的子弹
};

struct ENEMYTANK
{
	D3DXVECTOR3 _position;//坦克的位置
	D3DXVECTOR3 _look;//坦克观察向量
	bool _isalive;//坦克生存状态
	float _keeptime;//坦克保持着新行走状态的时间
	D3DXMATRIX  _rotation;//坦克的旋转角度
	float _walktime; //表示坦克直线行走的时间段；
	float _launchtime;//多久发射一次子弹
	float _unlaunchtime;//不发子弹的时间间隔
	bool _launch;//是否发射子弹
};

class EnemyTank :public XFileMesh 
{

	friend BULLET;//把子弹类的更新函数设为友元，因为要允许子弹类来访问成员函数Collisiontest(D3DXVECTOR3 _position)；
	public:
		EnemyTank(LPDIRECT3DDEVICE9 _Device);
		~EnemyTank();//在这里考虑析构函数怎么写，回去修改MyTank类

		//=======================================================
		//坦克相关函数
		//=======================================================
		bool InitTank(); //初始化坦克
		void Render();//渲染坦克
		void upDate(float timeDelta);//更新坦克状态
		void resetTank(ENEMYTANK *etank);//重置坦克初始状态
		void addTank();//真假一个坦克
		void removedeadtank();
		//=======================================================
		//子弹相关函数
		//=======================================================
		void InitEbullet ();	//初始化子弹函数 ---在这里使用和我方坦克不一样的子弹
		void resetEbullet(Ebullet *ebullet,D3DXVECTOR3 position,D3DXVECTOR3 look);	//指定一颗新子弹的状态，这里要遍历i，获取true的坦克，指定子弹的位置和方向（内部调用）
		void addEbullet (D3DXVECTOR3 position,D3DXVECTOR3 look);		//但判断true时，用于向子弹链表加入一个子弹（内部调用）
		void UpDateBullet(float timeDelta);	//更新子弹状态，每个子弹在reset时就要接受坦克当时的方向
		void RenderBullet();	//渲染子弹
		void removedead();		//移除死亡的子弹（内部调用）

		//
		//碰撞函数
		//
		
	private :
		
		bool Collisiontest(D3DXVECTOR3 _position);
	    std::list<ENEMYTANK> _allenetank; //坦克链表
		std::list <Ebullet> _Ebullet;//敌方坦克子弹
		float angle;//坦克旋转的角度（随机设定）；
		LPD3DXMESH Bulletmesh;
		LPD3DXBUFFER  eppAdjacency;
		LPD3DXBUFFER  eppMaterials;
		DWORD epNumMaterials;
		LPD3DXMESH eSourceMesh;
		LPD3DXPMESH ePMesh;//进行渐进网格
		D3DMATERIAL9*       eg_pMaterials   ; // 网格的材质信息  
		LPDIRECT3DTEXTURE9* eg_pTextures    ; // 网格的纹理信息 
		double deadline;//死亡距离
		D3DXMATRIX rotation;//坦克碰到围墙时要旋转

};