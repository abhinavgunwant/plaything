#include <iostream>
#include <string>

#include "raylib.h"

#include "shapes.hpp"

using namespace std;

Shape2D Text(char* text, int x, int y) {
	return { SHAPE_2D_TEXT, (float)x, (float)y, TextData { text, 20, GRAY } };
}
Shape2D Text(char* text, int x, int y, int fontSize) {
	return { SHAPE_2D_TEXT, (float)x, (float)y, TextData { text, fontSize, GRAY } };
}

Shape2D Text(char* text, int x, int y, int fontSize, Color color) {
	return { SHAPE_2D_TEXT, (float)x, (float)y, TextData { text, fontSize, color } };
}

Shape2D Text(char* text, int x, int y, Color color) {
	return { SHAPE_2D_TEXT, (float)x, (float)y, TextData { text, 20, color } };
}

Shape3D Cube(Vector3 position, float width, float height, float length, Color color) {
	return { SHAPE_3D_CUBE, CubeData { position, width, height, length, color } };
}

Shape3D Line(Vector3 startPos, Vector3 endPos, Color color) {
	LineData ld = LineData{ startPos, endPos, color };
	Shape3DData sd;
	sd.lineData = ld;

	return { SHAPE_3D_LINE, sd };
}

Shape3D DefaultCube() {
	return { SHAPE_3D_CUBE, CubeData { VEC_ZERO, 0.0f, 0.0f, 0.0f, RED } };
}

Shape3D XAxis() {
	LineData ld = LineData { VEC_ZERO, { 50.0f, 0.0f, 0.0f }, RED };
	Shape3DData sd;
	sd.lineData = ld;

	return { SHAPE_3D_LINE, sd };
}

Shape3D YAxis() {
	LineData ld = LineData{ VEC_ZERO, { 0.0f, 50.0f, 0.0f }, GREEN };
	Shape3DData sd;
	sd.lineData = ld;

	return { SHAPE_3D_LINE, sd };
}

Shape3D ZAxis() {
	LineData ld = LineData{ VEC_ZERO, { 0.0f, 0.0f, 50.0f }, BLUE };
	Shape3DData sd;
	sd.lineData = ld;

	return { SHAPE_3D_LINE, sd };
}
