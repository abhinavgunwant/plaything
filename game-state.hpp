#ifndef PLAYTHING_GAME_STATE
#define PLAYTHING_GAME_STATE

#include "items.hpp"

struct State {
	bool cursorEnabled;
    short utilBeltActiveIndex;

    /**
     * Holds the entity id of the first box of util belt.
     */
    int utilBeltEntityIdStart;

    /**
     * Array of items in the utility belt.
     */
    Item utilityItems[6];
};

extern State globalState;

#endif
