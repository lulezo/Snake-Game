#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include "SDL2/SDL.h"
#include "movingobstacle.h"  
#include "controller.h"
#include "renderer.h"                                   

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);

  int GetScore() const;
  int GetSize() const;
  bool FoodCell(int x, int y);
  bool PoisonCell(int x, int y);
  bool poisonPlaced{false};
  bool movingObstaclesPlaced{false};
  bool obstaclesPlaced{false};

  std::vector<std::shared_ptr<MovingObstacle>> obstacles;
  int nObstacles{3};
  std::vector<SDL_Point> movingObstaclesPoints{{16777216, 16777216}, {16777216, 16777216}, {16777216, 16777216}};

 private:
  Snake snake;
  GameState gamestate;                                           
  SDL_Point food;
  SDL_Point poison{16777216, 16777216};

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  int prevScore{0};

  void PlaceFood();
  void PlacePoison();
  void Update();
};

#endif