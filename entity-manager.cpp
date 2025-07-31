#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>
#include <string>

#include "items.hpp"
#include "raylib.h"

#include "common.hpp"
#include "shapes.hpp"
#include "entity-manager.hpp"
#include "shapes.hpp"
#include "game-state.hpp"

#define INV_OUTER_BOX_COLOR { 0, 0, 0, 160 }
#define INV_SLOT_DIMENSION SCREEN_HEIGHT/10
#define INV_ROWS 4
#define INV_SLOT_NUM 6
#define INV_SLOT_GAP 10
#define INV_SLOT_START_X (SCREEN_WIDTH - 6 * INV_SLOT_DIMENSION - INV_SLOT_GAP * (INV_SLOT_NUM - 1)) / 2

#define UTIL_BOX_Y SCREEN_HEIGHT - invBoxDim - INV_SLOT_GAP

#define ADD(x)      menuEntityIds.push_back(x)
#define E           em.addEntity

using namespace std;

BoundingBox getBBForCube(Vector3 position, CubeData cubeData) {
    float w_2 = cubeData.width/2;
    float l_2 = cubeData.length/2;
    float h_2 = cubeData.height/2;

    float x1 = position.x - h_2;
    float x2 = position.x + h_2;
    float y1 = position.y - w_2;
    float y2 = position.y + w_2;
    float z1 = position.z - l_2;
    float z2 = position.z + l_2;

    return BoundingBox {
        { min(x1, x2), min(y1, y2), min(z1, z2) },
        { max(x1, x2), max(y1, y2), max(z1, z2) }
    };
}

// Entity2D declarations
Entity2D::Entity2D() { this->id = 0; }

Entity2D::Entity2D(uint32_t id) { this->id = id; }

Entity2D::Entity2D(uint32_t id, Shape2D shape) {
    this->id = id;
    this->shapes.push_back(shape);
}

Entity2D::Entity2D(string text, int x, int y, int fontSize, Color color) {
    this->text = text;
    Shape2D s;
    s.x = x;
    s.y = y;
    s.type = SHAPE_2D_TEXT;
    s.shapeData.textData.fontSize = fontSize;
    s.shapeData.textData.color = color;

    shapes.push_back(s);
}

Entity2D::Entity2D(uint32_t id, string text, int x, int y, int fontSize, Color color) {
    this->id = id;
    this->text = text;
    Shape2D s;
    s.x = x;
    s.y = y;
    s.type = SHAPE_2D_TEXT;
    s.shapeData.textData.fontSize = fontSize;
    s.shapeData.textData.color = color;

    shapes.push_back(s);
}

Entity2D::~Entity2D() {
    for (Shape2D shape : shapes) {
        if (shape.shapeData.textData.heapAllocated == true) {
            cout << "\nfound a heap allocated string: " << shape.shapeData.textData.text;
            delete shape.shapeData.textData.text;
            cout << "\ndeleted the heap allocated string!";
            shape.shapeData.textData.heapAllocated = false;
        }
    }
}

Entity2D::Entity2D(Shape2D shape) {
    this->id = 0;
    this->shapes.push_back(shape);
}

void Entity2D::addShape(Shape2D &shape) { this->shapes.push_back(shape); }
void Entity2D::addShape(Shape2D shape) { this->shapes.push_back(shape); }
uint32_t Entity2D::getId() { return this->id; }

// Entity3D declarations
Entity3D::Entity3D() { id = 0; }

Entity3D::Entity3D(uint32_t id) { this->id = id; }

Entity3D::Entity3D(uint32_t id, Shape3D &shape) {
    this->id = id;
    shapes.push_back(shape);
}

Entity3D::Entity3D(uint32_t id, Shape3D shape) {
    this->id = id;
    shapes.push_back(shape);
}

Entity3D::Entity3D(Shape3D shape) {
    id = 0;
    shapes.push_back(shape);
}

Entity3D::Entity3D(Shape3D &shape) {
    id = 0;
    shapes.push_back(shape);
}

Entity3D::~Entity3D() { }

uint32_t Entity3D::getId() { return id; }

void Entity3D::addShape(Shape3D shape) { shapes.push_back(shape); }
void Entity3D::addShape(Shape3D &shape) { shapes.push_back(shape); }

// EntityManager declarations
EntityManager::EntityManager() { init(); }

void EntityManager::init() {
    idCounter = 1;

    entities2d.reserve(128);
    entities3d.reserve(256);

	cout << "\nEntity manager created!Initializing camera...";
	initCamera();
    cout << "\n--> Camera initialized!";
    // addEntity(Line2D(SCREEN_WIDTH_2, SCREEN_HEIGHT_2 - 10, SCREEN_WIDTH_2, SCREEN_HEIGHT_2 + 10, RED));
}

void EntityManager::initCamera() {
    this->camera = { 0 };
    this->camera.position = { 5.0f, 1.8f, 5.0f };
    this->camera.target = { 0.0f, 0.0f, 0.0f };
    this->camera.up = { 0.0f, 1.0f, 0.0f };
    this->camera.fovy = 60.0f;
    this->camera.projection = CAMERA_PERSPECTIVE;
}

void EntityManager::handleSystemEntityInit(Entity3D entity, uint32_t id) {
    switch (entity.item.type) {
        case ITEM_FURNACE: furnaceSystem.addFurnaceId(id); return;
        default: return;
    }
}

inline void EntityManager::setCameraPosition(Vector3 position) {
    this->camera.position = position;
}

void EntityManager::updateCameraTarget(Vector3 target) {
    this->camera.target = target;
}

uint32_t EntityManager::addEntity(Entity3D entity) {
    if (entity.getId() == 0) {
        uint32_t id = idCounter++;
        Entity3D newEntity = Entity3D(id);
        newEntity.shapes = entity.shapes;
        newEntity.item = entity.item;
        entities3d.push_back(newEntity);
        handleSystemEntityInit(newEntity, id);
        return id;
    }

    this->entities3d.push_back(entity);

    if (entity.getId() > idCounter) {
        idCounter = entity.getId() + 1;
    }

    handleSystemEntityInit(entity, entity.getId());
    return entity.getId();
}

uint32_t EntityManager::addEntity(Shape3D shape) {
    uint32_t id = this->idCounter++;
    Entity3D e = Entity3D(id);
    e.shapes.push_back(shape);
    entities3d.push_back(e);
    return id;
}

uint32_t EntityManager::addEntity(Shape3D &shape) {
    uint32_t id = this->idCounter++;
    Entity3D e = Entity3D(id);
    e.shapes.push_back(shape);
    entities3d.push_back(e);
    return id;
}

uint32_t EntityManager::addEntity(Entity2D entity) {
    if (entity.getId() == 0) {
        uint32_t id = this->idCounter++;
        Entity2D newEntity = Entity2D(id);
        newEntity.shapes = entity.shapes;
        entities2d.push_back(newEntity);
        return id;
    }

    entities2d.push_back(entity);
    return entity.getId();
}

uint32_t EntityManager::addEntity(string text, int x, int y, int fontSize, Color color) {
    uint32_t id = idCounter++;

    entities2d.push_back(Entity2D(id, text, x, y, fontSize, color));

    return id;
}

uint32_t EntityManager::addEntity(string text, int x, int y, Color color) {
    uint32_t id = idCounter++;

    entities2d.push_back(Entity2D(id, text, x, y, 20, color));

    return id;
}

uint32_t EntityManager::addEntity(Shape2D shape) {
    uint32_t id = this->idCounter++;
    Entity2D e = Entity2D(id, shape);
    entities2d.push_back(e);
    return id;
}

uint32_t EntityManager::addEntity(Shape2D &shape) {
    uint32_t id = this->idCounter++;
    Entity2D e = Entity2D(id, shape);
    entities2d.push_back(e);
    return id;
}

bool EntityManager::deleteEntity(uint32_t id) {
    if (entities2d.empty() && entities3d.empty()) { return false; }

    try {
        for (
            vector<Entity3D>::reverse_iterator itr = entities3d.rbegin();
            itr != entities3d.rend();
            ++itr
        ) {
            if ((*itr).getId() == id) {
                entities3d.erase(itr.base() - 1);

                if (idCounter == id + 1) {
                    idCounter = id;
                }

                return true;
            }
        }
    } catch (exception &e) {
        cout << "!!!! Exception: " << e.what();
    }

    try {
        for (
            vector<Entity2D>::reverse_iterator itr = entities2d.rbegin();
            itr != entities2d.rend();
            ++itr
        ) {
            if ((*itr).getId() == id) {
                entities2d.erase(itr.base() - 1);

                if (idCounter == id + 1) {
                    idCounter = id;
                }

                return true;
            }
        }
    } catch (exception &e) {
        cout << "!!!! Exception: " << e.what();
    }

    return false;
}

void EntityManager::deleteEntities(vector<uint32_t> ids) {
    vector<uint32_t> deletedEntities;
    deletedEntities.reserve(64);

    cout << "\nDeleting entities. ID counter at: " << idCounter;

    for (vector<uint32_t>::reverse_iterator i = ids.rbegin(); i != ids.rend(); ++i) {
        if (deleteEntity(*i)) {
            deletedEntities.push_back(*i);
        }
    }

    cout << "\nDeleted entities: ";

    for (uint32_t id: deletedEntities) { cout << id << ", "; }

    cout << ". ID counter at: " << idCounter;
}

EntityCollision EntityManager::getRayCastCollision(
    EntityCollisionType ect,
    float maxDistance
) {
    Ray ray = GetScreenToWorldRay({ SCREEN_WIDTH_2, SCREEN_HEIGHT_2 }, camera);
    RayCollision closestCollision;
    uint32_t entityId = 0;
    bool found = false;

    if (ect == ECT_TERRAIN) {
        // TODO: check if terrain is within the `maxDistance`.
        return { GetRayCollisionBox(ray, { { -50, -0.1, -50 }, { 50, 0, 50 } }), ENTITY_TYPE_3D, 1 };
    } else {
        float distance = maxDistance;

        for (Entity3D e : entities3d) {
            Shape3D s = *(e.shapes.begin());
            BoundingBox box;

            if (s.type == SHAPE_3D_CUBE) {
                box = getBBForCube(s.shapeData.cubeData.position, s.shapeData.cubeData);
            } else {
                continue;
            }

            RayCollision collision = GetRayCollisionBox(ray, box);

            if (collision.hit && collision.distance < distance && distance > 0) {
                found = true;
                distance = collision.distance;
                closestCollision = collision;
                entityId = e.getId();
            }
        }
    }

    if (found) {
        return { closestCollision, ENTITY_TYPE_3D, entityId };
    }

    return { false, numeric_limits<float>::max(), VEC_ZERO, VEC_ZERO };
}

Entity3D EntityManager::get3DEntity(uint32_t id) {
    for (Entity3D e : entities3d) {
        if (e.getId() == id) {
            return Entity3D(e);
        }
    }

    return Entity3D(0);
}

Entity2D EntityManager::get2DEntity(uint32_t id) {
    for (Entity2D e : entities2d) {
        if (e.getId() == id) {
            return Entity2D(e);
        }
    }

    return Entity2D(0);
}

void EntityManager::blockEntityInteraction(uint32_t id) {
    Entity3D e = get3DEntity(id);

    if (e.item.type != ITEM_NONE) {
        for (
            vector<BlockedEntity>::iterator itr = blockedEntities.begin();
            itr < blockedEntities.end();
            ++itr
        ) {
            if ((*itr).id == id) {
                cout << "\nReset the block timer for entity " << id << ".";
                (*itr).time = 0.5;
                return;
            }
        }

        cout << "\nAdded entitiy " << id << " to blocked list.";
        blockedEntities.push_back({ id, 0.5 });
    } else {
        cout << "\nItem was none!";
    }
}

void EntityManager::allowEntityInteraction(uint32_t id) {
    if (blockedEntities.empty()) { return; }

    Entity3D e = get3DEntity(id);

    if (e.item.type != ITEM_NONE) {
        for (
            vector<BlockedEntity>::iterator itr = blockedEntities.begin();
            itr < blockedEntities.end();
            ++itr
        ) {
            if ((*itr).id == id) {
                blockedEntities.erase(itr);
                return;
            }
        }
    }
}

bool EntityManager::isEntityBlocked(uint32_t id) {
    if (blockedEntities.empty()) { return false; }

    for (BlockedEntity e : blockedEntities) {
        if (e.id == id) { return true; }
    }

    return false;
}

// TODO: reimplement this!
void EntityManager::showEntityInteractionMenu(uint32_t id) {
    if (menuBlockTimer > 0) { return; }
    cout << "\nshowing menu";

    menuBlockTimer = 0.25;
    EnableCursor();

    Entity3D entity = get3DEntity(id);
    Color base = { 255, 255, 255, 80 };
    int xOffset = 1120;

    globalState.itemIdInUse = id;

    switch (entity.item.type) {
        case ITEM_FURNACE: {
            #define FURNACE     entity.item.itemData.furnace
            #define KEY(a, b)   a + to_string(b+1) + "Qty"
            #define QTY(a)      "x" + to_string((int) FURNACE.a)

            const uint16_t d = INV_SLOT_DIMENSION;
            ADD(E(Rect(xOffset, 200, 400, 500, INV_OUTER_BOX_COLOR)));
            ADD(E("Furnace", xOffset + 160, 210, WHITE));

            ADD(E("Wood:", xOffset + 175, 240, WHITE));
            ADD(E(Rect(xOffset + 155, 260, d, d, base)));

            ADD(E("Input:", xOffset + 170, 360, WHITE));
            ADD(E(Rect(xOffset + 105, 380, d, d, base)));
            ADD(E(Rect(xOffset + 205, 380, d, d, base)));

            ADD(E("Output:", xOffset + 165, 480, WHITE));
            ADD(E(Rect(xOffset + 55, 500, d, d, base)));
            ADD(E(Rect(xOffset + 155, 500, d, d, base)));
            ADD(E(Rect(xOffset + 255, 500, d, d, base)));

            if (FURNACE.woodQty > 0) {
                uint32_t woodId = E(QTY(woodQty), xOffset + 155, 330, 20, WHITE);
                ADD(woodId);
                itemMenuMap["woodQty"] = woodId;

                Image wood = LoadImage("./assets/wood_invIcon.png");
                ADD(E(Img(xOffset + 155, 260, d, d, wood)));
            }

            for (
                int i=0, offset1 = xOffset + 105, offset2 = xOffset + 55;
                i<3;
                ++i, offset1 += 100, offset2 += 100
            ) {
                if (FURNACE.outputQty[i] > 0) {
                    switch(FURNACE.outputTypes[i]) {
                        case FARM_ITEM_METAL_FRAGS: {
                            Image metalFrags = LoadImage("./assets/metalFrags_invIcon.png");
                            ADD(E(Img(offset2, 500, d, d, metalFrags)));
                            break;
                        }
                        case FARM_ITEM_CHARCOAL: {
                            Image charcoal = LoadImage("./assets/charcoal_invIcon.png");
                            ADD(E(Img(offset2, 500, d, d, charcoal)));
                            break;
                        }
                        default: continue;
                    }

                    uint32_t id1 = E(QTY(outputQty[i]), offset2, 570, WHITE);
                    
                    itemMenuMap[KEY("out", i)] = id1;
                    ADD(id1);
                }

                if (i > 1) { continue; }

                if (FURNACE.oreQty[i] > 0) {
                    switch(FURNACE.oreTypes[i]) {
                        case FARM_ITEM_METAL_ORE: {
                            Image metalOre = LoadImage("./assets/metalOre_invIcon.png");
                            ADD(E(Img(offset1, 380, d, d, metalOre)));
                            break;
                        }
                        default: continue;
                    }

                    uint32_t id2 = E(QTY(oreQty[i]), offset1, 450, WHITE);
                    itemMenuMap[KEY("ore", i)] = id2;
                    ADD(id2);
                }
            }


            if (FURNACE.on) {
                ADD(E("Turn Off", xOffset + 155, 630, WHITE));
                ADD(E(Rect(xOffset + 145, 600, 110, d, { 255, 0, 0, 160 })));
            } else {
                ADD(E("Turn On", xOffset + 155, 630, WHITE));
                ADD(E(Rect(xOffset + 155, 600, d, d, { 255, 0, 0, 160 })));
            }

            #undef KEY
            #undef QTY
            #undef FURNACE
            break;
        }
        default: break;
    }
}

void EntityManager::hideEntityInteractionMenu() {
    if (menuBlockTimer > 0) { return; }
    cout << "\nhiding menu";

    DisableCursor();
    menuBlockTimer = 0.25;

    deleteMenuEntities();
    globalState.itemIdInUse = 0;
    itemMenuMap.clear();
}

bool EntityManager::isEntityInteractiveMenuShown() { return !menuEntityIds.empty(); }

uint32_t EntityManager::getItemMenuEntityId(string key) {
    if (itemMenuMap.find(key) != itemMenuMap.end()) {
        return itemMenuMap[key];
    }

    return 0;
}

void EntityManager::updateItemMenuText(string key, string newText) {
    uint32_t id = getItemMenuEntityId(key);

    if (id == 0) { return; }

    Entity2D e = get2DEntity(id);

    #define TEXT_DATA   s.shapeData.textData

    if (!e.shapes.empty() && (*(e.shapes.begin())).type == SHAPE_2D_TEXT) {
        Shape2D s = (*(e.shapes.begin()));

        em.update2DEntity(
            id,
            Entity2D(id, newText, s.x, s.y, TEXT_DATA.fontSize, TEXT_DATA.color)
        );
    }

    #undef TEXT_DATA
}

void EntityManager::showInventoryMenu() {
    menuBlockTimer = 0.25;
    inventoryMenuBlocked = true;
    EnableCursor();

    // cout << "showing menu";

    const uint16_t dim = INV_SLOT_DIMENSION;
    const uint16_t dimGap = dim + INV_SLOT_GAP;
    const uint16_t xStart = INV_SLOT_START_X;
    const uint16_t xEnd = INV_SLOT_START_X + dimGap * 6 - INV_SLOT_GAP;
    const uint16_t yEnd = SCREEN_HEIGHT - dimGap * 2;
    const uint16_t yStart = yEnd - (INV_SLOT_GAP + dim)*4;

    const uint16_t outerBoxX = dimGap * 6 + INV_SLOT_GAP;
    const uint16_t outerBoxY = dimGap * 5 + INV_SLOT_GAP;

    ADD(E(Rect(xStart - INV_SLOT_GAP, yStart - INV_SLOT_GAP, outerBoxX, outerBoxY, INV_OUTER_BOX_COLOR)));

    for (uint16_t y = yStart; y <= yEnd; y += dimGap) {
        for (uint16_t x = xStart; x <= xEnd; x += dimGap) {
            ADD(E(Rect(x, y, dim, dim, { 0, 0, 0, 160 })));
        }
    }
}

void EntityManager::deleteMenuEntities() {
    deleteEntities(menuEntityIds);

    menuEntityIds.clear();
}

void EntityManager::hideInventoryMenu() {
    if (menuBlockTimer > 0) { return; }

    menuBlockTimer = 0.25;
    inventoryMenuBlocked = true;

    cout << "\nhiding menu";

    DisableCursor();

    deleteMenuEntities();
    globalState.itemIdInUse = 0;
    itemMenuMap.clear();
}

bool EntityManager::isInventoryMenuShown() { return isEntityInteractiveMenuShown(); }
bool EntityManager::isInventoryMenuBlocked() { return inventoryMenuBlocked; }

void EntityManager::updateEntityInteraction(float time) {
    if (menuBlockTimer > 0) {
        menuBlockTimer -= time;
    } else if (menuBlockTimer <= 0) {
        menuBlockTimer = 0;
        inventoryMenuBlocked = false;
    }

    if (blockedEntities.empty() && !inventoryMenuBlocked) { return; }

    bool runAgain;

    do {
        runAgain = false;

        for (
            vector<BlockedEntity>::iterator itr = blockedEntities.begin();
            itr < blockedEntities.end();
            ++itr
        ) {
            (*itr).time -= time;

            if ((*itr).time < 0) {
                cout << "\nRemoving entity " << (*itr).id << " from blocked list.";
                blockedEntities.erase(itr);
                runAgain = true;
                break;
            }
        }
    } while (runAgain);
}

void EntityManager::update2DEntity(uint32_t id, Entity2D newEntity) {
    bool found = false;
    uint32_t index = 0;

    for (
        vector<Entity2D>::iterator itr = entities2d.begin();
        itr < entities2d.end();
        ++itr) {
        if ((*itr).getId() == id) {
            // cout << "Entity with id: " << id << " updated!";

            if (newEntity.getId() != id) {
                Entity2D anotherEntity(id);
                anotherEntity.text = newEntity.text;
                anotherEntity.shapes = newEntity.shapes;

                (*itr) = anotherEntity;
            } else {
                (*itr) = newEntity;
            }
            return;
        }
    }

    if (found) {
        try {
            entities2d.at(index) = newEntity;
        } catch (exception &e) {
            cout << "\n!!!! exception while updating entity: " << e.what();
        }
    } else {
        cout << "\nError: The 2D entity was not found!";
    }
}

void EntityManager::update3DEntity(uint32_t id, Entity3D newEntity) {
    bool found = false;
    uint32_t index = 0;

    for (
        vector<Entity3D>::iterator itr = entities3d.begin();
        itr < entities3d.end();
        ++itr) {
        if ((*itr).getId() == id) {
            // cout << "Entity with id: " << id << " updated!";
            (*itr) = newEntity;
            return;
        }
    }

    if (found) {
        try {
            entities3d.at(index) = newEntity;
        } catch (exception &e) {
            cout << "\n!!!! exception while updating entity: " << e.what();
        }
    }
}

void EntityManager::drawEntities() {
    BeginMode3D(this->camera);

    for (Entity3D e : this->entities3d) {
        for (const Shape3D & shape : e.shapes) {
            draw3D(shape);
        }
    }

    EndMode3D();

    for (Entity2D e : this->entities2d) {
        if (!e.text.empty()) {
            Shape2D firstShape = (*(e.shapes.begin()));

            if (firstShape.type == SHAPE_2D_TEXT) {
                TextData t = firstShape.shapeData.textData;
                char c[2000];
                strcpy_s(c, (char *)e.text.c_str());
                DrawText(c, firstShape.x, firstShape.y, t.fontSize, t.color);
                continue;
            }
        }

        for (const Shape2D& shape : e.shapes) {
            draw2D(shape);
        }
    }
}

void EntityManager::draw3D(const Shape3D& shape) {
    // cout << "\nDrawing ";

    switch (shape.type) {
        case SHAPE_3D_CUBE: {
            CubeData c = shape.shapeData.cubeData;
            DrawCube(c.position, c.height, c.width, c.length, c.color);
            // cout << "cube";
            break;
        }
        case SHAPE_3D_LINE: {
            LineData l = shape.shapeData.lineData;
            DrawLine3D(l.startPos, l.endPos, l.color);
            // cout << "3d line";
            break;
        }
        default: break;
    }
}

uint32_t EntityManager::lastId() { return idCounter; }

void EntityManager::draw2D(const Shape2D& shape) {
    // cout << "\nDrawing ";

    switch (shape.type) {
        case SHAPE_2D_TEXT: {
            TextData t = shape.shapeData.textData;
            DrawText(t.text, shape.x, shape.y, t.fontSize, t.color);
            // cout << "text";
            break;
        }

        case SHAPE_2D_LINE: {
            Dimension2DData d = shape.shapeData.dimensionData;
            DrawLine(shape.x, shape.y, shape.x + d.width, shape.y + d.height, d.color);
            // cout << "2d line";
            break;
        }

        case SHAPE_2D_RECTANGLE: {
            Dimension2DData d = shape.shapeData.dimensionData;
            DrawRectangle(shape.x, shape.y, d.width, d.height, d.color);
            // cout << "rectangle";
            break;
        }

        case SHAPE_2D_IMAGE: {
            ImageData i = shape.shapeData.imageData;
            DrawTexture(shape.shapeData.imageData.texture, shape.x, shape.y, WHITE);
            break;
        }
    }
}

void EntityManager::manageEntities() {
    float frameTime = GetFrameTime();

    drawEntities();
    updateEntityInteraction(frameTime);
    runSystems(frameTime);

    if (!em.isEntityInteractiveMenuShown()) {
        UpdateCamera(em.getCamera(), CAMERA_FIRST_PERSON);
    }
}

void EntityManager::runSystems(float frameTime) {
    furnaceSystem.run(frameTime);
}

// EntityManager's Global instance definition
EntityManager em;

void setupEntities() {
    cout << "Setting up entities";

    // Basic scene
    em.addEntity(Cube(VEC_ZERO, 100.0f, -0.01f, 100.0f, BROWN));

    Entity3D grid = Entity3D(XAxis());
    grid.addShape(YAxis());
    grid.addShape(ZAxis());

    em.addEntity(Cube({ 0, 0.5f, 0 }, 1.0f, 1.0f, 1.0f, RED));
    em.addEntity(grid);
    em.addEntity(Text((char *)"Raylib Test", 5, 0, BLACK));

    // crosshair
    em.addEntity(Line2D(SCREEN_WIDTH_2 - 5, SCREEN_HEIGHT_2, 10, 0, RED));
    em.addEntity(Line2D(SCREEN_WIDTH_2, SCREEN_HEIGHT_2 - 5, 0, 10, RED));

    // GUI
    // Draw inventory boxes for utility belt
    const uint16_t invBoxDim = INV_SLOT_DIMENSION;
    const uint16_t y = UTIL_BOX_Y;
    uint16_t startX = INV_SLOT_START_X;

    bool first = true;

    int startXArr[6];

    for (uint8_t i=0; i<INV_SLOT_NUM; ++i, startX += invBoxDim + INV_SLOT_GAP, first = false) {
        startXArr[i] = startX;

        if (first) {
            uint32_t id = em.addEntity(Rect(startX, y, invBoxDim, invBoxDim, COLOR_INV_BG));
            globalState.utilBeltEntityIdStart = id;
            continue;
        }

        em.addEntity(Rect(startX, y, invBoxDim, invBoxDim, COLOR_INV_BG));
    }

    // Setup utility belt
    // Hammer

    globalState.utilityItems[0] = Item(ITEM_HAMMER);
    globalState.utilityItems[1] = Item(ITEM_BLUEPRINT);
    globalState.utilityItems[2] = Item(ITEM_PISTOL);
    globalState.utilityItems[3] = Item(ITEM_FURNACE);
    globalState.utilityItems[4] = Item(ITEM_NONE);
    globalState.utilityItems[5] = Item(ITEM_NONE);

    Image hammer = LoadImage("./assets/hammer_invIcon.png");
    Image blueprint = LoadImage("./assets/blueprint_invIcon.png");
    Image pistol = LoadImage("./assets/pistol_invIcon.png");
    Image furnace = LoadImage("./assets/furnace_invIcon.png");

    em.addEntity(Img(startXArr[0], y, invBoxDim, invBoxDim, hammer));
    em.addEntity(Img(startXArr[1], y, invBoxDim, invBoxDim, blueprint));
    em.addEntity(Img(startXArr[2], y, invBoxDim, invBoxDim, pistol));
    em.addEntity(Img(startXArr[3], y, invBoxDim, invBoxDim, furnace));
}

#undef INV_OUTER_BOX_COLOR
#undef INV_SLOT_DIMENSION
#undef INV_SLOT_NUM
#undef INV_SLOT_GAP
#undef INV_SLOT_GAP
#undef INV_SLOT_START_X
#undef E
#undef ADD

