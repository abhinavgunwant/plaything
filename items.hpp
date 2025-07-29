#ifndef PLAYTHING_ITEMS
#define PLAYTHING_ITEMS

#include <cstdint>
#include <string>
#include "raylib.h"

using namespace std;

enum ItemType {
    ITEM_NONE,
    ITEM_BLUEPRINT,
    ITEM_HAMMER,
    ITEM_ASSAULT_RIFLE,
    ITEM_PISTOL,
    ITEM_FURNACE,
    ITEM_TREE,
    ITEM_NODE_STONE,
    ITEM_NODE_METAL,
    ITEM_NODE_SULFUR,
    ITEM_BUILDING_FOUNDATION,
};

/**
 * Types of items that can be farmed.
 */
enum FarmItemTypes {
    FARM_ITEM_NONE,
    FARM_ITEM_WOOD,
    FARM_ITEM_STONES,
    FARM_ITEM_METAL_ORE,
    FARM_ITEM_METAL_FRAGS,
    FARM_ITEM_HQM_ORE,
    FARM_ITEM_HQM,
    FARM_ITEM_SULFUR_ORE,
    FARM_ITEM_SULFUR,
};

/**
 * Represents the ore node (or stone node).
 */
struct OreNode {
    uint8_t state;
};

struct Furnace {
    /** Quantity of wood in the input slot. */
    uint16_t woodQty;

    /** Quantity of ores in their respective slots. */
    uint16_t oreQty[2];

    /** Quantity of output items in their respective slots. */
    uint16_t outputQty[3];

    /** Types of ores in their respective slots. */
    FarmItemTypes oreTypes[2];

    /** Types of output items in their respective slots. */
    FarmItemTypes outputTypes[3];

    /** Whether the furnace is running */
    bool on;
};

union ItemData {
    Furnace furnace;
    OreNode node;
};

class Item {
    private:
        /** used for throttling the use method. */
        bool interactionBlocked;

    public:
        ItemType type;
        Texture2D invImageTexture;
        uint8_t clip;
        uint8_t ammo;

        ItemData itemData;

        /** Health of the item */
        uint16_t health;

        Item();
        Item(ItemType type);
        Item(ItemType type, Image invImage); // TODO: if not needed, remove this. The invoice icon drawing is being taken care of by the entity manager
        ~Item();

        void onClick();
        void onRightClick();
        void onKeyPress();
        void onUse();
};

string getFarmItemName(FarmItemTypes type);
string getItemTypeName(ItemType type);

#endif

