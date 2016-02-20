#pragma once
#include"XFileMesh.h"//网格x文件头文件
#include<list>
//鸟的结构
struct BIRD
{
	D3DXVECTOR3 _position;//鸟的位置
	D3DXVECTOR3 _direction;//鸟的飞行方向
};
class Ground
{
	private :
		LPDIRECT3DDEVICE9 Device;			//D3D设备指针
		LPDIRECT3DTEXTURE9 Tex[2];			//纹理接口
		std::list <D3DXVECTOR3> _allWalls;	//围墙的链表
		std::list <BIRD> _allBird;			//鸟的位置链表		
		XFileMesh *Wall;					//围墙的X文件指针
		D3DXMATRIX WallPos;					//墙壁的位置矩阵
		D3DXMATRIX WallSca;					//墙壁的缩放矩阵
		D3DXMATRIX BirdSca;					//鸟的缩放矩阵
		XFileMesh *Bird;					//天空的鸟的X文件指针
		D3DXVECTOR3 BirdDir1;				//鸟的飞行方向1
		D3DXVECTOR3 BirdDir2;				//鸟的飞行方向2
		D3DXMATRIX BirdRota;				//	鸟的转向问题
		bool ifrota;						//是否转向
		
	public:
		Ground (LPDIRECT3DDEVICE9 _Device){Device=_Device;};//构造函数
		~Ground();											//析构函数
		bool initObject ();									//初始化函数
		bool createWall(D3DXVECTOR3 centre);
		bool createBird (D3DXVECTOR3 centre);
		void renderGround ();
		void updateBird(float timeDelta);					//更新鸟状态的函数
		float wallRang;					//围墙的大小
		void getWallRange (float * wallrange){*wallrange=wallRang;};//获取围墙的大小
};