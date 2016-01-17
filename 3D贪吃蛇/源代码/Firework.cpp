#include"Firework.h"
void Firework::resetParticle(Attribute* attribute)
{
	attribute->_isAlive =true;
	attribute->_position =_origin;
	D3DXVECTOR3 min=D3DXVECTOR3(-1.0f,-1.0f,-1.0f);
	D3DXVECTOR3 max=D3DXVECTOR3(1.0f,1.0f,1.0f);

	d3d::GetRandomVector(
		&attribute->_velocity ,
		&min,
		&max
		);

	D3DXVec3Normalize(&attribute->_velocity ,&attribute->_velocity );
	attribute->_velocity *=100.0f;

	attribute->_color =D3DXCOLOR (d3d::GetRandomFloat (0.0f,1.0f),d3d::GetRandomFloat (0.0f,1.0f),d3d::GetRandomFloat (0.0f,1.0f),1.0f);

	attribute->_age=0;
	attribute->_lifeTime =1.0f;
}
void Firework::update (float timeDelta)
{
	std::list <Attribute>::iterator i;
	for(i=_particles.begin();i!=_particles.end ();i++)
	{
		if(i->_isAlive ==true)
		{
			i->_age  +=timeDelta;
			i->_position +=i->_velocity *timeDelta;
			if(i->_age >i->_lifeTime )
				i->_isAlive =false;
		}
	}
}

Firework::Firework (D3DXVECTOR3 *position,int num)
{
	_origin        = *position;
	_size          = 0.9f;
	_vbSize        = 2048;
	_vbOffset      = 0;   
	_vbBatchSize   = 512; 

	for(int i = 0; i < num; i++)
		addParticle();
}

void Firework::preRender ()
{
	PSystem::preRender ();
	_device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_ONE);
	_device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_ONE);
	_device->SetRenderState (D3DRS_ZWRITEENABLE,false);
}
void Firework::postRender ()
{
	PSystem::postRender ();
	_device->SetRenderState (D3DRS_ZWRITEENABLE,true);
}