#include "Diger.h"
#include <fstream>
#include <iostream>
#include <queue>
//#include "fontsub.h"

using namespace std;

#define COOL_DOWN 3.0

Digger dig(27*16,19*16);
Digger dig_copy;
bool UP=false, DOWN=false, RIGHT=false, LEFT=false;
bool space=false;
static int coef=Width/screenWidth;
int R=int(Height/(2.0*dig.radius)), C=int(Width/(2.0*dig.radius));
int field[672/32][960/32]={0};
bool gold[672/32][960/32]={0};
int field_copy[672/32][960/32]={0};
bool gold_copy[672/32][960/32]={0};
const int difficulty=10;
static int SCORE=0;
static int LIVES=3;
static Digger bot1(432,16);
static Digger bot2(864,16);
static vector<Digger> bots;
static vector<Digger> bots_copy;
float t;
float dif=COOL_DOWN;

struct coord
{
	int i;
	int j;
};

void YouLoser()
{
	glClearColor(0,0,0,0);
	glColor3f(1.0,0,0);
	glLineWidth(10);
	char string[16]="YOU LOSE :(";
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0,Height/2,0);
	for (int i=0; i<strlen(string); ++i)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);
	glPopMatrix();
}

void YouWin()
{
	glClearColor(0,0,0,0);
	glColor3f(1.0,0,0);
	glLineWidth(10);
	char string[16]="YOU WIN :)";
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0,Height/2,0);
	glViewport(0,0,Width/2.0,Height/2.0);
	for (int i=0; i<strlen(string); ++i)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);
	glPopMatrix();
	glViewport(0,0,Width,Height);
}

void DrawScore()
{
	bool bad=false;
	for (int i=0; i< bots.size(); ++i)
		if (bots[i].alive==true) bad=true;
	if (LIVES!=0 && bad)
	{
		glClearColor(0,0,0,0);
		glLineWidth(4);
		char string[16]="SCORE: ";
		char h[5];
		_itoa(SCORE,h,10);
		strcat_s(string, h);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(0,Height/2,0);
		for (int i=0; i<strlen(string); ++i)
		{
			glColor3ub(1.0,0.0,0);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);
		}
		glPopMatrix();
		return;
	}
	else return;
}

bool isGround(Digger bot)
{
	switch(bot.dir)
	{
		case up:
			for (int j=int(bot.x-bot.radius)/coef; j<int(bot.x+bot.radius)/coef; ++j)
				if (ground[screenHeight-int(bot.y+bot.radius+bot.speed)/coef-1][j]==true) return false;
			break;
		case down:
			for (int j=int(bot.x-bot.radius)/coef; j<int(bot.x+bot.radius)/coef; ++j)
				if (ground[screenHeight-int(bot.y-bot.radius-bot.speed)/coef-1][j]==true) return false;
			break;
		case direction::right:
			for (int i=int(screenHeight-int(bot.y-bot.radius)/coef-1); i<int(screenHeight-int(bot.y+bot.radius)/coef-1); ++i)
				if (ground[i][int(bot.x+bot.radius)/coef]==true) return true;
			break;
		default:
			for (int i=int(screenHeight-int(bot.y-bot.radius)/coef-1); i<int(screenHeight-int(bot.y+bot.radius)/coef-1); ++i)
				if (ground[i][int(bot.x-bot.radius-bot.speed)/coef]==true) return false;
			break;
	}
	return true;
}

void SaveField(int field_copy[672/32][960/32])
{
	for (int i=0; i<R; ++i)
		for (int j=0; j<C; ++j)
			field_copy[i][j]=field[i][j];
	return;
}

void PushLevel()
{
	for (int i=0; i<screenHeight; ++i)
		for (int j=0; j<screenWidth; ++j)
			ground_copy[i][j]=ground[i][j];
	for (int i=0; i<screenHeight/(2.0*dig.radius); ++i)
		for (int j=0; j<screenWidth/(2.0*dig.radius); ++j)
		{
			field_copy[i][j]=field[i][j];
			gold_copy[i][j]=gold[i][j];
		}
	dig_copy=dig;
	for (int i=0; i<bots.size(); ++i)
		bots_copy.push_back(Digger(bots[i]));
}

void PopLevel()
{
	for (int i=0; i<screenHeight; ++i)
		for (int j=0; j<screenWidth; ++j)
			ground[i][j]=ground_copy[i][j];
	for (int i=0; i<screenHeight/(2.0*dig.radius); ++i)
		for (int j=0; j<screenWidth/(2.0*dig.radius); ++j)
		{
			field[i][j]=field_copy[i][j];
			gold[i][j]=gold_copy[i][j];
		}
	dig=dig_copy;
	bots.clear();
	for (int i=0; i<bots_copy.size(); ++i)
		bots.push_back(bots_copy[i]);
}

void SeeCoord(int row, int col, int new_row, int new_col, queue<coord> & q, int field_copy[672/32][960/32])
{
	if (field_copy[new_row][new_col]==0)
	{
		field_copy[new_row][new_col]=field_copy[row][col]+1;
		coord next;
		next.i=new_row;
		next.j=new_col;
		q.push(next);
	}
	return;
}

void RunWave(coord c, int field_copy[672/32][960/32])
{
	queue<coord> q;
	q.push(c);
	while (!q.empty())
	{
		c=q.front();
		int iT=c.i+1;
		int iB=c.i-1;
		int jR=c.j+1;
		int jL=c.j-1;

		if (iT<R) SeeCoord(c.i, c.j, iT, c.j, q, field_copy);
		if (iB>=0) SeeCoord(c.i, c.j, iB, c.j, q, field_copy);
		if (jR<C) SeeCoord(c.i, c.j, c.i, jR, q, field_copy);
		if (jL>=0) SeeCoord(c.i, c.j, c.i, jL, q, field_copy);
		q.pop();
	}
}

void find_shortest_way(coord target, int field_copy[][960/32])
{
	if (field_copy[target.i][target.j]==0) return;
	while (field_copy[target.i][target.j]!=1)
	{
		if ((target.j>=1) && field_copy[target.i][target.j-1]==field_copy[target.i][target.j]-1)
		{
			field_copy[target.i][target.j]=-2;
			target.j-=1;
		}
		else if ((target.j<C-1) && field_copy[target.i][target.j+1]==field_copy[target.i][target.j]-1)
		{
			field_copy[target.i][target.j]=-2;
			target.j+=1;
		}
		else if ((target.i>=1) && field_copy[target.i-1][target.j]==field_copy[target.i][target.j]-1)
		{
			field_copy[target.i][target.j]=-2;
			target.i-=1;
		}
		else if((target.i<C-1) && field_copy[target.i+1][target.j]==field_copy[target.i][target.j]-1)
		{
			field_copy[target.i][target.j]=-2;
			target.i+=1;
		}
	}
	field_copy[target.i][target.j]=-2;
	return;
}

void level1(vector<vector<char>> l)
{
	int coef=Height/screenHeight;
	for (int i=0; i<R; ++i)
	{
		for (int j=0; j<C; ++j)
		{
			if (l[i][j]=='x')
			{
				for (int row=int(i*2*dig.radius)/coef; row<int((i+1)*2*dig.radius)/coef; ++row)
					for (int col=int(j*2*dig.radius)/coef; col<int((j+1)*2*dig.radius)/coef; ++col)
						ground[row][col]=true;
				field[R-i-1][j]=-1;
				gold[R-i-1][j]=false;
			}
			else if(l[i][j]=='g')
			{
				for (int row=int(i*2*dig.radius)/coef; row<int((i+1)*2*dig.radius)/coef; ++row)
					for (int col=int(j*2*dig.radius)/coef; col<int((j+1)*2*dig.radius)/coef; ++col)
						ground[row][col]=true;
				field[R-i-1][j]=-1;
				gold[R-i-1][j]=true;
			}
			else
			{
				for (int row=int(i*2*dig.radius)/coef; row<int((i+1)*2*dig.radius)/coef; ++row)
					for (int col=int(j*2*dig.radius)/coef; col<int((j+1)*2*dig.radius)/coef; ++col)
						ground[row][col]=false;
				field[R-i-1][j]=0;
				gold[R-i-1][j]=false;
			}
		}
	}
	bots.push_back(bot1);
	bots.push_back(bot2);
	PushLevel();
}

void DrawBullets(vector<Bullet> & bullets, vector<Digger> & bots)
{
	for (int k=0; k<bullets.size(); ++k)
	{
		if (bullets[k].is)
		{
			for (int i=0; i<bots.size(); ++i)
			{
				if (fabs(bullets[k].x-bots[i].x)<=bullets[k].radius+bots[i].radius && fabs(bullets[k].y-bots[i].y)<=bullets[k].radius+bots[i].radius)
				{
					bots[i].alive=false;
					bots.erase(bots.begin()+i);
					bullets[k].is=false;
					break;
				}
			}
			glColor3b(120,150,255);
			bullets[k].Draw();
			if (ground[screenHeight-int(bullets[k].y)/coef-1][int(bullets[k].x)/coef]==false)
			{		
				glColor3b(150, 120, 255);
				bullets[k].move();
			}
			else
			{
				bullets[k].radius=dig.radius;
				glColor3b(255, 0, 0);
				for (int i=(bullets[k].y-bullets[k].radius)/coef; i<(bullets[k].y+bullets[k].radius)/coef; ++i)
				{
					if (i<screenHeight && i>=0)
					{
						for (int j=(bullets[k].x-bullets[k].radius)/coef; j<(bullets[k].x+bullets[k].radius)/coef; ++j)
						{
							if (j<screenWidth && j>=0)
								ground[screenHeight-i-1][j]=false;
							if (gold[i/(screenHeight/R)][j/(screenWidth/C)]==true)
								gold[i/(screenHeight/R)][j/(screenWidth/C)]=false;
						}
					}
				}
				bullets[k].is=false;
				bullets.erase(bullets.begin()+k);
			}	
		}
		else bullets.erase(bullets.begin()+k);
	}
	return;
}

void DrawGround()
{
	for (int i=0; i<screenHeight; i+=1)
		for (int j=0; j<screenWidth; j+=1)
			if (ground[i][j]==true)
			{
				glColor3ub(255,255,255);
					glRectf(float((j)*coef),float((screenHeight-i-1)*coef),float((j)*coef+coef), float((screenHeight-i-1)*coef+coef));
				glEnd();
			}
	return;
}

void DrawGold()
{
	for (int i=0; i<int(Height/(2.0*dig.radius)); ++i)
		for (int j=0; j<int(Width/(2.0*dig.radius)); ++j)
			if (gold[i][j]==true)
			{
				glColor3ub(255,255,51);
				Gold g(float(j)*2.0*dig.radius+16.0, float(i)*2.0*dig.radius+16.0);
				g.Draw();
			}
}

void BotMove(Digger & bot, int field_copy[672/32][960/32])
{
	int coef=int(Height/screenHeight);
	if (abs(int((dig.x-bot.x))/(2*dig.radius))<=difficulty && abs(int((dig.y-bot.y))/(2*dig.radius))<=difficulty)
	{
		int i=int(bot.y/(2.0*bot.radius));
		int j=int(bot.x/(2.0*bot.radius));
		int iT=i+1;
		int iB=i-1;
		int jR=j+1;
		int jL=j-1;

		if (jR<=C-1 && field_copy[i][jR]==-2 && ground[screenHeight-int(bot.y)/coef-1][int(bot.x+bot.radius)/coef]==false) bot.move_right();
		if (jL>=0 && field_copy[i][jL]==-2 && ground[screenHeight-int(bot.y)/coef-1-1][int(bot.x-bot.radius)/coef-1]==false) bot.move_left();
		if (iT<=R-1 && field_copy[iT][j]==-2 && ground[screenHeight-int(bot.y+bot.radius)/coef-1][int(bot.x)/coef]==false) bot.move_up();
		if (iB>=0 && field_copy[iB][j]==-2 && ground[screenHeight-int(bot.y-bot.radius)/coef-1][int(bot.x)/coef]==false) bot.move_down();
	}
	else
	{
		srand((unsigned) time (NULL));
		direction dir_new=direction(rand()%4);
		switch(dir_new)
		{
			case up:
				if (ground[screenHeight-int(bot.y+bot.radius+bot.speed)/coef-1][int(bot.x)/coef]==false) 
					bot.move_up();
				break;
			case down:
				if (ground[screenHeight-int(bot.y-bot.radius-bot.speed)/coef-1][int(bot.x)/coef]==false)
					bot.move_down();
				break;
			case direction::right:
				if (ground[screenHeight-int(bot.y)/coef-1][int(bot.x+bot.radius)/coef]==false) 
					bot.move_right();
				break;
			default:
				if (ground[screenHeight-int(bot.y)/coef-1-1][int(bot.x-bot.radius)/coef-1]==false) 
					bot.move_left();
				break;
		}
	}
}

void DrawLevel1()
{
	int coef=Width/screenWidth;
	for (int i=(dig.y-dig.radius)/coef; i<(dig.y+dig.radius)/coef; ++i)
	{
		if (i<screenHeight && i>=0)
		{
			for (int j=(dig.x-dig.radius)/coef; j<(dig.x+dig.radius)/coef; ++j)
			{
				if (j<screenWidth && j>=0)
					ground[screenHeight-i-1][j]=false;
				if (gold[i/(screenHeight/R)][j/(screenWidth/C)]==true)
				{
					gold[i/(screenHeight/R)][j/(screenWidth/C)]=false;
					SCORE+=50;
				}
			}
		}
	}
	
	for (int i=0; i<Height/int(2*dig.radius); i++)
	{
		for (int j=0; j<Width/int(2*dig.radius);++j)
		{
			bool ok=true;
			for (int row=int(i*2*dig.radius)/coef; row<int((i+1)*2*dig.radius)/coef && ok; ++row)
				for (int col=int(j*2*dig.radius)/coef; col<int((j+1)*2*dig.radius)/coef && ok; ++col)
				{
					if (ground[row][col]==false)
					{
						field[R-i-1][j]=0;
						ok=false;
					}
				}
			if (ok) field[R-i-1][j]=-1;	
		}
	}

	static int field_copy[672/32][960/32];
	
	for (int k=0; k<bots.size(); ++k)
	{
		SaveField(field_copy);
		coord start;
		start.i=int(bots[k].y/(2.0*bots[k].radius));
		start.j=int(bots[k].x/(2.0*bots[k].radius));
		field_copy[start.i][start.j]=1;
		coord target;
		target.i=int(dig.y/(2.0*dig.radius));
		target.j=int(dig.x/(2.0*dig.radius));
		RunWave(start, field_copy);
		find_shortest_way(target, field_copy);
		BotMove(bots[k], field_copy);
 		if (bots[k].alive) bots[k].Draw();
		if (fabs(bots[k].x-dig.x)<2*dig.radius && fabs(bots[k].y-dig.y)<2*dig.radius)
		{
			dig.alive=false;
			dig.Draw();
			LIVES--;
			PopLevel();
			glutPostRedisplay();
			return;
		}
	}

	dig.Draw();

	static vector <Bullet> bullets;
	Bullet temp;

	if (UP)
	{
		LEFT=false;
		RIGHT=false;
		dig.move_up();
	}
	if (DOWN)
	{
		LEFT=false;
		RIGHT=false;
		dig.move_down();
	}
	if (RIGHT) dig.move_right();
	if (LEFT) dig.move_left();
	dif=(float)clock()/CLOCKS_PER_SEC-t;
	if (space && dig.alive && bullets.empty() && dif>=COOL_DOWN)
	{
		t=(float)clock()/CLOCKS_PER_SEC;
		temp=dig.fire();
		bullets.push_back(temp);
	}
	
	DrawBullets(bullets, bots);
	DrawGround();
	DrawGold();
}

void Display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3b(150, 120, 255);
	DrawLevel1();
	DrawScore();
	if (LIVES==0) YouLoser();
	if (bots.empty()) YouWin();
	glFinish();
}

void Reshape(GLint w, GLint h)
{
  Width = w;
  Height = h;

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Keyboard(unsigned char key, int x, int y)
{
	#define ESCAPE 27
	#define SPACE 32
	switch (key)
	{
		case ' ':
			space=true;
			break;
		case ESCAPE:
			exit(0);
			break;
	}
}

void KeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
		case ' ':
			space=false;
			break;
	}
}

void SpecialKeyBoard(int k, int x, int y)
{
	switch (k)
	{
		case GLUT_KEY_DOWN:
			DOWN=true;
			break;
		case GLUT_KEY_UP:
			UP=true;
			break;
		case GLUT_KEY_LEFT:
			LEFT=true;
			break;
		case GLUT_KEY_RIGHT:
			RIGHT=true; 
			break;
	}
	/*if (k==GLUT_KEY_DOWN) dig.move_down();
	if (k==GLUT_KEY_UP) dig.move_up();
	if (k==GLUT_KEY_LEFT) dig.move_left();
	if (k==GLUT_KEY_RIGHT) dig.move_right();*/
	return;
}

void SpecialKeyBoardUp(int k, int x, int y)
{
	switch (k)
	{
		case GLUT_KEY_DOWN:
			DOWN=false;
			break;
		case GLUT_KEY_UP:
			UP=false;
			break;
		case GLUT_KEY_LEFT:
			LEFT=false;
			break;
		case GLUT_KEY_RIGHT:
			RIGHT=false; 
			break;
	}
}

void tmf(int val)
{
	if (LIVES!=0) glutPostRedisplay();
	glutTimerFunc(32,tmf,0);
}

void main(int argc, char *argv[])
{

	ifstream fi("level.txt");
	int row = int(Height/(2*dig.radius));
	int col = int(Width/(2*dig.radius));
	vector<vector<char>> l;
	vector <char> temp(col);
	for (int i=0; i<row; ++i)
	{
		for(int j=0; j<col; ++j)
			fi>>temp[j];
		l.push_back(temp);
	}
	
	level1(l);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Digger");
	
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutTimerFunc(32,tmf,0);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(SpecialKeyBoard);
	glutSpecialUpFunc(SpecialKeyBoardUp);
	
	glutMainLoop();
}