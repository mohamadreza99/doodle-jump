
#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <cmath>
#include "hash.h"
#include <SDL2_rotozoom.h>
using namespace std;

SDL_Surface* screen;
SDL_Surface* menu;
SDL_Surface* doodle;
SDL_Surface* doodler;
SDL_Surface* doodlerp;
SDL_Surface* doodlel;
SDL_Surface* background;
SDL_Surface* pausecover;
SDL_Surface* nplat;
SDL_Surface* fakeplat1;
SDL_Surface* fakeplat2;
SDL_Surface* bnose;
SDL_Surface* nose;
SDL_Surface* rotated;
SDL_Surface* spring1;
SDL_Surface* spring2;
SDL_Surface* heli;
SDL_Surface* likheli1;
SDL_Surface* likheli2;
SDL_Surface* jet;
SDL_Surface* likjet1;
SDL_Surface* likjet2;
SDL_Surface* likjet3;
SDL_Surface* play;
SDL_Surface* playon;
SDL_Surface* resume;
SDL_Surface* resumeon;
SDL_Surface* menubotton;
SDL_Surface* menubottonon;
SDL_Surface* shotball;
SDL_Surface* mplat;
SDL_Surface* enemy1;
SDL_Surface* gameover;
SDL_Surface* hole;
SDL_Surface* text;
SDL_Surface* text2;
TTF_Font* doodlefont;
Sound* springsound;
Sound* touchsound;
Sound* faketouchsound;
Sound* helisound;
Sound* jetsound;
Sound* losesound;
Sound* enemysound;
inline void loadimage();
inline void loadsound();
inline void overlab();
inline void dontexit(int& x, int& y);
inline void touchplat(int& x, int& y,float& dy);
inline void scroll(int& y, int& h, float& dy);
inline void makenplat();
inline void showdoodleshooting(SDL_Surface* rotates[], int x, int y);
inline void mainmenu();
inline void pause();
inline void physics(int& x, int& y, float& dy);
inline void helifunction(int& x, int& y, float& dy);
inline void jetfunction(int& x, int& y, float& dy);

struct point
{
	int x, y;
}plat [100];
const int n = 40;
SDL_Event e;
SDL_Event e1;
bool ifspring;
int esc = 0;
int cx = 320;
int cy = 512;
double angle;
int z[100];
long long int loopcount;
struct d
{
	int spring;
	int heli;
	int jet;
	int shooting;
	int fakeplat[100];
	int enemy;
	bool lose;
}d;
void fakeplatform(int& x, int& y, float& dy, int fakeplatpos[]);
struct position
{
	int x;
	int y;
};
struct Shot 
{
	position pos;
	bool enabled;
}shot;
point fakep[100];
int fakeplatpos[100];
int a, b;
float db;
double score;
#undef main
int main()
{
	srand(time(NULL));
	InitEngine(screen, "doodle jump", 640, 1024);
	loadimage();
	loadsound();
	int x = 100, y = 100, h = 300;
	float dy = 0;
	SDL_Surface* rotates[140];
	for (int i = 0; i < 140; i++)
	{
		rotates[i] = rotozoomSurface(nose, i-70, 1, 1);
	}
	for (int i = 0; i < 100; i++)
	{
		z[i] = -7;
	}
	mainmenu();
	while (!ExitRequested)
	{
		Update();
		SDL_BlitSurface(background, screen, 0, 0);

		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			pause();
		}
		physics(x, y, dy);
		makenplat();
		dontexit(x, y);
		touchplat(x, y, dy);
		if (y<h)
		{
			scroll(y, h, dy);
		}
		overlab();

		for (int i = 5; i < n; i++)
		{

			SDL_BlitSurface(nplat, screen, plat[i].x, plat[i].y);
			if (i == 7)
			{
				if (d.spring == 0)
				{

					SDL_BlitSurface(spring1, screen, plat[i].x + 25, plat[i].y - 20);
				}
				else if (d.spring < 20)
				{
					SDL_BlitSurface(spring2, screen, plat[i].x + 25, plat[i].y - 50);
					d.spring++;
				}
				else
					d.spring = 0;
			}
			if (i==8)
			{
				SDL_BlitSurface(heli, screen, plat[i].x + 25, plat[i].y - 33);
			}
			if (i==9)
			{
				SDL_BlitSurface(jet, screen, plat[i].x + 25, plat[i].y - 69);
			}
			if (i==21)
			{
				if (d.enemy==0)
				{
					SDL_BlitSurface(enemy1, screen, plat[i].x + 25, plat[i].y - 60);
				}
				else if (d.enemy < 60)
				{
					SDL_BlitSurface(enemy1, screen, plat[i].x + 1000, plat[i].y);
					d.enemy++;
				}
				else
				{
					d.enemy = 0;
				}
				if (x + 85 >= plat[i].x + 25 && x + 45 <= plat[i].x + 25 + 45 && y + 120 >= plat[i].y - 60)
				{
					//if (y + 30 >= plat[i].y - 55 && y  < plat[i].y && !d.heli && !d.jet)
					//{
					//	d.lose = true;
					//	PlaySound(losesound, 1);
					//}
					d.enemy++;
					if (dy>=0)
					{
						dy = -40;
					}
					else
					{
						d.enemy = 0;
					}
				}
			}

		}

		fakeplatform(x, y, dy, fakeplatpos);
		for (int i = 15; i < 20; i++)
		{
			SDL_BlitSurface(mplat, screen, plat[i].x, plat[i].y);
			plat[i].x += z[i];
			if (plat[i].x + 114 >= 600 && plat[i].x + 114 < 640)
			{
				z[i] = -7;
			}
			if (plat[i].x <= 20)
			{
				z[i] = 7;
			}

		}
		if (!Mouse.left && !d.heli && !d.jet)
		if (dy<-19)
		{
			doodle = doodlerp;
			SDL_BlitSurface(doodle, screen, x, y);
		}
		else
		{
			doodle = doodler;
			SDL_BlitSurface(doodle, screen, x, y);
		}
		if (Mouse.left && !d.heli && !d.jet)
		{
			showdoodleshooting(rotates, x, y);
			shot.enabled = true;
		}
		if (shot.enabled && d.shooting == 0)
		{
			angle = atan2(cx - Mouse.x, cy - Mouse.y);
			angle = (angle * 180) / M_PI;
			//angle = fabs(angle);
			//cout << angle << endl;
			shot.pos.x = x + 50;
			shot.pos.y = y + 35;
			d.shooting++;
		}
		if (d.shooting>0 && d.shooting<25)
		{
			SDL_BlitSurface(shotball, screen, shot.pos.x, shot.pos.y);
			shot.pos.x += 10;// *cos(angle);
			shot.pos.y -= 20;// *sin(angle);
			shot.enabled = false;
			d.shooting++;
			if (d.shooting == 24)
			{
				d.shooting = 0;
			}
		}
		helifunction(x, y, dy);
		jetfunction(x, y, dy);
		SDL_UpdateScreen();
		SDL_Delay(30);
		score++;
		if (d.lose)
		{
			//SDL_FillRect(screen, NULL, BLUE);
			break;
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	b = 1024;
	db = -2;
	for (int i = 0; i < 1000; i++)
	{
		SDL_BlitSurface(background, screen, 0, b);
		//SDL_BlitSurface(gameover, screen, 100, 100);
		if(b>0)
			b += db;
		else
		{
			SDL_BlitSurface(gameover, screen, 100, 100);
			SDL_BlitSurface(text, screen, 100, 400);
			SDL_BlitSurface(text2, screen, 400, 400);
		}
		SDL_UpdateScreen();
		Update();
	}
    return 0;
}





inline void loadimage()
{
	doodler = ImgLoader("assets/images/likright_X.png", 255, 255, 255);
	doodlerp = ImgLoader("assets/images/likrightodskok_X.png", 255, 255, 255);
	doodlel = ImgLoader("assets/images/likleft_X.png", 255, 255, 255);
	background = ImgLoader("assets/images/bck_X.png", 255, 255, 255);
	pausecover = ImgLoader("assets/images/pausecover_X.png", 255, 255, 255);
	nplat = ImgLoader("assets/images/normal platform.png", 255, 255, 255);
	bnose = ImgLoader("assets/images/likpucaodskok_X.png", 255, 255, 255);
	nose = ImgLoader("assets/images/liknjuska_X.png", 255, 255, 255);
	spring1 = ImgLoader("assets/images/spring f1.png", 255, 255, 255);
	spring2 = ImgLoader("assets/images/spring f2.png", 255, 255, 255);
	heli = ImgLoader("assets/images/cap f1.png", 255, 255, 255);
	menu = ImgLoader("assets/images/menu1_X.png", 255, 255, 255);
	play = ImgLoader("assets/images/play_X.png", 255, 255, 255);
	playon = ImgLoader("assets/images/playon_X.png", 255, 255, 255);
	resume = ImgLoader("assets/images/resume_X.png", 255, 255, 255);
	resumeon = ImgLoader("assets/images/resumeon_X.png", 255, 255, 255);
	menubotton = ImgLoader("assets/images/menu_X.png", 255, 255, 255);
	menubottonon = ImgLoader("assets/images/menuon_X.png", 255, 255, 255);
	likheli1 = ImgLoader("assets/images/likheli1.png", 255, 255, 255);
	likheli2 = ImgLoader("assets/images/likheli2.png", 255, 255, 255);
	jet = ImgLoader("assets/images/jetpack f11.png", 255, 255, 255);
	likjet1 = ImgLoader("assets/images/likjet1.png", 255, 255, 255);
	likjet2 = ImgLoader("assets/images/likjet2.png", 255, 255, 255);
	likjet3 = ImgLoader("assets/images/likjet3.png", 255, 255, 255);
	shotball = ImgLoader("assets/images/shotball.png", 255, 255, 255);
	fakeplat1 = ImgLoader("assets/images/fake platform f1.png", 255, 255, 255);
	fakeplat2 = ImgLoader("assets/images/fake platform f4.png", 255, 255, 255);
	mplat = ImgLoader("assets/images/right left moving paltform.png", 255, 255, 255);
	enemy1 = ImgLoader("assets/images/monster two f1.png", 255, 255, 255);
	gameover = ImgLoader("assets/images/game over.png", 255, 255, 255);
	doodlefont = TTF_OpenFont("assets/images/doodlejumpbold_v2.ttf", 30);
	text = TTF_RenderText_Solid(doodlefont, "your score: ", 0, 0, 0);
	text2 = TTF_RenderText_Solid(doodlefont, "12369 ", 0, 0, 0);
	hole = ImgLoader("assets/images/hole_X.png", 255, 255, 255);
}
inline void loadsound()
{
	springsound = LoadSound("assets/sounds/feder.ogg");
	touchsound = LoadSound("assets/sounds/jump.ogg");
	faketouchsound = LoadSound("assets/sounds/eggmonsterhit.ogg");
	helisound = LoadSound("assets/sounds/propeller2.ogg");
	jetsound = LoadSound("assets/sounds/jetpack5.ogg");
	losesound = LoadSound("assets/sounds/pada.ogg");
	enemysound = LoadSound("assets/sounds/jumponmonster.ogg");
}
inline void makenplat()
{
	static int k = 0;
	if (k == 0)
	for (int i = 0; i < n; i++)
	{
		plat[i].x = rand() % 526;
		plat[i].y = rand() % 1024;
		k++;
	}
}
void overlab()
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (j == i)
				continue;
			if (plat[i].x + 114 >= plat[j].x && plat[i].x <= plat[j].x + 114 && plat[i].y + 30 >= plat[j].y && plat[i].y <= plat[j].y + 30)
			{
				plat[j].x += 1000;
			}
			//if (plat[i].x + 114 >= plat[j].x && plat[i].x <= plat[j].x + 114 && plat[i].y + 30+55 <= plat[j].y )
			//{
			//	plat[i].x += 1000;
			//}
		}
	}
	for (int i = 0; i < 15; i++)
	{
		for (int j = 15; j < 20; j++)
		{
			if (plat[i].y + 30 >= plat[j].y && plat[i].y <= plat[j].y + 30)
			{
				plat[i].x += 1000;
			}
		}
	}
}
inline void dontexit(int& x, int& y)
{
	if (x >= 600)
	{
		x = -60;
	}
	if (x<-100)
	{
		x = 599;
	}
}
inline void touchplat(int& x, int& y,float& dy)
{
	if (dy >= 0)
	for (int i = 5; i < n; i++)
	{
		if (i==7 && x+65>= plat[i].x  && x+45 <= plat[i].x +55 && y+120 <= plat[i].y+20 && y+120 >= plat[i].y)
		{
			dy = -35;
			PlaySound(springsound, 1);
			d.spring ++;
		}
		else if (x+85 >= plat[i].x && x+45 <= plat[i].x +114 && y+120 <= plat[i].y+30 && y+120 >= plat[i].y  )
		{
			PlaySound(touchsound, 1);
			dy = -20;
		}
	}
}
inline void scroll(int& y, int& h, float& dy)
{
	y = h;
	for (int i = 0; i < n; i++)
	{
		plat[i].y = plat[i].y - dy;
		if (plat[i].y>1024)
		{
			plat[i].y = 0;
			plat[i].x = rand() % 526;
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < 5; i++)
	{
		fakep[i] = plat[i];
	}
}
inline void showdoodleshooting(SDL_Surface* rotates[],int x,int y)
{
	float rad = atan2(cx - Mouse.x, cy - Mouse.y);
	int deg = 180 * rad / M_PI;
	if (deg>-70 && deg<70)
	{
		rotated = rotates[deg + 70];
		SDL_BlitSurface(bnose, screen, x, y);
		SDL_BlitSurface(rotated, screen, x + 60 - (rotated->w) / 2, y + 70 - (rotated->h) / 2);
	}
	else if (deg >= 70)
	{
		rotated = rotates[139];
		SDL_BlitSurface(bnose, screen, x, y);
		SDL_BlitSurface(rotated, screen, x + 60 - (rotated->w) / 2, y + 70 - (rotated->h) / 2);
	}
	else
	{
		rotated = rotates[0];
		SDL_BlitSurface(bnose, screen, x, y);
		SDL_BlitSurface(rotated, screen, x + 60 - (rotated->w) / 2, y + 70 - (rotated->h) / 2);
	}
}

inline void mainmenu()
{
	int a = 100, b = 400;
	float db = 0;
	while (true)
	{
		Update();
		SDL_BlitSurface(menu, screen, 0, 0);
		SDL_BlitSurface(nplat, screen, 100, 700);
		SDL_BlitSurface(doodler, screen, a, b);
		SDL_BlitSurface(play, screen, 100, 250);
		db += 0.5;
		b += db;
		if (b >= 580)
			db -= 25;
		if (Mouse.x>100 && Mouse.x<253 && Mouse.y>250 && Mouse.y<310)
		{
			SDL_BlitSurface(playon, screen, 100, 250);
			if (Mouse.left)
			{
				break;
			}
		}
		SDL_UpdateScreen();
		SDL_Delay(30);
	}
}
inline void pause()
{
	while (true)
	{
		Update();
		if (event.key.keysym.sym == SDLK_q)
		{
			break;
		}
		SDL_BlitSurface(pausecover, screen, 0, -100);
		SDL_BlitSurface(resume, screen, 100, 500);
		SDL_BlitSurface(menubotton, screen, 100, 600);
		if (Mouse.x > 100 && Mouse.x <= 253 && Mouse.y >= 500 && Mouse.y <= 560)
		{
			SDL_BlitSurface(resumeon, screen, 100, 500);
			if (Mouse.left)
			{
				break;
			}
		}
		if (Mouse.x > 100 && Mouse.x <= 253 && Mouse.y >= 600 && Mouse.y <= 660)
		{
			SDL_BlitSurface(menubottonon, screen, 100, 600);
			if (Mouse.left)
			{
				mainmenu();
				break;
			}
		}
		SDL_UpdateScreen();
		SDL_Delay(30);
	}
}
inline void physics(int& x, int& y, float& dy)
{
	dy += 0.5;
	y += dy;
	if (y>900)
	{
		d.lose = true;
		PlaySound(losesound, 1);
	}
	if (event.key.keysym.sym == SDLK_RIGHT)
	{
		x += 7;
	}
	if (event.key.keysym.sym == SDLK_LEFT)
	{
		x -= 7;
	}
}
inline void helifunction(int& x,int& y,float& dy)
{
	if (y + 30 <= plat[8].y && y + 120 >= plat[8].y && x + 80 > plat[8].x + 25 && x + 40 < plat[8].x + 25 + 59 && d.heli == 0 && !d.jet)
	{
		dy = -50;
		PlaySound(helisound, 1);
		d.heli++;
	}
	if (d.heli<80 && d.heli>0)
	{
		d.heli++;
		if (d.heli == 79)
			d.heli = 0;
		if (d.heli % 2 == 0)
			SDL_BlitSurface(likheli1, screen, x, y);
		else
			SDL_BlitSurface(likheli2, screen, x, y);
	}
	//if (d.heli < 50 && d.heli>39)
	//{
	//	d.heli++;
	//	if (d.heli == 49)
	//		d.heli = 0;
	//	SDL_BlitSurface(doodle, screen, x, y);
	//	SDL_BlitSurface(heli, screen, x - 5, y);
	//}
}
inline void jetfunction(int& x, int& y, float& dy)
{
	if (y + 30 <= plat[9].y && y + 120 >= plat[9].y && x + 80 > plat[9].x + 25 && x + 40 < plat[9].x + 25 + 50 && d.jet == 0 && !d.heli)
	{
		dy = -70;
		PlaySound(jetsound, 1);
		d.jet++;
	}
	if (d.jet < 120 && d.jet>0)
	{
		d.jet++;
		if (d.jet < 40)
			SDL_BlitSurface(likjet2, screen, x, y);
		if (d.jet >= 40 && d.jet < 80)
			SDL_BlitSurface(likjet1, screen, x, y);
		if (d.jet >= 80)
			SDL_BlitSurface(likjet3, screen, x, y);
		if (d.jet == 119)
		{
			d.jet = 0;
		}
	}
}
void fakeplatform(int& x, int& y,float& dy, int fakeplatpos[])
{
	for (int i = 0; i < 5; i++)
	{
		if (!d.fakeplat[i])
			SDL_BlitSurface(fakeplat1, screen, fakep[i].x, fakep[i].y);
	}
	for (int i = 0; i < 5; i++)
	{
		if (x + 85 >= plat[i].x && x + 45 <= plat[i].x + 114 && y + 120 <= plat[i].y + 30 && y + 120 >= plat[i].y && dy >= 0 && d.fakeplat[i] == 0)
		{
			fakep[i].x += 1000;
			fakep[i].y += 1000;
			PlaySound(faketouchsound, 1);
			d.fakeplat[i]++;
		}
		if (d.fakeplat[i]>0 && d.fakeplat[i]<120)
		{
			SDL_BlitSurface(fakeplat2, screen, plat[i].x, plat[i].y + fakeplatpos[i]);
			fakeplatpos[i] += 7;
			d.fakeplat[i]++;
			if (d.fakeplat[i] == 119)
			{
				d.fakeplat[i] = 0;
				fakeplatpos[i] = 0;
			}
		}
	}
}