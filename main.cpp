//Only detect damage at the first frame, not all frames
//Use melee; Create circle around player when it attacks

#include <raylib.h>
#include "raymath.h"
#include <vector>

#include "PlayerStateMachine.cpp"
#include "EnemyStateMachine.cpp"
#include "Tiles.hpp"

const float FPS(60.0f);

int camType;

Camera2D camera_view = {0};

int main() {
	InitWindow(1280, 720, "Castro_Hung_Taino_Homework04");
  
	std::vector<Enemy> enemies;
	Player p({400, 300}, {0, 0}, 20, 300);
	Enemy e({200, 200}, 20, 30, &p);
	Enemy e2({600, 600},20, 30, &p);
	//Enemy::Enemy(Vector2 position, Vector2 direction, float rad, float spd, Player p)
  enemies.emplace_back(e);
  enemies.emplace_back(e2);
  
	//Setting up camera
	camera_view.target = {p.pos.x, p.pos.y};
	camera_view.offset = {1280/2, 720/2};
	camera_view.zoom = 1.0f;
  
  LevelData level;
  std::ifstream level_data("level_data.txt");
  LoadLevelData(level, level_data);
  ConstructGrid(level, level_data);
  // Make it so this is declared inside the TXT file;
  Tile tile_list[4];
  tile_list[0] = {{0, 0, 32, 32}, false};
  tile_list[1] = {{64, 0, 32, 32}, false};
  tile_list[2] = {{128, 0, 32, 32}, false};
  tile_list[3] = {{64, 32, 32, 32}, false};

  Texture tile_map = LoadTexture(level.TILE_MAP.c_str());


	while(!WindowShouldClose()) {
		float delta_time = GetFrameTime();
		for(int x = 1; x < level.GRID_X_NUM; x++){
      for(int y = 1; y < level.GRID_Y_NUM; y++){
        Rectangle source = {tile_list[level.GRID[x][y]].location};
        Rectangle dest = {float(x) * 32, float(y) * 32, 32, 32};
        DrawTexturePro(tile_map, source, dest, {0, 0}, 0.0f, WHITE);
      }
    }

		if (p.hp > 0) {
			camera_view.target = {p.pos.x, p.pos.y};
		} else if (p.hp <= 0) {
			camera_view.target = {640, 360};
		} else if ((e.hp <= 0) && (e2.hp <= 0)) {
			camera_view.target = {640, 360};
		}
		

		p.Update(delta_time);
		e.Update(delta_time);
		e2.Update(delta_time);

		BeginDrawing();
		BeginMode2D(camera_view);
		ClearBackground(BLACK);

		if (p.hp > 0) {
			p.Draw();
			if (e.hp > 0) {
				e.Draw();
			}

			if (e2.hp > 0) {
				e2.Draw();
			}
		} else if (p.hp <= 0) {
			DrawText("You Lose!", (440), (260), 100, WHITE);
		} else if ((e.hp <= 0) && (e2.hp <= 0)) {
			DrawText("You Win!", (440), (260), 100, WHITE);
		}
  



		EndMode2D();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
