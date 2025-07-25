#ifndef PLAYTHING_SHAPES
#define PLAYTHING_SHAPES

#include <string>
#include "raylib.h"
#include "common.hpp"

enum Shape3DType {
	SHAPE_CUBE,
	SHAPE_LINE3D,
};

struct CubeData {
	Vector3 position;
	float width;
	float height;
	float length;
	Color color;
};

struct LineData {
	Vector3 startPos;
	Vector3 endPos;
	Color color;
};

union Shape3DData {
	CubeData cubeData;
	LineData lineData;
};

struct Shape3D {
	Shape3DType type;
	Shape3DData shapeData;
};

Shape3D Cube(Vector3 position, float width, float height, float length, Color color);
Shape3D Line(Vector3 startPos, Vector3 endPos, Color color);
Shape3D DefaultCube();
Shape3D XAxis();
Shape3D YAxis();
Shape3D ZAxis();

#endif
