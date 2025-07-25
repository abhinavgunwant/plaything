#ifndef PLAYTHING_ENTITY_MANAGER
#define PLAYTHING_ENTITY_MANAGER

#include <vector>

#include "raylib.h"

#include "shapes.hpp"

using namespace std;

/**
	Does basic entity management.
 */
class Entity {
	private:
		uint32_t id;

	public:
		vector<Shape3D> shapes;

		Entity();
		Entity(uint32_t id);
		Entity(uint32_t id, Shape3D& shape);
		Entity(Shape3D &shape);
		Entity(Shape3D shape);

		void addShape(Shape3D &shape);
		void addShape(Shape3D shape);
		inline uint32_t getId();
};

class EntityManager {
private:
	uint32_t idCounter;
	Camera3D camera;
	vector<Entity> entities;
	void initCamera();

public:
	EntityManager();
	inline void setCameraPosition(Vector3 position);
	void updateCameraTarget(Vector3 target);
	Camera3D* getCamera() { return &this->camera; }

	uint32_t addEntity(Entity entity);
	uint32_t addEntity(Shape3D shape);
	uint32_t addEntity(Shape3D &shape);
	Entity* getEntity(uint32_t id);
	void drawEntities();
	void draw(const Shape3D &shape);
};

#endif
