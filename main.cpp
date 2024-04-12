//Only detect damage at the first frame, not all frames
//Use melee; Create circle around player when it attacks

#include <raylib.h>
#include "raymath.h"
#include <vector>
#include <memory>

#include "PlayerStateMachine.cpp"
#include "EnemyStateMachine.cpp"
#include "Level.hpp"
#include "Entity.hpp"

const float FPS(60.0f);
int camType;

Camera2D camera_view = {0};

int main() {
	InitWindow(1280, 720, "Castro_Hung_Taino_Homework04");
  
  LevelData level;
  std::ifstream level_data("level_data.txt");
  LoadLevelData(level, level_data);
  ConstructGrid(level, level_data);

  // Construct tile_list
  Tile tile_list[level.NUM_TILES];
  for(int x  = 0; x < level.NUM_TILES; x++){
    tile_list[x] = LoadTile(level_data);
  }

  // Construct the Player and make Reference For Entity List
  Player p = {0, {0,0}, {0,0}, 0.0f, 0.0f}; 
  Player &player = p;


  // Make the Entity List
  std::vector<Entity*> elist = {};
  for(int x = 0; x < level.NUM_ENEMIES; x++){
    Enemy enemy = LoadEnemy(level_data);
    Enemy* e = new Enemy(enemy.hp, enemy.pos, enemy.speed);
    e->player = &player;
    elist.emplace_back(e);
  }

  // Load the Player's Values
  Player pp = LoadPlayer(level_data); 
  player = pp; 

  // Load Tilemap
  Texture tile_map = LoadTexture(level.TILE_MAP.c_str());

  // Determine Walls
  std::vector<Rectangle> walls;
  for(int x = 0; x < level.GRID_X_NUM; x++){
    for(int y = 0; y < level.GRID_Y_NUM; y++){
      if(tile_list[level.GRID[x][y]].has_collision == true){
        Rectangle wall = {float(x) * 32, float(y) * 32, 32, 32};
        walls.emplace_back(wall);
      }
    }
  }

  //Setting up camera
	camera_view.target = {player.pos.x, player.pos.y};
	camera_view.offset = {1280/2, 720/2};
	camera_view.zoom = 1.0f;
 
    Entity* tempEntity;
	while(!WindowShouldClose()) {
		float delta_time = GetFrameTime();
   
    // Logic if we Win
		if (player.hp > 0) {
			camera_view.target = {player.pos.x, player.pos.y};
		} else if (player.hp <= 0) {
			camera_view.target = {640, 360};
		}
    if (elist.size() <= 0) {
			camera_view.target = {640, 360};
      std::cout << "YAY";
		}
    
    // Here goes all the Update Logic
    player.Update(delta_time);
    for(int x = 0; x < elist.size(); x++){
      elist[x]->Update(delta_time);
      if(elist[x]->hp <= 0){
        tempEntity = elist[x];
        elist.erase(elist.begin() + x);
        elist.shrink_to_fit();
        std::cout << elist.size();
      }
    }

    // COLLISION STEP
    // Enemy Player Collisions
    for(int x = 0; x < elist.size(); x++){
      Rectangle enemy = {elist[x]->pos.x, elist[x]->pos.y, 40, 40};
      if(CheckCollisionCircleRec(player.pos, player.radius, enemy)){
        ;
      }
    }

    // Walls Collisions
    for(int x = 0; x < walls.size(); x++){
      // Make this a for loop of Enemies 
      /* // Checks if the player is colliding, checks if it's to the right, left, under, above */
      /* bool collided = !(player.pos.x > walls[x].pos.x + walls[x].size.x || */
      /*             player.pos.x + player.size.x < walls[x].x || */
      /*             player.pos.y > walls[x].position.y + walls[x].size.y || */
      /*             player.pos.y + player.size.y < walls[x].y); */
      /* Vector2 prev_pos = Vector2Subtract(pPtr->position, Vector2Scale(pPtr->velocity, TIMESTEP)); */
      /* bool above = (prev_pos.y + player.size.y <= walls[x].y); */
      /* bool left = (prev_pos.x + player.size.x <= walls[x].x); */
      /* if(collided){ */
      /*   if(above || !above){ */
      /*     player.pos.y = player.pos.y - player.direction.y * player.speed * delta_time; */
      /*     player.direction.y = 0; */
      /*   } */
      /*   if(left || !left){ */
      /*     player.pos.x = player.pos.x  - player.direction.x * player.speed * delta_time; */
      /*     player.direction.x = 0; */
      /*   } */
      /* } */
    }

    BeginDrawing();
		BeginMode2D(camera_view);
		ClearBackground(BLACK);
    for(int x = 0; x < level.GRID_X_NUM; x++){
      for(int y = 0; y < level.GRID_Y_NUM; y++){
        Rectangle source = {tile_list[level.GRID[x][y]].location};
        Rectangle dest = {float(x) * 32, float(y) * 32, 32, 32};
        DrawTexturePro(tile_map, source, dest, {0, 0}, 0.0f, WHITE);
      }
    }

    for(int x = 0; x < walls.size(); x++){
      DrawRectangleRec(walls[x], RED);
    }

    player.Draw();
    for(int x = 0; x < elist.size(); x++){
      elist[x]->Draw();
    }

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
