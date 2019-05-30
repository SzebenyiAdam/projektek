#include "Model.h"
#include "Variables.h"
#include "Utilities.h"
#include <windows.h>
#include <SOIL.h>
#include <GLUT.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

using namespace std;
inline float ToRad(float angle_degrees) {
	return angle_degrees * M_PI / 180.0f;
}

float xrot = -15, yrot = 0;
float xpos = 0, ypos = 3, zpos = 0;
double secs;

GLfloat light_ambient[] = { 0.8f,0.8f,0.8f,1.0f };

GLuint textures[26];

Variables v;
Utilities u;

Model_reader main_game_obj;
Model_reader end_zone;
Model_reader playable_world;
Model_reader tree_lower;
Model_reader tree_upper;
Model_reader mountain;
Model_reader star;

void Collect_stars()
{
	if ((zpos >= 19.0f) && (zpos < 21.0f))
	{
		v.HideStar1();
	}
	if ((zpos >= 9.0f) && (zpos < 12.0f) && (xpos > 7.5f))
	{
		v.HideStar2();
	}
	if ((zpos >= 9.0f) && (zpos < 12.0f) && (xpos < -17.5f))
	{
		v.HideStar3();
	}
	if ((zpos >= 27.0f) && (zpos < 29.0f) && (xpos < -5.0f))
	{
		v.HideStar4();
	}
	if ((zpos >= 27.0f) && (zpos < 29.0f) && (xpos < 7.0f) && (xpos > 5.0f))
	{
		v.HideStar5();
	}
	if ((zpos >= 27.0f) && (zpos < 29.0f) && (xpos > 21.0f))
	{
		v.HideStar6();
	}
	if ((zpos < 23.0f) && (xpos > 10.0f) && (xpos < 13.0f))
	{
		v.HideStar7();
	}
	if ((zpos > 49.0f) && (zpos < 52.0f) && (xpos > 10.0f) && (xpos < 13.0f))
	{
		v.HideStar8();
	}
	if ((zpos > 55.0f) && (xpos > 24.5f))
	{
		v.HideStar9();
	}
	if ((zpos > 55.0f) && (xpos < -0.5f))
	{
		v.HideStar10();
	}
}

bool Game_over()
{
	if ((xpos > -0.5f) && (xpos < 0.5f) && (zpos < 28.7f) && (zpos > -1.0f))
	{
		return false;
	}
	if ((zpos < 28.7f) && (zpos > 27.7f) && (xpos < 22.2f) && (xpos > -6.2f))
	{
		return false;
	}
	if ((xpos > 11.5f) && (xpos < 12.5f) && (zpos < 104.0f) && (zpos > 21.0f))
	{
		return false;
	}
	if ((zpos < 10.7f) && (zpos > 9.7f) && (xpos < 9.4f) && (xpos > -19.3f))
	{
		return false;
	}
	if ((zpos < 70.9f) && (zpos > 69.9f) && (xpos < 26.1f) && (xpos > -2.3f))
	{
		return false;
	}
	return true;
}

void Wait_one_sec(double elapsed_time)
{
	if (((v.GetGoFw()) && (v.GetGoBw())) || ((v.GetGoRight()) && (v.GetGoLeft())))
	{
		secs += elapsed_time;
		if (secs >= v.GetWaiting())
		{
			v.StopMoving();
		}
	}
	else
	{
		secs = 0;
	}
	//cout << secs << endl;
}

void timer(int) {
	glutPostRedisplay();
	Collect_stars();
	v.DifficultyHandler();
	double elapsed_secs = u.calcElapsedTime();
	Wait_one_sec(elapsed_secs);
	v.RotateStars(elapsed_secs);
	glutTimerFunc(1000 / 120, timer, 0);

	if (v.GetGoFw())
	{
		xpos += (float)sin(yrot*3.14f / 180.0f) * v.GetSpeed() * elapsed_secs;
		zpos += (float)cos(yrot*3.14f / 180.0f) * v.GetSpeed() * elapsed_secs;
		v.IncBallZRot(elapsed_secs);
		Game_over();
	}
	if (v.GetGoBw())
	{
		xpos -= (float)sin(yrot*3.14f / 180.0f) * v.GetSpeed() * elapsed_secs;
		zpos -= (float)cos(yrot*3.14f / 180.0f) * v.GetSpeed() * elapsed_secs;
		v.DecBallZRot(elapsed_secs);
		Game_over();
	}

	if (v.GetGoLeft())
	{
		xpos += (float)cos(yrot*3.14f / 180.0f) * v.GetSpeed() * elapsed_secs;
		zpos += (float)sin(yrot*3.14f / 180.0f) * -v.GetSpeed() * elapsed_secs;
		v.DecBallXRot(elapsed_secs);
		Game_over();
	}

	if (v.GetGoRight())
	{
		xpos -= (float)cos(yrot*3.14f / 180.0f) * v.GetSpeed() * elapsed_secs;
		zpos -= (float)sin(yrot*3.14f / 180.0f) * -v.GetSpeed() * elapsed_secs;
		v.IncBallXRot(elapsed_secs);
		Game_over();
	}

	if ((v.GetGoFw()) || (v.GetGoBw()))
	{
		v.SetBallXRotDefault();
	}
	if ((v.GetGoRight()) || (v.GetGoLeft()))
	{
		v.SetBallZRotDefault();
	}

	if (v.GetDefaultPos())
	{
		if (xpos > 1.0f)
		{
			xpos -= 0.5f;
		}
		else if (xpos < -1.0f)
		{
			xpos += 0.5f;
		}
		else if ((xpos >= -1.0f) && (xpos <= 1.0f))
		{
			xpos = 0;
			v.SetStarsDefault();
		}

		if (zpos > 1.0f)
		{
			zpos -= 0.5f;
		}
		else if (zpos < -1.0f)
		{
			zpos += 0.5f;
		}
		else if ((zpos >= -1.0f) && (zpos <= 1.0f))
		{
			zpos = 0;
			v.SetStarsDefault();
		}
	}
}

bool LoadTextures()
{
	char *texture_names[] = { "Data/ground.jpg","Data/sky.png","Data/gameover.jpg","Data/won.png","Data/remaining.png","Data/0.png","Data/1.png","Data/2.png","Data/3.png","Data/4.png","Data/5.png" 
									,"Data/6.png","Data/7.png","Data/8.png","Data/9.png","Data/10.png","Data/go_end.png","Data/menu.png","Data/help.png","Data/mark.png","Data/wrong.png"
									,"Data/v_easy.png","Data/easy.png","Data/medium.png","Data/hard.png","Data/impossible.png" };

	for (int i = 0; i < 26; ++i) {
		textures[i] = SOIL_load_OGL_texture
		(texture_names[i],
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);
		if (textures[i] == 0) {
			cout << "Not found, or corrupt " << texture_names[i] << " .....\n";
			return false;
		}
		else cout << "Loaded " << texture_names[i] << " \n";
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	return true;
}

void Loading_models()
{
	main_game_obj.load("Data/ball.obj", 0.01f, "Data/ball.jpg");
	playable_world.load("Data/steelbeam.obj", 1.0f, "Data/steelbeam.png");
	tree_lower.load("Data/tree_lower.obj", 5.0f, "Data/tree_lower.jpg");
	tree_upper.load("Data/tree_upper.obj", 5.0f, "Data/tree_upper.png");
	mountain.load("Data/mountain.obj", 25.0f, "Data/mountain.png");
	star.load("Data/star.obj", 0.08f, "Data/star.png");
	end_zone.load("Data/steelbeam.obj", 1.0f, "Data/steelbeam_green.png");
}

void Draw_star(float x, float y, float z)
{
	glPushMatrix();
	glRotatef(90.0f, 1, 0, 0);
	glTranslatef(x, y, z);
	glRotatef(v.GetRotateStars(), 0, 0, 1);
	star.draw(0, 0, 0);
	glPopMatrix();
}

void Main_game_object()
{
	glPushMatrix();
	glTranslatef(xpos, 0, zpos + 10);
	glRotatef(v.GetBallZRot(), 1, 0, 0);
	glRotatef(v.GetBallXRot(), 0, 0, 1);
	main_game_obj.draw(0, 0, 0);
	glPopMatrix();

}

void Draw_far_objs()
{
	mountain.draw(-150, -7, 300);
	mountain.draw(-100, -7, 500);
	mountain.draw(130, -7, 300);
	mountain.draw(130, -7, 100);
}

void Draw_steel_beam_in_other_direction(float x, float y, float z)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(90.0f, 0, 1, 0);
	playable_world.draw(0, 0, 0);
	glPopMatrix();
}

void Draw_tree(float x, float y, float z)
{
	tree_lower.draw(x, y, z);
	tree_upper.draw(x, y, z);
}

void Draw_models()
{
	Draw_far_objs();

	Main_game_object();
	Draw_steel_beam_in_other_direction(0, -2.5, 23);
	playable_world.draw(8.0, -2.5, 38);
	Draw_steel_beam_in_other_direction(12, -2.5, 45);
	Draw_steel_beam_in_other_direction(12, -2.5, 73);

	glPushMatrix();
	glTranslatef(12, -2.5, 101.5);
	glRotatef(90.0f, 0, 1, 0);
	end_zone.draw(0, 0, 0);
	glPopMatrix();

	playable_world.draw(-5, -2.5, 20);
	playable_world.draw(12, -2.5, 80);

	if (v.GetStar1())
	{
		Draw_star(0, 30, -0.1);
	}
	else
	{
		v.IncStarsArray(0);
	}

	if (v.GetStar2())
	{
		Draw_star(8.5, 20, -0.1);
	}
	else
	{
		v.IncStarsArray(1);
	}

	if (v.GetStar3())
	{
		Draw_star(-18.5, 20, -0.1);
	}
	else
	{
		v.IncStarsArray(2);
	}

	if (v.GetStar4())
	{
		Draw_star(-5.5, 38, -0.1);
	}
	else
	{
		v.IncStarsArray(3);
	}

	if (v.GetStar5())
	{
		Draw_star(5.5, 38, -0.1);
	}
	else
	{
		v.IncStarsArray(4);
	}

	if (v.GetStar6())
	{
		Draw_star(21.5, 38, -0.1);
	}
	else
	{
		v.IncStarsArray(5);
	}

	if (v.GetStar7())
	{
		Draw_star(12.0, 32, -0.1);
	}
	else
	{
		v.IncStarsArray(6);
	}

	if (v.GetStar8())
	{
		Draw_star(12.0, 60, -0.1);
	}
	else
	{
		v.IncStarsArray(7);
	}

	if (v.GetStar9())
	{
		Draw_star(25.0, 80, -0.1);
	}
	else
	{
		v.IncStarsArray(8);
	}

	if (v.GetStar10())
	{
		Draw_star(-1.0, 80, -0.1);
	}
	else
	{
		v.IncStarsArray(9);
	}

}

static void resize(int width, int height)
{
	float x_difference = 0.0f;
	float y_difference = 0.0f;
	float aspect_ratio = (float)width / (float)height;
	if (aspect_ratio < 1.7778) {
		height = (width / 16.0) * 9.0;
		y_difference = ((width / aspect_ratio) - (width / 1.7778)) / 2;
	}
	else if (aspect_ratio > 1.7778) {
		width = (height / 9.0) * 16.0;
		x_difference = ((height * aspect_ratio) - (height * 1.7778)) / 2;
	}
	glViewport(x_difference, y_difference, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.01f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void display(void)
{

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	float verticalAngle = ToRad(xrot);
	float horizontalAngle = ToRad(yrot);

	float dx = cos(verticalAngle) * sin(horizontalAngle);
	float dy = sin(verticalAngle);
	float dz = cos(verticalAngle) * cos(horizontalAngle);

	float cx = xpos + dx, cy = ypos + dy, cz = zpos + dz;
	gluLookAt(xpos, ypos, zpos, cx, cy, cz, 0, 1, 0);
	glEnable(GL_TEXTURE_2D);

	Draw_models();
	//cout << difficulty << endl;
	//cout << "x:" << xpos << ", z:" << zpos << endl;
	//cout << "moving:" << moving << endl;

	//skybox
	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	GLUquadric* eg = gluNewQuadric();
	gluQuadricDrawStyle(eg, GLU_FILL);
	gluQuadricOrientation(eg, GLU_INSIDE);
	gluQuadricTexture(eg, GL_TRUE);
	gluQuadricNormals(eg, GLU_SMOOTH);

	gluSphere(eg, 500.0, 460, 180);
	gluDeleteQuadric(eg);
	glEnd();
	glPopMatrix();

	// ground
	if (((Game_over()) && (v.GetRemainingStars() != 0)) || ((v.GetMoving() == 2) && ((v.GetRemainingStars() != 0) || (zpos < 78.0f))))
	{
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if ((v.GetRemainingStars() == 0) && (zpos >= 78.0f))
	{
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-500.0f, -6.5f, 500.0f);
	glTexCoord2f(125.0f, 0.0f); glVertex3f(500.0f, -6.5f, 500.0f);
	glTexCoord2f(125.0f, 125.0f); glVertex3f(500.0f, -6.5f, -500.0f);
	glTexCoord2f(0.0f, 125.0f); glVertex3f(-500.0f, -6.5f, -500.0f);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	Draw_models();
	Draw_tree(5, -7.5, 70);
	Draw_tree(25, -7.5, 60);
	Draw_tree(-5, -7.5, 50);
	Draw_tree(5, -7.5, 40);
	Draw_tree(-8, -7.5, 30);
	Draw_tree(6, -6.5, 25);
	Draw_tree(16, -6.5, 25);

	glDisable(GL_TEXTURE_2D);

	if (((Game_over()) && (v.GetRemainingStars() != 0)) || ((v.GetMoving() == 2) && ((v.GetRemainingStars() != 0) || (zpos < 78.0f)))) {
		v.StopGoFw();
		v.StopGoBw();
		v.StopGoLeft();
		v.StopGoRight();
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		glBegin(GL_QUADS);

		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, 0.15f, -0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, 0.15f, -0.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f, -0.15f, -0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, -0.15f, -0.5f);
		glEnd();
	}
	if ((v.GetRemainingStars() == 0) && (zpos >= 78.0f))
	{
		v.StopGoFw();
		v.StopGoBw();
		v.StopGoLeft();
		v.StopGoRight();
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[3]);
		glBegin(GL_QUADS);

		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.1f, 0.15f, -0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, 0.15f, -0.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.1f, 0.05f, -0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.1f, 0.05f, -0.5f);
		glEnd();
		if (v.GetDifficulty() == 1)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[21]);
			glBegin(GL_QUADS);

			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.17f, 0.09f, -0.49f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.17f, 0.09f, -0.49f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.17f, 0.04f, -0.49f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.17f, 0.04f, -0.49f);
			glEnd();
		}
		if (v.GetDifficulty() == 2)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[22]);
			glBegin(GL_QUADS);

			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.17f, 0.09f, -0.49f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.17f, 0.09f, -0.49f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.17f, 0.04f, -0.49f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.17f, 0.04f, -0.49f);
			glEnd();
		}
		if (v.GetDifficulty() == 3)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[23]);
			glBegin(GL_QUADS);

			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.17f, 0.09f, -0.49f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.17f, 0.09f, -0.49f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.17f, 0.04f, -0.49f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.17f, 0.04f, -0.49f);
			glEnd();
		}
		if (v.GetDifficulty() == 4)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[24]);
			glBegin(GL_QUADS);

			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.17f, 0.09f, -0.49f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.17f, 0.09f, -0.49f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.17f, 0.04f, -0.49f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.17f, 0.04f, -0.49f);
			glEnd();
		}
		if (v.GetDifficulty() == 5)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[25]);
			glBegin(GL_QUADS);

			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.17f, 0.09f, -0.49f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.17f, 0.09f, -0.49f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.17f, 0.04f, -0.49f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.17f, 0.04f, -0.49f);
			glEnd();
		}
	}

	if ((v.GetRemainingStars() != 0) && (zpos >= 78.0f))
	{
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[20]);
		glBegin(GL_QUADS);

		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, 0.15f, -0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, 0.15f, -0.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f, 0.05f, -0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, 0.05f, -0.5f);
		glEnd();
	}

	if ((zpos < 78.0f))
	{
		if (v.GetRemainingStars() == 10)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[15]);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.065f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.095f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.095f, 0.174f, -0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.065f, 0.174f, -0.5f);
			glEnd();
		}
		if (v.GetRemainingStars() == 9)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[14]);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.065f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.095f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.095f, 0.174f, -0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.065f, 0.174f, -0.5f);
			glEnd();
		}
		if (v.GetRemainingStars() == 8)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[13]);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.065f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.095f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.095f, 0.174f, -0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.065f, 0.174f, -0.5f);
			glEnd();
		}
		if (v.GetRemainingStars() == 7)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[12]);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.065f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.095f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.095f, 0.174f, -0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.065f, 0.174f, -0.5f);
			glEnd();
		}
		if (v.GetRemainingStars() == 6)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[11]);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.065f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.095f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.095f, 0.174f, -0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.065f, 0.174f, -0.5f);
			glEnd();
		}
		if (v.GetRemainingStars() == 5)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[10]);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.065f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.095f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.095f, 0.174f, -0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.065f, 0.174f, -0.5f);
			glEnd();
		}
		if (v.GetRemainingStars() == 4)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[9]);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.065f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.095f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.095f, 0.174f, -0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.065f, 0.174f, -0.5f);
			glEnd();
		}
		if (v.GetRemainingStars() == 3)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[8]);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.065f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.095f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.095f, 0.174f, -0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.065f, 0.174f, -0.5f);
			glEnd();
		}
		if (v.GetRemainingStars() == 2)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[7]);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.065f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.095f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.095f, 0.174f, -0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.065f, 0.174f, -0.5f);
			glEnd();
		}
		if (v.GetRemainingStars() == 1)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[6]);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.065f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.095f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.095f, 0.174f, -0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.065f, 0.174f, -0.5f);
			glEnd();
		}
		if (v.GetRemainingStars() == 0)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[5]);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.065f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.095f, 0.215f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.095f, 0.174f, -0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.065f, 0.174f, -0.5f);
			glEnd();

			glBindTexture(GL_TEXTURE_2D, textures[16]);
			glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, 0.15f, -0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, 0.15f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f, 0.05f, -0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, 0.05f, -0.5f);
			glEnd();
		}

		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[4]);
		glBegin(GL_QUADS);

		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.08f, 0.218f, -0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.383f, 0.218f, -0.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.383f, 0.17f, -0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.08f, 0.17f, -0.5f);
		glEnd();
	}

	if (v.GetMenu())
	{
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[17]);
		glBegin(GL_QUADS);

		glColor4f(1.0f, 1.0f, 1.0f, 0.9f);

		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, 0.15f, -0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, 0.15f, -0.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f, -0.15f, -0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, -0.15f, -0.5f);
		glEnd();

		if (v.GetDifficulty() == 1)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[19]);
			glBegin(GL_QUADS);

			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, 0.06f, -0.49f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, 0.06f, -0.49f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f, 0.02f, -0.49f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, 0.02f, -0.49f);
			glEnd();
		}
		if (v.GetDifficulty() == 2)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[19]);
			glBegin(GL_QUADS);

			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, 0.025f, -0.49f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, 0.025f, -0.49f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f, -0.015f, -0.49f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, -0.015f, -0.49f);
			glEnd();
		}
		if (v.GetDifficulty() == 3)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[19]);
			glBegin(GL_QUADS);

			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, -0.008f, -0.49f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, -0.008f, -0.49f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f, -0.048f, -0.49f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, -0.048f, -0.49f);
			glEnd();
		}
		if (v.GetDifficulty() == 4)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[19]);
			glBegin(GL_QUADS);

			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, -0.04f, -0.49f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, -0.04f, -0.49f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f, -0.08f, -0.49f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, -0.08f, -0.49f);
			glEnd();
		}
		if (v.GetDifficulty() == 5)
		{
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[19]);
			glBegin(GL_QUADS);

			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, -0.074f, -0.49f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, -0.074f, -0.49f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f, -0.114f, -0.49f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, -0.114f, -0.49f);
			glEnd();
		}

	}

	if (v.GetHelp()) {
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[18]);
		glBegin(GL_QUADS);

		glColor4f(1.0f, 1.0f, 1.0f, 0.9f);

		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2f, 0.15f, -0.48f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, 0.15f, -0.48f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f, -0.15f, -0.48f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2f, -0.15f, -0.48f);
		glEnd();

	}

	glutSwapBuffers();

}

static void key_down(unsigned char key, int x, int y)
{
	if ((!Game_over()) && (v.GetMoving() != 2) && (!v.GetMenu()))
	{
		switch (key)
		{
		case 'w':
			v.StartGoFw();
			v.StartMoving();
			break;
		case 's':
			v.StartGoBw();
			v.StartMoving();
			break;
		case 'a':
			v.StartGoLeft();
			v.StartMoving();
			break;
		case 'd':
			v.StartGoRight();
			v.StartMoving();
			break;
		}
	}

	if ((!v.GetGoFw()) && (!v.GetGoBw()) && (!v.GetGoLeft()) && (!v.GetGoRight()))
	{
		switch (key)
		{
		case 'r':
			v.SetDefaultPos();
			v.SetStarsDefault();
			break;
		}
	}

	if ((xpos == 0) && (zpos == 0))
	{
		switch (key)
		{
		case 13:
			v.HideMenu();
			v.NotInDefPos();
			break;
		}
	}

	switch (key)
	{
	case '+':
		if (light_ambient[1]<0.8f) {
			for (int i = 0; i<3; i++) {
				light_ambient[i] += 0.1f;
			}
		}
		break;
	case '-':
		if (light_ambient[1]>0.0) {
			for (int i = 0; i<3; i++) {
				light_ambient[i] -= 0.1f;
			}
		}
		break;
	case 27:
		glutDestroyWindow(1);
		exit(0);
		break;
	}
}

static void key_up(unsigned char key, int x, int y)
{
		switch (key)
		{
		case 'w':
			v.StopGoFw();
			break;
		case 's':
			v.StopGoBw();
			break;
		case 'a':
			v.StopGoLeft();
			break;
		case 'd':
			v.StopGoRight();
			break;
		}
		if ((v.GetMoving() == 1) && (!v.GetGoFw()) && (!v.GetGoBw()) && (!v.GetGoLeft()) && (!v.GetGoRight()))
		{
			v.StopMoving();
		}
}

static void special_key_down(int key, int x, int y) {
	switch (key) {
	case  GLUT_KEY_F1:
		v.ChangeHelp();
		break;
	}
	if (v.GetMenu())
	{
		switch (key)
		{
		case  GLUT_KEY_DOWN:
			if (v.GetDifficulty() < 5)
			{
				v.IncDifficulty();
			}
			break;
		case  GLUT_KEY_UP:
			if (v.GetDifficulty() > 1)
			{
				v.DecDifficulty();
			}
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);

	glutCreateWindow("Game");

	glutTimerFunc(0, timer, 0);
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(key_down);
	glutKeyboardUpFunc(key_up);
	glutSpecialFunc(special_key_down);

	glClearColor(0.0, 0.0, 0.0, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);


	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	GLfloat material[] = { 0.5,0.5,0.5,1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);


	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_ambient);
	Loading_models();
	LoadTextures();
	glutMainLoop();

	return 0;
}