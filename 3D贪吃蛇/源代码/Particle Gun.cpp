#include"Particle Gun.h"
ParticleGun::ParticleGun(CamerClass * camera)
{
	_camera          = camera;
	_size            = 3.0f;
	_vbSize          = 2048;
	_vbOffset        = 0;  
	_vbBatchSize     = 512; 
}
void ParticleGun::resetParticle(Attribute* attribute)
{
 	attribute->_isAlive =true;
	D3DXVECTOR3 cameraPos;
	_camera->getposition(&cameraPos);
	D3DXVECTOR3 cameraDir;
	_camera->getlook (&cameraDir);
	attribute->_position =cameraPos;
	attribute->_position .z+=15.0f;
	attribute->_position .y -= 15.0f;
	attribute->_velocity =cameraDir*100.0f;
	attribute->_color = D3DXCOLOR(0.0f,0.0f,1.0f,1.0f);
	attribute->_age=0.0f;
	attribute->_lifeTime =1.0f;//ªÓ1√Î
}

void ParticleGun::update(float timeDelta)
{
	std::list<Attribute>::iterator i;
	for(i = _particles.begin(); i != _particles.end(); i++)
	{
		i->_position += i->_velocity * timeDelta;
		i->_age += timeDelta;
		if(i->_age > i->_lifeTime) 
			i->_isAlive = false;
	}
	removeDeadParticles();
}
