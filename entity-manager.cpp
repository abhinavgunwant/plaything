#include <iostream>
#include <vector>
#include <memory>

#include "raylib.h"

#include "entity-manager.hpp"
#include "shapes.hpp"

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
inline uint32_t Entity2D::getId() { return this->id; }

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

inline uint32_t Entity3D::getId() { return this->id; }

void Entity3D::addShape(Shape3D shape) { this->shapes.push_back(shape); }
void Entity3D::addShape(Shape3D &shape) { this->shapes.push_back(shape); }

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
    Entity2D e = Entity2D(id);
    e.shapes.push_back(shape);
    entities2d.push_back(e);
    return id;
}

uint32_t EntityManager::addEntity(Shape2D &shape) {
    uint32_t id = this->idCounter++;
    Entity2D e = Entity2D(id);
    e.shapes.push_back(shape);
    entities2d.push_back(e);
    return id;
}

Entity3D* EntityManager::get3DEntity(uint32_t id) {
    for (Entity3D e : entities3d) {
        if (e.getId() == id) {
            return &e;
        }
    }

    return nullptr;
}

Entity2D* EntityManager::get2DEntity(uint32_t id) {
    for (Entity2D e : entities2d) {
        if (e.getId() == id) {
            return &e;
        }
    }

    return nullptr;
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
            draw2D(shape);
        }
    }
}

void EntityManager::draw3D(const Shape3D& shape) {
    switch (shape.type) {
        case SHAPE_3D_CUBE: {
            CubeData c = shape.shapeData.cubeData;
            DrawCube(c.position, c.height, c.width, c.length, c.color);
            //cout << "\nDrawing cube";
            break;
        }
        case SHAPE_3D_LINE: {
            LineData l = shape.shapeData.lineData;
            DrawLine3D(l.startPos, l.endPos, l.color);
            break;
        }
    }
}

void EntityManager::draw2D(const Shape2D& shape) {
    switch (shape.type) {
        case SHAPE_2D_TEXT: {
            TextData t = shape.shapeData.textData;
            DrawText(t.text, shape.x, shape.y, t.fontSize, t.color);
            break;
        }
    }
}
