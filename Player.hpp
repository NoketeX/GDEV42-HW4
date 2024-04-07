#ifndef PLAYER
#define PLAYER

#include <raylib.h>
#include "raymath.h"
#include "Entity.hpp"

//You create classes and functions here, but definitions are done outside of the header

class Player;

class playerState {
public: 
	//Virtual means you can overrite it in future child classes
	virtual ~playerState() {} //Destructor, for when we remove instance
	virtual void Enter(Player& p) = 0; //For initializing certain things when you enter a state
	virtual void Update(Player& p, float delta_time) = 0; //For updating the player every frame
};

class PlayerIdle : public playerState {
public:
	void Enter(Player& p);
	void Update(Player& p, float delta_time);
};

class PlayerMoving : public playerState {
public:
	void Enter(Player& p);
	void Update(Player& p, float delta_time);
};

class PlayerAttack : public playerState {
public:
	void Enter(Player& p);
	void Update(Player& p, float delta_time);
};

class PlayerBlock : public playerState {
public:
	void Enter(Player& p);
	void Update(Player& p, float delta_time);
};

class PlayerDodge : public playerState {
public:
	float counter;
	
	void Enter(Player& p);
	void Update(Player& p, float delta_time);
};

class Player : public Entity{
public:
	Vector2 pos;
	Vector2 direction; //For direction
	float radius;
	Color color;
	int hp = 20;
  float invframes;
  float dmg_rng = -1;
	float speed;

	Player(Vector2 pos, Vector2 direction, float rad, float spd); //Constructs the player

	void Update(float delta_time); //Updates player per frame
	void Draw(); //Draws the player
	void SetState(playerState* new_state); //Accepts state, which changes the state of the player
  void TakeDamage(Entity& player, int damage);

	PlayerIdle idle;
	PlayerMoving moving;
	PlayerAttack attack;
	PlayerBlock block;
	PlayerDodge dodge;
 
private:
	playerState* current_state;
};

#endif
