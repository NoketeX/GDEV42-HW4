#ifndef ENTITY 
#define ENTITY

#include <raylib.h> 
#include <raymath.h>


class Entity {
  public: 
    int hp;

    // SetState is not declared here as StateMachine should be Separate from the Entity
    virtual void Update(float delta_time){};
    virtual void Draw(){};
}; 

#endif
