#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include <iostream>
class PhysicEntity
{
protected:

	PhysicEntity(PhysBody* _body, Module* _listener)
		: body(_body)
		, listener(_listener)
	{

	}

public:
	virtual ~PhysicEntity() = default;
	virtual void Update() = 0;

	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	}
	PhysBody* GetBody() const { return body; }
	
protected:
	PhysBody* body;
	Module* listener;
};
class Circle : public PhysicEntity
{
public:
	Circle(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateCircle(_x, _y, 14), _listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		Vector2 position{ (float)x, (float)y };
		float scale = 2.0f;
		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
		Vector2 origin = { (float)texture.width / 2.0f + 8, (float)texture.height / 2.0f + 8};
		float rotation = body->GetRotation() * RAD2DEG;
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

private:
	Texture2D texture;

};
class Box : public PhysicEntity
{
public:
	Box(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 100, 50), _listener)
		, texture(_texture)
	{
		
	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x, (float)y, (float)texture.width, (float)texture.height },
			Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f}, body->GetRotation() * RAD2DEG, WHITE);

	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}

private:
	Texture2D texture;

};
class Map : public PhysicEntity
{
public:
	// Pivot 0, 0
	static constexpr int rick_head[150] = {
			92, 736,
			308, 736,
			308, 640,
			308, 684,
			280, 650,
			364, 610,
			364, 464,
			334, 448,
			334, 438,
			364, 380,
			364, 246,
			362, 246,
			348, 232,
			348, 218,
			354, 194,
			360, 188,
			376, 188,
			376, 192,
			382, 206,
			384, 224,
			384, 736,
			414, 736,

			414, 212,
			410, 202,
			408, 188,
			406, 178,
			404, 170,
			400, 160,
			396, 150,
			390, 136,
			384, 128,
			370, 108,
			346, 84,
			340, 80,
			330, 72,
			320, 66,
			304, 58,
			294, 54,
			276, 48,
			268, 46,
			258, 44,

			242, 40,
			208, 40,
			192, 44,
			182, 46,
			174, 48,
			156, 54,
			144, 58,
			130, 66,
			120, 72,
			110, 80,
			104, 84,
			80, 108,
			66, 128,
			60, 136,
			54, 150,
			50, 160,
			46, 170,
			44, 178,
			42, 188,
			40, 202,
			38, 212,

			38, 252,
			40, 262,
			42, 270,
			48, 288,
			52, 298,

			96, 384,
			96, 394,
			38, 452,
			36, 452,
			36, 610,
			92, 638,
			116, 650,
			92, 684,

	};

	Map(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateChain(_x, _y, rick_head, 150), _listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x, (float)y }, body->GetRotation() * RAD2DEG, 2.0f, WHITE);
	}

private:
	Texture2D texture;
};
class LeftFlipper : public PhysicEntity
{
public:
	// Pivot 0, 0
	static constexpr int points[24] = {
			0,10,
			4,4,
			10,0,
			16, 0,
			70, 26,
			78, 32,
			78, 36,
			72, 40,
			66, 40,
			10, 26,
			2, 20,
			0, 16,
			
	};

	PhysBody* GetBody() const { return body; }
	b2RevoluteJointDef GetJoint() const { return lFlipperJointDef; }
	


	LeftFlipper(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, Map* map)
		: PhysicEntity(physics->CreateRectangle(_x + 50, _y, 100, 30), _listener), texture(_texture)
	{
		InitializeJoint(map->GetBody()->body);
		
		
	}


	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x - 50, (float)y }, body->GetRotation() * RAD2DEG, 2.0f, WHITE);
		Cooldown();
		Push();
	}

private:
	Texture2D texture;
	b2RevoluteJointDef lFlipperJointDef;
	b2RevoluteJoint* lFlipperJoint;
	b2World* myWorld = GetBody()->body->GetWorld();;
	void InitializeJoint(b2Body* mapBody)
	{
		b2Body* lFlipperBody = GetBody()->body;
		lFlipperBody->SetAngularVelocity(0.0f);
		b2Vec2 localAnchor;
		localAnchor.Set(0,0);
		b2Vec2 worldAnchor = lFlipperBody->GetWorldPoint(localAnchor);
		lFlipperJointDef.Initialize(lFlipperBody, mapBody, worldAnchor);


		lFlipperJointDef.lowerAngle = 0 * b2_pi;
		lFlipperJointDef.upperAngle = 0.15f * b2_pi;
		lFlipperJointDef.enableLimit = true;
		lFlipperJointDef.maxMotorTorque = 50.0f;
		lFlipperJointDef.motorSpeed = 0.0f;
		lFlipperJointDef.enableMotor = true;

		lFlipperJoint = (b2RevoluteJoint*)myWorld->CreateJoint(&lFlipperJointDef);

		std::cout << "x anchor: " << lFlipperJoint->GetLocalAnchorA().x << std::endl;
		std::cout << "y anchor: " << lFlipperJoint->GetLocalAnchorA().y << std::endl;
	}

	void Push() 
	{
		dt = GetFrameTime() * GetFPS();

		if (timer > 0)
		{
			timer -= dt;
			lFlipperJoint->SetMotorSpeed(10.0f);
		}
		else
		{
			lFlipperJoint->SetMotorSpeed(-5.0f);
		}
		if (IsKeyPressed(KEY_LEFT) && cooldown <= 0)
		{
			timer = timerLenght;
			cooldown = 20;
		}
	}

	void Cooldown() 
	{
		dt = GetFrameTime() * GetFPS();
		if (cooldown > 0) 
		{
			cooldown -= dt;
		}
	}

	float timer = 0;
	float timerLenght = 10;
	float dt;

	float cooldown = 20;
};
class RightFlipper : public PhysicEntity
{
public:
	// Pivot 0, 0
	static constexpr int points[24] = {
			0,10,
			4,4,
			10,0,
			16, 0,
			70, 26,
			78, 32,
			78, 36,
			72, 40,
			66, 40,
			10, 26,
			2, 20,
			0, 16,

	};

	PhysBody* GetBody() const { return body; }
	b2RevoluteJointDef GetJoint() const { return rFlipperJointDef; }



	RightFlipper(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, Map* map)
		: PhysicEntity(physics->CreateRectangle(_x + 32, _y, 100, 30), _listener), texture(_texture)
	{
		InitializeJoint(map->GetBody()->body);


	}


	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x - 32, (float)y }, body->GetRotation() * RAD2DEG, 2.0f, WHITE);
		Cooldown();
		Push();
	}

private:
	Texture2D texture;
	b2RevoluteJointDef rFlipperJointDef;
	b2RevoluteJoint* rFlipperJoint;
	b2World* myWorld = GetBody()->body->GetWorld();;
	void InitializeJoint(b2Body* mapBody)
	{
		b2Body* rFlipperBody = GetBody()->body;
		rFlipperBody->SetAngularVelocity(0.0f);
		b2Vec2 localAnchor;
		localAnchor.Set(0, 0);
		b2Vec2 worldAnchor = rFlipperBody->GetWorldPoint(localAnchor);
		rFlipperJointDef.Initialize(rFlipperBody, mapBody, worldAnchor);


		rFlipperJointDef.lowerAngle = -0.15f * b2_pi;
		rFlipperJointDef.upperAngle = 0 * b2_pi;
		rFlipperJointDef.enableLimit = true;
		rFlipperJointDef.maxMotorTorque = 50.0f;
		rFlipperJointDef.motorSpeed = 0.0f;
		rFlipperJointDef.enableMotor = true;

		rFlipperJoint = (b2RevoluteJoint*)myWorld->CreateJoint(&rFlipperJointDef);

		std::cout << "x anchor: " << rFlipperJoint->GetLocalAnchorA().x << std::endl;
		std::cout << "y anchor: " << rFlipperJoint->GetLocalAnchorA().y << std::endl;
	}

	void Push()
	{
		dt = GetFrameTime() * GetFPS();

		if (timer > 0)
		{
			timer -= dt;
			//Yes, sure its -
			rFlipperJoint->SetMotorSpeed(-10.0f);
		}
		else
		{
			rFlipperJoint->SetMotorSpeed(5);
		}
		if (IsKeyPressed(KEY_RIGHT) && cooldown <= 0)
		{
			timer = timerLenght;
			cooldown = 20;
		}
	}

	void Cooldown()
	{
		dt = GetFrameTime() * GetFPS();
		if (cooldown > 0)
		{
			cooldown -= dt;
		}
	}

	float timer = 0;
	float timerLenght = 10;
	float dt;

	float cooldown = 20;
};
class Kicker : public PhysicEntity
{
public:
	Kicker(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 28, 14), _listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		b2body = body->body;
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x, (float)y }, body->GetRotation() * RAD2DEG, 2.0f, WHITE);
		Cooldown();
		Push();
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}

private:
	Texture2D texture;
	b2Body* b2body;
	void Push()
	{
		dt = GetFrameTime() * GetFPS();

		if (timer > 0)
		{
			timer -= dt;
			b2Vec2 velocity;
			velocity.Set(0, -20);
			b2body->SetLinearVelocity(velocity);

		}
		else
		{
			b2Vec2 velocity;
			velocity.Set(0, 10);
			b2body->SetLinearVelocity(velocity);
		}
		if (IsKeyPressed(KEY_DOWN) && cooldown <= 0)
		{
			timer = timerLenght;
			cooldown = 40;
		}
	}
	float timer = 0;
	float timerLenght = 10;
	float dt;

	float cooldown = 40;

	void Cooldown()
	{
		dt = GetFrameTime() * GetFPS();
		if (cooldown > 0)
		{
			cooldown -= dt;
		}
	}
};
class Ball : public PhysicEntity
{
public:
	Ball(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateCircle(_x, _y, 14), _listener)
		, texture(_texture)
	{
		ChangeGravity(2);
	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x, (float)y, (float)texture.width * 2.0f, (float)texture.height * 2.0f },
			Vector2{ (float)texture.width / 1.0f, (float)texture.height / 1.0f }, body->GetRotation() * RAD2DEG, WHITE);
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}

private:
	Texture2D texture;
	void ChangeGravity(float gravity) 
	{
		body->body->SetGravityScale(gravity);
	}
};

class Bumper1 : public PhysicEntity
{
public:

	static constexpr int bumper1[24] = {
			0,10,
			4,4,
			10,0,
			16, 0,
			70, 26,
			78, 32,
			78, 36,
			72, 40,
			66, 40,
			10, 26,
			2, 20,
			0, 16,

	};

	PhysBody* GetBody() const { return body; }

	Bumper1(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, Map* map)
		: PhysicEntity(physics->CreateChain(_x, _y, bumper1, 24), _listener), texture(_texture)
	{
		
	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x - 32, (float)y }, body->GetRotation() * RAD2DEG, 2.0f, WHITE);
	}

private:
	Texture2D texture;

};

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	sensed = false;
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Load default Game textures
	circle = LoadTexture("Assets/wheel.png"); 
	box = LoadTexture("Assets/crate.png");
	rick = LoadTexture("Assets/rick_head.png");

	//Load Game Textures
	map = LoadTexture("Assets/exportedSprites/Base.png");
	leftFlipper = LoadTexture("Assets/exportedSprites/FlipperLeft.png");
	rightFlipper = LoadTexture("Assets/exportedSprites/FlipperRight.png");
	ball = LoadTexture("Assets/exportedSprites/Ball.png");
	
	//Load Game Sounds
	bonus_fx = App->audio->LoadFx("Assets/bonus.wav");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH /2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);


	//Draw and Create OBJ Map
	physicMap = new Map(App->physics, 0, 0, this, map);
	entities.emplace_back(physicMap);

	//Draw and Create OBJ LeftFlipper
	physicLeftFlipper = new LeftFlipper(App->physics,SCREEN_WIDTH / 4.5f - 2, SCREEN_HEIGHT - SCREEN_HEIGHT / 6, this, leftFlipper, physicMap);
	entities.emplace_back(physicLeftFlipper);

	physicRightFlipper = new RightFlipper(App->physics, SCREEN_WIDTH / 2 , SCREEN_HEIGHT - SCREEN_HEIGHT / 6, this, rightFlipper, physicMap);
	entities.emplace_back(physicRightFlipper);

	//Draw and Create OBJ Kicker
	physicKicker = new Kicker(App->physics, SCREEN_WIDTH - 48, SCREEN_HEIGHT - 40, this, kicker);
	entities.emplace_back(physicKicker);

	//Draw and Create OBJ Ball
	physicBall = new Ball(App->physics, SCREEN_WIDTH - 48, SCREEN_HEIGHT - SCREEN_HEIGHT / 6, this, ball);
	entities.emplace_back(physicBall);

	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	
	if(IsKeyPressed(KEY_SPACE))
	{
		ray_on = !ray_on;
		ray.x = GetMouseX();
		ray.y = GetMouseY();
	}

	if(IsKeyPressed(KEY_ONE))
	{
		entities.emplace_back(new Ball(App->physics, GetMouseX(), GetMouseY(), this, ball));
		
	}

	if(IsKeyPressed(KEY_TWO))
	{
		entities.emplace_back(new Box(App->physics, GetMouseX(), GetMouseY(), this, box));
	}


	// Prepare for raycast ------------------------------------------------------
	
	vec2i mouse;
	mouse.x = GetMouseX();
	mouse.y = GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	vec2f normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------


	for (PhysicEntity* entity : entities)
	{
		entity->Update();
		if (ray_on)
		{
			int hit = entity->RayHit(ray, mouse, normal);
			if (hit >= 0)
			{
				ray_hit = hit;
			}
		}
	}
	

	// ray -----------------
	if(ray_on == true)
	{
		vec2f destination((float)(mouse.x-ray.x), (float)(mouse.y-ray.y));
		destination.Normalize();
		destination *= (float)ray_hit;

		DrawLine(ray.x, ray.y, (int)(ray.x + destination.x), (int)(ray.y + destination.y), RED);

		if (normal.x != 0.0f)
		{
			DrawLine((int)(ray.x + destination.x), (int)(ray.y + destination.y), (int)(ray.x + destination.x + normal.x * 25.0f), (int)(ray.y + destination.y + normal.y * 25.0f), Color{ 100, 255, 100, 255 });
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

	App->audio->PlayFx(bonus_fx);

	/*
	int x, y;
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
