#ifndef TILES
#define TILES

#include <raylib.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

struct Tile {
  Rectangle location;
  bool has_collision;
};

// Level Data containing the string of the tile_map's address on the local file system
// Determines the Number of Tiles and the Grid. 
struct LevelData {
  std::string TILE_MAP;
  int NUM_TILES;
  int GRID_X_NUM;
  int GRID_Y_NUM;
  std::vector<std::vector<int>> GRID;
};

// Loads the level LevelData Itself and reads the level_data file
void LoadLevelData(LevelData& level, std::ifstream& level_data){
  // Here comes the formatting of the level_data; 
  // This should include: Enemy Starting Positions - 3 Enemies Must Exist
  // Player's Starting Positions
  // Would be nice if we can determine what tiles are impassable;
  level_data.ignore(10, '\n');
  level_data >> level.TILE_MAP;
  level_data.ignore(256, '\n');
  level_data.ignore(10, '\n');
  level_data >> level.NUM_TILES;
  level_data.ignore(256, '\n');
  level_data.ignore(4, '\n');
  level_data >> level.GRID_X_NUM >> level.GRID_Y_NUM;
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
  }
}
#endif
