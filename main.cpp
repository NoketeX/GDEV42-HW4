//Only detect damage at the first frame, not all frames
//Use melee; Create circle around player when it attacks

#include <raylib.h>
#include "raymath.h"
#include <vector>

#include "PlayerStateMachine.cpp"
#include "EnemyStateMachine.cpp"
#include "Tiles.hpp"
#include "Entity.hpp"

const float FPS(60.0f);

int camType;

Camera2D camera_view = {0};

int main() {
	InitWindow(1280, 720, "Castro_Hung_Taino_Homework04");
  
	Player p(20, {400, 300}, {0, 0}, 20, 300);
  Player& player = p;
	Enemy e(5, {200, 200}, 20, 30, &p);
	Enemy e2(5, {600, 600},20, 30, &p);
  
	//Setting up camera
	camera_view.target = {p.pos.x, p.pos.y};
	camera_view.offset = {1280/2, 720/2};
	camera_view.zoom = 1.0f;
  
  LevelData level;
  std::ifstream level_data("level_data.txt");
  LoadLevelData(level, level_data);
  ConstructGrid(level, level_data);

  // Make it so this is declared inside the TXT file;
  Tile tile_list[level.NUM_TILES];
  for(int x  = 0; x < level.NUM_TILES; x++){
    tile_list[x] = LoadTile(level_data);
    std::cout << tile_list[x].has_collision;
  }

  Texture tile_map = LoadTexture(level.TILE_MAP.c_str());

  std::vector<Entity*> elist = {};
  elist.emplace_back(&e);
  elist.emplace_back(&e2);

  std::vector<Rectangle> walls;
  for(int x = 0; x < level.GRID_X_NUM; x++){
    for(int y = 0; y < level.GRID_Y_NUM; y++){

      if(tile_list[level.GRID[x][y]].has_collision == true){
        Rectangle wall = {float(x) * 32, float(y) * 32, 32, 32};
        walls.emplace_back(wall);
      }
    }
  }

	while(!WindowShouldClose()) {
		float delta_time = GetFrameTime();
    
		if (p.hp > 0) {
			camera_view.target = {p.pos.x, p.pos.y};
		} else if (p.hp <= 0) {
			camera_view.target = {640, 360};
		} else if ((e.hp <= 0) && (e2.hp <= 0)) {
			camera_view.target = {640, 360};
		}
    
    // Here goes all the Update Logic
    player.Update(delta_time);
    for(int x = 0; x < elist.size(); x++){
      elist[x]->Update(delta_time);
    }

    // COLLISION STEP
    // Enemy Player Collisions
    for(int x = 0; x < elist.size(); x++){
      Rectangle enemy = {elist[x]->pos.x, elist[x]->pos.y, 40, 40};
      if(CheckCollisionCircleRec(player.pos, player.radius, enemy)){
      }
    }

    // Walls Collisions
    for(int x = 0; x < walls.size(); x++){
      if(CheckCollisionCircleRec(player.pos, player.radius, walls[x])){
        continue;
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

    p.Draw();
    for(int x = 0; x < elist.size(); x++){
      elist[x]->Draw();
    }

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
