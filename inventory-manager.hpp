#ifndef PLAYTHING_INVENTORY_MANAGER
#define PLAYTHING_INVENTORY_MANAGER

#include <inttypes.h>
#include <vector>
#include <unordered_map>

#include "common.hpp"
#include "items.hpp"

#define INV_OUTER_BOX_COLOR { 0, 0, 0, 160 }
#define INV_SLOT_DIMENSION SCREEN_HEIGHT/10
#define INV_ROWS 4
#define INV_SLOTS_ROW 6
#define INV_SLOT_GAP 10
#define INV_SLOT_START_X (SCREEN_WIDTH - 6 * INV_SLOT_DIMENSION - INV_SLOT_GAP * (INV_SLOTS_ROW - 1)) / 2
#define INV_RECT(x, y, dim) Rect(x, y, dim, dim, COLOR_INV_BG)
#define INV_RECTA(x, y, dim) Rect(x, y, dim, dim, COLOR_INV_BG_ACTIVE)
#define UTIL_BELT_IMG(x, y, dim, i) Img(x, y, dim, dim, utilityBelt[i].item.invImageTexture)

#define UTIL_BELT_SLOTS 6
#define UTIL_BOX_Y SCREEN_HEIGHT - INV_SLOT_DIMENSION - INV_SLOT_GAP

using namespace std;

/** Array containing all the items that are allowed in the inventory */
extern const uint8_t ALLOWED_INVENTORY_ITEMS[];

struct InventorySlot {
    /** Entity id of the inventory rectangle. */
    uint32_t rectEntityId;

    /** Entity id of the inventory image. */
    uint32_t imgEntityId;

    /** Index of item in the ALLOWED_INVENTORY_ITEMS array */
    Item item;
};

class InventoryManager {
    private:
        /** Index of the active utility belt item */
        int8_t ubIndex;
        float menuBlockTimer;

        /** Contains the IDs of menu entities */
        vector<uint32_t> invMenuEntityIds;

        /** Whether utility belt has been initialized. */
        bool ubInitPending;
        bool inventoryShown;
        bool entityMenuShown;
        bool inventoryMenuBlocked;

        InventorySlot utilityBelt[6];
        InventorySlot userInventory[24];

        unordered_map<string, uint32_t> itemMenuMap;

        /** Lazily initialize utility belt. */
        void initUtilBelt();

    public:
        InventoryManager();

        uint32_t getItemMenuEntityId(string key);
        void updateItemMenuText(string key, string newText);

        /** Shows inventory menu. */
        void showInventoryMenu();

        /** Hides inventory menu. */
        void hideInventoryMenu();
        void showEntityInteractionMenu(uint32_t);
        void hideEntityInteractionMenu();
        void showUtilityBelt();
        void hideUtilityBelt();

        int8_t getUbIndex();

        /** Set what item is active in the utility belt */
        void setUbIndex(int8_t index);

        void updateMenuInteraction(float time);

        /** Whether any item in utility belt is active. */
        bool isUbActive();
        Item getActiveUbItem();

        bool isInventoryShown();
        bool isEntityMenuShown();
        bool isInventoryMenuBlocked();
};

extern InventoryManager im;

#endif

