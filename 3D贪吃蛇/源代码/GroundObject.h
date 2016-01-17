#pragma once
#include"d3dUtility.h"
#include<list>
#include"EnemyTank.h"

class EnemyTank;

struct EXPLODEB
{
	D3DXVECTOR3 _position;//爆炸的位置
	int sign;//dds文件标记
};
struct BILLBOARD
{
	D3DXVECTOR3 _position;
};
struct WALL
{
	D3DXVECTOR3 _position;
};
class GroundObject:public XFileMesh
{
	friend EnemyTank; //把敌方坦克类设为友元，因为敌方坦克需要判断是否撞上树木或者是围墙
	private : 
		std::list <EXPLODEB> _allbound;
		LPDIRECT3DVERTEXBUFFER9 Vb;//顶点索引
		LPDIRECT3DTEXTURE9 Tex;//纹理接口
		D3DXMATRIX MatNPC;//公告板的位置
		std::list <BILLBOARD> _allBillBoard;//公告版链表
		D3DXMATRIX MatWall;//墙的位置
		std::list<WALL> _allWall;//墙的链表


		//公告板的顶点结构
		struct vertex
		{
			vertex(){}
			vertex(float x,float y,float z,float u,float v)
			{
				_x=x;_y=y,_z=z;
				_u=u;_v=v;
			}
			float _x,_y, _z;
			float _u,_v;
			static const DWORD FVF;
		};

	public:
		GroundObject(LPDIRECT3DDEVICE9 dvice);//构造函数
		~GroundObject();//析构函数
		//========================================================
		//地面公告板类函数
		//========================================================
		void InitObject(wchar_t *filename,wchar_t *wallname,int numofboard,int numwall);//初始化公告版 
		void RanderObject(D3DXMATRIX *view);//渲染公告版 //这里准备把D3DXMATRIX matNPC去掉，直接随机生成位置
		bool BCollisiontest(D3DXVECTOR3 _position); //判断坦克与地面物体是否会产生碰撞；
		bool BTCollisiontest(D3DXVECTOR3 _position );//判断子弹与地面物体是否会产生碰撞；//不和上面的碰撞测试同一个函数的原因是，得到碰撞结果后处理情况不同
		void renderbound(D3DXMATRIX *V,float timeDelta);//绘制爆炸函数
};