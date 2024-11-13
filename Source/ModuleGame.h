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
class Kicker;
class Ball;

class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

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
	LeftFlipper* physicLeftFlipper;
	Kicker* physicKicker;
	Ball* physicBall;

	uint32 bonus_fx;

	vec2<int> ray;
	bool ray_on;
};
