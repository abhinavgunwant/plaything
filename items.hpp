#ifndef PLAYTHING_ITEMS
#define PLAYTHING_ITEMS

#include <cstdint>
#include "raylib.h"

enum ItemType {
    ITEM_NONE,
    ITEM_BLUEPRINT,
    ITEM_HAMMER,
    ITEM_ASSAULT_RIFLE,
    ITEM_PISTOL,
};

class Item {
    public:
        ItemType type;
        Texture2D invImageTexture;
        uint8_t clip;
        uint8_t ammo;

        /** Health of the item */
        uint8_t health;

        Item();
        Item(ItemType type);
        Item(ItemType type, Image invImage); // TODO: if not needed, remove this. The invoice icon drawing is being taken care of by the entity manager
        void onClick();
        void onRightClick();
        void onKeyPress();
};

#endif

