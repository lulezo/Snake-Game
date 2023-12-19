#include "movingobstacle.h"
#include <iostream>

MovingObstacle::MovingObstacle() : 
    engine(dev()),
    random_w(0, static_cast<int>(grid_width - 1)),
    random_h(0, static_cast<int>(grid_height - 1))
{
    obstacleSpeed = 1;
    speedAdjustmentCounter = 0;
    PlaceMovingObstacle();
}

void MovingObstacle::simulate(Snake &snake, GameState::States &gamestate)
{
    threads.emplace_back(std::thread(&MovingObstacle::moveObstacle, this, std::ref(snake), std::ref(gamestate)));
}

void MovingObstacle::moveObstacle(Snake &snake, GameState::States &gamestate)
{
    while (true) {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::cout << "Hello from worker thread # " << std::this_thread::get_id() << std::endl;
        if (!snake.alive || gamestate == GameState::States::quit)
        {
            break;
        }
        if (gamestate == GameState::States::paused)
        {
            //wait until gamestate.state = running
            MovingObstacle::waitForUnpause(gamestate);
        }
    
        // Move the obstacle with the desired speed
        if (speedAdjustmentCounter > 50) {
            movingObstacle.x += obstacleSpeed;
            speedAdjustmentCounter = 0;
        }
        // If crosses border, reset values
        movingObstacle.x = fmod(movingObstacle.x + grid_width, grid_width);
        speedAdjustmentCounter++;
    }
}

void MovingObstacle::waitForUnpause(GameState::States &gamestate)
{
   while (true)
    {
        if (gamestate == GameState::States::running)
            return;
    }
}

void MovingObstacle::PlaceMovingObstacle()
{
    int x, y;
    x = random_w(engine);
    y = random_h(engine);
    
    movingObstacle.x = x;
    movingObstacle.y = y;
}

SDL_Point MovingObstacle::GetMovingObstacle() const
{
    return movingObstacle;
}

MovingObstacle::~MovingObstacle() {
    for (auto &t : threads)
    {
        t.join();
    }
}