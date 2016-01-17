#pragma once
#include"XFileMesh.h"//����x�ļ�ͷ�ļ�
#include<list>
//��Ľṹ
struct BIRD
{
	D3DXVECTOR3 _position;//���λ��
	D3DXVECTOR3 _direction;//��ķ��з���
};
class Ground
{
	private :
		LPDIRECT3DDEVICE9 Device;			//D3D�豸ָ��
		LPDIRECT3DTEXTURE9 Tex[2];			//����ӿ�
		std::list <D3DXVECTOR3> _allWalls;	//Χǽ������
		std::list <BIRD> _allBird;			//���λ������		
		XFileMesh *Wall;					//Χǽ��X�ļ�ָ��
		D3DXMATRIX WallPos;					//ǽ�ڵ�λ�þ���
		D3DXMATRIX WallSca;					//ǽ�ڵ����ž���
		D3DXMATRIX BirdSca;					//������ž���
		XFileMesh *Bird;					//��յ����X�ļ�ָ��
		D3DXVECTOR3 BirdDir1;				//��ķ��з���1
		D3DXVECTOR3 BirdDir2;				//��ķ��з���2
		D3DXMATRIX BirdRota;				//	���ת������
		bool ifrota;						//�Ƿ�ת��
		
	public:
		Ground (LPDIRECT3DDEVICE9 _Device){Device=_Device;};//���캯��
		~Ground();											//��������
		bool initObject ();									//��ʼ������
		bool createWall(D3DXVECTOR3 centre);
		bool createBird (D3DXVECTOR3 centre);
		void renderGround ();
		void updateBird(float timeDelta);					//������״̬�ĺ���
		float wallRang;					//Χǽ�Ĵ�С
		void getWallRange (float * wallrange){*wallrange=wallRang;};//��ȡΧǽ�Ĵ�С
};