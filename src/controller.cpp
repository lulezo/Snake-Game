#include "controller.h"

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite, double angle) const {
  if (snake.direction != opposite || snake.size == 1) 
  {
    snake.direction = input;
    snake.angle = angle;
  }
  return;
}

void Controller::HandleInput(GameState::States &gamestate, Snake &snake) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      gamestate = GameState::States::quit;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown, 270);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp, 90);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight, 180);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft, 0);
          break;
        
        case SDLK_SPACE:
        {
          gamestate = GameState::States::paused;
          break;
        }
      }
    }
  }
}