//Only detect damage at the first frame, not all frames
//Use melee; Create circle around player when it attacks

#include <raylib.h>
#include "raymath.h"
#include <vector>

#include "PlayerStateMachine.cpp"
#include "EnemyStateMachine.cpp"

const float FPS(60.0f);

int camType;

Camera2D camera_view = {0};

int main() {
	InitWindow(1280, 720, "Castro_Hung_Taino_Homework03");
  
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

	while(!WindowShouldClose()) {
		float delta_time = GetFrameTime();
		
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
