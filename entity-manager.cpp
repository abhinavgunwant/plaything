#include <iostream>
#include <vector>
#include <memory>

#include "raylib.h"

#include "entity-manager.hpp"
#include "shapes.hpp"

using namespace std;

// Entity declarations
Entity::Entity() { this->id = 0; }

Entity::Entity(uint32_t id) { this->id = id; }

Entity::Entity(uint32_t id, Shape3D &shape) {
    this->id = id;
    this->shapes.push_back(shape);
}

Entity::Entity(Shape3D shape) {
    this->id = 0;
    this->shapes.push_back(shape);
}

Entity::Entity(Shape3D &shape) {
    this->id = 0;
    this->shapes.push_back(shape);
}

inline uint32_t Entity::getId() { return this->id; }

void Entity::addShape(Shape3D shape) { this->shapes.push_back(shape); }
void Entity::addShape(Shape3D &shape) { this->shapes.push_back(shape); }

// EntityManager declarations
EntityManager::EntityManager() {
	cout << "\nEntity manager created!Initializing camera...";
    this->idCounter = 1;
	this->initCamera();
    cout << "\n--> Camera initialized!";
}

inline void EntityManager::setCameraPosition(Vector3 position) {
    this->camera.position = position;
}

void EntityManager::updateCameraTarget(Vector3 target) {
    this->camera.target = target;
}

void EntityManager::initCamera() {
    this->camera = { 0 };
    this->camera.position = { 10.0f, 10.0f, 10.0f };
    this->camera.target = { 0.0f, 0.0f, 0.0f };
    this->camera.up = { 0.0f, 1.0f, 0.0f };
    this->camera.fovy = 90.0f;
    this->camera.projection = CAMERA_PERSPECTIVE;
}

uint32_t EntityManager::addEntity(Entity entity) {
    if (entity.getId() == 0) {
        uint32_t id = this->idCounter++;
        Entity newEntity = Entity(id);
        newEntity.shapes = entity.shapes;
        this->entities.push_back(newEntity);
        return id;
    }

    this->entities.push_back(entity);
    return entity.getId();
}

uint32_t EntityManager::addEntity(Shape3D shape) {
    uint32_t id = this->idCounter++;
    Entity e = Entity(id);
    e.shapes.push_back(shape);
    entities.push_back(e);
    return id;
}

uint32_t EntityManager::addEntity(Shape3D &shape) {
    uint32_t id = this->idCounter++;
    Entity e = Entity(id);
    e.shapes.push_back(shape);
    entities.push_back(e);
    return id;
}

Entity* EntityManager::getEntity(uint32_t id) {
    for (Entity e : entities) {
        if (e.getId() == id) {
            return &e;
        }
    }

    return nullptr;
}

void EntityManager::drawEntities() {
    BeginMode3D(this->camera);

    for (Entity e : this->entities) {
        for (const Shape3D & shape : e.shapes) {
            draw(shape);
        }
    }

    EndMode3D();
}

void EntityManager::draw(const Shape3D& shape) {
    switch (shape.type) {
        case SHAPE_CUBE: {
            CubeData c = shape.shapeData.cubeData;
            DrawCube(c.position, c.height, c.width, c.length, c.color);
            //cout << "\nDrawing cube";
            break;
        }
        case SHAPE_LINE3D: {
            LineData l = shape.shapeData.lineData;
            DrawLine3D(l.startPos, l.endPos, l.color);
            break;
        }
    }
}