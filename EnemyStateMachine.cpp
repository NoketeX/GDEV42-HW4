#include <raylib.h>
#include "raymath.h"
#include <iostream>
#include <cmath>

#include "Enemy.hpp"


void Enemy::Update(float delta_time) {
	current_state->Update(*this, delta_time);
}

void Enemy::Draw() { 
  DrawRectanglePro({pos.x + 20, pos.y + 20, 40, 40}, {20, 20}, angle * 180/M_PI, color);
	DrawCircleLines(pos.x+20, pos.y+20, attack_rng, RED); // Attack
	DrawCircleLines(pos.x+20, pos.y+20, detect_rng, BLUE); // Detection 
	DrawCircleLines(pos.x+20, pos.y+20, aggro_rng, GREEN); // Aggro
}
// https://www.youtube.com/watch?v=b6OvrRbGU68

//Defining the SetState function
void Enemy::SetState(EnemyState* new_state) { //We can define things outside of the class
	//Where you assign the new state
	current_state = new_state;
	current_state->Enter(*this);
} 

void Enemy::TakeDamage(Entity* e, int damage){
  e->hp -= damage;  
  std::cout << e->hp;
}
void Enemy::DealDamage(Entity* p, int damage){
  p->TakeDamage(p, damage);  
  std::cout << p->hp;
}

//Defining the enemy constructor

Enemy::Enemy(Vector2 position, float spd, float rad, Entity* p) {
	pos = position;
	speed = spd;
	radius = rad;
  player = p;
	SetState(&wandering);
}

//Editing the states of the Enemy
void EnemyWandering::Enter(Enemy& e) {
	e.color = PINK;
  e.angle = 0;
  counter = 0;
  randx = GetRandomValue(-20, 20);
  randy = GetRandomValue(-20, 20);
}

void EnemyWandering::Update(Enemy& e, float delta_time){
  counter += delta_time;  
  // take damage and invincible; 

  e.dmgtimer -= delta_time;
  // check for damage

}

void EnemyChase::Enter(Enemy& e) {
	e.color = YELLOW;
  e.speed = 10;
}

void EnemyChase::Update(Enemy& e, float delta_time) {
  // Angle for Player angle
  // e.angle = Vector2Angle(e.player->pos,e.pos);
  // Vector2 direction = Vector2Normalize(Vector2Subtract(e.player->pos, e.pos));
  // e.pos = Vector2Add(e.pos, Vector2Scale(direction, (e.s * 8) * delta_time));


 e.dmgtimer -= delta_time;
  // Collision betwen player and Enemy (with dmg timer check)
  //
  // Checks if player is in the radius for the enemy
}

//Enemy is preparing to attack
void EnemyReady::Enter(Enemy& e) {
	e.color = ORANGE;
	counter = 0.500f;
}

void EnemyReady::Update(Enemy& e, float delta_time) {
	counter -= delta_time;
  // e.angle = Vector2Angle(e.player->pos,e.pos); - Angle towards Player
  //
  //Checks if collide and if player is invincible


  e.dmgtimer -= delta_time; // damage timer
  // Player position Collide with enemy and damage timer; (if they can take damage)
}

void EnemyAttack::Enter(Enemy& e) {
	e.color = RED;
  e.speed = 30;
  counter = 0.170;
}

void EnemyAttack::Update(Enemy& e, float delta_time) {
  // Find the angle between player pos and enemy post, check if these are colliding, and check if player is invincible
  // If not take damage and add invincibility for 0.3 saeconds;

	counter -= delta_time;
  
  e.dmgtimer -= delta_time; // Timer for enemy doing the attack move  // Add TakeDamage
  //
  // Collision checking i player is colliding and also the enemy has attack rn

  if(counter >= 0){ // I counter is >= 0 then it immediately dashes to the player position 
  }

  else if(counter <= 0){ // If the counter or attacking reaches 0 it goes into wandering
    e.SetState(&e.wandering);
  }
}
