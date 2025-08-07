#ifndef PLAYTHING_ITEMS
#define PLAYTHING_ITEMS

#include <cstdint>
#include <string>
#include "raylib.h"

#define DEFAULT_FURNACE { 0, 0, { 0, 0 }, { 0, 0 }, { 0, 0, 0 },\
    { ITEM_NONE, ITEM_NONE },\
    { ITEM_NONE, ITEM_NONE, ITEM_NONE }, false }

#define DEMO_FURNACE(wood, ore, output, outputType, charcoal, oreType, on) {\
    wood, 255, { ore, 0 }, { 255, 0 }, { output, charcoal, 0 },\
    { oreType, ITEM_NONE },\
    { outputType, FARM_ITEM_CHARCOAL, ITEM_NONE }, on }

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
    ITEM_STORAGE_BOX_SMALL,
    ITEM_STORAGE_BOX_LARGE,
    FARM_ITEM_WOOD,
    FARM_ITEM_CHARCOAL,
    FARM_ITEM_STONES,
    FARM_ITEM_METAL_ORE,
    FARM_ITEM_METAL_FRAGS,
    FARM_ITEM_HQM_ORE,
    FARM_ITEM_HQM,
    FARM_ITEM_SULFUR_ORE,
    FARM_ITEM_SULFUR,
};

extern string iconPaths[];

/**
 * Represents the ore node (or stone node).
 */
struct OreNode {
    uint8_t state;
};

struct Furnace {
    /** Quantity of wood in the input slot. */
    uint16_t woodQty;

    /** Health of wood between 0-255 */
    uint8_t woodHealth;

    /** Quantity of ores in their respective slots. */
    uint16_t oreQty[2];

    /** Health of ores between 0-255 */
    uint8_t oreHealth[2];

    /** Quantity of output items in their respective slots. */
    uint16_t outputQty[3];

    /** Types of ores in their respective slots. */
    ItemType oreTypes[2];

    /** Types of output items in their respective slots. */
    ItemType outputTypes[3];

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

        uint16_t qty;

        uint8_t clip; // TODO: remove
        uint8_t ammo; // TODO: remove

        ItemData itemData;

        /** Health of the item */
        uint16_t health;

        Item();
        Item(ItemType type);
        Item(ItemType type, Image invImage); // TODO: if not needed, remove this. The invoice icon drawing is being taken care of by the entity manager
        ~Item();

        void printState();

        //// Following group of member functions work only while active in
        //// utility belt.
        void onClick();
        void onRightClick();
        void onKeyPress();
        /**
         * Controls what happens when user presses the `E` key.
         *
         * ## params
         * - entityId - the entity id of the entity on which the user pressed
         *   `E`.
         */
        void onUse(uint32_t entityId);
};

string getItemTypeName(ItemType type);

#endif

