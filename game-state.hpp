#ifndef PLAYTHING_GAME_STATE
#define PLAYTHING_GAME_STATE

#include "raylib.h"

#include "items.hpp"

struct State {
	bool cursorEnabled;
    short utilBeltActiveIndex;

    /**
     * Holds the entity id of the first box of util belt.
     */
    int utilBeltEntityIdStart;

    Item utilityItems[6];

    /** The id of item that the user is currEntly using. `0` if none. */
    uint32_t itemIdInUse;
};

extern State globalState;

#endif
