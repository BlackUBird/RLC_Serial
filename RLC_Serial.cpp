#include "DxLib.h"
#include <math.h>

#define SCREEN_X 1280	//画面の横サイズ
#define SCREEN_Y 720	//画面の縦サイズ

//***********************************************************************
#define PLANE_LABELMAX 15	//ラベルの文字数制限

//座標平面用構造体
typedef struct
{
	int OriginX;	//画面上での原点のX座標
	int OriginY;	//画面上での原点のY座標
	int DivX;	//画面上でのX軸の目盛りの間隔(補助線を引く間隔)
	int DivY;	//画面上でのY軸の目盛りの間隔(同上)
	long double OnePixelX;	//横軸1ピクセルの重み
	long double OnePixelY;	//縦軸1ピクセルの重み
	char LabelX[PLANE_LABELMAX];	//X座標のラベル
	char LabelY[PLANE_LABELMAX];	//Y座標のラベル
} PLANE;
PLANE nowPlane = {0};
PLANE initPlane = {0};

//各値を取得するための関数
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

//画面上での原点の座標を設定
void Plane_SetOrigin(int ox,int oy)
{
	nowPlane.OriginX = ox;
	nowPlane.OriginY = oy;
}

//画面上での目盛りの間隔を設定
void Plane_SetDivision(int dx,int dy)
{
	nowPlane.DivX = dx;
	nowPlane.DivY = dy;
}

//1ピクセルの重みを設定
void Plane_SetOnePixel(long double opx,long double opy)
{
	nowPlane.OnePixelX = opx;
	nowPlane.OnePixelY = opy;
}

//座標平面を表示
void Plane_Draw(void)
{
	int i;	//雑用
	//色(補助線)
	int cr;
	cr = GetColor(128,128,128);

	//画面を白で埋める
	DrawBox(0,0,SCREEN_X,SCREEN_Y,GetColor(255,255,255),TRUE);
	
	//補助線を表示
	//横
	for(i = nowPlane.OriginY % nowPlane.DivY ; i <= SCREEN_Y ; i+=nowPlane.DivY)
	{
		DrawLine(0,i,SCREEN_X,i,cr);
	}
	//縦
	for(i = nowPlane.OriginX % nowPlane.DivX ; i <= SCREEN_X ; i+=nowPlane.DivX)
	{
		DrawLine(i,0,i,SCREEN_Y,cr);
	}
	
	//軸を表示
	cr = GetColor(0,0,0);
	DrawBox(0,nowPlane.OriginY-1,SCREEN_X,nowPlane.OriginY+1,cr,TRUE);	//横軸
	DrawBox(nowPlane.OriginX-1,0,nowPlane.OriginX+1,SCREEN_Y,cr,TRUE);	//縦軸
}

//画面上の平面を移動させる
void Plane_Move(int x,int y)
{
	Plane_SetOrigin(Plane_GetOriginX() + x,Plane_GetOriginY() + y);
}

//座標平面をリセット
void Plane_Reset(void)
{
	nowPlane = initPlane;
}

//座標平面を初期化
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

	//初期化時の各値を記憶
	initPlane = nowPlane; 
}
//***********************************************************************
//***********************************************************************
//プレイヤー用構造体
typedef struct
{
	char Key[256];	//キーの押下状態
	int State;	//プレイヤーの状態
} PLAYER;
PLAYER nowPlayer = {0};

//構造体の値を取得
char Player_GetKeyState(int keycode)
{
	return nowPlayer.Key[keycode];
}
int Player_GetState(void)
{
	return nowPlayer.State;
}

//キーの押下状態を更新
void Player_KeyUpdate()
{
	GetHitKeyStateAll(nowPlayer.Key);
}

//平面について
void Player_Play_Plane(void)
{
	int ox,oy;	//原点
	ox = Plane_GetOriginX();
	oy = Plane_GetOriginY();

	//Bキー（ブースト）
	int boost = 0;
	if( Player_GetKeyState(KEY_INPUT_B) == 1 )
	{
		boost = 19;
	}

	//方向キー
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
	//原点の位置を更新
	Plane_SetOrigin(ox,oy);

	//0キー(半角数字ゼロキー)（リセット）
	if( Player_GetKeyState(KEY_INPUT_0) == 1 )
	{
		Plane_Reset();
	}

	long double opx,opy,updx,updy;	//1ピクセルの重みxy,更新用xy
	opx = Plane_GetOnePixelX();
	opy = Plane_GetOnePixelY();
	//Xキー（X軸のみ変化）
	//Yキー（Y軸のみ変化）
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
	//Wキー（1ピクセルの重みを増やす）
	if( Player_GetKeyState(KEY_INPUT_W) == 1 )
	{
		opx += updx;
		opy += updy;
	}
	//Sキー（1ピクセルの重みを減らす）
	if( Player_GetKeyState(KEY_INPUT_S) == 1 )
	{
		opx -= updx;
		opy -= updy;
	}
	//1ピクセルの重みを更新
	Plane_SetOnePixel(opx,opy);
}
//***********************************************************************
//***********************************************************************
#define GRAPH_POINT_RADIUS 1
//グラフ用構造体
typedef struct
{
	long double PointX;	//グラフの点のX座標(測定値)
	long double PointY;	//グラフの点のY座標(測定値)
	int Color;	//色
	int Number;	//識別番号
} GRAPH;

//グラフの各点を描画
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

//グラフの各点を直線で結ぶ
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

//グラフの初期化処理
GRAPH Graph_Initialize(long double px,long double py,int cr)
{
	GRAPH gr = {px,py,cr};
	return gr;
}
//***********************************************************************
//***********************************************************************
#define PLANE_SUPPORTBOX_HEIGHT 90
//座標平面の情報を表示
void Plane_Support_Draw(int boxcr,int framecr)
{
	int i;	//雑用
	//下地
	DrawBox(0,SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT,SCREEN_X,SCREEN_Y,boxcr,TRUE);
	DrawBox(0,SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT,SCREEN_X,SCREEN_Y,framecr,FALSE);

	int x,y;
	x = 3;
	y = SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT;

	//各種ステータスを表示
	i=0;
	//画面上の原点座標
	DrawFormatString(x,y+18*i++,GetColor(0,0,0),"Origin(%d,%d)",Plane_GetOriginX(),Plane_GetOriginY());
	//目盛りの間隔
	DrawFormatString(x,y+18*i++,GetColor(0,0,0),"DivisionSizeX,Y   :%d,%d",Plane_GetDivisionX(),Plane_GetDivisionY());
	//1ピクセルの重み
	DrawFormatString(x,y+18*i++,GetColor(0,0,0),"ValueofOnePixelX,Y:%e,%e",Plane_GetOnePixelX(),Plane_GetOnePixelY());
	//ラベル
	DrawFormatString(x,y+18*i++,GetColor(0,0,0),"LabelX,Y          :\"%s\",\"%s\"",Plane_GetLabelX(),Plane_GetLabelY());
}

//グラフの情報を表示
void Graph_Support_Draw(GRAPH grp,int x,int y,double C)
{
	/*int x,y;
	x = 20*22;
	y = SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT;*/

	//ステータス表示
	DrawFormatString(x,y,grp.Color,"GraphNumber:%d  C:%e[F]",grp.Number,C);
}
//***********************************************************************
//表
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
//取得(ゲッター)
double Chart_GetValueX(void)
{
	return nowChart.ValueX;
}

//初期化処理
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
//各値の保存
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
//RLC直列回路に流れる電流を返す
long double RLCSerial_DC_GetI(long double R,long double L,long double C,long double t,long double E)
{
	//eの肩の数を決定,演算,返却
	long double a;
	int b;
	double ld_b;
	b = (int)(R*R-4*L/C);	//ルートの中身
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
	//R^2と4L/Cによる場合分けの判断
	int x;	//ルートの中身
	x = R*R - 4*L*(int)pow(10.0,TenExpL-TenExpC)/C;

	//±の前の部分
	double a;
	a =-1*R*pow(10.0,-1*TenExpL)/(2*L);

	double b,w;

	//場合分け
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
		//ω
		b = pow(10.0,TenExpL-TenExpC)*4*L/C - (double)(R*R);//ルートの中身
		w = sqrt(b)*pow(10.0,-1*TenExpL) / (2*L);
		return exp(a*t)*sin(w*t)*2*E/sqrt(b);
	}
	else
	{//R^2>4L/C
		/*DrawCircle(SCREEN_X-15,3,2,GetColor(0,0,255),TRUE);
		DrawFormatString(0,36,GetColor(0,0,255),"x = %d",x);*/
		//ω
		b = (double)R*R - 4*L*pow(10.0,TenExpL-TenExpC)/C;
		w = sqrt(b)*pow(10.0,-1*TenExpL) / (2*L);
		return E*(exp((a+w)*t) - exp((a-w)*t))/sqrt(b);
		return E*(exp((a+w)*t) - exp((a-w)*t))/(2*w*L*pow(10.0,TenExpL));
	}
}

//***********************************************************************
//FPS固定用
static int mStartTime;      //測定開始時刻
static int mCount;          //カウンタ
static float mFps;          //fps
static const int N = 60;	//平均を取るサンプル数
static const int FPS = 60;	//設定したFPS

bool FPS_Update(){
	if( mCount == 0 ){ //1フレーム目なら時刻を記憶
		mStartTime = GetNowCount();
	}
	if( mCount == N ){ //60フレーム目なら平均を計算する
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
	int tookTime = GetNowCount() - mStartTime;	//かかった時間
	int waitTime = mCount*1000/FPS - tookTime;	//待つべき時間
	if( waitTime > 0 ){
		Sleep(waitTime);	//待機
	}
}
//***********************************************************************

#define SAMPLE_MAX 640	//標本数
#define SAMPLE_PLUS_MAX 640		//正側の標本数
#define SAMPLE_MINUS_MAX 640	//負側の標本数

// プログラムは WinMain から始まります
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{
//	ChangeWindowMode(FALSE);	//ウィンドウモードで表示
	SetGraphMode(SCREEN_X,SCREEN_Y,16);
	if( DxLib_Init() == -1 )		// ＤＸライブラリ初期化処理
	{
		return -1 ;			// エラーが起きたら直ちに終了
	}

	int i;	//雑用

	int COUNT = 0;	//カウント

	//初期化用
	int init_ox = SCREEN_X/2,init_oy = SCREEN_Y/2;	//画面上の原点
	int init_dx = 20,init_dy = 20;	//目盛りの間隔(ピクセル)
	long double init_opx = 5*pow(10.0,-19.0),init_opy = 5*pow(10.0,-6.0);	//1ピクセルの重み
	char init_lblx[PLANE_LABELMAX] = "t[sec]",init_lbly[PLANE_LABELMAX] = "I[A]";	//軸ラベル

	GRAPH graph1[SAMPLE_MAX];	//グラフその１ R^2=4L/C
//	GRAPH graph2[SAMPLE_MAX];	//グラフその2  R^2<4L/C
//	GRAPH graph3[SAMPLE_MAX];	//グラフその3  R^2>4L/C
	GRAPH graph4[SAMPLE_MAX];	//グラフその4
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

	//ox,oy:原点は画面中央
	//dx,dy:目盛間隔は20ピクセル
	//opx,opy:1ピクセルの重みはx:5*10^-19,y:5*10^-6
	//x軸ラベル:t
	//y軸ラベル:I
	Plane_Initialize(init_ox,init_oy,init_dx,init_dy,init_opx,init_opy,init_lblx,init_lbly);
	while(ClearDrawScreen() == 0 && ProcessMessage() == 0 && SetDrawScreen(DX_SCREEN_BACK) == 0)
	{
		if(CheckHitKey( KEY_INPUT_ESCAPE ) == 1)
		{
			break;	//ESCキーが押されたら終了(強制終了)
		}

		//10Fごとにキー入力受付
		if((COUNT % 10) == 0)
		{
			Player_KeyUpdate();
			Player_Play_Plane();
			COUNT = 0;
		}

		Plane_Draw();	//座標平面を描画

		//グラフを描画
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
			//6.0*pow(10.0,-20.0) のとき R^2 - 4L/C = 0
			
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
		DrawFormatString(20*22,SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT+18*j++,GetColor(0,0,0),"R=%d[Ω]  L=%e[H]  E=%lf[V]",R,L*pow(10.0,ETL),E);
		Graph_Support_Draw(graph4[0],20*22,SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT+18*j++,C1*pow(10.0,ETC));
		Graph_Support_Draw(graph5[0],20*22,SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT+18*j++,C2*pow(10.0,ETC));
		Graph_Support_Draw(graph6[0],20*22,SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT+18*j++,C3*pow(10.0,ETC));
		DrawFormatString(3,SCREEN_Y-PLANE_SUPPORTBOX_HEIGHT+18*j++,GetColor(0,0,0),"Config            :0=Reset W,S=ChangeValuofOnePixel(HoldingXorY=OnlyXorOnlyY) ↑↓←→=MoveOrigin(HoldingB=Boost) ");

		ScreenFlip();
		FPS_Update();
		FPS_Wait();
		COUNT += 1;
	}

	WaitKey() ;				// キー入力待ち

	DxLib_End() ;				// ＤＸライブラリ使用の終了処理

	return 0 ;				// ソフトの終了 
}