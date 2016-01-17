//=========================================================================
//							粒子系统基类
//
//=========================================================================


#pragma once
#include"d3dUtility.h"
#include<list>

//粒子结构
struct Particle
{
	D3DXVECTOR3 _position;
	D3DCOLOR    _color;
};


//粒子属性
struct Attribute
{
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _velocity;//粒子的速度
	D3DXVECTOR3 _acceleration ;//加速度
	float _lifeTime;
	float _color;
	float _colorFade;
	bool _isAlive;
	float _age;
};
class PSystem
{
	public:
		PSystem();
		virtual ~PSystem();

		virtual bool init( IDirect3DDevice9* device, wchar_t * texFileName); //创建顶点缓存 载入纹理
		virtual void reset();//重新设定系统中粒子的属性

		virtual void resetParticle (Attribute * attribute)=0; //重新设置粒子属性值
		virtual void addParticle();//为系统增加一个粒子

		virtual void update(float timeDelta)=0;//对系统中所有粒子进行更新

		//绘制前中后
		virtual void preRender ();
		virtual void render ();
		virtual void postRender ();

		bool isEmpty();//检测当前系统是否有粒子
		bool isDead();//检测所有粒子是否都死亡

		protected:
			virtual void removeDeadParticles(); //对属性表进行搜索，移除死亡粒子。
		protected:
			IDirect3DDevice9* _device;
			D3DXVECTOR3 _origin;//系统粒子源
			float _emitRate;
			float _size;//系统中所有粒子的尺寸
			IDirect3DTexture9* _tex;
			IDirect3DVertexBuffer9* _vb;
			std::list<Attribute>  _particles;
			int _maxParticles;
			d3d::BoundingBox  _boundingBox;//外接体

			DWORD _vbSize;
			DWORD _vbOffset;
			DWORD _vbBatchSize;
};