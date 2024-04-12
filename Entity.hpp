#ifndef ENTITY 
#define ENTITY

#include <raylib.h> 
#include <raymath.h>
class Entity {
  public: 
    int hp;
    Vector2 pos;
    Vector2 direction; //For direction
    float radius;
    Color color;


    // SetState is not declared here as StateMachine should be Separate from the Entity
    virtual void Update(float delta_time){};
    virtual void Draw(){};

    virtual void Damage(int damage){};
}; 
#endif
