//To compile: g++ -Isrc/include -Lsrc/lib -o snake src/main.cpp src/controller.cpp src/game.cpp src/renderer.cpp src/snake.cpp src/movingobstacle.cpp src/gamestate.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -pthread

#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main(int argc, char* argv[]) {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};


  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}