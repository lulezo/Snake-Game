#include "renderer.h"

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create texture for Snake Head
  surface = SDL_LoadBMP( "SnakeHead.bmp" );
  if (nullptr == surface) {
    std::cerr << "Surface could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
  bitmapTexHead = SDL_CreateTextureFromSurface(sdl_renderer, surface);
  SDL_FreeSurface(surface);

  // Create texture for Snake body
  surface = SDL_LoadBMP( "SnakeBody.bmp" );
  if (nullptr == surface) {
    std::cerr << "Surface could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
  bitmapTexBody = SDL_CreateTextureFromSurface(sdl_renderer, surface);
  SDL_FreeSurface(surface);

  // Initialize TTF
  if (TTF_Init() < 0) {
    std::cerr << "SDL could not initialize TTF.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Font
  PauseFont = TTF_OpenFont("24_Display.ttf", 32);
  surface = TTF_RenderText_Solid(PauseFont, "PAUSE", {255, 255, 255});
  pauseTex = SDL_CreateTextureFromSurface(sdl_renderer, surface);
  SDL_FreeSurface(surface);

  int xcenter = screen_width / 2, ycenter = screen_height / 2;
  PauseBlock.w = screen_width, PauseBlock.h = screen_height;
  PauseBlock.x = xcenter - (PauseBlock.w / 2), PauseBlock.y = ycenter - (PauseBlock.h / 2);
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_DestroyTexture(bitmapTexHead);
  SDL_DestroyTexture(bitmapTexBody);
  SDL_DestroyTexture(pauseTex);
  SDL_Quit();
}
 
  void Renderer::Render(Snake const snake, SDL_Point const &food, SDL_Point const &poison, std::vector<SDL_Point> const &movingObstaclesPoints)
  {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render poison
  SDL_SetRenderDrawColor(sdl_renderer, 0x7A, 0x4D, 0x7D, 0xFF);
  block.x = poison.x * block.w;
  block.y = poison.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render Moving Obstacle
  SDL_SetRenderDrawColor(sdl_renderer, 0x29, 0x52, 0x1C, 0xFF);
  for (auto &point : movingObstaclesPoints)
  {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's body
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderCopyEx(sdl_renderer, bitmapTexBody, NULL, &block, snake.angle, NULL, flip);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_RenderCopyEx(sdl_renderer, bitmapTexHead, NULL, &block, snake.angle, NULL, flip);
  } else {
    SDL_SetTextureColorMod(bitmapTexHead, 0xFF, 0x00, 0x00);
    SDL_RenderCopyEx(sdl_renderer, bitmapTexHead, NULL, &block, snake.angle, NULL, flip);
  }
  
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::Pause()
{
  SDL_SetRenderDrawColor(sdl_renderer, 0xEC, 0xFF, 0xFB, SDL_ALPHA_OPAQUE);
  SDL_RenderCopy(sdl_renderer, pauseTex, NULL, &PauseBlock);
  SDL_RenderPresent(sdl_renderer);
}