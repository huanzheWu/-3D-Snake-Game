#pragma once
#include<d3dx9.h>
class CamerClass
{
	
public:
	enum CameraType{air,land};
	CamerClass();
	CamerClass(CameraType cameraType);
	~CamerClass();
private:
		D3DXVECTOR3 position;
		D3DXVECTOR3 up;
		D3DXVECTOR3 look;
		D3DXVECTOR3 right;
		CameraType  cameratype;

public:
		

		//三个绕up、right、look旋转的函数
		void pitch(float angle);
		void roll(float angle);
		void yaw(float angle);

		//三个平移函数
		void walk(float units);
		void fly(float units);
		void strafe(float units);

		//获取虚拟摄像机的方向向量
		//实现：把look赋值给look;
		//如何使用：从主调函数传进来一个向量；
		void  getlook(D3DXVECTOR3 *Look);
		void  getup(D3DXVECTOR3 *Up);
		void  getright(D3DXVECTOR3 *Right);


		void getposition(D3DXVECTOR3 *P);
		void setposition(D3DXVECTOR3 *P);
		void setCamraType(CameraType cameraType);
		
		//设置观察矩阵,相当于原来的取景变换函数
		void SetViewMatrix(D3DXMATRIX * view);

};

