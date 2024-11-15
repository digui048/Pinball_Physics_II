#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include <iostream>

enum class ColliderType {
	FLIPPER,
	BUMPER,
	BALL,
	KICKER,
	MAP,
	OUTBOUNDS
};
class PhysicEntity
{
protected:

	PhysicEntity(PhysBody* _body, Module* _listener, ColliderType ctype)
		: body(_body)
		, listener(_listener)
		, ctype(ctype)
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
	void ConvertToStatic() { body->body->SetType(b2_staticBody); }
	void RotateStatic(float angle_) { 

		b2Vec2 position = body->body->GetPosition(); // Obtain the position of the body
		float angle = body->body->GetAngle(); // Obtain the angle of the body

		// Rotation in radians
		float newAngle = angle + DEG2RAD * angle_;

		// Set the position and angle of the body
		body->body->SetTransform(position, newAngle);
	
	}
	ColliderType GetType() const { return ctype; }
	
protected:
	PhysBody* body;
	Module* listener;
	ColliderType ctype;
};
class Score
{
private:
	int score = 0;
public:
	void AddScore(int points)
	{
		score += points;
	}
	void ResetScore()
	{
		score = 0;
	}
	int GetScore()
	{
		return score;
	}
};
class Circle : public PhysicEntity
{
public:
	Circle(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateCircle(_x, _y, 14), _listener,ColliderType::BALL)
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
		: PhysicEntity(physics->CreateRectangle(_x, _y, 100, 50), _listener,ColliderType::BALL)
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
		: PhysicEntity(physics->CreateChain(_x, _y, rick_head, 150), _listener,ColliderType::MAP)
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
class OutBounds : public PhysicEntity
{
public:
	OutBounds(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, SCREEN_HEIGHT, 2), _listener, ColliderType::OUTBOUNDS)
	{
		ConvertToStatic();
	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}
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
		: PhysicEntity(physics->CreateRectangle(_x + 50, _y + 20, 80, 20), _listener, ColliderType::FLIPPER), texture(_texture)
	{
		InitializeJoint(map->GetBody()->body);
		
		
	}


	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x - 50, (float)y - 12 }, (body->GetRotation() - 0.25) * RAD2DEG, 2.0f, WHITE);
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
		localAnchor.Set(-0.8,0);
		b2Vec2 worldAnchor = lFlipperBody->GetWorldPoint(localAnchor);
		lFlipperJointDef.Initialize(lFlipperBody, mapBody, worldAnchor);


		lFlipperJointDef.lowerAngle = -0.15 * b2_pi;
		lFlipperJointDef.upperAngle = 0 * b2_pi;
		lFlipperJointDef.enableLimit = true;
		lFlipperJointDef.maxMotorTorque = 1000.0f;
		lFlipperJointDef.motorSpeed = 0.0f;
		lFlipperJointDef.enableMotor = true;

		lFlipperJoint = (b2RevoluteJoint*)myWorld->CreateJoint(&lFlipperJointDef);

	}

	void Push() 
	{
		dt = GetFrameTime() * GetFPS();

		if (timer > 0)
		{
			timer -= dt;
			lFlipperJoint->SetMotorSpeed(250.0f);
		}
		else
		{
			lFlipperJoint->SetMotorSpeed(-5.0f);
		}
		if (IsKeyPressed(KEY_LEFT) && cooldown <= 0)
		{
			timer = timerLenght;
			cooldown = 40;
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

	float cooldown = 40;
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
		: PhysicEntity(physics->CreateRectangle(_x, _y, 80, 20), _listener, ColliderType::FLIPPER), texture(_texture)
	{
		InitializeJoint(map->GetBody()->body);


	}


	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x - 30, (float)y - 30}, (body->GetRotation() + 0.25) * RAD2DEG, 2.0f, WHITE);
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
		localAnchor.Set(0.8, 0);
		b2Vec2 worldAnchor = rFlipperBody->GetWorldPoint(localAnchor);
		rFlipperJointDef.Initialize(rFlipperBody, mapBody, worldAnchor);


		rFlipperJointDef.lowerAngle = 0 * b2_pi;
		rFlipperJointDef.upperAngle = 0.15 * b2_pi;
		rFlipperJointDef.enableLimit = true;
		rFlipperJointDef.maxMotorTorque = 5000.0f;
		rFlipperJointDef.motorSpeed = 0.0f;
		rFlipperJointDef.enableMotor = true;

		rFlipperJoint = (b2RevoluteJoint*)myWorld->CreateJoint(&rFlipperJointDef);

	}

	void Push()
	{
		dt = GetFrameTime() * GetFPS();

		if (timer > 0)
		{
			timer -= dt;
			rFlipperJoint->SetMotorSpeed(-250.0f);
		}
		else
		{
			rFlipperJoint->SetMotorSpeed(5);
		}
		if (IsKeyPressed(KEY_RIGHT) && cooldown <= 0)
		{
			timer = timerLenght;
			cooldown = 40;
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

	float cooldown = 40;
};
class Kicker : public PhysicEntity
{
public:
	Kicker(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 28, 14), _listener, ColliderType::KICKER)
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
		: PhysicEntity(physics->CreateCircle(_x, _y, 14), _listener, ColliderType::BALL)
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
			Vector2{ (float)texture.width, (float)texture.height }, body->GetRotation() * RAD2DEG, WHITE);
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

	static constexpr int bumper1[20] = {
			56, 92,
			12, 116,
			0, 104,
			0, 82,
			32, 18,
			38, 8,
			42, 0,
			52, 0,
			56, 6,
			56, 18,
	};

	PhysBody* GetBody() const { return body; }

	Bumper1(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, Map* map)
		: PhysicEntity(physics->CreateChain(_x, _y, bumper1, 20), _listener, ColliderType::BUMPER), texture(_texture)
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
class Bumper1mirror : public PhysicEntity
{
public:

	static constexpr int bumper1chain[18] = {
			0, 94,
			44, 116,
			56, 102,
			56, 82,
			18, 8,
			14, 0,
			4, 0,
			0, 4,
			0, 22,
	};

	PhysBody* GetBody() const { return body; }

	Bumper1mirror(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, Map* map)
		: PhysicEntity(physics->CreateChain(_x, _y, bumper1chain, 18), _listener, ColliderType::BUMPER), texture(_texture)
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
class Bumper2 : public PhysicEntity
{
public:

	static constexpr int bumper1[20] = {
			26, 38,
			26, 52,
			24, 58,
			18, 58,
			12, 54,
			0, 18,
			0, 4,
			6, 0,
			12, 2,
			16, 10,
	};

	PhysBody* GetBody() const { return body; }

	Bumper2(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, Map* map)
		: PhysicEntity(physics->CreateChain(_x, _y, bumper1, 20), _listener, ColliderType::BUMPER), texture(_texture)
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
class Bumper2mirror : public PhysicEntity
{
public:

	static constexpr int bumper1[24] = {
			0, 38,
			0, 52,
			4, 58,
			8, 58,
			14, 54,
			20, 36,
			26, 18,
			26, 2,
			22, 0,
			18, 0,
			12, 2,
			8, 16,
	};

	PhysBody* GetBody() const { return body; }

	Bumper2mirror(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, Map* map)
		: PhysicEntity(physics->CreateChain(_x, _y, bumper1, 24), _listener, ColliderType::BUMPER), texture(_texture)
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
class Bumper3 : public PhysicEntity
{
public:

	static constexpr int bumper1[26] = {
			76, 116,
			74, 124, 
			68, 128,
			60, 128,
			54, 124,
			52, 116,
			52, 10,
			54, 2,
			60, 0,
			68, 0,
			74, 2,
			76, 10,
			76, 46,
	};

	PhysBody* GetBody() const { return body; }

	Bumper3(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, Map* map)
		: PhysicEntity(physics->CreateChain(_x, _y, bumper1, 26), _listener, ColliderType::BUMPER), texture(_texture)
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
class Bumper4 : public PhysicEntity
{
public:


	PhysBody* GetBody() const { return body; }

	Bumper4(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, Map* map)
		: PhysicEntity(physics->CreateCircleKinematic(_x, _y, 36), _listener, ColliderType::BUMPER), texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x - 64, (float)y - 64 }, body->GetRotation() * RAD2DEG, 2.0f, WHITE);
	}

private:
	Texture2D texture;

};
class Bumper5 : public PhysicEntity
{
public:

	static constexpr int bumper1[14] = {
			0, 58,
			0, 68,
			10, 78,
			18, 78,
			70, 26,
			70, 10,
			58, 0,
	};

	PhysBody* GetBody() const { return body; }

	Bumper5(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, Map* map)
		: PhysicEntity(physics->CreateChain(_x, _y, bumper1, 14), _listener, ColliderType::BUMPER), texture(_texture)
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
class Bumper5mirror : public PhysicEntity
{
public:

	static constexpr int bumper1[14] = {
			10, 0,
			0, 10,
			0, 26,
			52, 78,
			58, 78,
			70, 68,
			70, 58,
	};

	PhysBody* GetBody() const { return body; }

	Bumper5mirror(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, Map* map)
		: PhysicEntity(physics->CreateChain(_x, _y, bumper1, 14), _listener, ColliderType::BUMPER), texture(_texture)
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
	
	//Bumpers
	bumper1 = LoadTexture("Assets/exportedSprites/Bumper1.png");
	bumper1Mirrored = LoadTexture("Assets/exportedSprites/Bumper1mirror.png");
	bumper2 = LoadTexture("Assets/exportedSprites/Bumper2.png");
	bumper2Mirrored = LoadTexture("Assets/exportedSprites/Bumper2mirrored.png");
	bumper3 = LoadTexture("Assets/exportedSprites/Bumper3.png");
	bumper4 = LoadTexture("Assets/exportedSprites/Bumper4.png");
	bumper5 = LoadTexture("Assets/exportedSprites/Bumper5.png");
	bumper5Mirrored = LoadTexture("Assets/exportedSprites/Bumper5mirrored.png");

	//Load Game Sounds
	bonus_fx = App->audio->LoadFx("Assets/bonus.wav");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH /2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);


	//Draw and Create OBJ Map
	physicMap = new Map(App->physics, 0, 0, this, map);
	entities.emplace_back(physicMap);

	//Draw and Create OBJ OutBounds
	physicOutBounds_down = new OutBounds(App->physics, SCREEN_WIDTH / 2, SCREEN_HEIGHT -24, this, map);
	entities.emplace_back(physicOutBounds_down);

	physicOutBounds_up = new OutBounds(App->physics, SCREEN_WIDTH / 2, 0, this, map);
	entities.emplace_back(physicOutBounds_up);

	physicOutBounds_left = new OutBounds(App->physics, 0, SCREEN_HEIGHT / 2, this, map);
	physicOutBounds_left->RotateStatic(90);
	entities.emplace_back(physicOutBounds_left);

	physicOutBounds_right = new OutBounds(App->physics, SCREEN_WIDTH, SCREEN_HEIGHT / 2, this, map);
	physicOutBounds_right->RotateStatic(90);
	entities.emplace_back(physicOutBounds_right);

	//Draw and Create OBJ LeftFlipper
	physicLeftFlipper = new LeftFlipper(App->physics,SCREEN_WIDTH / 4.5f - 2, SCREEN_HEIGHT - SCREEN_HEIGHT / 6, this, leftFlipper, physicMap);
	entities.emplace_back(physicLeftFlipper);

	//Draw and Create OBJ RightFlipper
	physicRightFlipper = new RightFlipper(App->physics, SCREEN_WIDTH / 2 + 32 , SCREEN_HEIGHT - SCREEN_HEIGHT / 6 +22, this, rightFlipper, physicMap);
	entities.emplace_back(physicRightFlipper);

	//Draw and Create OBJ Kicker
	physicKicker = new Kicker(App->physics, SCREEN_WIDTH - 48, SCREEN_HEIGHT - 40, this, kicker);
	entities.emplace_back(physicKicker);

	//Draw and Create OBJ Ball
	physicBall = new Ball(App->physics, SCREEN_WIDTH - 48, SCREEN_HEIGHT - SCREEN_HEIGHT / 6, this, ball);
	entities.emplace_back(physicBall);

	//Bumpers
	physicalBumper1 = new Bumper1(App->physics, SCREEN_WIDTH - SCREEN_WIDTH / 3 - 20, SCREEN_HEIGHT - SCREEN_HEIGHT / 3 - 20, this, bumper1, physicMap);
	entities.emplace_back(physicalBumper1);

	physicalBumper1mirror = new Bumper1mirror(App->physics, SCREEN_WIDTH / 3 - 20 - 56, SCREEN_HEIGHT - SCREEN_HEIGHT / 3 - 20, this, bumper1Mirrored, physicMap);
	entities.emplace_back(physicalBumper1mirror);

	physicalBumper2 = new Bumper2(App->physics, SCREEN_WIDTH / 4 - 20, SCREEN_HEIGHT / 3 - 20, this, bumper2, physicMap);
	entities.emplace_back(physicalBumper2);

	physicalBumper3 = new Bumper3(App->physics, SCREEN_WIDTH - SCREEN_WIDTH/2 + 30, SCREEN_HEIGHT / 3, this, bumper3, physicMap);
	entities.emplace_back(physicalBumper3);

	physicalBumper4 = new Bumper4(App->physics, SCREEN_WIDTH - SCREEN_WIDTH / 2 + 30, SCREEN_HEIGHT / 3 + 80, this, bumper4, physicMap);
	entities.emplace_back(physicalBumper4);

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

	// Game entities
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

	// Game loop ------------------------------------------------------

	if (game_over == true)
	{
		DrawText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 40, RED);
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

void ModuleGame::OnCollision(PhysicEntity* bodyA, PhysicEntity* bodyB)
{

	App->audio->PlayFx(bonus_fx);
	printf("AAA");

	switch (bodyB->GetType())
	{
	case ColliderType::KICKER:
		LOG("Collision KICKER");
		break;
	case ColliderType::FLIPPER:
		LOG("Collision FLIPPER");
		break;
	case ColliderType::BUMPER:
		LOG("Collision BUMPER");
		break;
	case ColliderType::OUTBOUNDS:
		printf("Collision OUTBOUNDS");
		if (lostlife <= 0)
		{
			game_over = true;
		}
		else
		{
			lostlife--;
		}
		break;
	case ColliderType::MAP:
		LOG("Collision MAP");
		break;
	default:
		break;
	}
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
