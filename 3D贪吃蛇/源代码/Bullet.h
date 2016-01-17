#pragma once
#include<list>
#include"d3dUtility.h"
#include"CameraClass.h"
#include"XFileMesh.h"
#include"DirectInputClass.h"
//子弹的属性
struct Bullet
{
		float  _age;//年龄
		float _lifetime;//寿命
		bool _alive;//是否活着
		D3DXVECTOR3 _position;//子弹位置
		D3DXVECTOR3 _speed;//子弹的速度（方向）
		enum Whos{my,enemy};//谁的子弹
};

class BULLET :public XFileMesh{
	private :
		CamerClass * _camera;
		std::list <Bullet>  _allbuttet;
	public :
		BULLET ();
		BULLET(LPDIRECT3DDEVICE9 Device,CamerClass * camera);//初始化的时候把虚拟摄像机传进来以便获取摄像机当前位置
		void resetbullet(Bullet *bullet);//初始化一个新子弹的初始状态
		void addbullet();
		bool InitBullet ();
		void Update(float timeDeltas);
		void render ();
		void removedead();

};