#ifndef PLAYTHING_ENTITY_MANAGER
#define PLAYTHING_ENTITY_MANAGER

#include <vector>

#include "raylib.h"

#include "shapes.hpp"

using namespace std;

class Entity2D {
    private:
        uint32_t id;

    public:
        vector<Shape2D> shapes;

        Entity2D();
        Entity2D(uint32_t id);
        Entity2D(Shape2D shape);
        Entity2D(uint32_t id, Shape2D shape);

		void addShape(Shape2D &shape);
		void addShape(Shape2D shape);
		inline uint32_t getId();
};

/**
	Does basic entity management for 3D.
 */
class Entity3D {
	private:
		uint32_t id;

	public:
		vector<Shape3D> shapes;

		Entity3D();
		Entity3D(uint32_t id);
		Entity3D(uint32_t id, Shape3D& shape);
		Entity3D(Shape3D &shape);
		Entity3D(Shape3D shape);

		void addShape(Shape3D &shape);
		void addShape(Shape3D shape);
		inline uint32_t getId();
};

class EntityManager {
private:
	uint32_t idCounter;
	Camera3D camera;
	vector<Entity3D> entities3d;
	vector<Entity2D> entities2d;
	void initCamera();

public:
	EntityManager();
	inline void setCameraPosition(Vector3 position);
	void updateCameraTarget(Vector3 target);
	Camera3D* getCamera() { return &this->camera; }

	uint32_t addEntity(Entity3D entity);
	uint32_t addEntity(Shape3D shape);
	uint32_t addEntity(Shape3D &shape);
	uint32_t addEntity(Entity2D entity);
	uint32_t addEntity(Shape2D shape);
	uint32_t addEntity(Shape2D &shape);
	Entity3D* get3DEntity(uint32_t id);
	Entity2D* get2DEntity(uint32_t id);
	void drawEntities();
	void draw2D(const Shape2D &shape);
	void draw3D(const Shape3D &shape);
};

#endif
