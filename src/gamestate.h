#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState {
public:
    GameState();
    enum class States {running, paused, quit};
    States state;
};

#endif