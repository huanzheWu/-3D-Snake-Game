#pragma once
#include"d3dUtility.h"
#include<list>
#include"XFileMesh.h"
#include"DirectInputClass.h"
#include"CameraClass.h"
#include"Snow.h"
#include"Ground.h"

//������Ϊ�궨��
#define ADDBODY		1
#define SPEEDUP		2
#define SPEEDDOWN	3
#define CUTDOWN		4
#define ADDSCORE	5
#define NOADDSCORE	6
#define EXPAND		7
#define DEAD		8

//�ߵĽṹ��
struct SNAKE
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
	//λ�á�ǰ�����򣨰����һ�ŵķ���������������
};


//ת�۵�ṹ��
struct TURNING
{
	//��Ҫת�۵�λ���Լ���ת����
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
};

class Snake 
{
	public :
		Snake(PDIRECT3DDEVICE9 _Device){Device=_Device;};
		~Snake();
	
	private:
		XFileMesh * Psnake;//��ͷ��XFileMesh��ָ��
		LPDIRECT3DDEVICE9 Device;//D3D�豸ָ��
		std::list <TURNING> _allTurning;//����ת�۵���Ϣ������	//��ʲôʱ����Ҫ��������أ�---��ͷת���ʱ��
		std::list<SNAKE> _allSnakeNode;//һ�������������нڵ���Ϣ����������������½ڵ�ʱ�������ڵ��ȡ���һ���ڵ㡣
		IDirect3DTexture9 *tex[2];

		int score ;//��Ϸ�÷�
		wchar_t *a; //�����ȡ����ʱ����intת��Ϊwchar_t��
		int WallRange;//Χǽ�Ĵ�С
		float timeOfSpeed ;//�߼��ٵ�ʱ��
		ID3DXFont *PFont ; //����ӿ�ָ��
		bool hadAddScore; //����Ƿ�Ե��˼ӷֵ���
	    int speedEnable ;//����Ƿ�Ե����ٵ���
	    float  speedTime ;//��Ǽ���״̬�ĳ���ʱ��
		float  speed;//�ߵ�����ʱ��
		Snow * snow; //ѩ������ϵͳ
		bool expandwall;
		bool isalive;

	private:
		void addTurning(D3DXVECTOR3 position,D3DXVECTOR3 direction);//���һ��ת�۵�,��Ҫ��ͷ���������Ϣ
		void addSnake(D3DXVECTOR3 position,D3DXVECTOR3 direction);//���ߵ�β�����һ���ڵ㡣
		void renderTime (int time ); //��Ⱦʱ��
		void RenderSnake();//��Ⱦ����
		void isDead (float range);//�ж����Ƿ�ײǽ������
	public:
		bool initSnake ();//��ʼ������������һ��������ͷ��X�ļ�ģ�ͣ�ָ��ģ�͵ĳ�ʼλ�õȵ�
		void upDateSanke(float timeDelta,DInputClass* Dinput);//���½ڵ��λ���뷽��
		void Render ();
		//********************��һ�˳��Ӿ�����************************
		void getHeadPos(D3DXVECTOR3* position);
		wchar_t*  getScore ();//��ȡ�÷�
		int getscore(){return score*100;};
		bool ifDead ();
		bool isDangerous(float range);
		void changeWallRange(Ground *ground);
		void renderScore();
};