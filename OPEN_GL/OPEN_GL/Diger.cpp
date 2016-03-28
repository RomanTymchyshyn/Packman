#include "Diger.h"

Digger::Digger(float x_, float y_, float radius_, direction dir_, float speed_, bool alive_)
{
	x=x_;
	y=y_;
	radius=radius_;
	dir=dir_;
	if (dir==right)
	{
		x_eye=x;
		y_eye=y+radius/2.0;
	}
	if (dir==left)
	{
		x_eye=x;
		y_eye=y+radius/2.0;
	}
	if (dir==up)
	{
		x_eye=x+radius/2.0;
		y_eye=y;
	}
	if (dir==left)
	{
		x_eye=x+radius/2.0;
		y_eye=y;
	}
	alive=alive_;
	speed=speed_;
}

Digger::~Digger()
{
	x=0.0;
	y=0.0;
	x_eye=0.0;
	y_eye=0.0;
	radius=0.0;
	alive=false;
}

void Digger::move_up()
{
	if (y < Height - radius && alive)
	{
		switch(dir)
		{
			case up:
				y+=speed;
				dir=up;
				y_eye=y;
				x_eye=x+radius/2.0;
				break;
			case right:
				if (fabs((x-16.0)/(2.0*radius) - int((x-16.0)/(2.0*radius))) < 0.001 )
				{
					y+=speed;
					dir=up;
					y_eye=y;
					x_eye=x+radius/2.0;
				}
				else move_right();
				break;
			case left:
				if (fabs((x-16.0)/(2.0*radius) - int((x-16.0)/(2.0*radius))) < 0.001 )
				{
					y+=speed;
					dir=up;
					y_eye=y;
					x_eye=x+radius/2.0;
				}
				else move_left();
				break;
			default:
				y+=speed;
				dir=up;
				y_eye=y;
				x_eye=x+radius/2.0;
				break;
		}
	}
}

void Digger::move_down()
{
	if (y > 0 - radius/6.0 + speed + radius && alive)
	{
		switch(dir)
		{
			case down:
				y-=speed;
				dir=down;
				y_eye=y;
				x_eye=x-radius/2.0;
				break;
			case right:
				if (fabs((x-16.0)/(2.0*radius) - int((x-16.0)/(2.0*radius))) < 0.001 )
				{
					y-=speed;
					dir=down;
					y_eye=y;
					x_eye=x-radius/2.0;
				}
				else move_right();
				break;
			case left:
				if (fabs((x-16.0)/(2.0*radius) - int((x-16.0)/(2.0*radius))) < 0.001 )
				{
					y-=speed;
					dir=down;
					y_eye=y;
					x_eye=x-radius/2.0;
				}
				else move_left();
				break;
			default:
				y-=speed;
				dir=down;
				y_eye=y;
				x_eye=x-radius/2.0;
				break;
		}
	}
}

void Digger::move_right()
{
	if (x < Width - radius && alive)
	{
		switch(dir)
		{
			case right:
				x+=speed;
				dir=right;
				y_eye=y+radius/2.0;
				x_eye=x;
				break;
			case up:
				if (fabs((y-16.0)/(2.0*radius) - int((y-16.0)/(2.0*radius))) < 0.001 )
				{
					x+=speed;
					dir=right;
					y_eye=y+radius/2.0;
					x_eye=x;
				}
				else move_up();
				break;
			case down:
				if (fabs((y-16.0)/(2.0*radius) - int((y-16.0)/(2.0*radius))) < 0.001 )
				{
					x+=speed;
					dir=right;
					y_eye=y+radius/2.0;
					x_eye=x;
				}
				else move_down();
				break;
			default:
				x+=speed;
				dir=right;
				y_eye=y+radius/2.0;
				x_eye=x;
				break;
		}
	}
}

void Digger::move_left()
{
	if (x > radius && alive)
	{
		switch(dir)
		{
			case left:
				x-=speed;
				dir=left;
				y_eye=y+radius/2.0;
				x_eye=x;
				break;
			case up:
				if (fabs((y-16.0)/(2.0*radius) - int((y-16.0)/(2.0*radius))) < 0.001 )
				{
					x-=speed;
					dir=left;
					y_eye=y+radius/2.0;
					x_eye=x;
				}
				else move_up();
				break;
			case down:
				if (fabs((y-16.0)/(2.0*radius) - int((y-16.0)/(2.0*radius))) < 0.001 )
				{
					x-=speed;
					dir=left;
					y_eye=y+radius/2.0;
					x_eye=x;
				}
				else move_down();
				break;
			default:
				x-=speed;
				dir=left;
				y_eye=y+radius/2.0;
				x_eye=x;
				break;
		}
	}
}

Bullet Digger::fire()
{
	Bullet bullet(x,y,dir,5,16.0,true);

	return bullet;
}

void Digger::Draw()
{
	if (!alive) return;
	glColor3b(150, 120, 255);
	static double a=0;
	static int c=0;
	if (c==0)
	{
		a+=0.2;
		if (a>1.0) c++;
	}
	else
		if (c==1)
		{
			a-=0.2;
			if (a<0) c--;
		}
	double t0, t1;
	if (dir==right)
	{
		t0=a;
		t1=2.0*PI-a;
	}
	if (dir==left)
	{
		t0=PI+a;
		t1=3.0*PI-a;
	}
	if (dir==up)
	{
		t0=PI/2.0+a;
		t1=(5.0*PI)/2-a;
	}
	if (dir==down)
	{
		t0=(3.0*PI)/2.0+a;
		t1=(7.0*PI)/2.0-a;
	}
    glBegin( GL_TRIANGLE_FAN );
               glVertex2d(x, y);// вершина в центрі круга
               for(double i = t0; i <= t1 ; i+=0.35 )
                   glVertex2d(cos( i ) * radius + x, sin( i ) * radius + y);
    glEnd();
	glColor3b(0, 0, 0);
	
	glBegin( GL_TRIANGLE_FAN );
			glVertex2d(x_eye, y_eye);// вершина в центрі круга
			for(double i = 0.; i <= 6.40; i+=0.05 )
                   glVertex2d(cos( i ) * radius/6.0 + x_eye, sin( i ) * radius/6.0 + y_eye);
    glEnd();
}