#ifndef __d3dUtilityH__
#define __d3dUtilityH__
#include <d3dx9.h>
#include <string>

namespace d3d
{
	//ѡ���Դ����
	D3DLIGHT9 InitDirectionalight(D3DXVECTOR3 *Direction,D3DXCOLOR * Color);
	D3DLIGHT9 InitPointlight(D3DXVECTOR3 *Position,D3DXCOLOR* Color);
	D3DLIGHT9 InitSpotlight(D3DXVECTOR3 *Position,D3DXVECTOR3 * Direction,D3DXCOLOR* Color );

	const float INFINITY=FLT_MAX;//�����ֵ
	const float EPSILON =0.001f;//����float�Ƚ�

	DWORD FtoDw(float f);//��float��ת��ΪDWORD��
	wchar_t*  IntToWchart(int willchance);//��intת��ΪWchar_t��

		//�����
	struct BoundingBox
	{
		BoundingBox();
		bool isPointInside(D3DXVECTOR3& p);
		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;
	};

	//�������
	float GetRandomFloat(float lowBound,float highBound);
	void  GetRandomVector(D3DXVECTOR3* out,D3DXVECTOR3 * min,D3DXVECTOR3 * max);
	//������ɫ
	const D3DXCOLOR WHILT ( D3DCOLOR_XRGB(255,255,255));
	const D3DXCOLOR BLACK (D3DCOLOR_XRGB(0,0,0));
	const D3DXCOLOR RED (D3DCOLOR_XRGB(255,0,0));
	const D3DXCOLOR GREEN(D3DCOLOR_XRGB(0,255,0));
	const D3DXCOLOR BLUE(D3DCOLOR_XRGB(0,0,255));
	const D3DXCOLOR YELLOW(D3DCOLOR_XRGB(255,255,0));
	const D3DXCOLOR BABYBLUE(D3DCOLOR_XRGB(0,255,255));
	const D3DXCOLOR PURPLE(D3DCOLOR_XRGB(255,0,255));
	//���ò��ʺ���
	D3DMATERIAL9 IintMetirial (D3DXCOLOR* a,D3DXCOLOR *b,D3DXCOLOR *c,D3DXCOLOR *d,float p);
	bool InitD3D(
		HINSTANCE hInstance,     
		int width, int height,   
		bool windowed,             
		D3DDEVTYPE deviceType,     
		IDirect3DDevice9** device,HWND* hwnd);

	int EnterMsgLoop( 
		bool (*ptr_display)(float timeDelta));  

	LRESULT CALLBACK WndProc(
		HWND hwnd,
		UINT msg, 
		WPARAM wParam,
		LPARAM lParam);

	template<class T> void Release(T t)
	{
		if( t )
		{
			t->Release();
			t = 0;
		}
	}
		
	template<class T> void Delete(T t)
	{
		if( t )
		{
			delete t;
			t = 0;
		}
	}
}

#endif 