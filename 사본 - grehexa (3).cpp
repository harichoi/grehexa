#include <windows.h>
#include <Stdio.h>
#include <math.h>
#include <string.h>
#include <memory.h>
#include "resource.h"

#define HEIGHT 8
#define WIDTH 10
#define DEFX 31
#define DEFY 9

class Point
{
public:
	double x;
	double y;
};
class IPoint
{
public:
	int x;
	int y;
};

Point makepoint(double x, double y)
{
	Point a;
	a.x=x;
	a.y=y;
	return a;
};
IPoint Imakepoint(int x, int y)
{
	IPoint a;
	a.x=x;
	a.y=y;
	return a;
};

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
void draw(HWND hWnd);
void process();
void calcul();
int finish();
void rotate();

HBITMAP blockbit[100], scorebit, scoremaskbit;
HBITMAP maskbit[100], bgbit;
HBITMAP sb1bit,sb2bit,sb3bit,sbm1bit,sbm2bit,sbm3bit;
HBITMAP levelbit[100],levelmaskbit, copyright;
const int nblock=5;
int block[100][100], tblock[100][100], dieblock[100][100];
Point pblock[100][100], wblock[100][100];
int bombblock[100][100];
int selectx, selecty, ptx, pty;
int mousex, mousey;
int wherego;
int type;
IPoint cho[100];
int nr;
int hide;
int score;

int WINAPI WinMain(HINSTANCE hInst,
				   HINSTANCE hPrev,
				   LPSTR lpCmd,
				   int nShow)
{
	maskbit[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_MASK));
	maskbit[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_MASK2));
	maskbit[3]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_MASK3));
	maskbit[4]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_MASK4));
	maskbit[5]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_MASK5));
	blockbit[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BLOCK1));
	blockbit[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BLOCK2));
	blockbit[3]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BLOCK3));
	blockbit[4]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BLOCK4));
	blockbit[5]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BLOCK5));
	blockbit[6]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_STAR));
	blockbit[7]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BOMB));
	bgbit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BACKGROUND));
	sb1bit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_SB1));
	sb2bit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_SB2));
	sb3bit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_SB3));
	sbm1bit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_SBM1));
	sbm2bit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_SBM2));
	sbm3bit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_SBM3));
	levelbit[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_LEVEL1));
	levelmaskbit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_LEVELMASK));
	copyright=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_COPYRIGHT));
	scorebit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_SCORE));
	scoremaskbit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_SCOREMASK));
	
	
	srand(GetTickCount());
	HWND hWnd;
	WNDCLASS wnd;
	MSG msg;

	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wnd.hCursor = LoadCursor(hInst,MAKEINTRESOURCE(IDC_CURSOR1));
	wnd.hIcon = 0;//LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1));
	wnd.hInstance = hInst;
	wnd.lpfnWndProc = WndProc;
	wnd.lpszClassName = "grehexa";
	wnd.lpszMenuName =0;// MAKEINTRESOURCE(IDR_MENU1);
	wnd.style = 0;

	RegisterClass(&wnd);

	hWnd = CreateWindow("grehexa","GREMCUS-grehexa",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,625,375,0,0,hInst,0);
	ShowWindow(hWnd,SW_SHOW);

	selectx++;selecty++;
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
int visit[100];
int check;
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	int ttype;
	int i, j, k, ax, ay, temp;
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(1);
		break;
	case WM_CREATE:
		for(i=1 ; i<=HEIGHT+1 ; i++)
			for(j=1 ; j<=WIDTH ; j++){
				Sleep(1);
				if(i==HEIGHT+1 && j%2==1)
					continue;
				block[i][j]=rand()%5+1;
			}
			calcul();
		//	process();
		break;
	case WM_TIMER:
		for(i=1 ; i<=HEIGHT+1 ; i++)
			for(j=1 ; j<=WIDTH ; j++){
				if(i==HEIGHT+1 && j%2==1)
					continue;
				if(nr==0){
					if(hide==0){
					pblock[i][j].y+=(wblock[i][j].y-pblock[i][j].y)/3;
					pblock[i][j].x+=(wblock[i][j].x-pblock[i][j].x)/3;
					}
				}
				else
				{
					int hh;
					hh=0;
					for(k=1 ; k<=3 ; k++)
						if(cho[k].y==i && cho[k].x==j)
							hh=1;
					if(hh==1){
						if(wblock[i][j].x>pblock[i][j].x)
							pblock[i][j].x+=3.5;
						if(wblock[i][j].y>pblock[i][j].y)
							pblock[i][j].y+=3.5;
						if(wblock[i][j].x<pblock[i][j].x)
							pblock[i][j].x-=3.5;
						if(wblock[i][j].y<pblock[i][j].y)
							pblock[i][j].y-=3.5;
					}
					else if(hide==0)
					{
						pblock[i][j].y+=(wblock[i][j].y-pblock[i][j].y)/3;
						pblock[i][j].x+=(wblock[i][j].x-pblock[i][j].x)/3;
					}
				}
				
			}
		if(finish())
		{
			process();
			if(nr>0)
				nr--, rotate();
		}
		draw(hWnd);

		if(hide>0)
			hide++;
		if(hide>5)
			hide=0;
		break;
	case WM_RBUTTONDOWN:
		if(finish()){
		for(i=1 ; i<=HEIGHT+1 ; i++)
			for(j=1 ; j<=WIDTH ; j++){
				if(i==HEIGHT+1 && j%2==1)
					continue;
				bombblock[i][j]--;
			}
			wherego=2;
			if(type!=3)
				nr=3;
			else nr=1;
		draw(hWnd);
		}
		break;
	case WM_LBUTTONDOWN:
		if(finish()){
		for(i=1 ; i<=HEIGHT+1 ; i++)
			for(j=1 ; j<=WIDTH ; j++){
				if(i==HEIGHT+1 && j%2==1)
					continue;
				bombblock[i][j]--;
			}
			wherego=1;
			if(type!=3)
				nr=3;
			else nr=1;
		draw(hWnd);
		}
		break;
	case WM_MOUSEMOVE:
		mousex=LOWORD(lParam);
		mousey=HIWORD(lParam);

		temp=999999999;
			for(i=2 ; i<=HEIGHT ; i++)
		for(j=1 ; j<=WIDTH-1 ; j++)
		{
			if(i==HEIGHT+1 && j%2==1)
				continue;
			if(j%2==1)
				ax=j*25+DEFX, ay=i*31+DEFY, ttype=1;
			else
				ax=j*25+DEFX+7, ay=i*31+DEFY, ttype=2;

			if(hypot(mousex-ax,mousey-ay)<temp)
				temp=hypot(mousex-ax,mousey-ay), selectx=j,selecty=i, type=ttype;
		}
	for(i=1 ; i<=HEIGHT ; i++)
		for(j=1 ; j<=WIDTH-1 ; j++)
		{
			if(i==HEIGHT+1 && j%2==0)
				continue;
			if(j%2==0)
				ax=j*25+DEFX, ay=i*31+DEFY+15, ttype=1;
			else
				ax=j*25+DEFX+7,ay=i*31+DEFY+15, ttype=2;
			if(hypot(mousex-ax,mousey-ay)<temp)
				temp=hypot(mousex-ax,mousey-ay), selectx=j,selecty=i, type=ttype;
			
		}
	for(i=2 ; i<=HEIGHT ; i++)
		for(j=2 ; j<=WIDTH-1 ; j++)
		{
			if(j%2==1 && i==HEIGHT)
				continue;
			if(i==HEIGHT+1 && j%2==0 || block[i][j]!=6)
				continue;
		ax=j*25+DEFX-7;
		ay=i*31+DEFY+12;
		if(hypot(mousex-ax,mousey-ay)-10<temp)
			temp=hypot(mousex-ax,mousey-ay)-10, selectx=j,selecty=i, type=3;
		}
		draw(hWnd); 
		break;
	case WM_KEYDOWN:
		break;
	case WM_COMMAND:
		
		break;
	case WM_PAINT:
		SetTimer(hWnd,1,50,0);
		draw(hWnd);
		break;
	};
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

void draw(HWND hWnd)
{
	int possible;
	int max;
	int i, j, k;
	char str[256];
	HBITMAP bit, *old, *old2;
	HBRUSH *old3, brush;
	HDC hDC;
	HDC MemDC;
	HDC bitDC;
	possible=finish();

	hDC=GetDC(hWnd);
	bit=CreateCompatibleBitmap(hDC,1200,1200);
	MemDC=CreateCompatibleDC(hDC);
	bitDC=CreateCompatibleDC(hDC);
	//hDC=BeginPaint(hWnd,&ps);
	old = (HBITMAP *)SelectObject(MemDC,bit);

	Rectangle(MemDC,0,0,1024,768);

	SetBkMode(MemDC,0);

	//배경  출력
		old2=(HBITMAP *)SelectObject(bitDC,bgbit);
		BitBlt(MemDC,0,0,1024,768,bitDC,0,0,SRCCOPY);
		SelectObject(bitDC,old2);

//block 출력

	for(i=1 ; i<=HEIGHT+1 ; i++)
	{
		for(j=1 ; j<=WIDTH ; j++)
		{
			if(i==HEIGHT+1 && j%2==1)
				continue;
			old2=(HBITMAP *)SelectObject(bitDC,maskbit[1]);
			BitBlt(MemDC,pblock[i][j].x,pblock[i][j].y,50,50,bitDC,0,0,SRCAND);
			SelectObject(bitDC,old2);
			old2=(HBITMAP *)SelectObject(bitDC,blockbit[block[i][j]]);
			BitBlt(MemDC,pblock[i][j].x,pblock[i][j].y,50,50,bitDC,0,0,SRCPAINT);
			SelectObject(bitDC,old2);
			if(block[i][j]==7)
			{
				sprintf(str,"%d",bombblock[i][j]);
				TextOut(MemDC,pblock[i][j].x+DEFX,pblock[i][j].y+DEFY,str,strlen(str));
			}
		}
	}
	int nc=1;
	if(possible){
		if(type==3){
			if(selectx%2==0){
			selecty--;
			selectx--;}
			else if(selectx%2==1){
		//	selecty--;
			selectx--;}
			old2=(HBITMAP *)SelectObject(bitDC,sbm3bit);
			int uu=1-selectx%2;
			BitBlt(MemDC,selectx*25-36,selecty*31-46-uu*15,150,150,bitDC,0,0,SRCAND);
			SelectObject(bitDC,old2);
			old2=(HBITMAP *)SelectObject(bitDC,sb3bit);
			BitBlt(MemDC,selectx*25-36,selecty*31-46-uu*15,150,150,bitDC,0,0,SRCPAINT);
			SelectObject(bitDC,old2);
			for(i=selecty ; i<=selecty+2 ; i++)
			{
				for(j=selectx ; j<=selectx+2 ; j++)
				{
					if(selectx%2==0){
						if(i==selecty && j==selectx)
							continue;
						if(i==selecty && j==selectx+2)
							continue;
						i--;
					}
					else
					{
						if(i==selecty+2 && j==selectx)
							continue;
						if(i==selecty+2 && j==selectx+2)
							continue;
					}
					old2=(HBITMAP *)SelectObject(bitDC,maskbit[1]);
					BitBlt(MemDC,pblock[i][j].x,pblock[i][j].y,50,50,bitDC,0,0,SRCAND);
					SelectObject(bitDC,old2);
					old2=(HBITMAP *)SelectObject(bitDC,blockbit[block[i][j]]);
					if(possible)
					cho[nc++]=Imakepoint(j,i);
					BitBlt(MemDC,pblock[i][j].x,pblock[i][j].y,50,50,bitDC,0,0,SRCPAINT);
					SelectObject(bitDC,old2);
				if(selectx%2==0)
					i++;
				}
			}
			if(selectx%2==0){
		//	selecty++;
			selectx++;}
			else if(selectx%2==1){
			selecty++;
			selectx++;}
		}
			if(type==2){
				old2=(HBITMAP *)SelectObject(bitDC,sbm1bit);
				int uu=1-selectx%2;
				BitBlt(MemDC,selectx*25-36,selecty*31-46-uu*15,150,150,bitDC,0,0,SRCAND);
				SelectObject(bitDC,old2);
				old2=(HBITMAP *)SelectObject(bitDC,sb1bit);
				BitBlt(MemDC,selectx*25-36,selecty*31-46-uu*15,150,150,bitDC,0,0,SRCPAINT);
				SelectObject(bitDC,old2);
				for(i=selecty ; i<=selecty+1 ; i++)
				{
					for(j=selectx ; j<=selectx+1 ; j++)
					{
						if(selectx%2==0){
							if(i==selecty && j==selectx)
								continue;
							i--;
						}
						else
						{
							if(i==selecty+1 && j==selectx)
								continue;
						}
						old2=(HBITMAP *)SelectObject(bitDC,maskbit[1]);
						BitBlt(MemDC,pblock[i][j].x,pblock[i][j].y,50,50,bitDC,0,0,SRCAND);
						SelectObject(bitDC,old2);
						old2=(HBITMAP *)SelectObject(bitDC,blockbit[block[i][j]]);
						if(possible)
						cho[nc++]=Imakepoint(j,i);
						BitBlt(MemDC,pblock[i][j].x,pblock[i][j].y,50,50,bitDC,0,0,SRCPAINT);
						SelectObject(bitDC,old2);
						if(selectx%2==0)
							i++;
					}
				}
			}
			if(type==1)
			{
				if(selectx%2==1)
					selecty--;
				old2=(HBITMAP *)SelectObject(bitDC,sbm2bit);
				int uu=1-selectx%2;
				BitBlt(MemDC,selectx*25-36,selecty*31-46-uu*15,150,150,bitDC,0,0,SRCAND);
				SelectObject(bitDC,old2);
				old2=(HBITMAP *)SelectObject(bitDC,sb2bit);
				BitBlt(MemDC,selectx*25-36,selecty*31-46-uu*15,150,150,bitDC,0,0,SRCPAINT);
				SelectObject(bitDC,old2);
				for(i=selecty ; i<=selecty+1 ; i++)
				{
					for(j=selectx ; j<=selectx+1 ; j++)
					{
						if(selectx%2==0){
							
							if(i==selecty+1 && j==selectx+1)
								continue;
			//			i--;
						}
						else
						{
							if(i==selecty && j==selectx+1)
								continue;
						}
						old2=(HBITMAP *)SelectObject(bitDC,maskbit[1]);
						BitBlt(MemDC,pblock[i][j].x,pblock[i][j].y,50,50,bitDC,0,0,SRCAND);
						SelectObject(bitDC,old2);
						old2=(HBITMAP *)SelectObject(bitDC,blockbit[block[i][j]]);
						if(possible)
							cho[nc++]=Imakepoint(j,i);
						BitBlt(MemDC,pblock[i][j].x,pblock[i][j].y,50,50,bitDC,0,0,SRCPAINT);
						SelectObject(bitDC,old2);
			//		if(selectx%2==0)
			//			i++;
					}
				}
				if(selectx%2==1)
					selecty++;
		
			}
		}
		
	if(hide>0)
	{	
		for(i=1 ; i<=HEIGHT+1 ; i++)
		{
			for(j=1 ; j<=WIDTH ; j++)
			{
				if(i==HEIGHT+1 && j%2==1)
					continue;
				if(dieblock[i][j]==0)
					continue;
			old2=(HBITMAP *)SelectObject(bitDC,maskbit[hide]);
			BitBlt(MemDC,wblock[i][j].x,wblock[i][j].y,50,50,bitDC,0,0,SRCAND);
			SelectObject(bitDC,old2);
			old2=(HBITMAP *)SelectObject(bitDC,blockbit[tblock[i][j]]);
			BitBlt(MemDC,wblock[i][j].x,wblock[i][j].y,50,50,bitDC,0,0,SRCPAINT);
			SelectObject(bitDC,old2);
			}
		}
	}
	
	old2=(HBITMAP *)SelectObject(bitDC,levelmaskbit);
	BitBlt(MemDC,300,0,500,500,bitDC,0,0,SRCAND);
	SelectObject(bitDC,old2);
	old2=(HBITMAP *)SelectObject(bitDC,levelbit[1]);
	BitBlt(MemDC,300,0,500,500,bitDC,0,0,SRCPAINT);
	SelectObject(bitDC,old2);
	old2=(HBITMAP *)SelectObject(bitDC,copyright);
	BitBlt(MemDC,300,200,500,500,bitDC,0,0,SRCCOPY);
	SelectObject(bitDC,old2);
	old2=(HBITMAP *)SelectObject(bitDC,scoremaskbit);
	BitBlt(MemDC,345,250,500,500,bitDC,0,0,SRCAND);
	SelectObject(bitDC,old2);
	old2=(HBITMAP *)SelectObject(bitDC,scorebit);
	BitBlt(MemDC,345,250,500,500,bitDC,0,0,SRCPAINT);
	SelectObject(bitDC,old2);
	SetTextColor(MemDC,RGB(255,255,255));
	sprintf(str,"%d 점",score);
	TextOut(MemDC,530-strlen(str)*10,280,str,strlen(str));
/*
	for(i=1 ; i<=HEIGHT ; i++)
		for(j=1 ; j<=WIDTH-1 ; j++)
		{
			if(i==HEIGHT+1 && j%2==1)
				continue;
			if(j%2==1)
				Ellipse(MemDC,j*25-2+DEFX-7,i*31-2+DEFY+12,j*25+2+DEFX-7,i*31+2+DEFY+12);
			else
				Ellipse(MemDC,j*25-2+DEFX-7,i*31-2+DEFY,j*25+2+DEFX-7,i*31+2+DEFY);
		}*//*
	for(i=1 ; i<=HEIGHT ; i++)
		for(j=1 ; j<=WIDTH-1 ; j++)
		{
			if(i==HEIGHT+1 && j%2==0)
				continue;
			if(j%2==0)
				Ellipse(MemDC,j*25-2+DEFX,i*31-2+DEFY+15,j*25+2+DEFX,i*31+2+DEFY+15);
			else
				Ellipse(MemDC,j*25-2+DEFX+7,i*31-2+DEFY+15,j*25+2+DEFX+7,i*31+2+DEFY+15);
		}*/
	

BitBlt(hDC,0,0,1024,768,MemDC,0,0,SRCCOPY);
	SelectObject(MemDC,old);
	ReleaseDC(hWnd,MemDC);
	ReleaseDC(hWnd,bitDC);
	ReleaseDC(hWnd,hDC);
	DeleteObject(bit);
	DeleteObject(old);
	DeleteObject(old2);
	DeleteObject(old3);
	DeleteObject(MemDC);
	DeleteObject(bitDC);
	DeleteObject(hDC);


	//	EndPaint(hWnd,&ps);
}

void swap(double &a,double &b)
{
	double temp;
	temp=a;
	a=b;
	b=temp;
}

void process()
{
	int i, j, k;

	int reblock[100][100];
	
	for(i=0 ; i<=HEIGHT+1 ; i++)
		for(j=0 ; j<=WIDTH ; j++)
			dieblock[i][j]=reblock[i][j]=0;
	for(i=1 ; i<=HEIGHT+1 ; i++)
		for(j=1 ; j<=WIDTH ; j++)
			tblock[i][j]=reblock[i][j]=block[i][j];
	for(i=1 ; i<=HEIGHT ; i++)
		for(j=1 ; j<=WIDTH ; j++)
		{
			if(j%2==1)
			{
				if(block[i][j]==block[i][j+1] && block[i][j]==block[i][j+2])
					if(block[i][j]==block[i+1][j] && block[i][j]==block[i+1][j+2] && block[i][j]==block[i+2][j+1])
					{
						reblock[i+1][j+1]=6;
						reblock[i][j]=0;
						reblock[i][j+1]=0;
						reblock[i][j+2]=0;
						reblock[i+1][j]=0;
						reblock[i+1][j+2]=0;
						reblock[i+2][j+1]=0;
						score+=2000;
					}
			}
			else
			{
				if(block[i][j]==block[i-1][j+1] && block[i][j]==block[i][j+2])
					if(block[i][j]==block[i+1][j] && block[i][j]==block[i+1][j+1] && block[i][j]==block[i+1][j+2])
					{
						reblock[i][j+1]=6;
						reblock[i][j]=0;
						reblock[i-1][j+1]=0;
						reblock[i][j+2]=0;
						reblock[i+1][j]=0;
						reblock[i+1][j+1]=0;
						reblock[i+1][j+2]=0;
						score+=2000;
					}
			}
			if(j%2==1)
			{
				if(block[i][j]==block[i+1][j] && block[i+1][j+1]==block[i][j])
				{
					reblock[i][j]=0;
					reblock[i+1][j]=0;
					reblock[i+1][j+1]=0;
					hide=1;
					nr=0;
				}
				if(block[i][j]==block[i][j+1] && block[i+1][j+1]==block[i][j])
				{
					reblock[i][j]=0;
					reblock[i][j+1]=0;
					reblock[i+1][j+1]=0;
					hide=1;
					nr=0;
				}
			}
			if(j%2==0)
			{
				if(block[i][j]==block[i+1][j] && block[i][j+1]==block[i][j])
				{
					reblock[i][j]=0;
					reblock[i+1][j]=0;
					reblock[i][j+1]=0;
					hide=1;
					nr=0;
				}
				if(block[i][j]==block[i-1][j+1] && block[i][j+1]==block[i][j])
				{
					reblock[i][j]=0;
					reblock[i-1][j+1]=0;
					reblock[i][j+1]=0;
					hide=1;
					nr=0;
				}
			}
		}
		int nnnnn=0;
	for(i=1 ; i<=HEIGHT+1 ; i++)
		for(j=1 ; j<=WIDTH ; j++){
			if(i==HEIGHT+1 && j%2==1)
				continue;
			if(reblock[i][j]==0)
			{
				nnnnn++;
				dieblock[i][j]=1;
				for(k=i ; k>=1 ; k--)
					reblock[k][j]=reblock[k-1][j], pblock[k][j]=pblock[k-1][j];
			}
		}
		if(nnnnn>2)
		score+=(nnnnn-2)*5;
	for(i=1 ; i<=HEIGHT+1 ; i++)
		for(j=1 ; j<=WIDTH ; j++){
			if(i==HEIGHT+1 && j%2==1)
				continue;
			if(reblock[i][j]==0){
				reblock[i][j]=rand()%5+1;
				if(rand()%2==0)
					reblock[i][j]=7, bombblock[i][j]=9;
			}
		}
	for(i=1 ; i<=HEIGHT+1 ; i++)
		for(j=1 ; j<=WIDTH ; j++)
			block[i][j]=reblock[i][j];
}

void calcul()
{
	int i, j;
	for(i=0 ; i<=HEIGHT+1 ; i++)
	{
		for(j=1 ; j<=WIDTH ; j++)
		{
			if(i==HEIGHT+1 && j%2==1)
				continue;
			if(j%2==1)
				wblock[i][j]=makepoint(j*25,i*31);
			else
				wblock[i][j]=makepoint(j*25,i*31-15);
			if(i==0)
				wblock[i][j].y=0;
		}
	}
	for(i=0 ; i<=HEIGHT+1 ; i++)
		for(j=1 ; j<=WIDTH ; j++)
			pblock[i][j]=makepoint(j*25,-50);
		hide=0;
}

void rotate()
{
	if(type<=2){
	if(cho[1].x>cho[2].x)
	{
		IPoint e;
		e=cho[1];
		cho[1]=cho[2];
		cho[2]=e;
	}
	if(cho[1].y>cho[2].y && cho[1].x==cho[2].x)
	{
		IPoint e;
		e=cho[1];
		cho[1]=cho[2];
		cho[2]=e;
	}
	if(cho[1].x>cho[3].x)
	{
		IPoint e;
		e=cho[1];
		cho[1]=cho[3];
		cho[3]=e;
	}
	if(cho[1].y>cho[3].y && cho[1].x==cho[3].x)
	{
		IPoint e;
		e=cho[1];
		cho[1]=cho[3];
		cho[3]=e;
	}
	if(cho[2].x<cho[3].x)
	{
		IPoint e;
		e=cho[2];
		cho[2]=cho[3];
		cho[3]=e;
	}
	if(cho[2].y>cho[3].y && cho[2].x == cho[3].x)
	{
		IPoint e;
		e=cho[2];
		cho[2]=cho[3];
		cho[3]=e;
	}
	}
	else
	{
		if(selectx%2==0){
			cho[1]=Imakepoint(selectx-1,selecty-1);
			cho[2]=Imakepoint(selectx,selecty-1);
			cho[3]=Imakepoint(selectx+1,selecty-1);
			cho[4]=Imakepoint(selectx+1,selecty);
			cho[5]=Imakepoint(selectx,selecty+1);
			cho[6]=Imakepoint(selectx-1,selecty);
		}
		if(selectx%2==1){
			cho[1]=Imakepoint(selectx-1,selecty);
			cho[2]=Imakepoint(selectx,selecty-1);
			cho[3]=Imakepoint(selectx+1,selecty);
			cho[4]=Imakepoint(selectx+1,selecty+1);
			cho[5]=Imakepoint(selectx,selecty+1);
			cho[6]=Imakepoint(selectx-1,selecty+1);
		}
	}
//	char str[256];
//	sprintf(str,"%d %d , %d %d , %d %d",cho[1].x,cho[1].y, cho[2].x,cho[2].y,cho[3].x,cho[3].y);
//	MessageBox(0,str,str,0);
int i;
if(wherego==1){
	if(type==3)
	{
	int temp, temp3;
	Point temp2;
	temp=block[cho[6].y][cho[6].x];
	temp2=pblock[cho[6].y][cho[6].x];
	temp3=bombblock[cho[6].y][cho[6].x];
	for(i=6 ; i>=2 ; i--)
	{
		block[cho[i].y][cho[i].x]=block[cho[i-1].y][cho[i-1].x];
		pblock[cho[i].y][cho[i].x]=pblock[cho[i-1].y][cho[i-1].x];		
		bombblock[cho[i].y][cho[i].x]=bombblock[cho[i-1].y][cho[i-1].x];		
	}
	block[cho[1].y][cho[1].x]=temp;
	pblock[cho[1].y][cho[1].x]=temp2;
	bombblock[cho[1].y][cho[1].x]=temp3;
	}
	else{
	int temp,temp3;
	Point temp2;
	temp=block[cho[3].y][cho[3].x];
	temp2=pblock[cho[3].y][cho[3].x];
	temp3=bombblock[cho[3].y][cho[3].x];
	for(i=3 ; i>=2 ; i--)
	{
		block[cho[i].y][cho[i].x]=block[cho[i-1].y][cho[i-1].x];
		pblock[cho[i].y][cho[i].x]=pblock[cho[i-1].y][cho[i-1].x];
		bombblock[cho[i].y][cho[i].x]=bombblock[cho[i-1].y][cho[i-1].x];		
	}
	block[cho[1].y][cho[1].x]=temp;
	pblock[cho[1].y][cho[1].x]=temp2;
	bombblock[cho[1].y][cho[1].x]=temp3;
	}
}
else if(wherego==2)
{
	
	if(type==3)
	{
	int temp,temp3;
	Point temp2;
	temp=block[cho[1].y][cho[1].x];
	temp2=pblock[cho[1].y][cho[1].x];
	temp3=bombblock[cho[1].y][cho[1].x];
	for(i=1 ; i<=5 ; i++)
	{
		block[cho[i].y][cho[i].x]=block[cho[i+1].y][cho[i+1].x];
		pblock[cho[i].y][cho[i].x]=pblock[cho[i+1].y][cho[i+1].x];	
		bombblock[cho[i].y][cho[i].x]=bombblock[cho[i+1].y][cho[i+1].x];	
	}
	block[cho[6].y][cho[6].x]=temp;
	pblock[cho[6].y][cho[6].x]=temp2;
	bombblock[cho[6].y][cho[6].x]=temp3;
	}
	else{
	int temp, temp3;
	Point temp2;
	temp=block[cho[1].y][cho[1].x];
	temp2=pblock[cho[1].y][cho[1].x];
	temp3=bombblock[cho[1].y][cho[1].x];
	for(i=1 ; i<=2 ; i++)
	{
		block[cho[i].y][cho[i].x]=block[cho[i+1].y][cho[i+1].x];
		pblock[cho[i].y][cho[i].x]=pblock[cho[i+1].y][cho[i+1].x];	
		bombblock[cho[i].y][cho[i].x]=bombblock[cho[i+1].y][cho[i+1].x];	
	}
	block[cho[3].y][cho[3].x]=temp;
	pblock[cho[3].y][cho[3].x]=temp2;
	bombblock[cho[3].y][cho[3].x]=temp3;
	}
}
}

int finish()
{
	int i, j;
	double ttt=0;
	for(i=1 ; i<=HEIGHT+1 ; i++)
	{
		for(j=1 ; j<=WIDTH ; j++)
		{
			if(i==HEIGHT+1 && j%2==1)
				continue;
			ttt+=hypot(wblock[i][j].y-pblock[i][j].y,wblock[i][j].x-pblock[i][j].x);
		}
	}
	if(ttt<15 && nr==0)
		return 1;
	if(ttt<15)
		return 1;
	return 0;
}