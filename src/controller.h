#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "SDL2/SDL.h"
#include "gamestate.h"
#include "snake.h"


class Controller {
 public:
    void HandleInput(GameState::States &gamestate, Snake &snake) const;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite, double angle) const;
};

#endif