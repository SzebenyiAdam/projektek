#pragma once
class Variables
{
public:
	Variables();

	void StartGoFw();
	void StopGoFw();
	bool GetGoFw();

	void StartGoBw();
	void StopGoBw();
	bool GetGoBw();

	void StartGoLeft();
	void StopGoLeft();
	bool GetGoLeft();

	void StartGoRight();
	void StopGoRight();
	bool GetGoRight();


	void ShowMenu();
	void HideMenu();
	bool GetMenu();

	void ChangeHelp();
	bool GetHelp();

	void IncStarsArray(int item);
	void SetStarsDefault();

	void HideStar1();
	void HideStar2();
	void HideStar3();
	void HideStar4();
	void HideStar5();
	void HideStar6();
	void HideStar7();
	void HideStar8();
	void HideStar9();
	void HideStar10();
	bool GetStar1();
	bool GetStar2();
	bool GetStar3();
	bool GetStar4();
	bool GetStar5();
	bool GetStar6();
	bool GetStar7();
	bool GetStar8();
	bool GetStar9();
	bool GetStar10();

	int GetRemainingStars();

	void StartMoving();
	void StopMoving();
	int GetMoving();

	void DifficultyHandler();
	int GetDifficulty();
	void IncDifficulty();
	void DecDifficulty();

	float GetSpeed();
	float GetWaiting();
	float GetBallRotSpeed();
	float GetBallXRot();
	float GetBallZRot();

	void IncBallXRot(float elapsed_secs);
	void DecBallXRot(float elapsed_secs);
	void IncBallZRot(float elapsed_secs);
	void DecBallZRot(float elapsed_secs);

	void SetBallXRotDefault();
	void SetBallZRotDefault();

	void RotateStars(float elapsed_secs);
	float GetRotateStars();

	void SetDefaultPos();
	void NotInDefPos();
	bool GetDefaultPos();

private:
	bool gforward = false;
	bool gbackward = false;
	bool gleft = false;
	bool gright = false;


	bool menu = true;
	bool help = false;

	int remaining_stars = 10;
	bool star1 = true;
	bool star2 = true;
	bool star3 = true;
	bool star4 = true;
	bool star5 = true;
	bool star6 = true;
	bool star7 = true;
	bool star8 = true;
	bool star9 = true;
	bool star10 = true;
	int stars[10] = { 0,0,0,0,0,0,0,0,0,0 };

	float speed = 0.0f;
	float waiting = 0.0f;
	float ball_rotation_speed = 0.0f;
	int difficulty = 1;
	int moving = 0;
	float ball_x_rotation = 0.0f;
	float ball_z_rotation = 0.0f;
	float star_rot = 0.0f;
	bool default_pos = true;
};

