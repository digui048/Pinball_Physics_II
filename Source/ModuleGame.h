#pragma once

#include "Globals.h"
#include "Module.h"
#include "p2Point.h"

#include "raylib.h"
#include <vector>


class PhysBody;
class PhysicEntity;
class Map;
class LeftFlipper;
class RightFlipper;
class Kicker;
class Ball;
class Score;
class OutBounds;

class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysicEntity* bodyA, PhysicEntity* bodyB);

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
	Texture2D Bumper1, Bumper1Mirrored, Bumper2, Bumper2Mirrored, Bumper3, Bumper4, Bumper5, Bumper5Mirrored;
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

	//Score
	Score* score;

	//Game variables
	int lostlife = 3;
	bool game_over;

	uint32 bonus_fx;

	vec2<int> ray;
	bool ray_on;
};
