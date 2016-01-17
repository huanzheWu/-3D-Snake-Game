#pragma once
#include"d3dUtility.h"
#include"XFileMesh.h"
#include"CameraClass.h"
#include"DirectInputClass.h"
class Tank : public XFileMesh
{
	public:
		Tank (LPDIRECT3DDEVICE9 _Device);
	private :
		D3DXMATRIX matHero;	
	public :
		void getBox (D3DXVECTOR3* min,D3DXVECTOR3* max);
		bool collisiontest (D3DXVECTOR3  Emin,D3DXVECTOR3 Emax);
		void SetTank(CamerClass * camera,DInputClass * DInput);

};
