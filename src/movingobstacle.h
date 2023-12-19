#ifndef MOVINGOBSTACLE_H
#define MOVINGOBSTACLE_H

#include <random>
#include "snake.h"
#include "gamestate.h"
#include <vector>
#include <thread>

class MovingObstacle
{
public:
    MovingObstacle();
    ~MovingObstacle();
    void simulate(Snake &snake, GameState::States &gamestate);
    void PlaceMovingObstacle();
    void waitForUnpause(GameState::States &gamestate);
    //getters setters
    SDL_Point GetMovingObstacle() const;

protected:
    std::vector<std::thread> threads;

private:
    void moveObstacle(Snake &snake, GameState::States &gamestate);
    size_t grid_width{32};
    size_t grid_height{32};
    int obstacleSpeed;
    int speedAdjustmentCounter;
    SDL_Point movingObstacle;

    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;
};

#endif