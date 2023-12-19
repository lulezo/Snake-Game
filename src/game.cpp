#include "game.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;

  // Add obstacles
  for (size_t i = 0; i < nObstacles; ++i)
  {
    obstacles.push_back(std::make_shared<MovingObstacle>());
  }

  // Create a thread for each obstacle
  //for (size_t i = 0; i < obstacles.size(); ++i)
  //{
    //obstacles[i]->simulate(snake, gamestate.state);
  //}

  while (gamestate.state != GameState::States::quit) {                
    if (gamestate.state == GameState::States::running)
    {
      frame_start = SDL_GetTicks();

      // Continuously get the coordinates of the moving obstacles 
      if (score > 2)
      {
        for (size_t i = 0; i < obstacles.size(); ++i)
        {
          if (!movingObstaclesPlaced)
          {
            obstacles[i]->simulate(snake, gamestate.state);
          }
          movingObstaclesPoints[i] = obstacles[i]->GetMovingObstacle();
        }
        movingObstaclesPlaced = true;
      }

      // Input, Update, Render - the main game loop.
      controller.HandleInput(gamestate.state, snake);
      Update();
      renderer.Render(snake, food, poison, movingObstaclesPoints);
      
      frame_end = SDL_GetTicks();

      // Keep track of how long each loop through the input/update/render cycle takes.
      frame_count++;
      frame_duration = frame_end - frame_start;

      // After every second, update the window title.
      if (frame_end - title_timestamp >= 1000) {
        renderer.UpdateWindowTitle(score, frame_count);
        frame_count = 0;
        title_timestamp = frame_end;
      }
      // If the time for this frame is too small (i.e. frame_duration is smaller than the target ms_per_frame), 
      // delay the loop to achieve the correct frame rate.
      if (frame_duration < target_frame_duration) {
        SDL_Delay(target_frame_duration - frame_duration);
      }
    }
    else if (gamestate.state == GameState::States::paused)
    {
      renderer.Pause();
      SDL_Event e;
      while (SDL_PollEvent(&e)) 
      {
        if (e.type == SDL_KEYDOWN) 
        {
          if (e.key.keysym.sym == SDLK_SPACE)
          {
            gamestate.state = GameState::States::running;
          }
        }
      }
    }
  }
}
 
void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::PlacePoison() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item nor a food item before placing poison.
    if (!snake.SnakeCell(x, y) && !Game::FoodCell(x, y)) {
      poison.x = x;
      poison.y = y;
      poisonPlaced = true;
      return;
    }
  }
}

// Inefficient method to check if cell is occupied by food.
bool Game::FoodCell(int x, int y) {
  if (x == static_cast<int>(food.x) && y == static_cast<int>(food.y)) {
    return true;
  }
  return false;
}

bool Game::PoisonCell(int x, int y) {
  if (x == static_cast<int>(poison.x) && y == static_cast<int>(poison.y)) {
    return true;
  }
  return false;
}

void Game::Update() {
  if (!snake.alive || gamestate.state == GameState::States::quit) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }

  // Place poison only when score is greater than 2
  if (!poisonPlaced && score > 1)
  {
    PlacePoison();
  }
  // Check if there's poison over here
  if (poison.x == new_x && poison.y == new_y) {
    score--;
    PlacePoison();
    // Shrink snake and decrease speed.
    if (snake.size > 1) {
      snake.ShrinkBody();
      snake.speed -= 0.02;
    }
    else { snake.alive = false; }
  }

  // Check if there's moving obstacle over here
  for (auto &pointInObstacle : movingObstaclesPoints)
  {
    if (pointInObstacle.x == new_x && pointInObstacle.y == new_y)
    {
      snake.alive = false;
      return;
    }
    for (auto &pointInSnake : snake.body)
    {
      if (pointInObstacle.x == pointInSnake.x && pointInObstacle.y == pointInSnake.y)
      {
      snake.alive = false;
      return;
      } 
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }