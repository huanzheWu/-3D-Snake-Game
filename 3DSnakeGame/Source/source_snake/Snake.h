#pragma once
#include"d3dUtility.h"
#include<list>
#include"XFileMesh.h"
#include"DirectInputClass.h"
#include"CameraClass.h"
#include"Snow.h"
#include"Ground.h"

//道具行为宏定义
#define ADDBODY		1
#define SPEEDUP		2
#define SPEEDDOWN	3
#define CUTDOWN		4
#define ADDSCORE	5
#define NOADDSCORE	6
#define EXPAND		7
#define DEAD		8

//蛇的结构体
struct SNAKE
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
	//位置、前进方向（把最后一颗的方向向量赋给它）
};


//转折点结构体
struct TURNING
{
	//需要转折点位置以及所转方向；
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
};

class Snake 
{
	public :
		Snake(PDIRECT3DDEVICE9 _Device){Device=_Device;};
		~Snake();
	
	private:
		XFileMesh * Psnake;//蛇头的XFileMesh类指针
		LPDIRECT3DDEVICE9 Device;//D3D设备指针
		std::list <TURNING> _allTurning;//保存转折点信息的链表	//那什么时候需要添加链表呢？---蛇头转向的时候
		std::list<SNAKE> _allSnakeNode;//一条保存了蛇所有节点信息的链表；方便在添加新节点时，遍历节点获取最后一个节点。
		IDirect3DTexture9 *tex[2];

		int score ;//游戏得分
		wchar_t *a; //用与获取分数时，将int转换为wchar_t。
		int WallRange;//围墙的大小
		float timeOfSpeed ;//蛇加速的时间
		ID3DXFont *PFont ; //字体接口指针
		bool hadAddScore; //标记是否吃到了加分道具
	    int speedEnable ;//标记是否吃到加速道具
	    float  speedTime ;//标记加速状态的持续时间
		float  speed;//蛇的行走时间
		Snow * snow; //雪花粒子系统
		bool expandwall;
		bool isalive;

	private:
		void addTurning(D3DXVECTOR3 position,D3DXVECTOR3 direction);//添加一个转折点,需要蛇头传入相关信息
		void addSnake(D3DXVECTOR3 position,D3DXVECTOR3 direction);//在蛇的尾部添加一个节点。
		void renderTime (int time ); //渲染时间
		void RenderSnake();//渲染蛇身
		void isDead (float range);//判断蛇是否撞墙死亡；
	public:
		bool initSnake ();//初始化函数，在这一步创建蛇头的X文件模型，指定模型的初始位置等等
		void upDateSanke(float timeDelta,DInputClass* Dinput);//更新节点的位置与方向
		void Render ();
		//********************第一人称视觉试验************************
		void getHeadPos(D3DXVECTOR3* position);
		wchar_t*  getScore ();//获取得分
		int getscore(){return score*100;};
		bool ifDead ();
		bool isDangerous(float range);
		void changeWallRange(Ground *ground);
		void renderScore();
};