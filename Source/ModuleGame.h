#pragma once

#include "Globals.h"
#include "Module.h"
#include "p2Point.h"
#include "Timer.h"
#include "raylib.h"
#include <vector>


class PhysBody;
class PhysicEntity;
class Map;
class LeftFlipper;
class RightFlipper;
class Kicker;
class Ball;
class OutBounds;
class Bumper1;
class Bumper1mirror;
class Bumper2;
class Bumper2mirror;
class Bumper3;
class Bumper4;
class Bumper5;
class Bumper5mirror;

struct Score
{
private:
	int score;
	int highscore;
	int previousscore;
public:
	Score() : score(0),highscore(0) {}
	void AddScore(int points)
	{
		score += points;
	}
	void ResetScore()
	{
		score = 0;
	}
	// Saves the highest score and it doesn't change until better score is achieved and returns the highest score
	void SaveScore()
	{
		if (score > highscore)
		{
			highscore = score;
		}
	}
	// Saves the previous score and returns it
	void SavePreviousScore()
	{
		previousscore = score;
	}
	// Getters
	int GetScore()
	{
		return score;
	}
	int GetHighScore()
	{
		return highscore;
	}
	int GetPreviousScore()
	{
		return previousscore;
	}
};

class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void OnBumperHit();

private:
	void Cooldown();
	TimerBumper bumperHitTimer;
	int bumperHitCount;
	const float hitTimeLimit = 3.0f; // Time limit in seconds
	const int bonusScore = 5; // Bonus score for hitting the bumper twice

public:

	std::vector<PhysicEntity*> entities;
	
	//Physic bodies
	PhysBody* sensor;
	bool sensed;

	//Textures
	Texture2D circle;
	Texture2D box;
	Texture2D rick;

	//Map texture
	Texture2D map;
	//Flipper textures
	Texture2D rightFlipper, leftFlipper, leftFlipperStraight;
	//Bumpers Textures
	Texture2D bumper1, bumper1Mirrored, bumper2, bumper2Mirrored, bumper3, bumper4, bumper5, bumper5Mirrored, empty;
	//Ball & kicker textures
	Texture2D ball, kicker;
	//Game over texture
	Texture2D gameover;

	//Custom classes objects 
	Map* physicMap;
	OutBounds* physicOutBounds_down;
	OutBounds* physicOutBounds_up;
	OutBounds* physicOutBounds_left;
	OutBounds* physicOutBounds_right;
	LeftFlipper* physicLeftFlipper;
	RightFlipper* physicRightFlipper;
	Kicker* physicKicker;
	Ball* physicBall;
	Bumper1* physicalBumper1;
	Bumper1* physicalBumper1_1;
	Bumper1mirror* physicalBumper1mirror;
	Bumper2* physicalBumper2;
	Bumper2mirror* physicalBumper2mirror;
	Bumper3* physicalBumper3;
	Bumper4* physicalBumper4;
	Bumper4* physicalBumper4_1;
	Bumper5* physicalBumper5;
	Bumper5mirror* physicalBumper5mirror;
	//Score
	Score score;

	//Game variables
	int lostlife = 3;
	bool defgame_over = false;
	int defrounds = 1;
	bool game_over = false;
	bool death = false;
	bool setTrans = false;
	int rounds;

	float cooldown_;

	uint32 bonus_fx;
	uint32 bumper_fx;
	uint32 flipper_fx;
	uint32 kicker_fx;
	uint32 death_fx;
	uint32 gameover_fx;
	uint32 map_fx;
	uint32 round_fx;

	vec2<int> ray;
	bool ray_on;


};
