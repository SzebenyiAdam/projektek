#include "Variables.h"



Variables::Variables()
{
}

void Variables::StartGoFw()
{
	gforward = true;
}
void Variables::StopGoFw()
{
	gforward = false;
}
bool Variables::GetGoFw()
{
	return gforward;
}

void Variables::StartGoBw()
{
	gbackward = true;
}
void Variables::StopGoBw()
{
	gbackward = false;
}
bool Variables::GetGoBw()
{
	return gbackward;
}

void Variables::StartGoLeft()
{
	gleft = true;
}
void Variables::StopGoLeft()
{
	gleft = false;
}
bool Variables::GetGoLeft()
{
	return gleft;
}

void Variables::StartGoRight()
{
	gright = true;
}
void Variables::StopGoRight()
{
	gright = false;
}
bool Variables::GetGoRight()
{
	return gright;
}

void Variables::ShowMenu()
{
	menu = true;
}
void Variables::HideMenu()
{
	menu = false;
}
bool Variables::GetMenu()
{
	return menu;
}

void Variables::ChangeHelp()
{
	help = !help;
}
bool Variables::GetHelp()
{
	return help;
}

void Variables::IncStarsArray(int item)
{
	stars[item]++;
	if (stars[item] == 1)
	{
		remaining_stars--;
	}
}
void Variables::SetStarsDefault()
{
	for (int i = 0; i < 10; i++)
	{
		stars[i] = 0;
	}
	remaining_stars = 10;
	menu = true;
	star1 = true;
	star2 = true;
	star3 = true;
	star4 = true;
	star5 = true;
	star6 = true;
	star7 = true;
	star8 = true;
	star9 = true;
	star10 = true;
	moving = 0;
	ball_x_rotation = 0.0f;
	ball_z_rotation = 0.0f;
}

int Variables::GetRemainingStars()
{
	return remaining_stars;
}

void Variables::StartMoving()
{
	moving = 1;
}
void Variables::StopMoving()
{
	moving = 2;
}
int Variables::GetMoving()
{
	return moving;
}

void Variables::DifficultyHandler()
{
	if (difficulty == 1)
	{
		speed = 5.0f;
		ball_rotation_speed = 180.0f;
		waiting = 1.0f;
	}
	if (difficulty == 2)
	{
		speed = 9.5f;
		ball_rotation_speed = 350.0f;
		waiting = 0.8f;
	}
	if (difficulty == 3)
	{
		speed = 13.0f;
		ball_rotation_speed = 500.0f;
		waiting = 0.5f;
	}
	if (difficulty == 4)
	{
		speed = 17.0f;
		ball_rotation_speed = 700.0f;
		waiting = 0.3f;
	}
	if (difficulty == 5)
	{
		speed = 23.0f;
		ball_rotation_speed = 1000.0f;
		waiting = 0.25f;
	}
}

void Variables::IncDifficulty()
{
	difficulty = difficulty + 1;
}
void Variables::DecDifficulty()
{
	difficulty = difficulty - 1;
}

float Variables::GetSpeed()
{
	return speed;
}
float Variables::GetWaiting()
{
	return waiting;
}
float Variables::GetBallRotSpeed()
{
	return ball_rotation_speed;
}
float Variables::GetBallXRot()
{
	return ball_x_rotation;
}
float Variables::GetBallZRot()
{
	return ball_z_rotation;
}

void Variables::IncBallXRot(float elapsed_secs)
{
	ball_x_rotation += ball_rotation_speed * elapsed_secs;
}
void Variables::DecBallXRot(float elapsed_secs)
{
	ball_x_rotation -= ball_rotation_speed * elapsed_secs;
}
void Variables::IncBallZRot(float elapsed_secs)
{
	ball_z_rotation += ball_rotation_speed * elapsed_secs;
}
void Variables::DecBallZRot(float elapsed_secs)
{
	ball_z_rotation -= ball_rotation_speed * elapsed_secs;
}

void Variables::SetBallXRotDefault()
{
	ball_x_rotation = 0;
}
void Variables::SetBallZRotDefault()
{
	ball_z_rotation = 0;
}

int Variables::GetDifficulty()
{
	return difficulty;
}

//stars
void Variables::HideStar1()
{
	star1 = false;
}
void Variables::HideStar2()
{
	star2 = false;
}
void Variables::HideStar3()
{
	star3 = false;
}
void Variables::HideStar4()
{
	star4 = false;
}
void Variables::HideStar5()
{
	star5 = false;
}
void Variables::HideStar6()
{
	star6 = false;
}
void Variables::HideStar7()
{
	star7 = false;
}
void Variables::HideStar8()
{
	star8 = false;
}
void Variables::HideStar9()
{
	star9 = false;
}
void Variables::HideStar10()
{
	star10 = false;
}

bool Variables::GetStar1()
{
	return star1;
}
bool Variables::GetStar2()
{
	return star2;
}
bool Variables::GetStar3()
{
	return star3;
}
bool Variables::GetStar4()
{
	return star4;
}
bool Variables::GetStar5()
{
	return star5;
}
bool Variables::GetStar6()
{
	return star6;
}
bool Variables::GetStar7()
{
	return star7;
}
bool Variables::GetStar8()
{
	return star8;
}
bool Variables::GetStar9()
{
	return star9;
}
bool Variables::GetStar10()
{
	return star10;
}

void Variables::RotateStars(float elapsed_secs)
{
	star_rot += 140.0f * elapsed_secs;
}
float Variables::GetRotateStars()
{
	return star_rot;
}

void Variables::SetDefaultPos()
{
	default_pos = true;
}
void Variables::NotInDefPos()
{
	default_pos = false;
}
bool Variables::GetDefaultPos()
{
	return default_pos;
}

