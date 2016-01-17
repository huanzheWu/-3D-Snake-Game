#pragma once
#include"Psystem.h"
#include"CameraClass.h"
class Firework :public PSystem
{
	public  :
		Firework(D3DXVECTOR3 *position ,int nums);
		void resetParticle (Attribute *attribute);
		void update(float timeDelta);
		void preRender();
		void postRender();
};