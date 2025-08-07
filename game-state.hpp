#ifndef PLAYTHING_GAME_STATE
#define PLAYTHING_GAME_STATE

struct State {
    bool consoleShown;
    bool cursorEnabled;
};

extern State globalState;

#endif
