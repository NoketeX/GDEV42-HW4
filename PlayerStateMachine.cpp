#include <raylib.h>
#include "raymath.h"

#include "Player.hpp"
#include <iostream>
//Where we will define everything that is in the Player header file

void Player::Update(float delta_time) { //We can define things outside of the class
	current_state->Update(*this, delta_time); //*this refers to the Player; This also updates the player's current state
}

void Player::Draw() { 
	DrawCircleV(pos, radius, color);
  DrawCircleLines(pos.x, pos.y, dmg_rng, PURPLE);

  DrawText(TextFormat("%d", hp), pos.x-500, pos.y-300, 50, WHITE);
}

//Defining the SetState function
void Player::SetState(playerState* new_state) { //We can define things outside of the class
	//Where you assign the new state
	current_state = new_state;
	current_state->Enter(*this);
}

void Player::Damage(int damage){
  if(current_state == &idle){
    hp -= damage;
  }
  if(current_state == &moving){
    hp -= damage;
  }
  if(current_state == &attack){
    hp -= damage;
  }
  if(current_state == &block){
    damage = damage / 2;
    hp -= damage;
  }
  if(current_state == &dodge){
    damage = 0;
    hp -= damage;
  } 
}

//Defining the player constructor
Player::Player(int health, Vector2 position, Vector2 direction, float rad, float spd) {
	hp = health;
  pos = position;
	direction = direction;
	radius = rad;
	speed = spd;
	SetState(&idle);
}

//Defining the enter function of the idle state
void PlayerIdle::Enter(Player& p) { //Set color, set cooldown timer, etc...; Only called once when entering the state
	p.color = BLUE;
}

//Defining the update function of the idle state
void PlayerIdle::Update(Player& p, float delta_time) { //Set color, set cooldown timer, etc...; Only called once when entering the state

	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D)) {
		p.SetState(&p.moving);
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		p.SetState(&p.attack);
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		p.SetState(&p.block);
	}

  p.invframes -= delta_time;
}

//Defining the enter function of the moving state
void PlayerMoving::Enter(Player& p) {
	p.color = YELLOW;
}

//Defining the update function of the moving state
void PlayerMoving::Update(Player& p, float delta_time) { //Set color, set cooldown timer, etc...; Only called once when entering the state

	if (p.hp > 0) {
		if (IsKeyDown(KEY_W)) {
			p.direction.y = -1;
			p.pos.y += (p.speed * p.direction.y)  * delta_time;
		}

		if (IsKeyDown(KEY_S)) {
			p.direction.y = 1;
			p.pos.y += (p.speed * p.direction.y) * delta_time;
		}

		if (IsKeyDown(KEY_A)) {
			p.direction.x = -1;
			p.pos.x += (p.speed * p.direction.x) * delta_time;
		}

		if (IsKeyDown(KEY_D)) {
			p.direction.x = 1;
			p.pos.x += (p.speed * p.direction.x) * delta_time;
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			p.SetState(&p.attack);
		}

		if (IsKeyPressed(KEY_SPACE)) {
			p.SetState(&p.dodge);
		}

		//Setting direction to 0 when their respective directional keys are not being pressed
		if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) {
			p.direction.x = 0;
		}

		if (!IsKeyDown(KEY_W) && !IsKeyDown(KEY_S)) {
			p.direction.y = 0;
		}

		if (!IsKeyDown(KEY_W) && !IsKeyDown(KEY_A) && !IsKeyDown(KEY_S) && !IsKeyDown(KEY_D)) {
			p.SetState(&p.idle);
		}
	  p.invframes -= delta_time;
	}
}

//Defining the enter function of the attack state
void PlayerAttack::Enter(Player& p) {
	p.color = RED;
  p.dmg_rng = 60;
}

//Defining the update function of the attack state
void PlayerAttack::Update(Player& p, float delta_time) {
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    p.dmg_rng = -1;
		p.SetState(&p.idle);
	}
  p.invframes -= delta_time;
}

//Defining the enter function of the block state
void PlayerBlock::Enter(Player& p) {
	p.color = PURPLE;
}

//Defining the update function of the block state
void PlayerBlock::Update(Player& p, float delta_time) {
	if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) { 
		p.SetState(&p.idle);
	}
  p.invframes -= delta_time;
}

//Defining the enter function of the dodge state
void PlayerDodge::Enter(Player& p) {
	p.color = ORANGE;
	counter = 0.170;
}

//Defining the update function of the dodge state
void PlayerDodge::Update(Player& p, float delta_time) {
	counter -= delta_time;

	//Essentially: Takes the direction where the player was headed previously, makes them move in that direction twice as fast during the duration of the counter
	p.pos.x += ((p.speed * p.direction.x) * 2) * delta_time;
	p.pos.y += ((p.speed * p.direction.y) * 2) * delta_time;

  p.invframes -= delta_time;
	if (counter <= 0) {
		p.SetState(&p.idle);
	}
}

