#include"d3dUtility.h"
#include"Psystem.h"

PSystem::PSystem()
{
	_device = 0;
	_vb     = 0;
	_tex    = 0;
}

PSystem::~PSystem()
{
	d3d::Release<IDirect3DVertexBuffer9*>(_vb);
	d3d::Release<IDirect3DTexture9*>(_tex);
}

//初始化函数
bool PSystem:: init (IDirect3DDevice9 * device,wchar_t *texFileName)
{
	_device = device; 
	HRESULT hr=0;
	//创建顶点缓存
	hr=device->CreateVertexBuffer (
		_vbSize* sizeof(Particle),
		D3DUSAGE_DYNAMIC|D3DUSAGE_POINTS|D3DUSAGE_WRITEONLY,
		D3DFVF_XYZ|D3DFVF_DIFFUSE,
		D3DPOOL_DEFAULT, 
		&_vb,
		0
		);
	if(FAILED(hr))
	{
		::MessageBox(0, L"CreateVertexBuffer() - FAILED", L"PSystem", 0);
		return false;
	}
	//创建纹理
	hr=D3DXCreateTextureFromFile(
		device,
		texFileName,
		&_tex);
	if(FAILED(hr))
	{
		::MessageBox(0, L"CreateTextureFromFile - FAILED", L"PSystem", 0);
		return false;
	}
}

//渲染前的设置函数
void PSystem ::preRender()
{
	
	_device->SetRenderState(D3DRS_LIGHTING, false);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE, true); 
	_device->SetRenderState(D3DRS_POINTSIZE, d3d::FtoDw(_size));
	_device->SetRenderState(D3DRS_POINTSIZE_MIN, d3d::FtoDw(0.0f));

	// control the size of the particle relative to distance
	_device->SetRenderState(D3DRS_POINTSCALE_A, d3d::FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_B, d3d::FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_C, d3d::FtoDw(1.0f));
		
	// use alpha from texture
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}


void PSystem ::render()
{
	//世界变换
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	_device->SetTransform(D3DTS_WORLD,&world);

	//如果链表中有元素 ，则进入
	if(!_particles.empty ())
	{	
		preRender ();
		_device->SetTexture (0,_tex); //设置材质
		_device->SetFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE); //设置顶点格式
		_device->SetStreamSource (0,_vb,0,sizeof(Particle)); //设置资源流
	
		//检测偏移量是否超过内存大小
		if(_vbOffset>=_vbSize)
			_vbOffset=0;
	
		DWORD numParticlesInBatch =0;

		Particle *v=0;
		_vb->Lock (
			_vbOffset *sizeof(Particle),
			_vbBatchSize * sizeof(Particle),
			(void **)&v,
			_vbOffset? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD );

		//遍历链表 开始画
		std::list <Attribute>::iterator i;
		for(i=_particles.begin();i!=_particles.end ();i++)
		{
			if(i->_isAlive==true)
			{
				v->_color=i->_color;
				v->_position=i->_position;
				v++;
				numParticlesInBatch++;
				if(numParticlesInBatch==_vbBatchSize)
				{
					_vb->Unlock ();
					//所以，这里实际上只是把粒子画出来？和位置无关？
					_device->DrawPrimitive(D3DPT_POINTLIST,_vbOffset,_vbBatchSize);
					_vbOffset+=_vbBatchSize;
					if(_vbOffset>=_vbSize)
						_vbOffset=0;
					numParticlesInBatch=0;
					_vb->Lock (
								_vbOffset *sizeof(Particle),
								_vbBatchSize * sizeof(Particle),
								(void **)&v,
								_vbOffset? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD );
				}
			}
		}
		_vb->Unlock ();

	//还要处理一种情况，当此次渲染中活着的粒子数不是_vbBatchSize的倍数，也即无法满足最后一个片段中（numParticlesInBatch==_vbBatchSize）的条件
	//这时最后一个片段没有被绘制到，所以要来绘制最后一个片段
		if(numParticlesInBatch)  //最后一个片段中有粒子
		{
			_device->DrawPrimitive(D3DPT_POINTLIST,_vbOffset,numParticlesInBatch);
		}
		_vbOffset+=_vbBatchSize;
		postRender();	
	}
}



//绘制后函数
void PSystem::postRender ()
{
	_device->SetRenderState (D3DRS_LIGHTING,true);
	_device->SetRenderState (D3DRS_ALPHABLENDENABLE,false);
	_device->SetRenderState (D3DRS_POINTSPRITEENABLE,false);
	_device->SetRenderState (D3DRS_POINTSCALEENABLE,false);
}

//重新设定系统中每个粒子的属性
void PSystem::reset ()
{
	std::list <Attribute>::iterator i;
	for(i=_particles.begin ();i!=_particles.end ();i++)
	{
		resetParticle(&(*i)); //这个函数是抽象函数，强制要求在子类中实现
	}
}

void PSystem::addParticle ()
{
	Attribute attribute;
	resetParticle(&attribute);//初始化一下
	_particles.push_back (attribute);//向尾部加入一个元素；
}

//移除死亡的粒子
void PSystem::removeDeadParticles ()
{
	std::list<Attribute>::iterator i;
	i=_particles.begin();
	while(i!=_particles.end ())
	{
		if(i->_isAlive==false)
			i=_particles.erase (i);
		else 
			i++;
	}
}


//判断当前系统是否有粒子

bool PSystem::isEmpty ()
{
	return _particles.empty();
}

//判断粒子是否全部死亡
bool PSystem::isDead ()
{
	std::list <Attribute> ::iterator i;
	for(i=_particles.begin ();i!=_particles.end ();i++)
	{
		if(i->_isAlive ==true)
			return false;
	}
	return true ;
}
