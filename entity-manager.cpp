#include <iostream>
#include <vector>
#include <limits>

#include "raylib.h"

#include "common.hpp"
#include "shapes.hpp"
#include "entity-manager.hpp"
#include "shapes.hpp"
#include "game-state.hpp"

using namespace std;

// Entity2D declarations
Entity2D::Entity2D() { this->id = 0; }

Entity2D::Entity2D(uint32_t id) { this->id = id; }

Entity2D::Entity2D(uint32_t id, Shape2D shape) {
    this->id = id;
    this->shapes.push_back(shape);
}

Entity2D::Entity2D(Shape2D shape) {
    this->id = 0;
    this->shapes.push_back(shape);
}

void Entity2D::addShape(Shape2D &shape) { this->shapes.push_back(shape); }
void Entity2D::addShape(Shape2D shape) { this->shapes.push_back(shape); }
uint32_t Entity2D::getId() { return this->id; }

// Entity3D declarations
Entity3D::Entity3D() { this->id = 0; }

Entity3D::Entity3D(uint32_t id) { this->id = id; }

Entity3D::Entity3D(uint32_t id, Shape3D &shape) {
    this->id = id;
    this->shapes.push_back(shape);
}

Entity3D::Entity3D(Shape3D shape) {
    this->id = 0;
    this->shapes.push_back(shape);
}

Entity3D::Entity3D(Shape3D &shape) {
    this->id = 0;
    this->shapes.push_back(shape);
}

uint32_t Entity3D::getId() { return this->id; }

void Entity3D::addShape(Shape3D shape) { this->shapes.push_back(shape); }
void Entity3D::addShape(Shape3D &shape) { this->shapes.push_back(shape); }

// EntityManager declarations
EntityManager::EntityManager() {
    this->init();
}

void EntityManager::init() {
    this->idCounter = 1;

	cout << "\nEntity manager created!Initializing camera...";
	this->initCamera();
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

inline void EntityManager::setCameraPosition(Vector3 position) {
    this->camera.position = position;
}

void EntityManager::updateCameraTarget(Vector3 target) {
    this->camera.target = target;
}

uint32_t EntityManager::addEntity(Entity3D entity) {
    if (entity.getId() == 0) {
        uint32_t id = this->idCounter++;
        Entity3D newEntity = Entity3D(id);
        newEntity.shapes = entity.shapes;
        this->entities3d.push_back(newEntity);
        return id;
    }

    this->entities3d.push_back(entity);
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

EntityCollision EntityManager::getRayCastCollision(EntityCollisionType ect) {
    Ray ray = GetScreenToWorldRay({ SCREEN_WIDTH_2, SCREEN_HEIGHT_2 }, camera);
    RayCollision closestCollision;
    uint32_t entityId = 0;

    if (ect == ECT_TERRAIN) {
        return { GetRayCollisionBox(ray, { { -50, -0.1, -50 }, { 50, 0, 50 } }), ENTITY_TYPE_3D, 1 };
    } else {
        float distance = numeric_limits<float>::max();

        for (Entity3D e : entities3d) {
            Shape3D s = *(e.shapes.begin());
            BoundingBox box;

            if (s.type == SHAPE_3D_CUBE) {
                Vector3 p = s.shapeData.cubeData.position;
                const float width_2 = s.shapeData.cubeData.width/2;
                const float height_2 = s.shapeData.cubeData.height/2;
                const float length_2 = s.shapeData.cubeData.length/2;

                box = {
                    { p.x - width_2, p.y - length_2, p.z - height_2 },
                    { p.x + width_2, p.y + length_2, p.z + height_2 }
                };
            } else {
                continue;
            }

            RayCollision collision = GetRayCollisionBox(ray, box);

            if (collision.hit && collision.distance < distance && distance > 0) {
                distance = collision.distance;
                closestCollision = collision;
                entityId = e.getId();
            }
        }
    }

    return { closestCollision, ENTITY_TYPE_3D, entityId };
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

void EntityManager::update2DEntity(uint32_t id, Entity2D newEntity) {
    bool found = false;
    uint32_t index = 0;

    for (
        vector<Entity2D>::iterator itr = entities2d.begin();
        itr < entities2d.end();
        ++itr) {
        if ((*itr).getId() == id) {
            cout << "Entity with id: " << id << " updated!";
            (*itr) = newEntity;
            return;
        }
    }

    if (found) {
        try {
            entities2d.at(index) = newEntity;
        } catch (exception &e) {
            cout << "\n!!!! exception while updating entity: " << e.what();
        }
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
            cout << "Entity with id: " << id << " updated!";
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
        for (const Shape2D& shape : e.shapes) {
            // if (e.getId() == 7) {
            //     shape2DInfo(shape);
            // }

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
    }
}

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

// EntityManager's Global instance definition
EntityManager em;

void setupEntities() {
    cout << "Setting up entities";

    // Basic scene
    em.addEntity(Cube(VEC_ZERO, -0.01f, 100.0f, 100.0f, BROWN));

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
    const int invBoxDim = SCREEN_HEIGHT / 10;
    const int totalInvBoxes = 6;
    const int gap = 10;
    const int y = SCREEN_HEIGHT - invBoxDim - gap;
    int startX = (SCREEN_WIDTH - 6 * invBoxDim - gap * (totalInvBoxes - 1)) / 2;

    bool first = true;

    int startXArr[6];

    for (int i=0; i<totalInvBoxes; ++i, startX += invBoxDim + gap, first = false) {
        startXArr[i] = startX;

        if (first) {
            int id = em.addEntity(Rect(startX, y, invBoxDim, invBoxDim, COLOR_INV_BG));
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

    Image hammer = LoadImage("./assets/hammer_navicon.png");
    Image blueprint = LoadImage("./assets/blueprint_navicon.png");
    Image pistol = LoadImage("./assets/pistol_navicon.png");

    em.addEntity(Img(startXArr[0], y, invBoxDim, invBoxDim, hammer));
    em.addEntity(Img(startXArr[1], y, invBoxDim, invBoxDim, blueprint));
    em.addEntity(Img(startXArr[2], y, invBoxDim, invBoxDim, pistol));
}

