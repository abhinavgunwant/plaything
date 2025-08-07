#include <iostream>

#include "systems.hpp"
#include "entity-manager.hpp"
#include "inventory-manager.hpp"
#include "game-state.hpp"

using namespace std;

#define KEY(a, b)   a + to_string(b+1) + "Qty"
#define XSTR(a)     "x" + to_string(a)

void furnaceCook(
    uint8_t *health, uint8_t shd, uint8_t mhd, uint8_t hhd, uint16_t *qty,
    ItemType * oreTypes, ItemType * outTypes, uint16_t *out,
    bool inUse
) {

    // Macro to update outputs
    #define U_OUT(decr, type)    do {\
        health[j] -= decr;\
        if (health[j] > prevHealth) {\
            if (qty[j] > 0) {\
                --qty[j];\
                for (int i=0; i<3; ++i) {\
                    if (outTypes[i] == type && out[i] < 1000) {\
                        ++out[i];\
                        if (inUse) {\
                            cout<<"\n!! out"<<i+1<<"Qty: "<<out[i];\
                            im.updateItemMenuText(KEY("out", i), XSTR(out[i]));\
                        }\
                        break;\
                    } else if (outTypes[i] == ITEM_NONE) {\
                        outTypes[i] = type; out[i] = 1;\
                        if (inUse) {\
                            cout<<"\n!! out"<<i+1<<"Qty: "<<out[i];\
                            im.updateItemMenuText(KEY("out", i), XSTR(out[i]));\
                        }\
                        break;\
                    }\
                }\
                if (inUse) {\
                    im.updateItemMenuText(KEY("ore", j), XSTR(qty[j]));\
                }\
            } else { health[j] = 0; }\
        }\
    } while(false)

    for (int j=0; j<2; ++j) {
        uint8_t prevHealth = health[j];

        switch (oreTypes[j]) {
            case FARM_ITEM_SULFUR_ORE: U_OUT(shd, FARM_ITEM_SULFUR); break;
            // case FARM_ITEM_METAL_ORE: U_OUT(mhd, FARM_ITEM_METAL_ORE); break;
            case FARM_ITEM_METAL_ORE: U_OUT(mhd, FARM_ITEM_METAL_FRAGS); break;
            case FARM_ITEM_HQM_ORE: U_OUT(hhd, FARM_ITEM_HQM); break;
            default: break;
        }

        if (qty[j] == 0 && health != 0 && oreTypes[j] != ITEM_NONE) {
            health = 0;
            oreTypes[j] = ITEM_NONE;
        }
    }

    #undef UPDATE_OUT
}

void FurnaceSystem::run(float frameTime) {
    if (ids.empty()) { return; }

    const float frameTime_ = frameTime * 255;
    const uint8_t sulfurHealthDelta = frameTime_;
    const uint8_t woodHealthDelta = 0.8 * frameTime_;
    const uint8_t metalHealthDelta = 0.33 * frameTime_;
    const uint8_t hqmHealthDelta = 0.2 * frameTime_;

    // cout << "\nRunning furnace system. frametime: " << frameTime << ", sulfurHealthDelta: "
    //     << (int)sulfurHealthDelta << ", woodHealthDelta: " << (int)woodHealthDelta << ", metalHealthDelta: "
    //     << (int)metalHealthDelta << ", hqmHealthDelta: " << (int)hqmHealthDelta;

    uint16_t qty[2]; // ore quantities
    uint8_t health[2];

    ItemType oreTypes[2];
    ItemType outTypes[3];
    uint16_t out[3];

    for (uint32_t id: ids) {
        Entity3D e = em.get3DEntity(id);

        if (e.getId() == 0) {
            removeFurnaceId(id);
            continue;
        }

        bool inUse = em.entityIdInUse == e.getId();

        #define WOOD            e.item.itemData.furnace.woodQty
        #define WOOD_HEALTH     e.item.itemData.furnace.woodHealth
        #define ITEM_FURNACE    e.item.itemData.furnace

        for (int i=0; i<3; ++i) {
            if (i < 2) {
                oreTypes[i] = ITEM_FURNACE.oreTypes[i];
                qty[i] = ITEM_FURNACE.oreQty[i];
                health[i] = ITEM_FURNACE.oreHealth[i];
            }

            outTypes[i] = ITEM_FURNACE.outputTypes[i];
            out[i] = ITEM_FURNACE.outputQty[i];
        }

        if (WOOD > 0) {
            uint16_t wQty = WOOD;
            uint8_t prevHealth = WOOD_HEALTH;
            uint8_t wHealth = WOOD_HEALTH;

            wHealth -= woodHealthDelta;

            if (wHealth > prevHealth) {
                if (wQty > 0) {
                    --wQty;

                    for (int i=0; i<3; ++i) {
                        if (outTypes[i] == FARM_ITEM_CHARCOAL && out[i] < 1000) {
                            ++out[i];
                            if (inUse) {
                                im.updateItemMenuText(KEY("out", i), XSTR(out[i]));
                            }
                            break;
                        } else if (outTypes[i] == ITEM_NONE) {
                            out[i] = 1;
                            outTypes[i] = FARM_ITEM_CHARCOAL;
                            if (inUse) {
                                im.updateItemMenuText(KEY("out", i), XSTR(out[i]));
                            }
                            break;
                        }
                    }

                    if (inUse) {
                        im.updateItemMenuText("woodQty", "x"+to_string(wQty));
                    }
                }

                if (wQty == 0) {
                    wHealth = 0;
                }
            }

            WOOD = wQty;
            WOOD_HEALTH = wHealth;

            // cout << ". Wood health before: " << (int) prevHealth << ", and after: " << (int)wHealth;
        }

        furnaceCook(
            health, sulfurHealthDelta, metalHealthDelta, hqmHealthDelta, qty,
            oreTypes, outTypes, out, inUse
        );

        for (int i=0; i<3; ++i) {
            if (i < 2) {
                ITEM_FURNACE.oreTypes[i] = oreTypes[i];
                ITEM_FURNACE.oreQty[i] = qty[i];
                ITEM_FURNACE.oreHealth[i] = health[i];
            }

            ITEM_FURNACE.outputTypes[i] = outTypes[i];
            ITEM_FURNACE.outputQty[i] = out[i];
        }

        // e.item.printState();

        em.update3DEntity(id, e);

        #undef WOOD
        #undef WOOD_HEALTH
        #undef ITEM_FURNACE
    }
}

void FurnaceSystem::addFurnaceId(uint32_t id) { ids.push_back(id); }

void FurnaceSystem::removeFurnaceId(uint32_t id) {
    for (
        vector<uint32_t>::iterator itr = ids.begin();
        itr<ids.end();
        ++itr
    ) {
        if ((*itr) == id) {
            ids.erase(itr);
            return;
        }
    }
}

