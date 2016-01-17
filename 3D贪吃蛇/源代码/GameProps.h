#pragma once
#include"d3dUtility.h"
#include"XFileMesh.h"
#include<list>
#include"Mytank.h"
#include"EnemyTank.h"

//时间的顶点格式
struct TIME
{
	TIME(){}
	TIME(float _x,float _y,float _z,float _rhw,unsigned long _color,float _tu,float _tv)
	{
		x=_x,y=_y,z=_z,rhw=_rhw,color=_color,tu=_tu,tv=_tv;
	}
	float x, y, z, rhw;
	unsigned long color;
	float tu, tv;
};

//爆炸结构体
struct EXPLODE
{
	D3DXVECTOR3 _position;
	int sign;//标记当前对应的公告板应该绘制那一张DDS文件
};
struct PROBULLET //子弹结构体
{
	D3DXVECTOR3 _position;//位置
	D3DXVECTOR3 _speed;//速度方向
	float _isalive;//是否活着
	float _happentime;//已经发射时间
	float _lifetime;//子弹存活时间
};

struct PROPS  //道具结构体
{
	int ID;//标记是哪一种道具
	D3DXVECTOR3 _position; //道具出现的位置
	float _timeDelta; //道具出现的间隔时间
	float _happenTime; //道具刚被吃掉的时间
	wchar_t _fileName; //道具x文件名称
	bool _isalive; //是否活着（还没被吃掉）
};


struct PLANE//飞机结构体
{
	D3DXVECTOR3 _position;//飞机的位置
	D3DXVECTOR3 _look;//飞机的前进方向
	float _lifetime;//飞机寿命（控制飞行的距离）
	float _keeptime;//出现的时间
	bool _isalive;//是否活着
};

class GameProps //继承XFileMesh类
{
	private:
		float time;
		std::list <PLANE> _allplane;//飞机链表
		std::list<PROPS> _allProps;//道具链表
		std::list <EXPLODE> _allbound;
		std::list<PROBULLET> _allbullet;//子弹链表
		XFileMesh *PPops1; //道具一的x文件指针
		XFileMesh *PPops2; //道具二的x文件指针
		XFileMesh *PPops3;//道具三的x文件指针
		XFileMesh *PEffect1; //第一种道具对应的效果（飞机轰炸）的x文件模型
		XFileMesh *PBullet ;//子弹x文件指针
		LPDIRECT3DDEVICE9 Device;

		LPDIRECT3DVERTEXBUFFER9 Vb;//顶点缓存
		LPDIRECT3DTEXTURE9 TimeTex[61];//纹理接口（60个，保存时间）

		int sign;//一个标记，标记加速道具是否被吃到
		int sign3;//标记第三个道具是否被吃到

		float speeduptime;//加速的时间

	public:	
		GameProps(LPDIRECT3DDEVICE9 Device);
		~GameProps();
		void InitProps();//初始化函数
		void RenderProps();//渲染道具函数
		bool Collision(D3DXVECTOR3 position);  //检测坦克是否吃到了道具（吃==碰撞）
		void upDateProps(float timeDelta); //更新道具状态
		void effect1(D3DXMATRIX *V,float timeDelta);//效果一（对应道具一）
		void Effect (float timeDelta);
		bool Effect2(float timeDelta);//判断加速道具是否被吃到的函数
		void updateplane (D3DXMATRIX * V,float timeDelte);//更新飞机状态（包括子弹）
		void resetplane(PLANE *plane);
		void addplane(int numofpalne);//添加飞机函数
		void removedead();//移除死亡的飞机和子弹
		void addbullet(D3DXVECTOR3 position);//添加子弹函数//这里添加子弹时需要知道子弹的初始位置；
		void resetbullet(D3DXVECTOR3 position,PROBULLET* bullet);//初始化一个子弹
		bool Effect3();
		void renderbound(D3DXMATRIX *V,float timeDelta);//绘制爆炸函数
		bool rendertime(float timeDelta);//绘制时间函数

};
