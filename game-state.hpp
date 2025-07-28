#ifndef PLAYTHING_GAME_STATE
#define PLAYTHING_GAME_STATE

struct State {
	bool cursorEnabled;
    short utilBeltActiveIndex;

    /**
     * Holds the entity id of the first box of util belt.
     */
    int utilBeltEntityIdStart;
};

extern State globalState;

#endif
