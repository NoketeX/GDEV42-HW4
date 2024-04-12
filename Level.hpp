#ifndef LEVEL 
#define LEVEL

#include <raylib.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Entity.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

struct Tile {
  Rectangle location;
  int has_collision;
};

// Level Data containing the string of the tile_map's address on the local file system
// Determines the Number of Tiles and the Grid. 
struct LevelData {
  std::string TILE_MAP;
  int NUM_TILES;
  int NUM_ENEMIES;
  int GRID_X_NUM;
  int GRID_Y_NUM;
  std::vector<std::vector<int>> GRID;
};

// Loads the level LevelData Itself and reads the level_data file
// Would include: Tile_Map, Num_Tiles, Num_Enemies, GridXGridY
void LoadLevelData(LevelData& level, std::ifstream& level_data){
  level_data.ignore(10, '\n');
  level_data >> level.TILE_MAP;
  level_data.ignore(256, '\n');
  
  level_data.ignore(10, '\n');
  level_data >> level.NUM_TILES;
  level_data.ignore(256, '\n');
  
  level_data.ignore(11, '\n');
  level_data >> level.NUM_ENEMIES;
  level_data.ignore(256, '\n');
  
  level_data.ignore(4, '\n'); 
  level_data >> level.GRID_X_NUM >> level.GRID_Y_NUM;
  level_data.ignore(256, '\n');
}

// Takes a file, for every row from when this is called
// it continually takes a row of string and checks for integers
void ConstructGrid(LevelData& level, std::ifstream& level_data){
  std::string line;
  while(getline(level_data, line)){
    std::vector<int> row;
    std::istringstream iss(line);
    int number;
    while(iss >> number){
      row.push_back(number);
    }
    level.GRID.push_back(row);
    // Empty space, breaks and goes into newline
    if(line.empty()){  
      break;
    }
  }
}

// Loads the Tiles
Tile LoadTile(std::ifstream& level_data){
  level_data.ignore(6, '\n');
  Rectangle rect;
  std::string collidable;
  bool collide = false;
  level_data >> rect.x >> rect.y >> rect.width >> rect.height >> collidable;
  // To support booleans from std::string, tried Char* but would not work
  if(collidable == "true"){
    collide = true;
  }
  Tile tile = {rect, collide};
  level_data.ignore(256, '\n');
  return tile;
}

// Enemy hp, position, radius, speed
Enemy LoadEnemy(std::ifstream& level_data){
  level_data.ignore(7, '\n');
  int hp;
  float x; float y;
  float radius;
  float speed;
  level_data >> hp >> x >> y >> speed;
  Enemy enemy = {hp, {x, y}, speed};
  level_data.ignore(256, '\n');
  return enemy;
}

// player hp, position, direction, radius, speed
Player LoadPlayer(std::istream& level_data){
  level_data.ignore(6, '\n');
  int hp;
  float x; float y;
  float x2; float y2;
  float radius;
  float speed;
  level_data >> hp >> x >> y >> x2 >> y2 >> radius >> speed;
  Player player = {hp, {x, y}, {x2, y2}, radius, speed};
  level_data.ignore(256, '\n');
  return player;
}

#endif
