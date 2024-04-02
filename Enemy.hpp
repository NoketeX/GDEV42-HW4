#ifndef ENEMY
#define ENEMY

#include "Player.hpp"
#include <raylib.h>
#include "raymath.h"

//You create classes and functions here, but definitions are done outside of the header

class Enemy;

class EnemyState {
public: 
	//Virtual means you can overrite it in future child classes
	virtual ~EnemyState() {} //Destructor, for when we remove instance
	virtual void Enter(Enemy& e) = 0; //For initializing certain things when you enter a state
	virtual void Update(Enemy& e, float delta_time) = 0; //For updating the player every frame
};

class EnemyWandering : public EnemyState {
public:
	float counter;
  float time;
  float randx, randy;

	void Enter(Enemy& e);
	void Update(Enemy& e, float delta_time);
};

class EnemyChase : public EnemyState {
public:
	void Enter(Enemy& e);
	void Update(Enemy& e, float delta_time);
};

class EnemyReady : public EnemyState {
public:
	float counter;

	void Enter(Enemy& e);
	void Update(Enemy& e, float delta_time);
};

class EnemyAttack : public EnemyState {
public:
	float counter;

	void Enter(Enemy& e);
	void Update(Enemy& e, float delta_time);
};

class Enemy {
public:
	Vector2 pos;
  float angle = 0;
	float r;
	Color c;
	int hp = 20;
	float s;
  float dmgcount;
  float dmgtimer;

	Enemy(Vector2 pos, float rad, float spd, Player* p); //Constructs the enemy
  Player* player;
	void Update(float delta_time); //Updates player per frame
	void Draw(); //Draws the enemy
	void SetState(EnemyState* new_state); //Accepts state, which changes the state of the enemy
  void TakeDamage(int damage);

	EnemyWandering wandering;
	EnemyChase chase;
	EnemyReady ready;
	EnemyAttack attack;

private:
	EnemyState* current_state;
};

#endif
