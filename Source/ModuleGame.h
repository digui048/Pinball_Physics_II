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
public:
	Score() : score(0) {}
	void AddScore(int points)
	{
		score += points;
	}
	void ResetScore()
	{
		score = 0;
	}
	// Saves the highest score and it doesn't change until better score is achieved and returns the highest score
	int SaveScore()
	{
		static int highscore = 0;
		if (score > highscore)
		{
			highscore = score;
		}
		return highscore;
	}
	// Getters
	int GetScore()
	{
		return score;
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
	TimerBumper bumperHitTimer;
	int bumperHitCount;
	const float hitTimeLimit = 2.0f; // Time limit in seconds
	const int bonusScore = 100; // Bonus score for hitting the bumper twice

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
	Texture2D rightFlipper, leftFlipper;
	//Bumpers Textures
	Texture2D bumper1, bumper1Mirrored, bumper2, bumper2Mirrored, bumper3, bumper4, bumper5, bumper5Mirrored;
	//Ball & kicker textures
	Texture2D ball, kicker;


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
	void TestDoubleBumper(PhysBody* bodyA, PhysBody* bodyB);
	//Score
	Score score;

	//Game variables
	int lostlife = 3;
	bool game_over;
	bool death;
	bool bonus;
	uint32 bonus_fx;

	vec2<int> ray;
	bool ray_on;
};
