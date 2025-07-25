#include <iostream>
#include <string>

#include "raylib.h"

#include "shapes.hpp"

using namespace std;

Shape3D Cube(Vector3 position, float width, float height, float length, Color color) {
	return { SHAPE_CUBE, CubeData { position, width, height, length, color } };
}

Shape3D Line(Vector3 startPos, Vector3 endPos, Color color) {
	LineData ld = LineData{ startPos, endPos, color };
	ShapeData sd;
	sd.lineData = ld;

	return { SHAPE_LINE3D, sd };
}

Shape3D DefaultCube() {
	return { SHAPE_CUBE, CubeData { VEC_ZERO, 0.0f, 0.0f, 0.0f, RED } };
}

Shape3D XAxis() {
	LineData ld = LineData { VEC_ZERO, { 50.0f, 0.0f, 0.0f }, RED };
	ShapeData sd;
	sd.lineData = ld;

	return { SHAPE_LINE3D, sd };
}

Shape3D YAxis() {
	LineData ld = LineData{ VEC_ZERO, { 0.0f, 50.0f, 0.0f }, GREEN };
	ShapeData sd;
	sd.lineData = ld;

	return { SHAPE_LINE3D, sd };
}

Shape3D ZAxis() {
	LineData ld = LineData{ VEC_ZERO, { 0.0f, 0.0f, 50.0f }, BLUE };
	ShapeData sd;
	sd.lineData = ld;

	return { SHAPE_LINE3D, sd };
}
