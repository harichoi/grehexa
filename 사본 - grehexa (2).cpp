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

HBITMAP blockbit[100];
HBITMAP maskbit, bgbit;
HBITMAP sb1bit,sb2bit,sbm1bit,sbm2bit;
const int nblock=5;
int block[100][100];
Point pblock[100][100], wblock[100][100];
int selectx, selecty, ptx, pty;
int mousex, mousey;
int type;
IPoint cho[5];
int nr;

int WINAPI WinMain(HINSTANCE hInst,
				   HINSTANCE hPrev,
				   LPSTR lpCmd,
				   int nShow)
{
	maskbit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_MASK));
	blockbit[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BLOCK1));
	blockbit[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BLOCK2));
	blockbit[3]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BLOCK3));
	blockbit[4]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BLOCK4));
	blockbit[5]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BLOCK5));
	bgbit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BACKGROUND));
	sb1bit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_SB1));
	sb2bit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_SB2));
	sbm1bit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_SBM1));
	sbm2bit=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_SBM2));
	
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

	hWnd = CreateWindow("grehexa","GREMCUS-grehexa",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,325,375,0,0,hInst,0);
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
	int i, j, ax, ay, temp;
	int wherex[100], think[100];
	int wherey[100];
		int first=0, pos=0;
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
			process();
		break;
	case WM_TIMER:
		for(i=1 ; i<=HEIGHT+1 ; i++)
			for(j=1 ; j<=WIDTH ; j++){
				if(i==HEIGHT+1 && j%2==1)
					continue;
				pblock[i][j].y+=(wblock[i][j].y-pblock[i][j].y)/3;
				pblock[i][j].x+=(wblock[i][j].x-pblock[i][j].x)/3;
			}
		if(finish())
		{
			process();
			if(nr>0)
				nr--, rotate();
		}
		draw(hWnd);
		break;
	case WM_LBUTTONDOWN:
		nr=3;
		draw(hWnd);
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
		draw(hWnd); 
		break;
	case WM_RBUTTONDOWN:
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
			old2=(HBITMAP *)SelectObject(bitDC,maskbit);
			BitBlt(MemDC,pblock[i][j].x,pblock[i][j].y,50,50,bitDC,0,0,SRCAND);
			SelectObject(bitDC,old2);
			old2=(HBITMAP *)SelectObject(bitDC,blockbit[block[i][j]]);
			BitBlt(MemDC,pblock[i][j].x,pblock[i][j].y,50,50,bitDC,0,0,SRCPAINT);
			SelectObject(bitDC,old2);
		}
	}
	int nc=1;
	if(type==2){
	old2=(HBITMAP *)SelectObject(bitDC,sbm1bit);
	int uu=1-selectx%2;
	BitBlt(MemDC,selectx*25-36,selecty*31-44-uu*15,150,150,bitDC,0,0,SRCAND);
	SelectObject(bitDC,old2);
	old2=(HBITMAP *)SelectObject(bitDC,sb1bit);
	BitBlt(MemDC,selectx*25-36,selecty*31-44-uu*15,150,150,bitDC,0,0,SRCPAINT);
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
			old2=(HBITMAP *)SelectObject(bitDC,maskbit);
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
	BitBlt(MemDC,selectx*25-36,selecty*31-44-uu*15,150,150,bitDC,0,0,SRCAND);
	SelectObject(bitDC,old2);
	old2=(HBITMAP *)SelectObject(bitDC,sb2bit);
	BitBlt(MemDC,selectx*25-36,selecty*31-44-uu*15,150,150,bitDC,0,0,SRCPAINT);
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
			old2=(HBITMAP *)SelectObject(bitDC,maskbit);
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
/*	
	for(i=2 ; i<=HEIGHT ; i++)
		for(j=1 ; j<=WIDTH-1 ; j++)
		{
			if(i==HEIGHT+1 && j%2==1)
				continue;
			if(j%2==1)
				Ellipse(MemDC,j*25-2+DEFX,i*31-2+DEFY,j*25+2+DEFX,i*31+2+DEFY);
			else
				Ellipse(MemDC,j*25-2+DEFX+7,i*31-2+DEFY,j*25+2+DEFX+7,i*31+2+DEFY);
		}
	for(i=1 ; i<=HEIGHT ; i++)
		for(j=1 ; j<=WIDTH-1 ; j++)
		{
			if(i==HEIGHT+1 && j%2==0)
				continue;
			if(j%2==0)
				Ellipse(MemDC,j*25-2+DEFX,i*31-2+DEFY+15,j*25+2+DEFX,i*31+2+DEFY+15);
			else
				Ellipse(MemDC,j*25-2+DEFX+7,i*31-2+DEFY+15,j*25+2+DEFX+7,i*31+2+DEFY+15);
		}
*/	

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
			reblock[i][j]=0;
	for(i=1 ; i<=HEIGHT+1 ; i++)
		for(j=1 ; j<=WIDTH ; j++)
			reblock[i][j]=block[i][j];
	for(i=1 ; i<=HEIGHT ; i++)
		for(j=1 ; j<=WIDTH ; j++)
		{
			if(j%2==1)
			{
				if(block[i][j]==block[i+1][j] && block[i+1][j+1]==block[i][j])
				{
					reblock[i][j]=0;
					reblock[i+1][j]=0;
					reblock[i+1][j+1]=0;
					nr=0;
				}
				if(block[i][j]==block[i][j+1] && block[i+1][j+1]==block[i][j])
				{
					reblock[i][j]=0;
					reblock[i][j+1]=0;
					reblock[i+1][j+1]=0;
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
					nr=0;
				}
				if(block[i][j]==block[i-1][j+1] && block[i][j+1]==block[i][j])
				{
					reblock[i][j]=0;
					reblock[i-1][j+1]=0;
					reblock[i][j+1]=0;
					nr=0;
				}
			}
		}
	for(i=1 ; i<=HEIGHT+1 ; i++)
		for(j=1 ; j<=WIDTH ; j++){
			if(i==HEIGHT+1 && j%2==1)
				continue;
			if(reblock[i][j]==0)
			{
				for(k=i ; k>=1 ; k--)
					reblock[k][j]=reblock[k-1][j], pblock[k][j]=pblock[k-1][j];
			}
		}
	for(i=1 ; i<=HEIGHT+1 ; i++)
		for(j=1 ; j<=WIDTH ; j++){
			if(i==HEIGHT+1 && j%2==1)
				continue;
			if(reblock[i][j]==0)
				reblock[i][j]=rand()%5+1;
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
		}
	}
	for(i=0 ; i<=HEIGHT+1 ; i++)
		for(j=1 ; j<=WIDTH ; j++)
			pblock[i][j]=makepoint(j*25,0);
}

void rotate()
{
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
//	char str[256];
//	sprintf(str,"%d %d , %d %d , %d %d",cho[1].x,cho[1].y, cho[2].x,cho[2].y,cho[3].x,cho[3].y);
//	MessageBox(0,str,str,0);
	int temp;
	temp=block[cho[3].y][cho[3].x];
	block[cho[3].y][cho[3].x]=block[cho[2].y][cho[2].x];
	block[cho[2].y][cho[2].x]=block[cho[1].y][cho[1].x];
	block[cho[1].y][cho[1].x]=temp;
	Point temp2;
	temp2=pblock[cho[3].y][cho[3].x];
	pblock[cho[3].y][cho[3].x]=pblock[cho[2].y][cho[2].x];
	pblock[cho[2].y][cho[2].x]=pblock[cho[1].y][cho[1].x];
	pblock[cho[1].y][cho[1].x]=temp2;
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
	if(ttt<1.5)
		return 1;
	return 0;
}