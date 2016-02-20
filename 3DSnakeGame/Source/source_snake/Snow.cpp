#include"Snow.h"


Snow::Snow(d3d::BoundingBox *boundingBox,int numParticles)
{
	_boundingBox=*boundingBox;
	_size=5.0f; //系统中每个粒子的大小
	_vbSize=2048;//顶点缓存的大小
	_vbOffset=0;
	_vbBatchSize=512;//一段缓冲的大小
	for(int i=0;i<numParticles;i++)
		addParticle();
}

void Snow::resetParticle(Attribute *attribute)
{
	attribute->_isAlive =true;
	//在外接体内随机产生一个位置
	d3d::GetRandomVector(&attribute->_position ,&_boundingBox._min,&_boundingBox._max);
	//y取最高点
	attribute->_position.y =_boundingBox._max.y ;
	//设置随机的速度
	attribute->_velocity.x=d3d::GetRandomFloat (0.0f,1.0f)*-30.0f;
	attribute->_velocity .y =d3d::GetRandomFloat (0.0f,1.0f)*-100.0f;
	attribute->_velocity.z =0.0f;
	//颜色
	attribute->_color =d3d::WHILT ;
}
void Snow::update (float timeDelta)
{
	std::list <Attribute>::iterator i;
	for(i=_particles.begin();i!=_particles.end ();i++)
	{
		i->_position +=i->_velocity *timeDelta;

		if (_boundingBox.isPointInside(i->_position )==false)
		{
			resetParticle(&(*i));
		}
	}
}