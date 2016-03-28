#pragma once

#include <stdlib.h>
#include <glut.h>
#include <cmath>
#include <vector>
#include <time.h>

using namespace std;

const double PI=atan(1.0f)*4;

extern enum direction{right, left, up, down};

static const int screenWidth=240, screenHeight=168;

static bool ground[screenHeight][screenWidth];
static bool ground_copy[screenHeight][screenWidth];

static int Width=960, Height=672;

class Bullet;

class Digger
{
	protected:
		float x, y;//координати центру
		float x_eye, y_eye;
		float speed;
	public:
		direction dir;
		float radius;
		bool alive;
		Digger(float x_=Width/2, float y_=Height/2, float radius_=16.0, direction dir_=direction(0), float speed=8.0, bool alive_=1);
		void Draw();
		void move_up();
		void move_down();
		void move_right();
		void move_left();
		Bullet fire();
		~Digger();
		friend bool isGround(Digger bot);
		friend class Bullet;
		friend void DrawLevel1();
		friend void BotMove(Digger & bot, int field_copy[672/32][960/32]);
		friend void DrawBullets(vector<Bullet> & bullets, vector<Digger> & bots);
};

class Bullet
{
		float x, y;
		direction dir;
		float radius;
		float speed;
	public:
		bool is;
		Bullet(float x_=0, float y_=0, direction dir=direction::right, float radius_=5.0, float speed_=16.0, bool is_=true);
		void move();
		void Draw();
		friend void DrawLevel1();
		friend void DrawBullets(vector<Bullet> & bullets, vector<Digger> & bots);
		~Bullet(){x=0.0, y=0.0, radius=0.0, speed=0.0, is=false;};
};

class Gold
{
		float x, y;
		float radius;
	public:
		Gold(float x_, float y_, float radius_=6);
		void Draw();
		friend void DrawLevel1();
		friend void DrawBullets(vector<Bullet> & bullets, vector<Digger> & bots);
		~Gold(){x=0,y=0, radius=0;};
};