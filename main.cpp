//Only detect damage at the first frame, not all frames
//Use melee; Create circle around player when it attacks
// https://www.geeksforgeeks.org/dynamic-_cast-in-cpp/
//
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
 
  bool win;
	while(!WindowShouldClose()) {
		float delta_time = GetFrameTime();
   
    // Logic if we Win
		if (player.hp > 0) {
			camera_view.target = {player.pos.x, player.pos.y};
		} else if (player.hp <= 0) {
			camera_view.target = {640, 360};
      win = false;
		}
    if (elist.size() <= 0) {
			camera_view.target = {640, 360};
      win = true;
		}
    
    // Here goes all the Update Logic
    player.Update(delta_time);
    for(int x = 0; x < elist.size(); x++){
      elist[x]->Update(delta_time);
      if(elist[x]->hp <= 0){
        elist.erase(elist.begin() + x);
        elist.shrink_to_fit();
        std::cout << elist.size();
      }
    }

    // Walls Collisions
    for(int x = 0; x < walls.size(); x++){
      // Checks if the player is colliding, checks if it's to the right, left, under, above
      bool collided = CheckCollisionCircleRec(player.pos, player.radius, walls[x]); 
      Vector2 prev_pos = Vector2Subtract(player.pos, Vector2Scale(Vector2Scale(player.direction, player.speed), delta_time));
      bool above = (prev_pos.y + player.radius <= walls[x].y);
      bool left = (prev_pos.x + player.radius <= walls[x].x);
      if(collided){
        if(above || !above){
          player.pos.y = player.pos.y - player.direction.y * player.speed * delta_time;
          player.direction.y = 0;
        }
        if(left || !left){
          player.pos.x = player.pos.x  - player.direction.x * player.speed * delta_time;
          player.direction.x = 0;
        }
      }

      // ENEMY WALL COLLISIONS  
      for(int y = 0; y < elist.size(); y++){
        Enemy* enemy = dynamic_cast<Enemy*>(elist[y]);
        Rectangle e = {enemy->pos.x, enemy->pos.y, 40, 40};
        collided = CheckCollisionRecs(e, walls[x]);
        prev_pos = Vector2Subtract(elist[y]->pos, Vector2Scale(Vector2Scale({enemy->randx, enemy->randy}, enemy->speed), delta_time / 2.0f));
        above = (prev_pos.y + 40 <= walls[x].y);
        left = (prev_pos.x + 40 <= walls[x].x);
        if(collided){
          if(above || !above){
            enemy->pos.y = enemy->pos.y - enemy->randy * enemy->speed * delta_time;
          }
          if(left || !left){
            enemy->pos.x = enemy->pos.x - enemy->randx * enemy->speed * delta_time;
          }
        }
      }
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

    if(win == true){
      DrawText("You Win!", (440), (260), 100, WHITE);
    }
    else if(win == false && player.hp <= 0){
      DrawText("You Lose!", (440), (260), 100, WHITE);
    }

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
