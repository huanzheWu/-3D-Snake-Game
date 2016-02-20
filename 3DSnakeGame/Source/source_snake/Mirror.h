#pragma once
#include"d3dUtility.h"
class Mirror{
public:
	Mirror(LPDIRECT3DDEVICE9 device)
	{
		Device=device;
		MirrorLength=1000;
	}
	~Mirror();
private:
		LPDIRECT3DDEVICE9 Device;
		int MirrorLength;
		IDirect3DVertexBuffer9 *Vb;
		IDirect3DTexture9 *Mirrortex;
		D3DXMATRIX ModdlePlace;
		//定义顶点格式

	struct MirrorVertex
	{
		MirrorVertex(float x,float y,float z,float nx,float ny,float nz,float u,float v)
		{
			_x=x,_y=y,_z=z;
			_nx=nx,_ny=ny,_nz=nz;
			_u=u,_v=v;
		}
		float _x,_y,_z;
		float _nx, _ny, _nz;
		float _u,_v;
		static const DWORD FVF_Mirror;
	};


public:
		void InitMirror(int length);
		D3DXMATRIX PreSet(D3DXMATRIX moddleplace);
		void LastSet();
};