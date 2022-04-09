#include "DxLib.h"
#include <math.h>

#define SCREEN_X 1280	//��ʂ̉��T�C�Y
#define SCREEN_Y 720	//��ʂ̏c�T�C�Y

//***********************************************************************
#define PLANE_LABELMAX 15	//���x���̕���������

//���W���ʗp�\����
typedef struct
{
	int OriginX;	//��ʏ�ł̌��_��X���W
	int OriginY;	//��ʏ�ł̌��_��Y���W
	int DivX;	//��ʏ�ł�X���̖ڐ���̊Ԋu(�⏕���������Ԋu)
	int DivY;	//��ʏ�ł�Y���̖ڐ���̊Ԋu(����)
	long double OnePixelX;	//����1�s�N�Z���̏d��
	long double OnePixelY;	//�c��1�s�N�Z���̏d��
	char LabelX[PLANE_LABELMAX];	//X���W�̃��x��
	char LabelY[PLANE_LABELMAX];	//Y���W�̃��x��
} PLANE;
PLANE nowPlane = {0};
PLANE initPlane = {0};

//�e�l���擾���邽�߂̊֐�
int Plane_GetOriginX(void)
{
	return nowPlane.OriginX;
}
int Plane_GetOriginY(void)
{
	return nowPlane.OriginY;
}
int Plane_GetDivisionX(void)
{
	return nowPlane.DivX;
}
int Plane_GetDivisionY(void)
{
	return nowPlane.DivY;
}
long double Plane_GetOnePixelX(void)
{
	return nowPlane.OnePixelX;
}
long double Plane_GetOnePixelY(void)
{
	return nowPlane.OnePixelY;
}
char* Plane_GetLabelX(void)
{
	return nowPlane.LabelX;
}
char* Plane_GetLabelY(void)
{
	return nowPlane.LabelY;
}

PLANE Plane_GetInit(void)
{
	return initPlane;
}

//��ʏ�ł̌��_�̍��W��ݒ�
void Plane_SetOrigin(int ox,int oy)
{
	nowPlane.OriginX = ox;
	nowPlane.OriginY = oy;
}

//��ʏ�ł̖ڐ���̊Ԋu��ݒ�
void Plane_SetDivision(int dx,int dy)
{
	nowPlane.DivX = dx;
	nowPlane.DivY = dy;
}

//1�s�N�Z���̏d�݂�ݒ�
void Plane_SetOnePixel(long double opx,long double opy)
{
	nowPlane.OnePixelX = opx;
	nowPlane.OnePixelY = opy;
}

//���W���ʂ�\��
void Plane_Draw(void)
{
	int i;	//�G�p
	//�F(�⏕��)
	int cr;
	cr = GetColor(128,128,128);

	//��ʂ𔒂Ŗ��߂�
	DrawBox(0,0,SCREEN_X,SCREEN_Y,GetColor(255,255,255),TRUE);
	
	//�⏕����\��
	//��
	for(i = nowPlane.OriginY % nowPlane.DivY ; i <= SCREEN_Y ; i+=nowPlane.DivY)
	{
		DrawLine(0,i,SCREEN_X,i,cr);
	}
	//�c
	for(i = nowPlane.OriginX % nowPlane.DivX ; i <= SCREEN_X ; i+=nowPlane.DivX)
	{
		DrawLine(i,0,i,SCREEN_Y,cr);
	}
	
	//����\��
	cr = GetColor(0,0,0);
	DrawBox(0,nowPlane.OriginY-1,SCREEN_X,nowPlane.OriginY+1,cr,TRUE);	//����
	DrawBox(nowPlane.OriginX-1,0,nowPlane.OriginX+1,SCREEN_Y,cr,TRUE);	//�c��
}

//��ʏ�̕��ʂ��ړ�������
void Plane_Move(int x,int y)
{
	Plane_SetOrigin(Plane_GetOriginX() + x,Plane_GetOriginY() + y);
}

//���W���ʂ����Z�b�g
void Plane_Reset(void)
{
	nowPlane = initPlane;
}

//���W���ʂ�������
void Plane_Initialize(int ox,int oy,int dx,int dy,
					  long double opx,long double opy,
					  char lblx[],char lbly[])
{
	Plane_SetOrigin(ox,oy);
	Plane_SetDivision(dx,dy);
	Plane_SetOnePixel(opx,opy);
	for(int i = 0 ; i < PLANE_LABELMAX ; i++)
	{
		nowPlane.LabelX[i] = lblx[i];
		nowPlane.LabelY[i] = lbly[i];
	}

	//���������̊e�l���L��
	initPlane = nowPlane; 
}
//***********************************************************************
//***********************************************************************
//�v���C���[�p�\����
typedef struct
{
	char Key[256];	//�L�[�̉������
	int State;	//�v���C���[�̏��
} PLAYER;
PLAYER nowPlayer = {0};

//�\���̂̒l���擾
char Player_GetKeyState(int keycode)
{
	return nowPlayer.Key[keycode];
}
int Player_GetState(void)
{
	return nowPlayer.State;
}

//�L�[�̉�����Ԃ��X�V
void Player_KeyUpdate()
{
	GetHitKeyStateAll(nowPlayer.Key);
}

//���ʂɂ���
void Player_Play_Plane(void)
{
	int ox,oy;	//���_
	ox = Plane_GetOriginX();
	oy = Plane_GetOriginY();

	//B�L�[�i�u�[�X�g�j
	int boost = 0;
	if( Player_GetKeyState(KEY_INPUT_B) == 1 )
	{
		boost = 19;
	}

	//�����L�[
	if( Player_GetKeyState(KEY_INPUT_RIGHT) == 1 )
	{
		ox -= (1+boost);
	}
	if( Player_GetKeyState(KEY_INPUT_LEFT) == 1 )
	{
		ox += (1+boost);
	}
	if( Player_GetKeyState(KEY_INPUT_DOWN) == 1 )
	{
		oy -= (1+boost);
	}
	if( Player_GetKeyState(KEY_INPUT_UP) == 1 )
	{
		oy += (1+boost);
	}
	//���_�̈ʒu���X�V
	Plane_SetOrigin(ox,oy);

	//0�L�[(���p�����[���L�[)�i���Z�b�g�j
	if( Player_GetKeyState(KEY_INPUT_0) == 1 )
	{
		Plane_Reset();
	}

	long double opx,opy,updx,updy;	//1�s�N�Z���̏d��xy,�X�V�pxy
	opx = Plane_GetOnePixelX();
	opy = Plane_GetOnePixelY();
	//X�L�[�iX���̂ݕω��j
	//Y�L�[�iY���̂ݕω��j
	if( Player_GetKeyState(KEY_INPUT_X) == Player_GetKeyState(KEY_INPUT_Y) )
	{
		updx = opx/2;
		updy = opy/2;
	}
	else if( Player_GetKeyState(KEY_INPUT_X) == 1 )
	{
		updx = opx/2;
		updy = 0;
	}
	else if( Player_GetKeyState(KEY_INPUT_Y) == 1 )
	{
		updx = 0;
		updy = opy/2;
	}
	else{;}
	//W�L�[�i1�s�N�Z���̏d�݂𑝂₷�j
	if( Player_GetKeyState(KEY_INPUT_W) == 1 )
	{
		opx += updx;
		opy += updy;
	}
	//S�L�[�i1�s�N�Z���̏d�݂����炷�j
	if( Player_GetKeyState(KEY_INPUT_S) == 1 )
	{
		opx -= updx;
		opy -= updy;
	}
	//1�s�N�Z���̏d�݂��X�V
	Plane_SetOnePixel(opx,opy);
}
//***********************************************************************
//***********************************************************************
#define GRAPH_POINT_RADIUS 1
//�O���t�p�\����
typedef struct
{
	long double PointX;	//�O���t�̓_��X���W(����l)
	long double PointY;	//�O���t�̓_��Y���W(����l)
	int Color;	//�F
	int Number;	//���ʔԍ�
} GRAPH;

//�O���t�̊e�_��`��
void Graph_DrawPoint(GRAPH gr)
{
	long double px,py;
	px = gr.PointX / Plane_GetOnePixelX();
	py = gr.PointY / Plane_GetOnePixelY();

	DrawCircle(Plane_GetOriginX()+(int)px,
			   Plane_GetOriginY()-(int)py,
			   GRAPH_POINT_RADIUS,
			   gr.Color,
			   TRUE);
}

//�O���t�̊e�_�𒼐��Ō���
void Graph_DrawLine(GRAPH gr1,GRAPH gr2)
{
	long double px1,py1,px2,py2;
	px1 = gr1.PointX / Plane_GetOnePixelX();
	py1 = gr1.PointY / Plane_GetOnePixelY();
	px2 = gr2.PointX / Plane_GetOnePixelX();
	py2 = gr2.PointY / Plane_GetOnePixelY();
	
	int ox,oy;
	ox = Plane_GetOriginX();
	oy = Plane_GetOriginY();

	DrawLine(ox+(int)px1,oy-(int)py1,
		ox+(int)px2,oy-(int)py2,
		gr1.Color);
}

//�O���t�̏���������
GRAPH Graph_Initialize(long double px,long double py,int cr)
{
	GRAPH gr = {px,py,cr};
	return gr;
}
//***********************************************************************
//***********************************************************************
#define PLANE_SUPPORTBOX_HEIGHT 90
//���W���ʂ̏���\��
void Plane_Support_Draw(int boxcr,int framecr)
{
	int i;	//�G�p
	//���n
	DrawBox(0,SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT,SCREEN_X,SCREEN_Y,boxcr,TRUE);
	DrawBox(0,SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT,SCREEN_X,SCREEN_Y,framecr,FALSE);

	int x,y;
	x = 3;
	y = SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT;

	//�e��X�e�[�^�X��\��
	i=0;
	//��ʏ�̌��_���W
	DrawFormatString(x,y+18*i++,GetColor(0,0,0),"Origin(%d,%d)",Plane_GetOriginX(),Plane_GetOriginY());
	//�ڐ���̊Ԋu
	DrawFormatString(x,y+18*i++,GetColor(0,0,0),"DivisionSizeX,Y   :%d,%d",Plane_GetDivisionX(),Plane_GetDivisionY());
	//1�s�N�Z���̏d��
	DrawFormatString(x,y+18*i++,GetColor(0,0,0),"ValueofOnePixelX,Y:%e,%e",Plane_GetOnePixelX(),Plane_GetOnePixelY());
	//���x��
	DrawFormatString(x,y+18*i++,GetColor(0,0,0),"LabelX,Y          :\"%s\",\"%s\"",Plane_GetLabelX(),Plane_GetLabelY());
}

//�O���t�̏���\��
void Graph_Support_Draw(GRAPH grp,int x,int y,double C)
{
	/*int x,y;
	x = 20*22;
	y = SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT;*/

	//�X�e�[�^�X�\��
	DrawFormatString(x,y,grp.Color,"GraphNumber:%d  C:%e[F]",grp.Number,C);
}
//***********************************************************************
//�\
#define CHART_LABEL_MAX 20

typedef struct
{
	double ValueX;
	double ValueY;
	char LabelX[CHART_LABEL_MAX];
	char LabelY[CHART_LABEL_MAX];
	int Color;
	int FontSize;
	int Page;
} CHART;
CHART nowChart = {0};
CHART initChart = {0};
//�擾(�Q�b�^�[)
double Chart_GetValueX(void)
{
	return nowChart.ValueX;
}

//����������
void Chart_Initialize(double vx,double vy,char lblx[],char lbly[],int cr,int font,int page)
{
	initChart.ValueX = vx;
	initChart.ValueY = vy;
	for(int i = 0 ; i < CHART_LABEL_MAX ; i++)
	{
		initChart.LabelX[i] = lblx[i];
		initChart.LabelY[i] = lbly[i];
	}
	initChart.Color = cr;
	initChart.FontSize = font;
	initChart.Page = page;

	nowChart = initChart;
}
//***********************************************************************
//�e�l�̕ۑ�
void Save_Value_Binary(char fname[],double valx[],double valy[])
{
	FILE *fp;

	if( (fp = fopen(fname,"wb")) == NULL )
	{
		return;
	}
	else
	{
		fwrite(valx,sizeof(valx[0]),sizeof(valx)/sizeof(valx[0]),fp);
		fwrite(valy,sizeof(valy[0]),sizeof(valy)/sizeof(valx[0]),fp);
	}
	fclose(fp);
}

void Save_Value_CSV(char fname[],double valx[],double valy[])
{
	FILE *fp;

	if( (fp = fopen(fname,"w")) == NULL )
	{
		return;
	}
	else
	{
		for(int i = 0 ; i < sizeof(valx)/sizeof(valx[0]) ; i++)
		{
			fprintf(fp,"%lf,%lf\n",valx[i],valy[i]);
		}
	}
	fclose(fp);
}

//***********************************************************************
//RLC�����H�ɗ����d����Ԃ�
long double RLCSerial_DC_GetI(long double R,long double L,long double C,long double t,long double E)
{
	//e�̌��̐�������,���Z,�ԋp
	long double a;
	int b;
	double ld_b;
	b = (int)(R*R-4*L/C);	//���[�g�̒��g
//	L /= pow(10.0,-20.0);
	a = R*(-1)/(2*L);
	if( b == 0)
	{//R^2=4L/C
		return E/L*t*exp(a*t);
	}
	else if( b < 0 )
	{//R^2<4L/C
		ld_b = sqrt((-1*(double)b)/(2*L));
		return (E/L/ld_b)*exp(a*t)*sin(ld_b*t);
	}
	else
	{//R^2>4L/C
		ld_b = sqrt((double)b)/(2*L);
		return (E/ld_b)*(exp((a+ld_b)*t) - exp((a-ld_b)*t));
	}
}

double RLCSerial_DC_GetI_2(int R,int L,int TenExpL,int C,int TenExpC,double t,double E)
{
	//R^2��4L/C�ɂ��ꍇ�����̔��f
	int x;	//���[�g�̒��g
	x = R*R - 4*L*(int)pow(10.0,TenExpL-TenExpC)/C;

	//�}�̑O�̕���
	double a;
	a =-1*R*pow(10.0,-1*TenExpL)/(2*L);

	double b,w;

	//�ꍇ����
	if( x == 0 )
	{//R^2=4L/C
		/*DrawCircle(SCREEN_X-3,3,2,GetColor(255,0,0),TRUE);
		DrawFormatString(0,0,GetColor(255,0,0),"x = %d",x);*/
		return E*t*exp(a*t)/(L*pow(10.0,TenExpL));
	}
	else if( x < 0 )
	{//R^2 < 4L/C
		/*DrawCircle(SCREEN_X-9,3,2,GetColor(0,255,0),TRUE);
		DrawFormatString(0,18,GetColor(0,255,0),"x = %d",x);*/
		//��
		b = pow(10.0,TenExpL-TenExpC)*4*L/C - (double)(R*R);//���[�g�̒��g
		w = sqrt(b)*pow(10.0,-1*TenExpL) / (2*L);
		return exp(a*t)*sin(w*t)*2*E/sqrt(b);
	}
	else
	{//R^2>4L/C
		/*DrawCircle(SCREEN_X-15,3,2,GetColor(0,0,255),TRUE);
		DrawFormatString(0,36,GetColor(0,0,255),"x = %d",x);*/
		//��
		b = (double)R*R - 4*L*pow(10.0,TenExpL-TenExpC)/C;
		w = sqrt(b)*pow(10.0,-1*TenExpL) / (2*L);
		return E*(exp((a+w)*t) - exp((a-w)*t))/sqrt(b);
		return E*(exp((a+w)*t) - exp((a-w)*t))/(2*w*L*pow(10.0,TenExpL));
	}
}

//***********************************************************************
//FPS�Œ�p
static int mStartTime;      //����J�n����
static int mCount;          //�J�E���^
static float mFps;          //fps
static const int N = 60;	//���ς����T���v����
static const int FPS = 60;	//�ݒ肵��FPS

bool FPS_Update(){
	if( mCount == 0 ){ //1�t���[���ڂȂ玞�����L��
		mStartTime = GetNowCount();
	}
	if( mCount == N ){ //60�t���[���ڂȂ畽�ς��v�Z����
		int t = GetNowCount();
		mFps = 1000.f/((t-mStartTime)/(float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

void FPS_Draw(){
	DrawFormatString(0, 0, GetColor(255,255,255), "%.1f", mFps);
}

void FPS_Wait(){
	int tookTime = GetNowCount() - mStartTime;	//������������
	int waitTime = mCount*1000/FPS - tookTime;	//�҂ׂ�����
	if( waitTime > 0 ){
		Sleep(waitTime);	//�ҋ@
	}
}
//***********************************************************************

#define SAMPLE_MAX 640	//�W�{��
#define SAMPLE_PLUS_MAX 640		//�����̕W�{��
#define SAMPLE_MINUS_MAX 640	//�����̕W�{��

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{
//	ChangeWindowMode(FALSE);	//�E�B���h�E���[�h�ŕ\��
	SetGraphMode(SCREEN_X,SCREEN_Y,16);
	if( DxLib_Init() == -1 )		// �c�w���C�u��������������
	{
		return -1 ;			// �G���[���N�����璼���ɏI��
	}

	int i;	//�G�p

	int COUNT = 0;	//�J�E���g

	//�������p
	int init_ox = SCREEN_X/2,init_oy = SCREEN_Y/2;	//��ʏ�̌��_
	int init_dx = 20,init_dy = 20;	//�ڐ���̊Ԋu(�s�N�Z��)
	long double init_opx = 5*pow(10.0,-19.0),init_opy = 5*pow(10.0,-6.0);	//1�s�N�Z���̏d��
	char init_lblx[PLANE_LABELMAX] = "t[sec]",init_lbly[PLANE_LABELMAX] = "I[A]";	//�����x��

	GRAPH graph1[SAMPLE_MAX];	//�O���t���̂P R^2=4L/C
//	GRAPH graph2[SAMPLE_MAX];	//�O���t����2  R^2<4L/C
//	GRAPH graph3[SAMPLE_MAX];	//�O���t����3  R^2>4L/C
	GRAPH graph4[SAMPLE_MAX];	//�O���t����4
	GRAPH graph5[SAMPLE_MAX];
	GRAPH graph6[SAMPLE_MAX];

	/*long double R,L,C1,C2,C3,E;
	R = 1000.0;
	L = 15.0*pow(10.0,-15.0);
	C1 = 6.0*pow(10.0,-20.0);
	C2 = 4.0*pow(10.0,-20.0);
	C3 = 8.0*pow(10.0,-20.0);
	E = 10;*/

	int R = 1000;
	int L = 15;
	int ETL = -15;	//ExponentialTensL(gabagabahonnyaku)
	int C1 = 6;
	int ETC = -20;
	int C2 = 4;
	int C3 = 8;
	double E = 10.0;

	//ox,oy:���_�͉�ʒ���
	//dx,dy:�ڐ��Ԋu��20�s�N�Z��
	//opx,opy:1�s�N�Z���̏d�݂�x:5*10^-19,y:5*10^-6
	//x�����x��:t
	//y�����x��:I
	Plane_Initialize(init_ox,init_oy,init_dx,init_dy,init_opx,init_opy,init_lblx,init_lbly);
	while(ClearDrawScreen() == 0 && ProcessMessage() == 0 && SetDrawScreen(DX_SCREEN_BACK) == 0)
	{
		if(CheckHitKey( KEY_INPUT_ESCAPE ) == 1)
		{
			break;	//ESC�L�[�������ꂽ��I��(�����I��)
		}

		//10F���ƂɃL�[���͎�t
		if((COUNT % 10) == 0)
		{
			Player_KeyUpdate();
			Player_Play_Plane();
			COUNT = 0;
		}

		Plane_Draw();	//���W���ʂ�`��

		//�O���t��`��
		for(i = 0 ; i < sizeof(graph1)/sizeof(graph1[0]) ; i++)
		{
//			graph1[i].PointX = i * Plane_GetOnePixelX();
//			graph2[i].PointX = i * Plane_GetOnePixelX();
//			graph3[i].PointX = i * Plane_GetOnePixelX();
			graph4[i].PointX = i * Plane_GetOnePixelX();
			graph5[i].PointX = i * Plane_GetOnePixelX();
			graph6[i].PointX = i * Plane_GetOnePixelX();

			graph4[i].Number = 4;
			graph5[i].Number = 5;
			graph6[i].Number = 6;
			
			//R = 1k
			//L = 15f
			//C = ?
			//t = now pointX
			//E = 10
//			graph1[i].PointY = RLCSerial_DC_GetI(R,L,C1,graph1[i].PointX,E);
//			graph2[i].PointY = RLCSerial_DC_GetI(R,L,C2,graph2[i].PointX,E);
//			graph3[i].PointY = RLCSerial_DC_GetI(R,L,C3,graph3[i].PointX,E);
			graph4[i].PointY = RLCSerial_DC_GetI_2(R,L,ETL,C1,ETC,graph4[i].PointX,E);
			graph5[i].PointY = RLCSerial_DC_GetI_2(R,L,ETL,C2,ETC,graph5[i].PointX,E);
			graph6[i].PointY = RLCSerial_DC_GetI_2(R,L,ETL,C3,ETC,graph6[i].PointX,E);
			//6.0*pow(10.0,-20.0) �̂Ƃ� R^2 - 4L/C = 0
			
//			graph1[i].Color = GetColor(255,0,0);
//			graph2[i].Color = GetColor(0,255,0);
//			graph3[i].Color = GetColor(0,0,255);
			graph4[i].Color = GetColor(255,0,0);
			graph5[i].Color = GetColor(0,255,0);
			graph6[i].Color = GetColor(0,0,255);
//			Graph_DrawPoint(graph1[i]);
//			Graph_DrawPoint(graph2[i]);
//			Graph_DrawPoint(graph3[i]);
			Graph_DrawPoint(graph4[i]);
			Graph_DrawPoint(graph5[i]);
			Graph_DrawPoint(graph6[i]);
			if(i != 0)
			{
//				Graph_DrawLine(graph1[i-1],graph1[i]);
//				Graph_DrawLine(graph2[i-1],graph2[i]);
//				Graph_DrawLine(graph3[i-1],graph3[i]);
				Graph_DrawLine(graph4[i-1],graph4[i]);
				Graph_DrawLine(graph5[i-1],graph5[i]);
				Graph_DrawLine(graph6[i-1],graph6[i]);
			}			
		}
		Plane_Support_Draw(GetColor(255,255,255),GetColor(0,0,0));

		int j=0;
		DrawFormatString(20*22,SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT+18*j++,GetColor(0,0,0),"R=%d[��]  L=%e[H]  E=%lf[V]",R,L*pow(10.0,ETL),E);
		Graph_Support_Draw(graph4[0],20*22,SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT+18*j++,C1*pow(10.0,ETC));
		Graph_Support_Draw(graph5[0],20*22,SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT+18*j++,C2*pow(10.0,ETC));
		Graph_Support_Draw(graph6[0],20*22,SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT+18*j++,C3*pow(10.0,ETC));
		DrawFormatString(3,SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT+18*j++,GetColor(0,0,0),"Config            :0=Reset W,S=ChangeValuofOnePixel(HoldingXorY=OnlyXorOnlyY) ��������=MoveOrigin(HoldingB=Boost) ");

		ScreenFlip();
		FPS_Update();
		FPS_Wait();
		COUNT += 1;
	}

	WaitKey() ;				// �L�[���͑҂�

	DxLib_End() ;				// �c�w���C�u�����g�p�̏I������

	return 0 ;				// �\�t�g�̏I�� 
}