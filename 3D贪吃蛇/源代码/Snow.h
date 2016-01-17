#pragma once
#include"Psystem.h"
#include"d3dUtility.h"
//从基类PSystem中继承而来
class Snow :public PSystem
{
	public:
		Snow(d3d::BoundingBox *boundingBox,int numParticles);//构造函数
		void update (float timeDelta);
		void resetParticle(Attribute * attribute);
		
};