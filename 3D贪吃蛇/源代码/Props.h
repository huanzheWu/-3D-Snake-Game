#include"d3dUtility.h"
#include<list>
#include"BillBoard.h"

//道具功能宏定义
#define ADDBODY		1
#define SPEEDUP		2
#define SPEEDDOWN	3
#define CUTDOWN		4
#define ADDSCORE	5
#define NOADDSCORE  6
#define EXPAND		7
#define DEAD		8


//道具结构体
struct PROPS
{
	int _id;//标记道具
	D3DXVECTOR3 _position;//位置
	float _timeDelta;//周期
	float _timeHappen;//消失的时间
	bool _isalive;//状态
};

class Props 
{
	public:
		Props(PDIRECT3DDEVICE9 _Device){Device=_Device;};//构造函数
		~Props();//析构函数

	private:   //数据成员
		ID3DXFont *PFont ; //字体接口指针
	 	PDIRECT3DDEVICE9 Device;//D3D设备接口指针
		IDirect3DTexture9 * prop5tex[8]; //道具5的材质指针数组
		IDirect3DTexture9 * prop6tex[8]; //道具6的材质指针数组
		IDirect3DTexture9 * prop7tex[8]; //道具七的材质指针数组
		
		LPDIRECT3DVERTEXBUFFER9 Vb;
		std::list <PROPS> _allprops;//道具链表
		BillBoard *prop1;//第一种道具（用公告板来表示）(加长身体)
		BillBoard *prop2;//第二种道具（加速）
		BillBoard *prop3;//第三种道具（减速）
		BillBoard *prop4;//第四种道具（消去长度）
		float prop5TimeLeft,prop6TimeLeft,prop7TimeLeft;
		float prop5TimeExit,prop6TimeExit,prop7TimeExit;
		bool  isaddscore;
		int propscore;  //道具得分

		//纹理动画公告板的顶点格式
		struct vertex
		{
			vertex(){}
			vertex(float x,float y,float z,float u,float v)
			{
				_x=x;_y=y,_z=z;
				_u=u;_v=v;
			}
			float _x,_y, _z;
			float _u,_v;
			static const DWORD FVF;
		};

	private://内部调用函数
		void RenderTime (int time,RECT rect );
		void RenderDynaminc(IDirect3DTexture9 * proptex[8],D3DXMATRIX positon,D3DXMATRIX V);
	public:
		bool initProps();//初始化道具函数
		void upDateProps(float timeDelta);//更新道具函数
		void RenderProps();//绘制道具函数
		int Collision(D3DXVECTOR3 position);//碰撞函数
		int getpropscore (){return propscore;};
};