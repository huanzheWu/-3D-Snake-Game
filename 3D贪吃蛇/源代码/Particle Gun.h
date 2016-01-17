#pragma  once
#include"Psystem.h"
#include"CameraClass.h"
class ParticleGun:public PSystem
{
public:
	ParticleGun(CamerClass * camera);
	void resetParticle (Attribute *attribute);
	void update (float timeDelta);
private :
	CamerClass * _camera;
};